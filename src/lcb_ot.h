//
// Created by Ellis Breen on 23/01/2018.
//

#ifndef OPENTRACING_CPP_LCB_OT_H
#define OPENTRACING_CPP_LCB_OT_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stddef.h"
#ifdef __cplusplus
//extern "C" {
#endif

// borrowed from libcouchbase C defs

typedef struct opentracing_string_t {
    const char* data;
    int size;
} opentracing_string_t;

typedef struct opentracing_tracer_t opentracing_tracer_t;


typedef bool(*opentracing_foreach_key_value_callback_t)(
        const opentracing_string_t* key, const opentracing_string_t* value);

typedef struct opentracing_span_context_t {
    void (*destructor)(void* self);
    void (*foreach_baggage_item)(void* self,
                                 opentracing_foreach_key_value_callback_t f);
} opentracing_span_context_t;

typedef enum opentracing_span_reference_type_t {
    opentracing_span_reference_child_of_ref = 1,
    opentracing_span_reference_follows_from_ref = 2
} opentracing_span_reference_type_t;
typedef struct opentracing_span_references_t {
    opentracing_span_reference_type_t type;
    const opentracing_span_context_t* context;
    struct opentracing_span_references_t* next;
} opentracing_span_references_t;

typedef enum opentracing_value_index_t {
    opentracing_value_index_bool,
    opentracing_value_index_double,
    opentracing_value_index_int64,
    opentracing_value_index_uint64,
    opentracing_value_index_string_buffer,
    opentracing_value_index_string,
    opentracing_value_index_values,
    opentracing_value_index_dictionary,
    opentracing_value_index_null = -1
} opentracing_value_index_t;

typedef struct opentracing_values_t opentracing_values_t;

typedef struct opentracing_dictionary_t opentracing_dictionary_t;
typedef struct opentracing_string_buffer_t {
    char* data;
    int size;
    int capacity;
} opentracing_string_buffer_t;

typedef struct opentracing_value_t {
    opentracing_value_index_t value_index;
    union {
        bool bool_value;
        double double_value;
        int64_t int64_value;
        uint64_t uint64_value;
        opentracing_string_buffer_t string_buffer_value;
        opentracing_string_t string_value;
        opentracing_values_t* list_value;
        opentracing_dictionary_t* dict_value;
    } data;
} opentracing_value_t;

#define LCB_OPENTRACING_DICT(name,type)\
typedef struct name {\
    type key;\
    opentracing_value_t value;\
    struct name* next;\
} name;

// opentracing string utilities

void lcb_ot_pt_str(const opentracing_string_t* string)
{
    char* buffer= malloc(string->size+1);
    memcpy(buffer,string->data,string->size);
    buffer[string->size]=0;
    printf("%s",buffer);
    free(buffer);
}

#ifdef OT_STR_GEN
#error OT_STR_GEN defined
#endif

#define OT_STR_GEN_VAL(X) {#X,sizeof(#X)/sizeof(char)}
#define STAT_STR_GEN(X) static opentracing_string_t result=OT_STR_GEN_VAL(X)
#define OT_STR_GEN(X) { STAT_STR_GEN(X); return &result; };
// enum-based tag/span IDs with reflection

// spans
#define PP_EACH_SPAN_ID(MAND,OPT,DIV)\
    OPT(RequestQueue) DIV\
    MAND(RequestEncoding) DIV\
    MAND(DispatchToServer) DIV\
    MAND(ResponseDecoding) DIV\
    OPT(ResponseResolution)

typedef struct lcb_span_id_t {
    const char* reserved;
    enum id {
        #define MAND(X) lcb_span_id_##X
        #define OPT(X) MAND(X)
        #define DIV ,
            PP_EACH_SPAN_ID(MAND,OPT,DIV)
        #undef DIV
        #undef OPT
        #undef MAND
    } m_id;
} lcb_span_id_t;

const opentracing_string_t* lcb_ot_id_str(lcb_span_id_t id)
{
    switch(id.m_id)
    {
        #define MAND(X)\
            case lcb_span_id_##X:\
                OT_STR_GEN(X)
        #define OPT(X) MAND(X)
        #define DIV
            break;
        PP_EACH_SPAN_ID(MAND,OPT,DIV);
        #undef DIV
        #undef OPT
        #undef MAND
            default:
                OT_STR_GEN("");
    }
    return NULL;
}



#ifdef PP_EACH_TAG_ID
#error PP_EACH_TAG_ID defined already
#endif
#define PP_EACH_TAG_ID(NAMESPACE_FN,DIV)\
    NAMESPACE_FN(couchbase,operation_id,service) DIV\
    NAMESPACE_FN(local,address) DIV\
    NAMESPACE_FN(peer,address)

#define DIV ,
#define NS_ENUM(X,...) lcb_tag_ns_##X
    typedef enum lcb_tag_ns_t { PP_EACH_TAG_ID(NS_ENUM,DIV) } lcb_tag_ns_t;
#undef NS_ENUM
#undef DIV

#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME

#define ENCODE1(FN,PREFIX,x,...) FN(PREFIX,x,...)
#define ENCODE2(FN,PREFIX,x,...) FN(PREFIX,x,...) ENCODE1(FN,PREFIX,__VA_ARGS__)
#define ENCODE3(FN,PREFIX,x,...) FN(PREFIX,x,...) ENCODE2(FN,PREFIX,__VA_ARGS__)
#define ENCODE4(FN,PREFIX,x,...) FN(PREFIX,x,...) ENCODE3(FN,PREFIX,__VA_ARGS__)
#define ENCODE5(FN,PREFIX,x,...) FN(PREFIX,x,...) ENCODE4(FN,PREFIX,__VA_ARGS__)
#define ENCODE6(FN,PREFIX,x,...) FN(PREFIX,x,...) ENCODE5(FN,PREFIX,__VA_ARGS__)

#define FOO(PREFIX_FN,PREFIX,...) GET_MACRO(__VA_ARGS__, ENCODE4, ENCODE3, ENCODE2, ENCODE1)(PREFIX_FN,PREFIX,__VA_ARGS__)
#define WRAP(PREFIX,VAL,...) lcb_tag_id_##PREFIX##_##VAL,
#define TAG_ENUM(X,...) typedef enum lcb_tag_id_##X##_t {FOO(WRAP,X,__VA_ARGS__) } lcb_tag_id_##X##_t;
#define DIV
PP_EACH_TAG_ID(TAG_ENUM,DIV)
#undef TAG_ENUM
#undef DIV
//tags
typedef struct lcb_tag_id_t
{
    const char* reserved; // NULL
    enum lcb_tag_ns_t ns;
    size_t b;

} lcb_tag_id_t;

const opentracing_string_t* lcb_ot_tag_str(lcb_tag_id_t id)
{
    const opentracing_string_t* prefix={0};
    switch(id.ns)
    {
        #define PREFIX_FN(PREFIX,X,...) case lcb_tag_id_##PREFIX##_##X : OT_STR_GEN(PREFIX.X)
        #define MAND(X,...)\
            case lcb_tag_ns_##X:\
                switch(id.b)\
                {\
                    FOO(PREFIX_FN,X,__VA_ARGS__)\
                }
        #define DIV break;
        PP_EACH_TAG_ID(MAND, DIV);
        #undef DIV
        #undef MAND
            default:
                OT_STR_GEN("");
                break;
    }
    return NULL;
}
#define DIV
#define TAG_TYPE(NAMESPACE,...)\
\
typedef struct lcb_tag_set_##NAMESPACE\
{\
    opentracing_value_t __VA_ARGS__;\
} lcb_tag_set_##NAMESPACE;\

PP_EACH_TAG_ID(TAG_TYPE,DIV);
#undef TAG_TYPE
#undef DIV

#define DIV
union tag_t {
#define TAG_ENTRY(X,...)\
    lcb_tag_set_##X m_##X;
    PP_EACH_TAG_ID(TAG_ENTRY,DIV);
} tag_t;
#undef TAG_ENTRY
#undef DIV

#undef PP_EACH_TAG_ID

#define OT_STR_VAL(X) {opentracing_value_index_string,.data.string_value=OT_STR_GEN_VAL(X)}
#define TAG_ID(nspace,type) {NULL, lcb_tag_ns_##nspace, offsetof(lcb_tag_set_##nspace,type)/sizeof(opentracing_value_t)};

int main(int argc,const char* argv[])
{
    lcb_tag_id_t x=TAG_ID(couchbase,operation_id);
    lcb_ot_pt_str(lcb_ot_tag_str(x));
    lcb_tag_set_couchbase y={.operation_id=OT_STR_VAL(Hello),.service=OT_STR_VAL(World)};
    lcb_span_id_t test={NULL,lcb_span_id_DispatchToServer};
    lcb_ot_pt_str(lcb_ot_id_str(test));
}

#undef OT_STR_GEN



// use a union of opentracing_string_t and tag/span enum types, the latter will always start with a NULL const char*
#define LCB_OT_STR_UNION(type) \
typedef union lcb_opentracing_##type##_t {\
    opentracing_string_t name;\
    lcb_##type##_t id;\
} lcb_opentracing_##type##_t;

LCB_OT_STR_UNION(tag_id);
LCB_OT_STR_UNION(span_id);


LCB_OPENTRACING_DICT(lcb_opentracing_tags_t,lcb_tag_id_t);
typedef struct lcb_opentracing_start_span_options_t {
    struct timespec start_timestamp;
    const opentracing_span_references_t* references;
    const lcb_opentracing_tags_t* tags;
} lcb_opentracing_start_span_options_t;

typedef struct lcb_opentracing_finish_span_options_t {
    struct timespec finish_timestamp;
} lcb_opentracing_finish_span_options_t;

typedef struct lcb_opentracing_span_t {
    void (*destructor)(void* self);
    void (*finish)(void* self,
                   const lcb_opentracing_finish_span_options_t* options);
    void (*set_operation_id)(void* self,
                               const lcb_opentracing_span_id_t* name);
    void (*set_tag)(void* self,
                    const lcb_opentracing_tag_id_t* key,
                    const opentracing_value_t* value);
    // baggage callbacks currently neither called nor implemented by libcouchbase, but left for future expansion
    void (*set_baggage_item)(void* self,
                             const opentracing_string_t* restricted_key,
                             const opentracing_string_t* value); // currently unused, leave as NULL if unimplemented
    const opentracing_value_t* (*baggage_item)(
        const void* self,
        const opentracing_string_t* restricted_key); // currently unused, leave as NULL if unimplemented
    void (*log)(void* self,
                const opentracing_dictionary_t* fields);
    const opentracing_span_context_t* (*get_context)(const void* self);
    const opentracing_tracer_t* (*get_tracer)(const void* self);
} lcb_opentracing_span_t;


struct lcb_opentracing_tracer_t {
    void (*destructor)(void* self);
    lcb_opentracing_span_t* (*start_span_with_options)(
            const void* self,
            const opentracing_string_t* operation_name,
            const lcb_opentracing_start_span_options_t* options);

    void (*close)(void* self);
};



#endif //OPENTRACING_CPP_LCB_OT_H
