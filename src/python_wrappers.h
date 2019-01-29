//
// Created by Ellis Breen on 2019-04-23.
//
#ifndef COUCHBASE_PYTHON_CLIENT_PYTHON_WRAPPERS_H
#define COUCHBASE_PYTHON_CLIENT_PYTHON_WRAPPERS_H

#include "Python.h"

typedef struct pycbc_stack_context_decl *pycbc_stack_context_handle;
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

void pycbc_dict_add_text_kv_strn(PyObject *dict,
                                 pycbc_strn_base_const strn_key,
                                 pycbc_strn_base_const strn_value);

void pycbc_dict_add_text_kv(PyObject *dict, const char *key, const char *value);

const char *pycbc_strn_buf(pycbc_strn buf);

int pycbc_strn_valid(pycbc_strn buf);

size_t pycbc_strn_len(pycbc_strn_base_const buf);

pycbc_strn_unmanaged pycbc_strn_from_managed(PyObject *source);

char *pycbc_strn_buf_psz(pycbc_strn_unmanaged buf);

void pycbc_strn_free(pycbc_strn_unmanaged buf);

pycbc_generic_array pycbc_strn_base_const_array(pycbc_strn_base_const orig);

#endif // COUCHBASE_PYTHON_CLIENT_PYTHON_WRAPPERS_H
