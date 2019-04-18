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

#include "pycbc_subdocops.h"
pycbc_sd_metainfo pycbc_get_metainfo(pycbc_sdspec_details details)
{
    pycbc_sd_metainfo result = {0};
#define PYCBC_METAINFO_PATH_ONLY(UC, LC, ...) \
    result = (pycbc_sd_metainfo){.is_multival = 0, .category = PYCBC_PATH_ONLY};
#define PYCBC_METAINFO_COUNTER(UC, LC, ...) \
    result = (pycbc_sd_metainfo){.is_multival = 0, .category = PYCBC_COUNTER};
#define PYCBC_METAINFO_NP(UC, LC, ...) \
    result = (pycbc_sd_metainfo){.is_multival = 0, .category = PYCBC_NIL};
#define PYCBC_METAINFO_VAL_GEN(UC, LC, ...) \
    result = (pycbc_sd_metainfo){.is_multival = 0, .category = PYCBC_STR};
#define PYCBC_METAINFO_IS_MVAL(UC, LC, ...) \
    result = (pycbc_sd_metainfo){.is_multival = 1, .category = PYCBC_STR};

#define PYCBC_METAINFO_SDCMD_CASE(UC, LC, ...) \
    PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_METAINFO_PATH_ONLY, __VA_ARGS__)
#define PYCBC_METAINFO_SDCMD_CASE_NP(UC, LC, ...) \
    PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_METAINFO_NP, __VA_ARGS__)
#define PYCBC_METAINFO_SDCMD_CASE_VAL(UC, LC, ...) \
    PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_METAINFO_VAL_GEN, __VA_ARGS__)
#define PYCBC_METAINFO_SDCMD_CASE_MVAL(UC, LC, ...) \
    PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_METAINFO_IS_MVAL, __VA_ARGS__)
#define PYCBC_METAINFO_SDCMD_CASE_COUNTER(UC, LC, ...) \
    PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_METAINFO_COUNTER, __VA_ARGS__)
    switch (details.op) {
        PYCBC_X_SD_OPS(PYCBC_METAINFO_SDCMD_CASE,
                       PYCBC_METAINFO_SDCMD_CASE_NP,
                       PYCBC_METAINFO_SDCMD_CASE_VAL,
                       PYCBC_METAINFO_SDCMD_CASE_MVAL,
                       PYCBC_METAINFO_SDCMD_CASE_COUNTER)
    default:
        result.err = LCB_SUBDOC_PATH_EINVAL;
        break;
    }
    result.has_valbuf = (result.category == PYCBC_STR);
    return result;
#undef PYCBC_METAINFO_PATH_ONLY
#undef PYCBC_METAINFO_COUNTER
#undef PYCBC_METAINFO_NP
#undef PYCBC_VAL_GEN
#undef PYCBC_METAINFO_IS_MVAL

#undef PYCBC_METAINFO_SDCMD_CASE
#undef PYCBC_METAINFO_SDCMD_CASE_NP
#undef PYCBC_METAINFO_SDCMD_CASE_VAL
#undef PYCBC_METAINFO_SDCMD_CASE_MVAL
#undef PYCBC_METAINFO_SDCMD_CASE_COUNTER
};

pycbc_sdspec pycbc_build_spec(lcb_SUBDOCOPS *subdocops,
                              pycbc_sdspec_details details)
{
    pycbc_sdspec result = {0};

#    define PYCBC_BUILDSPEC_PATH_ONLY(UC, LC, ...)  \
        lcb_subdocops_##LC(subdocops,               \
                           details.index,           \
                           details.flags,           \
                           details.pathbuf->buffer, \
                           details.pathbuf->length);
#    define PYCBC_BUILDSPEC_COUNTER(UC, LC, ...)                       \
        lcb_subdocops_##LC(                                            \
                subdocops,                                             \
                details.index,                                         \
                details.flags,                                         \
                details.pathbuf->buffer,                               \
                details.pathbuf->length,                               \
                (details.valbuf && details.valbuf->buffer)             \
                        ? strtol((const char *)details.valbuf->buffer, \
                                 NULL,                                 \
                                 10)                                   \
                        : 0);
#    define PYCBC_BUILDSPEC_NP(UC, LC, ...) \
        lcb_subdocops_##LC(subdocops, details.index, details.flags);
#    define PYCBC_BUILDSPEC_VAL_GEN(UC, LC, ...)    \
        lcb_subdocops_##LC(subdocops,               \
                           details.index,           \
                           details.flags,           \
                           details.pathbuf->buffer, \
                           details.pathbuf->length, \
                           details.valbuf->buffer,  \
                           details.valbuf->length);

#    define PYCBC_BUILDSPEC_SDCMD_CASE(UC, LC, ...) \
        PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_BUILDSPEC_PATH_ONLY, __VA_ARGS__)
#    define PYCBC_BUILDSPEC_SDCMD_CASE_NP(UC, LC, ...) \
        PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_BUILDSPEC_NP, __VA_ARGS__)
#    define PYCBC_BUILDSPEC_SDCMD_CASE_VAL(UC, LC, ...) \
        PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_BUILDSPEC_VAL_GEN, __VA_ARGS__)
#    define PYCBC_BUILDSPEC_SDCMD_CASE_MVAL(UC, LC, ...) \
        PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_BUILDSPEC_VAL_GEN, __VA_ARGS__)
#    define PYCBC_BUILDSPEC_SDCMD_CASE_COUNTER(UC, LC, ...) \
        PYCBC_SDCMD_CASE_GENERIC(UC, LC, PYCBC_BUILDSPEC_COUNTER, __VA_ARGS__)
    switch (details.op) {
        PYCBC_X_SD_OPS(PYCBC_BUILDSPEC_SDCMD_CASE,
                       PYCBC_BUILDSPEC_SDCMD_CASE_NP,
                       PYCBC_BUILDSPEC_SDCMD_CASE_VAL,
                       PYCBC_BUILDSPEC_SDCMD_CASE_MVAL,
                       PYCBC_BUILDSPEC_SDCMD_CASE_COUNTER)
    default:
        result.err = LCB_SUBDOC_PATH_EINVAL;
        break;
    }
    return result;
}

