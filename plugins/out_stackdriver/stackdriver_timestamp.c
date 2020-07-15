/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2019-2020 The Fluent Bit Authors
 *  Copyright (C) 2015-2018 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include "stackdriver.h"
#include "stackdriver_helper.h"
#include "stackdriver_timestamp.h"

static int is_integer(char *str, int size) {
    for (int i = 0; i < size; ++ i) {
        if (!isdigit(str[i])) {
            return FLB_FALSE;
        }
    }
    return FLB_TRUE;
}

static void try_assign_time(long long seconds, long long nanos, 
                        struct flb_time *tms)
{
    if (seconds != 0 || nanos != 0) {
        tms->tm.tv_sec = seconds;
        tms->tm.tv_nsec = nanos;    
    }
}

static int extract_format_timestamp(msgpack_object *obj,
                                    struct flb_time *tms)
{
    int format_timestamp_seconds_found = FLB_FALSE;
    int format_timestamp_nanos_found = FLB_FALSE;
    long long seconds = 0;
    long long nanos = 0;

    msgpack_object_kv *p;
    msgpack_object_kv *pend;
    msgpack_object_kv *tmp_p;
    msgpack_object_kv *tmp_pend;

    if (obj->via.map.size == 0) {    	
        return FLB_FALSE;
    }
    p = obj->via.map.ptr;
    pend = obj->via.map.ptr + obj->via.map.size;

    for (; p < pend; ++p) {
        if (!validate_key(p->key, "timestamp", 9) 
            || p->val.type != MSGPACK_OBJECT_MAP) {
            continue;
        }

        tmp_p = p->val.via.map.ptr;
        tmp_pend = p->val.via.map.ptr + p->val.via.map.size;

        for (; tmp_p < tmp_pend; ++tmp_p) {
            if (validate_key(tmp_p->key, "seconds", 7)) {
                format_timestamp_seconds_found = FLB_TRUE;
                if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    seconds = tmp_p->val.via.i64;
                }
                else if (tmp_p->val.type == MSGPACK_OBJECT_STR 
                         && is_integer(tmp_p->val.via.str.ptr, 
                                       tmp_p->val.via.str.size)) {
                    seconds = atoll(tmp_p->val.via.str.ptr);
                }
                else {
                    seconds = 0;
                }
                
                if (format_timestamp_nanos_found == FLB_TRUE) {
                    try_assign_time(seconds, nanos, tms);
                    return FLB_TRUE;
                }
            }
            else if (validate_key(tmp_p->key, "nanos", 5)) {
                format_timestamp_nanos_found = FLB_TRUE;

                if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    nanos = tmp_p->val.via.i64;
                }
                else if (tmp_p->val.type == MSGPACK_OBJECT_STR 
                         && is_integer(tmp_p->val.via.str.ptr, 
                                       tmp_p->val.via.str.size)) {
                    nanos = atoll(tmp_p->val.via.str.ptr);
                }
                else {
                    nanos = 0;
                }

                if (format_timestamp_seconds_found == FLB_TRUE) {
                    try_assign_time(seconds, nanos, tms);
                    return FLB_TRUE;
                }
            }
        }
    }
    return FLB_FALSE;
}

timestamp_status extract_timestamp(msgpack_object *obj,
                                   struct flb_time *tms,
                                   flb_sds_t time)
{
    if (extract_format_timestamp(obj, tms)) {
        return FORMAT_TIMESTAMP;
    }
    return TIMESTAMP_NOT_PRESENT;
}
