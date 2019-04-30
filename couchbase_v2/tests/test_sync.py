from couchbase_v2.bucket import Bucket
from couchbase_v2.views.iterator import View
from couchbase_v2.tests.base import ApiImplementationMixin
from couchbase_v2.tests.importer import get_configured_classes

class SyncImplMixin(ApiImplementationMixin):
    factory = Bucket
    viewfactory = View
    should_check_refcount = True

configured_cases = get_configured_classes(SyncImplMixin)
globals().update(configured_cases)
