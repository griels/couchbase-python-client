#
# Copyright 2013, Couchbase, Inc.
# All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import warnings

# Pythons > (2.7||3.2) silence deprecation warnings by default.
# Many folks are not happy about this, as it avoids letting them
# know about potential upcoming breaking changes in their code.
# Here we add a warning filter for any deprecation warning thrown
# by Couchbase
warnings.filterwarnings(action='default',
                        category=DeprecationWarning,
                        module=r"^couchbase($|\..*)")

from couchbase.user_constants import *
import couchbase._libcouchbase as _LCB
#import cppyy
try:
    from couchbase._version import __version__

except ImportError:
    __version__ = "0.0.0-could-not-find-git"


def set_json_converters(encode, decode):
    """
    Modify the default JSON conversion functions. This affects all
    :class:`~couchbase.bucket.Bucket` instances.

    These functions will called instead of the default ones (``json.dumps``
    and ``json.loads``) to encode and decode JSON (when :const:`FMT_JSON` is
    used).

    :param callable encode: Callable to invoke when encoding an object to JSON.
        This should have the same prototype as ``json.dumps``, with the
        exception that it is only ever passed a single argument.

    :param callable decode: Callable to invoke when decoding an object to JSON.
        This should have the same prototype and behavior
        as ``json.loads`` with the exception that it is only ever
        passed a single argument.

    :return: A tuple of ``(old encoder, old decoder)``

    No exceptions are raised, and it is the responsibility of the caller to
    ensure that the provided functions operate correctly, otherwise exceptions
    may be thrown randomly when encoding and decoding values
    """
    ret = _LCB._modify_helpers(json_encode=encode, json_decode=decode)
    return (ret['json_encode'], ret['json_decode'])


def set_pickle_converters(encode, decode):
    """
    Modify the default Pickle conversion functions. This affects all
    :class:`~couchbase.bucket.Bucket` instances.

    These functions will be called instead of the default ones
    (``pickle.dumps`` and ``pickle.loads``) to encode and decode values to and
    from the Pickle format (when :const:`FMT_PICKLE` is used).

    :param callable encode: Callable to invoke when encoding an object to
        Pickle. This should have the same prototype as ``pickle.dumps`` with
        the exception that it is only ever called with a single argument

    :param callable decode: Callable to invoke when decoding a Pickle encoded
        object to a Python object. Should have the same prototype as
        ``pickle.loads`` with the exception that it is only ever passed a
        single argument

    :return: A tuple of ``(old encoder, old decoder)``

    No exceptions are raised and it is the responsibility of the caller to
    ensure that the provided functions operate correctly.
    """
    ret = _LCB._modify_helpers(pickle_encode=encode, pickle_decode=decode)
    return (ret['pickle_encode'], ret['pickle_decode'])


def _to_json(*args):
    """
    Utility function to encode an object to json using the user-defined
    JSON encoder (see :meth:`set_json_converters`).

    :param args: Arguments passed to the encoder
    :return: Serialized JSON string
    """
    return _LCB._get_helper('json_encode')(*args)


def _from_json(*args):
    """
    Utility function to decode a JSON string to a Python object using
    the user-defined JSON decoder (see :meth:`set_json_converters`).

    :param args: Arguments passed to the decoder
    :return: Python object converted from JSON
    """
    return _LCB._get_helper('json_decode')(*args)


def enable_logging():
    """
    Enables integration with Python's `logging` module.

    This function enables the C library's logging to be propagated to
    the Python standard `logging` module.

    Calling this function affects any :class:`~.Bucket` objects created
    afterwards (but not before). Note that currently this will also
    override any ``LCB_LOGLEVEL`` directive inside the environment as
    well.

    The "root" couchbase logger is ``couchbase``.
    """
    import couchbase._logutil
    couchbase._logutil.configure(True)


def disable_logging():
    import couchbase._logutil
    couchbase._logutil.configure(False)


class Couchbase(object):
    @classmethod
    def connect(self, bucket, **kwargs):
        from couchbase.bucket import _depr
        from couchbase.connection import Connection
        _depr('Couchbase.connect()', 'Bucket()')
        return Connection(bucket, **kwargs)

from basictracer import BasicTracer
from basictracer.recorder import InMemoryRecorder
from opentracing.ext import tags


recorder = InMemoryRecorder()
tracer = BasicTracer(recorder=recorder)

def get_tracer():
    return tracer

def get_recorder():
    return recorder


def get_sampled_spans():
    return [span for span in recorder.get_spans() if span.context.sampled]




def test_span_log_kv():
    recorder = InMemoryRecorder()
    tracer = BasicTracer(recorder=recorder)

    span = tracer.start_span('x')
    span.log_kv({
        'foo': 'bar',
        'baz': 42,
    })
    span.finish()

    finished_spans = recorder.get_spans()
    assert len(finished_spans) == 1
    assert len(finished_spans[0].logs) == 1
    assert len(finished_spans[0].logs[0].key_values) == 2
    assert finished_spans[0].logs[0].key_values['foo'] == 'bar'
    assert finished_spans[0].logs[0].key_values['baz'] == 42


from opentracing_instrumentation import traced_function

import inspect

from basictracer.span import BasicSpan
from opentracing.span import Span

def lcb_span(BasicSpan):
    def __init__(
            self,
            tracer,
            operation_name=None,
            context=None,
            parent_id=None,
            tags=None,
            start_time=None):
        super(BasicSpan, self).__init__(tracer, context)
        self._tracer = tracer
        self._lock = Lock()

        self.operation_name = operation_name
        self.start_time = start_time
        self.parent_id = parent_id
        self.tags = tags if tags is not None else {}
        self.duration = -1
        self.logs = []

    def set_operation_name(self, operation_name):
        with self._lock:
            self.operation_name = operation_name
        return super(lcb_span, self).set_operation_name(operation_name)

    def set_tag(self, key, value):
        with self._lock:
            if key == tags.SAMPLING_PRIORITY:
                self.context.sampled = value > 0
            if self.tags is None:
                self.tags = {}
            self.tags[key] = value
        return super(lcb_span, self).set_tag(key, value)

    def log_kv(self, key_values, timestamp=None):
        with self._lock:
            self.logs.append(LogData(key_values, timestamp))
        return super(lcb_span, self).log_kv(key_values, timestamp)

    def finish(self, finish_time=None):
        with self._lock:
            finish = time.time() if finish_time is None else finish_time
            self.duration = finish - self.start_time
            self._tracer.record(self)

    def set_baggage_item(self, key, value):
        new_context = self._context.with_baggage_item(key, value)
        with self._lock:
            self._context = new_context
        return self

    def get_baggage_item(self, key):
        with self._lock:
            return self.context.baggage.get(key)

def decorate_class(cls, **kwargs):
    getmembers = inspect.getmembers(cls, inspect.isfunction)
    if len(getmembers) == 0:
        getmembers = inspect.getmembers(cls)
        filter(lambda k, v: k.startswith('_'), cls.__dict__.items())
    # return class_decorator()(cls)
    # print(str(cls)+":"+str(getmembers))
    for name, method in getmembers:
        # print(name)
        # print(str(method))
        #     #if name.startswith('_'):
        # types.MemberDescriptorType:
        setattr(cls, name, traced_function(getattr(cls, name), **kwargs))
    # for attr in cls.__dict__:
    # if callable(getattr(cls,attr)):
    # setattr(cls, attr, fiddle(getattr(cls,attr)))
    return cls
