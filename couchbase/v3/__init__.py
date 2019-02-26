import pyrsistent
from couchbase import JSON
from couchbase.v3 import IResult
from couchbase.v3.bucket import Bucket
from couchbase.v3.mutate_in import *
from couchbase.v3.exceptions import *
from couchbase.v3.options import *
from couchbase.v3.result import IResult
from couchbase.v3.subdoc import *
from couchbase.v3.result import *
from couchbase.v3.readspec import *

class JSONDocument(pyrsistent.PRecord):
    def __init__(self,
                 parent=None,  # type: JSONDocument
                 **kwargs):
        super(JSONDocument, self).__init__(parent=parent, **kwargs)
        self.parent = parent

    def put(self, key, value):
        # type: (str, JSON) -> JSONDocument
        result = copy.copy(self)  # type: JSONDocument
        result.parent = self
        result.put(key, value)
        return result





# Query stuff
