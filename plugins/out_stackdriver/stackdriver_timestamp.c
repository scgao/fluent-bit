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
#include <fluent-bit/flb_regex.h>

#define format_time_pattern "^([1-9]\\d{3}-((0[1-9]|1[0-2])-"  \
                            "(0[1-9]|1\\d|2[0-8])|(0[13-9]|1[0-2])-"   \
                            "(29|30)|(0[13578]|1[02])-31))T"   \
                            "([01]\\d|2[0-3])\:[0-5]\\d\:[0-5]\\d" \
                            "(.\\d+)?(Z|[+-][01]\\d\:[0-5]\\d)$"

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
    if (seconds != 0) {
        tms->tm.tv_sec = seconds;
        tms->tm.tv_nsec = nanos;    
    }
}

static long long get_integer(msgpack_object obj)
{
    if (obj.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        return obj.via.i64;
    }
    else if (obj.type == MSGPACK_OBJECT_STR 
             && is_integer(obj.via.str.ptr, 
                           obj.via.str.size)) {
        return atoll(obj.via.str.ptr);
    }
    return 0;
}

static int extract_format_timestamp_object(msgpack_object *obj,
                                           struct flb_time *tms)
{
    int seconds_found = FLB_FALSE;
    int nanos_found = FLB_FALSE;
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
                seconds_found = FLB_TRUE;
                seconds = get_integer(tmp_p->val);
                
                if (nanos_found == FLB_TRUE) {
                    try_assign_time(seconds, nanos, tms);
                    return FLB_TRUE;
                }
            }
            else if (validate_key(tmp_p->key, "nanos", 5)) {
                nanos_found = FLB_TRUE;
                nanos = get_integer(tmp_p->val);

                if (seconds_found == FLB_TRUE) {
                    try_assign_time(seconds, nanos, tms);
                    return FLB_TRUE;
                }
            }
        }
    }
    return FLB_FALSE;
}

static int extract_format_timestamp_duo_fields(msgpack_object *obj,
                                               struct flb_time *tms)
{
    int seconds_found = FLB_FALSE;
    int nanos_found = FLB_FALSE;
    long long seconds = 0;
    long long nanos = 0;

    msgpack_object_kv *p;
    msgpack_object_kv *pend;

    if (obj->via.map.size == 0) {    	
        return FLB_FALSE;
    }
    p = obj->via.map.ptr;
    pend = obj->via.map.ptr + obj->via.map.size;

    for (; p < pend; ++p) {
        if (validate_key(p->key, "timestampSeconds", 16)) {
            seconds_found = FLB_TRUE;
            seconds = get_integer(p->val);

            if (nanos_found == FLB_TRUE) {
                try_assign_time(seconds, nanos, tms);
                return FLB_TRUE;
            }
        }
        else if (validate_key(p->key, "timestampNanos", 14)) {
            nanos_found = FLB_TRUE;
            nanos = get_integer(p->val);

            if (seconds_found == FLB_TRUE) {
                try_assign_time(seconds, nanos, tms);
                return FLB_TRUE;
            }
        } 
    }

    return FLB_FALSE;
}

static int validate_format_time(char *str, int size)
{   
    struct flb_regex *regex;
    int status = 0;

    regex = flb_regex_create(format_time_pattern);
    status = flb_regex_match(regex, str, size);
    flb_regex_destroy(regex);

    if (status == 1) {
        return FLB_TRUE;
    }
    return FLB_FALSE;
}

static timestamp_status extract_format_time(msgpack_object *obj,
                                            flb_sds_t time)
{
    msgpack_object_kv *p;
    msgpack_object_kv *pend;

    if (obj->via.map.size == 0) {    	
        return TIMESTAMP_NOT_PRESENT;
    }
    p = obj->via.map.ptr;
    pend = obj->via.map.ptr + obj->via.map.size;

    for (; p < pend; ++p) {
        if (validate_key(p->key, "time", 4)) {
            if (p->val.type == MSGPACK_OBJECT_STR
                && validate_format_time(p->val.via.str.ptr, 
                                        p->val.via.str.size)) {
                
                time = flb_sds_copy(time, p->val.via.str.ptr, 
                                    p->val.via.str.size);
                return FORMAT_TIME;
            }
            return INVALID_FORMAT_TIME;
        }
    }
    
    return TIMESTAMP_NOT_PRESENT;
}

timestamp_status extract_timestamp(msgpack_object *obj,
                                   struct flb_time *tms,
                                   flb_sds_t time)
{
    if (extract_format_timestamp_object(obj, tms)) {
        return FORMAT_TIMESTAMP_OBJECT;
    }
    if (extract_format_timestamp_duo_fields(obj, tms)) {
        return FORMAT_TIMESTAMP_DUO_FIELDS;
    }
    return extract_format_time(obj, time);
}
