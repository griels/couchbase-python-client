import sys
import couchbase_v2


sys.modules['couchbase'] = couchbase_v2
from .mutate_in import *
from .exceptions import *
from .options import *
from .result import *
from .subdoc import *
from .result import *

# Query stuff
