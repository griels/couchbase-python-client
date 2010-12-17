/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2010 Membase, Inc.
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
#include "internal.h"

/**
 * Associate a "cookie" with an instance of libmembase. You may only store
 * <b>one</b> cookie with each instance of libmembase.
 *
 * @param instance the instance to associate the cookie with
 * @param cookie the cookie to associate with this instance.
 *
 * @author Trond Norbye
 */
LIBMEMBASE_API
void libmembase_set_cookie(libmembase_t instance, const void *cookie)
{
    instance->cookie = cookie;
}

/**
 * Get the cookie associated with a given instance of libmembase.
 *
 * @param instance the instance to query
 * @return The cookie associated with this instance.
 *
 * @author Trond Norbye
 */
LIBMEMBASE_API
const void *libmembase_get_cookie(libmembase_t instance)
{
   return instance->cookie;
}
