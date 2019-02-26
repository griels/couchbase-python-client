from couchbase.v3.options import OptionBlock


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


