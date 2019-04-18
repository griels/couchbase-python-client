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

#include "lcb_v4_backport.h"
#include "pycbc.h"

lcb_STATUS pycbc_crypto_register(lcb_INSTANCE *instance,
                                 const char *name,
                                 lcbcrypto_PROVIDER *provider)
{
    lcbcrypto_register(instance, name, provider);
    return LCB_SUCCESS;
}

lcb_STATUS pycbc_crypto_unregister(lcb_INSTANCE *instance, const char *name)
{
    lcbcrypto_unregister(instance, name);
    return LCB_SUCCESS;
}

lcb_STATUS pycbc_respsubdoc_status(const pycbc_SDENTRY *ent)
{
    return ent->status;
}

pycbc_strn_base_const pycbc_respsubdoc_value(const pycbc_SDENTRY *ent)
{
    pycbc_strn_base_const result;
    result.buffer = ent->value;
    result.length = ent->nvalue;
    return result;
}

int pycbc_sdresult_next(const lcb_RESPSUBDOC *resp,
                        pycbc_SDENTRY *dest,
                        size_t *index)
{
    return lcb_sdresult_next(resp, dest, index);
}

uint64_t pycbc_mutation_token_seqno(const lcb_MUTATION_TOKEN *pToken)
{
    return LCB_MUTATION_TOKEN_SEQ(pToken);
}

uint64_t pycbc_mutation_token_vbid(const lcb_MUTATION_TOKEN *pToken)
{
    return LCB_MUTATION_TOKEN_VB(pToken);
}

uint64_t pycbc_mutation_token_uuid(const lcb_MUTATION_TOKEN *pToken)
{
    return LCB_MUTATION_TOKEN_ID(pToken);
}

int lcb_mutation_token_is_valid(const lcb_MUTATION_TOKEN *pTOKEN)
{
    return LCB_MUTATION_TOKEN_ISVALID(pTOKEN);
}

PYCBC_X_FOR_EACH_OP_POSTFIX(, PYCBC_IMPL, PYCBC_IMPL)

lcb_STATUS lcb_cmdget_key(lcb_CMDBASE *ctx, pycbc_pybuffer *buf)
{
    LCB_CMD_SET_KEY(ctx, buf->buffer, buf->length);
    return LCB_SUCCESS;
}

GET_ATTRIBS(PYCBC_SCOPE_SET)

void lcb_cmdgetreplica_create(lcb_CMDGETREPLICA **pcmd, int strategy)
{
#if PYCBC_LCB_API > 0x030000
    switch (strategy) {
    case LCB_REPLICA_MODE_ANY:
        lcb_cmdgetreplica_create_first(pcmd);
        break;
    case LCB_REPLICA_MODE_ALL:
        lcb_cmdgetreplica_create_all(pcmd);
        break;

    case LCB_REPLICA_MODE_IDX0:
    case LCB_REPLICA_MODE_IDX1:
    case LCB_REPLICA_MODE_IDX2:
    default:
        lcb_cmdgetreplica_create_select(pcmd, strategy - LCB_REPLICA_MODE_IDX0);
        break;
    }
#else
    (*pcmd)->strategy = strategy;
    switch (strategy) {
    case LCB_REPLICA_MODE_ANY:
        (*pcmd)->strategy = LCB_REPLICA_FIRST;
        break;
    case LCB_REPLICA_MODE_ALL:
        (*pcmd)->strategy = LCB_REPLICA_ALL;
        break;
    case LCB_REPLICA_MODE_IDX0:
        (*pcmd)->strategy = LCB_REPLICA_SELECT;
        (*pcmd)->index = 0;
        break;
    case LCB_REPLICA_MODE_IDX1:
        (*pcmd)->strategy = LCB_REPLICA_SELECT;
        (*pcmd)->index = 1;
        break;
    case LCB_REPLICA_MODE_IDX2:
        (*pcmd)->strategy = LCB_REPLICA_SELECT;
        (*pcmd)->index = 2;
        break;
    default:
        break;
    }

#endif
}
#include "pycbc_subdocops.h"
LIBCOUCHBASE_API lcb_STATUS lcb_subdocops_create(lcb_SUBDOCOPS **operations,
                                                 size_t capacity)
{
    lcb_SUBDOCOPS *res = (lcb_SUBDOCOPS *)calloc(1, sizeof(lcb_SUBDOCOPS));
    res->nspecs = capacity;
    res->specs = (lcb_SDSPEC *)calloc(res->nspecs, sizeof(lcb_SDSPEC));
    *operations = res;
    return LCB_SUCCESS;
}
LIBCOUCHBASE_API lcb_STATUS
lcb_cmdsubdoc_operations(lcb_CMDSUBDOC *cmd, const lcb_SUBDOCOPS *operations)
{
    // cmd->cmdflags |= operations->options;
    cmd->specs = operations->specs;
    cmd->nspecs = operations->nspecs;
#ifdef PYCBC_DEBUG_SUBDOC
    for (size_t i = 0; i < cmd->nspecs; ++i) {
                PYCBC_DEBUG_LOG("Command %d: {.cmd=%d, .options=%d, path=%.*s,
                value=%.*s
    } ",i, operations->specs[i].sdcmd,
                operations->specs[i].options,
                operations->specs[i].path.contig.nbytes,
                operations->specs[i].path.contig.bytes,
                operations->specs[i].value.u_buf.contig.nbytes,
                operations->specs[i].value.u_buf.contig.bytes)
}
#endif
return LCB_SUCCESS;
}

LIBCOUCHBASE_API lcb_STATUS lcb_subdocops_destroy(lcb_SUBDOCOPS *operations)
{
    if (operations) {
        if (operations->specs) {
            size_t ii;
            for (ii = 0; ii < operations->nspecs; ii++) {
                if (operations->specs[ii].sdcmd == LCB_SDCMD_COUNTER) {
                    free((void *)operations->specs[ii]
                                 .value.u_buf.contig.bytes);
                }
            }
        }
        free(operations->specs);
    }
    free(operations);
    return LCB_SUCCESS;
}

PYCBC_X_SD_OPS(PYCBC_SDCMD_CASE,
               PYCBC_SDCMD_CASE_NP,
               PYCBC_SDCMD_CASE_VAL,
               PYCBC_SDCMD_CASE_MVAL,
               PYCBC_SDCMD_CASE_COUNTER,
               LITERAL,
               LITERAL)
