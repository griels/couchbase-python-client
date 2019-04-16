import enum
from typing import *


class MutateInSpecItemBase(object):
    def __init__(self,
                 path,  # type: str
                 xattr=None,
                 createParents=None,
                 options=None  # type: MutateOptions
                 ):
        # type: (...)->None
        self.path = path
        self.options = options


class MutateReplace(MutateInSpecItemBase):
    def __init__(self, *args, **kwargs):
        super(MutateReplace, self).__init__(*args, **kwargs)

    def __call__(self, *args, **kwargs):
        return

    fish = 0


class MutateInsert(MutateInSpecItemBase):
    def __init__(self, *args, **kwargs):
        super(MutateInsert, self).__init__(*args, **kwargs)

    def __call__(self, *args, **kwargs):
        return

    cheese = 9


class MutateInSpecItem(enum.Enum):
    replace = MutateReplace
    insert = MutateInsert

    def __call__(self, *args, **kwargs):
        return self.value(*args, **kwargs)


MutateInSpec = Tuple[MutateInSpecItem, ...]


class MutationToken(object):
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
        token=token or (None,None,None)
        super(SDK2MutationToken,self).__init__(token[2],token[0],token[1])
            #LCB_MUTATION_TOKEN_VB(mutinfo),
            #LCB_MUTATION_TOKEN_ID(mutinfo),
            #LCB_MUTATION_TOKEN_SEQ(mutinfo),
            #conn->bucket);