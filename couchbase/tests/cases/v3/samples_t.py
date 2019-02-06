#-*- coding:utf-8 -*-
#
# Copyright 2018, Couchbase, Inc.
# All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from time import sleep

from nose.plugins.attrib import attr

from couchbase import FMT_JSON, FMT_PICKLE, FMT_BYTES, FMT_UTF8
from couchbase.exceptions import (KeyExistsError, ValueFormatError,
                                  ArgumentError, NotFoundError,
                                  NotStoredError)
from couchbase.tests.base import ConnectionTestCase
import json

"""

import com.couchbase.client.core.Core
import com.couchbase.client.core.env.CoreEnvironment
import com.couchbase.client.scala.api._
import com.couchbase.client.scala.CouchbaseCluster
import com.couchbase.client.scala.document.{Document, JsonObject}
import com.couchbase.client.scala.query.{N1qlQueryResult, N1qlResult}

import scala.concurrent.{Await, ExecutionContext}
import scala.concurrent.duration._
import scala.util.{Failure, Success}
"""
import couchbase.v3
class Samples(ConnectionTestCase):
    coll = None  # type: couchbase.v3.Collection
    def setUp(self, **kwargs):
        self.cluster=couchbase.v3.Cluster("{}://{}/{}".format(self.cluster_info.protocol, self.cluster_info.host, "default"))
        self.bucket=self.cluster.bucket("default")

        self.scope=self.bucket.Scope("fred")
        self.coll = self.scope.collection("john")
        self.coll.get()
    def test_blockingapi(self):
        # cluster = CouchbaseCluster.create("localhost")
        bucket = self.bucket
        # cluster.openBucket("default")
        import couchbase
        x=couchbase.FMT_BYTES
        scope = bucket.Scope("scope")
        coll = scope.collection("people")
        coll2 = bucket.collection("scope", "people")
        # Also supported: val coll = bucket.openCollection("scope", "people")
        # As the methods below block on a Scala Future, they need an implicit ExecutionContext in scope
        self.do_the_stuff()

    def do_the_stuff(self):

         # All methods have both a named/default parameters version, and an [X]Options version
        fetched1= self.coll.get("id")
        fetched3 = self.coll.get("id", timeout = couchbase.v3.Seconds(1))
        fetched5 = self.coll.get("id", couchbase.v3.GetOptions().timeout(couchbase.v3.Seconds(1)))
        JsonObject = fetched1.content

        import collections
        from typing import NamedTuple

        MyUserEntity=NamedTuple("MyUserEntity",[('id',str),('firstname',str),('age',int)])
        fetched1.contentAs[MyUserEntity]("users[0]")
#         getResult.users(0).getAs[User]
#         getResult.users.getAs[List[User]]
#
#         case class MyUsersEntity(users: List[User])
#         val users = getResult.contentAs[MyUsersEntity]
#
#         getResult.some.field.getString
#
#
#         // getOrError is a convenience method that either returns JsonDocument (no Option) or throws DocumentNotFoundException
#         // TODO May remove these
#         val fetched2 = coll.getOrError("id")
#         val fetched7 = coll.getOrError("id", GetOptions().timeout(1000.milliseconds))
#
#
#         // getAndLock and getAndTouch work pretty much the same as get
#         val fetched4 = coll.getAndLock("id", 5.seconds)
#
#
#         // Simple subdoc lookup
#         val result: LookupInResult = coll.lookupIn("id", LookupSpec().get("field1", "field2"))
#         coll.lookupIn("id", LookupSpec().get("field1", "field2"), timeout = 10.seconds)
#         coll.lookupIn("id", LookupSpec().get("field1", "field2"), LookupInOptions().timeout(10.seconds))
#
#
#         // Parsing subdoc results.  LookupInResult is similar to a Document.
#         println(result.content(0).asInstanceOf[String])
#         println(result.content("field1").asInstanceOf[String])
#         println(result.contentAs[String]("field1"))
#         println(result.field1.asInstanceOf[String])
#         result match {
#           case LookupInResult(field1, field2) =>
#             println(field1.asInstanceOf[String])
#             println(field2.asInstanceOf[Int])
#         }
#         case class MyProjection(field1: String, field2: Int)
#         val proj = result.contentAs[MyProjection]
#
#
#         // JsonObject works pretty much as in SDK2, though it's now immutable
#         val age: Option[Any] = fetched5.get.content.get("age")
#         val age2: Option[Int] = fetched5.get.content.getInt("age")
#         // And Scala's Dynamic feature lets us do some cool stuff:
#         val age3: Int = fetched5.get.content.age.getInt
#         // JsonObject is immutable so each of these puts creates a new copy
#
#
#         // Various ways of inserting
#         val inserted: MutationResult = coll.insert("id", JsonObject.create)
#         coll.insert("id", JsonObject.create, timeout = 1000.milliseconds, expiration = 10.days, replicateTo = ReplicateTo.ALL, persistTo = PersistTo.MAJORITY)
#         coll.insert("id", JsonObject.create, timeout = 1000.milliseconds, persistTo = PersistTo.MAJORITY)
#         coll.insert("id", JsonObject.create, InsertOptions().timeout(1000.milliseconds).persistTo(PersistTo.MAJORITY))
#         case class User(name: String, age: Int)
#         coll.insert("john", User("John", 25), timeout = 5.seconds)
#
#
#         // Various ways of replacing
#         if (fetched1.isDefined) {
#           // JsonDocument will be an immutable Scala case class and it's trivial to copy it with different content:
#           // val toReplace = fetched1.get.copy(content = JsonObject.empty())
#           val toReplace = fetched1.get
#           val replaced: MutationResult = coll.replace(toReplace.id, JsonObject.create, toReplace.cas)
#           coll.replace(toReplace.id, JsonObject.create, toReplace.cas, timeout = 1000.milliseconds, persistTo = PersistTo.MAJORITY)
#           coll.replace(toReplace.id, JsonObject.create, toReplace.cas, ReplaceOptions().timeout(1000.milliseconds).persistTo(PersistTo.MAJORITY))
#           coll.replace(toReplace.id, User("John", 25), toReplace.cas, timeout = 5.seconds)
#         }
#
#
#         // Subdoc mutations
#         val mutateResult: MutationResult = coll.mutateIn("id", MutateInSpec().insert("hello", "world").upsert("foo", "bar"))
#         coll.mutateIn("id", MutateInSpec().insert("hello", "world"), MutateInOptions().timeout(10.seconds))
#         coll.mutateIn("id", MutateInSpec().insert("hello", "world"), cas = 42, timeout = 10.seconds)
#
#
         # Queries
         queryResult = self.cluster.query("select * from `beer-sample`")

         self.cluster.query("select * from `beer-sample` where beer = $name",
           QueryOptions().namedParameter("name", "Speckled Hen"))

         cluster.query("select * from `beer-sample` where beer = ? and producer = ?",
           QueryOptions().positionalParameters("Budweiser", "Anheuser-Busch")
             //        .scanConsistency(AtPlus(consistentWith = List(inserted.mutationToken())))
             .timeout(5.seconds))

         cluster.query("select * from `beer-sample`",
           QueryOptions().scanConsistency(StatementPlus())
             .serverSideTimeout(10.seconds))

#         bucket.query(s"select * from {} where beer = 'Speckled Hen'")
#
#         case class BeerProjection(name: String, producer: String)
#
#         val beers1: N1qlResult[BeerProjection] = cluster.queryAs[BeerProjection]("select name, producer from `beer-sample`")
#         val beers2: N1qlResult[BeerProjection] = cluster.queryAs[BeerProjection]("select {} from {}")
#       }
#
#
#     // There are two asynchronous APIs.  This one returns Scala Futures (similar to a Java CompletableFuture).  The API
#     // is basically identical to the synchhronous one above, just returning a Future.  Most of this code is just giving
#     // basic usage for Scala Futures.
#     def asyncApi(): Unit = {
#
#       // When using Scala Futures you tell them how to run (thread-pools etc.) with an ExecutionContext (similar to a
#       // Java executor), normally provided as an implicit argument (e.g. all Futures below will automatically use this
#       // variable, as it's in-scope, marked implicit, and has the correct type).  This basic one is a simple thread-pool.
#       implicit val ec = ExecutionContext.Implicits.global
#
#       val cluster = CouchbaseCluster.create("localhost")
#       val bucket = cluster.openBucket("default")
#       val scope = bucket.openScope("scope")
#       val coll = scope.openCollection("people").async()
#
#       // Gets return Future[Option[JsonDocument]].  A basic way to handle a Future's result is this:
#       coll.get("id", timeout = 1000.milliseconds) onComplete {
#         case Success(doc) =>
#           // doc is an Option[JsonDocument]
#           if (doc.isDefined) println("Got a doc!")
#           else println("No doc :(")
#
#         case Failure(err) =>
#           // err is a RuntimeException
#           println("Error! " + err)
#       }
#
#       // Or block on it (discouraged)
#       val getFuture = coll.get("id")
#       val doc = Await.result(getFuture, atMost = 5.seconds)
#
#       // Futures are powerful and support things like map and filter.  Many of the operations supported by Project Reactor
#       // are possible with Futures (though they're missing backpressure and many of Reactor's more advanced operators)
#       // Get-and-replace
#       val replace = coll.get("id", timeout = 1000.milliseconds)
#         .map(doc => {
#           coll.replace(doc.id, JsonObject.empty, doc.cas, timeout = 1000.milliseconds, persistTo = PersistTo.MAJORITY)
#         })
#
#       Await.result(replace, atMost = 5.seconds)
#
#       // Another, maybe tidier way of writing that get-replace
#       val replace2 = for {
#         doc <- coll.get("id", timeout = 1000.milliseconds)
#         doc <- {
#           // coll.replace(doc.copy(content = JsonObject.empty()))
#           coll.replace(doc.id, JsonObject.create, doc.cas)
#         }
#       } yield doc
#
#       Await.result(replace, atMost = 5.seconds)
#
#       // Insert
#       coll.insert("id", JsonObject.create, timeout = 1000.milliseconds, persistTo = PersistTo.MAJORITY) onComplete {
#         case Success(doc) =>
#         case Failure(err) =>
#       }
#
#     }
#
#
#     // Finally, this API wraps the reactive library Project Reactor
#     // The API is basically identical to the blocking one except returning Reactor Mono's.  Most of this code is showing
#     // normal Reactor usage.
#     // Disabled for now to keep up with rapid prototyping, but it'll look something like this
#     //  def reactiveAPI(): Unit = {
#     //    val cluster = CouchbaseCluster.create("localhost")
#     //    val bucket = cluster.openBucket("default")
#     //    val scope = new Scope(cluster, bucket, "scope")
#     //    val coll = scope.openCollection("people").reactive()
#     //
#     //    // As the methods below wrap a Scala Future, they need an implicit ExecutionContext in scope
#     //    implicit val ec = ExecutionContext.Implicits.global
#     //
#     //    // Get
#     //    coll.get("id", timeout = 1000.milliseconds)
#     //      .map(doc => {
#     //        if (doc.isDefined) println("Got doc")
#     //        else println("No doc :(")
#     //      })
#     //      // As normal with Reactive, blocking is discouraged - just for demoing
#     //      .block()
#     //
#     //    // Get-replace
#     //    coll.getOrError("id", timeout = 1000.milliseconds)
#     //      .flatMap(doc => {
#     //        // val newDoc = doc.copy(content = JsonObject.empty())
#     //        val newDoc = doc
#     //        coll.replace(newDoc)
#     //      })
#     //      // As normal with Reactive, blocking is discouraged - just for demoing
#     //      .block()
#     //  }
# }"""
#
#     def test_trivial_set(self):
#         rv = self.cb.upsert(self.gen_key(), 'value1')
#         self.assertTrue(rv)
#         self.assertTrue(rv.cas > 0)
#         rv = self.cb.upsert(self.gen_key(), 'value2')
#         self.assertTrue(rv.cas > 0)
#
#     def test_utf8_set(self):
#         rv = self.cb.upsert("documentID", "<xml></xml>", format=FMT_UTF8)
#         self.assertTrue(rv)
#         self.assertTrue(rv.cas > 0)
#         rv = self.cb.get("documentID")
#         self.assertTrue(rv.cas > 0)
#         self.assertEqual(rv.value, "<xml></xml>")
#
#     def test_utf8_set_nonascii(self):
#
#         rv = self.cb.upsert("documentID",  u'Öüç', format=FMT_UTF8)
#         self.assertTrue(rv)
#         self.assertTrue(rv.cas > 0)
#         rv = self.cb.get("documentID")
#         self.assertTrue(rv.cas > 0)
#         self.assertEqual(rv.value,  u'Öüç')
#
#     def test_set_with_cas(self):
#         key = self.gen_key('cas')
#         rv1 = self.cb.upsert(key, 'value1')
#         self.assertTrue(rv1.cas > 0)
#
#         self.assertRaises(KeyExistsError, self.cb.upsert,
#                           key, 'value2', cas=rv1.cas+1)
#
#         rv2 = self.cb.upsert(key, 'value3', cas=rv1.cas)
#         self.assertTrue(rv2.cas > 0)
#         self.assertNotEqual(rv1.cas, rv2.cas)
#
#         rv3 = self.cb.upsert(key, 'value4')
#         self.assertTrue(rv3.cas > 0)
#         self.assertNotEqual(rv3.cas, rv2.cas)
#         self.assertNotEqual(rv3.cas, rv1.cas)
#
#     @attr('slow')
#     def test_set_with_ttl(self):
#         key = self.gen_key('ttl')
#         self.cb.upsert(key, 'value_ttl', ttl=2)
#         rv = self.cb.get(key)
#         self.assertEqual(rv.value, 'value_ttl')
#         # Make sure the key expires
#         sleep(3)
#         self.assertRaises(NotFoundError, self.cb.get, key)
#
#     def test_set_objects(self):
#         key = self.gen_key('set_objects')
#         for v in (None, False, True):
#             for fmt in (FMT_JSON, FMT_PICKLE):
#                 rv = self.cb.upsert(key, v, format=fmt)
#                 self.assertTrue(rv.success)
#                 rv = self.cb.get(key)
#                 self.assertTrue(rv.success)
#                 self.assertEqual(rv.value, v)
#
#     def test_multi_set(self):
#         kv = self.gen_kv_dict(prefix='set_multi')
#         rvs = self.cb.upsert_multi(kv)
#         self.assertTrue(rvs.all_ok)
#         for k, v in rvs.items():
#             self.assertTrue(v.success)
#             self.assertTrue(v.cas > 0)
#
#         for k, v in rvs.items():
#             self.assertTrue(k in rvs)
#             self.assertTrue(rvs[k].success)
#
#         self.assertRaises((ArgumentError,TypeError), self.cb.upsert_multi, kv,
#                           cas = 123)
#
#     def test_add(self):
#         key = self.gen_key('add')
#         self.cb.remove(key, quiet=True)
#         rv = self.cb.insert(key, "value")
#         self.assertTrue(rv.cas)
#
#         self.assertRaises(KeyExistsError,
#                           self.cb.insert, key, "value")
#
#     def test_replace(self):
#         key = self.gen_key('replace')
#         rv = self.cb.upsert(key, "value")
#         self.assertTrue(rv.success)
#
#         rv = self.cb.replace(key, "value")
#         self.assertTrue(rv.cas)
#
#         rv = self.cb.replace(key, "value", cas=rv.cas)
#         self.assertTrue(rv.cas)
#
#         self.assertRaises(KeyExistsError,
#                           self.cb.replace, key, "value", cas=0xdeadbeef)
#
#         self.cb.remove(key, quiet=True)
#         self.assertRaises(NotFoundError,
#                           self.cb.replace, key, "value")
#
#     def test_from_json_string(self):
#         JSONstr = '{"key1": "value1", "key2": 123}'
#         JSONobj = json.loads(JSONstr)
#         self.cb.upsert("documentID", JSONobj, format=FMT_JSON)
#
#
# if __name__ == '__main__':
#     unittest.main()
