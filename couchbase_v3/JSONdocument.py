import copy

import pyrsistent
import json


class JSONDocument(object):
    record=None  # type: pyrsistent.PRecord

    def __init__(self,parent=None,
                 **kwargs):
        self.record=parent.record if parent else pyrsistent.PRecord()#**kwargs)
        for k,v in kwargs.items():
            self.record=self.record.set(k,v)

    def put(self, key, value):
        # type: (str, JSON) -> JSONDocument
        return JSONDocument(parent=self,**{key:value})

    @classmethod
    def decode(cls,input):
        result=cls()
        result.record=pyrsistent.freeze(input)
        return result