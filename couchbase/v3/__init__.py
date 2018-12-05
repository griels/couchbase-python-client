import abc
import time
from abc import abstractmethod, ABCMeta, abstractproperty
from time import sleep

from couchbase import JSON
from couchbase import FMT_JSON, FMT_PICKLE, FMT_BYTES, FMT_UTF8
from couchbase.exceptions import (KeyExistsError, ValueFormatError,
                                  ArgumentError, NotFoundError,
                                  NotStoredError, CouchbaseError)
from couchbase.tests.base import ConnectionTestCase
import pyrsistent
from couchbase.bucket import Bucket as SDK2Bucket
import copy
import six
from typing import *
from couchbase.result import ValueResult
from couchbase import JSON
from couchbase import subdocument as SD
from couchbase.cluster import Cluster as SDK2Cluster
import couchbase.exceptions
import couchbase.result
SDType = type(SD)
Document = ValueResult
class KeyValueException(CouchbaseError):
    """
    A server error occurred while executing a K/V operation. Assumes that the service has returned a response.
    Message
    The XError message returned by the memcached server
    Properties
    The memcached response status
    XError and Enhanced error message information
    The document id
    The opaque used in the request"""
    pass

"""
Derived Exceptions
Expected to be handled specifically by the application to perform an additional action such as retrying to check if the key has become unlocked.
"""

class KeyNotFoundException(KeyValueException):
    pass
class KeyExistsException(KeyValueException):
    pass
class ValueTooBigException(KeyValueException):
    pass
class KeyLockedException(KeyValueException):
    pass
class TempFailException(KeyValueException):
    pass
class PathNotFoundException(KeyValueException):
    pass
class PathExistsException(KeyValueException):
    pass
class InvalidRangeException(KeyValueException):
    pass
class KeyDeletedException(KeyValueException):
    pass
class QueryException(KeyValueException):
    """
    A server error occurred while executing a N1QL query. Assumes that that the service has returned a response.
    Message
    The error message returned by the Query service
    Properties
    The error(s) returned by response from the server by the Query/N1QL service
    Any additional information returned by the server, the node it executed on, payload, HTTP status
    """
    pass
"""
Derived Exceptions
TBD? May be nothing to extend...
"""
class SearchException(CouchbaseError):
    pass

    """Message
    The error message returned by the Search service
    Properties
    The error(s) returned by response from the server by the FTS Service
    Any additional information returned by the server, the node it executed on, payload, HTTP status
    """

"""Derived Exceptions
TBD? May be nothing to extend...
"""
class AnalyticsException(CouchbaseError):
    pass
    """A server error occurred while executing an Analytics query. Assumes that that the service has returned a response
    Message
    The error message returned by the Analytics service
    Properties
    The error(s) returned by response from the server, contextId, any additional information returned by the server, the node it executed on, payload, HTTP status.
    """
"""
Derived Exceptions
TBD? May be nothing to extend...
"""
class ViewException(CouchbaseError):
    """A server error occurred while executing a View query.  Assumes that that the service has returned a response.
    Message
    The error message returned by the View service
    Properties
    The error(s) returned by response from the server, contextId, any additional information returned by the server, the node it executed on, payload, HTTP status.
    """
    pass

class PartialViewResultException(ViewException):
    #? (returns rows that it did get)
    pass

class InvalidConfigurationException(CouchbaseError):
    """An invalid configuration was supplied to the client.
    Message
    "A configuration error has occurred." details and inner exceptions, any stacktrace info.
    Properties
    TBD
    """
    pass
class BootstrappingException(CouchbaseError):
    """The client cannot initiate or fails while performing the bootstrap process.
    Message
    "A bootstrapping error has occurred." details and inner exceptions, any stacktrace info.
    Properties
    TBD"""
    pass
class ServiceNotFoundException(CouchbaseError):
    """The client requests or queries a service that is not enabled or available in the cluster.
    Message
    "The service requested is not enabled or cannot be found on the node requested.." details and inner exceptions, any stacktrace info.
    Properties
    TBD"""

class TimeoutException(CouchbaseError):
    """---
    Message
    Properties
    Reason: (Exception) Explains the underlying reason we expect this was caused.
    """
    pass
class NetworkException(CouchbaseError):
    """A network error happens while executing a query or performing the K/V operation; the query or operation does not succeed with a valid service response.
    Message
    Properties
    Bubble up any platform idiomatic information."""
    pass
class NodeUnavailableException(CouchbaseError):
    """The client attempts to use a node which is either offline or cannot fulfill a request.
    Message
    "The node that the operation has been requested on is down or not available". details and inner exceptions, any stacktrace info.
    Properties
    TBD"""
    pass
class BucketMissingException(CouchbaseError):
    """The application attempts to open or use a bucket which does exist or is not available at that time.
    Message
    "The requested bucket '{bucketname}' cannot be found."
    Properties
    TBD"""
    pass
class CollectionMissingException(CouchbaseError):
    """The application attempts to open or use a collection which does not exist or is not available at that time.
    Message
    "The requested collection '{collectionname}' cannot be found."
    Properties
    TBD
    """
    pass

class AuthenticationException(CouchbaseError):
    """An authorization failure is returned by the server for given resource and credentials.
    Message
    "An authorization error has occurred"
    Properties
    TBD"""
class AccessDeniedException(CouchbaseError):
    pass


# previous exceptions


class CASMismatchException(CouchbaseError):
    pass


class ReplicaNotConfiguredException(CouchbaseError):
    pass


class DurabilityAmbiguous(CouchbaseError):
    pass


class DocumentConcurrentlyModifiedException(CouchbaseError):
    pass


class DocumentMutationLostException(CouchbaseError):
    pass


class ReplicaNotAvailableException(CouchbaseError):
    pass


class FiniteDuration(float):
    @staticmethod
    def time():
        return FiniteDuration(time.time())


class Duration(float):
    def __init__(self, seconds  # type: Union[float,int]
                 ):
        # type: (...)->None
        super(Duration, self).__init__(seconds)


class Seconds(FiniteDuration):
    def __init__(self,
                 seconds  # type: Union[float,int]
                 ):
        # type: (...)->None
        super(Seconds, self).__init__(seconds)

class Minutes(FiniteDuration):
    def __init__(self,
                 minutes  # type: float
                 ):
        pass

class OptionBlock(dict):
    def __init__(self, *args, **kwargs):
        # type: (*Any, **Any) -> None
        super(OptionBlock, self).__init__(**kwargs)

    def __getattr__(self, name):
        return property(fget=self.get, fset=self.__setitem__)

    def timeout(self, duration):
        # type: (Seconds)->OptionBlock
        self['ttl'] = duration
        return self


class GetOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(GetOptions, self).__init__(*args, **kwargs)


class IReplaceOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(IReplaceOptions, self).__init__(*args, **kwargs)


class ReplaceOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(ReplaceOptions, self).__init__(*args, **kwargs)


class AppendOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(AppendOptions, self).__init__(*args, **kwargs)


class RemoveOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(RemoveOptions, self).__init__(*args, **kwargs)


class MutateInOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(MutateInOptions, self).__init__(*args, **kwargs)


class PrependOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(PrependOptions, self).__init__(*args, **kwargs)


class UnlockOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(UnlockOptions, self).__init__(*args, **kwargs)


class IncrementOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(IncrementOptions, self).__init__(*args, **kwargs)


class DecrementOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(DecrementOptions, self).__init__(*args, **kwargs)


OptionDerivative = Union[
    GetOptions, IReplaceOptions, ReplaceOptions, AppendOptions, RemoveOptions, MutateInOptions, PrependOptions, UnlockOptions, IncrementOptions, DecrementOptions]


# noinspection PyPep8Naming
def GetSpec():
    # type: ()-> SDType
    return SD


# noinspection PyPep8Naming
def LookupInSpec():
    # type: ()-> SDType
    return SD


# noinspection PyPep8Naming
def MutateSpec():
    # type: ()-> SDType
    return SD


class LookupOperation(object):
    pass


class GetOperation(LookupOperation):
    def __init__(self, path,  # type: str
                 xattr  # type: bool
                 ):
        self.path = path
        self.xattr = xattr


class GetFullDocumentOperation(LookupOperation):
    pass


class GetStringOperation(LookupOperation):
    def __init__(self,
                 path,  # type: str
                 xattr  # type: bool
                 ):
        self.path = path
        self.xattr = xattr


class GetIntOperation(LookupOperation):
    def __init__(self,
                 path,  # type: str
                 xattr  # type: bool
                 ):
        self.path = path
        self.xattr = xattr


class ExistsOperation(LookupOperation):
    def __init__(self,
                 path,  # type: str
                 xattr  # type: bool
                 ):
        self.path = path
        self.xattr = xattr


class ReadSpec(list):
    def get_full_document(self):
        # type: ()->ReadSpec
        result = copy.deepcopy(self)
        result.append(GetFullDocumentOperation())
        return result

    def get(self, *path  # type: Tuple[str,...]
            ):
        # type: (...) -> ReadSpec
        if not hasattr(path, "__iter__"):
            return self.get_single(path)
        result = copy.deepcopy(self)
        result += map(lambda v: GetOperation(v, False), path)
        return result

    def get_single(self, path,  # type: str
                   xattr=False  # type: bool
                   ):
        # type (...) -> ReadSpec
        result = copy.deepcopy(self)
        result.append(GetOperation(path, xattr))

    def get_str(self, path,  # type: str
                xattr=False  # type: bool
                ):
        result = copy.deepcopy(self)
        result.append(GetStringOperation(path, xattr))

    def get_int(self, path,  # type: str
                xattr=False  # type: bool
                ):
        result = copy.deepcopy(self)
        result.append(GetIntOperation(path, xattr))

    def exists(self, path,  # type: str
               xattr=False  # type: bool
               ):
        result = copy.deepcopy(self)
        result.append(ExistsOperation(path, xattr))


class Value(int):
    pass
    # def __init__(self,
    #             value  # type: Union[str,bytes,SupportsInt]
    #             ):
    #    super(Value, self).__init__(value)


class Cardinal(OptionBlock):
    Value = Value

    class ONE(Value):
        def __init__(self, *args, **kwargs):
            super(Cardinal.ONE, self).__init__(1)

    TWO = Value(2)
    THREE = Value(3)
    NONE = Value(0)


class ReplicateTo(Cardinal):
    pass


class PersistTo(Cardinal):
    pass


class Durability(object):
    MajorityAndPersistActive = None


class MutationToken:
    def __init__(self, sequenceNumber,  # type: int
                 vbucketId,  # type: int
                 vbucketUUID  # type: int
                 ):
        self.sequenceNumber = sequenceNumber
        self.vbucketId = vbucketId
        self.vbucketUUID = vbucketUUID

    def partition_id(self):
        # type: (...)->int
        pass

    def partition_uuid(self):
        # type: (...)->int
        pass

    def sequence_number(self):
        # type: (...)->int
        pass

    def bucket_name(self):
        # type: (...)->str
        pass

class SDK2MutationToken(MutationToken):
    def __init__(self, token):
        super(SDK2MutationToken,self).__init__(token[2],token[0],token[1])
            #LCB_MUTATION_TOKEN_VB(mutinfo),
            #LCB_MUTATION_TOKEN_ID(mutinfo),
            #LCB_MUTATION_TOKEN_SEQ(mutinfo),
            #conn->bucket);




class IResult:
    def __init__(self,
                 cas,  # type: int
                 error  # type: int
                ):
        self.cas = cas
        self.error = error
    def cas(self):
        # type: ()->int
        return self.cas
    def error(self):
        # type: ()->int
        return self.error
    def success(self):
        # type: ()->bool
        return not self.error

class IGetResult(IResult):

    @abstractproperty
    def id(self):
        # type: ()->str
        pass

    @abstractproperty
    def expiry(self):
        # type: ()->FiniteDuration
        pass


class IMutationResult(IResult):
    def __init__(self,
                 cas,  # type: int
                 mutation_token=None  # type: MutationToken
                 ):
        super(IMutationResult,self).__init__(cas)
        self.mutationToken = mutation_token

    def mutation_token(self):
        # type: () -> MutationToken
        return self.mutationToken


class MutationResult(IMutationResult):
    pass


Proxy_T=TypeVar('T',Any)


class ContentProxyBase(object):
    def __init__(self,content):
        self.content=content

    def __call__(self, jsonpath):
        return self.content


class ContentProxyMeta(type):
    def __new__(cls, name, bases, dct,
                *args,  # type: Tuple[Any...,Proxy_T]
                **kwargs):
        # type: (...)->Type[ContentProxyBase[Proxy_T]]
        return type.__new__(cls, name, bases, dct, *args, **kwargs)


class ContentProxy:
    def __init__(self,content):
        self.content=content

    def __getitem__(self,
                    item  # type: Type[Proxy_T]
                    ):
        # type: (...)->ContentProxyMeta[Proxy_T]
        return ContentProxyMeta(Proxy_T)()


class GetResult(IGetResult):
    def __init__(self,
                 content,  # type: JSONDocument
                 *args,  # type: Any
                 **kwargs  # type: Any
                 ):
        # type: (...) ->None
        self._content = content  # type: JSONDocument
        self.dict = kwargs

    def content_as_array(self):
        # type: (...) ->List
        return list(self.content)

    @property
    def contentAs(self):
        # type: (...)->ContentProxy
        return ContentProxy(self._content)

    def __getitem__(self, t):
        return
    @property
    def id(self):
        # type: () -> str
        return self.dict['id']

    @property
    def cas(self):
        # type: () -> int
        return self.dict['cas']

    @property
    def expiry(self):
        # type: () -> Seconds
        return self.dict['expiry']

    @property
    def content(self):
        # type: () -> JSONDocument
        return self._content

class JSONDocument(pyrsistent.PRecord):
    def __init__(self,
                 parent=None,  # type: JSONDocument
                 **kwargs):
        super(JSONDocument, self).__init__(parent=parent, **kwargs)
        self.parent = parent

    def put(self, key, value):
        # type: (str, JSON) -> JSONDocument
        result = copy.copy(self)  # type: JSONDocument
        result.parent = self
        result.put(key, value)
        return result


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


class Bucket(six.with_metaclass(Scope)):

    def __init__(self, *args, **kwargs):
        self.collections = pyrsistent.PRecord()

    def clone_bucket_with_coll(self, name):
        # type: (Bucket, str) -> Bucket
        cloned = copy.copy(self)
        fresh_collection = Collection(self, name)
        cloned.collections = cloned.collections.set(name, fresh_collection)
        return cloned

    def open_collection(self, name):
        # type: (str) -> Bucket
        if name in self.collections.keys():
            return self.collections.get(name)
        else:
            return self.clone_bucket_with_coll(name)

    def scope(self, name):
        # type: (Bucket, str)->Bucket
        return Scope.scope(self, name)


class CollectionBase:
    def transform(self, key):
        return self.name + self.separator + key

    def __init__(self, bucket, name, separator=":"):
        self.bucket = copy.deepcopy(bucket)  # type: SDK2Bucket
        self.name = name
        self.separator = separator

    def __new__(cls, *args, **kwargs):
        import inspect
        for name, member in inspect.getmembers(cls, inspect.ismethod):
            setattr(Bucket, name, lambda self, x, *args, **kwargs: member(self, self.transform(x), *args, **kwargs))


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


M1=TypeVar('M1')


def mutation_forwarder(func,  # type: Callable[[M1],Any]
                       arg_vars,  # type: Dict[str,Any]
                       options,  # type: Dict[str,Any]
                       ):
    # type: (...)->Callable[M1,IMutationResult]
    return mutation_result(forwarder(func, arg_vars, options))


class Collection(CollectionBase):

    def __init__(self, parent, name):
        self._bucket = parent  # type: SDK2Bucket
        self.name = name

    def __getattr__(self, item):
        return getattr(self.bucket, item)

    @property
    def bucket(self):
        # type: (...)->SDK2Bucket
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
        if not spec:
            x = self.bucket.get(key, **options)
        else:
            x = self.bucket.lookup_in(key, *spec, **options)
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


class IBucket(SDK2Bucket):
    def name(self):
        # type: (...)->str
        pass

    def scope(self,
              scopeName  # type: str
              ):
        # type: (...)->Scope
        pass

    def default_collection(self,
                           options=None  # type: CollectionOptions
                           ):
        # type: (...)->ICollection
        pass

    def collection(self,
                   collection_name,  # type: str
                   options=None  # type: CollectionOptions
                   ):
        # type: (...)->ICollection
        pass

    def view_query(self,
                   design_doc,  # type: str
                   view_name,  # type: str
                   view_options=None):
        # type: (...)->IViewResult
        pass

    def spatial_view_query(self,
                           design_doc,  # type: str
                           view_name,  # type: str
                           options=None  # type: ViewOptions
                           ):
        # type: (...)->ISpatialViewResult
        pass

    def views(self):
        # type: (...)->IViewManager
        pass

    def ping(self,
             options=None  # type: PingOptions
             ):
        # type: (...)->IPingResult
        pass



class QueryOptions(OptionBlock):
    def __init__(self):
        pass


class Cluster(SDK2Cluster):
    def __init__(self,
                 conn,  # type: str
                 options  # type: ClusterOptions
                 ):
        pass
    def bucket(self,
               name,  # type: str,
               options=None  # type: BucketOptions
               ):
        # type: (...)->IBucket
        pass

    def query(self,
              statement,  # type: str,
              options=None  # type: QueryOptions
              ):
        # type: (...)->IQueryResult
        """
        Executes a N1QL query against the remote cluster returning a IQueryResult with the results of the query.
        :param statement: N1QL query
        :param options: the optional parameters that the Query service takes. See The N1QL Query API for details or a SDK 2.0 implementation for detail.
        :return: An IQueryResult object with the results of the query or error message if the query failed on the server.
        :except Any exceptions raised by the underlying platform - HTTP_TIMEOUT for example.
        :except ServiceNotFoundException - service does not exist or cannot be located.

        """
        pass

    def analytics_query(self,
                        statement,  # type: str,
                        options=None  # type: AnalyticsOptions
                        ):
        # type: (...)->IAnalyticsResult
        """
        Executes an Analytics query against the remote cluster and returns a IAnalyticsResult with the results of the query.
        :param statement: the analytics statement to execute
        :param options: the optional parameters that the Analytics service takes based on the Analytics RFC.
        :return: An IAnalyticsResult object with the results of the query or error message if the query failed on the server.
        Throws Any exceptions raised by the underlying platform - HTTP_TIMEOUT for example.
        :except ServiceNotFoundException - service does not exist or cannot be located.
        """
        pass

    def search_query(self,
                     query,  # type: ISearchQuery
                     options=None  # type: SearchOptions
                     ):
        # type: (...)->ISearchResult
        """
        Executes a Search or FTS query against the remote cluster and returns a ISearchResult implementation with the results of the query.

        :param query: the fluent search API to construct a query for FTS
        :param options: the options to pass to the cluster with the query based off the FTS/Search RFC
        :return: An ISearchResult object with the results of the query or error message if the query failed on the server.
        Any exceptions raised by the underlying platform - HTTP_TIMEOUT for example.
        :except    ServiceNotFoundException - service does not exist or cannot be located.

        """
        pass

    def diagnostics(self,
                    reportId=None  # type: str
                    ):
        # type: (...)->IDiagnosticsResult
        """
        Creates a diagnostics report that can be used to determine the healthfulness of the Cluster.
        :param reportId - an optional string name for the generated report.
        :return:A IDiagnosticsResult object with the results of the query or error message if the query failed on the server.

        """
        pass

    def users(self):
        # type: (...)->IUserManager
        pass

    def indexes(self):
        # type: (...)->IIndexManager
        pass

    def nodes(self):
        # type: (...)->INodeManager
        pass

    def buckets(self):
        # type: (...)->IBucketManager
        pass

    def disconnect(self,
                   options=None  # type: DisconnectOptions
                   ):
        # type: (...)->None
        """
        Closes and cleans up any resources used by the Cluster and any objects it owns. Note the name is platform idiomatic.

        :param options - TBD
        :return: None
        :except Any exceptions raised by the underlying platform

        """
        pass

    def manager(self):
        # type: (...)->ClusterManager
        """

        Manager
        Returns a ClusterManager object for managing resources at the Cluster level.

        Caveats and notes:
        It is acceptable for a Cluster object to have a static Connect method which takes a Configuration for ease of use.
        To facilitate testing/mocking, it's acceptable for this structure to derive from an interface at the implementers discretion.
        The "Get" and "Set" prefixes are considered platform idiomatic and can be adjusted to various platform idioms.
        The Configuration is passed in via the ctor; overloads for connection strings and various other platform specific configuration are also passed this way.
        If a language does not support ctor overloading, then an equivalent method can be used on the object.

        :return:
        """
        pass



# Query stuff
