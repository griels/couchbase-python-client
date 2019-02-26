import time
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


class OptionBlockMeta(type):
    def __new__(cls, *args, **kwargs):
        pass


OptionBlockDeriv=TypeVar('U')


def forward_args(arg_vars, *options):
    # type: (Optional[Dict[str,Any]],Tuple[OptionBlockDeriv,...])->OptionBlockDeriv[str,Any]
    arg_vars=arg_vars or {}
    end_options = options[0] if options and options[0] else OptionBlock()
    print("got arg_vars {} and options {}, produced end_options {}".format(arg_vars,options,end_options))
    kwargs=arg_vars.pop('kwargs',{})
    end_options.update(kwargs)
    end_options.update(dict((k.replace("timeout", "ttl"), v) for k, v in
                            arg_vars.items() if k != "self"))

    return end_options


def get_mutation_result(result):
    return MutationResult(result.cas, SDK2MutationToken(result.mutinfo))


def mutation_result(func):
    def mutated(*args,**kwargs):
        result=func(*args,**kwargs)
        return get_mutation_result(result)

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