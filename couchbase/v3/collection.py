from typing import *

from couchbase import subdocument as SD
from couchbase.v3.options import forward_args, get_mutation_result, mutation_result
from couchbase.v3.options import OptionBlock
from couchbase.v3.durability import ReplicateTo, PersistTo
from couchbase.bucket import Bucket as SDK2Bucket
from couchbase.v3.result import *
from couchbase.v3.mutate_in import *
from couchbase._libcouchbase import Bucket as _Bucket


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


class CollectionOptions(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(Collection.CollectionOptions, self).__init__(*args, **kwargs)

class LockOptions(OptionBlock):
    pass

class Collection(object):

    def __init__(self,
                 parent,  # type: Bucket
                 name=None,  # type: str
                 options=None  # type: CollectionOptions
                 ):
        # type: (...)->None
        self.parent = parent
        self._bucket = parent._bucket  # type: _Bucket
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

    class GetOptions(OptionBlock):
        def __init__(self, *args, **kwargs):
            super(Collection.GetOptions, self).__init__(*args, **kwargs)

    class GetAndLockOptions(GetOptions,LockOptions):
        pass

    @overload
    def get(self,
            key,  # type:str
            spec=None,  # type: Optional[GetSpec]
            options=None,  # type: GetOptions
            ):
        # type: (...) -> GetResult
        pass

    @overload
    def get(self,
            key,  # type:str
            spec=None,  # type: Optional[GetSpec]
            timeout=None,  # type: Seconds
            quiet=None,  # type: bool
            replica=False,  # type: bool
            no_format=False  # type: bool
            ):
        # type: (...) -> GetResult
        pass

    def _get_generic(self, key, spec, options, kwargs):
        options = forward_args(locals(), kwargs)
        project = options.pop('project', None)
        if project:
            if len(project) < 17:
                spec = map(SD.get, project)
        bc = self.bucket  # type: SDK2Bucket
        if not spec:
            x = bc.get(key, **options)
        else:
            x = bc.lookup_in(key, *spec, **options)
        return options, x

    def get(self,
            key,  # type: str
            spec=None,  # type: Optional[GetSpec]
            *options,  # type: Any
            **kwargs  # type: Any
            ):
        # type: (...) -> GetResult

        options, x = self._get_generic(key, spec, options, kwargs)
        return get_result(options, x)

    @overload
    def get_and_touch(self,
                      id,  # type: str
                      expiration,  # type: int
                      options=None  # type: GetAndTouchOptions
                      ):
        # type: (...)->IGetResult
        pass

    def get_and_touch(self,
                      id,  # type: str
                      expiration,  # type: int
                      *options,  # type: Any
                      **kwargs  # type: Any
                      ):
        # type: (...)->IGetResult
        cb = self._bucket  # type: SDK2Bucket
        x = cb.get(id, **forward_args(kwargs, *options))
        return get_result(options, x)

    def get_and_lock(self,
                     id,  # type: str
                     expiration,  # type: int
                     options=None  # type: GetAndLockOptions
                     ):
        # type: (...)->IGetResult
        cb = self.bucket  # type: SDK2Bucket
        x = cb.get(id, expiration, False, False, False)
        cb.lock(id,options)
        return get_result(x, options)

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
               options=None,  # type: ExistsOptions
               ):
        # type: (...)->IExistsResult
        pass

    class UpsertOptions(OptionBlock):
        def __init__(self, *args, **kwargs):
            super(Collection.UpsertOptions, self).__init(*args, **kwargs)

    @overload
    def upsert(self, key, value, options  # type: UpsertOptions
               ):
        pass

    @overload
    def upsert(self, key, value, cas=0, ttl=0, format=None,
               persist_to=0, replicate_to=0, durability_level=0):
        # type: (Union[str, bytes], Any, int, int, int, int, int, int) -> IMutationResult
        pass

    @mutation_result
    def upsert(self,
               id,  # type: str
               value,  # type: Any
               *args,  # type: Any
               **kwargs  # type: Any
               ):
        # type: (...)->IMutationResult
        """Unconditionally store the object in Couchbase.

        :param key:
            The key to set the value with. By default, the key must be
            either a :class:`bytes` or :class:`str` object encodable as
            UTF-8. If a custom `transcoder` class is used (see
            :meth:`~__init__`), then the key object is passed directly
            to the transcoder, which may serialize it how it wishes.
        :type key: string or bytes

        :param value: The value to set for the key.
            This should be a native Python value which will be transparently
            serialized to JSON by the library. Do not pass already-serialized
            JSON as the value or it will be serialized again.

            If you are using a different `format` setting (see `format`
            parameter), and/or a custom transcoder then value for this
            argument may need to conform to different criteria.

        :param int cas: The _CAS_ value to use. If supplied, the value
            will only be stored if it already exists with the supplied
            CAS

        :param int ttl: If specified, the key will expire after this
            many seconds

        :param int format: If specified, indicates the `format` to use
            when encoding the value. If none is specified, it will use
            the `default_format` For more info see
            :attr:`~.default_format`

        :param int persist_to:
            Perform durability checking on this many nodes nodes for
            persistence to disk. See :meth:`endure` for more information

        :param int replicate_to: Perform durability checking on this
            many replicas for presence in memory. See :meth:`endure` for
            more information.

        :param int durability_level: Durability level

        :raise: :exc:`.ArgumentError` if an argument is supplied that is
            not applicable in this context. For example setting the CAS
            as a string.
        :raise: :exc`.CouchbaseNetworkError`
        :raise: :exc:`.KeyExistsError` if the key already exists on the
            server with a different CAS value.
        :raise: :exc:`.ValueFormatError` if the value cannot be
            serialized with chosen encoder, e.g. if you try to store a
            dictionary in plain mode.
        :return: :class:`~.Result`.

        Simple set::

            cb.upsert('key', 'value')

        Force JSON document format for value::

            cb.upsert('foo', {'bar': 'baz'}, format=couchbase.FMT_JSON)

        Insert JSON from a string::

            JSONstr = '{"key1": "value1", "key2": 123}'
            JSONobj = json.loads(JSONstr)
            cb.upsert("documentID", JSONobj, format=couchbase.FMT_JSON)

        Force UTF8 document format for value::

            cb.upsert('foo', "<xml></xml>", format=couchbase.FMT_UTF8)

        Perform optimistic locking by specifying last known CAS version::

            cb.upsert('foo', 'bar', cas=8835713818674332672)

        Several sets at the same time (mutli-set)::

            cb.upsert_multi({'foo': 'bar', 'baz': 'value'})

        .. seealso:: :meth:`upsert_multi`
        """

        cb = self.bucket._bucket  # type:
        return cb.upsert(id, value, **forward_args(*args, **kwargs))
        ret

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
                  options=None  # type: LookupInOptions
                  ):
        # type: (...)->ILookupInResult
        pass

    def mutate_in(self, id,  # type: str,
                  spec,  # type: MutateInSpec
                  options=None  # type: MutateInOptions
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

    @overload
    def append(self,key,  # type: str
               value,  # type: couchbase.JSON
               cas=0,  # type: long
               format=None,  # type: long
               persist_to=0,  # type: int
               replicate_to=0  # type: int
               ):
        # type: (...) -> couchbase.result.Result

        pass

    def append(self,
               id,  # type: str
               value,  # type: str
               *args,  # type: Any
               **kwargs  # type: Any
               ):
        # type: (...)->IMutationResult
        """Append a string to an existing value in Couchbase.

        :param string value: The data to append to the existing value.

        Other parameters follow the same conventions as :meth:`upsert`.

        The `format` argument must be one of
        :const:`~couchbase.FMT_UTF8` or :const:`~couchbase.FMT_BYTES`.
        If not specified, it will be :const:`~.FMT_UTF8` (overriding the
        :attr:`default_format` attribute). This is because JSON or
        Pickle formats will be nonsensical when random data is appended
        to them. If you wish to modify a JSON or Pickle encoded object,
        you will need to retrieve it (via :meth:`get`), modify it, and
        then store it again (using :meth:`upsert`).

        Additionally, you must ensure the value (and flags) for the
        current value is compatible with the data to be appended. For an
        example, you may append a :const:`~.FMT_BYTES` value to an
        existing :const:`~couchbase.FMT_JSON` value, but an error will
        be thrown when retrieving the value using :meth:`get` (you may
        still use the :attr:`data_passthrough` to overcome this).

        :raise: :exc:`.NotStoredError` if the key does not exist

        .. seealso:: :meth:`upsert`, :meth:`append_multi`)
        """
        cb=self.bucket._bucket  # type: SDK2Bucket
        x=cb.append(id,value, forward_args(kwargs, *args))
        return get_mutation_result(x)

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
        pass
        # return mutation_forwarder(self.bucket.prepend, locals(), kwargs)(id, None)

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


UpsertOptions = Collection.UpsertOptions
GetOptions = Collection.GetOptions
