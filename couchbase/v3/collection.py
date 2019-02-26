import copy
import pyrsistent
from typing import overload

from couchbase import subdocument as SD
from couchbase.v3 import OptionBlock, SDK2MutationToken
from couchbase.v3.result import GetResult, MutationResult
from couchbase.v3.durability import ReplicateTo, PersistTo
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
        pass

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


class ScopedType(object):
    def __init__(self, name=None):
        if name:
            self.name = name
        self.record = pyrsistent.PRecord()

    def scope(self, name):
        result = copy.deepcopy(self)
        result.name = name
        return result

    def default_scope(self):
        result = copy.deepcopy(self)
        return result

    def __deepcopy__(self, memodict={}):
        result = copy.copy(self)
        return result


class Scope(type):
    name = None  # type: str

    def __init__(cls, name, bases, dct):
        super(type, cls).__init__()  # name,bases,dct)
        import inspect
        for name, item in inspect.getmembers(cls,
                                             lambda x: inspect.isdatadescriptor(x) or inspect.isgetsetdescriptor(x)):
            def proxy_get(self):
                return self.record.get(name)

            def proxy_set(self, value):
                return self.record.set(name, value)

            proxy_property = property(fget=copy.deepcopy(proxy_get), fset=copy.deepcopy(proxy_set))
            setattr(cls, name, proxy_property)

        cls.__init__ = lambda self, name, *args, **kwargs: Scope.__start__(cls, self, name, *args, **kwargs)

    @classmethod
    def scope(mcs, self, name):
        # type: (Any, str)->mcs
        result = copy.deepcopy(self)
        result.name = name
        return result

    def name(cls):
        # type (...)->str
        """

        :return:    A string value that is the name of the collection.
        :except     ScopeNotFoundException
        :except     AuthorizationException
        """
        pass
    def collection(cls,
                   collection_name,  # type: str
                   options  # type: CollectionOptions
                   ):
        """

        :param collection_name:
        :param options:
        :return:
        """
        """
        Gets an ICollection instance given a collection name.
    
    Response
    A ICollection implementation for a collection with a given name.
    Throws
    Any exceptions raised by the underlying platform
    CollectionNotFoundException
    AuthorizationException

        :param collection_name: string identifier for a given collection.
        :param options: collection options
        :return:
        """
        pass


def forward_args(arg_vars, *options):
    # type: (Dict[str,Any],Tuple[OptionBlock,...])->OptionDerivative[str,Any]
    end_options = options[0] if options and options[0] else OptionBlock()
    kwargs=arg_vars.pop('kwargs',{})
    end_options.update(kwargs)
    end_options.update(dict((k.replace("timeout", "ttl"), v) for k, v in
                            arg_vars.items() if k != "self"))

    return end_options


def mutation_result(func):
    def mutated(*args,**kwargs):
        result=func(*args,**kwargs)
        return MutationResult(result.cas, SDK2MutationToken(result.mutinfo))
    return mutated


def forwarder(func,  # type: Callable[[M1],Any]
              arg_vars,  # type: Dict[str,Any]
              options    # type: Dict[str,Any]
              ):
    def forwarded(*args,**kwargs):
        kwargs.update(forward_args(arg_vars, options))
        return func(*args, **kwargs)
    return forwarded


def mutation_forwarder(func,  # type: Callable[[M1],Any]
                       arg_vars,  # type: Dict[str,Any]
                       options,  # type: Dict[str,Any]
                       ):
    # type: (...)->Callable[M1,IMutationResult]
    return mutation_result(forwarder(func, arg_vars, options))