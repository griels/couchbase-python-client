from couchbase.result import ValueResult
from couchbase import subdocument as SD

SDType = type(SD)
Document = ValueResult


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

"""
  MutateInOp.replace('some.path', {xattr: '_sync'}),
  MutateInOp.insert('some.other.path`, {xattr: '_sync', createParents: true})
], ...)"""


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
