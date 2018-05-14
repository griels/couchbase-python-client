/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2018 Couchbase, Inc.
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
 */

#if !defined(LCB_TRACING_H) && defined(LCB_TRACING)
#define LCB_TRACING_H

/**
 * @file
 * End to end tracing
 *
 * @uncommitted
 */

#ifdef __cplusplus
namespace lcb
{
namespace trace
{
class Span;
class Tracer;
} // namespace trace
} // namespace lcb
typedef lcb::trace::Span lcbtrace_SPAN;
extern "C" {
#else /* C only! */
typedef struct lcbtrace_SPAN_Cdummy lcbtrace_SPAN;
#endif

#define LCBTRACE_F_THRESHOLD 0x01

struct lcbtrace_TRACER;
typedef struct lcbtrace_TRACER {
    lcb_U16 version;
    lcb_U64 flags;
    void *cookie;
    void (*destructor)(struct lcbtrace_TRACER *tracer);
    union {
        struct {
            void (*report)(struct lcbtrace_TRACER *tracer, lcbtrace_SPAN *span);
        } v0;
    } v;
} lcbtrace_TRACER;

/**
 * @uncommitted
 * @return tracer
 */
LIBCOUCHBASE_API lcbtrace_TRACER *lcb_get_tracer(lcb_t instance);

/**
 * @uncommitted
 * @return tracer
 */
LIBCOUCHBASE_API void lcb_set_tracer(lcb_t instance, lcbtrace_TRACER *tracer);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API lcbtrace_TRACER *lcbtrace_new(lcb_t instance, lcb_U64 flags);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API void lcbtrace_destroy(lcbtrace_TRACER *tracer);

typedef enum {
    LCBTRACE_REF_NONE = 0,
    LCBTRACE_REF_CHILD_OF = 1,
    LCBTRACE_REF_FOLLOWS_FROM,
    LCBTRACE_REF__MAX
} lcbtrace_REF_TYPE;

typedef struct {
    lcbtrace_REF_TYPE type;
    lcbtrace_SPAN *span;
} lcbtrace_REF;

#define LCBTRACE_OP_GET "get"

/** zero means the library will trigger timestamp automatically */
#define LCBTRACE_NOW 0

/**
 * @return time in microseconds
 */
LIBCOUCHBASE_API
lcb_U64 lcbtrace_now(void);

/**
 * @uncommitted
 * @param tracer
 * @param operation
 * @param now
 * @param ctx
 */
LIBCOUCHBASE_API
lcbtrace_SPAN *lcbtrace_span_start(lcbtrace_TRACER *tracer, const char *operation, lcb_U64 now, lcbtrace_REF *ref);

/**
 * @uncommitted
 * @param span
 * @param now
 */
LIBCOUCHBASE_API
void lcbtrace_span_finish(lcbtrace_SPAN *span, lcb_U64 now);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_U64 lcbtrace_span_get_start_ts(lcbtrace_SPAN *span);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_U64 lcbtrace_span_get_finish_ts(lcbtrace_SPAN *span);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
int lcbtrace_span_is_orphaned(lcbtrace_SPAN *span);

/**
 * @uncomitted
 */
LIBCOUCHBASE_API
const char *lcbtrace_span_get_operation(lcbtrace_SPAN *span);

#define LCBTRACE_OP_REQUEST_ENCODING "request_encoding"
#define LCBTRACE_OP_DISPATCH_TO_SERVER "dispatch_to_server"
#define LCBTRACE_OP_RESPONSE_DECODING "response_decoding"

#define LCBTRACE_OP_ADD "add"
#define LCBTRACE_OP_APPEND "append"
#define LCBTRACE_OP_COUNTER "counter"
#define LCBTRACE_OP_GET "get"
#define LCBTRACE_OP_GET_FROM_REPLICA "get_from_replica"
#define LCBTRACE_OP_INSERT "insert"
#define LCBTRACE_OP_OBSERVE_CAS "observe_cas"
#define LCBTRACE_OP_OBSERVE_CAS_ROUND "observe_cas_round"
#define LCBTRACE_OP_OBSERVE_SEQNO "observe_seqno"
#define LCBTRACE_OP_PREPEND "prepend"
#define LCBTRACE_OP_REMOVE "remove"
#define LCBTRACE_OP_REPLACE "replace"
#define LCBTRACE_OP_TOUCH "touch"
#define LCBTRACE_OP_UNLOCK "unlock"
#define LCBTRACE_OP_UPSERT "upsert"
#define LCBTRACE_OP_UPSERT "upsert"

#define LCBTRACE_OP_STORE2NAME(code)                \
    (code == LCB_ADD) ? LCBTRACE_OP_ADD :           \
    (code == LCB_PREPEND) ? LCBTRACE_OP_PREPEND :   \
    (code == LCB_APPEND) ? LCBTRACE_OP_APPEND :     \
    LCBTRACE_OP_UPSERT

#define LCBTRACE_TAG_DB_TYPE "db.type"
#define LCBTRACE_TAG_SPAN_KIND "span.kind"
/**
 * Bucket name
 */
#define LCBTRACE_TAG_DB_INSTANCE "db.instance"
/**
 * The client's identifier string (the 'u' property in the updated HELLO request),
 * the same one that is shared with the server to identify the SDK.
 */
#define LCBTRACE_TAG_COMPONENT "component"
/**
 * The unique ID of the operation
 */
#define LCBTRACE_TAG_OPERATION_ID "couchbase.operation_id"
/**
 * The service type, one of the following:
 * kv, view, n1ql, search, analytics
 */
#define LCBTRACE_TAG_SERVICE "couchbase.service"
#define LCBTRACE_TAG_SERVICE_KV "kv"
#define LCBTRACE_TAG_SERVICE_VIEW "view"
#define LCBTRACE_TAG_SERVICE_N1QL "n1ql"
#define LCBTRACE_TAG_SERVICE_SEARCH "search"
#define LCBTRACE_TAG_SERVICE_ANALYTICS "analytics"

#define LCBTRACE_TAG_LOCAL_ID "couchbase.local_id"
/**
 * The local socket hostname / IP and port, in the format: {hostname}:{port}
 * To be added to dispatch spans when the local socket is available.
 */
#define LCBTRACE_TAG_LOCAL_ADDRESS "local.address"
/**
 * The remote socket hostname / IP and port, in the format: {hostname}:{port}
 * To be added to dispatch spans when the local socket is available.
 */
#define LCBTRACE_TAG_PEER_ADDRESS "peer.address"
/**
 * The server duration with precision suffix. The suffix is required to indicate
 * precision because KV is recorded in microseconds and N1QL query metrics
 * use milliseconds.
 * For example: 123us, 32.12ms
 */
#define LCBTRACE_TAG_PEER_LATENCY "peer.latency"

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_U64 lcbtrace_span_get_span_id(lcbtrace_SPAN *span);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_U64 lcbtrace_span_get_trace_id(lcbtrace_SPAN *span);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcbtrace_SPAN *lcbtrace_span_get_parent(lcbtrace_SPAN *span);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_error_t lcbtrace_span_get_tag_str(lcbtrace_SPAN *span, const char *name, char **value, size_t *nvalue);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_error_t lcbtrace_span_get_tag_uint64(lcbtrace_SPAN *span, const char *name, lcb_U64 *value);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_error_t lcbtrace_span_get_tag_double(lcbtrace_SPAN *span, const char *name, double *value);

/**
 * @uncommitted
 */
LIBCOUCHBASE_API
lcb_error_t lcbtrace_span_get_tag_bool(lcbtrace_SPAN *span, const char *name, int value);

/**
 * @uncommitted
 * @param span
 * @param name
 * @param value
 */
LIBCOUCHBASE_API
void lcbtrace_span_add_tag_str(lcbtrace_SPAN *span, const char *name, const char *value);

/**
 * @uncommitted
 * @param span
 * @param name
 * @param value
 */
LIBCOUCHBASE_API
void lcbtrace_span_add_tag_uint64(lcbtrace_SPAN *span, const char *name, lcb_U64 value);

/**
 * @uncommitted
 * @param span
 * @param name
 * @param value
 */
LIBCOUCHBASE_API
void lcbtrace_span_add_tag_double(lcbtrace_SPAN *span, const char *name, double value);

/**
 * @uncommitted
 * @param span
 * @param name
 * @param value
 */
LIBCOUCHBASE_API
void lcbtrace_span_add_tag_bool(lcbtrace_SPAN *span, const char *name, int value);

/**
 * Sets the tracing context for the command.
 *
 * @param cmd the command structure
 * @param ctx the lcbtrace_SPAN pointer
 * @uncommitted
 */
#define LCB_CMD_SET_TRACESPAN(cmd, span)                                                                               \
    do {                                                                                                               \
        (cmd)->_hashkey.type = LCB_KV_TRACESPAN;                                                                       \
        (cmd)->_hashkey.contig.bytes = span;                                                                           \
    } while (0);

/**
 * @uncommitted
 */
typedef struct {
    void *state;
    void (*report)(void *state, lcbtrace_SPAN *span);
} ldcptrace_REPORTER;

#ifdef __cplusplus
}
#endif
#endif /* LCB_TRACING_H */