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

#ifndef COUCHBASE_PYTHON_CLIENT_LCB_V4_BACKPORT_H
#define COUCHBASE_PYTHON_CLIENT_LCB_V4_BACKPORT_H

#include <libcouchbase/api3.h>
#include <libcouchbase/subdoc.h>
#include <libcouchbase/couchbase.h>
#include <libcouchbase/ixmgmt.h>
#include <libcouchbase/n1ql.h>
#include <libcouchbase/views.h>
#include <libcouchbase/views.h>
#include "libcouchbase/cbft.h"

#define PYCBC_ENDURE 1

#define pycbc_RESPGET_USE(X) &temp
#define pycbc_RESPGET_DEFINE(X) LENGTHTYPE_DEFINE(X)
#define pycbc_RESPGET_RET(X) X
#define lcb_cmdping_destroy(CMD)
#define lcb_cmddiag_create(CMD) \
    lcb_CMDDIAG cmd_real = {0}; \
    *(CMD) = &cmd_real;
#define lcb_cmddiag_destroy(CMD)
#define lcb_cmdstats_create(DEST) \
    lcb_CMDSTATS cmd_real = {0};  \
    *(DEST) = &cmd_real;
#define lcb_cmdstats_destroy(DEST) 0
#define lcb_cmdget_expiration(cmd, time) cmd->exptime = time;
#define lcb_cmdget_timeout(cmd, time) cmd->exptime = time;
#define lcb_cmdtouch_create(CMD) \
    lcb_CMDTOUCH cmd_real = {0}; \
    *(CMD) = &cmd_real;
#define lcb_cmdtouch_destroy(CMD) 0
#define lcb_cmdtouch_expiration(cmd, time) cmd->exptime = time;
#define lcb_cmdtouch_timeout(cmd, time) cmd->exptime = time;
#define lcb_cmdrget_expiration(cmd, time) cmd->exptime = time;
#define lcb_cmdcounter_delta(cmd, x) (cmd)->delta = x;
#define lcb_cmdcounter_initial(cmd, x) \
    (cmd)->initial = x;                \
    (cmd)->create = 1;
#define lcb_cmdcounter_timeout(cmd, x) (cmd)->exptime = x;
#define lcb_cmdcounter_expiration(cmd, x) (cmd)->exptime = x
#define lcb_cmdstore_flags(CMD, VAL) cmd->flags = VAL;
#define lcb_cmdgetreplica_create_all(ptr) (*(ptr))->strategy = LCB_REPLICA_ALL
#define lcb_cmdgetreplica_create_select(ptr, INDEX) \
    (*(ptr))->strategy = LCB_REPLICA_SELECT;        \
    (*(ptr))->index = INDEX;
#define lcb_cmdgetreplica_create_first(ptr) \
    (*(ptr))->strategy = LCB_REPLICA_FIRST
#define lcb_cmdgetreplica_expiration(CMD, TTL) (CMD)->exptime = TTL
#define lcb_cmdstore_create(CMD, OP) \
    PYCBC_ASSIGN((CMD)->operation, (lcb_storage_t)(OP));
#define lcb_cmdstore_destroy(...) 0
#define lcb_cmdstore_cas(CMD, CAS) PYCBC_ASSIGN((CMD)->cas, CAS);
#define lcb_cmdstore_expiration(CMD, TTL) \
    PYCBC_ASSIGN((CMD)->exptime, (lcb_U32)(TTL));
#define lcb_cmdsubdoc_cas(CMD, CAS) (CMD)->cas = (lcb_U32)(CAS)
#define lcb_cmdsubdoc_expiration(CMD, EXP) (CMD)->exptime = (lcb_U32)(EXP)
#define lcb_resphttp_headers(htresp, dest) *(dest) = htresp->headers
#define lcb_resphttp_http_status(resp, dest) *(dest) = (resp)->htstatus
#define lcb_resphttp_body(resp, bodybuffer, bodylength) \
    *(bodybuffer) = resp->body;                         \
    (*bodylength) = resp->nbody;
#define lcb_respn1ql_http_response(INNER, DEST) *(DEST) = INNER->htresp;
#define lcb_respn1ql_row(INNER, ROW, ROW_COUNT) \
    {                                           \
        *(ROW) = (INNER)->row;                  \
        *(ROW_COUNT) = (INNER)->nrow;           \
    }
#define lcb_respn1ql_cookie(RESP, DEST) *(DEST) = (RESP)->cookie;
#define lcb_respn1ql_is_final(RESP) (RESP)->rflags &LCB_RESP_F_FINAL
#define lcb_respn1ql_status(RESP) (RESP)->rc
#define lcb_cmdn1ql_create(CMD) \
    lcb_CMDN1QL cmd_real = {0}; \
    cmd = &cmd_real;
#define lcb_cmdn1ql_callback(CMD, CALLBACK) (CMD)->callback = (CALLBACK)
#define lcb_cmdn1ql_query(CMD, PARAMS, NPARAMS) \
    (CMD)->query = PARAMS;                      \
    (CMD)->nquery = NPARAMS;
#define lcb_cmdn1ql_handle(CMD, HANDLE) (CMD)->handle = HANDLE
#define lcb_cmdn1ql_adhoc(CMD, ENABLE)               \
    ((CMD)->cmdflags) &= (~LCB_CMDN1QL_F_PREPCACHE); \
    ((CMD)->cmdflags) |= (ENABLE ? LCB_CMDN1QL_F_PREPCACHE : 0);
#define lcb_cmdn1ql_parent_span(...) lcb_n1ql_set_parent_span(__VA_ARGS__)
#define lcb_n1ql(INSTANCE, COOKIE, QUERY) \
    lcb_n1ql_query(INSTANCE, COOKIE, QUERY)
#define lcb_respview_document(CTX, DEST) *(DEST) = CTX->docresp
#define lcb_respview_key(CTX, DEST, NDEST) \
    *(DEST) = (CTX)->key;                  \
    *(NDEST) = (CTX)->nkey;
#define lcb_respview_geometry(CTX, DEST, NDEST) \
    *(DEST) = (CTX)->geometry;                  \
    *(NDEST) = (CTX)->ngeometry;
#define lcb_respview_row(CTX, DEST, NDEST) \
    *(DEST) = (CTX)->value;                \
    *(NDEST) = (CTX)->nvalue;
#define lcb_respview_doc_id(RESP, DOCID, NDOCID) \
    *(DOCID) = (RESP)->docid;                    \
    *(NDOCID) = (RESP)->ndocid;
#define lcb_http_cancel(instance, req)       \
            lcb_cancel_http_request(instance, req)
#define lcb_cmdhttp_create(CMD, TYPE) CMD->type = TYPE;
#define lcb_cmdhttp_body(HTCMD, BODY, NBODY) \
    HTCMD->body = BODY;                      \
    HTCMD->nbody = NBODY;
#define lcb_cmdhttp_content_type(HTCMD, CTYPE, CTYPELEN) \
    HTCMD->content_type = CTYPE;                         \
    PYCBC_DUMMY(HTCMD->ncontent_type = CTYPELEN;)
#define lcb_cmdhttp_method(HTCMD, METHOD) HTCMD->method = METHOD;
#define lcb_cmdhttp_handle(HTCMD, HANDLE) HTCMD->reqhandle = HANDLE;
#define lcb_respview_cookie(RESP, DEST) *(DEST) = (RESP)->cookie
#define lcb_respview_is_final(RESP) (RESP)->rflags &LCB_RESP_F_FINAL
#define lcb_respview_status(RESP) (RESP)->rc
#define lcb_cmdview_create(DEST) \
    lcb_CMDVIEW cmd_real = {0};  \
    *(DEST) = &cmd_real;
#define lcb_cmdview_design_document(VCMD, DESIGN, NDESIGN) \
    (VCMD)->ddoc = DESIGN;                                 \
    (VCMD)->nddoc = NDESIGN;
#define lcb_cmdview_view_name(VCMD, VIEW, NVIEW) \
    (VCMD)->view = VIEW;                         \
    (VCMD)->nview = NVIEW;
#define lcb_cmdview_option_string(VCMD, OPTSTR, NOPTSTR) \
    (VCMD)->optstr = OPTSTR;                             \
    (VCMD)->noptstr = NOPTSTR
#define lcb_cmdview_post_data(VCMD, BODY, NBODY) \
    (VCMD)->postdata = BODY;                     \
    (VCMD)->npostdata = NBODY
#define lcb_cmdview_handle(VCMD, HANDLE) (VCMD)->handle = HANDLE
#define lcb_cmdview_callback(VCMD, CALLBACK) (VCMD)->callback = CALLBACK;
#define lcb_cmdview_include_docs(VCMD, ENABLE)                                 \
    (VCMD)->cmdflags = ((VCMD)->cmdflags & ~LCB_CMDVIEWQUERY_F_INCLUDE_DOCS) | \
                       (ENABLE ? LCB_CMDVIEWQUERY_F_INCLUDE_DOCS : 0)
#define lcb_cmdview_no_row_parse(VCMD, ENABLE)                               \
    (VCMD)->cmdflags = ((VCMD)->cmdflags & ~LCB_CMDVIEWQUERY_F_NOROWPARSE) | \
                       (ENABLE ? LCB_CMDVIEWQUERY_F_NOROWPARSE : 0)
#define lcb_cmdview_spatial(VCMD, ENABLE)                                 \
    (VCMD)->cmdflags = ((VCMD)->cmdflags & ~LCB_CMDVIEWQUERY_F_SPATIAL) | \
                       (ENABLE ? LCB_CMDVIEWQUERY_F_SPATIAL : 0)
#define lcb_cmdview_parent_span(...) lcb_view_set_parent_span(__VA_ARGS__)
#define lcb_cmdview_destroy(CMD)
#define lcb_cmdsubdoc_create(CMD) \
    lcb_CMDSUBDOC cmd_real = {0}; \
    *(CMD) = &cmd_real;
#define lcb_cmdsubdoc_destroy(CMD)
#define lcb_cmdsubdoc_key(CMD, KEY, NKEY) LCB_CMD_SET_KEY(CMD, KEY, NKEY)
#define lcb_respgetcid_cookie(RESP, DEST) *(DEST) = (RESP)->cookie;
#define lcb_respgetcid_status(RESP) (RESP)->rc
#define lcb_respgetcid_collection_id(RESP, DEST) *(DEST) = (RESP)->collection_id
#define lcb_respgetcid_manifest_id(RESP, DEST) *(DEST) = (RESP)->manifest_id
#define lcb_cmdsubdoc_parent_span(CMD, SPAN) \
    LCB_CMD_SET_TRACESPAN((CMD), (SPAN));
#define lcb_cmdobserve_parent_span(CMD, SPAN) \
    LCB_CMD_SET_TRACESPAN((CMD), (SPAN));
#define lcb_cmdendure_parent_span(CMD, ...) \
    LCB_CMD_SET_TRACESPAN((CMD), __VA_ARGS__);
#define lcb_cmdfts_parent_span(...) lcb_fts_set_parent_span(__VA_ARGS__)
#define lcb_cmdping_create(CMD) \
    lcb_CMDPING cmd_real = {0}; \
    *(CMD) = &cmd_real;

#define lcb_respping_cookie(RESP, DEST) *(DEST) = (RESP)->cookie;
#define lcb_respping_status(RESP) (RESP)->rc
#define lcb_respping_result_size(RESP) resp->nservices
#define lcb_respping_result_service(RESP, INDEX, DEST) \
    *(DEST) = (RESP)->services[INDEX].type
#define lcb_respping_result_latency(RESP, INDEX, DEST) \
    *(DEST) = (RESP)->services[INDEX].latency
#define lcb_respping_result_status(RESP, INDEX) (RESP)->services[INDEX].rc
#define lcb_respping_result_remote(RESP, INDEX, BUFFER, NBUFFER) \
    *(BUFFER) = (RESP)->services[INDEX].server;                  \
    *(NBUFFER) = strlen((RESP)->services[INDEX].server);
#define lcb_respping_value(RESP, JSON, NJSON) \
    *(JSON) = resp->json;                     \
    *(NJSON) = resp->njson;

#define LCB_PING_SERVICE_KV LCB_PINGSVC_KV
#define LCB_PING_SERVICE_VIEWS LCB_PINGSVC_VIEWS
#define LCB_PING_SERVICE_N1QL LCB_PINGSVC_N1QL
#define LCB_PING_SERVICE_FTS LCB_PINGSVC_FTS
#define LCB_PING_SERVICE_ANALYTICS LCB_PINGSVC_ANALYTICS
#define LCB_PING_SERVICE__MAX LCB_PINGSVC__MAX
#define PYCBC_CMD_SET_KEY_SCOPE(SCOPE, CMD, KEY)                    \
    PYCBC_DEBUG_LOG("Setting key %.*s", (KEY).length, (KEY).buffer) \
    LCB_CMD_SET_KEY(CMD, (KEY).buffer, (KEY).length)
#define PYCBC_CMD_SET_VALUE_SCOPE(SCOPE, CMD, KEY)                    \
    PYCBC_DEBUG_LOG("Setting value %.*s", (KEY).length, (KEY).buffer) \
    LCB_CMD_SET_VALUE(CMD, (KEY).buffer, (KEY).length)
#define PYCBC_get_ATTR(CMD, attrib, ...) CMD->attrib = __VA_ARGS__;
#define PYCBC_touch_ATTR(CMD, attrib, ...) CMD->attrib = __VA_ARGS__;
#define PYCBC_getreplica_ATTR(CMD, attrib, ...) CMD->attrib = __VA_ARGS__;
#define PYCBC_unlock_ATTR(CMD, attrib, ...) CMD->attrib = __VA_ARGS__;
#define PYCBC_remove_ATTR(CMD, attrib, ...) CMD->attrib = __VA_ARGS__;
#define PYCBC_endure_ATTR(CMD, attrib, ...) CMD->attrib = __VA_ARGS__;
#define PYCBC_ASSIGN(LHS, RHS)                                    \
    PYCBC_DEBUG_LOG_CONTEXT(                                      \
            context, "Assigning %s (%d) to %s", #RHS, RHS, #LHS); \
    LHS = RHS;
#define pycbc_resphttp_cookie(resp, type, target) \
    (*((type *)(target))) = resp->cookie;
#define PYCBC_RESP_GET(SCOPE, UCSCOPE, ATTRIB, TYPE) \
    PYCBC_SCOPE_GET(SCOPE, const lcb_RESP##UCSCOPE *, ATTRIB, TYPE);
#define pycbc_verb(VERB, INSTANCE, ...) \
    pycbc_verb_postfix(3, VERB, INSTANCE, __VA_ARGS__)
#define pycbc_rget(INSTANCE, COOKIE, CMD) \
    pycbc_verb_postfix(3, rget, (INSTANCE), (COOKIE), (CMD))
typedef lcb_DURABILITYLEVEL pycbc_DURABILITY_LEVEL;
typedef lcb_error_t lcb_STATUS;
typedef const lcb_RESPGET *pycbc_RESPGET;
typedef lcb_PINGSVCTYPE lcb_PING_SERVICE;
typedef lcb_RESPGET lcb_RESPGETREPLICA;
typedef struct lcb_st lcb_INSTANCE;
typedef struct {
    lcb_SDSPEC *specs;
    size_t nspecs;
    lcb_U32 options;
} lcb_SUBDOCOPS;

typedef enum {
    LCB_REPLICA_MODE_ANY = 0x00,
    LCB_REPLICA_MODE_ALL = 0x01,
    LCB_REPLICA_MODE_IDX0 = 0x02,
    LCB_REPLICA_MODE_IDX1 = 0x03,
    LCB_REPLICA_MODE_IDX2 = 0x04,
    LCB_REPLICA_MODE__MAX
} lcb_REPLICA_MODE;
typedef lcb_CMDBASE *pycbc_CMDBASE;
typedef lcb_CMDGET *pycbc_CMDGET;
typedef lcb_CMDTOUCH *pycbc_CMDTOUCH;
typedef lcb_CMDGETREPLICA *pycbc_CMDGETREPLICA;
typedef lcb_CMDREMOVE *pycbc_CMDREMOVE;
typedef lcb_CMDUNLOCK *pycbc_CMDUNLOCK;
typedef lcb_CMDENDURE *pycbc_CMDENDURE;
typedef lcb_CMDHTTP *pycbc_CMDHTTP;
typedef lcb_CMDSTORE *pycbc_CMDSTORE;

typedef lcb_SDSPEC pycbc_SDSPEC;

typedef lcb_VIEWHANDLE pycbc_VIEW_HANDLE;
typedef lcb_http_request_t pycbc_HTTP_HANDLE;
typedef lcb_FTSHANDLE pycbc_FTS_HANDLE;
typedef lcb_N1QLHANDLE pycbc_N1QL_HANDLE;

int lcb_mutation_token_is_valid(const lcb_MUTATION_TOKEN *pTOKEN);

struct pycbc_pybuffer_real;
lcb_STATUS lcb_cmdget_key(lcb_CMDBASE *ctx, struct pycbc_pybuffer_real *buf);


void lcb_cmdgetreplica_create(lcb_CMDGETREPLICA **pcmd, int strategy);

uint64_t pycbc_mutation_token_seqno(const struct lcb_MUTATION_TOKEN *pToken);

uint64_t pycbc_mutation_token_vbid(const struct lcb_MUTATION_TOKEN *pToken);

uint64_t pycbc_mutation_token_uuid(const struct lcb_MUTATION_TOKEN *pToken);



#define PYCBC_OBSERVE_STANDALONE

#define DEFAULT_VERBPOSTFIX 3

#define CMDSCOPE_GENERIC(UC, LC, CC, ...)            \
    {                                                \
        lcb_CMD##UC cmd_real = {0};                  \
        lcb_CMD##UC *cmd = &cmd_real;                \
        CC(UC, LC, cmd);                             \
        int cmd_done = 0;                            \
        __VA_ARGS__;                                 \
        goto GT_##UC##_DONE;                         \
        GT_##UC##_DONE : if (cmd_done) goto GT_DONE; \
    }
#define CMDSCOPE(UC, LC, ...) CMDSCOPE_GENERIC(UC, LC, PYCBC_DUMMY, __VA_ARGS__)
#define CMDSCOPE_FAIL(UC)    \
    {                        \
        cmd_done = 1;        \
        goto GT_##UC##_DONE; \
    };
#define CMDSCOPE_CREATECMD_RAW(UC, LC, CMD, ...) \
    CMDSCOPE_CREATECMD_RAW_V3(UC, LC, CMD, __VA_ARGS__)

#define CMDSCOPE_CREATECMD(UC, LC, CMD, ...) \
    CMDSCOPE_CREATECMD_V3(UC, LC, CMD, __VA_ARGS__)

#define CMDSCOPE_DESTROYCMD(UC, LC, CMD, ...) \
    CMDSCOPE_DESTROYCMD_V3(UC, LC, CMD, __VA_ARGS__)

#define CMDSCOPE_DESTROYCMD_RAW(UC, LC, CMD, ...) \
    CMDSCOPE_DESTROYCMD_RAW_V3(UC, LC, CMD, __VA_ARGS__)

#define PYCBC_CMD_SET_TRACESPAN(TYPE, CMD, SPAN) \
    LCB_CMD_SET_TRACESPAN((CMD), (SPAN));

#define GENERIC_SPAN_OPERAND(SCOPE, INSTANCE, CMD, HANDLE, CONTEXT) \
    lcb_cmd##SCOPE##_parent_span(INSTANCE, HANDLE, (CONTEXT)->span)
#define GENERIC_NULL_OPERAND(SCOPE, COMMAND, INSTANCE, HANDLE, CONTEXT, ...) \
    lcb_##SCOPE(INSTANCE, __VA_ARGS__);

#define PYCBC_LOG_KEY(CMD, key)                     \
    PYCBC_DEBUG_LOG("setting trace span on %.*s\n", \
                    (int)(CMD)->key.contig.nbytes,  \
                    (const char *)(CMD)->key.contig.bytes);

#if LCB_VERSION > 0x020807
#    define PYCBC_CRYPTO_VERSION 1
#else
#    define PYCBC_CRYPTO_VERSION 0
#endif

#define GET_ATTRIBS(X) X(get, lcb_CMDGET *, locktime, lock, int);


typedef lcb_PINGSVCTYPE lcb_PING_SERVICE;
typedef lcb_RESPVIEWQUERY lcb_RESPVIEW;
typedef lcb_CMDVIEWQUERY lcb_CMDVIEW;

#    define PYCBC_SCOPE_GET_DECL(SCOPE, CTXTYPE, ATTRIB, TYPE) \
        TYPE pycbc_cmd##SCOPE##_##ATTRIB(const CTXTYPE ctx)

#    define PYCBC_SCOPE_SET_DECL(SCOPE, CTXTYPE, ATTRIB, MEMBER, TYPE) \
        lcb_STATUS lcb_cmd##SCOPE##_##ATTRIB(CTXTYPE ctx, TYPE value)

#    define PYCBC_SCOPE_SET(SCOPE, CTXTYPE, ATTRIB, MEMBER, TYPE)     \
        lcb_STATUS lcb_cmd##SCOPE##_##ATTRIB(CTXTYPE ctx, TYPE value) \
        {                                                             \
            ctx->MEMBER = value;                                      \
            return LCB_SUCCESS;                                       \
        }

#    define PYCBC_SCOPE_GET(SCOPE, CTXTYPE, ATTRIB, TYPE) \
        TYPE pycbc_##SCOPE##_##ATTRIB(const CTXTYPE ctx)  \
        {                                                 \
            return ctx->ATTRIB;                           \
        }

GET_ATTRIBS(PYCBC_SCOPE_SET_DECL);

#    define LCB_STORE_WRAPPER(b) ADD_MACRO(LCB_STORE_##b);

enum {
#    define PYCBC_BACKPORT_STORE(X) LCB_STORE_##X = LCB_##X
#    define ALL_ENUMS(X) X(APPEND), X(PREPEND), X(SET), X(ADD), X(REPLACE)
    ALL_ENUMS(PYCBC_BACKPORT_STORE)
};


#define PYCBC_RESP_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
    PYCBC_KEY_ACCESSORS##POSTFIX(UC, LC) PYCBC_NOKEY_ACCESSORS##POSTFIX(UC, LC)

#define PYCBC_GET_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
    PYCBC_RESP_ACCESSORS##POSTFIX(UC, LC);           \
    PYCBC_VAL_ACCESSORS##POSTFIX(UC, LC);            \
    PYCBC_ITMFLAGS_ACCESSORS_U32##POSTFIX(UC, LC)

#define PYCBC_HTTP_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
    PYCBC_RESP_ACCESSORS##POSTFIX(UC, LC);            \
    PYCBC_FLAGS_ACCESSORS_U32##POSTFIX(UC, LC)        \
            PYCBC_HOST_ACCESSORS##POSTFIX(UC, LC)
#define PYCBC_STATS_ACCESSORS_POSTFIX(POSTFIX, UC, LC)                         \
    PYCBC_RESP_ACCESSORS##POSTFIX(UC, LC) PYCBC_VAL_ACCESSORS##POSTFIX(UC, LC) \
            PYCBC_FLAGS_ACCESSORS_U64##POSTFIX(UC, LC)

#define PYCBC_COUNT_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
    PYCBC_RESP_ACCESSORS##POSTFIX(UC, LC)              \
            PYCBC_LLUVAL_ACCESSORS##POSTFIX(UC, LC)

#define PYCBC_NOKEY_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
    PYCBC_NOKEY_ACCESSORS##POSTFIX(UC, LC)

#    define PYCBC_ENDURE_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
        PYCBC_ENDURE_ACCESSORS##POSTFIX(UC, LC)

#    define ENDUREOPS(X, ...) X(ENDURE, endure)

#    define PYCBC_OBSERVE_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
        PYCBC_OBSERVE_ACCESSORS##POSTFIX(UC, LC)
#    define OBSERVEOPS(X, ...) X(OBSERVE, observe)

#define PYCBC_RESP_ACCESSORS_DECLS(UC, LC)                                   \
    lcb_STATUS lcb_resp##LC##_key(                                           \
            const lcb_RESP##UC *resp, const char **buffer, size_t *len);     \
    lcb_STATUS lcb_resp##LC##_status(const lcb_RESP##UC *resp);              \
    lcb_STATUS lcb_resp##LC##_cookie(const lcb_RESP##UC *resp, void **dest); \
    lcb_STATUS lcb_resp##LC##_cas(const lcb_RESP##UC *resp, uint64_t *dest);

#define PYCBC_KEY_ACCESSORS_DECLS(UC, LC) \
    lcb_STATUS lcb_resp##LC##_value(      \
            const lcb_RESP##UC *resp, const char **dest, size_t *length);

#define PYCBC_VAL_ACCESSORS_DECLS(UC, LC) \
    lcb_STATUS lcb_resp##LC##_value(      \
            const lcb_RESP##UC *resp, const char **dest, size_t *length);

#define PYCBC_LLUVAL_ACCESSORS_DECLS(UC, LC) \
    lcb_STATUS lcb_resp##LC##_value(const lcb_RESP##UC *resp, lcb_U64 *dest);

#define PYCBC_FLAGS_ACCESSORS_DECLS(UC, LC) \
    lcb_STATUS lcb_resp##LC##_flags(const lcb_RESP##UC *resp, lcb_U64 *dest);

#define PYCBC_NOKEY_ACCESSORS_DECLS(UC, LC)                                  \
    lcb_STATUS lcb_resp##LC##_status(const lcb_RESP##UC *resp);              \
    lcb_STATUS lcb_resp##LC##_cookie(const lcb_RESP##UC *resp, void **dest); \
    lcb_STATUS lcb_resp##LC##_cas(const lcb_RESP##UC *resp, lcb_uint64_t *dest);

#define PYCBC_LLUVAL_ACCESSOR_DECLS(UC, LC) \
    lcb_STATUS lcb_resp##LC##_value(const lcb_RESP##UC *resp, lcb_U64 *dest);

#define PYCBC_FLAGS_ACCESSORS_U32_DECLS(UC, LC)               \
    lcb_STATUS lcb_resp##LC##_flags(const lcb_RESP##UC *resp, \
                                    lcb_uint32_t *dest);
#define PYCBC_ITMFLAGS_ACCESSORS_U32_DECLS(UC, LC)            \
    lcb_STATUS lcb_resp##LC##_flags(const lcb_RESP##UC *resp, \
                                    lcb_uint32_t *dest);

#define PYCBC_FLAGS_ACCESSORS_U64_DECLS(UC, LC) \
    lcb_STATUS lcb_resp##LC##_flags(const lcb_RESP##UC *resp, lcb_U64 *dest);

#define PYCBC_HOST_ACCESSORS_DECLS(UC, LC) \
    lcb_STATUS lcb_cmd##LC##_host(         \
            lcb_CMD##UC *cmd, const char *host, size_t len);
#define PYCBC_ENDURE_ACCESSORS_DECLS(UC, LC) PYCBC_RESP_ACCESSORS_DECLS(UC, LC)
#define PYCBC_OBSERVE_ACCESSORS_DECLS(UC, LC) PYCBC_RESP_ACCESSORS_DECLS(UC, LC)

#define PYCBC_X_FOR_EACH_OP_POSTFIX(POSTFIX)                      \
    PYCBC_RESP_ACCESSORS_POSTFIX(POSTFIX, REMOVE, remove)         \
    PYCBC_RESP_ACCESSORS_POSTFIX(POSTFIX, UNLOCK, unlock)         \
    PYCBC_RESP_ACCESSORS_POSTFIX(POSTFIX, TOUCH, touch)           \
    PYCBC_GET_ACCESSORS_POSTFIX(POSTFIX, GET, get)                \
    PYCBC_RESP_ACCESSORS_POSTFIX(POSTFIX, GETREPLICA, getreplica) \
    PYCBC_COUNT_ACCESSORS_POSTFIX(POSTFIX, COUNTER, counter)      \
    PYCBC_STATS_ACCESSORS_POSTFIX(POSTFIX, STATS, stats)          \
    PYCBC_NOKEY_ACCESSORS_POSTFIX(POSTFIX, PING, ping)            \
    PYCBC_NOKEY_ACCESSORS_POSTFIX(POSTFIX, DIAG, diag)            \
    PYCBC_HTTP_ACCESSORS_POSTFIX(POSTFIX, HTTP, http)             \
    PYCBC_ENDURE_ACCESSORS_POSTFIX(POSTFIX, ENDURE, endure)       \
    PYCBC_OBSERVE_ACCESSORS_POSTFIX(POSTFIX, OBSERVE, observe)

PYCBC_X_FOR_EACH_OP_POSTFIX(_DECLS)

#ifndef PYCBC_CRYPTO_VERSION
#if LCB_VERSION > 0x020807
#define PYCBC_CRYPTO_VERSION 1
#else
#define PYCBC_CRYPTO_VERSION 0
#endif
#endif

lcb_STATUS pycbc_crypto_register(lcb_INSTANCE instance, const char *name, lcbcrypto_PROVIDER *provider);

#endif // COUCHBASE_PYTHON_CLIENT_LCB_V4_BACKPORT_H
