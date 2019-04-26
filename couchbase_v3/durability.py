from couchbase_v3.options import OptionBlock
import couchbase._libcouchbase as _LCB


class Value(int):
    def __init__(self,
                 value  # type: Union[str,bytes,SupportsInt]
                 ):
        self.value=value

    def __int__(self):
        return self.value

    def __float__(self):
        return self.value


class Cardinal(OptionBlock):
    #Value = Value

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
    MAJORITY_AND_PERSIST_ACTIVE = _LCB.LCB_DURABILITYLEVEL_MAJORITY_AND_PERSIST_ON_MASTER
    NONE = _LCB.LCB_DURABILITYLEVEL_NONE
    MAJORITY = _LCB.LCB_DURABILITYLEVEL_MAJORITY
    PERSIST_TO_MAJORITY = _LCB.LCB_DURABILITYLEVEL_PERSIST_TO_MAJORITY



