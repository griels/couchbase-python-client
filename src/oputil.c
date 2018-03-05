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

#include "oputil.h"
#include "pycbc.h"
#include "structmember.h"

void
pycbc_common_vars_finalize(struct pycbc_common_vars *cv, pycbc_Bucket *conn)
{
    if (cv->mctx) {
        cv->mctx->fail(cv->mctx);
        cv->mctx = NULL;
    }
    lcb_sched_fail(conn->instance);
    Py_XDECREF(cv->mres);

    if (conn->lockmode) {
        pycbc_oputil_conn_unlock(conn);
    }
}

int
pycbc_common_vars_wait(struct pycbc_common_vars *cv, pycbc_Bucket *self)
{
    Py_ssize_t nsched = cv->is_seqcmd ? 1 : cv->ncmds;

    if (cv->mctx) {
        cv->mctx->done(cv->mctx, cv->mres);
        cv->mctx = NULL;
    }
    lcb_sched_leave(self->instance);
    self->nremaining += nsched;

    if (self->flags & PYCBC_CONN_F_ASYNC) {
        /** For async, just do the right thing :) */
        cv->ret = (PyObject *)cv->mres;
        ((pycbc_AsyncResult *)cv->mres)->nops = nsched;

        /** INCREF once more so it's alive in the event loop */
        Py_INCREF(cv->ret);
        cv->mres = NULL;
        return 0;

    } else if (self->pipeline_queue) {
        cv->ret = Py_None;
        Py_INCREF(Py_None);
        return 0;
    }
    pycbc_oputil_wait_common(self);

    if (!pycbc_assert(self->nremaining == 0)) {
        fprintf(stderr, "Remaining count != 0. Adjusting");
        self->nremaining = 0;
    }

    if (pycbc_multiresult_maybe_raise(cv->mres)) {
        return -1;
    }

    cv->ret = pycbc_multiresult_get_result(cv->mres);
    Py_DECREF(cv->mres);
    cv->mres = NULL;

    if (cv->ret == NULL) {
        return -1;
    }

    return 0;
}

int
pycbc_common_vars_init(struct pycbc_common_vars *cv,
                       pycbc_Bucket *self,
                       int argopts,
                       Py_ssize_t ncmds,
                       int want_vals)
{
    if (-1 == pycbc_oputil_conn_lock(self)) {
        return -1;
    }

    cv->ncmds = ncmds;
    cv->mres = (pycbc_MultiResult*)pycbc_multiresult_new(self);
    cv->argopts = argopts;

    if (argopts & PYCBC_ARGOPT_SINGLE) {
        cv->mres->mropts |= PYCBC_MRES_F_SINGLE;
    }

    if (!cv->mres) {
        pycbc_oputil_conn_unlock(self);
        return -1;
    }

    lcb_sched_enter(self->instance);
    return 0;
}

/**
 * Check that the object is not one of Python's typical string types
 */
#define _is_not_strtype(o) \
    (PyBytes_Check(o) == 0 && PyByteArray_Check(o) == 0 && PyUnicode_Check(o) == 0)

int
pycbc_oputil_check_sequence(PyObject *sequence,
                            int allow_list,
                            Py_ssize_t *ncmds,
                            pycbc_seqtype_t *seqtype)
{
    int ret = 0;
    pycbc_seqtype_t dummy;
    if (!seqtype) {
        seqtype = &dummy;
    }

    *ncmds = 0;

    if (PyDict_Check(sequence)) {
        *ncmds = PyDict_Size(sequence);
        *seqtype = PYCBC_SEQTYPE_DICT;
        ret = 0;

    } else if (allow_list == 0 &&
            PyObject_IsInstance(sequence, pycbc_helpers.itmcoll_base_type) == 0) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                           "Keys must be a dictionary",
                           sequence);
        ret = -1;

    } else if (PyList_Check(sequence)) {
        *seqtype = PYCBC_SEQTYPE_LIST;
        *ncmds = PyList_GET_SIZE(sequence);

    } else if (PyTuple_Check(sequence)) {
        *seqtype = PYCBC_SEQTYPE_TUPLE;
        *ncmds = PyTuple_GET_SIZE(sequence);

    } else if (PyObject_IsInstance(sequence, pycbc_helpers.itmcoll_base_type)) {
        *ncmds = PyObject_Length(sequence);
        if (*ncmds == -1) {
            PYCBC_EXC_WRAP(PYCBC_EXC_INTERNAL, 0,
                           "ItemCollection subclass did not return proper length");
            ret = -1;
        }
        *seqtype = PYCBC_SEQTYPE_GENERIC | PYCBC_SEQTYPE_F_ITM;
        if (PyObject_IsInstance(sequence, pycbc_helpers.itmopts_dict_type)) {
            *seqtype |= PYCBC_SEQTYPE_F_OPTS;
        }


    } else if (_is_not_strtype(sequence)) {
        /**
         * Previously we used PySequence_Check, but this failed on things
         * which didn't have __getitem__ (they had a length, but the elements
         * were not ordered, but we don't care about that here
         */
        *seqtype = PYCBC_SEQTYPE_GENERIC;
        *ncmds = PyObject_Length(sequence);

        if (*ncmds == -1) {
            PyErr_Clear();
            PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                               "Keys must be iterable and have known length",
                               sequence);
            ret = -1;
        }

    } else {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                           "Keys must be iterable and have known length",
                           sequence);
        ret = -1;
    }

    if (ret == 0 && *ncmds < 1) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0, "Key list is empty", sequence);
        ret = -1;
    }

    return ret;
}

int
pycbc_maybe_set_quiet(pycbc_MultiResult *mres, PyObject *quiet)
{
    /**
     * If quiet is 'None', then we default to Connection.quiet
     */
    int tmp = 0;
    if (quiet == NULL || quiet == Py_None) {
        mres->mropts |= (mres->parent->quiet) ? PYCBC_MRES_F_QUIET : 0;
        return 0;
    }

    tmp |= PyObject_IsTrue(quiet);

    if (tmp == -1) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS,
                           0, "quiet must be True, False, or None'", quiet);
        return -1;
    }

    mres->mropts |= tmp ? PYCBC_MRES_F_QUIET : 0;

    return 0;
}

PyObject *
pycbc_oputil_iter_prepare(pycbc_seqtype_t seqtype,
                          PyObject *sequence,
                          PyObject **iter,
                          Py_ssize_t *dictpos)
{
    if (seqtype & PYCBC_SEQTYPE_GENERIC) {
        *iter = PyObject_GetIter(sequence);
        if (!*iter) {
            PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                               "Couldn't get iterator from object. Object "
                               "should implement __iter__",
                               sequence);
        }
        return *iter;
    } else if (seqtype & PYCBC_SEQTYPE_DICT) {
        *dictpos = 0;
    }
    *iter = NULL;
    return sequence;
}

/**
 * I thought it better to make the function call a bit more complex, so as to
 * have the iteration logic unified in a single place
 */
int
pycbc_oputil_sequence_next(pycbc_seqtype_t seqtype,
                           PyObject *seqobj,
                           Py_ssize_t *dictpos,
                           int ii,
                           PyObject **key,
                           PyObject **value,
                           pycbc_stack_context_handle context )
{
    if (seqtype & PYCBC_SEQTYPE_DICT) {
        int rv = PyDict_Next(seqobj, dictpos, key, value);
        if (rv < 1) {
            PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_INTERNAL,
                               0, "Couldn't iterate", seqobj);
            return -1;
        }

        Py_XINCREF(*key);
        Py_XINCREF(*value);
        return 0;
    }

    *value = NULL;
    if (seqtype & PYCBC_SEQTYPE_LIST) {
        *key = PyList_GET_ITEM(seqobj, ii);
        Py_INCREF(*key);
    } else if (seqtype & PYCBC_SEQTYPE_TUPLE) {
        *key = PyTuple_GET_ITEM(seqobj, ii);
        Py_INCREF(*key);
    } else {
        *key = PyIter_Next(seqobj);
        if (!*key) {
            PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                               "PyIter_Next returned NULL", seqobj);
            return -1;
        }
    }

    return 0;
}

static int
extract_item_params(struct pycbc_common_vars *cv,
                    PyObject *k,
                    pycbc_Item **itm,
                    PyObject **options)
{
    /** Key will always be an item */
    Py_ssize_t tsz;

    if (!PyTuple_Check(k)) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0, "Expected Tuple", k);
        return -1;
    }

    tsz = PyTuple_GET_SIZE(k);
    if (tsz != 2 && tsz != 1) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                           "Tuple from __iter__ must return 1 or 2 items", k);
        return -1;
    }

    *itm = (pycbc_Item *) PyTuple_GET_ITEM(k, 0);
    if (!PyObject_IsInstance((PyObject*)*itm, (PyObject *)&pycbc_ItemType)) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                           "Expected 'Item' instance", (PyObject*)*itm);
        return -1;
    }

    if (tsz == 2) {
        *options = PyTuple_GET_ITEM(k, 1);

        if (*options == Py_None) {
            *options = NULL;

        } else if (!PyDict_Check(*options)) {
            PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS,
                               0, "Options must be None or dict", *options);
            return -1;
        }
    }

    if (! (*itm)->key) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                           "Item is missing key", (PyObject *)*itm);
        return -1;
    }

    /** Store the item inside the mres dictionary */
    PyDict_SetItem(pycbc_multiresult_dict(cv->mres),
                   (*itm)->key, (PyObject *)*itm);
    cv->mres->mropts |= PYCBC_MRES_F_UALLOCED;
    return 0;
}

int
pycbc_oputil_iter_multi(pycbc_Bucket *self,
                        pycbc_seqtype_t seqtype,
                        PyObject *collection,
                        struct pycbc_common_vars *cv,
                        int optype,
                        pycbc_oputil_keyhandler handler,
                        void *arg,
                        pycbc_stack_context_handle context)
{
    int rv = 0;
    int ii;
    PyObject *iterobj;
    PyObject *seqobj;
    Py_ssize_t dictpos = 0;

    seqobj = pycbc_oputil_iter_prepare(seqtype, collection, &iterobj, &dictpos);
    if (seqobj == NULL) {
        return -1;
    }

    for (ii = 0; ii < cv->ncmds; ii++) {
        PyObject *k, *v = NULL, *options = NULL;
        PyObject *arg_k = NULL;
        pycbc_Item *itm = NULL;

        rv = pycbc_oputil_sequence_next(seqtype,
                                        seqobj, &dictpos, ii, &k, &v, context);
        if (rv < 0) {
            goto GT_ITER_DONE;
        }

        if (seqtype & PYCBC_SEQTYPE_F_ITM) {
            if ((rv = extract_item_params(cv, k, &itm, &options)) != 0) {
                goto GT_ITER_DONE;
            }
            arg_k = itm->key;

        } else {
            arg_k = k;
        }

        rv = handler(self, cv, optype, arg_k, v, options, itm, arg, context);

        GT_ITER_DONE:
        Py_XDECREF(k);
        Py_XDECREF(v);

        if (rv == -1) {
            break;
        }
    }

    Py_XDECREF(iterobj);
    return rv;
}

int
pycbc_oputil_conn_lock(pycbc_Bucket *self)
{
    int status;
    int mode;

    if (!self->lockmode) {
        return 0;
    }

    mode = self->lockmode == PYCBC_LOCKMODE_WAIT ? WAIT_LOCK : NOWAIT_LOCK;
    if (mode == WAIT_LOCK) {
        /**
         * We need to unlock the GIL here so that other objects can potentially
         * access the Connection (and thus unlock it).
         */
        Py_BEGIN_ALLOW_THREADS
        status = PyThread_acquire_lock(self->lock, mode);
        Py_END_ALLOW_THREADS
    } else {
        status = PyThread_acquire_lock(self->lock, mode);
    }

    if (!status) {
        PYCBC_EXC_WRAP(PYCBC_EXC_THREADING,
                       0,
                       "Couldn't lock. If LOCKMODE_WAIT was passed, "
                       "then this means that something has gone wrong "
                       "internally. Otherwise, this means you are using "
                       "the Connection object from multiple threads. This "
                       "is not allowed (without an explicit "
                       "lockmode=LOCKMODE_WAIT constructor argument");
        return -1;
    }
    return 0;
}

void
pycbc_oputil_conn_unlock(pycbc_Bucket *self)
{
    if (!self->lockmode) {
        return;
    }
    PyThread_release_lock(self->lock);
}

void
pycbc_oputil_wait_common(pycbc_Bucket *self)
{
    /**
     * If we have a 'lockmode' specified, check to see that nothing else is
     * using us. We lock in any event.
     *
     * We have two modes:
     *  - LOCKMODE_WAIT explicitly allows access from multiple threads.
     *      In this mode, we actually wait to acquire the lock.
     *
     *  - LOCKMODE_EXC  will raise an exception if it cannot lock immediately
     *
     * Note that LOCKMODE_EXC won't do strict checking - i.e. it's perfectly
     * possible
     */

    PYCBC_CONN_THR_BEGIN(self);
    lcb_wait3(self->instance, LCB_WAIT_NOCHECK);
    PYCBC_CONN_THR_END(self);
}

/**
 * Returns 1 if durability was found, 0 if durability was not found, and -1
 * on error.
 */
int
pycbc_handle_durability_args(pycbc_Bucket *self,
                             pycbc_dur_params *params,
                             char persist_to,
                             char replicate_to)
{
    if (self->dur_global.persist_to || self->dur_global.replicate_to) {
        if (persist_to == 0 && replicate_to == 0) {
            persist_to = self->dur_global.persist_to;
            replicate_to = self->dur_global.replicate_to;
        }
    }

    if (persist_to || replicate_to) {
        int nreplicas = lcb_get_num_replicas(self->instance);
        params->persist_to = persist_to;
        params->replicate_to = replicate_to;
        if (replicate_to > nreplicas || persist_to > (nreplicas + 1)) {
            PYCBC_EXC_WRAP(PYCBC_EXC_LCBERR, LCB_DURABILITY_ETOOMANY,
                           "Durability requirements will never be satisfied");
            return -1;
        }

        return 1;
    }

    return 0;
}

int
pycbc_encode_sd_keypath(pycbc_Bucket *conn, PyObject *src,
                      pycbc_pybuffer *keybuf, pycbc_pybuffer *pathbuf)
{
    PyObject *kobj, *pthobj;
    int rv;

    if (!PyTuple_Check(src) || PyTuple_GET_SIZE(src) != 2) {
        PYCBC_EXC_WRAP(PYCBC_EXC_ARGUMENTS, 0,
                       "Sub-document key must be a 2-tuple");
        return -1;
    }

    kobj = PyTuple_GET_ITEM(src, 0);
    pthobj = PyTuple_GET_ITEM(src, 1);

    rv = pycbc_tc_encode_key(conn, kobj, keybuf);
    if (rv != 0) {
        return rv;
    }
    rv = pycbc_tc_simple_encode(pthobj, pathbuf, PYCBC_FMT_UTF8);
    if (rv != 0) {
        PYCBC_PYBUF_RELEASE(keybuf);
    }
    return rv;
}

static int
sd_convert_spec(PyObject *pyspec, lcb_SDSPEC *sdspec,
    pycbc_pybuffer *pathbuf, pycbc_pybuffer *valbuf)
{
    PyObject *path = NULL;
    PyObject *val = NULL;
    int op = 0;
    unsigned flags = 0;

    if (!PyTuple_Check(pyspec)) {
        PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0, "Expected tuple for spec", pyspec);
        return -1;
    }

    if (!PyArg_ParseTuple(pyspec, "iOI|O", &op, &path, &flags, &val)) {
        PYCBC_EXCTHROW_ARGS();
        return -1;
    }
    if (pycbc_tc_simple_encode(path, pathbuf, PYCBC_FMT_UTF8) != 0) {
        goto GT_ERROR;
    }

    sdspec->sdcmd = op;
    sdspec->options = flags;
    LCB_SDSPEC_SET_PATH(sdspec, pathbuf->buffer, pathbuf->length);
    if (val != NULL) {
        int is_multival = 0;

        if (PyObject_IsInstance(val, pycbc_helpers.sd_multival_type)) {
            /* Verify the operation allows it */
            switch (op) {
            case LCB_SDCMD_ARRAY_ADD_FIRST:
            case LCB_SDCMD_ARRAY_ADD_LAST:
            case LCB_SDCMD_ARRAY_INSERT:
                is_multival = 1;
                break;
            default:
                PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ARGUMENTS, 0,
                    "MultiValue not supported for operation", pyspec);
                goto GT_ERROR;
            }
        }

        if (pycbc_tc_simple_encode(val, valbuf, PYCBC_FMT_JSON) != 0) {
            goto GT_ERROR;
        }

        if (is_multival) {
            /* Strip first and last [ */
            const char *buf = (const char *)valbuf->buffer;
            size_t len = valbuf->length;

            for (; isspace(*buf) && len; len--, buf++) {
            }
            for (; len && isspace(buf[len-1]); len--) {
            }
            if (len < 3 || buf[0] != '[' || buf[len-1] != ']') {
                PYCBC_EXC_WRAP_OBJ(PYCBC_EXC_ENCODING, 0,
                    "Serialized MultiValue shows invalid JSON (maybe empty?)",
                    pyspec);
                goto GT_ERROR;
            }

            buf++;
            len -= 2;
            valbuf->buffer = buf;
            valbuf->length = len;
        }

        LCB_SDSPEC_SET_VALUE(sdspec, valbuf->buffer, valbuf->length);
    }
    return 0;

    GT_ERROR:
    PYCBC_PYBUF_RELEASE(valbuf);
    PYCBC_PYBUF_RELEASE(pathbuf);
    return -1;
}

int
pycbc_sd_handle_speclist(pycbc_Bucket *self, pycbc_MultiResult *mres,
    PyObject *key, PyObject *spectuple, lcb_CMDSUBDOC *cmd)
{
    int rv = 0;
    lcb_error_t err = LCB_SUCCESS;
    Py_ssize_t nspecs = 0;
    pycbc__SDResult *newitm = NULL;
    lcb_SDSPEC *specs = NULL, spec_s = { 0 };
    pycbc_pybuffer pathbuf_s = { NULL }, valbuf_s = { NULL };
    pycbc_pybuffer *pathbufs = NULL, *valbufs = NULL;

    if (!PyTuple_Check(spectuple)) {
        PYCBC_EXC_WRAP(PYCBC_EXC_ARGUMENTS, 0, "Value must be a tuple!");
        return -1;
    }

    nspecs = PyTuple_GET_SIZE(spectuple);
    if (nspecs == 0) {
        PYCBC_EXC_WRAP(PYCBC_EXC_ARGUMENTS, 0, "Need one or more commands!");
        return -1;
    }

    newitm = pycbc_sdresult_new(self, spectuple);
    newitm->key = key;
    Py_INCREF(newitm->key);

    if (nspecs == 1) {
        PyObject *single_spec = PyTuple_GET_ITEM(spectuple, 0);
        pathbufs = &pathbuf_s;
        valbufs = &valbuf_s;

        cmd->specs = &spec_s;
        cmd->nspecs = 1;
        rv = sd_convert_spec(single_spec, &spec_s, pathbufs, valbufs);
    } else {
        Py_ssize_t ii;
        specs = calloc(nspecs, sizeof *specs);
        pathbufs = calloc(nspecs, sizeof *pathbufs);
        valbufs = calloc(nspecs, sizeof *valbufs);

        cmd->specs = specs;
        cmd->nspecs = nspecs;

        for (ii = 0; ii < nspecs; ++ii) {
            PyObject *cur = PyTuple_GET_ITEM(spectuple, ii);
            rv = sd_convert_spec(cur, specs + ii, pathbufs + ii, valbufs + ii);
            if (rv != 0) {
                break;
            }
        }
    }

    if (rv == 0) {
        err = lcb_subdoc3(self->instance, mres, cmd);
        if (err == LCB_SUCCESS) {
            PyDict_SetItem((PyObject*)mres, key, (PyObject*)newitm);
            pycbc_assert(Py_REFCNT(newitm) == 2);
        }
    }

    free(specs);
    {
        size_t ii;
        for (ii = 0; nspecs > 0 && ii < (size_t) nspecs; ++ii) {
            PYCBC_PYBUF_RELEASE(pathbufs + ii);
            PYCBC_PYBUF_RELEASE(valbufs + ii);
        }
    }

    if (nspecs > 1) {
        free(pathbufs);
        free(valbufs);
    }

    Py_DECREF(newitm);

    if (err != LCB_SUCCESS) {
        PYCBC_EXCTHROW_SCHED(err);
        return -1;
    } else if (rv != 0) {
        return -1;
    }
    return 0;
}

#ifdef LCB_TRACING
#include <stdio.h>
#include <libcouchbase/couchbase.h>
#include <libcouchbase/api3.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* strlen */
#ifdef _WIN32
#define PRIx64 "I64x"
#define PRId64 "I64d"
#else
#include <inttypes.h>
#endif

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <libcouchbase/tracing.h>
//#include "include/opentracing/lcb_ot.h"
#include "../contrib/cJSON/cJSON.h"

#define COMPONENT_NAME "demo"
#ifdef LCB_TRACING

pycbc_stack_context_handle
get_stack_context4(PyObject *kwargs, const char *operation, uint64_t now, lcbtrace_REF *ref, lcbtrace_TRACER *tracer) {
    pycbc_stack_context_handle *pcontext;
    PyObject *span = PyDict_GetItemString(kwargs, "span");
    if (!operation && span && PyArg_ParseTuple(span, "O!", &TracerType, &pcontext) && pcontext)
    {
        return *pcontext;
    } else{
        return new_stack_context(operation, now, ref, tracer);
    }
}

pycbc_stack_context_handle
new_stack_context(const char *operation, uint64_t now, lcbtrace_REF *ref, lcbtrace_TRACER *tracer) {
    pycbc_stack_context_handle context;
    context.tracer = tracer;
    context.span = lcbtrace_span_start(tracer, operation, now, ref);
    lcbtrace_span_add_tag_str(context.span, LCBTRACE_TAG_COMPONENT, COMPONENT_NAME);
    return context;
}

#endif


struct zipkin_payload;

typedef struct zipkin_payload {
    char *data;
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
#define BUFSZ 1000
        size_t nbuf = BUFSZ;
        char *buf;
        lcbtrace_SPAN *parent;
        uint64_t start;
        zipkin_payload *payload = calloc(1, sizeof(zipkin_payload));
        printf("got span %p\n",span);
        cJSON *json = cJSON_CreateObject();

        buf = calloc(nbuf, sizeof(char));
        cJSON_AddItemToObject(json, "name", cJSON_CreateString(lcbtrace_span_get_operation(span)));
        snprintf(buf, nbuf, "%" PRIx64, lcbtrace_span_get_span_id(span));
        cJSON_AddItemToObject(json, "id", cJSON_CreateString(buf));
        snprintf(buf, nbuf, "%" PRIx64, lcbtrace_span_get_trace_id(span));
        cJSON_AddItemToObject(json, "traceId", cJSON_CreateString(buf));
        parent = lcbtrace_span_get_parent(span);
        if (parent) {
            snprintf(buf, nbuf, "%" PRIx64, lcbtrace_span_get_trace_id(parent));
            cJSON_AddItemToObject(json, "parentId", cJSON_CreateString(buf));
        }
        start = lcbtrace_span_get_start_ts(span);
        cJSON_AddItemToObject(json, "timestamp", cJSON_CreateNumber(start));
        cJSON_AddItemToObject(json, "duration", cJSON_CreateNumber(lcbtrace_span_get_finish_ts(span) - start));

        {
            cJSON *endpoint = cJSON_CreateObject();

            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_DB_TYPE, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(endpoint, "serviceName", cJSON_CreateString(buf));
            }
            cJSON_AddItemToObject(json, "localEndpoint", endpoint);
        }

        {
            cJSON *tags = cJSON_CreateObject();
            uint64_t latency, operation_id;
            if (lcbtrace_span_get_tag_uint64(span, LCBTRACE_TAG_PEER_LATENCY, &latency) == LCB_SUCCESS) {
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_PEER_LATENCY, cJSON_CreateNumber(latency));
            }
            if (lcbtrace_span_get_tag_uint64(span, LCBTRACE_TAG_OPERATION_ID, &operation_id) == LCB_SUCCESS) {
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_OPERATION_ID, cJSON_CreateNumber(operation_id));
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_COMPONENT, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_COMPONENT, cJSON_CreateString(buf));
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_PEER_ADDRESS, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_PEER_ADDRESS, cJSON_CreateString(buf));
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_LOCAL_ADDRESS, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_LOCAL_ADDRESS, cJSON_CreateString(buf));
            }
            nbuf = BUFSZ;
            if (lcbtrace_span_get_tag_str(span, LCBTRACE_TAG_DB_INSTANCE, &buf, &nbuf) == LCB_SUCCESS) {
                buf[nbuf] = '\0';
                cJSON_AddItemToObject(tags, LCBTRACE_TAG_DB_INSTANCE, cJSON_CreateString(buf));
            }
            if (cJSON_GetArraySize(tags) > 0)  {
                cJSON_AddItemToObject(json, "tags", tags);
            } else {
                cJSON_Delete(tags);
            }
        }
        free(buf);


        payload->data = cJSON_PrintUnformatted(json);
        cJSON_Delete(json);
        if (state->last) {
            state->last->next = payload;
        }
        state->last = payload;
        state->content_length += strlen(payload->data) + 1; /* for comma/closing bracket */
        if (state->root == NULL) {
            state->root = payload;
        }
    }
}

void pycbc_loop_send(int sock, char *bytes, ssize_t nbytes)
{
    do {
        ssize_t rv = send(sock, bytes, nbytes, 0);
        if (rv < 0) {
            perror("failed to send data to zipkin: ");
            exit(EXIT_FAILURE);
        } else if (rv < nbytes) {
            nbytes -= rv;
            bytes += rv;
            continue;
        }
        break;
    } while (1);
}

int zipkin_init_dump(const zipkin_state *state, int sock, int rv);

void pycbc_zipkin_flush(lcbtrace_TRACER *tracer)
{
    zipkin_state *state = NULL;
    int sock, rv;

    if (tracer == NULL) {
        return;
    }
    state = tracer->cookie;
    if (state == NULL) {
        return;
    }
    if (state->root == NULL || state->content_length == 0) {
        return;
    }
    sock = zipkin_init_dump(state, sock, rv);
    {
        zipkin_payload *ptr = state->root;
        pycbc_loop_send(sock, "[", 1);
        while (ptr) {
            zipkin_payload *tmp = ptr;
            pycbc_loop_send(sock, ptr->data, strlen(ptr->data));
            ptr = ptr->next;
            if (ptr) {
                pycbc_loop_send(sock, ",", 1);
            }
            free(tmp->data);
            free(tmp);
        }
        pycbc_loop_send(sock, "]", 1);
    }
    close(sock);
    state->root = state->last = NULL;
    state->content_length = 0;
}

int zipkin_init_dump(const zipkin_state *state, int sock, int rv) {
    {
        struct addrinfo hints, *addr, *a;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        rv = getaddrinfo(state->json_api_host, state->json_api_port, &hints, &addr);
        if (rv != 0) {
            fprintf(stderr, "failed to resolve zipkin address getaddrinfo: %s\n", gai_strerror(rv));
            //exit(EXIT_FAILURE);
        }
        for (a = addr; a != NULL; a = a->ai_next) {
            sock = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
            if (sock == -1) {
                perror("failed to create socket for zipkin: ");
                continue;
            }
            rv = connect(sock, a->ai_addr, a->ai_addrlen);
            if (rv == -1) {
                perror("failed to connect socket for zipkin: ");
                continue;
            }
            break;
        }
        if (a == NULL) {
            fprintf(stderr, "unable to connect to zipkin. terminating\n");
            //exit(EXIT_FAILURE);
        }
        freeaddrinfo(addr);
    }
    {
        char preamble[1000] = "";
        size_t size;

        snprintf(preamble, sizeof(preamble),
                 "POST /api/v2/spans HTTP/1.1\r\n"
                         "Content-Type: application/json\r\n"
                         "Accept: */*\r\n"
                         "Connection: close\r\n"
                         "Host: %s:%s\r\n"
                         "Content-Length: %ld\r\n\r\n",
                 state->json_api_host, state->json_api_port, (long)state->content_length + 1 /* for open bracket */);
        size = strlen(preamble);

        rv = send(sock, preamble, size, 0);
        if (rv == -1) {
            perror("failed to send HTTP headers to zipkin: ");
            //exit(EXIT_FAILURE);
        }
    }
    return sock;
}

lcbtrace_TRACER *pycbc_zipkin_new(void)
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
    return tracer;
}




void pycbc_do_encoding(lcbtrace_SPAN *span, lcbtrace_TRACER *tracer, encoding_fn fun, void *payload) {
    lcbtrace_SPAN *encoding;
    lcbtrace_REF ref;
    ref.type = LCBTRACE_REF_CHILD_OF;
    ref.span = span;

    encoding = lcbtrace_span_start(tracer, LCBTRACE_OP_REQUEST_ENCODING, 0, &ref);
    lcbtrace_span_add_tag_str(encoding, LCBTRACE_TAG_COMPONENT, COMPONENT_NAME);
    fun(payload);
    lcbtrace_span_finish(encoding, LCBTRACE_NOW);
}

void pycbc_do_decoding(lcbtrace_SPAN *span, lcbtrace_TRACER *tracer, decoding_fn fun, void* payload) {
    lcbtrace_SPAN *decoding;
    lcbtrace_REF ref;

    ref.type = LCBTRACE_REF_CHILD_OF;
    ref.span = span;

    decoding = lcbtrace_span_start(tracer, LCBTRACE_OP_RESPONSE_DECODING, 0, &ref);
    lcbtrace_span_add_tag_str(decoding, LCBTRACE_TAG_COMPONENT, COMPONENT_NAME);
    fun(payload);
    lcbtrace_span_finish(decoding, LCBTRACE_NOW);
}

void pycbc_encoding_function(void* encoding_time_us) { usleep((int)encoding_time_us); }
void pycbc_decoding_function(void* encoding_time_us) { usleep((int)encoding_time_us); }

void do_span(lcb_error_t *err, struct lcb_st *instance, lcbtrace_SPAN *span,
             lcbtrace_TRACER *tracer) {/* Assign the handlers to be called for the operation types */

    span = lcbtrace_span_start(tracer, "transaction", 0, NULL);
    lcbtrace_span_add_tag_str(span, LCBTRACE_TAG_COMPONENT, COMPONENT_NAME);

    pycbc_do_encoding(span, tracer, pycbc_encoding_function, NULL);

    pycbc_do_decoding(span, tracer, pycbc_decoding_function, NULL);

    lcbtrace_span_finish(span, LCBTRACE_NOW);

    pycbc_zipkin_flush(tracer);
}




static PyGetSetDef Tracer_TABLE_getset[] = {
      /*  { "default_format",
                (getter)Tracer_get_format,
                (setter)Tracer_set_format,
                PyDoc_STR("The default format to use for encoding values "
                                  "(passed to transcoder)")
        },

*/
        { NULL }
};

static struct PyMemberDef Tracer_TABLE_members[] = {
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

static PyMethodDef Tracer_TABLE_methods[] = {

#define OPFUNC(name, doc) \
{ #name, (PyCFunction)pycbc_Tracer_##name, METH_VARARGS|METH_KEYWORDS, \
    PyDoc_STR(doc) }

      /*  *//** Basic Operations *//*
        OPFUNC(upsert, "Unconditionally store a key in Couchbase"),

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
               PyObject *args, PyObject *kwargs)
{
    lcb_error_t err;
    int rv = 0;
    self->tracer=pycbc_zipkin_new();
    pycbc_Bucket* bucket;
    lcb_t instance;
    /**
     * This xmacro enumerates the constructor keywords, targets, and types.
     * This was converted into an xmacro to ease the process of adding or
     * removing various parameters.
     */
#define XCTOR_ARGS(X) \
    X("bucket", "O!",  &bucket)

    static char *kwlist[] = {
#define X(s, target, type) s,
            XCTOR_ARGS(X)
#undef X
            NULL
    };

#define X(s, type, ...) type
    static char *argspec = "|" XCTOR_ARGS(X);
#undef X
    if (self->init_called) {
        PyErr_SetString(PyExc_RuntimeError, "__init__ was already called");
        return -1;
    }

    self->init_called = 1;
#define X(s, type, ...) __VA_ARGS__,
    rv = PyArg_ParseTuple(args, kwargs, argspec, kwlist,
                                     XCTOR_ARGS(X) NULL);
#undef X
    lcb_set_tracer(bucket->instance, self->tracer);


    return rv;
}

static void
Tracer_dtor(pycbc_Tracer_t *self)
{

   /* Py_XDECREF(self->dtorcb);
*/
    lcbtrace_destroy(self->tracer);
    Py_TYPE(self)->tp_free((PyObject*)self);
}
int
pycbc_TracerType_init(PyObject **ptr)
{
    PyTypeObject *p = &TracerType;
    *ptr = (PyObject*)p;

    if (p->tp_name) {
        return 0;
    }

    p->tp_name = "Tracer";
    p->tp_new = PyType_GenericNew;
    p->tp_init = (initproc)Tracer__init__;
    p->tp_dealloc = (destructor)Tracer_dtor;

    p->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
    p->tp_doc = PyDoc_STR("The connection object");

    p->tp_basicsize = sizeof(pycbc_Tracer_t);

    p->tp_methods = Tracer_TABLE_methods;
    p->tp_members = Tracer_TABLE_members;
    p->tp_getset = Tracer_TABLE_getset;

    pycbc_DummyTuple = PyTuple_New(0);
    pycbc_DummyKeywords = PyDict_New();

    return PyType_Ready(p);
}
#endif