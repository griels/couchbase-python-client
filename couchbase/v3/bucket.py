import couchbase.bucket
from couchbase.v3 import *
from couchbase.v3.options import *
#from couchbase.v3.collection import Collection, CollectionOptions, forward_args


class BucketOptions(OptionBlock):
    pass


class Bucket(object):
    _bucket=None  # type: couchbase.bucket.Bucket

    @overload
    def __init__(self,
                 *options  # type: BucketOptions
                 ):
        # type: (...)->None
        pass

    def __init__(self,
                 *args,
                 **kwargs
                ):
        # type: (...)->None
        self._bucket=couchbase.bucket.Bucket(**forward_args(args,kwargs))

    def name(self):
        # type: (...)->str
        pass

    def scope(self,
              scopeName  # type: str
              ):
        # type: (...)->Scope
        pass

    def default_collection(self,
                           options=None  # type: CollectionOptions
                           ):
        # type: (...)->Collection
        return Collection(self)

    def collection(self,
                   collection_name,  # type: str
                   options=None  # type: CollectionOptions
                   ):
        # type: (...)->Collection
        return Collection(self, collection_name)

    def view_query(self,
                   design_doc,  # type: str
                   view_name,  # type: str
                   view_options=None):
        # type: (...)->IViewResult
        pass

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