import time
from couchbase_v2.result import *
import copy

class FiniteDuration(object):
    def __init__(self, seconds  # type: Union[float,int]
                 ):
        self.value=seconds

    @staticmethod
    def time():
        return FiniteDuration(time.time())

    def __float__(self):
        return float(self.value)

    def __int__(self):
        return self.value

    def __add__(self, other):
        result=copy.deepcopy(self)
        result.value+=other
        return result

    def __lt__(self, other):
        return self.value<other.value

    def __gt__(self, other):
        return self.value>other.value

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


OptionBlockDeriv = TypeVar('U')


def forward_args(arg_vars, *options):
    # type: (Optional[Dict[str,Any]],Tuple[OptionBlockDeriv,...])->OptionBlockDeriv[str,Any]
    arg_vars = arg_vars or {}
    temp_options = options[0] if options and options[0] else OptionBlock()
    print("got arg_vars {} and options {}, produced end_options {}".format(arg_vars, options, temp_options))
    kwargs = arg_vars.pop('kwargs', {})
    temp_options.update(kwargs)
    temp_options.update(arg_vars)
    arg_mapping = {'spec':{'specs':lambda x:x},'id':{},'timeout': {'ttl': int}, 'self': {}, 'options': {}}
    end_options = {}
    for k, v in temp_options.items():
        map_item = arg_mapping.get(k, None)
        if not (map_item is None):
            for out_k, out_f in map_item.items():
                end_options[out_k] = out_f(v)
        else:
            end_options[k] = v
    return end_options


def get_mutation_result(result):
    return MutationResult(result.cas, SDK2MutationToken(result._mutinfo) if hasattr(result,'_mutinfo') else None)


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