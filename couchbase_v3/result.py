from abc import abstractproperty

from couchbase_v2.JSONdocument import JSONDocument
from couchbase_v2.mutate_in import *

Proxy_T = TypeVar('Proxy_T')


class ContentProxy:
    def __init__(self, content):
        self.content = content

    def __getitem__(self,
                    item  # type: Type[Proxy_T]
                    ):
        # type: (...)->Proxy_T
        decode_method=getattr(item,'decode',None)
        return decode_method(self.content) if decode_method else item(self.content)


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
        super(IMutationResult, self).__init__(cas,None)
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
    def content_as(self):
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


def get_result(self, x, options=None):
    options = options or {}
    return GetResult(x.value, cas=x.cas, expiry=options.pop('timeout', None), id=x.key)


class MutationResult(IMutationResult):
    pass
