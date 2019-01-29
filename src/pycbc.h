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
#ifndef PYCBC_H_
#define PYCBC_H_
/**
 * This file contains the base header for the Python Couchbase Client
 * @author Mark Nunberg
 */
#ifndef PYCBC_LCB_API
//#define PYCBC_LCB_API 0x0310000
#endif
//#define PYCBC_LCB_API 0x03100000

#ifndef PYCBC_TRACING_DISABLE
#define PYCBC_TRACING
#endif

#ifdef PYCBC_TRACING
#define PYCBC_TRACE_FINISH_SPANS
#define PYCBC_GLOBAL_SCHED
#define PYCBC_POSTINCREMENT
#define PYCBC_FREE_ACCOUNTING
#define PYCBC_FREE_CONTEXTS

#ifdef PYCBC_REF_ACCOUNTING_ENABLE
#define PYCBC_REF_ACCOUNTING
#endif

#define PYCBC_REF_CLEANUP_ENABLE

#ifdef PYCBC_REF_ACCOUNTING
#ifdef PYCBC_REF_CLEANUP_ENABLE
#define PYCBC_REF_CLEANUP
#endif
#endif
#endif

#ifndef __FUNCTION_NAME__
#if defined(_WIN32) || defined(WIN32) // WINDOWS
#define __FUNCTION_NAME__ __FUNCTION__
#else //*NIX
#define __FUNCTION_NAME__ __func__
#endif
#endif

#ifdef PYCBC_TRACING
typedef struct pycbc_stack_context_decl *pycbc_stack_context_handle;
#else
typedef void* pycbc_stack_context_handle;
#endif
#define PYCBC_TABBED_CONTEXTS
#ifndef  PYCBC_DEBUG
#ifdef PYCBC_DEBUG_ENABLE
#define PYCBC_DEBUG
#else
#define PYCBC_DEBUG
#endif
#endif

#ifdef PYCBC_DEBUG
#ifdef PYCBC_TRACING
void pycbc_debug_log_prefix(const char* FILE, const char* FUNC, int LINE, pycbc_stack_context_handle CONTEXT);
void pycbc_debug_log(const char* FILE, const char* FUNC, int LINE, pycbc_stack_context_handle CONTEXT, const char* format, ...);
#define PYCBC_DEBUG_LOG_PREFIX(FILE, FUNC, LINE, CONTEXT)  pycbc_debug_log_prefix(FILE,FUNC,LINE,CONTEXT);
#define PYCBC_DEBUG_LOG_CONTEXT_FULL(FILE,FUNC,LINE,CONTEXT,...) pycbc_debug_log(FILE,FUNC,LINE,CONTEXT,__VA_ARGS__);
#else
#define PYCBC_DEBUG_LOG_PREFIX(FILE, FUNC, LINE, CONTEXT)  pycbc_debug_log_prefix_nocontext(FILE,FUNC,LINE);
#define PYCBC_DEBUG_LOG_CONTEXT_FULL(FILE,FUNC,LINE,CONTEXT,...) pycbc_debug_log_nocontext(FILE,FUNC,LINE,__VA_ARGS__);
#endif
void pycbc_debug_log_prefix_nocontext(const char* FILE, const char* FUNC, int LINE);
void pycbc_debug_log_nocontext(const char* FILE, const char* FUNC, int LINE, const char* format, ...);
#define PYCBC_DEBUG_LOG_NOCONTEXT_FULL(FILE,FUNC,LINE,...) pycbc_debug_log_nocontext(FILE,FUNC,LINE,__VA_ARGS__);
#define PYCBC_DEBUG_LOG_PREFIX_NOCONTEXT(FILE, FUNC, LINE) pycbc_debug_log_prefix_nocontext(FILE,FUNC,LINE);
#else
#define PYCBC_DEBUG_LOG_NOCONTEXT_FULL(FILE,FUNC,LINE,CONTEXT,...)
#define PYCBC_DEBUG_LOG_CONTEXT_FULL(FILE,FUNC,LINE,CONTEXT,FORMAT,...)
#define PYCBC_DEBUG_LOG_PREFIX(FILE, FUNC, LINE, CONTEXT)
#define PYCBC_DEBUG_LOG_PREFIX_NOCONTEXT(FILE, FUNC, LINE)
#endif

#define PYCBC_GC 3
#define PYCBC_AUTO_DEREF_FAILED

#ifdef PYCBC_DEBUG
#define PYCBC_DEBUG_LOG_RAW(...) fprintf(stderr,__VA_ARGS__);
void pycbc_print_pyformat(const char *format, ...);
void pycbc_exception_log(const char *file,
                         const char *func,
                         int line,
                         int clear);
#define PYCBC_DEBUG_PYFORMAT_FILE_FUNC_LINE_CONTEXT(  \
        FILE, FUNC, LINE, CONTEXT, ...)       \
    PYCBC_DEBUG_LOG_PREFIX(FILE, FUNC, LINE, CONTEXT) \
    pycbc_print_pyformat(__VA_ARGS__);        \
    fprintf(stderr, "\n");
#define PYCBC_DEBUG_PYFORMAT_FILE_FUNC_LINE_NOCONTEXT( \
        FILE, FUNC, LINE, ...)                 \
    PYCBC_DEBUG_LOG_PREFIX_NOCONTEXT(FILE, FUNC, LINE) \
    pycbc_print_pyformat(__VA_ARGS__);         \
    fprintf(stderr, "\n");

#define PYCBC_DEBUG_PYFORMAT_FILE_FUNC_AND_LINE(FILE, FUNC, LINE, ...) \
    PYCBC_DEBUG_PYFORMAT_FILE_FUNC_LINE_NOCONTEXT(                             \
            FILE, FUNC, LINE, __VA_ARGS__)
#define PYCBC_DEBUG_PYFORMAT_CONTEXT(CONTEXT, ...)         \
    PYCBC_DEBUG_PYFORMAT_FILE_FUNC_LINE_CONTEXT(__FILE__,          \
                                                __FUNCTION_NAME__, \
                                                __LINE__,          \
                                                CONTEXT,           \
                                                __VA_ARGS__)
#define PYCBC_DEBUG_PYFORMAT(...)    \
    PYCBC_DEBUG_PYFORMAT_FILE_FUNC_AND_LINE( \
            __FILE__, __FUNCTION_NAME__, __LINE__, __VA_ARGS__)
#define PYCBC_EXCEPTION_LOG_NOCLEAR \
    pycbc_exception_log(__FILE__, __FUNCTION_NAME__, __LINE__, 0);
#define PYCBC_EXCEPTION_LOG \
    pycbc_exception_log(__FILE__, __FUNCTION_NAME__, __LINE__, 1);
#define PYCBC_DEBUG_FLUSH fflush(stderr);
#else
#define PYCBC_DEBUG_LOG_RAW(...)
#define PYCBC_DEBUG_PYFORMAT_CONTEXT(CONTEXT, FORMAT, ...)
#define PYCBC_DEBUG_PYFORMAT(...)
#define PYCBC_EXCEPTION_LOG_NOCLEAR
#define PYCBC_EXCEPTION_LOG PyErr_Clear();
#define PYCBC_DEBUG_FLUSH
#endif

#define PYCBC_DEBUG_LOG_WITH_FILE_FUNC_AND_LINE_POSTFIX( \
        FILE, FUNC, LINE, CONTEXT, ...)         \
    PYCBC_DEBUG_LOG_CONTEXT_FULL(FILE,FUNC, LINE,CONTEXT,__VA_ARGS__)

#define PYCBC_DEBUG_LOG_WITH_FILE_FUNC_AND_LINE_POSTFIX_NOCONTEXT( \
        FILE, FUNC, LINE, ...)                            \
    PYCBC_DEBUG_LOG_NOCONTEXT_FULL(FILE,FUNC, LINE,__VA_ARGS__)

#define PYCBC_DEBUG_LOG_WITH_FILE_FUNC_LINE_CONTEXT_NEWLINE( \
        FILE, FUNC, LINE, CONTEXT, ...)                      \
    PYCBC_DEBUG_LOG_WITH_FILE_FUNC_AND_LINE_POSTFIX(         \
            FILE, FUNC, LINE, CONTEXT, __VA_ARGS__)
#define PYCBC_DEBUG_LOG_WITH_FILE_FUNC_AND_LINE_NEWLINE(FILE, FUNC, LINE, ...) \
    PYCBC_DEBUG_LOG_WITH_FILE_FUNC_AND_LINE_POSTFIX_NOCONTEXT(                 \
            FILE, FUNC, LINE, __VA_ARGS__)
#define PYCBC_DEBUG_LOG(...)                         \
    PYCBC_DEBUG_LOG_WITH_FILE_FUNC_AND_LINE_NEWLINE( \
            __FILE__, __FUNCTION_NAME__, __LINE__, __VA_ARGS__)
#define PYCBC_DEBUG_LOG_CONTEXT(CONTEXT, ...)            \
    PYCBC_DEBUG_LOG_WITH_FILE_FUNC_LINE_CONTEXT_NEWLINE( \
            __FILE__, __FUNCTION_NAME__, __LINE__, CONTEXT, __VA_ARGS__)

#ifdef PYCBC_DEBUG
#define PYCBC_MALLOC(X) \
    malloc_and_log(__FILE__, __FUNCTION_NAME__, __LINE__, 1, X, #X)
#define PYCBC_MALLOC_TYPED(X, Y) \
    malloc_and_log(__FILE__, __FUNCTION_NAME__, __LINE__, X, sizeof(Y), #Y)
#define PYCBC_CALLOC(X, Y) \
    calloc_and_log(__FILE__, __FUNCTION_NAME__, __LINE__, X, Y, "unknown")
#define PYCBC_CALLOC_TYPED(X, Y) \
    calloc_and_log(__FILE__, __FUNCTION_NAME__, __LINE__, X, sizeof(Y), #Y)
int pycbc_free_debug(const char* FILE, const char* FUNC, int line, void* X);
#define PYCBC_FREE(X)  pycbc_free_debug(__FILE__, __FUNCTION_NAME__, __LINE__, X)


#define LOG_REFOP(Y, OP)                                                    \
    {                                                                       \
        pycbc_assert((Y) && Py_REFCNT(Y) > 0);                              \
        PYCBC_DEBUG_PYFORMAT("%p has count of %ld: *** %R ***: OP: %s",     \
                             Y,                                             \
                             (long)(((PyObject *)(Y)) ? Py_REFCNT(Y) : 0),  \
                             ((PyObject *)(Y) ? (PyObject *)(Y) : Py_None), \
                             #OP);                                          \
        Py_##OP((PyObject *)Y);                                             \
    }
#define LOG_REFOPX(Y, OP)                                                   \
    {                                                                       \
        pycbc_assert(!(Y) || Py_REFCNT(Y) > 0);                             \
        PYCBC_DEBUG_PYFORMAT("%p has count of %ld: *** %R ***: OP: %s",     \
                             Y,                                             \
                             (long)(((PyObject *)(Y)) ? Py_REFCNT(Y) : 0),  \
                             ((PyObject *)(Y) ? (PyObject *)(Y) : Py_None), \
                             #OP);                                          \
        Py_##X##OP((PyObject *)(Y));                                        \
    }

#else
#define PYCBC_FREE(X) free(X)
#define PYCBC_MALLOC(X) malloc(X)
#define PYCBC_MALLOC_TYPED(X, Y) malloc((X) * sizeof(Y))
#define PYCBC_CALLOC(X, Y) calloc(X, Y)
#define PYCBC_CALLOC_TYPED(X, Y) calloc(X, sizeof(Y))
#define LOG_REFOP(Y, OP) Py_##OP(Y)
#define LOG_REFOPX(Y, OP) Py_X##OP(Y)
#endif
#define PYCBC_DECREF(X) LOG_REFOP(X, DECREF)
#define PYCBC_XDECREF(X) LOG_REFOPX(X, DECREF)
#define PYCBC_INCREF(X) LOG_REFOP(X, INCREF)
#define PYCBC_XINCREF(X) LOG_REFOPX(X, INCREF)

#include <Python.h>
#include <libcouchbase/couchbase.h>
#define PYCBC_COLLECTIONS

#if PYCBC_LCB_API>0x030000
#ifdef PYCBC_BACKPORT_CRYPTO
#include <libcouchbase/../../../libcouchbase_src-prefix/src/libcouchbase_src/src/internalstructs.h>
#endif
#define PYCBC_V4
#endif
#if PYCBC_LCB_API<0x031000
#define PYCBC_ENDURE 1
#include <libcouchbase/api3.h>
#else
#define PYCBC_ENDURE 0
#include <libcouchbase/utils.h>
#ifdef PYCBC_V4_EXPLICIT
#include <libcouchbase/api4.h>
#endif
#endif

#if PYCBC_LCB_API<0x030001
#include <libcouchbase/views.h>
#include <libcouchbase/n1ql.h>
//#include <libcouchbase/cbft.h>
#include <libcouchbase/ixmgmt.h>
#endif
// TODO: fix in libcouchbase
#ifdef _WIN32
//#if _MSC_VER >= 1600
//        #include <cstdint>
//#else
    typedef __int8              int8_t;
    typedef __int16             int16_t;
//#endif
#include "libcouchbase/sysdefs.h"
#define uint8_t lcb_uint8_t
#define uint16_t lcb_uint16_t
#define uint64_t lcb_uint64_t
#endif

#include <libcouchbase/crypto.h>
#include "../build/lcb_min_version.h"
#if LCB_VERSION < LCB_MIN_VERSION
#pragma message "Couchbase Python SDK requires libcouchbase " LCB_MIN_VERSION_TEXT " or greater"
#error "Please upgrade libcouchbase accordingly"
#endif

#include <pythread.h>
#include "mresdict.h"

#define PYCBC_REFCNT_ASSERT pycbc_assert

void pycbc_fetch_error(PyObject *err[3]);
void pycbc_store_error(PyObject *err[3]);
void *malloc_and_log(const char *file,
                     const char *func,
                     int line,
                     size_t quant,
                     size_t size,
                     const char *type_name);
void *calloc_and_log(const char *file,
                     const char *func,
                     int line,
                     size_t quant,
                     size_t size,
                     const char *type_name);

#define PYCBC_STASH_EXCEPTION(OP)                           \
    {                                                       \
        PyObject *pycbc_err[3] = {0};                       \
        pycbc_store_error(pycbc_err);                       \
        {                                                   \
            OP;                                             \
        }                                                   \
        if (pycbc_err[0] || pycbc_err[1] || pycbc_err[2]) { \
            pycbc_fetch_error(pycbc_err);                   \
        }                                                   \
    };

/**
 * See http://docs.python.org/2/c-api/arg.html for an explanation of this
 * definition.
 */
#ifdef PY_SSIZE_T_CLEAN
typedef Py_ssize_t pycbc_strlen_t;
#else
typedef int pycbc_strlen_t;
#endif

#define PYCBC_PACKAGE_NAME "couchbase"
#define PYCBC_MODULE_NAME "_libcouchbase"
#define PYCBC_FQNAME PYCBC_PACKAGE_NAME "." PYCBC_MODULE_NAME

#define PYCBC_TCNAME_ENCODE_KEY "encode_key"
#define PYCBC_TCNAME_ENCODE_VALUE "encode_value"
#define PYCBC_TCNAME_DECODE_KEY "decode_key"
#define PYCBC_TCNAME_DECODE_VALUE "decode_value"

/**
 * Python 2.x and Python 3.x have different ideas of what a basic string
 * and int types are. These blocks help us sort things out if we just want a
 * "plain" integer or string
 */
#if PY_MAJOR_VERSION == 3
#define PYCBC_POBJ_HEAD_INIT(t) { PyObject_HEAD_INIT(t) },

/**
 * The IntFrom* macros get us a 'default' integer type from a long, etc.
 * Implemented (if not a simple macro) in numutil.c
 */
#define pycbc_IntFromL PyLong_FromLong
#define pycbc_IntFromUL PyLong_FromUnsignedLong
#define pycbc_IntFromULL PyLong_FromUnsignedLongLong

/**
 * The IntAs* convert the integer type (long, int) into something we want
 */
#define pycbc_IntAsULL PyLong_AsUnsignedLongLong
#define pycbc_IntAsLL PyLong_AsLongLong
#define pycbc_IntAsUL PyLong_AsUnsignedLong
#define pycbc_IntAsL PyLong_AsLong

/**
 * The SimpleString macros generate strings for us. The 'Z' variant takes a
 * NUL-terminated string, while the 'N' variant accepts a length specifier
 */
#define pycbc_SimpleStringZ(c) PyUnicode_FromString(c)
#define pycbc_SimpleStringN(c, n) PyUnicode_FromStringAndSize(c, n)


#else

/**
 * This defines the PyObject head for our types
 */
#define PYCBC_POBJ_HEAD_INIT(t) PyObject_HEAD_INIT(t)

/**
 * See above block for explanation of these macros
 */
#define pycbc_IntFromL PyInt_FromLong
#define pycbc_IntFromUL PyLong_FromUnsignedLong
#define pycbc_IntFromULL PyLong_FromUnsignedLongLong
#define pycbc_SimpleStringZ(c) PyString_FromString(c)
#define pycbc_SimpleStringN(c, n) PyString_FromStringAndSize(c, n)

unsigned PY_LONG_LONG pycbc_IntAsULL(PyObject *o);
PY_LONG_LONG pycbc_IntAsLL(PyObject *o);
long pycbc_IntAsL(PyObject *o);
unsigned long pycbc_IntAsUL(PyObject *o);

#endif

const char *pycbc_cstr(PyObject *object);
const char *pycbc_cstrn(PyObject *object, Py_ssize_t *length);

#define PYCBC_CSTR(X) pycbc_cstr(X)
#define PYCBC_CSTRN(X, n) pycbc_cstrn((X), (Py_ssize_t *)(n))

PyObject* pycbc_replace_str(PyObject** string, const char* pat, const char* replace);
PyObject *pycbc_none_or_value(PyObject *maybe_value);

/**
 * Fetches a valid TTL from the object
 * @param obj an object to be parsed as the TTL
 * @param ttl a pointer to the TTL itself
 * @param nonzero whether to allow a value of 0 for the TTL
 * @return 0 on success, nonzero on error.
 */
int pycbc_get_ttl(PyObject *obj, unsigned long *ttl, int nonzero);

/**
 * Fetches a valid 32 bit integer from the object. The object must be a long
 * or int.
 * @param obj the object containing the number
 * @param out a pointer to a 32 bit integer to be populated
 * @return 0 on success, -1 on failure. On failure, the error indicator is also
 * set
 */
int pycbc_get_u32(PyObject *obj, lcb_uint32_t *out);

/**
 * Converts the object into an PyInt (2.x only) or PyLong (2.x or 3.x)
 */
PyObject *pycbc_maybe_convert_to_int(PyObject *o);

/**
 * Gives us a C buffer from a Python string.
 * @param orig the original object containg a string thing. This is something
 * we can convert into a byte buffer
 *
 * @param buf out, the C buffer, out, set to the new buffer
 * @param nbuf, out, the length of the new buffer
 * @param newkey, out, the new PyObject, which will back the buffer.
 * This should not be DECREF'd until the @c buf is no longer needed
 */
int pycbc_BufFromString(PyObject *orig,
                        char **buf,
                        Py_ssize_t *nbuf,
                        PyObject **newkey);


/**
 * These constants are used internally to figure out the high level
 * operation being performed.
 *
 * Note that not all operations are defined here; it is only those operations
 * where a single C function can handle multiple entry points.
 */
enum {
    PYCBC_CMD_GET = 500,
    PYCBC_CMD_LOCK,
    PYCBC_CMD_TOUCH,
    PYCBC_CMD_GAT,
    PYCBC_CMD_COUNTER,
    PYCBC_CMD_DELETE,
    PYCBC_CMD_UNLOCK,
    PYCBC_CMD_GETREPLICA,
    /** "Extended" get replica, provides for more options */
    PYCBC_CMD_GETREPLICA_INDEX,
    PYCBC_CMD_GETREPLICA_ALL,
    PYCBC_CMD_ENDURE
};

/**
 * Various exception types to be thrown
 */
enum {
    /** Argument Error. User passed the wrong arguments */
    PYCBC_EXC_ARGUMENTS,

    /** Couldn't encode/decode something */
    PYCBC_EXC_ENCODING,

    /** Operational error returned from LCB */
    PYCBC_EXC_LCBERR,

    /** Internal error. There's something wrong with our code */
    PYCBC_EXC_INTERNAL,

    /** HTTP Error */
    PYCBC_EXC_HTTP,

    /** ObjectThreadError */
    PYCBC_EXC_THREADING,

    /** Object destroyed before it could connect */
    PYCBC_EXC_DESTROYED,

    /** Illegal operation in pipeline context */
    PYCBC_EXC_PIPELINE
};

/* Argument options */
enum {
    /** Entry point is a single key variant */
    PYCBC_ARGOPT_SINGLE = 0x1,

    /** Entry point is a multi key variant */
    PYCBC_ARGOPT_MULTI = 0x2,

    PYCBC_ARGOPT_SUBDOC = 0x04,

    PYCBC_ARGOPT_SDMULTI = 0x08
};

/**
 * Format flags
 */
enum {
    PYCBC_FMT_LEGACY_JSON = 0x00,
    PYCBC_FMT_LEGACY_PICKLE = 0x01,
    PYCBC_FMT_LEGACY_BYTES = 0x02,
    PYCBC_FMT_LEGACY_UTF8 = 0x04,
    PYCBC_FMT_LEGACY_MASK = 0x07,

    PYCBC_FMT_COMMON_PICKLE = (0x01U << 24),
    PYCBC_FMT_COMMON_JSON = (0x02U << 24),
    PYCBC_FMT_COMMON_BYTES = (0x03U << 24),
    PYCBC_FMT_COMMON_UTF8 = (0x04U << 24),
    PYCBC_FMT_COMMON_MASK = (0xFFU << 24),

    PYCBC_FMT_JSON = PYCBC_FMT_LEGACY_JSON|PYCBC_FMT_COMMON_JSON,
    PYCBC_FMT_PICKLE = PYCBC_FMT_LEGACY_PICKLE|PYCBC_FMT_COMMON_PICKLE,
    PYCBC_FMT_BYTES = PYCBC_FMT_LEGACY_BYTES|PYCBC_FMT_COMMON_BYTES,
    PYCBC_FMT_UTF8 = PYCBC_FMT_LEGACY_UTF8|PYCBC_FMT_COMMON_UTF8
};

typedef enum {
    PYCBC_LOCKMODE_NONE = 0,
    PYCBC_LOCKMODE_EXC = 1,
    PYCBC_LOCKMODE_WAIT = 2,
    PYCBC_LOCKMODE_MAX
} pycbc_lockmode_t;

enum {
    PYCBC_CONN_F_WARNEXPLICIT = 1 << 0,
    PYCBC_CONN_F_USEITEMRESULT = 1 << 1,
    PYCBC_CONN_F_CLOSED = 1 << 2,

    /**
     * For use with (but not limited to) Twisted.
     *
     * Deliver results asynchronously. This means:
     * 1) Don't call lcb_wait()
     * 2) Return an AsyncContainer (i.e. a MultiResult)
     * 3) Invoke the MultiResult (AsyncContainer)'s callback as needed
     */
    PYCBC_CONN_F_ASYNC = 1 << 3,

    /** Whether this instance has been connected */
    PYCBC_CONN_F_CONNECTED = 1 << 4,

    /** Schedule destruction of iops and lcb instance for later */
    PYCBC_CONN_F_ASYNC_DTOR = 1 << 5
};

#if PYCBC_LCB_API<0x030001
typedef lcb_DURABILITYLEVEL pycbc_DURABILITY_LEVEL;
typedef lcb_RESPVIEWQUERY lcb_RESPVIEW;

typedef lcb_error_t lcb_STATUS;

#else
typedef lcb_DURABILITY_LEVEL pycbc_DURABILITY_LEVEL;

#endif
typedef struct {
    char persist_to;
    char replicate_to;
    pycbc_DURABILITY_LEVEL durability_level;
} pycbc_dur_params;

void pycbc_dict_add_text_kv(PyObject *dict, const char *key, const char *value);

struct pycbc_Tracer;

typedef struct {
    const void *v;
    size_t n;
} pycbc_generic_array;

typedef struct {
    char *buffer;
    size_t length;
} pycbc_strn_base;

typedef struct {
    const char *buffer;
    size_t length;
} pycbc_strn_base_const;

typedef pycbc_strn_base pycbc_strn;
typedef struct {
    pycbc_strn_base content;
} pycbc_strn_unmanaged;

extern pycbc_strn pycbc_invalid_strn;

extern const char PYCBC_UNKNOWN[];
static char *const PYCBC_DEBUG_INFO_STR = "debug_info";
#define sizeof_array(X) sizeof(X) / sizeof(X[0])

const char * pycbc_strn_buf(pycbc_strn buf);
int pycbc_strn_valid(pycbc_strn buf);

size_t pycbc_strn_len(pycbc_strn_base_const buf);
int pycbc_strn_repr_len(pycbc_strn buf);

const char *pycbc_strn_repr_buf(const pycbc_strn buf);

pycbc_strn_unmanaged pycbc_strn_ensure_psz_unmanaged(pycbc_strn *input);

pycbc_strn_unmanaged pycbc_strn_ensure_psz(pycbc_strn *input);

pycbc_strn_unmanaged pycbc_strn_from_managed(PyObject* source);

char *pycbc_strn_buf_psz(pycbc_strn_unmanaged buf);

void pycbc_strn_free(pycbc_strn_unmanaged buf);

pycbc_generic_array pycbc_strn_base_const_array(pycbc_strn_base_const orig);

#define PYCBC_STRN_FREE(BUF)                            \
    PYCBC_DEBUG_LOG("Freeing string buffer %.*s at %p", \
                    (int)(BUF).content.length,               \
                    (BUF).content.buffer,               \
                    (BUF).content.buffer)               \
    pycbc_strn_free(BUF);



#define RAWTYPE_DEFINE(X) X temp;
#define LENGTHTYPE_DEFINE(X) X temp;
#define RAWTYPE_USE(X) temp
#define LENGTHTYPE_USE(X) &temp.resp, &temp.nresp
#define PYCBC_RESP_GET(SCOPE,UCSCOPE,ATTRIB,TYPE)\
PYCBC_SCOPE_GET(SCOPE,const lcb_RESP##UCSCOPE*,ATTRIB,TYPE);

typedef const lcb_RESPGET* pycbc_RESPGET;

#define pycbc_RESPGET_USE(X) &temp
#define pycbc_RESPGET_DEFINE(X) LENGTHTYPE_DEFINE(X)
#define pycbc_RESPGET_RET(X) X
#define pycbc_strn_unmanaged_USE(X) &temp.content.buffer, &temp.content.length
#define pycbc_strn_unmanaged_DEFINE(X) X temp;
#define pycbc_strn_USE(X) &temp.buffer, &temp.length
#define pycbc_strn_DEFINE(X) X temp;
#define pycbc_strn_base_const_USE(X) &temp.buffer, &temp.length
#define pycbc_strn_base_const_DEFINE(X) X temp;

#define PYCBC_DUMMY(...)

#define GET_ATTRIBS(X) \
X(get, lcb_CMDGET*, locktime, lock, int);

#if PYCBC_LCB_API<0x031000

typedef lcb_PINGSVCTYPE lcb_PING_SERVICE;
#endif

#if PYCBC_LCB_API<0x030001
typedef lcb_RESPVIEWQUERY lcb_RESPVIEW;
typedef lcb_CMDVIEWQUERY lcb_CMDVIEW;
typedef lcb_error_t lcb_STATUS;

#    define PYCBC_SCOPE_GET_DECL(SCOPE, CTXTYPE, ATTRIB, TYPE) \
        TYPE pycbc_cmd##SCOPE##_##ATTRIB(const CTXTYPE ctx)

#    define PYCBC_SCOPE_SET_DECL(SCOPE, CTXTYPE, ATTRIB, MEMBER, TYPE)\
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

struct pycbc_pybuffer_real;
lcb_STATUS lcb_cmdget_key(lcb_CMDBASE* ctx, struct pycbc_pybuffer_real* buf);
GET_ATTRIBS(PYCBC_SCOPE_SET_DECL)

#else // PYCBC_LCB_API<0x030001
#    define PYCBC_SCOPE_GET(SCOPE, CTXTYPE, ATTRIB, TYPE)      \
        TYPE pycbc_##SCOPE##_##ATTRIB(CTXTYPE ctx)             \
        {                                                      \
            TYPE##_DEFINE(TYPE);                               \
            lcb_resp##SCOPE##_##ATTRIB(ctx, TYPE##_USE(TYPE)); \
            return temp;                                       \
        }

#endif // PYCBC_LCB_API < 0x030001


# if PYCBC_LCB_API <0x030001
#define LCB_STORE_WRAPPER(b) ADD_MACRO(LCB_STORE_##b);
#else
#define LCB_STORE_WRAPPER(b) handler(module, "LCB_" #b, LCB_STORE_##b);
#endif


#    if PYCBC_LCB_API < 0x031000
#define lcb_cmdping_create(CMD) lcb_CMDPING cmd_real={0}; *(CMD)=&cmd_real;
#define lcb_cmdping_destroy(CMD)
#define lcb_cmddiag_create(CMD) lcb_CMDDIAG cmd_real={0}; *(CMD)=&cmd_real;
#define lcb_cmddiag_destroy(CMD)

enum {
#        define PYCBC_BACKPORT_STORE(X) LCB_STORE_##X = LCB_##X
#        define ALL_ENUMS(X) X(APPEND), X(PREPEND), X(SET), X(ADD), X(REPLACE)
    ALL_ENUMS(PYCBC_BACKPORT_STORE)
};
typedef lcb_error_t lcb_STATUS;
#    else // PYCBC_LCB_API < 0x031000
//typedef lcb_RESPVIEWQUERY lcb_RESPVIEW;

#    endif // PYCBC_LCB_API < 0x031000

#define CMDSCOPE_GENERIC_FAIL fail=1; continue;
#define CMDSCOPE_GENERIC_ALL_PREFIX(PREFIX, UC, LC, INITIALIZER,  DESTRUCTOR, CMDS, ...)  \
    INITIALIZER(UC,LC, CMDS, __VA_ARGS__);\
    PYCBC_DEBUG_LOG("Called initialzer for %s, %s, with args %s", #UC, #LC, #__VA_ARGS__)\
    goto SKIP_##PREFIX##_##UC##_FAIL;\
    goto GT_##PREFIX##_##UC##_DONE;\
    GT_##PREFIX##_##UC##_DONE:\
    PYCBC_DEBUG_LOG("Cleanup up %s %s",#UC,#LC)\
    (void)(DESTRUCTOR(UC,LC,CMDS));\
    goto GT_DONE;\
    SKIP_##PREFIX##_##UC##_FAIL:\
    for (int finished=0,  fail=0;!(finished) && !fail; (finished=(1+DESTRUCTOR(UC,LC,CMDS))))

#define CMDSCOPE_GENERIC_ALL(UC, LC, INITIALIZER,  DESTRUCTOR, CMDS, ...)  \
    CMDSCOPE_GENERIC_ALL_PREFIX(,UC,LC,INITIALIZER, DESTRUCTOR, CMDS, __VA_ARGS__)

//#if PYCBC_LCB_API<0x031000
#define lcb_cmdstats_create(DEST) lcb_CMDSTATS cmd_real={0}; *(DEST)=&cmd_real;
#define lcb_cmdstats_destroy(DEST) 0
//#else

//#define lcb_cmdstats_create(CMD) (*(CMD)) = lcb_cmdstats_alloc();
//#define lcb_cmdstats_destroy(CMD) lcb_cmdstats_dispose(CMD);
//#endif

#define pycbc_verb_postfix(POSTFIX, VERB, INSTANCE, COOKIE, CMD) \
    pycbc_logging_monad_verb(__FILE__,                           \
                             __FUNCTION__,                       \
                             __LINE__,                           \
                             INSTANCE,                           \
                             COOKIE,                             \
                             CMD,                                \
                             #CMD,                               \
                             #VERB,                              \
                             lcb_##VERB##POSTFIX(INSTANCE, COOKIE, CMD))


#if PYCBC_LCB_API < 0x031000
#    define PYCBC_OBSERVE_STANDALONE
#endif

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

#if PYCBC_ENDURE
#    define PYCBC_ENDURE_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
        PYCBC_ENDURE_ACCESSORS##POSTFIX(UC, LC)

#    define ENDUREOPS(X, ...) X(ENDURE, endure)
#else
#    define PYCBC_ENDURE_ACCESSORS_POSTFIX(POSTFIX)
#endif

#ifdef PYCBC_OBSERVE_STANDALONE
#    define PYCBC_OBSERVE_ACCESSORS_POSTFIX(POSTFIX, UC, LC) \
        PYCBC_OBSERVE_ACCESSORS##POSTFIX(UC, LC)
#    define OBSERVEOPS(X, ...) X(OBSERVE, observe)
#else
#    define OBSERVEOPS(X)
#    define ENDUREOPS(X)
#    define PYCBC_OBSERVE_ACCESSORS_POSTFIX(POSTFIX)
#endif

#define PYCBC_RESP_ACCESSORS_DECLS(UC, LC)                               \
    lcb_STATUS lcb_resp##LC##_key(                                       \
            const lcb_RESP##UC *resp, const char **buffer, size_t *len); \
    lcb_STATUS lcb_resp##LC##_status(const lcb_RESP##UC *resp);          \
    lcb_STATUS lcb_resp##LC##_cookie(const lcb_RESP##UC *resp,           \
    void **dest);                 \
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

#define PYCBC_NOKEY_ACCESSORS_DECLS(UC, LC)                     \
    lcb_STATUS lcb_resp##LC##_status(const lcb_RESP##UC *resp); \
    lcb_STATUS lcb_resp##LC##_cookie(const lcb_RESP##UC *resp,  \
    void **dest);        \
    lcb_STATUS lcb_resp##LC##_cas(const lcb_RESP##UC *resp, lcb_uint64_t *dest);

#define PYCBC_LLUVAL_ACCESSOR_DECLS(UC, LC) \
    lcb_STATUS lcb_resp##LC##_value(const lcb_RESP##UC *resp, lcb_U64 *dest);

#define PYCBC_FLAGS_ACCESSORS_U32_DECLS(UC, LC)               \
    lcb_STATUS lcb_resp##LC##_flags(const lcb_RESP##UC *resp, \
                                    lcb_uint32_t *dest);
#define PYCBC_ITMFLAGS_ACCESSORS_U32_DECLS(UC, LC)               \
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

#define PYCBC_X_FOR_EACH_OP(X, NOKEY, STATSOPS, GETOP, COUNTERSOPS) \
    X(REMOVE, remove)                                               \
    X(UNLOCK, unlock)                                               \
    X(TOUCH, touch)                                                 \
    GETOP(GET, get)                                                 \
    X(GETREPLICA, getreplica)                                       \
    COUNTERSOPS(COUNTER, counter)                                   \
    STATSOPS(STATS, stats)                                          \
    NOKEY(PING, ping)                                               \
    NOKEY(DIAG, diag)                                               \
    OBSERVEOPS(X)                                                   \
    ENDUREOPS(X)                                                    \
    X(HTTP, http)

#if PYCBC_LCB_API < 0x030001
typedef lcb_RESPGET lcb_RESPGETREPLICA;
#    define PYCBC_X_FOR_EACH_OP_GEN
#    ifdef PYCBC_X_FOR_EACH_OP_GEN
PYCBC_X_FOR_EACH_OP_POSTFIX(_DECLS)
#    else
PYCBC_X_FOR_EACH_OP(PYCBC_RESP_ACCESSORS,
                    PYCBC_RESP_ACCESSORS,
                    PYCBC_GET_ACCESSORS,
                    PYCBC_GET_ACCESSORS,
                    PYCBC_COUNT_ACCESSORS)
#    endif
#endif

#if PYCBC_LCB_API > 0x030000

#    define PYCBC_FTS_DISABLED
typedef lcb_INSTANCE *lcb_t;
typedef lcb_STATUS lcb_error_t;

typedef lcb_VIEW_HANDLE *pycbc_VIEW_HANDLE;
typedef lcb_HTTP_HANDLE *pycbc_HTTP_HANDLE;
typedef lcb_FTS_HANDLE *pycbc_FTS_HANDLE;
typedef lcb_N1QL_HANDLE *pycbc_N1QL_HANDLE;
#    define DEFAULT_VERBPOSTFIX
#    define pycbc_rget(INSTANCE, COOKIE, CMD) \
        pycbc_verb_postfix(, getreplica, (INSTANCE), (COOKIE), (CMD))
#    define pycbc_verb(VERB, ...) pycbc_verb_postfix(, VERB, __VA_ARGS__)

#else
#    include "libcouchbase/cbft.h"
typedef struct lcb_st lcb_INSTANCE;
typedef lcb_VIEWHANDLE pycbc_VIEW_HANDLE;
typedef lcb_http_request_t pycbc_HTTP_HANDLE;
typedef lcb_FTSHANDLE pycbc_FTS_HANDLE;
typedef lcb_N1QLHANDLE pycbc_N1QL_HANDLE;
#    define DEFAULT_VERBPOSTFIX 3
#    define pycbc_verb(VERB, INSTANCE, ...) \
        pycbc_verb_postfix(3, VERB, INSTANCE, __VA_ARGS__)
#    define pycbc_rget(INSTANCE, COOKIE, CMD) \
        pycbc_verb_postfix(3, rget, (INSTANCE), (COOKIE), (CMD))

#endif

lcb_STATUS pycbc_logging_monad_verb(const char *FILE,
                                    const char *FUNC,
                                    int LINE,
                                    lcb_INSTANCE *instance,
                                    void *COOKIE,
                                    void *CMD,
                                    const char *CMDNAME,
                                    const char *VERB,
                                    lcb_STATUS result);

#define PYCBC_CMD_PROXY(UC, LC)                                    \
    lcb_STATUS pycbc_##LC(                                         \
            lcb_INSTANCE* instance, void *cookie, lcb_CMD##UC *cmd) \
    {                                                              \
        return pycbc_verb(LC, instance, cookie, cmd);              \
    };
#define PYCBC_CMD_PROXY_DECL(UC, LC) \
    lcb_STATUS pycbc_##LC(           \
            lcb_INSTANCE* instance, void *cookie, lcb_CMD##UC *cmd);
#define PYCBC_X_VERBS(X) \
    X(COUNTER, counter)  \
    X(GET, get)          \
    X(TOUCH, touch)      \
    X(UNLOCK, unlock)    \
    X(REMOVE, remove)    \
    X(STORE, store)      \
    X(HTTP, http)        \
    X(PING, ping)        \
    X(SUBDOC, subdoc)

PYCBC_X_VERBS(PYCBC_CMD_PROXY_DECL)

#if PYCBC_LCB_API > 0x030001

#else // PYCBC_LCB_API > 0x030001

#    define LCB_PING_SERVICE_KV LCB_PINGSVC_KV
#    define LCB_PING_SERVICE_VIEWS LCB_PINGSVC_VIEWS
#    define LCB_PING_SERVICE_N1QL LCB_PINGSVC_N1QL
#    define LCB_PING_SERVICE_FTS LCB_PINGSVC_FTS
#    define LCB_PING_SERVICE_ANALYTICS LCB_PINGSVC_ANALYTICS
#    define LCB_PING_SERVICE__MAX LCB_PINGSVC__MAX
#endif // PYCBC_LCB_API > 0x030001

#if PYCBC_LCB_API>0x030000

#define PYCBC_CMD_SET_KEY(TYPE, CMD, BUF, LEN)\
    PYCBC_DEBUG_LOG("Setting key %.*s on cmd %s at %llx", LEN,BUF,#TYPE,CMD);\
    lcb_cmd##TYPE##_key((CMD),((const char*)BUF),(LEN))
#define PYCBC_CMD_SET_VALUE(TYPE, CMD, BUF, LEN)\
    lcb_cmd##TYPE##_value((CMD),((const char*)BUF),(LEN))
#ifndef LIBCOUCHBASE_couchbase_internalstructs_h__
enum replica_legacy{
    LCB_REPLICA_FIRST,
    LCB_REPLICA_SELECT,
    LCB_REPLICA_ALL
};
#endif

#define lcb_cmdgetreplica_expiration(CMD, TTL)
#define lcb_cmdendure_cas(CMD, CAS) (CMD)->cas=CAS
#define lcb_cmdendure_key(cmd, buf, len) LCB_CMD_SET_KEY(((cmd)),buf, len)

#define PYCBC_get_ATTR(CMD,attrib,...) lcb_cmdget_##attrib((CMD),__VA_ARGS__);
#define PYCBC_touch_ATTR(CMD,attrib,...) lcb_cmdtouch_##attrib((CMD),__VA_ARGS__);
#define PYCBC_getreplica_ATTR(CMD,attrib,...) lcb_cmdgetreplica_##attrib((CMD),__VA_ARGS__);
#define PYCBC_unlock_ATTR(CMD, attrib,...) lcb_cmdunlock_##attrib(CMD,__VA_ARGS__);
#define PYCBC_remove_ATTR(CMD, attrib,...) lcb_cmdremove_##attrib(CMD,__VA_ARGS__);
#define PYCBC_endure_ATTR(CMD, attrib,...) lcb_cmdendure_##attrib(CMD,__VA_ARGS__);



#define CMDSCOPE_FAIL(UC) cmd_done=1; goto GT_##UC##_DONE;
#define CMDSCOPE_GENERIC(UC,LC,CC,...) { \
    int cmd_done=0;\
    CC(UC,LC,cmd);\
    PYCBC_DEBUG_LOG("Created CMD %s: %llx",#UC,cmd);\
    PYCBC_DEBUG_LOG("Calling CMD %s",#__VA_ARGS__);\
    __VA_ARGS__;\
    goto GT_##UC##_DONE;\
    GT_##UC##_DONE:\
    lcb_cmd##LC##_destroy(cmd);\
    PYCBC_DEBUG_LOG("Destroyed CMD %s",#UC);\
    if (cmd_done) goto GT_DONE;}

#define CMDSCOPE(UC, LC, ...) CMDSCOPE_GENERIC(UC, LC, CMDSCOPE_CREATECMD_RAW, __VA_ARGS__)



#define PYCBC_CMD_SET_KEY_SCOPE(SCOPE,CMD,KEY)\
        PYCBC_DEBUG_LOG("Setting key %.*s on %s",(KEY).length, (KEY).buffer, #SCOPE)\
        lcb_cmd##SCOPE##_key(CMD,(KEY).buffer, (KEY).length)

#define PYCBC_CMD_SET_VALUE_SCOPE(SCOPE,CMD,KEY)\
        PYCBC_DEBUG_LOG("Setting value %.*s on %s",(KEY).length, (KEY).buffer, #SCOPE)\
        lcb_cmd##SCOPE##_value(CMD,(KEY).buffer, (KEY).length)

#else // PYCBC_LCB_API>0x030000


#define PYCBC_CMD_SET_KEY_SCOPE(SCOPE,CMD,KEY)\
        PYCBC_DEBUG_LOG("Setting key %.*s",(KEY).length, (KEY).buffer)\
        LCB_CMD_SET_KEY(CMD,(KEY).buffer, (KEY).length)

#define PYCBC_CMD_SET_VALUE_SCOPE(SCOPE,CMD,KEY)\
        PYCBC_DEBUG_LOG("Setting value %.*s",(KEY).length, (KEY).buffer)\
        LCB_CMD_SET_VALUE(CMD,(KEY).buffer, (KEY).length)

#define PYCBC_get_ATTR(CMD,attrib,...) CMD->attrib=__VA_ARGS__;
#define PYCBC_touch_ATTR(CMD,attrib,...) CMD->attrib=__VA_ARGS__;
#define PYCBC_getreplica_ATTR(CMD,attrib,...) CMD->attrib=__VA_ARGS__;
#define PYCBC_unlock_ATTR(CMD, attrib,...) CMD->attrib=__VA_ARGS__;
#define PYCBC_remove_ATTR(CMD, attrib,...) CMD->attrib=__VA_ARGS__;
#define PYCBC_endure_ATTR(CMD, attrib,...) CMD->attrib=__VA_ARGS__;
#define CMDSCOPE_GENERIC(UC, LC, CC, ...)  \
    {                                                \
        lcb_CMD##UC cmd_real = {0};                  \
        lcb_CMD##UC *cmd = &cmd_real;                \
        CC(UC, LC, cmd);         \
        int cmd_done = 0;                            \
        __VA_ARGS__;                                 \
        goto GT_##UC##_DONE;                         \
        GT_##UC##_DONE : if (cmd_done) goto GT_DONE; \
    }
#define CMDSCOPE(UC, LC, ...) CMDSCOPE_GENERIC(UC, LC, PYCBC_DUMMY, __VA_ARGS__)
#define CMDSCOPE_FAIL(UC) { cmd_done=1; goto GT_##UC##_DONE;};



#define lcb_cmdget_expiration(cmd,time) cmd->exptime=time;
#define lcb_cmdget_timeout(cmd,time) cmd->exptime=time;
#define lcb_cmdtouch_create(CMD) lcb_CMDTOUCH cmd_real={0};*(CMD)=&cmd_real;
#define lcb_cmdtouch_destroy(CMD) 0

#define lcb_cmdtouch_expiration(cmd,time) cmd->exptime=time;
#define lcb_cmdtouch_timeout(cmd,time) cmd->exptime=time;
#define lcb_cmdrget_expiration(cmd,time) cmd->exptime=time;
#define lcb_cmdcounter_delta(cmd, x) (cmd)->delta=x;
#define lcb_cmdcounter_initial(cmd, x) (cmd)->initial=x; (cmd)->create=1;
#define lcb_cmdcounter_timeout(cmd, x) (cmd)->exptime=x;
#define lcb_cmdcounter_expiration(cmd, x) (cmd)->exptime=x
#define lcb_cmdstore_flags(CMD, VAL) cmd->flags=VAL;
#define lcb_cmdgetreplica_create_all(ptr) (*(ptr))->strategy=LCB_REPLICA_ALL
#define lcb_cmdgetreplica_create_select(ptr, INDEX) (*(ptr))->strategy=LCB_REPLICA_SELECT; (*(ptr))->index=INDEX;
#define lcb_cmdgetreplica_create_first(ptr) (*(ptr))->strategy=LCB_REPLICA_FIRST
#define lcb_cmdgetreplica_expiration(CMD,TTL) (CMD)->exptime=TTL
#define PYCBC_ASSIGN(LHS,RHS) PYCBC_DEBUG_LOG_CONTEXT(context, "Assigning %s (%d) to %s", #RHS, RHS, #LHS); LHS=RHS;
#define lcb_cmdstore_create(CMD,OP)         PYCBC_ASSIGN((CMD)->operation,(lcb_storage_t) (OP));
#define lcb_cmdstore_destroy(...) 0
#define lcb_cmdstore_cas(CMD,CAS) PYCBC_ASSIGN((CMD)->cas,CAS);
#define lcb_cmdstore_expiration(CMD,TTL)             PYCBC_ASSIGN((CMD)->exptime, (lcb_U32) (TTL));

#endif // PYCBC_LCB_API>0x030000

#if PYCBC_LCB_API>0x030001
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
#define LCB_SDCMD_FULLDOC_GET (LCB_SDCMD_GET_COUNT+1)

/**
 * Add the entire document
 */
#define LCB_SDCMD_FULLDOC_ADD (LCB_SDCMD_GET_COUNT+2)

/**
 * Upsert the entire document
 */
#define LCB_SDCMD_FULLDOC_UPSERT (LCB_SDCMD_GET_COUNT+3)
/**
 * Replace the entire document
 */
#define LCB_SDCMD_FULLDOC_REPLACE (LCB_SDCMD_GET_COUNT+4)

/**
 * Remove the entire document
 */
#define LCB_SDCMD_FULLDOC_REMOVE (LCB_SDCMD_GET_COUNT+5)

#endif
#define CMDSUBDOC_F_UPSERT_DOC (1 << 16)
#define    CMDSUBDOC_F_INSERT_DOC (1 << 17)
#define     CMDSUBDOC_F_ACCESS_DELETED (1 << 18)

#else //  PYCBC_LCB_API>0x030001
typedef struct{
    lcb_SDSPEC* specs;
    size_t nspecs;
    lcb_U32 options;
}  lcb_SUBDOCOPS;

typedef lcb_SDSPEC pycbc_SDSPEC;
#endif //  PYCBC_LCB_API>0x030001

#if PYCBC_LCB_API<0x031000
#define lcb_cmdsubdoc_cas(CMD,CAS) (CMD)->cas=(lcb_U32)(CAS)
#define lcb_cmdsubdoc_expiration(CMD,EXP) (CMD)->exptime=(lcb_U32)(EXP)

typedef enum {
    LCB_REPLICA_MODE_ANY = 0x00,
    LCB_REPLICA_MODE_ALL = 0x01,
    LCB_REPLICA_MODE_IDX0 = 0x02,
    LCB_REPLICA_MODE_IDX1 = 0x03,
    LCB_REPLICA_MODE_IDX2 = 0x04,
    LCB_REPLICA_MODE__MAX
} lcb_REPLICA_MODE;


void lcb_cmdgetreplica_create(lcb_CMDGETREPLICA **pcmd, int strategy);
#endif


#if PYCBC_LCB_API<0x030001

#define lcb_resphttp_headers(htresp,dest) *(dest)= htresp->headers
//#define lcb_resphttp_status(htresp) htresp->rc
#define lcb_resphttp_http_status(resp,dest) *(dest)=(resp)->htstatus
#define lcb_resphttp_body(resp, bodybuffer, bodylength) *(bodybuffer)=resp->body; (*bodylength)=resp->nbody;
//#define lcb_resphttp_cookie(RESP,DEST) *(DEST)=RESP->cookie
#define lcb_respn1ql_http_response(INNER,DEST) *(DEST)=INNER->htresp;
#define lcb_respn1ql_row(INNER,ROW,ROW_COUNT) {*(ROW)=(INNER)->row; *(ROW_COUNT)=(INNER)->nrow;}
#define lcb_respn1ql_cookie(RESP,DEST) *(DEST)=(RESP)->cookie;
#define lcb_respn1ql_is_final(RESP) (RESP)->rflags & LCB_RESP_F_FINAL
#define lcb_respn1ql_status(RESP) (RESP)->rc
#define lcb_cmdn1ql_create(CMD) lcb_CMDN1QL cmd_real={0}; cmd=&cmd_real;
#define lcb_cmdn1ql_callback(CMD,CALLBACK) (CMD)->callback=(CALLBACK)
#define lcb_cmdn1ql_query(CMD,PARAMS,NPARAMS) (CMD)->query=PARAMS; (CMD)->nquery=NPARAMS;
#define lcb_cmdn1ql_handle(CMD,HANDLE) (CMD)->handle=HANDLE
#define lcb_cmdn1ql_adhoc(CMD,ENABLE) ((CMD)->cmdflags)&=(~LCB_CMDN1QL_F_PREPCACHE);((CMD)->cmdflags)|=(ENABLE?LCB_CMDN1QL_F_PREPCACHE:0);
#define lcb_cmdn1ql_parent_span(...) lcb_n1ql_set_parent_span(__VA_ARGS__)
#define lcb_n1ql(INSTANCE,COOKIE,QUERY)        lcb_n1ql_query(INSTANCE,COOKIE,QUERY)
#define lcb_respview_document(CTX,DEST) *(DEST)=CTX->docresp
#define lcb_respview_key(CTX,DEST,NDEST) *(DEST)=(CTX)->key; *(NDEST)=(CTX)->nkey;
#define lcb_respview_geometry(CTX,DEST,NDEST) *(DEST)=(CTX)->geometry; *(NDEST)=(CTX)->ngeometry;
#define lcb_respview_row(CTX,DEST,NDEST) *(DEST)=(CTX)->value; *(NDEST)=(CTX)->nvalue;
#define lcb_respview_doc_id(RESP,DOCID,NDOCID) *(DOCID)=(RESP)->docid; *(NDOCID)=(RESP)->ndocid;


#define lcb_http_cancel(instance, req) lcb_cancel_http_request(instance,req)
#define pycbc_resphttp_cookie(resp,type,target) (*((type*)(target)))=resp->cookie;
#define lcb_cmdhttp_create(CMD,TYPE) CMD->type=TYPE;

//#define lcb_cmdhttp_host(CMD, HOST, NHOST) (CMD)->host=HOST; PYCBC_DUMMY((CMD)->nhost=NHOST);
#define lcb_cmdhttp_body(HTCMD, BODY, NBODY) HTCMD->body=BODY; HTCMD->nbody=NBODY;
#define lcb_cmdhttp_content_type(HTCMD, CTYPE, CTYPELEN) HTCMD->content_type=CTYPE; PYCBC_DUMMY(HTCMD->ncontent_type=CTYPELEN;)
#define lcb_cmdhttp_method(HTCMD, METHOD) HTCMD->method=METHOD;
#define lcb_cmdhttp_handle(HTCMD, HANDLE) HTCMD->reqhandle=HANDLE;

#define lcb_respview_cookie(RESP, DEST) *(DEST)=(RESP)->cookie
#define lcb_respview_is_final(RESP) (RESP)->rflags & LCB_RESP_F_FINAL
#define lcb_respview_status(RESP) (RESP)->rc
#define lcb_cmdview_create(DEST) lcb_CMDVIEW cmd_real={0}; *(DEST)=&cmd_real;
#define lcb_cmdview_design_document(VCMD, DESIGN, NDESIGN) (VCMD)->ddoc=DESIGN; (VCMD)->nddoc=NDESIGN;
#define lcb_cmdview_view_name(VCMD, VIEW, NVIEW) (VCMD)->view=VIEW; (VCMD)->nview=NVIEW;
#define lcb_cmdview_option_string(VCMD, OPTSTR, NOPTSTR) (VCMD)->optstr=OPTSTR; (VCMD)->noptstr=NOPTSTR
#define lcb_cmdview_post_data(VCMD, BODY, NBODY) (VCMD)->postdata=BODY; (VCMD)->npostdata=NBODY
#define lcb_cmdview_handle(VCMD, HANDLE) (VCMD)->handle=HANDLE
#define lcb_cmdview_callback(VCMD, CALLBACK) (VCMD)->callback=CALLBACK;
#define lcb_cmdview_include_docs(VCMD, ENABLE) (VCMD)->cmdflags=((VCMD)->cmdflags&~LCB_CMDVIEWQUERY_F_INCLUDE_DOCS)|(ENABLE?LCB_CMDVIEWQUERY_F_INCLUDE_DOCS:0)
#define lcb_cmdview_no_row_parse(VCMD, ENABLE) (VCMD)->cmdflags=((VCMD)->cmdflags&~LCB_CMDVIEWQUERY_F_NOROWPARSE)|(ENABLE?LCB_CMDVIEWQUERY_F_NOROWPARSE:0)
#define lcb_cmdview_spatial(VCMD, ENABLE) (VCMD)->cmdflags=((VCMD)->cmdflags&~LCB_CMDVIEWQUERY_F_SPATIAL)|(ENABLE?LCB_CMDVIEWQUERY_F_SPATIAL:0)
#define lcb_cmdview_parent_span(...) lcb_view_set_parent_span(__VA_ARGS__)
#define lcb_cmdview_destroy(CMD)

#endif // PYCBC_LCB_API<0x030001
#if PYCBC_LCB_API<0x031000
#define lcb_cmdsubdoc_create(CMD) lcb_CMDSUBDOC cmd_real={0}; *(CMD)=&cmd_real;
#define lcb_cmdsubdoc_destroy(CMD)
#define lcb_cmdsubdoc_key(CMD,KEY,NKEY) LCB_CMD_SET_KEY(CMD,KEY,NKEY)
#endif

#if PYCBC_LCB_API<0x031000
#define lcb_respgetcid_cookie(RESP,DEST) *(DEST)=(RESP)->cookie;
#define lcb_respgetcid_status(RESP) (RESP)->rc
#define lcb_respgetcid_collection_id(RESP,DEST) *(DEST)=(RESP)->collection_id
#define lcb_respgetcid_manifest_id(RESP,DEST) *(DEST)=(RESP)->manifest_id
#endif

#define CMDSCOPE_SDCMD_CREATE_RAW_V4(TYPE, LC, CMD, ...) \
    TYPE* CMD=NULL;lcb_##LC##_create(&CMD)

#define CMDSCOPE_SDCMD_CREATE_V4(TYPE, LC, CMD, ...) \
    TYPE* CMD=NULL;lcb_##LC##_create(&(CMD), __VA_ARGS__);

#define CMDSCOPE_SDCMD_DESTROY_V4(TYPE, LC, CMD, ...) \
    lcb_##LC##_destroy(CMD)

#define CMDSCOPE_SDCMD_DESTROY_RAW_V4(TYPE, LC, CMD, ...) \
    lcb_##LC##_destroy(CMD)

#ifdef PYCBC_V4_BACKPORT
#define ACCESSORS(X)\
    X(respget,itmflags,flags)

#define PYCBC_STRUCT_ACC(prefix.structname,accname,...)\
lcb_error_t lcb_##prefix##_accname(__VA_ARGS__){}

#endif

uint64_t pycbc_mutation_token_seqno(const lcb_MUTATION_TOKEN *pToken);

uint64_t pycbc_mutation_token_vbid(const lcb_MUTATION_TOKEN *pToken);

uint64_t pycbc_mutation_token_uuid(const lcb_MUTATION_TOKEN *pToken);
#if PYCBC_LCB_API<0x031000
int lcb_mutation_token_is_valid(const lcb_MUTATION_TOKEN *pTOKEN);
#endif

typedef lcb_CMDBASE* pycbc_CMDBASE;
typedef lcb_CMDGET* pycbc_CMDGET;
typedef lcb_CMDTOUCH* pycbc_CMDTOUCH;
typedef lcb_CMDGETREPLICA* pycbc_CMDGETREPLICA;
typedef lcb_CMDREMOVE* pycbc_CMDREMOVE;
typedef lcb_CMDUNLOCK* pycbc_CMDUNLOCK;

#if PYCBC_LCB_API<0x031000
typedef lcb_CMDENDURE* pycbc_CMDENDURE;
#endif

typedef lcb_CMDHTTP* pycbc_CMDHTTP;
typedef lcb_CMDSTORE* pycbc_CMDSTORE;

#define CMDSCOPE_CREATECMD_RAW_V4(UC, LC, CMD, ...) \
    lcb_CMD##UC* CMD=NULL;lcb_cmd##LC##_create(&CMD)

#define CMDSCOPE_CREATECMD_V4(UC, LC, CMD, ...) \
    lcb_CMD##UC* CMD=NULL;lcb_cmd##LC##_create(&CMD, __VA_ARGS__)

#define CMDSCOPE_DESTROYCMD_V4(UC, LC, CMD, ...) \
    lcb_cmd##LC##_destroy(CMD)

#define CMDSCOPE_DESTROYCMD_RAW_V4(UC, LC, CMD, ...) \
    lcb_cmd##LC##_destroy(CMD)
#define CMDSCOPE_CREATECMD_RAW_V3(UC, LC, CMD, ...) \
    lcb_CMD##UC CMD##_real={0};lcb_CMD##UC* CMD=&CMD##_real;

#define CMDSCOPE_CREATECMD_V3(UC, LC, CMD, ...) \
    lcb_CMD##UC CMD##_real={0};lcb_CMD##UC* CMD=&CMD##_real;lcb_cmd##LC##_create(CMD, __VA_ARGS__)

#define CMDSCOPE_DESTROYCMD_V3(UC, LC, CMD, ...) 0

#define CMDSCOPE_DESTROYCMD_RAW_V3(UC, LC, CMD, ...) 0

#ifdef PYCBC_V4
#define CMDSCOPE_CREATECMD_RAW(UC, LC, CMD, ...) \
    CMDSCOPE_CREATECMD_RAW_V4(UC,LC,CMD,__VA_ARGS__)

#define CMDSCOPE_CREATECMD(UC, LC, CMD, ...) \
    CMDSCOPE_CREATECMD_V4(UC,LC,CMD,__VA_ARGS__)

#define CMDSCOPE_DESTROYCMD(UC, LC, CMD, ...) \
    CMDSCOPE_DESTROYCMD_V4(UC,LC,CMD,__VA_ARGS__)

#define CMDSCOPE_DESTROYCMD_RAW(UC, LC, CMD, ...) \
    CMDSCOPE_DESTROYCMD_RAW_V4(UC,LC,CMD,__VA_ARGS__)

#else
#define CMDSCOPE_CREATECMD_RAW(UC, LC, CMD, ...) \
    CMDSCOPE_CREATECMD_RAW_V3(UC,LC,CMD,__VA_ARGS__)

#define CMDSCOPE_CREATECMD(UC, LC, CMD, ...) \
    CMDSCOPE_CREATECMD_V3(UC,LC,CMD,__VA_ARGS__)

#define CMDSCOPE_DESTROYCMD(UC, LC, CMD, ...) \
    CMDSCOPE_DESTROYCMD_V3(UC,LC,CMD,__VA_ARGS__)

#define CMDSCOPE_DESTROYCMD_RAW(UC, LC, CMD, ...) \
    CMDSCOPE_DESTROYCMD_RAW_V3(UC,LC,CMD,__VA_ARGS__)

#endif


#define CMDSCOPE_NG_V4(UC, LC) CMDSCOPE_GENERIC_ALL(UC, LC, CMDSCOPE_CREATECMD_RAW_V4, CMDSCOPE_DESTROYCMD_RAW_V4, cmd)
#define CMDSCOPE_NG_PARAMS_V4(UC, LC, ...) CMDSCOPE_GENERIC_ALL(UC,LC, CMDSCOPE_CREATECMD_V4, CMDSCOPE_DESTROYCMD_V4, cmd, __VA_ARGS__)
#define CMDSCOPE_NG(UC, LC) CMDSCOPE_GENERIC_ALL(UC, LC, CMDSCOPE_CREATECMD_RAW, CMDSCOPE_DESTROYCMD_RAW, cmd)
#define CMDSCOPE_NG_PARAMS(UC, LC, ...) CMDSCOPE_GENERIC_ALL(UC,LC, CMDSCOPE_CREATECMD, CMDSCOPE_DESTROYCMD, cmd, __VA_ARGS__)
#define CMDSCOPE_NG_GENERIC(TYPE, LC, CMDNAME, ...) CMDSCOPE_GENERIC_ALL(TYPE,LC, CMDSCOPE_SDCMD_CREATE_V4, CMDSCOPE_SDCMD_DESTROY_V4, CMDNAME, __VA_ARGS__)
#define CMDSCOPE_NG_GENERIC_PARAMS(PREFIX,TYPE, LC, CMDNAME, ...) CMDSCOPE_GENERIC_ALL_PREFIX(PREFIX,TYPE,LC, CMDSCOPE_SDCMD_CREATE_V4, CMDSCOPE_SDCMD_DESTROY_RAW_V4, CMDNAME, __VA_ARGS__)

#if PYCBC_LCB_API >0x030000
/** Set this flag to execute an actual get with each response */
#define LCB_CMDVIEWQUERY_F_INCLUDE_DOCS (1 << 16)

/**This view is spatial. Modifies how the final view path will be constructed */
#define LCB_CMDVIEWQUERY_F_SPATIAL (1 << 18)
/**Set this flag to only parse the top level row, and not its constituent
 * parts. Note this is incompatible with `F_INCLUDE_DOCS`*/
#define LCB_CMDVIEWQUERY_F_NOROWPARSE (1 << 17)
#endif


typedef struct {
    PyObject_HEAD

    /** LCB instance */
    lcb_INSTANCE* instance;
#ifdef PYCBC_TRACING
    /** Tracer **/
    struct pycbc_Tracer *tracer;
    PyObject *parent_tracer;
#endif
    /** Transcoder object */
    PyObject *tc;

    /** Default format, PyInt */
    PyObject *dfl_fmt;

    /** Callback to be invoked when connected */
    PyObject *conncb;

    /**
     * Callback to be invoked upon destruction. Because we can fall out
     * of scope in middle of an LCB function, this is required.
     *
     * The dtorcb is first called when the refcount of the connection
     */
    PyObject *dtorcb;

    /**
     * Test hook for reacting to durability/persistence settings from within
     * mutator functions
     */
    PyObject *dur_testhook;


    /** String bucket */
    PyObject *bucket;

    /** Bucket type */
    PyObject *btype;

    /** Pipeline MultiResult container */
    PyObject *pipeline_queue;

    /** If using a custom IOPS, this contains it */
    PyObject *iopswrap;

    /** Thread state. Used to lock/unlock the GIL */
    PyThreadState *thrstate;

    PyThread_type_lock lock;
    unsigned int lockmode;

    /** Whether to not raise any exceptions */
    unsigned int quiet;

    /** Whether GIL handling is in effect */
    unsigned int unlock_gil;

    /** Don't decode anything */
    unsigned int data_passthrough;

    /** whether __init__ has already been called */
    unsigned char init_called;

    /** How many operations are waiting for a reply */
    Py_ssize_t nremaining;

    unsigned int flags;

    pycbc_dur_params dur_global;
    unsigned long dur_timeout;

} pycbc_Bucket;

typedef struct {
    pycbc_strn_unmanaged collection;
    pycbc_strn_unmanaged scope;
} pycbc_Collection_coords;

typedef struct  {
    PyObject_HEAD
    pycbc_Bucket* bucket;
    pycbc_Collection_coords collection;
} pycbc_Collection;

typedef struct {
    lcb_U64 manifest_id;
    lcb_U32 collection_id;
} pycbc_coll_res_success_t;

typedef struct {
    pycbc_coll_res_success_t value;
    lcb_error_t err;
} pycbc_coll_res_t;

typedef struct {
    pycbc_coll_res_t result;
    pycbc_Collection* coll;
} pycbc_coll_context;
/*
lcb_error_t pycbc_Collection_get_cid_async(pycbc_Collection *collection, pycbc_coll_res_t *result);
pycbc_coll_res_t pycbc_Collection_get_cid(pycbc_Collection *collection);
*/
#ifdef PYCBC_NATIVE_COLLECTIONS
pycbc_Unit pycbc_Bucket_init_collection(pycbc_Collection* bucket, PyObject* args, PyObject* kwargs){
    return bucket;
}
#else
pycbc_Collection* pycbc_Bucket_init_collection(pycbc_Bucket* bucket, PyObject* args, PyObject* kwargs);

#define PYCBC_COLLECTION_XARGS(X)\
    X("collection",&collection,"O")
#endif

#ifdef PYCBC_TRACING
void *pycbc_capsule_value_or_null(PyObject *capsule, const char *capsule_name);

typedef struct pycbc_Tracer {
    PyObject_HEAD
    lcbtrace_TRACER *tracer;
} pycbc_Tracer_t;

typedef struct {
    PyObject_HEAD
#ifdef PYCBC_TRACING
    lcbtrace_SPAN *span;
#endif
} pycbc_Span_t;

typedef struct pycbc_context_children_t {
    pycbc_stack_context_handle value;
    struct pycbc_context_children_t *next;
} pycbc_context_children;

typedef struct pycbc_stack_context_decl {
#ifdef PYCBC_TRACING
    int is_stub;
    pycbc_Tracer_t* tracer;
    lcbtrace_SPAN* span;
    pycbc_stack_context_handle parent;
    size_t ref_count;
#ifdef PYCBC_REF_ACCOUNTING
#ifdef PYCBC_INLINE_ACC
    pycbc_stack_context_handle next;
    pycbc_stack_context_handle first_child;
#else
    pycbc_context_children *acc_node;
    pycbc_context_children *children;
#endif
#endif
#ifdef PYCBC_TABBED_CONTEXTS
    size_t depth;
#endif
#endif
} pycbc_stack_context;

pycbc_strn pycbc_get_string_tag_basic(lcbtrace_SPAN *span, const char *tagname);
PyObject *pycbc_Context_capsule(pycbc_stack_context_handle context);
void pycbc_Context_capsule_destructor(PyObject *context_capsule);
void *pycbc_Context_capsule_value(PyObject *context_capsule);

#define PYCBC_RES_CONTEXT(MRES) (MRES)?(MRES)->tracing_context:NULL

#else
#define PYCBC_RES_CONTEXT(MRES) NULL

#endif

typedef struct pycbc_Result pycbc_Result_t;
typedef struct pycbc_MultiResult_st pycbc_MultiResult;
void pycbc_set_dict_kv_object(PyObject *dict,
                              PyObject *key,
                              const char *value_str);


void pycbc_set_kv_ull(PyObject *dict, PyObject *keystr, lcb_uint64_t parenti_id);
void pycbc_set_kv_ull_str(PyObject *dict, const char *keystr, lcb_uint64_t parenti_id);
#define GENERIC_OPERAND(SCOPE, COMMAND, INSTANCE, HANDLE, CONTEXT, ...)\
    lcb_##SCOPE##_##COMMAND(INSTANCE, __VA_ARGS__);

#ifdef PYCBC_TRACING
int pycbc_is_async_or_pipeline(const pycbc_Bucket *self);

pycbc_stack_context_handle pycbc_Result_start_context(
        pycbc_stack_context_handle parent_context,
        PyObject *hkey,
        const char *component,
        char *operation,
        lcbtrace_REF_TYPE ref_type);
void pycbc_Result_propagate_context(pycbc_Result_t *res,
                                    pycbc_stack_context_handle parent_context,
                                    pycbc_Bucket *bucket);
void pycbc_MultiResult_init_context(pycbc_MultiResult *self,
                                    PyObject *curkey,
                                    pycbc_stack_context_handle context,
                                    pycbc_Bucket *bucket);
pycbc_stack_context_handle pycbc_MultiResult_extract_context(
        pycbc_MultiResult *self, PyObject *hkey, pycbc_Result_t **res);
#define PYCBC_MULTIRESULT_EXTRACT_CONTEXT(MRES, KEY, RES) \
    pycbc_MultiResult_extract_context(MRES, KEY, RES)
pycbc_stack_context_handle pycbc_Result_extract_context(
        const pycbc_Result_t *res);
#define PYCBC_RESULT_EXTRACT_CONTEXT(RESULT) \
    pycbc_Result_extract_context(RESULT)

pycbc_stack_context_handle pycbc_Context_init(
        pycbc_Tracer_t *py_tracer,
        const char *operation,
        lcb_uint64_t now,
        pycbc_stack_context_handle ref_context,
        lcbtrace_REF_TYPE ref_type,
        const char *component);

pycbc_stack_context_handle pycbc_Context_init_debug(
        const char *FILE,
        int LINE,
        const char *FUNC,
        pycbc_Tracer_t *py_tracer,
        const char *operation,
        lcb_uint64_t now,
        pycbc_stack_context_handle ref_context,
        lcbtrace_REF_TYPE ref_type,
        const char *component);
#define PYCBC_CONTEXT_INIT(                                          \
        py_tracer, operation, now, ref_context, ref_type, component) \
    pycbc_Context_init_debug(__FILE__,                               \
                             __LINE__,                               \
                             __FUNCTION_NAME__,                      \
                             py_tracer,                              \
                             operation,                              \
                             now,                                    \
                             ref_context,                            \
                             ref_type,                               \
                             component)

pycbc_stack_context_handle pycbc_Context_check(
        pycbc_stack_context_handle context,
        const char *file,
        const char *func,
        int line);

#ifdef PYCBC_DEBUG
#define PYCBC_CHECK_CONTEXT(CONTEXT) \
    pycbc_Context_check(CONTEXT, __FILE__, __FUNCTION_NAME__, __LINE__)
#else
#define PYCBC_CHECK_CONTEXT(CONTEXT) \
    pycbc_Context_check(CONTEXT, __FILE__, "N/A", __LINE__)
#endif

pycbc_stack_context_handle pycbc_Context_deref(
        pycbc_stack_context_handle context,
        int should_be_final,
        int account_for_children,
        pycbc_stack_context_handle from_context);
pycbc_stack_context_handle pycbc_Context_deref_debug(
        const char *file,
        const char *func,
        int line,
        pycbc_stack_context_handle context,
        int should_be_final,
        int dealloc_children,
        pycbc_stack_context_handle from_context);

#ifdef PYCBC_DEBUG
#define PYCBC_CONTEXT_DEREF_FROM_CONTEXT(                         \
        CONTEXT, SHOULD_BE_FINAL, DEALLOC_CHILDREN, FROM_CONTEXT) \
    pycbc_Context_deref_debug(__FILE__,                           \
                              __FUNCTION_NAME__,                  \
                              __LINE__,                           \
                              CONTEXT,                            \
                              SHOULD_BE_FINAL,                    \
                              DEALLOC_CHILDREN,                   \
                              FROM_CONTEXT);
#else
#define PYCBC_CONTEXT_DEREF_FROM_CONTEXT(                         \
        CONTEXT, SHOULD_BE_FINAL, DEALLOC_CHILDREN, FROM_CONTEXT) \
    pycbc_Context_deref(      CONTEXT,                            \
                              SHOULD_BE_FINAL,                    \
                              DEALLOC_CHILDREN,                   \
                              FROM_CONTEXT);

#endif

#ifdef PYCBC_DEBUG
#define PYCBC_CONTEXT_DEREF(CONTEXT, SHOULD_BE_FINAL) \
    pycbc_Context_deref_debug(__FILE__,               \
                              __FUNCTION_NAME__,      \
                              __LINE__,               \
                              CONTEXT,                \
                              SHOULD_BE_FINAL,        \
                              1,                      \
                              NULL)
#else
#define PYCBC_CONTEXT_DEREF(CONTEXT, SHOULD_BE_FINAL) \
    pycbc_Context_deref(CONTEXT, SHOULD_BE_FINAL, 1, NULL)
#endif

size_t pycbc_Context_get_ref_count(pycbc_stack_context_handle context);
size_t pycbc_Context_get_ref_count_debug(const char *FILE,
                                         const char *FUNC,
                                         int line,
                                         pycbc_stack_context_handle context);
#ifdef PYCBC_DEBUG
#define PYCBC_CONTEXT_GET_REF_COUNT(CONTEXT) \
    pycbc_Context_get_ref_count_debug(       \
            __FILE__, __FUNCTION_NAME__, __LINE__, CONTEXT)
#else
#define PYCBC_CONTEXT_GET_REF_COUNT(CONTEXT) \
    pycbc_Context_get_ref_count(CONTEXT)
#endif

void pycbc_ref_context(pycbc_stack_context_handle parent_context);
#define PYCBC_REF_CONTEXT(CONTEXT)                                     \
    PYCBC_DEBUG_LOG_CONTEXT(                                           \
            CONTEXT,                                                   \
            "starting reffing context %p cv %llu",                     \
            CONTEXT,                                                   \
            (long long unsigned)pycbc_Context_get_ref_count(CONTEXT)); \
    pycbc_ref_context(CONTEXT);                                        \
    PYCBC_DEBUG_LOG_CONTEXT(                                           \
            CONTEXT,                                                   \
            "finished reffing context %p to get %llu",                 \
            CONTEXT,                                                   \
            (long long unsigned)pycbc_Context_get_ref_count(CONTEXT));

pycbc_stack_context_handle pycbc_Tracer_start_span(
        pycbc_Tracer_t *py_tracer,
        PyObject *kwargs,
        const char *operation,
        lcb_uint64_t now,
        pycbc_stack_context_handle *context,
        lcbtrace_REF_TYPE ref_type,
        const char *component);
pycbc_stack_context_handle pycbc_Tracer_start_span_debug(
        const char *FILE,
        int LINE,
        const char *FUNCTION,
        pycbc_Tracer_t *py_tracer,
        PyObject *kwargs,
        const char *operation,
        lcb_uint64_t now,
        pycbc_stack_context_handle *context,
        lcbtrace_REF_TYPE ref_type,
        const char *component);

#ifdef PYCBC_DEBUG
#define PYCBC_TRACER_START_SPAN(...) \
    pycbc_Tracer_start_span_debug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define PYCBC_TRACER_START_SPAN(...) pycbc_Tracer_start_span(__VA_ARGS__)
#endif

void pycbc_Tracer_propagate(pycbc_Tracer_t *tracer);
void pycbc_Tracer_set_child(pycbc_Tracer_t *pTracer, lcbtrace_TRACER *pTRACER);

#define PYCBC_TRACE_GET_STACK_CONTEXT_TOPLEVEL(KWARGS, CATEGORY, TRACER, NAME) \
    PYCBC_TRACER_START_SPAN(                                                   \
            TRACER, KWARGS, CATEGORY, 0, NULL, LCBTRACE_REF_NONE, NAME)

#if PYCBC_LCB_API<0x031000
#define lcb_cmdsubdoc_parent_span(CMD, SPAN)                                   \
  LCB_CMD_SET_TRACESPAN((CMD), (SPAN));
#define lcb_cmdobserve_parent_span(CMD, SPAN)                                  \
  LCB_CMD_SET_TRACESPAN((CMD), (SPAN));
#define lcb_cmdendure_parent_span(CMD, ...)                                    \
  LCB_CMD_SET_TRACESPAN((CMD), __VA_ARGS__);

#else
#define lcb_cmdobserve_parent_span(CMD, SPAN)                                  \
  LCB_CMD_SET_TRACESPAN((CMD), (SPAN));
#endif

  #if PYCBC_LCB_API<0x030001
#define PYCBC_CMD_SET_TRACESPAN(TYPE, CMD, SPAN)                               \
  LCB_CMD_SET_TRACESPAN((CMD), (SPAN));
#else

#define PYCBC_CMD_SET_TRACESPAN(TYPE, CMD, SPAN)                               \
  lcb_cmd##TYPE##_parent_span((CMD), (SPAN));
#endif


#ifndef PYCBC_V4

#define lcb_cmdfts_parent_span(...)     lcb_fts_set_parent_span(__VA_ARGS__)
#define GENERIC_SPAN_OPERAND(SCOPE, INSTANCE, CMD, HANDLE, CONTEXT) \
    lcb_cmd##SCOPE##_parent_span(INSTANCE, HANDLE, (CONTEXT)->span)
#define GENERIC_NULL_OPERAND(SCOPE, COMMAND, INSTANCE, HANDLE, CONTEXT, ...)\
    lcb_##SCOPE(INSTANCE, __VA_ARGS__);
#else
#if PYCBC_LCB_API<0x030000
#define GENERIC_SPAN_OPERAND(SCOPE, INSTANCE, CMD, HANDLE, CONTEXT)

#else
#define GENERIC_SPAN_OPERAND(SCOPE, INSTANCE, CMD, HANDLE, CONTEXT) \
    lcb_cmd##SCOPE##_parent_span(CMD, (CONTEXT)->span)
#define GENERIC_NULL_OPERAND(SCOPE, COMMAND, INSTANCE, HANDLE, CONTEXT, ...)\
    lcb_##SCOPE(INSTANCE, __VA_ARGS__);
#endif
#endif

#ifndef PYCBC_V4
#define PYCBC_LOG_KEY(CMD,key)\
PYCBC_DEBUG_LOG("setting trace span on %.*s\n",        \
                            (int)(CMD)->key.contig.nbytes,          \
                            (const char *)(CMD)->key.contig.bytes);
#else
#define PYCBC_LOG_KEY(CMD,key)
#endif

#define PYCBC_TRACECMD_PURE(TYPE,CMD, CONTEXT)                          \
    {                                                              \
        if (PYCBC_CHECK_CONTEXT(CONTEXT)) {                        \
            PYCBC_LOG_KEY(CMD,key)\
            PYCBC_CMD_SET_TRACESPAN(TYPE,(CMD), (CONTEXT)->span);        \
        } else {                                                   \
            PYCBC_EXCEPTION_LOG_NOCLEAR;                           \
        }                                                          \
    }



#define PYCBC_TRACECMD_TYPED(TYPE,CMD, CONTEXT, MRES, CURKEY, BUCKET) \
    PYCBC_TRACECMD_PURE(TYPE,CMD, CONTEXT);                     \
    pycbc_MultiResult_init_context(MRES, CURKEY, CONTEXT, BUCKET);
#define PYCBC_TRACECMD(CMD, CONTEXT, MRES, CURKEY, BUCKET) \
    PYCBC_TRACECMD_PURE(,CMD, CONTEXT);                     \
    pycbc_MultiResult_init_context(MRES, CURKEY, CONTEXT, BUCKET);

#define PYCBC_TRACE_POP_CONTEXT(CONTEXT) PYCBC_CONTEXT_DEREF((CONTEXT), 1);

pycbc_stack_context_handle pycbc_wrap_setup(const char *CATEGORY,
                                            const char *NAME,
                                            pycbc_Tracer_t *TRACER,
                                            const char *STRINGNAME,
                                            PyObject *KWARGS);

void pycbc_wrap_teardown(pycbc_stack_context_handle sub_context,
                         pycbc_Bucket *self,
                         const char *NAME,
                         PyObject **RV);

#define PYCBC_TRACE_WRAP_TOPLEVEL_WITHNAME(                                    \
        RV, CATEGORY, NAME, TRACER, STRINGNAME, ...)                           \
    {                                                                          \
        pycbc_stack_context_handle sub_context =                               \
                pycbc_wrap_setup(CATEGORY, #NAME, TRACER, STRINGNAME, kwargs); \
        RV = NAME(__VA_ARGS__, sub_context);                                   \
        pycbc_wrap_teardown(sub_context, self, #NAME, &RV);                     \
    }

typedef struct pycbc_common_vars pycbc_common_vars_t;
int pycbc_wrap_and_pop(pycbc_stack_context_handle *contextptr,
                       int noterv,
                       int result,
                       pycbc_common_vars_t *cv);
pycbc_stack_context_handle pycbc_logging_monad(const char *FILE,
                        int LINE,
                        const char *FUNC,
                        const char *NAME,
                        pycbc_stack_context_handle context);

int pycbc_wrap_and_pop_debug(const char *FILE,
                             int LINE,
                             const char *FUNC,
                             const char *NAME,
                             pycbc_stack_context_handle *contextptr,
                             int noterv,
                             int result,
                             pycbc_common_vars_t *cv);

#define PYCBC_WRAP_AND_POP(CONTEXTPTR, RESULT, NAME, NOTERV, CV) \
    pycbc_wrap_and_pop_debug(__FILE__,                           \
                             __LINE__,                           \
                             __FUNCTION__,                       \
                             NAME,                               \
                             CONTEXTPTR,                         \
                             NOTERV,                             \
                             (RESULT),                           \
                             CV)

pycbc_stack_context_handle pycbc_explicit_named_setup(
        const char *FILE,
        int LINE,
        const char *FUNCTION,
        pycbc_stack_context_handle *CONTEXTPTR,
        const char *COMPONENTNAME,
        const char *CATEGORY,
        PyObject *KWARGS,
        pycbc_Tracer_t *self);

#define PYCBC_EXPLICIT_NAMED_SETUP(                        \
        CONTEXTPTR, COMPONENTNAME, CATEGORY, KWARGS, SELF) \
    pycbc_explicit_named_setup(__FILE__,                   \
                               __LINE__,                   \
                               __FUNCTION__,               \
                               CONTEXTPTR,                 \
                               COMPONENTNAME,              \
                               CATEGORY,                   \
                               KWARGS,                     \
                               SELF)

#define PYCBC_TRACE_WRAP_EXPLICIT_NAMED(CONTEXTPTR,                           \
                                        NAME,                                 \
                                        COMPONENTNAME,                        \
                                        CATEGORY,                             \
                                        KWARGS,                               \
                                        NOTERV,                               \
                                        CV,                                   \
                                        SELF,                                 \
                                        ...)                                  \
    PYCBC_WRAP_AND_POP(                                                       \
            CONTEXTPTR,                                                       \
            NAME(__VA_ARGS__,                                                 \
                 PYCBC_EXPLICIT_NAMED_SETUP(                                  \
                         CONTEXTPTR, COMPONENTNAME, CATEGORY, KWARGS, SELF->tracer)), \
            COMPONENTNAME,                                                    \
            NOTERV,                                                           \
            CV)



#define PYCBC_TRACE_WRAP_TOPLEVEL(RV, CATEGORY, NAME, TRACER, ...) \
    PYCBC_TRACE_WRAP_TOPLEVEL_WITHNAME(                            \
            RV, CATEGORY, NAME, TRACER, #NAME, __VA_ARGS__)

#else

#define PYCBC_CONTEXT_DEREF(X, Y)
#define PYCBC_CONTEXT_DEREF_FROM_CONTEXT(                         \
        CONTEXT, SHOULD_BE_FINAL, DEALLOC_CHILDREN, FROM_CONTEXT)
#define PYCBC_GET_STACK_CONTEXT(CATEGORY,TRACER, PARENT_CONTEXT) NULL
#define PYCBC_MULTIRESULT_EXTRACT_CONTEXT(MRES, KEY, RES) NULL
#define PYCBC_RESULT_EXTRACT_CONTEXT(RESULT) NULL
#define PYCBC_TRACECMD(...)
#define PYCBC_TRACECMD_TYPED(...)

#define PYCBC_TRACECMD_PURE(...)
#define PYCBC_TRACE_POP_CONTEXT(X) NULL
#define PYCBC_FINISH_IF_COMPLETE(SELF, CONTEXT)
#define PYCBC_TRACE_WRAP_TOPLEVEL_WITHNAME(RV, CATEGORY, NAME, TRACER, STRINGNAME, ...) { RV = NAME(__VA_ARGS__, NULL); }
#define PYCBC_WRAP_AND_POP(CPTR,VAL,CNAME,NOTERV,CV) VAL
#define PYCBC_EXPLICIT_NAMED_SETUP(...) NULL

#define PYCBC_TRACE_WRAP_TOPLEVEL(RV,CATEGORY,NAME,TRACER,...) { RV=NAME(__VA_ARGS__,NULL); }
#define PYCBC_TRACE_GET_STACK_CONTEXT_TOPLEVEL(...) NULL
#define GENERIC_SPAN_OPERAND(SCOPE, INSTANCE, CMD, HANDLE, CONTEXT) \
    lcb_cmd##SCOPE##_parent_span(INSTANCE, HANDLE, (CONTEXT)->span)
#define GENERIC_NULL_OPERAND(SCOPE, COMMAND, INSTANCE, HANDLE, CONTEXT, ...)\
    lcb_##SCOPE(INSTANCE, __VA_ARGS__);

#endif

#define PYCBC_TRACE_WRAP_EXPLICIT_NAMED_VOID(                             \
        CONTEXTPTR, NAME, COMPONENTNAME, CATEGORY, KWARGS, CV, SELF, ...) \
    {                                                                     \
        NAME(__VA_ARGS__,                                                 \
             PYCBC_EXPLICIT_NAMED_SETUP(                                  \
                     CONTEXTPTR, COMPONENTNAME, CATEGORY, KWARGS, SELF->tracer)); \
        (void)PYCBC_WRAP_AND_POP(CONTEXTPTR, 0, COMPONENTNAME, 0, CV);          \
    }


#define PYCBC_TRACE_WRAP_VOID(NAME, KWARGS, CONTEXT, SELF, ...) \
    PYCBC_TRACE_WRAP_EXPLICIT_NAMED_VOID(CONTEXT,               \
                                         NAME,                  \
                                         #NAME,                 \
                                         NAME##_category(),     \
                                         KWARGS,                \
                                         NULL,                  \
                                         SELF,                  \
                                         __VA_ARGS__)

#define PYCBC_TRACE_WRAP_EXPLICIT_NAMED(CONTEXTPTR,                           \
                                        NAME,                                 \
                                        COMPONENTNAME,                        \
                                        CATEGORY,                             \
                                        KWARGS,                               \
                                        NOTERV,                               \
                                        CV,                                   \
                                        SELF,                                 \
                                        ...)                                  \
    PYCBC_WRAP_AND_POP(                                                       \
            CONTEXTPTR,                                                       \
            NAME(__VA_ARGS__,                                                 \
                 PYCBC_EXPLICIT_NAMED_SETUP(                                  \
                         CONTEXTPTR, COMPONENTNAME, CATEGORY, KWARGS, SELF->tracer)), \
            COMPONENTNAME,                                                    \
            NOTERV,                                                           \
            CV)

#define PYCBC_TRACE_WRAP_NOTERV(NAME, KWARGS, NOTERV, CV, CONTEXT, SELF, ...) \
    PYCBC_TRACE_WRAP_EXPLICIT_NAMED(CONTEXT,                                  \
                                    NAME,                                     \
                                    #NAME,                                    \
                                    NAME##_category(),                        \
                                    KWARGS,                                   \
                                    NOTERV,                                   \
                                    CV,                                       \
                                    SELF,                                     \
                                    __VA_ARGS__)

#define PYCBC_TRACE_WRAP(NAME, KWARGS, ...) \
    PYCBC_TRACE_WRAP_NOTERV(NAME, KWARGS, 0, NULL, &context, self, __VA_ARGS__)

#define PYCBC_TRACECMD_SCOPED_GENERIC(                               \
        RV, SCOPE, COMMAND, INSTANCE, CMD, HANDLE, CONTEXT, SPAN_OPERAND, OPERAND, ...) \
    RV = OPERAND(SCOPE,COMMAND,INSTANCE,HANDLE, CONTEXT, __VA_ARGS__);

#define PYCBC_TRACECMD_SCOPED(                              \
        RV, SCOPE, COMMAND, INSTANCE, CMD, HANDLE, CONTEXT, ...) \
    PYCBC_TRACECMD_SCOPED_GENERIC(RV,                       \
                                  SCOPE,                    \
                                  COMMAND,                  \
                                  INSTANCE,                 \
                                  CMD,\
                                  HANDLE,                   \
                                  CONTEXT,                  \
                                  GENERIC_SPAN_OPERAND,          \
                                  GENERIC_OPERAND, \
                                  __VA_ARGS__)

#define PYCBC_TRACECMD_SCOPED_NULL(                              \
        RV, SCOPE, INSTANCE, CMD, HANDLE, CONTEXT, ...) \
    PYCBC_TRACECMD_SCOPED_GENERIC(RV,                       \
                                  SCOPE,                    \
                                  ,                  \
                                  INSTANCE,                 \
                                  CMD,\
                                  HANDLE,                   \
                                  CONTEXT,                  \
                                  GENERIC_SPAN_OPERAND,          \
                                  GENERIC_NULL_OPERAND, \
                                  __VA_ARGS__)


#define TRACED_FUNCTION(CATEGORY,QUALIFIERS,RTYPE,NAME,...)\
    const char* NAME##_category(void){ return CATEGORY; }\
    QUALIFIERS RTYPE NAME(__VA_ARGS__, pycbc_stack_context_handle context)

#define TRACED_FUNCTION_DECL(QUALIFIERS,RTYPE,NAME,...)\
    const char* NAME##_category(void);\
    QUALIFIERS RTYPE NAME(__VA_ARGS__, pycbc_stack_context_handle context);

#define TRACED_FUNCTION_WRAPPER(name, CATEGORY, CLASS)                    \
    PyObject *pycbc_##CLASS##_##name##_real(                              \
            pycbc_##CLASS *self,                                          \
            PyObject *args,                                               \
            PyObject *kwargs,                                             \
            pycbc_stack_context_handle context);                          \
    PyObject *pycbc_##CLASS##_##name(                                     \
            pycbc_##CLASS *self, PyObject *args, PyObject *kwargs)        \
    {                                                                     \
        PyObject *result;                                                 \
        PYCBC_TRACE_WRAP_TOPLEVEL_WITHNAME(result,                        \
                                           CATEGORY,                      \
                                           pycbc_##CLASS##_##name##_real, \
                                           self->tracer,                  \
                                           #CLASS "." #name,              \
                                           self,                          \
                                           args,                          \
                                           kwargs);                       \
        return result;                                                    \
    }                                                                     \
    PyObject *pycbc_##CLASS##_##name##_real(                              \
            pycbc_##CLASS *self,                                          \
            PyObject *args,                                               \
            PyObject *kwargs,                                             \
            pycbc_stack_context_handle context)

/*****************
 * Result Objects.
 *****************
 *
 * These objects are returned to indicate the status/value of operations.
 * The following defines a 'base' class and several 'subclasses'.
 *
 * See result.c and opresult.c
 */

#ifdef PYCBC_TRACING
#define TRACING_DATA                            \
    pycbc_stack_context_handle tracing_context; \
    char is_tracing_stub;                       \
    PyObject *tracing_output;
#else
#define TRACING_DATA
#endif

#define pycbc_Result_HEAD \
    PyObject_HEAD \
    lcb_error_t rc; \
    PyObject *key; \
    TRACING_DATA

#define pycbc_OpResult_HEAD \
    pycbc_Result_HEAD \
    lcb_uint64_t cas; \
    PyObject *mutinfo;

typedef struct pycbc_Result {
    pycbc_Result_HEAD
} pycbc_Result;

typedef struct {
    pycbc_OpResult_HEAD
} pycbc_OperationResult;


#define pycbc_ValResult_HEAD \
    pycbc_OpResult_HEAD \
    PyObject *value; \
    lcb_uint32_t flags;

typedef struct {
    pycbc_ValResult_HEAD
} pycbc_ValueResult;

/**
 * Item or 'Document' object
 */
typedef struct {
    pycbc_ValResult_HEAD
    PyObject* vdict;
} pycbc_Item;

typedef struct {
    pycbc_OpResult_HEAD
    /* List of results. (value,errcode) */
    PyObject *results;

    /* Original list of specs passed. We can cache this later on if access
     * by element is required. */
    PyObject *specs;
} pycbc__SDResult;

enum {
    PYCBC_HTTP_HVIEW = 1,
    PYCBC_HTTP_HRAW,
    PYCBC_HTTP_HN1QL,
    PYCBC_HTTP_HFTS,
    PYCBC_HTTP_HNONE
};


enum {
    /** 'quiet' boolean set */
    PYCBC_MRES_F_QUIET      = 1 << 0,

    /** We're using a user-created Item; Don't create our own results */
    PYCBC_MRES_F_ITEMS      = 1 << 1,

    /** Items are already allocated and present within the dictionary. */
    PYCBC_MRES_F_UALLOCED   = 1 << 2,

    /** For GET (and possibly others), force FMT_BYTES */
    PYCBC_MRES_F_FORCEBYTES = 1 << 3,

    /** The commands have durability requirements */
    PYCBC_MRES_F_DURABILITY = 1 << 4,

    /** The command is an async subclass. Do we need this? */
    PYCBC_MRES_F_ASYNC = 1 << 5,

    /** This result is from a call to one of the single-item APIs */
    PYCBC_MRES_F_SINGLE = 1 << 6,

    /* Hint to dispatch to the view callback functions */
    PYCBC_MRES_F_VIEWS = 1 << 7
};
/**
 * Contextual info for enhanced error logging
 */
typedef struct{
    const char* FILE;
    const char* FUNC;
    int LINE;
} pycbc_debug_info;

int pycbc_debug_info_is_valid(pycbc_debug_info* info);

typedef PyObject pycbc_enhanced_err_info;

/**
 * Object containing the result of a 'Multi' operation. It's the same as a
 * normal dict, except we add an 'all_ok' field, so a user doesn't need to
 * skim through all the pairs to determine if something failed.
 *
 * See multiresult.c
 */
struct pycbc_MultiResult_st {
    PYCBC_MULTIRESULT_BASE;

    /** parent Connection object */
    pycbc_Bucket *parent;

    /**
     * A list of fatal exceptions, i.e. ones not resulting from a bad
     * LCB error code
     */
    PyObject *exceptions;

    /** A failed LCB operation, if any */
    PyObject *errop;

    pycbc_dur_params dur;

    /** Quick-check value to see if everything went well */
    int all_ok;

    /** Options for 'MultiResult' */
    int mropts;

    pycbc_enhanced_err_info *err_info;
};

typedef struct {
    pycbc_MultiResult base;

    /* How many operations do we have remaining */
    unsigned int nops;

    /* Object for the callback */
    PyObject *callback;

    /* Object to be invoked with errors */
    PyObject *errback;
} pycbc_AsyncResult;


/**
 * This structure is passed to our exception throwing function, it's
 * usually wrapped by one of the macros below
 */
struct pycbc_exception_params {
    /** C Source file at which the error was thrown (populated by macro */
    const char *file;

    /** C Source line, as above */
    int line;

    /** LCB Error code, if any */
    lcb_error_t err;

    /** Error message, if any */
    const char *msg;

    /** Key at which the error occurred. Not always present */
    PyObject *key;

    /** Single result which triggered the error, if present */
    PyObject *result;

    /**
     * A MultiResult object. This contains other operations which may
     * or may not have failed. This allows a user to check the status
     * of multi operations in which one of the keys resulted in an
     * exception
     */
    PyObject *all_results;

    /**
     * Extra info which caused the error. This is usually some kind of
     * bad parameter.
     */
    PyObject *objextra;

    /**
     * Enhanced error info if required.
     */
    pycbc_enhanced_err_info *err_info;
};

/**
 * Initializes a pycbc_exception_params to contain the proper
 * source context info
 */
#define PYCBC_EXC_STATIC_INIT { __FILE__, __LINE__ }

/**
 * Argument object, used for passing more information to the
 * multi functions. This isn't documented API yest.
 */
typedef struct {
    PyDictObject dict;
    int dummy; /* avoid sizing issues */
} pycbc_ArgumentObject;


/**
 * Object used as the 'value' for observe responses
 */
typedef struct {
    PyObject_HEAD
    unsigned int flags;
    int from_master;
    unsigned PY_LONG_LONG cas;
} pycbc_ObserveInfo;

/**
 * Flags to use for each type to indicate which subfields are relevant to
 * print out.
 */
enum {
    PYCBC_RESFLD_RC     = 1 << 0,
    PYCBC_RESFLD_CAS    = 1 << 1,
    PYCBC_RESFLD_KEY    = 1 << 2,
    PYCBC_RESFLD_FLAGS  = 1 << 3,
    PYCBC_RESFLD_HTCODE = 1 << 4,
    PYCBC_RESFLD_VALUE  = 1 << 5,
    PYCBC_RESFLD_URL    = 1 << 6
};

#define PYCBC_RESULT_BASEFLDS (PYCBC_RESFLD_RC)
#define PYCBC_OPRESULT_BASEFLDS \
    (PYCBC_RESULT_BASEFLDS| \
            PYCBC_RESFLD_CAS| \
            PYCBC_RESFLD_KEY)

#define PYCBC_VALRESULT_BASEFLDS (PYCBC_OPRESULT_BASEFLDS| \
        PYCBC_RESFLD_VALUE| \
        PYCBC_RESFLD_FLAGS)

#define PYCBC_HTRESULT_BASEFLDS \
    (       PYCBC_RESULT_BASEFLDS   | \
            PYCBC_RESFLD_HTCODE     | \
            PYCBC_RESFLD_URL        | \
            PYCBC_RESFLD_VALUE)

#define PYCBC_RESPROPS_NAME "_fldprops"
/**
 * Wrapper around PyType_Ready which also injects the common flags properties
 */
int pycbc_ResultType_ready(PyTypeObject *p, int flags);

/**
 * Types used for tracing
 */
#ifdef PYCBC_TRACING
#define PYCBC_TRACING_TYPES(X)\
    X(Tracer, "The Tracer Object")

#else
#define PYCBC_TRACING_TYPES(X)
#endif

#define PYCBC_CRYPTO_TYPES(X)                         \
    X(CryptoProvider,                                 \
      "A Cryptography Provider for Field Encryption", \
      pycbc_CryptoProvideType_extra_init(ptr))        \
    X(NamedCryptoProvider, "A Named Cryptography Provider for Field Encryption")

#define PYCBC_COLLECTION_TYPES(X)\
    X(Collection,                                 \
          "A Couchbase Collection", \
          pycbc_CryptoProvideType_extra_init(ptr))
#define PYCBC_AUTODEF_TYPES(X) \
    PYCBC_CRYPTO_TYPES(X)     \
    PYCBC_TRACING_TYPES(X)\
    PYCBC_COLLECTION_TYPES(X)

/**
 * Extern PyTypeObject declaraions.
 */

/* multiresult.c */
extern PyTypeObject pycbc_MultiResultType;
extern PyTypeObject pycbc_AsyncResultType;

/* result.c */
extern PyTypeObject pycbc_ResultType;

/* opresult.c */
extern PyTypeObject pycbc_OperationResultType;
extern PyTypeObject pycbc_ValueResultType;
extern PyTypeObject pycbc_HttpResultType;
extern PyTypeObject pycbc_ItemType;
extern PyTypeObject pycbc__SDResultType;

/* views.c */
extern PyTypeObject pycbc_ViewResultType;

/* ext.c */
#define PYCBC_EXTERN(X, DOC, ...) extern PyTypeObject pycbc_##X##Type;

PYCBC_AUTODEF_TYPES(PYCBC_EXTERN);
#undef PYCBC_EXTERN
/**
 * Result type check macros
 */
#define PYCBC_VALRES_CHECK(o) \
        PyObject_IsInstance(o, &pycbc_ValueResultType)

#define PYCBC_OPRES_CHECK(o) \
    PyObject_IsInstance(o, (PyObject*)&pycbc_OperationResultType)

extern PyTypeObject pycbc_ArgumentType;

/**
 * XXX: This isn't used.
 */
extern PyObject *pycbc_ExceptionType;

/**
 * X-macro to define the helpers we pass from _bootstrap.py along to
 * the module's '_init_helpers' function. We use an xmacro here because
 * the parameters may change and the argument handling is rather complex.
 * See below (in the pycbc_helpers structure) and in ext.c for more usages.
 */
#define PYCBC_XHELPERS(X) \
    X(result_reprfunc) \
    X(fmt_utf8_flags) \
    X(fmt_bytes_flags) \
    X(fmt_json_flags) \
    X(fmt_pickle_flags) \
    X(pickle_encode) \
    X(pickle_decode) \
    X(json_encode) \
    X(json_decode) \
    X(lcb_errno_map) \
    X(misc_errno_map) \
    X(default_exception) \
    X(obsinfo_reprfunc) \
    X(itmcoll_base_type) \
    X(itmopts_dict_type) \
    X(itmopts_seq_type) \
    X(fmt_auto) \
    X(view_path_helper) \
    X(sd_result_type) \
    X(sd_multival_type)

#define PYCBC_XHELPERS_STRS(X) \
    X(tcname_encode_key, PYCBC_TCNAME_ENCODE_KEY) \
    X(tcname_encode_value, PYCBC_TCNAME_ENCODE_VALUE) \
    X(tcname_decode_key, PYCBC_TCNAME_DECODE_KEY) \
    X(tcname_decode_value, PYCBC_TCNAME_DECODE_VALUE) \
    X(ioname_modevent, "update_event") \
    X(ioname_modtimer, "update_timer") \
    X(ioname_startwatch, "start_watching") \
    X(ioname_stopwatch, "stop_watching") \
    X(ioname_mkevent, "io_event_factory") \
    X(ioname_mktimer, "timer_event_factory") \
    X(vkey_id,        "id") \
    X(vkey_key,       "key") \
    X(vkey_value,     "value") \
    X(vkey_geo,       "geometry") \
    X(vkey_docresp,   "__DOCRESULT__")

/**
 * Definition of global helpers. This is only instantiated once as
 * pycbc_helpers.
 */
struct pycbc_helpers_ST {
    #define X(n) PyObject *n;
    PYCBC_XHELPERS(X)
    #undef X

    #define X(n, s) PyObject *n;
    PYCBC_XHELPERS_STRS(X)
    #undef X
};

/**
 * We use this one a lot. This is defined in ext.c
 */
extern struct pycbc_helpers_ST pycbc_helpers;

/**
 * Threading macros
 */
#define PYCBC_USE_THREADS

#ifdef PYCBC_USE_THREADS
#define PYCBC_CONN_THR_BEGIN(conn) \
    if ((conn)->unlock_gil) { \
        pycbc_assert((conn)->thrstate == NULL); \
        (conn)->thrstate = PyEval_SaveThread(); \
    }

#define PYCBC_CONN_THR_END(conn) \
    if ((conn)->unlock_gil) { \
        pycbc_assert((conn)->thrstate); \
        PyEval_RestoreThread((conn)->thrstate); \
        (conn)->thrstate = NULL; \
    }

#else
#define PYCBC_CONN_THR_BEGIN(X)
#define PYCBC_CONN_THR_END(X)
#endif

/*******************************
 * Type Initialization Functions
 *******************************
 *
 * These functions are called once from the extension's import method.
 * See ext.c
 *
 * They basically initialize the corresponding Python type so that
 * we can use them further on.
 */

/** Initializes the constants, constants. */
void pycbc_init_pyconstants(PyObject *module);
PyObject *pycbc_lcb_errstr(lcb_t instance, lcb_error_t err);
PyObject *pycbc_print_constants(PyObject *mod, PyObject *args);

int pycbc_ResultType_init(PyObject **ptr);
int pycbc_BucketType_init(PyObject **ptr);
int pycbc_MultiResultType_init(PyObject **ptr);
int pycbc_ValueResultType_init(PyObject **ptr);
int pycbc_OperationResultType_init(PyObject **ptr);
int pycbc_SDResultType_init(PyObject **ptr);
int pycbc_HttpResultType_init(PyObject **ptr);
int pycbc_TranscoderType_init(PyObject **ptr);
int pycbc_ObserveInfoType_init(PyObject **ptr);
int pycbc_ItemType_init(PyObject **ptr);
int pycbc_EventType_init(PyObject **ptr);
int pycbc_TimerEventType_init(PyObject **ptr);
int pycbc_IOEventType_init(PyObject **ptr);
int pycbc_AsyncResultType_init(PyObject **ptr);
int pycbc_IOPSWrapperType_init(PyObject **ptr);
int pycbc_ViewResultType_init(PyObject **ptr);

#define PYCBC_COLLECTIONS_PROPER
#ifdef PYCBC_COLLECTIONS_PROPER
int pycbc_CollectionType_init(PyObject** ptr);
#endif

#define PYCBC_TYPE_INIT_DECL(TYPENAME, TYPE_DOC, ...) \
    int pycbc_##TYPENAME##Type_init(PyObject **ptr);

PYCBC_AUTODEF_TYPES(PYCBC_TYPE_INIT_DECL)

#undef PYCBC_TYPE_INIT_DECL
/**
 * Calls the type's constructor with no arguments:
 */

#define PYCBC_TYPE_CTOR_1_args(t)               PyObject_CallFunction((PyObject*)t, 0)
#define PYCBC_TYPE_CTOR_2_args(t, args)         PyObject_CallFunction((PyObject*)t, "O", args)
#define PYCBC_TYPE_CTOR_3_args(t, args, kwargs) PyObject_CallFunction((PyObject*)t, "OO", args, kwargs)

#define GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define PYCBC_TYPE_CTOR_CHOOSER(...) \
    GET_4TH_ARG(__VA_ARGS__, PYCBC_TYPE_CTOR_3_args, \
                PYCBC_TYPE_CTOR_2_args, PYCBC_TYPE_CTOR_1_args, )

#define PYCBC_TYPE_CTOR(...) PYCBC_TYPE_CTOR_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/**
 * Allocators for result functions. See callbacks.c:get_common
 */
PyObject *pycbc_result_new(pycbc_Bucket *parent);
PyObject *pycbc_multiresult_new(pycbc_Bucket *parent);
pycbc_ValueResult *pycbc_valresult_new(pycbc_Bucket *parent);
pycbc_OperationResult *pycbc_opresult_new(pycbc_Bucket *parent);
pycbc_Item *pycbc_item_new(pycbc_Bucket *parent);
pycbc__SDResult *pycbc_sdresult_new(pycbc_Bucket *parent, PyObject *specs);

/* Add a result to a list of multi results. Specify the index */
void pycbc_sdresult_addresult(pycbc__SDResult *obj, size_t ii, PyObject *item);

/**
 * Simple function, here because it's defined in result.c but needed in
 * opresult.c
 */
void pycbc_Result_dealloc(pycbc_Result *self);

/**
 * Traps the current exception and adds it to the current MultiResult
 * context.
 * @param mres The MultiResult object.
 *
 * This calls pycbc_exc_mktuple(), so the constrains there apply to this
 * function as well.
 */
void pycbc_multiresult_adderr(pycbc_MultiResult* mres);

/**
 * Raise an exception from a multi result. This will raise an exception if:
 * 1) There is a 'fatal' error in the 'exceptions' list
 * 2) There is an 'operr'. 'operr' can be a failed LCB code (if no_raise_enoent
 * is on, this is not present if the failed code was LCB_KEY_ENOENT)
 */
int pycbc_multiresult_maybe_raise(pycbc_MultiResult *self);

/**
 * Return the effective user-facing value from this MultiResult object.
 * This should only be called if 'maybe_raise' returns false.
 * @param self the object
 * @return a new reference to the final result, or NULL on error.
 */
PyObject* pycbc_multiresult_get_result(pycbc_MultiResult *self);

/**
 * Invokes a callback when an operation has been completed. This will either
 * invoke the operation's "error callback" or the operation's "result callback"
 * depending on the state.
 */
void pycbc_asyncresult_invoke(pycbc_AsyncResult *mres,
                              pycbc_enhanced_err_info *err_info);

/**
 * Initialize the callbacks for the lcb_t
 */
void pycbc_callbacks_init(lcb_t instance);
void pycbc_http_callbacks_init(lcb_t instance);
void pycbc_views_callbacks_init(lcb_t instance);

/**
 * "Real" exception handler.
 * @param mode one of the PYCBC_EXC_* constants
 * @param p a struct of exception parameters
 */
void pycbc_exc_wrap_REAL(int mode, struct pycbc_exception_params *p);

/**
 * Get the appropriate Couchbase exception object.
 * @param mode one of the PYCBC_EXC_* constants
 * @param err the libcouchbase error, if any
 * @return a borrowed reference to the appropriate exception class
 */
PyObject* pycbc_exc_map(int mode, lcb_error_t err);

/**
 * Creates a simple exception with a given message. The exception
 * is not thrown.
 */
PyObject* pycbc_exc_message(int mode, lcb_error_t err, const char *msg);

/**
 * Gets the error classifier categories (as a set of bit flags) for a given
 * error code.
 */
PyObject* pycbc_exc_get_categories(PyObject *self, PyObject *arg);
/**
 * Throws an exception. If an exception is pending, it is caught and wrapped,
 * delivered into the CouchbaseError's 'inner_cause' field
 *
 * @param e_mode one of the PYCBC_EXC_* constants
 * @param e_err the LCB error code (use 0 if none)
 * @param e_msg a string message, if any
 * @param e_key the key during the handling of which the error occurred
 * @param e_objextra the problematic object which actually caused the errror
 */
#define PYCBC_EXC_WRAP_EX(e_mode, e_err, e_msg, e_key, e_objextra, e_err_info) \
    {                                                                          \
        PYCBC_DEBUG_LOG("Raising exception at %s, %d", __FILE__, __LINE__)\
        struct pycbc_exception_params __pycbc_ep = {0};                        \
        __pycbc_ep.file = __FILE__;                                            \
        __pycbc_ep.line = __LINE__;                                            \
        __pycbc_ep.err = (lcb_error_t)e_err;                                                \
        __pycbc_ep.msg = e_msg;                                                \
        __pycbc_ep.key = e_key;                                                \
        __pycbc_ep.objextra = e_objextra;                                      \
        __pycbc_ep.err_info = e_err_info;                                      \
        Py_XINCREF(e_err_info);                                                \
        pycbc_exc_wrap_REAL(e_mode, &__pycbc_ep);                              \
    }

#define PYCBC_EXC_WRAP(mode, err, msg) \
    PYCBC_EXC_WRAP_EX(mode, err, msg, NULL, NULL, NULL)

#define PYCBC_EXC_WRAP_OBJ(mode, err, msg, obj) \
    PYCBC_EXC_WRAP_EX(mode, err, msg, NULL, obj, NULL)

#define PYCBC_EXC_WRAP_KEY(mode, err, msg, key) \
    PYCBC_EXC_WRAP_EX(mode, err, msg, key, NULL, NULL)

#define PYCBC_EXC_WRAP_KEY_ERR_INFO(mode, err, msg, key, err_info) \
    PYCBC_EXC_WRAP_EX(mode, err, msg, key, NULL, err_info)

#define PYCBC_EXC_WRAP_VALUE PYCBC_EXC_WRAP_KEY

int pycbc_handle_assert(const char *msg, const char* file, int line);

/**
 * Creates a tuple of (class, object, traceback), similar to what would be
 * returned from sys.exc_info()
 * @return The error tuple.
 *
 * Calling this function will also clear the error
 * state. This must be called only if PyErr_Occurred() is true.
 */
PyObject *pycbc_exc_mktuple(void);

/**
 * This macro can be used as an 'if' structure. It returns false if the
 * condition fails and try otherwise
 */
#define pycbc_assert(e) ((e) ? 1 : pycbc_handle_assert(#e, __FILE__, __LINE__))

/**
 * EXCTHROW macros. These provide error messages for common stages.
 */
#define PYCBC_EXCTHROW_WAIT(err) PYCBC_EXC_WRAP(PYCBC_EXC_LCBERR, err, \
       "There was a problem while trying to send/receive " \
       "your request over the network. This may be a result of a " \
       "bad network or a misconfigured client or server")

#define PYCBC_EXCTHROW_SCHED(err) PYCBC_EXC_WRAP(PYCBC_EXC_LCBERR, err, \
        "There was a problem scheduling your request, or determining " \
        "the appropriate server or vBucket for the key(s) requested. "\
        "This may also be a bug in the SDK if there are no network issues")

#define PYCBC_EXCTHROW_ARGS() PYCBC_EXC_WRAP(PYCBC_EXC_ARGUMENTS, 0, \
                                           "Bad/insufficient arguments provided")

#define PYCBC_EXCTHROW_EMPTYKEY() PYCBC_EXC_WRAP(PYCBC_EXC_ARGUMENTS, 0, \
        "Empty key (i.e. '', empty string) passed")

typedef struct pycbc_pybuffer_real {
    PyObject *pyobj;
    const void *buffer;
    size_t length;
} pycbc_pybuffer;

#define PYCBC_PYBUF_RELEASE(buf) do { \
    Py_XDECREF((buf)->pyobj); \
    (buf)->pyobj = NULL; \
} while (0)

/**
 * Encodes a key into a buffer.
 * @param conn the connection object
 * @param key. in-out. Input should be the Python key. Output should be the
 * new python object which contains the underlying buffer for the key.
 * @param buf a pointer to a buffer pointer
 * @param nbuf pointer to the length of the buffer
 *
 * The buf parameter will likely be tied to the key parameter, so be sure not
 * to decrement its refcount until buf is no longer needed
 *
 * @return
 * 0 on success, nonzero on error
 */
int pycbc_tc_encode_key(pycbc_Bucket *conn, PyObject *src, pycbc_pybuffer *dst);

/**
 * Decodes a key buffer into a python object.
 * @param conn the connection object
 * @param key the key buffer
 * @param nkey the size of the key
 * @param pobj a pointer to a PyObject*, will be set with a newly-created python
 * object which represents the converted key
 *
 * @return
 * 0 on success, nonzero on error
 */
int pycbc_tc_decode_key(pycbc_Bucket *conn, const void *key, size_t nkey,
                        PyObject **pobj);

/**
 * Encode a value with flags
 * @param value. in-out. Input should be the Python value, Output should be the
 * new python object which contains the converted value.
 * @param flag_v. Python object representing 'flags'. This is used for efficiency
 * in order to pass a pythonized version of the flags
 * @param buf a pointer to a buffer, likely tied to 'buf'
 * @param nbuf pointer to buffer length
 * @param flags pointer to a flags variable, will be set with the appropriate
 * flags
 */
int pycbc_tc_encode_value(pycbc_Bucket *conn, PyObject *srcbuf, PyObject *flag_v,
                          pycbc_pybuffer *dstbuf, lcb_U32 *dstflags);

/**
 * Decode a value with flags
 * @param conn the connection object
 * @param value as received from the server
 * @param nvalue length of value
 * @param flags flags as received from the server
 * @param pobj the pythonized value
 */
int pycbc_tc_decode_value(pycbc_Bucket *conn, const void *value, size_t nvalue,
                          lcb_U32 flags, PyObject **pobj);

/**
 * Like encode_value, but only uses built-in encoders
 */
int pycbc_tc_simple_encode(PyObject *src, pycbc_pybuffer *dst, lcb_U32 flags);

/**
 * Like decode_value, but only uses built-in decoders
 */
int pycbc_tc_simple_decode(PyObject **vp, const char *buf, size_t nbuf,
                           lcb_U32 flags);

/**
 * Automatically determine the format for the object.
 */
PyObject *
pycbc_tc_determine_format(PyObject *value);

PyObject *
pycbc_iowrap_new(pycbc_Bucket *conn, PyObject *pyio);

lcb_io_opt_t
pycbc_iowrap_getiops(PyObject *iowrap);

/**
 * Event callback handling
 */
void pycbc_invoke_connected_event(pycbc_Bucket *conn, lcb_error_t err);

/**
 * Schedule the dtor event
 */
void pycbc_schedule_dtor_event(pycbc_Bucket *self);

/**
 * Pipeline handlers
 */
PyObject* pycbc_Bucket__start_pipeline(pycbc_Bucket *);
PyObject* pycbc_Bucket__end_pipeline(pycbc_Bucket *);

/**
 * Control methods
 */
PyObject* pycbc_Bucket__cntl(pycbc_Bucket *, PyObject *, PyObject *);
PyObject* pycbc_Bucket__vbmap(pycbc_Bucket *, PyObject *);
PyObject* pycbc_Bucket__cntlstr(pycbc_Bucket *conn, PyObject *args, PyObject *kw);

/**
 * Health-check methods
 */
PyObject *pycbc_Bucket__ping(pycbc_Bucket *self,
                             PyObject *args,
                             PyObject *kwargs);

PyObject *pycbc_Bucket__diagnostics(pycbc_Bucket *self,
                                    PyObject *args,
                                    PyObject *kwargs);

/**
 * Encryption Provider
 */
typedef struct {
    PyObject_HEAD lcbcrypto_PROVIDER *lcb_provider;
} pycbc_CryptoProvider;

typedef struct {
    PyObject_HEAD pycbc_CryptoProvider *orig_py_provider;
    lcbcrypto_PROVIDER *lcb_provider;
    PyObject *name;
} pycbc_NamedCryptoProvider;

#define PP_FOR_EACH_CRYPTO_EXCEPTION(X, ...)                               \
    X(PYCBC_CRYPTO_PROVIDER_NOT_FOUND, = LCB_MAX_ERROR),                   \
            X(PYCBC_CRYPTO_PROVIDER_ALIAS_NULL),                           \
            X(PYCBC_CRYPTO_PROVIDER_MISSING_PUBLIC_KEY),                   \
            X(PYCBC_CRYPTO_PROVIDER_MISSING_SIGNING_KEY),                  \
            X(PYCBC_CRYPTO_PROVIDER_MISSING_PRIVATE_KEY),                  \
            X(PYCBC_CRYPTO_PROVIDER_SIGNING_FAILED),                       \
            X(PYCBC_CRYPTO_PROVIDER_ENCRYPT_FAILED),                       \
            X(PYCBC_CRYPTO_PROVIDER_DECRYPT_FAILED),                       \
            X(PYCBC_CRYPTO_PROVIDER_KEY_SIZE_EXCEPTION),                   \
            X(PYCBC_CRYPTO_CONFIG_ERROR), X(PYCBC_CRYPTO_EXECUTION_ERROR), \
            X(PYCBC_CRYPTO_ERROR)

typedef enum {
#define PYCBC_CRYPTO_X(NAME, ...) NAME __VA_ARGS__
#define COMMA ,
    PP_FOR_EACH_CRYPTO_EXCEPTION(PYCBC_CRYPTO_X),
    PYCBC_CRYPTO_PROVIDER_ERROR_MAX
#undef COMMA
#undef PYCBC_CRYPTO_X
} pycbc_crypto_err;

PyObject *pycbc_gen_crypto_exception_map(void);

#ifndef PYCBC_CRYPTO_VERSION
#if PYCBC_LCB_API>0x030000
#ifdef PYCBC_BACKPORT_CRYPTO
#define PYCBC_CRYPTO_VERSION 1
#else
#define PYCBC_CRYPTO_VERSION 2
#endif
#else
#if LCB_VERSION > 0x020807
#define PYCBC_CRYPTO_VERSION 1
#else
#define PYCBC_CRYPTO_VERSION 0
#endif
#endif
#endif
/**
 * Flag to check if logging is enabled for the library via Python's logging
 */
extern PyObject* pycbc_log_handler;
extern struct lcb_logprocs_st pycbc_lcb_logprocs;

/**
 * Dummy tuple/keywords, used for PyArg_ParseTupleAndKeywordArgs, which dies
 * if one of the arguments is NULL, so these contain empty tuples and dicts,
 * respectively.
 */
extern PyObject *pycbc_DummyTuple;
extern PyObject *pycbc_DummyKeywords;


#if PY_MAJOR_VERSION >= 3
PyObject *PyInit__libcouchbase(void);
#define INITERROR return NULL

#else
#define INITERROR return
PyMODINIT_FUNC
init_libcouchbase(void);
#endif

#endif /* PYCBC_H_ */
