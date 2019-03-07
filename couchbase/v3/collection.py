from typing import overload

from couchbase import subdocument as SD
from couchbase.v3 import OptionBlock, forward_args, GetResult, PersistTo, ReplicateTo, forwarder, mutation_forwarder
from couchbase.v3.bucket import Bucket
from typing import *
import couchbase.bucket

class CollectionOptions(OptionBlock):
    pass


class Collection(object):
    def __init__(self,
                 parent,  # type: Bucket
                 name = None,  # type: str
                 options = None  # type: CollectionOptions
                 ):
        # type: (...)->None
        self._bucket = parent._bucket  # type: couchbase.bucket.Bucket
        self.name = name

    def __getattr__(self, item):
        return getattr(self.bucket, item)

    @property
    def bucket(self):
        # type: (...)->couchbase.bucket.Bucket
        return self._bucket

    def do_with_bucket(self,
                       verb  # type: Callable[[Bucket],Any]
                       ):
        return verb(self.bucket)

    @overload
    def get(self,
            key,  # type:str
            spec=None,  # type: GetSpec
            options=None,  # type: GetOptions
            ):
        # type: (...) -> GetResult
        pass

    @overload
    def get(self,
            key,  # type:str
            spec=None,  # type: GetSpec
            timeout=None,  # type: Seconds
            quiet=None,  # type: bool
            replica=False,  # type: bool
            no_format=False  # type: bool
            ):
        # type: (...) -> GetResult
        pass

    def get(self,
            key,  # type: str
            spec=None,  # type: GetSpec
            **kwargs
            ):
        # type: (...) -> GetResult
        options = forward_args(locals(), kwargs)
        project=options.pop('project',None)
        if project:
            if len(project) < 17:
                spec = map(SD.get,project)
        bc=self.bucket  # type: couchbase.bucket.Bucket
        if not spec:
            x = bc.get(key, **options)
        else:
            x = bc.lookup_in(key, *spec, **options)
        return GetResult(x.value, cas=x.cas, expiry=options.pop('timeout',None), id=x.key)

    def get_and_touch(self,
                  id,  # type: str
                  expiration,  # type: int
                  options=None  # type: GetAndTouchOptions
                  ):
        # type: (...)->IGetResult
        pass

    def get_and_lock(self,
                     id,  # type: str
                     expiration,  # type: int
                     options=None  # type: GetAndLockOptions
                     ):
        # type: (...)->IGetResult
        cb = self.bucket  # type: SDK2Bucket
        cb.get(id, expiration, False, False, False)
        cb.lock(id)

    def get_from_replica(self,
                         id,  # type: str
                         replicaMode,  # type: ReplicaMode
                         options=None  # type: GetFromReplicaOptions
                         ):
        # type: (...)->IGetResult
        self.bucket.get()

    def touch(self,
              id,  # type: str
              *options  # type: TouchOptions
              ):
        # type: (...)->IMutationResult
        pass

    def unlock(self,
               id,  # type: str
               *options  # type: UnlockOptions
               ):
        # type: (...)->IMutationResult
        pass

    def exists(self, id,  # type: str,
               options = None,  # type: ExistsOptions
               ):
        # type: (...)->IExistsResult
        pass

    def upsert(self,
               id,  # type: str
               value,  # type: Any
               *options  # type: UpsertOptions
               ):
        # type: (...)->IMutationResult
        pass

    def insert(self, id,  # type: str
               value,  # type: Any
               *options  # type: InsertOptions
               ):
        # type: (...)->IMutationResult
        pass

    # old school named params

    @overload
    def replace(self,
                id,  # type: str
                value,  # type: Any
                options=None,  # type: ReplaceOptions
                cas=0,  # type: int
                timeout=None,  # type: Seconds
                format=None,  # type: bool
                persist_to=0,  # type: int
                replicate_to=0,  # type: int
                ):
        # type: (...)->IMutationResult
        return self.bucket.replace(id, value, **forward_args(locals(), options))

    @overload
    def replace(self,
                id,  # type: str
                value,  # type: Any
                options,  # type: ReplaceOptions
                *args, **kwargs
                ):
        # type: (...)->IMutationResult
        return self.bucket.replace(id, value, **forward_args(locals(), options))

    def replace(self,
                id,  # type: str
                value,  # type: Any
                *args, **kwargs
                ):
        # type: (...)->IMutationResult
        pass

    def remove(self,  # type: Collection
               id,  # type: str
               options=None,  # type: RemoveOptions
               cas=0,  # type: int
               persist_to=PersistTo.NONE  # type: PersistTo.Value
               ):
        # type: (...)->IMutationResult
        return self.bucket.remove(id, **forward_args(locals(), options))

    def lookup_in(self,
                 id,  # type: str,
                 spec,  # type: LookupInSpec
                 options = None  # type: LookupInOptions
                ):
        # type: (...)->ILookupInResult
        pass

    def mutate_in(self, id,  # type: str,
                 spec,  # type: MutateInSpec
                 options = None  # type: MutateInOptions
                ):
        # type: (...)->IMutateInResult
        pass

    def binary(self):
        # type: (...)->IBinaryCollection
        pass

    @overload
    def append(self,
               id,  # type: str
               value,  # type: str
               options=None,  # type: AppendOptions
               ):
        # type: (...)->IMutationResult
        pass

    @overload
    def append(self,
               id,  # type: str
               value,  # type: str
               cas=0,  # type: int
               format=None,  # type: long
               persist_to=PersistTo.NONE,  # type: PersistTo.Value
               replicate_to=ReplicateTo.NONE  # type: ReplicateTo.Value
               ):
        pass

    @forwarder
    def append(self,
               id,  # type: str
               value,  # type: str
               options=None,  # type: AppendOptions
               **kwargs
               ):
        # type: (...)->IMutationResult
        forwarder = mutation_forwarder(self.bucket.append, locals(), kwargs)
        return forwarder(id, None)

    @overload
    def prepend(self,
                id,  # type: str
                value,  # type: Any,
                cas=0,  # type: int
                format=None,  # type: int
                persist_to=PersistTo.NONE,  # type: PersistTo.Value
                replicate_to=ReplicateTo.NONE  # type: ReplicateTo.Value
                ):
        # type: (...)->IMutationResult
        pass

    @overload
    def prepend(self,
                id,  # type: str
                value,  # type: str
                *options  # type: PrependOptions
                ):
        # type: (...)->IMutationResult
        pass

    def prepend(self,
                id,  # type: str
                value,  # type: str
                *args,  # type: PrependOptions
                **kwargs  # type: Any
                ):
        # type: (...)->IMutationResult
        return mutation_forwarder(self.bucket.prepend, locals(), kwargs)(id, None)

    def increment(self,
                  id,  # type: str
                  *options  # type: IncrementOptions
                  ):
        # type: (...)->IMutationResult
        pass

    def decrement(self,
                  id,  # type: str
                  *options  # type: DecrementOptions
                  ):
        # type: (...)->IMutationResult
        pass

    def mutate(self,  # type: Collection
               id,  # type: str
               spec,  # type: MutateSpec
               options=None  # type: MutateOptions
               ):
        # type: (...)->    IMutateSubResult
        return self.bucket.mutate_in(id, spec, **forward_args(locals(), options))