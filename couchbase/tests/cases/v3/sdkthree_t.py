#
# Copyright 2018, Couchbase, Inc.
# All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# Copyright 2013, Couchbase, Inc.
# All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import tempfile
import os

from nose.plugins.attrib import attr

from couchbase.exceptions import (AuthError, ArgumentError,
                                  BucketNotFoundError, ConnectError,
                                  CouchbaseNetworkError,
                                  NotFoundError, InvalidError,
                                  TimeoutError)
from couchbase.tests.base import CouchbaseTestCase, SkipTest, ConnectionTestCase
from couchbase.connstr import ConnectionString
from couchbase import Couchbase
from couchbase.bucket import FluentBucket1
from couchbase.bucket import CPPYYBucket


class ConnectionTest(CouchbaseTestCase):

    def test_quiet(self):
        raise SkipTest("Fluent not functional yet")
        connparams = self.make_connargs()
        cb = self.factory(**connparams)
        self.assertRaises(NotFoundError, cb.get, 'missing_key')

        cb = self.factory(quiet=True, **connparams)
        cb = FluentBucket1(cb).get("fish")


class CPPYYtest(ConnectionTestCase):

    def test_quiet(self):
        connparams = self.make_connargs()
        cb = CPPYYBucket(**connparams)

        cb.upsert("foo", "test_string")
        result = cb.get("foo")
        self.assertEquals(result.str(), "test_string")
        del cb

if __name__ == '__main__':
    unittest.main()

