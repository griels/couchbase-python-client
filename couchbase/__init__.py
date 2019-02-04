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

from typing import Callable, Any, Union, NewType, Mapping, List
import os.path

JSON = Union[str, int, float, bool, None, Mapping[str, 'JSON'], List['JSON']]
#import cppyy
import logging
CPPYY=os.getenv("PYCBC_CPPYY")
def dump_declarations(decls,dump, depth=0):
    indent=' '*(depth*4)
    dump.write("{}declarations:[".format(indent))
    depth+=1
    indent=' '*(depth*4)
    for declar in decls:
        if not hasattr(declar,"declarations"):
            continue
        for subdecl in declar.declarations:
            # builder.decl()
            items = []
            items.append("{}{}".format(indent,str(subdecl)) + '\n')
            dump.writelines(items)
            if hasattr(subdecl,"declarations"):
                dump_declarations(subdecl.declarations,dump,depth)
    depth-=1
    indent=' '*(depth*4)
    dump.write("{}]".format(indent))


try:
    from distutils.sysconfig import get_python_inc
    std_inc = get_python_inc()

    elements = os.path.split(std_inc)
    python_install_base = list(elements[:-1]) + ["site", elements[-1], "couchbase", "cmake_build_release"]
    base_inc = os.path.join(*python_install_base)


    def add_cxx_includes(inc):
        cxx_incs = get_repo(inc)
        add_include(cxx_incs)
        add_include(cxx_incs, "libcouchbase")
        add_include(cxx_incs, "libcouchbase", "couchbase++")


    def get_repo(inc):
        return os.path.join(inc, "libcouchbase-cxx-prefix", "src", "libcouchbase-cxx", "include")


    def add_include(cxx_incs, *path_components):
        final_path=os.path.join(cxx_incs,*path_components)
        import logging
        logging.error("include path adding: {}".format(final_path))
        if os.path.exists(final_path):
            cppyy.add_include_path(final_path)
        else:
            logging.error("Path does not exist: {}".format(final_path))


    try:
        add_cxx_includes(os.path.join(os.path.abspath(os.path.curdir),"cmake-build-release"))
        add_cxx_includes(base_inc)
    except:
        pass

    if CPPYY:
        cppyy.include("libcouchbase/couchbase++.h")
        from cppyy.gbl import Couchbase
        try:
            os.rmdir("couchbase_genned")
        except:
            pass

        #os.makedirs("couchbase_genned/cppyy",exist_ok=True)
        print(dir(Couchbase.Client))
        print(dir(Couchbase.GetResponse))
        print(dir(Couchbase.Buffer))
        print(dir(Couchbase.Response))
        print(dir(Couchbase))

    import sys

    from pybindgen import FileCodeSink
    from pybindgen.gccxmlparser import ModuleParser

    inc_paths = ['/usr/local/opt/llvm/include/c++/v1',
                 '/Users/ellis_breen/root/workspaces/couchbase/couchbase-python-client-mainline/cmake-build-release/install/include',
                 os.path.abspath(os.path.join(get_repo('cmake-build-release')))]
    gccxml_options = {'xml_generator': "castxml", 'xml_generator_path': "/usr/local/bin/castxml",
                      'cflags': '-std=c++1y ' + ' '.join(map(lambda x: "-I " + x, inc_paths))}

    source_files=[os.path.join(get_repo("cmake-build-release"), "libcouchbase", "couchbase++.h")]

    def my_module_gen():
        module_parser = ModuleParser('a2')
        repo_dir = get_repo("cmake-build-release")
        base_dir = os.path.join(repo_dir, "libcouchbase")
        include_files = list(
            map(lambda x: os.path.abspath(os.path.join(base_dir, x)), filter(lambda y: y.endswith('.h'), os.listdir(base_dir))))
        print(include_files)
        paths=['/usr/local/opt/llvm/include/c++/v1',repo_dir,'/Users/ellis_breen/root/workspaces/couchbase/couchbase-python-client-mainline/cmake-build-release/install/include']
        print(paths)
        with open("build/genned.py","w+") as output:
            module=module_parser.parse([os.path.join(get_repo("cmake-build-release"), "libcouchbase", "couchbase++.h")],include_paths=paths,
                                includes=['"couchbase++.h"'], pygen_sink=FileCodeSink(output),
                                gccxml_options=gccxml_options)

        print(module)

    #my_module_gen()
    import pyplusplus
    import logging
    import pygccxml
    gccxml_config=pygccxml.parser.config.xml_generator_configuration_t(**gccxml_options)
    if False:
        declarations=pyplusplus.module_builder.boost_python_builder.parser.parse(source_files,config=gccxml_config)
        logging.error("declarations {}".format(declarations))
        #module_builder.module_builder_t()
        with open('dump.txt','w+') as dump:
            import pprint
            #pprint.pprint(declarations,dump)
            dump_declarations(declarations,dump)
        try:
            import mypy.stubgen

            #mypy.stubgen.generate_stub_for_module("cppyy.gbl.Couchbase", "couchbase_genned")
        except:
            pass
    import pyplusplus.code_creators.algorithm
    builder=pyplusplus.module_builder.module_builder_t(source_files,include_paths=inc_paths,cflags=gccxml_options['cflags'],gccxml_config=gccxml_config)
    creator=builder.build_code_creator("couchbase")
    with open('src/bindings_vanilla.cpp','w+') as bindings:
        output=creator.create()
        bindings.write(output)
except BaseException as e:
    import traceback
    logging.error(repr(e))
    logging.error("exception:{}".format(traceback.format_exc()))

    #exit(0)
try:
    pass
except Exception as e:
    import traceback
    print(traceback.format_exc())
    pass
from couchbase.crypto import *
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
og

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

    Calling this function affects any :class:`~couchbase.bucket.Bucket` objects created
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

