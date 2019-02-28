import time
from typing import *

from couchbase.v3.mutate_in import SDK2MutationToken
from couchbase.v3.result import *


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


class Durations:
    def __init__(self):
        pass

    @staticmethod
    def minutes(minutes  # type: int
                ):
        return Seconds(minutes*60)


    @staticmethod
    def days(days  # type: int
            ):
        return Durations.minutes(days*24*60)

    @staticmethod
    def seconds(seconds):
        return Seconds(seconds)


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


class QueryParameters(OptionBlock):
    def __init__(self, *args, **kwargs):
        super(QueryParameters, self).__init__(*args, **kwargs)


class QueryOptions(OptionBlock):
    def __init__(self, parameters=None,  # type: QueryParameters
                 *args,
                 **kwargs):
        super(QueryOptions, self).__init__(*args, **kwargs)


OptionDerivative = Union[
    GetOptions, IReplaceOptions, ReplaceOptions, AppendOptions, RemoveOptions, MutateInOptions, PrependOptions, UnlockOptions, IncrementOptions, DecrementOptions]


def forward_args(arg_vars, *options):
    # type: (Dict[str,Any],Tuple[OptionBlock,...])->OptionDerivative[str,Any]
    end_options = options[0] if options and options[0] else OptionBlock()
    kwargs=arg_vars.pop('kwargs',{})
    end_options.update(kwargs)
    end_options.update(dict((k.replace("timeout", "ttl"), v) for k, v in
                            arg_vars.items() if k != "self"))

    return end_options


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


def mutation_forwarder(func,  # type: Callable[[M1],Any]
                       arg_vars,  # type: Dict[str,Any]
                       options,  # type: Dict[str,Any]
                       ):
    # type: (...)->Callable[M1,IMutationResult]
    return mutation_result(forwarder(func, arg_vars, options))