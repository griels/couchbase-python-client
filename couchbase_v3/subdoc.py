from couchbase_v2.result import ValueResult
from couchbase_v2 import subdocument as SD

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


def LookupOperation(*args, **kwargs):
    return super(LookupOperation, cls).__new__(cls, tuple(args))

import couchbase_v2.subdocument as SD

def GetOperation(path,  # type: str
                 xattr  # type: bool
                 ):
        return SD.get(path,xattr=xattr)
        #super(GetOperation,cls).__new__(SD.get(path,xattr=xattr))


def GetFullDocumentOperation(path,  # type: str
                 xattr  # type: bool
                 ):
    return SD.get(path,xattr=xattr)

#
# class GetStringOperation(LookupOperation):
#     def __init__(self,
#                  path,  # type: str
#                  xattr  # type: bool
#                  ):
#         super(GetOperation,self).__init__(SD.get(path,xattr=xattr))
#
#
# class GetIntOperation(LookupOperation):
#     def __init__(self,
#                  path,  # type: str
#                  xattr  # type: bool
#                  ):
#         super(GetOperation,self).__init__(SD.get(path,xattr=xattr))
#
#
def ExistsOperation(
                 path,  # type: str
                 xattr  # type: bool
                 ):
        return SD.exists(path,xattr=xattr)
