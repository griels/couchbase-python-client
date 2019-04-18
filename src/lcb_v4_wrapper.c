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
#include <libcouchbase/crypto.h>

// TODO: these crypto functions should be implemented in LCB V4 API - inaccessible to end user at the moment

lcb_STATUS pycbc_crypto_register(lcb_INSTANCE* instance, const char *name, lcbcrypto_PROVIDER *provider){
    (void)instance;
    (void)name;
    (void)provider;
    PYCBC_EXC_WRAP(LCB_ERRTYPE_INTERNAL,
                   LCB_ERRTYPE_INTERNAL,
                   "Not supported in V4 yet");
    return LCB_NOT_SUPPORTED;
}

lcb_STATUS pycbc_crypto_unregister(lcb_INSTANCE* instance, const char *name){
    (void)instance;
    (void)name;
    PYCBC_EXC_WRAP(LCB_ERRTYPE_INTERNAL,
                   LCB_ERRTYPE_INTERNAL,
                   "Not supported in V4 yet");
    return LCB_NOT_SUPPORTED;
}


lcb_STATUS pycbc_respsubdoc_status(const pycbc_SDENTRY *ent)
{
    return lcb_respsubdoc_result_status(ent->resp, ent->index);
}

pycbc_strn_base_const pycbc_respsubdoc_value(const pycbc_SDENTRY *ent)
{
    pycbc_strn_base_const result;
    lcb_respsubdoc_result_value(
            ent->resp, ent->index, &result.buffer, &result.length);
    return result;
}


int pycbc_sdresult_next(const lcb_RESPSUBDOC *resp,
                        pycbc_SDENTRY *dest,
                        size_t *index)
{
    if (((*index) + 1) > lcb_respsubdoc_result_size(resp)) {
        return 0;
    }
    *dest = (pycbc_SDENTRY){.resp = resp, .index = *index};
    ++(*index);
    return 1;
}

uint64_t pycbc_mutation_token_seqno(const lcb_MUTATION_TOKEN *pToken)
{
    return pToken->seqno_;
}

uint64_t pycbc_mutation_token_vbid(const lcb_MUTATION_TOKEN *pToken)
{
    return pToken->vbid_;
}

uint64_t pycbc_mutation_token_uuid(const lcb_MUTATION_TOKEN *pToken)
{
    return pToken->uuid_;
}

lcb_STATUS pycbc_cmdn1ql_multiauth(lcb_CMDN1QL* cmd, int enable) {
    (void)cmd;
    (void)enable;
    return LCB_NOT_SUPPORTED;
}

lcb_STATUS pycbc_cmdanalytics_host(lcb_CMDANALYTICS* CMD, const char* host)
{
    return LCB_NOT_SUPPORTED;
}
