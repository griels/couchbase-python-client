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
#include "structmember.h"
/**
 * This file contains boilerplate for the module itself
 */

struct pycbc_helpers_ST pycbc_helpers;

PyObject *pycbc_log_handler = NULL;

static void log_handler(struct lcb_logprocs_st *procs, unsigned int iid,
    const char *subsys, int severity, const char *srcfile, int srcline,
    const char *fmt, va_list ap);

struct lcb_logprocs_st pycbc_lcb_logprocs = { 0 };

static PyObject *
_libcouchbase_init_helpers(PyObject *self, PyObject *args, PyObject *kwargs) {

#define X(n) \
    pycbc_helpers.n = PyDict_GetItemString(kwargs, #n); \
    if (!pycbc_helpers.n) { \
        PyErr_SetString(PyExc_EnvironmentError, "Can't find " #n); \
        return NULL; \
    } \

    PYCBC_XHELPERS(X);
#undef X

#define X(n) \
    Py_XINCREF(pycbc_helpers.n);
    PYCBC_XHELPERS(X)
#undef X

    (void)self;
    (void)args;

    Py_RETURN_NONE;
}

static void
get_helper_field(const char *s, PyObject *key, PyObject **cand, PyObject ***out)
{
    PyObject *sk = pycbc_SimpleStringZ(s);
    if (PyUnicode_Compare(sk, key) == 0) {
        *out = cand;
    }
    Py_DECREF(sk);
}

static PyObject *
_libcouchbase_modify_helpers(PyObject *self, PyObject *args, PyObject *kwargs)
{
    Py_ssize_t dictpos = 0;
    PyObject *curkey;
    PyObject *curval;
    PyObject *ret;

    if (kwargs == NULL || PyDict_Check(kwargs) == 0) {
        PYCBC_EXCTHROW_ARGS();
        return NULL;
    }

    ret = PyDict_New();

    while (PyDict_Next(kwargs, &dictpos, &curkey, &curval)) {
        PyObject **field = NULL;
        PyObject *dent = curval;

        #define X(name) \
        if (!field) { \
            get_helper_field(#name, \
                             curkey, \
                             &pycbc_helpers.name, \
                             &field); \
        }

        PYCBC_XHELPERS(X)
        #undef X

        if (!field) {
            PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0, "Unknown helper", curkey);
            Py_DECREF(ret);
            return NULL;
        }

        if (*field) {
            dent = *field;
        } else {
            dent = Py_None;
            Py_INCREF(dent);
        }

        PyDict_SetItem(ret, curkey, dent);
        Py_DECREF(dent);

        Py_INCREF(curval);
        *field = curval;
    }

    (void)args;
    (void)self;
    return ret;
}

static PyObject *
_libcouchbase_get_helper(PyObject *self, PyObject *arg)
{
    PyObject *key = NULL;
    PyObject **field = NULL;
    int rv;
    (void)self;

    rv = PyArg_ParseTuple(arg, "O", &key);
    if (!rv) {
        PYCBC_EXCTHROW_ARGS();
        return NULL;
    }

    #define X(name) \
    if (!field) { \
        get_helper_field(#name, key, &pycbc_helpers.name, &field); \
    }

    PYCBC_XHELPERS(X)
    #undef X

    if (!field) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0, "Unknown helper", key);
        return NULL;
    }
    if (*field) {
        Py_INCREF(*field);
        return *field;
    } else {
        Py_RETURN_NONE;
    }

}

static PyObject *
_libcouchbase_strerror(PyObject *self, PyObject *args, PyObject *kw)
{
    int rv;
    int rc = 0;
    rv = PyArg_ParseTuple(args, "i", &rc);
    if (!rv) {
        return NULL;
    }

    (void)self;
    (void)kw;

    return pycbc_lcb_errstr(NULL, rc);
}

static PyObject *
pycbc_lcb_version(pycbc_Bucket *self)
{
    const char *verstr;
    lcb_uint32_t vernum;
    PyObject *ret;

    verstr = lcb_get_version(&vernum);
    ret = Py_BuildValue("(s,k)", verstr, vernum);

    (void)self;

    return ret;
}

static PyObject *
set_log_handler(PyObject *self, PyObject *args)
{
    PyObject *val_O = NULL;
    PyObject *oldval = pycbc_log_handler;

    int rv;

    rv = PyArg_ParseTuple(args, "|O", &val_O);
    if (!rv) {
        return NULL;
    }

    if (val_O) {
        pycbc_log_handler = val_O;
        if (val_O != Py_None) {
            Py_INCREF(val_O);
            pycbc_log_handler = val_O;
        } else {
            pycbc_log_handler = NULL;
        }

        if (oldval) {
            return oldval;
        } else {
            Py_RETURN_NONE;
        }
    } else {
        /* Simple GET */
        if (oldval) {
            Py_INCREF(oldval);
            return oldval;
        } else {
            Py_RETURN_NONE;
        }
    }
}

static PyMethodDef _libcouchbase_methods[] = {
        { "_init_helpers", (PyCFunction)_libcouchbase_init_helpers,
                METH_VARARGS|METH_KEYWORDS,
                "internal function to initialize python-language helpers"
        },
        { "_strerror", (PyCFunction)_libcouchbase_strerror,
                METH_VARARGS|METH_KEYWORDS,
                "Internal function to map errors"
        },
        { "_modify_helpers", (PyCFunction)_libcouchbase_modify_helpers,
                METH_VARARGS|METH_KEYWORDS,
                "Internal function to modify helpers during runtime"
        },
        { "_get_helper", (PyCFunction)_libcouchbase_get_helper,
                METH_VARARGS,
                "Get a helper by name"
        },
        { "_get_errtype", (PyCFunction) pycbc_exc_get_categories,
                METH_VARARGS,
                "Get error categories for a given code"
        },
        { "lcb_version",
                (PyCFunction)pycbc_lcb_version,
                METH_NOARGS,
                PyDoc_STR(
                "Get `libcouchbase` version information\n"
                "\n"
                ":return: a tuple of ``(version_string, version_number)``\n"
                "  corresponding to the underlying libcouchbase version\n"

                "Show the versions ::\n" \
                "   \n"
                "   verstr, vernum = Connection.lcb_version()\n"
                "   print('0x{0:x}'.format(vernum))\n"
                "   # 0x020005\n"
                "   \n"
                "   print(verstr)\n"
                "   # 2.0.5\n"
                "\n"
                "\n")
        },
        { "lcb_logging", (PyCFunction)set_log_handler, METH_VARARGS,
                PyDoc_STR("Get/Set logging callback")
        },
        { "dump_constants",
                (PyCFunction)pycbc_print_constants, METH_NOARGS,
                PyDoc_STR("Print the constants to standard output")
        },
        { NULL }
};


#if PY_MAJOR_VERSION >= 3

#define PyString_FromString PyUnicode_FromString

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        PYCBC_MODULE_NAME,
        NULL,
        0,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
};
#endif

#if PY_MAJOR_VERSION >= 3
PyObject *PyInit__libcouchbase(void)
#define INITERROR return NULL

#else
#define INITERROR return
PyMODINIT_FUNC
init_libcouchbase(void)
#endif
{
    PyObject *m = NULL;

#ifndef PYCBC_CPYCHECKER

    /**
     * Each of our types has an initializer function that accepts a single
     * PyObject** pointer which is set to the newly created class object.
     *
     * More types should be added to this list. The first field is the type
     * name (not a string but a literal) and the second is the name of the
     * function used to initialize it
     */
#define X_PYTYPES(X) \
    X(Bucket,       pycbc_BucketType_init) \
    /** Remember to keep base classes in order */ \
    X(Result,           pycbc_ResultType_init) \
    X(OperationResult,  pycbc_OperationResultType_init) \
    X(ValueResult,      pycbc_ValueResultType_init) \
    X(MultiResult,      pycbc_MultiResultType_init) \
    X(HttpResult,       pycbc_HttpResultType_init) \
    X(ViewResult,       pycbc_ViewResultType_init) \
    X(Transcoder,       pycbc_TranscoderType_init) \
    X(ObserveInfo,      pycbc_ObserveInfoType_init) \
    X(Item,             pycbc_ItemType_init) \
    X(Event,            pycbc_EventType_init) \
    X(IOEvent,          pycbc_IOEventType_init) \
    X(TimerEvent,       pycbc_TimerEventType_init) \
    X(AsyncResult,      pycbc_AsyncResultType_init) \
    X(_IOPSWrapper,     pycbc_IOPSWrapperType_init) \
    X(_SDResult,        pycbc_SDResultType_init) \
    X(Tracer,           pycbc_TracerType_init)

#define X(name, inf) PyObject *cls_##name;
    X_PYTYPES(X)
#undef X

#define X(name, infunc) \
    if (infunc(&cls_##name) < 0) { INITERROR; }
    X_PYTYPES(X)
#undef X

#endif /* PYCBC_CPYCHECKER */

#if PY_MAJOR_VERSION >= 3
    moduledef.m_methods = _libcouchbase_methods;
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule(PYCBC_MODULE_NAME, _libcouchbase_methods);
#endif
    if (m == NULL) {
        INITERROR;
    }

#ifndef PYCBC_CPYCHECKER
    /**
     * Add the type:
     */
#define X(name, infunc) \
    PyModule_AddObject(m, #name, cls_##name);
    X_PYTYPES(X)
#undef X
#endif /* PYCBC_CPYCHECKER */

    /**
     * Initialize the helper names
     */
#define X(var, val) pycbc_helpers.var = pycbc_SimpleStringZ(val);
    PYCBC_XHELPERS_STRS(X);
#undef X

    pycbc_helpers.fmt_auto =
            PyObject_CallFunction((PyObject*)&PyBaseObject_Type, NULL, NULL);
    PyModule_AddObject(m, "FMT_AUTO", pycbc_helpers.fmt_auto);

    pycbc_init_pyconstants(m);

    /* Add various implementation specific flags */
    PyModule_AddIntConstant(m, "_IMPL_INCLUDE_DOCS", 0);

    /* Initialize the logging routines */
    pycbc_lcb_logprocs.v.v0.callback = log_handler;

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}

#ifndef va_copy
#define va_copy(a, b) (a) = (b)
#endif

/* Logging functionality */
static void log_handler(struct lcb_logprocs_st *procs,
                        unsigned int iid,
                        const char *subsys,
                        int severity,
                        const char *srcfile,
                        int srcline,
                        const char *fmt,
                        va_list ap)
{
    PyGILState_STATE gil_prev;
    PyObject *tmp;
    PyObject *kwargs;
    va_list vacp;

    if (!pycbc_log_handler) {
        return;
    }

    gil_prev = PyGILState_Ensure();

    kwargs = PyDict_New();
    va_copy(vacp, ap);
    tmp = PyUnicode_FromFormatV(fmt, vacp);
    va_end(ap);

    PyDict_SetItemString(kwargs, "message", tmp); Py_DECREF(tmp);

    tmp = pycbc_IntFromL(iid);
    PyDict_SetItemString(kwargs, "id", tmp); Py_DECREF(tmp);

    tmp = pycbc_IntFromL(severity);
    PyDict_SetItemString(kwargs, "level", tmp); Py_DECREF(tmp);

    tmp = Py_BuildValue("(s,i)", srcfile, srcline);
    PyDict_SetItemString(kwargs, "c_src", tmp); Py_DECREF(tmp);

    tmp = pycbc_SimpleStringZ(subsys);
    PyDict_SetItemString(kwargs, "subsys", tmp); Py_DECREF(tmp);

    PyObject_Call(pycbc_log_handler, pycbc_DummyTuple, kwargs);
    Py_DECREF(kwargs);
    PyGILState_Release(gil_prev);
}


#ifdef LCB_TRACING
#include <stdio.h>
#include <libcouchbase/couchbase.h>
#include <libcouchbase/api3.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* strlen */

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <libcouchbase/tracing.h>
//#include "include/opentracing/lcb_ot.h"
#include "../contrib/cJSON/cJSON.h"
#ifdef _WIN32
#define PRIx64 "I64x"
#define PRId64 "I64d"
#else
#include <inttypes.h>
#endif


#define COMPONENT_NAME "demo"
#ifdef LCB_TRACING
#include "oputil.h"



void pycbc_print_string( PyObject *curkey) {
#if PYTHON_ABI_VERSION >= 3
    {
        const char *keyname = PyUnicode_AsUTF8(curkey);
        printf("%s",  keyname);//(int)length, keyname);
    }
#else
    {
        printf("%s",PyString_AsString(curkey));
    };
#endif
}


void pycbc_print_repr( PyObject *pobj) {
    PyObject *curkey = PyObject_Repr(pobj);
    pycbc_print_string(curkey);
    Py_DecRef(curkey);
}

pycbc_stack_context_handle
pycbc_Tracer_span_start_real(pycbc_Tracer_t *py_tracer, const char *operation, uint64_t now, lcbtrace_REF *ref) {
    pycbc_stack_context_handle context = malloc(sizeof(pycbc_stack_context));
    context->tracer = py_tracer;
    context->span = lcbtrace_span_start(py_tracer->tracer, operation, now, ref);
    lcbtrace_span_add_tag_str(context->span, LCBTRACE_TAG_COMPONENT, COMPONENT_NAME);
    return context;
}

pycbc_stack_context_handle
pycbc_Tracer_span_start(pycbc_Tracer_t *py_tracer, PyObject *kwargs, const char *operation, uint64_t now,
                        pycbc_stack_context_handle context, lcbtrace_REF_TYPE ref_type) {

    PyObject *tracer = kwargs?PyDict_GetItemString(kwargs, "tracer"):NULL;
    if (!(py_tracer || (tracer && PyArg_ParseTuple(tracer, "O!", &pycbc_TracerType, &py_tracer) && py_tracer)))
    {
        abort();
    }

    if (context )
    {
        lcbtrace_REF ref;
        ref.type = ref_type;
        ref.span = context->span;
        return pycbc_Tracer_span_start_real(py_tracer, operation, now, &ref);
    }
    else
    {
        return pycbc_Tracer_span_start_real(py_tracer, operation, now, NULL);
    }
}
#else
pycbc_stack_context_handle
pycbc_Tracer_span_start(pycbc_Tracer_t *, PyObject *, const char *, uint64_t , pycbc_stack_context_handle , lcbtrace_REF_TYPE ) {
    return NULL;
}
#endif


int pycbc_is_async_or_pipeline(const pycbc_Bucket *self) { return self->flags & PYCBC_CONN_F_ASYNC || self->pipeline_queue; }

void pycbc_zipkin_destructor(lcbtrace_TRACER *tracer);

void pycbc_zipkin_report(lcbtrace_TRACER *tracer, lcbtrace_SPAN *span);

void pycbc_set_kv_ull(PyObject *span_args, char *key, uint64_t parenti_id);


struct zipkin_payload;

typedef struct zipkin_payload {
    char *data;
    PyObject* span_start_args;
    PyObject* span_tags_args;
    PyObject* span_finish_args;

    struct zipkin_payload *next;
} zipkin_payload;


typedef struct zipkin_state {
    PyObject_HEAD
    char *json_api_host;
    char *json_api_port;
    /* [0, 100], where 0 is "never", 100 is "always" */
    int sample_rate;
    zipkin_payload *root;
    zipkin_payload *last;
    size_t content_length;
    pycbc_Bucket* bucket;
    PyObject* parent;
    PyObject *start_span_method;
} zipkin_state;


void pycbc_zipkin_destructor(lcbtrace_TRACER *tracer)
{
    if (tracer) {
        if (tracer->cookie) {
            free(tracer->cookie);
            tracer->cookie = NULL;
        }
        free(tracer);
    }
}

#define LOGARGS(instance, lvl) instance->settings, "bootstrap", LCB_LOG_##lvl, __FILE__, __LINE__


void pycbc_init_traced_result(pycbc_Bucket *self, PyObject* mres_dict, PyObject *curkey,
                              pycbc_stack_context_handle context) {
    pycbc_pybuffer keybuf={0};
    pycbc_tc_encode_key(self, curkey, &keybuf);
    pycbc_tc_decode_key(self, keybuf.buffer, keybuf.length, &curkey);
    pycbc_ValueResult *item = pycbc_valresult_new(self);
    //Py_IncRef(item);
    item->tracing_context = context;
    item->is_tracing_stub = 1;
    printf("\nres %p: binding context %p to [", item, context);
    //pycbc_print_repr(curkey);
    printf("]\n");
    Py_IncRef(curkey);
    printf("\nPrior to insertion:[");
    pycbc_print_repr(mres_dict);
    printf("]\n");
    PyDict_SetItem(mres_dict, curkey, (PyObject*)item);
    printf("After insertion:[");
    pycbc_print_repr(mres_dict);
    printf("]\n");
    //pycbc_print_repr(mres_dict);
}

void pycbc_set_dict_kv_object(PyObject *dict, char *key_str, PyObject *item) {
    PyObject *key = pycbc_SimpleStringZ(key_str);
    PyDict_SetItem(dict, key, item);
    Py_DECREF(key);
}

void pycbc_init_span_args(zipkin_payload* payload)
{
    payload->span_start_args=PyDict_New();
    payload->span_tags_args=PyDict_New();
    payload->span_finish_args=PyDict_New();
    pycbc_set_dict_kv_object(payload->span_start_args, "tags", payload->span_tags_args);
}

#define CB_THR_END cb_thr_end
#define CB_THR_BEGIN cb_thr_begin
void pycbc_zipkin_report(lcbtrace_TRACER *tracer, lcbtrace_SPAN *span)
{
    zipkin_state *state = NULL;

    if (tracer == NULL) {
        return;
    }
    state = tracer->cookie;
    if (state == NULL) {
        return;
    }
    if (rand() % 100 > state->sample_rate) {
        return;
    }

    {
#define add_text(...) pycbc_dict_add_text_kv(__VA_ARGS__)
#define add_ull(...) pycbc_set_kv_ull(__VA_ARGS__)

#define dereference(...)
        //Py_DecRef(__VA_ARGS__)

#define BUFSZ 1000
        size_t nbuf = BUFSZ;
        char *buf;
        lcbtrace_SPAN *parent;
        uint64_t start;
        zipkin_payload *payload = calloc(1, sizeof(zipkin_payload));
        pycbc_init_span_args(payload);
        PyObject* span_args = payload->span_start_args;
        PyObject* tags_p = payload->span_tags_args;
        PyObject* finish_p = payload->span_finish_args;
        printf("got span %p\n",span);
        cJSON *json = cJSON_CreateObject();


        buf = calloc(nbuf, sizeof(char));
        cJSON_AddItemToObject(json, "name", cJSON_CreateString(lcbtrace_span_get_operation(span)));
        add_text(span_args,"operation_name",lcbtrace_span_get_operation(span));
        snprintf(buf, nbuf, "%" PRIx64, lcbtrace_span_get_span_id(span));
        cJSON_AddItemToObject(json, "id", cJSON_CreateString(buf));
        snprintf(buf, nbuf, "%" PRIx64, lcbtrace_span_get_trace_id(span));
        cJSON_AddItemToObject(json, "traceId", cJSON_CreateString(buf));
        parent = lcbtrace_span_get_parent(span);
        if (parent) {
            uint64_t parenti_id = lcbtrace_span_get_trace_id(parent);
            snprintf(buf, nbuf, "%" PRIx64, parenti_id);
            cJSON_AddItemToObject(json, "parentId", cJSON_CreateString(buf));
            add_ull(span_args, "child_of", parenti_id);
        }
        start = lcbtrace_span_get_start_ts(span);
        cJSON_AddItemToObject(json, "timestamp", cJSON_CreateNumber(start));

        cJSON_AddItemToObject(json, "duration", cJSON_CreateNumber(lcbtrace_span_get_finish_ts(span) - start));
        add_ull(finish_p, "finish", lcbtrace_span_get_finish_ts(span));
        add_ull(span_args, "start_time", start);
        {
            cJSON *endpoint = cJSON_CreateObject();

            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_DB_TYPE, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(endpoint, "serviceName", cJSON_CreateString(buf));
                add_text(tags_p, LCBTRACE_TAG_DB_TYPE, buf);
            }
            cJSON_AddItemToObject(json, "localEndpoint", endpoint);
            //add_text(tags_p, "localEndpoint", endpoint);
        }

        {
            cJSON *tags = cJSON_CreateObject();
            uint64_t latency, operation_id;
            if (lcbtrace_span_get_tag_uint64(span, LCBTRACE_TAG_PEER_LATENCY, &latency) == LCB_SUCCESS) {
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_PEER_LATENCY, cJSON_CreateNumber(latency));
                add_ull(tags_p, LCBTRACE_TAG_PEER_LATENCY, latency);
            }
            if (lcbtrace_span_get_tag_uint64(span, LCBTRACE_TAG_OPERATION_ID, &operation_id) == LCB_SUCCESS) {
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_OPERATION_ID, cJSON_CreateNumber(operation_id));
                add_ull(tags_p, LCBTRACE_TAG_OPERATION_ID, operation_id);
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_COMPONENT, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_COMPONENT, cJSON_CreateString(buf));
                add_text(tags_p, LCBTRACE_TAG_COMPONENT, buf);
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_PEER_ADDRESS, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_PEER_ADDRESS, cJSON_CreateString(buf));
                add_text(tags_p, LCBTRACE_TAG_PEER_ADDRESS, buf);
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_LOCAL_ADDRESS, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_LOCAL_ADDRESS, cJSON_CreateString(buf));
                add_text(tags_p, LCBTRACE_TAG_LOCAL_ADDRESS, buf);
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_DB_INSTANCE, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_DB_INSTANCE, cJSON_CreateString(buf));
                add_text(tags_p, LCBTRACE_TAG_DB_INSTANCE, buf);
            }
            if (cJSON_GetArraySize(tags) > 0)  {
                cJSON_AddItemToObject(json, "tags", tags);
            } else {
                cJSON_Delete(tags);
            }
        }
        dereference(tags_p);
        dereference(span_args);
        free(buf);


        payload->data = cJSON_PrintUnformatted(json);
        cJSON_Delete(json);
        if (state->last) {
            state->last->next = payload;
        }
        state->last = payload;
        //printf(payload);
        state->content_length += strlen(payload->data) + 1; /* for comma/closing bracket */
        if (state->root == NULL) {
            state->root = payload;
        }
    }

}


void pycbc_Tracer_propagate_span(pycbc_Tracer_t *tracer, struct zipkin_payload *payload) {
    zipkin_state *state = (zipkin_state *) tracer->tracer->cookie;
    pycbc_assert(state->parent);
    printf("\nabout to call: %p[\n", state->start_span_method);
    printf("] on %p=[", state->parent);
    pycbc_print_repr(state->parent);
    printf("] with args %p=[", payload->span_start_args);
    pycbc_print_repr(payload->span_start_args);
    printf("]\n");
    if (state->start_span_method && PyObject_IsTrue(state->start_span_method)) {
        PyObject *fresh_span = PyObject_CallFunction(state->start_span_method, "O",
                                                     payload->span_start_args);
        pycbc_assert(fresh_span);
        PyObject *finish_method = PyObject_GetAttr(PyObject_Type(fresh_span), pycbc_SimpleStringZ("finish"));
        pycbc_assert(PyObject_CallFunction(finish_method, "OO", fresh_span, payload->span_finish_args));
        Py_DECREF(finish_method);
        dereference(span_finish_args);
        dereference(fresh_span);
    }
}
void pycbc_zipkin_flush(pycbc_Tracer_t *tracer)
{
    zipkin_state *state = NULL;

    if (tracer == NULL) {
        return;
    }
    state = tracer->tracer->cookie;
    if (state == NULL) {
        return;
    }
    if (state->root == NULL || state->content_length == 0) {
        return;
    }
    {
        zipkin_payload *ptr = state->root;
        printf("flushing\n");
        while (ptr) {
            zipkin_payload *tmp = ptr;
            //printf("%s",tmp->data);
            if (tracer->parent) {
                pycbc_Tracer_propagate_span(tracer, tmp);
            }
            ptr = ptr->next;
            Py_DECREF(tmp->span_finish_args);
            Py_DECREF(tmp->span_tags_args);
            Py_DECREF(tmp->span_start_args);
            free(tmp->data);
            free(tmp);
        }
    }
    state->root = state->last = NULL;
    state->content_length = 0;
}

void pycbc_Tracer_propagate(  pycbc_Tracer_t *tracer) {
    pycbc_zipkin_flush(tracer);
}

void pycbc_set_kv_ull(PyObject *span_args, char *key, uint64_t parenti_id) {
    PyObject *pULL = PyLong_FromUnsignedLongLong(parenti_id);
    PyObject* keystr = pycbc_SimpleStringZ(key);
    PyDict_SetItem(span_args, keystr, pULL);
    Py_DECREF(pULL);
    Py_DECREF(keystr);
}


lcbtrace_TRACER *pycbc_zipkin_new(PyObject* parent)
{
    lcbtrace_TRACER *tracer = calloc(1, sizeof(lcbtrace_TRACER));
    zipkin_state *zipkin = calloc(1, sizeof(zipkin_state));
    tracer->destructor = pycbc_zipkin_destructor;
    tracer->flags = 0;
    tracer->version = 0;
    tracer->v.v0.report = pycbc_zipkin_report;
    zipkin->json_api_host = "localhost";
    zipkin->json_api_port = "9411";
    zipkin->sample_rate = 100;
    zipkin->root = NULL;
    zipkin->last = NULL;
    zipkin->content_length = 0;
    tracer->cookie = zipkin;
    zipkin->parent = parent;
    if (parent) {
        printf("\ninitialising tracer start_span method from:[");
        pycbc_print_repr(parent);
        printf("]\n");
        zipkin->start_span_method = PyObject_GetAttrString(parent, "start_span");
        if (zipkin->start_span_method)
        {
            printf("got start_span method:[");
            pycbc_print_repr(zipkin->start_span_method);
            printf("]\n");
        }
    }
    return tracer;
}

static PyGetSetDef pycbc_Tracer_TABLE_getset[] = {
        /*  { "default_format",
                  (getter)Tracer_get_format,
                  (setter)Tracer_set_format,
                  PyDoc_STR("The default format to use for encoding values "
                                    "(passed to transcoder)")
          },

  */
        { NULL }
};

static struct PyMemberDef pycbc_Tracer_TABLE_members[] = {
        /*  { "quiet", T_UINT, offsetof(pycbc_Tracer, quiet),
                  0,
                  PyDoc_STR("Whether to suppress errors when keys are not found "
                                    "(in :meth:`get` and :meth:`delete` operations).\n"
                                    "\n"
                                    "An error is still returned within the :class:`Result`\n"
                                    "object")
          },
  */

        { NULL }
};

static PyMethodDef pycbc_Tracer_TABLE_methods[] = {

#define OPFUNC(name, doc) \
{ #name, (PyCFunction)pycbc_Tracer_##name, METH_VARARGS|METH_KEYWORDS, \
    PyDoc_STR(doc) }

          /** Basic Operations */
       // OPFUNC(upsert, "Unconditionally store a key in Couchbase"),

#undef OPFUNC
/*


        { "_thr_lockop",
          (PyCFunction)Tracer__thr_lockop,
          METH_VARARGS,
          PyDoc_STR("Unconditionally lock/unlock the connection object "
                            "if 'lockmode' has been set. For testing uses only")
        },

*/

        { NULL, NULL, 0, NULL }
};


static int
Tracer__init__(pycbc_Tracer_t *self,
               PyObject *args, PyObject* kwargs)
{
    int rv = 0;
    PyObject* tracer =PyTuple_GetItem(args, 0);
    pycbc_Bucket* bucket= (pycbc_Bucket*) PyTuple_GetItem(args,1);
    //pycbc_print_repr(args);
    printf("I'm in ur tracer init with a bucket %p and tracer %p\n", bucket, tracer);
    self->tracer=pycbc_zipkin_new(tracer);
    self->parent=tracer!=Py_None?tracer:NULL;
    return rv;
}

static void
Tracer_dtor(pycbc_Tracer_t *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

#define PYCBC_TYPE_INIT(TYPENAME,TYPE_DOC)\
PyTypeObject pycbc_##TYPENAME##Type = {\
        PYCBC_POBJ_HEAD_INIT(NULL)\
        0\
};\
\
int \
pycbc_##TYPENAME##Type_init(PyObject **ptr)\
{\
    PyTypeObject *p = &pycbc_##TYPENAME##Type;\
    *ptr = (PyObject*)p;\
\
    if (p->tp_name) {\
        return 0;\
    }\
\
    p->tp_name = #TYPENAME;\
    p->tp_new = PyType_GenericNew;\
    p->tp_init = (initproc)TYPENAME##__init__;\
    p->tp_dealloc = (destructor)TYPENAME##_dtor;\
\
    p->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;\
    p->tp_doc = PyDoc_STR(TYPE_DOC);\
\
    p->tp_basicsize = sizeof(pycbc_##TYPENAME##_t);\
\
    p->tp_methods = pycbc_##TYPENAME##_TABLE_methods;\
    p->tp_members = pycbc_##TYPENAME##_TABLE_members;\
    p->tp_getset = pycbc_##TYPENAME##_TABLE_getset;\
    \
    pycbc_DummyTuple = PyTuple_New(0);\
    pycbc_DummyKeywords = PyDict_New();\
\
    return PyType_Ready(p);\
}
PYCBC_TRACING_TYPES(PYCBC_TYPE_INIT);
#endif