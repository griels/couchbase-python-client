from setuptools import setup, Extension
import platform
import warnings
import os.path
import json

CPP_BUILD = os.getenv("PYCBC_CPP")

module = None
extoptions = {}
import re

with open("cbuild_cfg.json") as JSONFILE:
    BUILD_CFG = json.load(JSONFILE)

SOURCEMODS = list(filter(re.compile(r'^.*\.c$').match, BUILD_CFG.get('source', [])))
SOURCEMODS_CPP = list(filter(re.compile(r'^.*\.(cpp|cxx|cc)$').match, BUILD_CFG.get('source', [])))

if platform.python_implementation() != 'PyPy':
    extoptions['sources'] = list(map(str, SOURCEMODS))
    extoptions['sources'] += list(map(str, SOURCEMODS_CPP))
    print("sources are {}".format(extoptions['sources']))
    module = Extension('couchbase._libcouchbase', **extoptions)
else:
    warnings.warn('The C extension libary does not work on PyPy. '
                  'You should install the couchbase_ffi module. Installation of this '
                  'module will continue but will be unusable without couchbase_ffi')
