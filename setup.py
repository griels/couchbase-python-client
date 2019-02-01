#!/usr/bin/env python
import sys
import os.path
import os
import platform
import warnings
import couchbase_version
import pip

from cmodule import module, setup_kw, extoptions

try:
    if os.environ.get('PYCBC_NO_DISTRIBUTE'):
        raise ImportError()

    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

import re
pkgdata = {}
pkgversion = None

try:
    couchbase_version.gen_version()
except couchbase_version.CantInvokeGit:
    pass

pkgversion = couchbase_version.get_version()


LCB_NAME = None
extoptions['extra_compile_args'] = []
extoptions['extra_link_args'] = []


def boolean_option(flag):
    return "-D{}={}".format(flag,os.environ.get(flag))


COMP_OPTION_PREFIX = "PYCBC_COMP_OPT_"


def comp_option(flag):
    return "-{}={}".format(flag.replace(COMP_OPTION_PREFIX, ""), os.environ.get(flag))


COMP_OPTION_BOOL_PREFIX = "PYCBC_COMP_OPT_BOOL_"


def comp_option_bool(flag):
    return "-{}".format(flag.replace(COMP_OPTION_BOOL_PREFIX, ""))


CLANG_SAN_OPTIONS={"address":"lsan","undefined":"ubsan"}

CLANG_SAN_PREFIX = "PYCBC_SAN_OPT_"

def comp_clang_san_option(flag):
    san_option = flag.replace(CLANG_SAN_PREFIX, "")
    fsanitize_statement = "-fsanitize={}".format(san_option)
    extoptions['extra_link_args']+=["{}".format(fsanitize_statement)]
    return fsanitize_statement


def comp_option_pattern(prefix):
    return re.escape(prefix) + ".*"


comp_flags = {"PYCBC_STRICT":boolean_option,
              "PYCBC_TABBED_CONTEXTS_ENABLE": boolean_option,
              "PYCBC_REF_ACCOUNTING": boolean_option,
              "PYCBC_TRACING_DISABLE": boolean_option, "PYCBC_DEBUG": boolean_option,
              "PYCBC_CRYPTO_VERSION": boolean_option, comp_option_pattern(COMP_OPTION_PREFIX): comp_option,
              comp_option_pattern(COMP_OPTION_BOOL_PREFIX): comp_option_bool,
              comp_option_pattern(CLANG_SAN_PREFIX):comp_clang_san_option}

debug_symbols = len(set(os.environ.keys()) & {"PYCBC_DEBUG", "PYCBC_DEBUG_SYMBOLS"}) > 0

comp_arg_additions = (action(actual_flag) for flag, action in comp_flags.items() for actual_flag in os.environ.keys() if
                      re.match(flag, actual_flag))
extoptions['extra_compile_args'] += list(comp_arg_additions) + ['-std=c++1y']
if sys.platform != 'win32':
    extoptions['libraries'] = ['couchbase','boost_python27']
    if debug_symbols:
        extoptions['extra_compile_args'] += ['-O0', '-g3']
        extoptions['extra_link_args'] += ['-O0', '-g3']
    if sys.platform == 'darwin':
        warnings.warn('Adding /usr/local to search path for OS X')
        extoptions['library_dirs'] = ['/usr/local/lib']
        extoptions['include_dirs'] = ['/usr/local/include']

else:
    if sys.version_info<(3,0,0):
        if pip.__version__<"9.0.0":
            raise pip.exceptions.InstallationError("Windows on Python earlier than v3 unsupported.")

    warnings.warn("I'm detecting you're running windows."
                  "You might want to modify "
                  "the 'setup.py' script to use appropriate paths")


    # The layout i have here is an ..\lcb-winbuild, in which there are subdirs
    # called 'x86' and 'x64', for x86 and x64 architectures. The default
    # 'nmake install' on libcouchbase will install them to 'deps'
    bit_type = platform.architecture()[0]
    lcb_root = os.path.join(os.path.pardir, 'lcb-winbuild')

    if bit_type.startswith('32'):
        lcb_root = os.path.join(lcb_root, 'x86')
    else:
        lcb_root = os.path.join(lcb_root, 'x64')

    lcb_root = os.path.join(lcb_root, 'deps')

    extoptions['libraries'] = ['libcouchbase']
    ## Enable these lines for debug builds
    if debug_symbols:
        extoptions['extra_compile_args'] += ['/Zi','/DEBUG','/O0']
        extoptions['extra_link_args'] += ['/DEBUG','-debug']
    extoptions['library_dirs'] = [os.path.join(lcb_root, 'lib')]
    extoptions['include_dirs'] = [os.path.join(lcb_root, 'include')]
    extoptions['define_macros'] = [('_CRT_SECURE_NO_WARNINGS', 1)]
    pkgdata['couchbase'] = ['libcouchbase.dll']


SOURCEMODS = [
        'exceptions',
        'ext',
        'result',
        'opresult',
        'callbacks',
        'cntl',
        'convert',
        'bucket',
        'store',
        'constants',
        'multiresult',
        'miscops',
        'typeutil',
        'oputil',
        'get',
        'counter',
        'http',
        'htresult',
        'ctranscoder',
        'crypto',
        'observe',
        'iops',
        'connevents',
        'pipeline',
        'views',
        'n1ql',
        'fts',
        'ixmgmt'
        ]

SOURCEMODS_CPP = [
    'bindings'
]
if platform.python_implementation() != 'PyPy':
    #extoptions['sources'] = [ os.path.join("src", m + ".c") for m in SOURCEMODS ]
    extoptions['sources'] = [ os.path.join("src", m + ".cpp") for m in SOURCEMODS_CPP]
    module = Extension('couchbase._libcouchbase', **extoptions)
    setup_kw = {'ext_modules': [module]}
else:
    warnings.warn('The C extension libary does not work on PyPy. '
            'You should install the couchbase_ffi module. Installation of this '
            'module will continue but will be unusable without couchbase_ffi')
    setup_kw = {}

cmake_build=os.environ.get("PYCBC_CMAKE_BUILD")

# Dummy dependency to prevent installation of Python < 3 package on Windows.

pip_not_on_win_python_lt_3 = (
    ["pip>=9.0; (sys_platform != 'win32' and python_version >= '2.7') or (python_version >= '3.0')"]
    if pip.__version__ >= "9.0.0"
    else [])

conan_and_cmake_deps = (['conan', 'cmake>=3.0.2'] if
                        cmake_build and sys.platform.startswith('darwin') else [])

typing_requires = (['typing'] if sys.version_info < (3, 7) else [])
general_requires = ['pyrsistent']


def cppyy_deps(deps):
    return {"dependency_links": list(map(
        lambda dep: u'https://bitbucket.org/wlav/{}/get/master.tar.bz2'.format(str.lower(dep)), deps))} if "linux" in platform.system().lower() else {
        "requires":deps
    }


from distutils.command.install_headers import install_headers as install_headers_orig

import os
import re
import sys
import platform

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class install_headers(install_headers_orig):

    def run(self):
        headers = self.distribution.headers or []
        for header in headers:
            dst = os.path.join(self.install_dir, os.path.dirname(header))
            self.mkpath(dst)
            (out, _) = self.copy_file(header, dst)
            self.outfiles.append(out)


deps = cppyy_deps(['cppyy-backend', 'cppyy'])
general_requires = ['pyrsistent'] + deps.pop("requires", [])
dependency_links = deps.pop("dependency_links", [])

if cmake_build:
    print("Cmake build")
    from cmake_build import CMakeExtension, CMakeBuild
    b_ext = CMakeBuild
    e_mods=[CMakeExtension('couchbase-python-client')]
    general_requires+=['cmake']
else:
    print("Legacy build")
    b_ext = build_ext
    e_mods=[module]

import glob

exec_requires = typing_requires + general_requires
cxx_includes_base='cmake-build-release/libcouchbase-cxx-prefix/src/libcouchbase-cxx/include/'
cxx_includes = 'cmake-build-release/libcouchbase-cxx-prefix/src/libcouchbase-cxx/include/libcouchbase/'
cxx_includes_inner = os.path.join(cxx_includes, 'couchbase++')
all_headers = ([os.path.join(cxx_includes, 'couchbase++.h')] +
               glob.glob(os.path.join(cxx_includes_inner, "*.h")))

extoptions['include_dirs']+=[cxx_includes_base]

setup(
    name = 'couchbase',
    version = pkgversion,
    url="https://github.com/couchbase/couchbase-python-client",
    author="Couchbase, Inc.",
    author_email="PythonPackage@couchbase.com",
    license="Apache License 2.0",
    description="Python Client for Couchbase",
    long_description=open("README.rst", "r").read(),
    keywords=["couchbase", "nosql", "pycouchbase", "libcouchbase"],

    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "License :: OSI Approved :: Apache Software License",
        "Intended Audience :: Developers",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: Implementation :: CPython",
        "Topic :: Database",
        "Topic :: Software Development :: Libraries",
        "Topic :: Software Development :: Libraries :: Python Modules"],

    packages = [
        'acouchbase',
        'couchbase',
        'couchbase.views',
        'couchbase.iops',
        'couchbase.asynchronous',
        'couchbase.tests',
        'couchbase.tests.cases',
        'gcouchbase',
        'txcouchbase',
        'acouchbase',
    ] + ([
        'acouchbase.tests',
        'acouchbase.py34only'
    ] if sys.version_info >= (3, 4) else []),
    package_data=pkgdata,
    setup_requires=exec_requires + conan_and_cmake_deps,
    install_requires=exec_requires + pip_not_on_win_python_lt_3,
    tests_require=['nose', 'testresources>=0.2.7', 'basictracer==2.2.0'],
    test_suite='couchbase.tests.test_sync',
    **setup_kw
)
