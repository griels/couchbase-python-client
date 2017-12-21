#
# Copyright 2017, Couchbase, Inc.
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

from couchbase.tests.base import ConnectionTestCase
import jsonschema

# For Python 2/3 compatibility
try:
    basestring
except NameError:
    basestring = str

server_schema = {"type": "object",
                 "properties": {"details": {"type": "string"},
                                "latency": {"anyOf": [{"type": "number"}, {"type": "string"}]},
                                "server": {"type": "string"},
                                "status": {"type": "number"}
                                },
                 "required": ["details", "latency", "server", "status"]}

servers_schema = {"type": "array",
                  "items": server_schema}


def gen_schema(name):
    return {"type": "object",
            "properties": {name: servers_schema},
            "required": [name]
            }


services_schema = {"anyOf":
                       [gen_schema(name) for name in ["n1ql", "views", "fts", "kv"]]
                   }

ping_schema = {"anyOf": [{
    "type": "object",
    "properties": {
        "services": services_schema
    },
    "required": ["services"]
}]}
server_and_port_schema = {"type": "string",
                          "pattern": "([0-9]{1,3}\.){3,3}[0-9]{1,3}:[0-9]+"}
connection_status_schema = {"type": "string",
                            "pattern": "connected"}
config_schema = {"type": "array",
                 "items": {"type": "object",
                 "properties": {
                     "id": {"type": "string"},
                     "last_activity_us": {"type": "number"},
                     "local": server_and_port_schema,
                     "remote": server_and_port_schema,
                     "status": connection_status_schema
                 }}}

python_id="PYCBC"

client_id_schema = {"type": "string",
                    "pattern": "^0x[a-f0-9]+/"+python_id}

three_part_ver_num = "([0-9]+\.)+[0-9]+"
version_id="2.3.2.dev4+gd5a6d8c"

import re

sdk_schema = {"type": "string",
              "pattern": "libcouchbase" +
                         re.escape("/") + three_part_ver_num + "_[0-9]+_(.*?)" +
                         re.escape(python_id + "/") +
                         three_part_ver_num + "\.[^\s]*"}

sdk_schema = {"type": "string",
              "pattern": "libcouchbase" +
                         re.escape("/") + three_part_ver_num + "_[0-9]+_(.*?)" +
                         re.escape(python_id + "/") +
                         three_part_ver_num + "\.[^\s]*"}

diagnostics_schema = {"type": "object",
                      "properties": {
                          "config": config_schema,
                          "id": client_id_schema,
                          "sdk": sdk_schema,
                          "version": {"type": "number"}

                      }}


class HealthTest(ConnectionTestCase):

    def setUp(self):
        super(HealthTest, self).setUp()
        self.skipUnlessMock()

    def test_ping(self):
        result = self.cb.ping()
        jsonschema.validate(result, services_schema)

    def test_diagnostics(self):
        result = self.cb.diagnostics()
        jsonschema.validate(result, diagnostics_schema)


if __name__ == '__main__':
    unittest.main()
