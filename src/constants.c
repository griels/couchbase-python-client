/**
 *     Copyright 2013 Couchbase, Inc.
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

#include "pycbc.h"
#include "iops.h"

/**
 * Very simple file that simply adds LCB constants to the module
 */

#define XERR(X) \
    X(SUCCESS) \
    X(AUTH_CONTINUE) \
    X(AUTH_ERROR) \
    X(DELTA_BADVAL) \
    X(E2BIG) \
    X(EBUSY) \
    X(ENOMEM) \
    X(ERANGE) \
    X(ERROR) \
    X(ETMPFAIL) \
    X(EINVAL) \
    X(CLIENT_ETMPFAIL) \
    X(KEY_EEXISTS) \
    X(KEY_ENOENT) \
    X(DLOPEN_FAILED) \
    X(DLSYM_FAILED) \
    X(NETWORK_ERROR) \
    X(NOT_MY_VBUCKET) \
    X(NOT_STORED) \
    X(NOT_SUPPORTED) \
    X(UNKNOWN_HOST) \
    X(PROTOCOL_ERROR) \
    X(ETIMEDOUT) \
    X(BUCKET_ENOENT) \
    X(CONNECT_ERROR) \
    X(EBADHANDLE) \
    X(SERVER_BUG) \
    X(PLUGIN_VERSION_MISMATCH) \
    X(INVALID_HOST_FORMAT) \
    X(INVALID_CHAR) \
    X(DURABILITY_ETOOMANY) \
    X(DUPLICATE_COMMANDS) \
    X(HTTP_ERROR) \
    X(SUBDOC_PATH_ENOENT) \
    X(SUBDOC_PATH_MISMATCH) \
    X(SUBDOC_PATH_EINVAL) \
    X(SUBDOC_DOC_E2DEEP) \
    X(SUBDOC_VALUE_E2DEEP) \
    X(SUBDOC_VALUE_CANTINSERT) \
    X(SUBDOC_DOC_NOTJSON) \
    X(SUBDOC_NUM_ERANGE) \
    X(SUBDOC_BAD_DELTA) \
    X(SUBDOC_PATH_EEXISTS) \
    X(SUBDOC_MULTI_FAILURE) \
    X(EMPTY_PATH)

#define XHTTP(X) \
    X(HTTP_METHOD_GET) \
    X(HTTP_METHOD_POST) \
    X(HTTP_METHOD_PUT) \
    X(HTTP_METHOD_DELETE)



#define XSTORAGE(X) \
    X(ADD) \
    X(REPLACE) \
    X(SET) \
    X(APPEND) \
    X(PREPEND)

typedef struct { const char *s; lcb_U32 u32; } STR_u32MAP;
const STR_u32MAP* getCompMap() {
    static const STR_u32MAP optmap[] = {
            { "on", LCB_COMPRESS_INOUT },
            { "off", LCB_COMPRESS_NONE },
            { "inflate_only", LCB_COMPRESS_IN },
            { "force", LCB_COMPRESS_INOUT|LCB_COMPRESS_FORCE },
            { NULL }
    };
    return optmap;
}
static const STR_u32MAP* u32_from_map(const char *s, const STR_u32MAP *lookup) {
    const STR_u32MAP *ret;
    for (ret = lookup; ret->s; ret++) {
        lcb_SIZE maxlen = strlen(ret->s);
        if (!strncmp(ret->s, s, maxlen)) { return ret; }
    }
    return NULL;
}
static const STR_u32MAP* str_from_map(lcb_U32 i, const STR_u32MAP *lookup) {
    const STR_u32MAP *ret;
    for (ret = lookup; ret->s; ret++) {
        if (ret->u32==i) { return ret; }
    }
    return NULL;
}
#define DO_CONVERT_STR2NUM(s, lookup, v) { \
    const STR_u32MAP *str__rv = u32_from_map(s, lookup); \
    if (str__rv) { (v) = str__rv->u32; } else { return LCB_ECTL_BADARG; } };

#define DO_CONVERT_NUM2STR(u32, lookup, v) { \
    const STR_u32MAP *str__rv = str_from_map(u32, lookup); \
    if (str__rv) { (v) = str__rv->s; } else { return LCB_ECTL_BADARG; } };

/* Union used for conversion to/from string functions */
typedef union {
    lcb_U32 u32;
    lcb_SIZE sz;
    int i;
    float f;
    void *p;
} u_STRCONVERT;

static lcb_error_t convert_compression(const char *arg, u_STRCONVERT *u) {
    const STR_u32MAP *optmap = getCompMap();
    DO_CONVERT_STR2NUM(arg, optmap, u->i);
    return LCB_SUCCESS;
};

static lcb_error_t convert_compression_num2str(const char *arg, u_STRCONVERT *u) {
    const STR_u32MAP *optmap = getCompMap();
    DO_CONVERT_NUM2STR(arg, optmap, u->p);
    return LCB_SUCCESS;
};

static void
do_all_constants(PyObject *module,
                 void (*handler)(PyObject*, const char*, PY_LONG_LONG))
{
    #define ADD_MACRO(sym) handler(module, #sym, sym)
    #define ADD_CONSTANT(name, val) handler(module, name, val)
    #define LCB_CONSTANT(postfix,...) ADD_CONSTANT(#postfix, LCB_##postfix)0

    #define X(b) ADD_MACRO(LCB_##b);
    XERR(X);
    XSTORAGE(X);
    XHTTP(X);
    #undef X

    ADD_MACRO(PYCBC_CMD_GET);
    ADD_MACRO(PYCBC_CMD_LOCK);
    ADD_MACRO(PYCBC_CMD_TOUCH);
    ADD_MACRO(PYCBC_CMD_GAT);

    ADD_MACRO(PYCBC_EXC_ARGUMENTS);
    ADD_MACRO(PYCBC_EXC_ENCODING);
    ADD_MACRO(PYCBC_EXC_LCBERR);
    ADD_MACRO(PYCBC_EXC_INTERNAL);
    ADD_MACRO(PYCBC_EXC_HTTP);
    ADD_MACRO(PYCBC_EXC_THREADING);
    ADD_MACRO(PYCBC_EXC_DESTROYED);
    ADD_MACRO(PYCBC_EXC_PIPELINE);

    ADD_MACRO(LCB_TYPE_BUCKET);
    ADD_MACRO(LCB_TYPE_CLUSTER);
    ADD_MACRO(LCB_HTTP_TYPE_VIEW);
    ADD_MACRO(LCB_HTTP_TYPE_MANAGEMENT);

    ADD_MACRO(PYCBC_RESFLD_CAS);
    ADD_MACRO(PYCBC_RESFLD_FLAGS);
    ADD_MACRO(PYCBC_RESFLD_KEY);
    ADD_MACRO(PYCBC_RESFLD_VALUE);
    ADD_MACRO(PYCBC_RESFLD_RC);
    ADD_MACRO(PYCBC_RESFLD_HTCODE);
    ADD_MACRO(PYCBC_RESFLD_URL);

    ADD_CONSTANT("FMT_JSON", (lcb_U32)PYCBC_FMT_JSON);
    ADD_CONSTANT("FMT_BYTES", (lcb_U32)PYCBC_FMT_BYTES);
    ADD_CONSTANT("FMT_UTF8", (lcb_U32)PYCBC_FMT_UTF8);
    ADD_CONSTANT("FMT_PICKLE", (lcb_U32)PYCBC_FMT_PICKLE);
    ADD_CONSTANT("FMT_LEGACY_MASK", (lcb_U32)PYCBC_FMT_LEGACY_MASK);
    ADD_CONSTANT("FMT_COMMON_MASK", (lcb_U32)PYCBC_FMT_COMMON_MASK);

    ADD_CONSTANT("OBS_PERSISTED", LCB_OBSERVE_PERSISTED);
    ADD_CONSTANT("OBS_FOUND", LCB_OBSERVE_FOUND);
    ADD_CONSTANT("OBS_NOTFOUND", LCB_OBSERVE_NOT_FOUND);
    ADD_CONSTANT("OBS_LOGICALLY_DELETED",
                 LCB_OBSERVE_PERSISTED| LCB_OBSERVE_NOT_FOUND);

    ADD_CONSTANT("OBS_MASK",
                 LCB_OBSERVE_PERSISTED|LCB_OBSERVE_FOUND| LCB_OBSERVE_NOT_FOUND);

    ADD_CONSTANT("LOCKMODE_WAIT", PYCBC_LOCKMODE_WAIT);
    ADD_CONSTANT("LOCKMODE_EXC", PYCBC_LOCKMODE_EXC);
    ADD_CONSTANT("LOCKMODE_NONE", PYCBC_LOCKMODE_NONE);

    ADD_MACRO(PYCBC_CONN_F_WARNEXPLICIT);
    ADD_MACRO(PYCBC_CONN_F_CLOSED);
    ADD_MACRO(PYCBC_CONN_F_ASYNC);
    ADD_MACRO(PYCBC_CONN_F_ASYNC_DTOR);

    ADD_MACRO(PYCBC_EVACTION_WATCH);
    ADD_MACRO(PYCBC_EVACTION_UNWATCH);
    ADD_MACRO(PYCBC_EVACTION_SUSPEND);
    ADD_MACRO(PYCBC_EVACTION_RESUME);
    ADD_MACRO(PYCBC_EVACTION_CLEANUP);
    ADD_MACRO(PYCBC_EVSTATE_INITIALIZED);
    ADD_MACRO(PYCBC_EVSTATE_ACTIVE);
    ADD_MACRO(PYCBC_EVSTATE_SUSPENDED);
    ADD_MACRO(PYCBC_EVTYPE_IO);
    ADD_MACRO(PYCBC_EVTYPE_TIMER);
    ADD_MACRO(LCB_READ_EVENT);
    ADD_MACRO(LCB_WRITE_EVENT);
    ADD_MACRO(LCB_RW_EVENT);

    ADD_MACRO(LCB_ERRTYPE_DATAOP);
    ADD_MACRO(LCB_ERRTYPE_FATAL);
    ADD_MACRO(LCB_ERRTYPE_INTERNAL);
    ADD_MACRO(LCB_ERRTYPE_NETWORK);
    ADD_MACRO(LCB_ERRTYPE_TRANSIENT);
    ADD_MACRO(LCB_ERRTYPE_INPUT);

    /* For CNTL constants */
    ADD_MACRO(LCB_CNTL_OP_TIMEOUT);
    ADD_MACRO(LCB_CNTL_VIEW_TIMEOUT);
    ADD_MACRO(LCB_CNTL_SSL_MODE);
    ADD_MACRO(LCB_SSL_ENABLED);
    ADD_MACRO(LCB_CNTL_N1QL_TIMEOUT);
    ADD_MACRO(LCB_CNTL_COMPRESSION_OPTS);

    /* View options */
    ADD_MACRO(LCB_CMDVIEWQUERY_F_INCLUDE_DOCS);
    ADD_MACRO(LCB_CMDVIEWQUERY_F_SPATIAL);

    ADD_MACRO(LCB_SDCMD_REPLACE);
    ADD_MACRO(LCB_SDCMD_DICT_ADD);
    ADD_MACRO(LCB_SDCMD_DICT_UPSERT);
    ADD_MACRO(LCB_SDCMD_ARRAY_ADD_FIRST);
    ADD_MACRO(LCB_SDCMD_ARRAY_ADD_LAST);
    ADD_MACRO(LCB_SDCMD_ARRAY_ADD_UNIQUE);
    ADD_MACRO(LCB_SDCMD_EXISTS);
    ADD_MACRO(LCB_SDCMD_GET);
    ADD_MACRO(LCB_SDCMD_COUNTER);
    ADD_MACRO(LCB_SDCMD_REMOVE);
    ADD_MACRO(LCB_SDCMD_ARRAY_INSERT);

    /* Bucket types */
    ADD_MACRO(LCB_BTYPE_UNSPEC);
    ADD_MACRO(LCB_BTYPE_COUCHBASE);
    ADD_MACRO(LCB_BTYPE_EPHEMERAL);
    ADD_MACRO(LCB_BTYPE_MEMCACHED);



#define __NL__
    /* Compression options */
#define LCB_FOR_EACH_COMPRESS_TYPE(X,DIV)\
__NL__\
    X(COMPRESS_NONE)DIV \
    X(COMPRESS_IN)DIV \
    X(COMPRESS_OUT)DIV \
    X(COMPRESS_INOUT)DIV \
    X(COMPRESS_FORCE)
LCB_FOR_EACH_COMPRESS_TYPE(LCB_CONSTANT,;);
#undef LCB_FOR_EACH_COMPRESS_TYPE
#undef LCB_CONSTANT

#ifdef LCB_N1XSPEC_F_DEFER
    ADD_MACRO(LCB_N1XSPEC_F_DEFER);
#endif
}

static void
do_constmod(PyObject *module, const char *name, PY_LONG_LONG value) {
    PyObject *o = PyLong_FromLongLong(value);
    PyModule_AddObject(module, name, o);
}

void
pycbc_init_pyconstants(PyObject *module)
{
    do_all_constants(module, do_constmod);
    /* We support built-in include_docs now! */
    PyModule_AddIntConstant(module, "_IMPL_INCLUDE_DOCS", 1);
}


PyObject *
pycbc_lcb_errstr(lcb_t instance, lcb_error_t err)
{
#if PY_MAJOR_VERSION == 3

    return PyUnicode_InternFromString(lcb_strerror(instance, err));
#else
    return PyString_InternFromString(lcb_strerror(instance, err));
#endif
}

static void
do_printmod(PyObject *module, const char *name, PY_LONG_LONG value)
{
    printf("%s = %lld\n", name, value);
}

PyObject *
pycbc_print_constants(PyObject *mod, PyObject *args)
{
    do_all_constants(NULL, do_printmod);
    (void)mod;
    (void)args;
    Py_RETURN_NONE;
}
