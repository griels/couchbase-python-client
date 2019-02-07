from setuptools import setup, Extension
import platform
import warnings
import os.path
module=None
extoptions = {}
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

if platform.python_implementation() != 'PyPy':
    extoptions['sources'] = [ os.path.join("src", m + ".c") for m in SOURCEMODS ]
    module = Extension('couchbase._libcouchbase', **extoptions)
else:
    warnings.warn('The C extension libary does not work on PyPy. '
                  'You should install the couchbase_ffi module. Installation of this '
                  'module will continue but will be unusable without couchbase_ffi')
