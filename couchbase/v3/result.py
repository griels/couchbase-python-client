from abc import abstractproperty
from typing import *
from couchbase.v3.mutate_in import *

Proxy_T=TypeVar('Proxy_T')


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


class IGetResult(IResult):

    @abstractproperty
    def id(self):
        # type: ()->str
        pass

    @abstractproperty
    def expiry(self):
        # type: ()->FiniteDuration
        pass


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


class MutationResult(IMutationResult):
    pass