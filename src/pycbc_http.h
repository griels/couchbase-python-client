//
// Created by Ellis Breen on 2019-02-26.
//

#ifndef COUCHBASE_PYTHON_CLIENT_HTTP_H
#define COUCHBASE_PYTHON_CLIENT_HTTP_H
#include "pycbc.h"
#if PYCBC_LCB_API<0x031000
typedef lcb_VIEW_HANDLE*  pycbc_VIEW_HANDLE;
typedef lcb_N1QL_HANDLE*  pycbc_N1QL_HANDLE;
typedef lcb_FTS_HANDLE* pycbc_FTS_HANDLE;
typedef lcb_HTTP_HANDLE* pycbc_HTTP_HANDLE;
#else
#include <libcouchbase/http.h>

#include "libcouchbase/cbft.h"
typedef lcb_VIEWHANDLE  pycbc_VIEW_HANDLE;
typedef lcb_N1QLHANDLE  pycbc_N1QL_HANDLE;
typedef lcb_FTSHANDLE pycbc_FTS_HANDLE;
#endif
#include <libcouchbase/couchbase.h>
#include <libcouchbase/ixmgmt.h>

typedef struct {
    pycbc_Result_HEAD
    PyObject *http_data;
    PyObject *headers;
    pycbc_Bucket *parent;
    union {
        pycbc_HTTP_HANDLE htreq;
        pycbc_VIEW_HANDLE vh;
        pycbc_N1QL_HANDLE nq;
        pycbc_FTS_HANDLE fts;
    } u;
    unsigned int format;
    unsigned short htcode;
    unsigned char done;
    unsigned char htype;
} pycbc_HttpResult;
typedef struct {
    pycbc_HttpResult base;
    PyObject *rows;
    long rows_per_call;
    char has_parse_error;
#ifdef PYCBC_TRACING
    PyObject *context_capsule;
#endif
} pycbc_ViewResult;


/* Not an allocator per-se, but rather an initializer */
void pycbc_httpresult_init(pycbc_HttpResult *self, pycbc_MultiResult *parent);

/* For observe info */
pycbc_ObserveInfo * pycbc_observeinfo_new(pycbc_Bucket *parent);

/**
 * If an HTTP result was successful or not
 */
int pycbc_httpresult_ok(pycbc_HttpResult *self);

pycbc_ViewResult *pycbc_propagate_view_result(
        pycbc_stack_context_handle context);

/**
 * Append data to the HTTP result
 * @param mres The multi result
 * @param htres The HTTP result
 * @param bytes Data to append
 * @param nbytes Length of data
 */
void
pycbc_httpresult_add_data(pycbc_MultiResult *mres, pycbc_HttpResult *htres,
                          const void *bytes, size_t nbytes);

/**
 * Append data to the HTTP result
 * @param mres The multi result
 * @param htres The HTTP result
 * @param strn the data as a pycbc_strn_unmanaged
 *
 */
void pycbc_httpresult_add_data_strn(pycbc_MultiResult *mres, pycbc_HttpResult *htres,
                                    pycbc_strn_base_const strn);

/**
 * Signal completion of an HTTP result.
 *
 * @param htres The HTTP result (Python)
 * @param mres The MultiResult object
 * @param err Error code (for the HTTP operation)
 * @param status The status code
 * @param headers The headers
 */
void
pycbc_httpresult_complete(pycbc_HttpResult *htres, pycbc_MultiResult *mres,
                          lcb_error_t err, short status,
                          const char * const *headers);

/**
 * Add more data to the view's row list.
 *
 * This function will attempt to parse the data as JSON, and store an appropriate
 * error code otherwise.
 *
 * @param vres The ViewResult object
 * @param mres The MultiResult object
 * @param data Buffer
 * @param n Length of buffer
 */
void
pycbc_viewresult_addrow(pycbc_ViewResult *vres, pycbc_MultiResult *mres,
                        const void *data, size_t n);

/**
 * Attempt to notify the relevant callbacks for new data, if the constraints
 * allow it.
 *
 * This will invoke the callback in asynchronous mode, and will break
 * the event loop
 *
 * @param vres The ViewResult
 * @param mres The MultiResult
 * @param bucket The Bucket
 * @param force_callback whether the async callback should be forcefully invoked,
 * ignoring the rows_per_call setting (usually only required on error or when
 * there are no more rows).
 */
void
pycbc_viewresult_step(pycbc_ViewResult *vres, pycbc_MultiResult *mres,
                      pycbc_Bucket *bucket, int force_callback);

#endif //COUCHBASE_PYTHON_CLIENT_HTTP_H
