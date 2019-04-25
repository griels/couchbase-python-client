from setuptools import setup, Extension
import platform
import warnings
import os.path
import json
import re

CPP_BUILD = os.getenv("PYCBC_CPP")

module = None
extoptions = {}

with open("cbuild_cfg.json") as JSONFILE:
    BUILD_CFG = json.load(JSONFILE)

PYCBC_LCB_API = os.getenv("PYCBC_LCB_API", BUILD_CFG.get('comp_options', {}).get('PYCBC_LCB_API', '0x032000'))
all_sources = BUILD_CFG.get('source', []) + BUILD_CFG.get('apis', {}).get(PYCBC_LCB_API, [])
SOURCEMODS = list(filter(re.compile(r'^.*\.c$').match, all_sources))
SOURCEMODS_CPP = list(filter(re.compile(r'^.*\.(cpp|cxx|cc)$').match, all_sources))

if platform.python_implementation() != 'PyPy':
    extoptions['sources'] = list(map(str, SOURCEMODS))
    extoptions['sources'] += list(map(str, SOURCEMODS_CPP))
    print("sources are {}".format(extoptions['sources']))
    module = Extension('couchbase._libcouchbase', **extoptions)
else:
    warnings.warn('The C extension libary does not work on PyPy. '
                  'You should install the couchbase_ffi module. Installation of this '
                  'module will continue but will be unusable without couchbase_ffi')
