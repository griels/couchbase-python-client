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

#ifndef COUCHBASE_PYTHON_CLIENT_LCB_V4_WRAPPER_H
#define COUCHBASE_PYTHON_CLIENT_LCB_V4_WRAPPER_H
#include <libcouchbase/couchbase.h>
#include <libcouchbase/utils.h>
#include <libcouchbase/crypto.h>
#ifdef PYCBC_BACKPORT_CRYPTO
#    include <libcouchbase/../../../libcouchbase_src-prefix/src/libcouchbase_src/src/internalstructs.h>
#endif
#define PYCBC_V4
#define PYCBC_ENDURE 0

#include "python_wrappers.h"

#define PYCBC_SCOPE_GET(SCOPE, CTXTYPE, ATTRIB, TYPE)      \
    TYPE pycbc_##SCOPE##_##ATTRIB(CTXTYPE ctx)             \
    {                                                      \
        TYPE##_DEFINE(TYPE);                               \
        lcb_resp##SCOPE##_##ATTRIB(ctx, TYPE##_USE(TYPE)); \
        return temp;                                       \
    }

typedef lcb_DURABILITY_LEVEL pycbc_DURABILITY_LEVEL;

#define PYCBC_FTS_DISABLED
typedef lcb_INSTANCE *lcb_t;
typedef lcb_STATUS lcb_error_t;

typedef lcb_VIEW_HANDLE *pycbc_VIEW_HANDLE;
typedef lcb_HTTP_HANDLE *pycbc_HTTP_HANDLE;
typedef lcb_FTS_HANDLE *pycbc_FTS_HANDLE;
typedef lcb_N1QL_HANDLE *pycbc_N1QL_HANDLE;

#define pycbc_rget(INSTANCE, COOKIE, CMD) \
    pycbc_verb_postfix(, getreplica, (INSTANCE), (COOKIE), (CMD))
#define pycbc_verb(VERB, ...) pycbc_verb_postfix(, VERB, __VA_ARGS__)
#define LCB_STORE_WRAPPER(b) handler(module, "LCB_" #b, LCB_STORE_##b);

#ifndef LIBCOUCHBASE_couchbase_internalstructs_h__
enum replica_legacy { LCB_REPLICA_FIRST, LCB_REPLICA_SELECT, LCB_REPLICA_ALL };
#endif


#define PYCBC_get_ATTR(CMD, attrib, ...) \
    lcb_cmdget_##attrib((CMD), __VA_ARGS__);
#define PYCBC_touch_ATTR(CMD, attrib, ...) \
    lcb_cmdtouch_##attrib((CMD), __VA_ARGS__);
#define PYCBC_getreplica_ATTR(CMD, attrib, ...) \
    lcb_cmdgetreplica_##attrib((CMD), __VA_ARGS__);
#define PYCBC_unlock_ATTR(CMD, attrib, ...) \
    lcb_cmdunlock_##attrib(CMD, __VA_ARGS__);
#define PYCBC_remove_ATTR(CMD, attrib, ...) \
    lcb_cmdremove_##attrib(CMD, __VA_ARGS__);
#define PYCBC_endure_ATTR(CMD, attrib, ...) \
    lcb_cmdendure_##attrib(CMD, __VA_ARGS__);

#define CMDSCOPE_FAIL(UC) \
    cmd_done = 1;         \
    goto GT_##UC##_DONE;
#define CMDSCOPE_GENERIC(UC, LC, CC, ...)                  \
    {                                                      \
        int cmd_done = 0;                                  \
        CC(UC, LC, cmd);                                   \
        PYCBC_DEBUG_LOG("Created CMD %s: %llx", #UC, cmd); \
        PYCBC_DEBUG_LOG("Calling CMD %s", #__VA_ARGS__);   \
        __VA_ARGS__;                                       \
        goto GT_##UC##_DONE;                               \
        GT_##UC##_DONE : lcb_cmd##LC##_destroy(cmd);       \
        PYCBC_DEBUG_LOG("Destroyed CMD %s", #UC);          \
        if (cmd_done)                                      \
            goto GT_DONE;                                  \
    }

#define CMDSCOPE(UC, LC, ...) \
    CMDSCOPE_GENERIC(UC, LC, CMDSCOPE_CREATECMD_RAW, __VA_ARGS__)

#define PYCBC_CMD_SET_KEY_SCOPE(SCOPE, CMD, KEY)                          \
    PYCBC_DEBUG_LOG(                                                      \
            "Setting key %.*s on %s", (KEY).length, (KEY).buffer, #SCOPE) \
    lcb_cmd##SCOPE##_key(CMD, (KEY).buffer, (KEY).length)

#define PYCBC_CMD_SET_VALUE_SCOPE(SCOPE, CMD, KEY)                          \
    PYCBC_DEBUG_LOG(                                                        \
            "Setting value %.*s on %s", (KEY).length, (KEY).buffer, #SCOPE) \
    lcb_cmd##SCOPE##_value(CMD, (KEY).buffer, (KEY).length)

#ifndef LIBCOUCHBASE_couchbase_internalstructs_h__
typedef lcb_SUBDOCOPS pycbc_SDSPEC;

/**@ingroup lcb-public-api
 * @defgroup lcb-subdoc Sub-Document API
 * @brief Experimental in-document API access
 * @details The sub-document API uses features from the upcoming Couchbase
 * 4.5 release which allows access to parts of the document. These parts are
 * called _sub-documents_ and can be accessed using the sub-document API
 *
 * @addtogroup lcb-subdoc
 * @{
 */

/**
 * @brief Sub-Document command codes
 *
 * These command codes should be applied as values to lcb_SDSPEC::sdcmd and
 * indicate which type of subdoc command the server should perform.
 */
typedef enum {
    /**
     * Retrieve the value for a path
     */
    LCB_SDCMD_GET = 1,

    /**
     * Check if the value for a path exists. If the path exists then the error
     * code will be @ref LCB_SUCCESS
     */
    LCB_SDCMD_EXISTS,

    /**
     * Replace the value at the specified path. This operation can work
     * on any existing and valid path.
     */
    LCB_SDCMD_REPLACE,

    /**
     * Add the value at the given path, if the given path does not exist.
     * The penultimate path component must point to an array. The operation
     * may be used in conjunction with @ref LCB_SDSPEC_F_MKINTERMEDIATES to
     * create the parent dictionary (and its parents as well) if it does not
     * yet exist.
     */
    LCB_SDCMD_DICT_ADD,

    /**
     * Unconditionally set the value at the path. This logically
     * attempts to perform a @ref LCB_SDCMD_REPLACE, and if it fails, performs
     * an @ref LCB_SDCMD_DICT_ADD.
     */
    LCB_SDCMD_DICT_UPSERT,

    /**
     * Prepend the value(s) to the array indicated by the path. The path should
     * reference an array. When the @ref LCB_SDSPEC_F_MKINTERMEDIATES flag
     * is specified then the array may be created if it does not exist.
     *
     * Note that it is possible to add more than a single value to an array
     * in an operation (this is valid for this commnand as well as
     * @ref LCB_SDCMD_ARRAY_ADD_LAST and @ref LCB_SDCMD_ARRAY_INSERT). Multiple
     * items can be specified by placing a comma between then (the values should
     * otherwise be valid JSON).
     */
    LCB_SDCMD_ARRAY_ADD_FIRST,

    /**
     * Identical to @ref LCB_SDCMD_ARRAY_ADD_FIRST but places the item(s)
     * at the end of the array rather than at the beginning.
     */
    LCB_SDCMD_ARRAY_ADD_LAST,

    /**
     * Add the value to the array indicated by the path, if the value is not
     * already in the array. The @ref LCB_SDSPEC_F_MKINTERMEDIATES flag can
     * be specified to create the array if it does not already exist.
     *
     * Currently the value for this operation must be a JSON primitive (i.e.
     * no arrays or dictionaries) and the existing array itself must also
     * contain only primitives (otherwise a @ref LCB_SUBDOC_PATH_MISMATCH
     * error will be received).
     */
    LCB_SDCMD_ARRAY_ADD_UNIQUE,

    /**
     * Add the value at the given array index. Unlike other array operations,
     * the path specified should include the actual index at which the item(s)
     * should be placed, for example `array[2]` will cause the value(s) to be
     * the 3rd item(s) in the array.
     *
     * The array must already exist and the @ref LCB_SDSPEC_F_MKINTERMEDIATES
     * flag is not honored.
     */
    LCB_SDCMD_ARRAY_INSERT,

    /**
     * Increment or decrement an existing numeric path. If the number does
     * not exist, it will be created (though its parents will not, unless
     * @ref LCB_SDSPEC_F_MKINTERMEDIATES is specified).
     *
     * The value for this operation should be a valid JSON-encoded integer and
     * must be between `INT64_MIN` and `INT64_MAX`, inclusive.
     */
    LCB_SDCMD_COUNTER,

    /**
     * Remove an existing path in the document.
     */
    LCB_SDCMD_REMOVE,

    /**
     * Count the number of elements in an array or dictionary
     */
    LCB_SDCMD_GET_COUNT,

    /**
     * Retrieve the entire document
     */
    LCB_SDCMD_FULLDOC_GET,

    /**
     * Add the entire document
     */
    LCB_SDCMD_FULLDOC_ADD,

    /**
     * Upsert the entire document
     */
    LCB_SDCMD_FULLDOC_UPSERT,
    /**
     * Replace the entire document
     */
    LCB_SDCMD_FULLDOC_REPLACE,

    /**
     * Remove the entire document
     */
    LCB_SDCMD_FULLDOC_REMOVE,

    LCB_SDCMD_MAX
} lcb_SUBDOCOP;
#else

/**
 * Retrieve the entire document
 */
#    define LCB_SDCMD_FULLDOC_GET (LCB_SDCMD_GET_COUNT + 1)

/**
 * Add the entire document
 */
#    define LCB_SDCMD_FULLDOC_ADD (LCB_SDCMD_GET_COUNT + 2)

/**
 * Upsert the entire document
 */
#    define LCB_SDCMD_FULLDOC_UPSERT (LCB_SDCMD_GET_COUNT + 3)
/**
 * Replace the entire document
 */
#    define LCB_SDCMD_FULLDOC_REPLACE (LCB_SDCMD_GET_COUNT + 4)

/**
 * Remove the entire document
 */
#    define LCB_SDCMD_FULLDOC_REMOVE (LCB_SDCMD_GET_COUNT + 5)

#endif
#define CMDSUBDOC_F_UPSERT_DOC (1 << 16)
#define CMDSUBDOC_F_INSERT_DOC (1 << 17)
#define CMDSUBDOC_F_ACCESS_DELETED (1 << 18)

#define PYCBC_CRYPTO_VERSION 2
#define OBSERVEOPS(X)
#define ENDUREOPS(X)

#define lcb_cmdobserve_parent_span(CMD, SPAN) \
    LCB_CMD_SET_TRACESPAN((CMD), (SPAN));
#define lcb_cmdgetreplica_expiration(CMD, TTL)
#define lcb_respstats_cookie(CMD, ...)
#define lcb_resphttp_key(CMD, ...)
#define lcb_resphttp_cas(CMD, ...)

#define PYCBC_CMD_SET_TRACESPAN(TYPE, CMD, SPAN) \
    lcb_cmd##TYPE##_parent_span((CMD), (SPAN));
#define GENERIC_SPAN_OPERAND(SCOPE, INSTANCE, CMD, HANDLE, CONTEXT) \
    lcb_cmd##SCOPE##_parent_span(CMD, (CONTEXT)->span)
#define GENERIC_NULL_OPERAND(SCOPE, COMMAND, INSTANCE, HANDLE, CONTEXT, ...) \
    lcb_##SCOPE(INSTANCE, __VA_ARGS__);
#define PYCBC_LOG_KEY(CMD, key)

uint64_t pycbc_mutation_token_seqno(const lcb_MUTATION_TOKEN *pToken);

uint64_t pycbc_mutation_token_vbid(const lcb_MUTATION_TOKEN *pToken);

uint64_t pycbc_mutation_token_uuid(const lcb_MUTATION_TOKEN *pToken);

lcb_STATUS pycbc_crypto_register(lcb_INSTANCE *instance,
                                 const char *name,
                                 lcbcrypto_PROVIDER *provider);
lcb_STATUS pycbc_crypto_unregister(lcb_INSTANCE *instance, const char *name);

typedef struct {
    const lcb_RESPSUBDOC *resp;
    size_t index;
} pycbc_SDENTRY;

lcb_STATUS pycbc_respsubdoc_status(const pycbc_SDENTRY *ent);

pycbc_strn_base_const pycbc_respsubdoc_value(const pycbc_SDENTRY *ent);

int pycbc_sdresult_next(const lcb_RESPSUBDOC *resp,
                        pycbc_SDENTRY *dest,
                        size_t *index);

#define LCB_PING_GET_TYPE_S(X, Y) \
case LCB_PING_SERVICE_##X:        \
    return #Y;

#define PYCBC_PP_ENCRYPT_CONSTANTS(X)
#define PYCBC_X_SD_OPS_FULLDOC(X, NP, VAL, MVAL, CTR, ...) \
    NP(FULLDOC_GET, fulldoc_get, __VA_ARGS__)              \
    X(FULLDOC_UPSERT, fulldoc_upsert, __VA_ARGS__)         \
    X(FULLDOC_ADD, fulldoc_add, __VA_ARGS__)               \
    X(FULLDOC_REPLACE, fulldoc_replace, __VA_ARGS__)       \
    NP(FULLDOC_REMOVE, fulldoc_remove, __VA_ARGS__)

#endif // COUCHBASE_PYTHON_CLIENT_LCB_V4_WRAPPER_H
