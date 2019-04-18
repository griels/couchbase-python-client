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
//#include "oputil.h"
#include "python_wrappers.h"
#if PYCBC_LCB_API>0x030000
#include "lcb_v4_wrapper.h"
#else
#include "lcb_v4_backport.h"
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
