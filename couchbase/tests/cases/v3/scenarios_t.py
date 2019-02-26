#-*- coding:utf-8 -*-
#
# Copyright 2018, Couchbase, Inc.
# All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
from couchbase.tests.base import ConnectionTestCase

from couchbase.v3.cluster import Cluster

try:
    from abc import ABC
except:
    from abc import ABCMeta

import logging

import couchbase.exceptions
from couchbase.bucket import Bucket
from couchbase.cluster import Cluster
from couchbase.v3 import *
from couchbase.v3.mutate_in import MutateInSpecItem as MI
from couchbase.v3.collection import Collection
from couchbase.v3.bucket import Bucket


class Scenarios(ConnectionTestCase):
    # implicit val ec = ExecutionContext.Implicits.global

    # private val cluster = CouchbaseCluster.create("localhost")
    coll = None  # type: Collection

    def setUp(self):
        self.factory = Bucket
        super(Scenarios, self).setUp()

        # prepare:
        # 1) Connect to a Cluster
        self.cluster = Cluster()
        self.bucket = self.cb
        self.scope = self.bucket.scope("scope")  # type: Bucket
        # 2) Open a Collection
        self.coll = self.scope.open_collection("people")  # type: Collection

    def test_scenario_A(self):
        # 1) fetch a full document that is a json document
        doc = self.coll.get("id", GetOptions().timeout(Seconds(10)))
        # 2) Make a modification to the content
        content = doc.content.put("field", "value")
        # 3) replace the document on the server
        result = self.coll.replace(doc.id, content, ReplaceOptions().timeout(Seconds(10)), cas=doc.cas)
        result = self.coll.replace(doc.id, content, ReplaceOptions(timeout=Seconds(10), cas=doc.cas))
        result = self.coll.replace(doc.id, content, timeout=Seconds(10), cas=doc.cas)
        # Default params also supported for all methods
        doc2 = self.coll.get("id", timeout=Seconds(10))
        content2 = doc2.content.update({"value": "bar"})

        self.coll.replace(doc2.id, content2, cas=doc2.cas, timeout=Seconds(10))

        # I include type annotations and getOrError above to make things clearer,
        # but it'd be more idiomatic to write this:
        try:
            self.coll.get("cheese",spec=None, options=GetOptions(replica=True))
            self.coll.get("cheese",spec=None,replica=True)
            self.coll.get("cheese",spec=None, options= GetOptions(replica=True),replica=True)

            result = self.coll.get("id", options=GetOptions().timeout(Seconds(10)), spec=GetSpec())
            self.coll.replace(result.id,
                              result.content
                              .put("field", "value")
                              .put("foo", "bar"),
                              cas=result.cas,
                              timeout=Seconds(10))
        except:
            print("could not get doc")

    def test_scenario_B(self):
        """
          Scenario B:

        1) fetch a document fragment which is a json array with elements
        2) make modifications to the content
        3) replace the fragment in the original document

        """
        subdoc = self.coll.get("id", LookupInSpec().get("someArray"), timeout=Seconds(10))
        result = None
        if subdoc:
            arr = subdoc.content_as_array()
            arr.append("foo")

            result = self.coll.mutate("id", MutateSpec().upsert("someArray", arr),
                                      MutateInOptions().timeout(Seconds(10)))

        self.assertIsInstance(result, IMutationResult)

    def test_mutatein(self):
        self.coll.mutate_in('somekey', (
            MI.replace('some.path', xattr= '_sync'),
            MI.insert('some.other.path', xattr= '_sync', createParents=True),MutateInSpecItem.replace("fish"),MutateInSpecItem.insert("cheese"))
        )
    def test_scenario_C_clientSideDurability(self):
        """
        Scenario C:

        1) Remove a document with Durability Requirements, both variants, thinking about error handling
        """

        # Use a helper wrapper to retry our operation in the face of durability failures
        # remove is idempotent iff the app guarantees that the doc's id won't be reused (e.g. if it's a UUID).  This seems
        # a reasonable restriction.
        self.retryIdempotentRemoveClientSide(lambda replicateTo:
                                             self.coll.remove("id",
                                                              RemoveOptions().durabilityClient(replicateTo,
                                                                                               PersistTo.ONE(
                                                                                                   analytics=True)),
                                                              persist_to=PersistTo.ONE(query=True), cas=0),
                                             ReplicateTo.TWO, ReplicateTo.TWO, FiniteDuration.time() + 30)

    def retryIdempotentRemoveClientSide(self,
                                        callback,  # type: Callable[[ReplicateTo.Value],Any]
                                        replicate_to,  # type: ReplicateTo.Value
                                        original_replicate_to,  # type: ReplicateTo.Value
                                        until  # type: FiniteDuration
                                        ):
        # type: (...)->None
        """
          * Automatically retries an idempotent operation in the face of durability failures
          * TODO this is quite complex logic.  Should this be folded into the client as a per-operation retry strategy?
          * @param callback an idempotent remove operation to perform
          * @param replicate_to the current ReplicateTo setting being tried
          * @param original_replicate_to the originally requested ReplicateTo setting
          * @param until prevent the operation looping indefinitely
          """
        success=False
        while not success:
            if time.time() >= until:
                # Depending on the durability requirements, may want to also log this to an external system for human review
                # and reconciliation
                raise RuntimeError("Failed to durably write operation")

            try:
                callback(replicate_to)
            except couchbase.exceptions.KeyExistsError:
                print("Our work here is done")
                break

            except ReplicaNotConfiguredException:
                print("Not enough replicas configured, aborting")
                break

            except DocumentConcurrentlyModifiedException:
                # Just retry
                #self.retryIdempotentRemoveClientSide(callback, replicate_to, original_replicate_to, until)
                continue

            except DocumentMutationLostException:
                # Mutation lost during a hard failover.  I *think* we just retry with the original replicate_to.  If enough replicas
                # still aren't available, it will presumably raise ReplicaNotAvailableException and retry with lower.
                #self.retryIdempotentRemoveClientSide(callback, original_replicate_to, original_replicate_to, until)
                replicate_to=original_replicate_to
                continue

            except ReplicaNotAvailableException:
                newReplicateTo = {ReplicateTo.ONE: ReplicateTo.NONE,
                                  ReplicateTo.TWO: ReplicateTo.ONE,
                                  ReplicateTo.THREE: ReplicateTo.TWO}.get(replicate_to, ReplicateTo.NONE)
                print("Temporary replica failure, retrying with lower durability {}".format(newReplicateTo))
                replicate_to=newReplicateTo

    def scenarioC_serverSideDurability(self):
        # Use a helper wrapper to retry our operation in the face of durability failures
        # remove is idempotent iff the app guarantees that the doc's id won't be reused (e.g. if it's a UUID).  This seems
        # a reasonable restriction.
        self.retryIdempotentRemoveServerSide(
            lambda: self.coll.remove("id", RemoveOptions().durabilityServer(Durability.MajorityAndPersistActive),
                                     cas=0))
        
    def retryIdempotentRemoveServerSide(self,
                                        callback,  # type: Callable[None],
                                        until  # type: FiniteDuration
                                        ):
        """
          * Automatically retries an idempotent operation in the face of durability failures
          * TODO Should this be folded into the client as a per-operation retry strategy?
          * @param callback an idempotent remove operation to perform
          * @param until prevent the operation looping indefinitely
          */"""
        while FiniteDuration.time() < until:

            try:
                callback()
                return
                # Not entirely clear what failures need to be handled yet, but will be considerably easier than observe() based
                # logic.  I think the only case to handle is:
            except DurabilityAmbiguous:
                # A guarantee is that the mutation is either written to a majority of nodes, or none.  But we don't know which.
                if self.coll.get("id").success():
                    continue
                logging.info("Our work here is done")
                return

            except couchbase.exceptions.NotFoundError:
                logging.info("Our work here is done")
                return
        # Depending on the durability requirements, may want to also log this to an external system for human review
        # and reconciliation
        raise RuntimeError("Failed to durably write operation")

    def test_scenario_D(self):
        """  Scenario D (variation of A):

        #1) do the same thing as A, but handle the "cas mismatch retry loop"
        """

        def respond():
            result = self.coll.get("id", timeout=Seconds(10))
            if result:
                 self.coll.replace(result.id,
                                   result.content
                                   .put("field", "value")
                                   .put("foo", "bar"),
                                   result.cas,
                                   timeout=Seconds(10))
            else:
                logging.error("could not get doc")

        self.retryOperationOnCASMismatch(respond, guard=50)

    def retryOperationOnCASMismatch(self,
                                    callback, # type: Callable[[],None]
                                    guard # type: int
                                    ):
        # type: (...) -> Nones
        if guard <= 0:
            raise RuntimeError("Failed to perform exception")

        try:
            callback()
        except CASMismatchException:
            self.retryOperationOnCASMismatch(callback, guard - 1)

    def test_scenario_E(self):
        """
              Scenario E (if applicable):

        1) Fetch a full Document and marshal it into a language entity rather than a generic json type
        2) Modify the entity
        3) store it back on the server with a replace
        """

        class User:
            def __init__(self,
                         name,  # type: str
                         age,  # type: int
                         address,  # type: str
                         phoneNumber  # type: str
                         ):
                self.name = name
                self.age = age
                self.address = address
                self.phoneNumber = phoneNumber
            def __copy__(self, **kwargs):
                result=copy.deepcopy(self)
                for k,v in kwargs:
                    setattr(result, k,v)
                return result

        result = self.coll.get("id", timeout=Seconds(10))
        if result:
            self.coll.replace(result.id, result.contentAs[User].copy(age=25), result.cas, timeout=Seconds(10))
        else:
            logging.error("could not get doc")

    # {
    #   user: {
    #     name = "bob",
    #     age = 23,
    #     address = "123 Fake Street"
    #   }
    # }

    def test_scenario_F_fulldoc(self):
        """
              Scenario F (if applicable):
        1) Fetch a Document fragment and marshal it into a language entity rather than a generic json type
        2) Modify the entity
        3) store it back on the server with a replace
        """

        class User:

            def __init__(self,
                         name,  # type: str
                         age,  # type: int
                         address  # type: str
                         ):
                self.name = name
                self.age = age
                self.address = address

        doc = self.coll.get("id")

        if doc:
            result=doc.contentAs[User].copy(age=25)
        else:
            logging.error("could not find doc")

    def test_scenarioF_subdoc(self):
        class UserPartial:
            def __init__(self,
                         name,  # type: str
                         age  # type: int
                         ):
                self.name = name
                self.age = age

        subdoc = self.coll.get("id", ReadSpec().get(("user.name", "user.age")))

        user = subdoc.contentAs[UserPartial]
        changed = user.copy(age=25)

        # Note: I have misgivings over whether this update-with-a-projection should be allowed
        # mergeUpsert will upsert fields user.name & user.age, leaving user.address alone
        self.coll.mutate(subdoc.id, MutateSpec().upsert("user", changed))


from typing import TypeVar, Generic

PlayerTV = TypeVar('PlayerTV', bound='BasePlayer')
TeamTV = TypeVar('TeamTV', bound='BaseTeam')


class BaseTeam(Generic[PlayerTV, TeamTV]):
    def t(self):
        # type: (...) -> TeamTV
        pass

    def p(self):
        # type: (...) -> PlayerTV
        pass


class BasePlayer(Generic[PlayerTV, TeamTV]):
    def p(self):
        # type: (...) -> PlayerTV
        pass


class Player(BasePlayer['Player', 'Team']):
    real_attr = 1


class Team(BaseTeam[Player, 'Team']):
    real_attr = 1

    def m1(self):
        # type: (...) -> None
        a=self.t().real_attr
        a=self.t().fake_attr

    def m2(self):
        a=self.p().real_attr
        a=self.p().fake_attr


def f1(t # type: Team
       ):
    a=t.t().real_attr
    a=t.t().fake_attr


