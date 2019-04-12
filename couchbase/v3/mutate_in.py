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
