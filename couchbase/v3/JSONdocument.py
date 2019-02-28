import copy

import pyrsistent


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