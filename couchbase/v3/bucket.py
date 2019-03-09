from couchbase.bucket import Bucket as SDK2Bucket
from couchbase.v3.collection import Collection as SDK3Collection, CollectionOptions
from couchbase.v3.options import *
import couchbase.v3.collection as cb_coll
from couchbase.v3.views import IViewResult


class BucketOptions(OptionBlock):
    pass


class Bucket(object):
    _bucket=None  # type: SDK2Bucket
    from couchbase.v3.collection import Scope

    @overload
    def __init__(self,name,
                 *options  # type: BucketOptions
                 ):
        # type: (...)->None
        pass

    def __init__(self,
                 name,
                 *args,
                 **kwargs
                ):
        # type: (...)->None
        self._name=name
        self._bucket=SDK2Bucket(name,**forward_args(kwargs,args))

    @property
    def name(self):
        # type: (...)->str
        return self._name

    def scope(self,
              scope_name  # type: str
              ):
        # type: (...)->Scope
        return cb_coll.Scope(scope_name)

    def default_collection(self,
                           options=None  # type: CollectionOptions
                           ):
        # type: (...)->SDK3Collection
        return SDK3Collection(self)

    def collection(self,
                   collection_name,  # type: str
                   options=None  # type: CollectionOptions
                   ):
        # type: (...)->SDK3Collection
        return SDK3Collection(self, collection_name)

    def view_query(self,
                   design_doc,  # type: str
                   view_name,  # type: str
                   *view_options # type: ViewOptions
                   ):
        # type: (...)->IViewResult
        cb=self._bucket # type: SDK2Bucket
        res=cb.query(design_doc, view_name, **forward_args(None,view_options))
        return IViewResult(res)

    def spatial_view_query(self,
                           design_doc,  # type: str
                           view_name,  # type: str
                           options=None  # type: ViewOptions
                           ):
        # type: (...)->ISpatialViewResult
        pass

    def views(self):
        # type: (...)->IViewManager
        pass

    def ping(self,
             options=None  # type: PingOptions
             ):
        # type: (...)->IPingResult
        pass