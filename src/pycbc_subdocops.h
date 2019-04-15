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

#ifndef COUCHBASE_PYTHON_CLIENT_PYCBC_SUBDOCOPS_H
#define COUCHBASE_PYTHON_CLIENT_PYCBC_SUBDOCOPS_H
//
// Created by Ellis Breen on 2019-04-04.
//
#include "pycbc.h"
#include "oputil.h"
#if PYCBC_LCB_API > 0x030001
#    define PYCBC_X_SD_OPS_FULLDOC(X, NP, VAL, MVAL, CTR,...) \
        NP(FULLDOC_GET, fulldoc_get, __VA_ARGS__)                      \
        X(FULLDOC_UPSERT, fulldoc_upsert, __VA_ARGS__)                 \
        X(FULLDOC_ADD, fulldoc_add, __VA_ARGS__)                       \
        X(FULLDOC_REPLACE, fulldoc_replace, __VA_ARGS__)               \
        NP(FULLDOC_REMOVE, fulldoc_remove, __VA_ARGS__)
#else
#    define PYCBC_X_SD_OPS_FULLDOC(X, NP, VAL, MVAL, CTR, ...) \
        NP(GET_FULLDOC, get_fulldoc, __VA_ARGS__)                      \
        X(SET_FULLDOC, set_fulldoc, __VA_ARGS__)                       \
        NP(REMOVE_FULLDOC, remove_fulldoc, __VA_ARGS__)
#endif

#define PYCBC_X_SD_OPS(X, NP, VAL, MVAL, CTR, ...) \
    X(GET, get, __VA_ARGS__ )                               \
    X(EXISTS, exists, __VA_ARGS__)                         \
    VAL(REPLACE, replace, __VA_ARGS__)                     \
    VAL(DICT_ADD, dict_add, __VA_ARGS__)                   \
    VAL(DICT_UPSERT, dict_upsert, __VA_ARGS__)             \
    MVAL(ARRAY_ADD_FIRST, array_add_first, __VA_ARGS__)    \
    MVAL(ARRAY_ADD_LAST, array_add_last, __VA_ARGS__)      \
    VAL(ARRAY_ADD_UNIQUE, array_add_unique, __VA_ARGS__)   \
    MVAL(ARRAY_INSERT, array_insert, __VA_ARGS__)          \
    CTR(COUNTER, counter, __VA_ARGS__)                     \
    X(REMOVE, remove, __VA_ARGS__)                         \
    X(GET_COUNT, get_count, __VA_ARGS__)                   \
    PYCBC_X_SD_OPS_FULLDOC(X, NP, VAL, MVAL, CTR, __VA_ARGS__)

#define DUMMY(...)
#define PYCBC_SDCMD_CASE_GENERIC(UC, LC, FN, ...) \
case LCB_SDCMD_##UC:                              \
    PYCBC_DEBUG_LOG("Handling case :" #UC)        \
    FN(UC, LC, __VA_ARGS__)                       \
    break;

#if PYCBC_LCB_API < 0x031000

#    define PYCBC_SDSPEC_SET_XX(POSTFIX, DEST, BUF, BUF_LEN)  \
        {                                                     \
            if (BUF && BUF_LEN) {                             \
                LCB_SDSPEC_SET_##POSTFIX(DEST, BUF, BUF_LEN); \
            }                                                 \
        }
#    define PYCBC_SDSPEC_SET_PATH(DEST, BUF, BUF_LEN) \
        PYCBC_SDSPEC_SET_XX(PATH, DEST, BUF, BUF_LEN)
#    define PYCBC_SDSPEC_SET_VALUE(DEST, BUF, BUF_LEN) \
        PYCBC_SDSPEC_SET_XX(VALUE, DEST, BUF, BUF_LEN)
#    define PYCBC_PATH_ONLY(UC, LC, DECL, BODY, ...)                                      \
        DECL(lcb_subdocops_##LC(lcb_SUBDOCOPS *operations,                         \
                           size_t index,                                      \
                           uint32_t flags,                                    \
                           const char *path,                                  \
                           size_t path_len))                                   \
        BODY({                                                                     \
            PYCBC_SDSPEC_SET_PATH(&operations->specs[index], path, path_len); \
            operations->specs[index].options = flags;                         \
            operations->specs[index].sdcmd = LCB_SDCMD_##UC;                  \
            return LCB_SUCCESS;                                               \
        })
#    define PYCBC_COUNTER(UC, LC, DECL, BODY, ...)                                        \
        DECL(lcb_subdocops_##LC(lcb_SUBDOCOPS *operations,                         \
                           size_t index,                                      \
                           uint32_t flags,                                    \
                           const char *path,                                  \
                           size_t path_len,                                   \
                           int64_t delta))                                     \
        BODY({                                                                     \
            char *value = (char *)calloc(22, sizeof(char));                   \
            size_t value_len = snprintf(value, 21, "%" PRId64, delta);        \
            PYCBC_SDSPEC_SET_PATH(&operations->specs[index], path, path_len); \
            PYCBC_SDSPEC_SET_VALUE(                                           \
                    &operations->specs[index], value, value_len);             \
            operations->specs[index].options = flags;                         \
            operations->specs[index].sdcmd = LCB_SDCMD_##UC;                  \
            return LCB_SUCCESS;                                               \
        })
#    define PYCBC_NP(UC, LC, DECL, BODY, ...)                                        \
        DECL(lcb_subdocops_##LC(                                              \
                lcb_SUBDOCOPS *operations, size_t index, uint32_t flags)) \
        BODY({                                                                \
            operations->specs[index].options = flags;                    \
            operations->specs[index].sdcmd = LCB_SDCMD_##UC;             \
            return LCB_SUCCESS;                                          \
        })
#    define PYCBC_VAL_GEN(UC, LC, DECL, BODY,...)                                        \
        DECL(lcb_subdocops_##LC(lcb_SUBDOCOPS *operations,                         \
                           size_t index,                                      \
                           uint32_t flags,                                    \
                           const char *path,                                  \
                           size_t path_len,                                   \
                           const char *value,                                 \
                           size_t value_len))                                  \
        BODY({                                                                     \
            PYCBC_SDSPEC_SET_PATH(&operations->specs[index], path, path_len); \
            PYCBC_SDSPEC_SET_VALUE(                                           \
                    &operations->specs[index], value, value_len);             \
            operations->specs[index].options = flags;                         \
            operations->specs[index].sdcmd = LCB_SDCMD_##UC;                  \
            return LCB_SUCCESS;                                               \
        })

#define LITERAL(...) __VA_ARGS__
#define LITERAL_DECL(...) __VA_ARGS__;
#    define PYCBC_SDCMD_FN_DEF(UC, LC, FN, ...) \
        LIBCOUCHBASE_API lcb_STATUS FN(UC, LC, __VA_ARGS__)
#    define PYCBC_SDCMD_CASE(UC, LC, ...) \
        PYCBC_SDCMD_FN_DEF(UC, LC, PYCBC_PATH_ONLY, __VA_ARGS__)
#    define PYCBC_SDCMD_CASE_NP(UC, LC, ...) \
        PYCBC_SDCMD_FN_DEF(UC, LC, PYCBC_NP, __VA_ARGS__)
#    define PYCBC_SDCMD_CASE_VAL(UC, LC, ...) \
        PYCBC_SDCMD_FN_DEF(UC, LC, PYCBC_VAL_GEN, __VA_ARGS__)
#    define PYCBC_SDCMD_CASE_MVAL(UC, LC, ...) \
        PYCBC_SDCMD_FN_DEF(UC, LC, PYCBC_VAL_GEN, __VA_ARGS__)
#    define PYCBC_SDCMD_CASE_COUNTER(UC, LC, ...) \
        PYCBC_SDCMD_FN_DEF(UC, LC, PYCBC_COUNTER, __VA_ARGS__)

#    define PYCBC_SD_OPS_GEN
#    ifdef PYCBC_SD_OPS_GEN
PYCBC_X_SD_OPS(PYCBC_SDCMD_CASE,
               PYCBC_SDCMD_CASE_NP,
               PYCBC_SDCMD_CASE_VAL,
               PYCBC_SDCMD_CASE_MVAL,
               PYCBC_SDCMD_CASE_COUNTER,
               LITERAL_DECL, DUMMY)
#    else

#    endif

LIBCOUCHBASE_API lcb_STATUS lcb_subdocops_create(lcb_SUBDOCOPS **operations,
                                                 size_t capacity);
LIBCOUCHBASE_API lcb_STATUS
lcb_cmdsubdoc_operations(lcb_CMDSUBDOC *cmd, const lcb_SUBDOCOPS *operations);

LIBCOUCHBASE_API lcb_STATUS lcb_subdocops_destroy(lcb_SUBDOCOPS *operations);

#    define lcb_subdoc lcb_subdoc3

#endif

typedef enum {
    PYCBC_NIL,
    PYCBC_PATH_ONLY,
    PYCBC_COUNTER,
    PYCBC_STR
} pycbc_sd_op_category;

typedef struct {
    int is_multival;
    pycbc_sd_op_category category;
    int has_valbuf;
    lcb_STATUS err;
} pycbc_sd_metainfo;

typedef struct {
    lcb_STATUS err;
} pycbc_sdspec;

typedef struct {
    lcb_SUBDOCOP op;
    size_t index;
    unsigned flags;
    pycbc_pybuffer *pathbuf;
    pycbc_pybuffer *valbuf;
    int64_t delta;
} pycbc_sdspec_details;

pycbc_sd_metainfo pycbc_get_metainfo(pycbc_sdspec_details details);
pycbc_sdspec pycbc_build_spec(lcb_SUBDOCOPS *subdocops,
                                     pycbc_sdspec_details details);

#endif // COUCHBASE_PYTHON_CLIENT_PYCBC_SUBDOCOPS_H
