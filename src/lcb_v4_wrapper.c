/**
 *     Copyright 2019 Couchbase, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include "lcb_v4_wrapper.h"
#include "pycbc.h"

// TODO: this should be implemented in LCB V4 API

lcb_STATUS pycbc_crypto_register(lcb_INSTANCE* instance, const char *name, lcbcrypto_PROVIDER *provider){
    (void)instance;
    (void)name;
    (void)provider;
    PYCBC_EXC_WRAP(LCB_ERRTYPE_INTERNAL,
                   LCB_ERRTYPE_INTERNAL,
                   "Not supported in V4 yet");
    return LCB_NOT_SUPPORTED;
}
