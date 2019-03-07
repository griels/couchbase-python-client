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

int
pycbc_httpresult_ok(pycbc_HttpResult *self)
{
    if (self->rc == LCB_SUCCESS &&
            ((self->htcode < 300 && self->htcode > 199) || self->htcode == 0)) {
        return 1;
    }
    return 0;
}

static PyObject *
HttpResult_success(pycbc_HttpResult *self, void *unused)
{
    PyObject *ret = NULL;
    if (pycbc_httpresult_ok(self)) {
        ret = Py_True;
    } else {
        ret = Py_False;
    }

    Py_INCREF(ret);

    (void)unused;
    return ret;
}

static PyObject *
HttpResult_headers(pycbc_HttpResult *self, void *unused)
{
    (void)unused;
    if (!self->headers) {
        Py_RETURN_NONE;
    }
    Py_INCREF(self->headers);
    return self->headers;
}


static void
HttpResult_dealloc(pycbc_HttpResult *self)
{
    if (self->u.htreq) {
        if (self->parent) {
            if (self->htype == PYCBC_HTTP_HVIEW) {
                lcb_view_cancel(self->parent->instance, self->u.vh);
            } else if (self->htype == PYCBC_HTTP_HN1QL) {
                lcb_n1ql_cancel(self->parent->instance, self->u.nq);
            } else if (self->htype == PYCBC_HTTP_HFTS) {
                lcb_fts_cancel(self->parent->instance, self->u.fts);
            } else {
                lcb_http_cancel(self->parent->instance, self->u.htreq);
            }
        }
        self->u.htreq = NULL;
    }
    Py_XDECREF(self->http_data);
    Py_XDECREF(self->headers);
    Py_XDECREF(self->parent);
    pycbc_Result_dealloc((pycbc_Result*)self);
}


static struct PyMemberDef HttpResult_TABLE_members[] = {
        { "http_status",
                T_USHORT, offsetof(pycbc_HttpResult, htcode),
                READONLY, PyDoc_STR("HTTP Status Code")
        },

        { "value",
                T_OBJECT_EX, offsetof(pycbc_HttpResult, http_data),
                READONLY, PyDoc_STR("HTTP Payload")
        },

        { "url",
                T_OBJECT_EX, offsetof(pycbc_HttpResult, key),
                READONLY, PyDoc_STR("HTTP URI")
        },
        { "done",
                T_BOOL, offsetof(pycbc_HttpResult, done),
                READONLY, PyDoc_STR("If the result is done")
        },
        { NULL }
};

static PyGetSetDef HttpResult_TABLE_getset[] = {
        { "success", (getter)HttpResult_success, NULL,
                PyDoc_STR("Whether the HTTP request was successful")
        },

        { "headers", (getter)HttpResult_headers, NULL,
                PyDoc_STR("Headers dict for the request. ")
        },

        { NULL }
};

static PyMethodDef HttpResult_TABLE_methods[] = {
        { NULL }
};

PyTypeObject pycbc_HttpResultType = {
        PYCBC_POBJ_HEAD_INIT(NULL)
        0
};

int
pycbc_HttpResultType_init(PyObject **ptr)
{
    PyTypeObject *p = &pycbc_HttpResultType;
    *ptr = (PyObject*)p;

    if (p->tp_name) {
        return 0;
    }
    p->tp_name = "HttpResult";
    p->tp_doc = PyDoc_STR("Generic object returned for HTTP operations\n");
    p->tp_new = PyType_GenericNew;
    p->tp_basicsize = sizeof(pycbc_HttpResult);
    p->tp_base = &pycbc_ResultType;
    p->tp_getset = HttpResult_TABLE_getset;
    p->tp_members = HttpResult_TABLE_members;
    p->tp_methods = HttpResult_TABLE_methods;
    p->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
    p->tp_dealloc = (destructor)HttpResult_dealloc;
    return pycbc_ResultType_ready(p, PYCBC_HTRESULT_BASEFLDS);
}

void
pycbc_httpresult_init(pycbc_HttpResult *self, pycbc_MultiResult *mres)
{
    PyDict_SetItem((PyObject*)mres, Py_None, (PyObject*)self);
    Py_DECREF(self);
    self->parent = mres->parent;
    Py_INCREF(self->parent);
}
