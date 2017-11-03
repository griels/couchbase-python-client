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

from couchbase.tests.base import ConnectionTestCase, RealServerTestCase
from _functools import reduce
import jsonschema
import pprint
# For Python 2/3 compatibility
try:
    basestring
except NameError:
    basestring = str

class HealthTest(ConnectionTestCase):
    def setUp(self):
        super(HealthTest,self).setUp()
        self.skipUnlessMock()
    def test_health(self):
        import pprint
        result=self.cb.get_health()
        server_schema = { "type": "object",
                         "properties": { "details" : { "type": "string"},
                                        "latency" : { "type": "string"},
                                        "server" : { "type": "string"},
                                        "status" : { "type": "number"}
                             },
                         "required": ["details","latency","server","status"] }
        servers_schema = { "type" : "array",
                          "items": server_schema}
        health_schema = { "anyOf":[{
            "type" : "object",
            "properties" : {
                "n1ql" : servers_schema,
                "views" :servers_schema,
                "kv" :servers_schema }
            }]}
        jsonschema.validate(result['json']['services'],health_schema)



if __name__ == '__main__':
    unittest.main()
