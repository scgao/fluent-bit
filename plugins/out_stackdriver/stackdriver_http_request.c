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
#include <fluent-bit/flb_regex.h>
#include "stackdriver.h"
#include "stackdriver_http_request.h"

typedef enum {
    NO_HTTPREQUEST = 1,
    HTTPREQUEST_EXISTED = 2
} http_request_status;

void init_http_request(struct http_request_field *http_request)
{
    http_request->requestMethod = flb_sds_create("");
    http_request->requestUrl = flb_sds_create("");
    http_request->userAgent = flb_sds_create("");
    http_request->remoteIp = flb_sds_create("");
    http_request->serverIp = flb_sds_create("");
    http_request->referer = flb_sds_create("");
    http_request->latency = flb_sds_create("");
    http_request->protocol = flb_sds_create("");

    http_request->requestSize = 0;
    http_request->responseSize = 0;
    http_request->status = 0;
    http_request->cacheFillBytes = 0;

    http_request->cacheLookup = FLB_FALSE;
    http_request->cacheHit = FLB_FALSE;
    http_request->cacheValidatedWithOriginServer = FLB_FALSE;
}

void destroy_http_request(struct http_request_field *http_request)
{
    flb_sds_destroy(http_request->requestMethod);
    flb_sds_destroy(http_request->requestUrl);
    flb_sds_destroy(http_request->userAgent);
    flb_sds_destroy(http_request->remoteIp);
    flb_sds_destroy(http_request->serverIp);
    flb_sds_destroy(http_request->referer);
    flb_sds_destroy(http_request->latency);
    flb_sds_destroy(http_request->protocol);
}

void add_http_request_field(struct http_request_field *http_request, 
                            msgpack_packer *mp_pck)
{    
    msgpack_pack_str(mp_pck, 11);
    msgpack_pack_str_body(mp_pck, "httpRequest", 11);

    if (flb_sds_is_empty(http_request->latency) == FLB_TRUE) {
        msgpack_pack_map(mp_pck, 14);
    }
    else {
        msgpack_pack_map(mp_pck, 15);

        msgpack_pack_str(mp_pck, 7);
        msgpack_pack_str_body(mp_pck, "latency", 7);
        msgpack_pack_str(mp_pck, flb_sds_len(http_request->latency));
        msgpack_pack_str_body(mp_pck, http_request->latency, 
                              flb_sds_len(http_request->latency));
    }

    /* String sub-fields */
    msgpack_pack_str(mp_pck, 13);
    msgpack_pack_str_body(mp_pck, "requestMethod", 13);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->requestMethod));
    msgpack_pack_str_body(mp_pck, http_request->requestMethod, 
                          flb_sds_len(http_request->requestMethod));

    msgpack_pack_str(mp_pck, 10);
    msgpack_pack_str_body(mp_pck, "requestUrl", 10);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->requestUrl));
    msgpack_pack_str_body(mp_pck, http_request->requestUrl, 
                          flb_sds_len(http_request->requestUrl));

    msgpack_pack_str(mp_pck, 9);
    msgpack_pack_str_body(mp_pck, "userAgent", 9);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->userAgent));
    msgpack_pack_str_body(mp_pck, http_request->userAgent, 
                          flb_sds_len(http_request->userAgent));

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck, "remoteIp", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->remoteIp));
    msgpack_pack_str_body(mp_pck, http_request->remoteIp, 
                          flb_sds_len(http_request->remoteIp));

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck, "serverIp", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->serverIp));
    msgpack_pack_str_body(mp_pck, http_request->serverIp, 
                          flb_sds_len(http_request->serverIp));

     msgpack_pack_str(mp_pck, 7);
    msgpack_pack_str_body(mp_pck, "referer", 7);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->referer));
    msgpack_pack_str_body(mp_pck, http_request->referer, 
                          flb_sds_len(http_request->referer));

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck, "protocol", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->protocol));
    msgpack_pack_str_body(mp_pck, http_request->protocol, 
                          flb_sds_len(http_request->protocol));

    /* Integer sub-fields */
    msgpack_pack_str(mp_pck, 11);
    msgpack_pack_str_body(mp_pck, "requestSize", 11);
    msgpack_pack_int64(mp_pck, http_request->requestSize);

    msgpack_pack_str(mp_pck, 12);
    msgpack_pack_str_body(mp_pck, "responseSize", 12);
    msgpack_pack_int64(mp_pck, http_request->responseSize);

    msgpack_pack_str(mp_pck, 6);
    msgpack_pack_str_body(mp_pck, "status", 6);
    msgpack_pack_int64(mp_pck, http_request->status);

    msgpack_pack_str(mp_pck, 14);
    msgpack_pack_str_body(mp_pck, "cacheFillBytes", 14);
    msgpack_pack_int64(mp_pck, http_request->cacheFillBytes);

    /* Boolean sub-fields */
    msgpack_pack_str(mp_pck, 11);
    msgpack_pack_str_body(mp_pck, "cacheLookup", 11);
    if (http_request->cacheLookup == FLB_TRUE) {
        msgpack_pack_true(mp_pck);
    }
    else {
        msgpack_pack_false(mp_pck);
    }

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck, "cacheHit", 8);
    if (http_request->cacheLookup == FLB_TRUE) {
        msgpack_pack_true(mp_pck);
    }
    else {
        msgpack_pack_false(mp_pck);
    }

    msgpack_pack_str(mp_pck, 30);
    msgpack_pack_str_body(mp_pck, "cacheValidatedWithOriginServer", 30);
    if (http_request->cacheValidatedWithOriginServer == FLB_TRUE) {
        msgpack_pack_true(mp_pck);
    }
    else {
        msgpack_pack_false(mp_pck);
    }
}

/* latency should be in the format: 
 *      whitespace (opt.) + integer + point & decimal (opt.)
 *      + whitespace (opt.) + "s" + whitespace (opt.) 
 */
static void validate_latency(msgpack_object_str latency_in_payload, 
                             struct http_request_field *http_request) {
    flb_sds_t pattern = flb_sds_create("^\\s*\\d\\d*(.\\d\\d*)?\\s*s\\s*$");
    char extract_latency[latency_in_payload.size];
    struct flb_regex *regex;

    int status = 0;
    int i = 0, j = 0;

    regex = flb_regex_create(pattern);
    status = flb_regex_match(regex, latency_in_payload.ptr, latency_in_payload.size);
    flb_regex_destroy(regex);
    flb_sds_destroy(pattern);

    if (status == 1) {
        for (; i < latency_in_payload.size; ++ i) {
            if (latency_in_payload.ptr[i] == '.' || latency_in_payload.ptr[i] == 's' 
                || isdigit(latency_in_payload.ptr[i])) {
                extract_latency[j] = latency_in_payload.ptr[i];
                ++ j;
            }
        }
        http_request->latency = flb_sds_copy(http_request->latency, extract_latency, j);
    }
}
                                                                                        
/* Return true if httpRequest extracted */
int extract_http_request(struct http_request_field *http_request, 
                         msgpack_object *obj, int *extra_subfields)
{
    http_request_status op_status = NO_HTTPREQUEST;
    flb_sds_t key_name;
    flb_sds_t sub_key_name;

    if (obj->via.map.size == 0) {
        return FLB_FALSE;
    }
                                                                                        
    msgpack_object_kv *p = obj->via.map.ptr;
    msgpack_object_kv *const pend = obj->via.map.ptr + obj->via.map.size;
                                                                                        
    for (; p < pend && op_status == NO_HTTPREQUEST; ++p) {
        key_name = flb_sds_create_len(p->key.via.str.ptr, p->key.via.str.size);

        if (p->val.type != MSGPACK_OBJECT_MAP
            || p->key.type != MSGPACK_OBJECT_STR
            || strcmp(HTTPREQUEST_FIELD_IN_JSON, key_name) != 0) {
            
            flb_sds_destroy(key_name);
            continue;
        }
        flb_sds_destroy(key_name);

        op_status = HTTPREQUEST_EXISTED;
        msgpack_object sub_field = p->val;
        
        msgpack_object_kv *tmp_p = sub_field.via.map.ptr;
        msgpack_object_kv *const tmp_pend = sub_field.via.map.ptr + sub_field.via.map.size;
                                                                                        
        /* Validate the subfields of httpRequest */
        for (; tmp_p < tmp_pend; ++tmp_p) {
            if (tmp_p->key.type != MSGPACK_OBJECT_STR) {
                continue;
            }
            sub_key_name = flb_sds_create_len(tmp_p->key.via.str.ptr, tmp_p->key.via.str.size);

            if (strcmp("requestMethod", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);

                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                http_request->requestMethod = flb_sds_copy(http_request->requestMethod, 
                                                           tmp_p->val.via.str.ptr, 
                                                           tmp_p->val.via.str.size);
            }
            else if (strcmp("requestUrl", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);

                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                http_request->requestUrl = flb_sds_copy(http_request->requestUrl, 
                                                        tmp_p->val.via.str.ptr, 
                                                        tmp_p->val.via.str.size);
            }
            else if (strcmp("userAgent", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                http_request->userAgent = flb_sds_copy(http_request->userAgent, 
                                                       tmp_p->val.via.str.ptr, 
                                                       tmp_p->val.via.str.size);
            }
            else if (strcmp("remoteIp", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                http_request->remoteIp = flb_sds_copy(http_request->remoteIp, 
                                                      tmp_p->val.via.str.ptr, 
                                                      tmp_p->val.via.str.size);
            }
            else if (strcmp("serverIp", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                http_request->serverIp = flb_sds_copy(http_request->serverIp, 
                                                      tmp_p->val.via.str.ptr, 
                                                      tmp_p->val.via.str.size);
            }
            else if (strcmp("referer", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                http_request->referer = flb_sds_copy(http_request->referer, 
                                                     tmp_p->val.via.str.ptr, 
                                                     tmp_p->val.via.str.size);
            }
            else if (strcmp("protocol", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                http_request->protocol = flb_sds_copy(http_request->protocol, 
                                                      tmp_p->val.via.str.ptr, 
                                                      tmp_p->val.via.str.size);
            }
            else if (strcmp("latency", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                validate_latency(tmp_p->val.via.str, http_request);
            }

            else if (strcmp("requestSize", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                    http_request->requestSize = atoll(tmp_p->val.via.str.ptr);
                }
                else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    http_request->requestSize = tmp_p->val.via.i64;
                }
            }
            else if (strcmp("responseSize", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                    http_request->responseSize = atoll(tmp_p->val.via.str.ptr);
                }
                else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    http_request->responseSize = tmp_p->val.via.i64;
                }
            }
            else if (strcmp("status", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                    http_request->status = atoll(tmp_p->val.via.str.ptr);
                }
                else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    http_request->status = tmp_p->val.via.i64;
                }
            }
            else if (strcmp("cacheFillBytes", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                    http_request->cacheFillBytes = atoll(tmp_p->val.via.str.ptr);
                }
                else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    http_request->cacheFillBytes = tmp_p->val.via.i64;
                }
            }

            else if (strcmp("cacheLookup", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_BOOLEAN) {
                    continue;
                }
                if (tmp_p->val.via.boolean) {
                    http_request->cacheLookup = FLB_TRUE;
                }
                else {
                    http_request->cacheLookup = FLB_FALSE;
                }
            }
            else if (strcmp("cacheHit", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_BOOLEAN) {
                    continue;
                }
                if (tmp_p->val.via.boolean) {
                    http_request->cacheHit = FLB_TRUE;
                }
                else {
                    http_request->cacheHit = FLB_FALSE;
                }
            }
            else if (strcmp("cacheValidatedWithOriginServer", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);
                
                if (tmp_p->val.type != MSGPACK_OBJECT_BOOLEAN) {
                    continue;
                }
                if (tmp_p->val.via.boolean) {
                    http_request->cacheValidatedWithOriginServer = FLB_TRUE;
                }
                else {
                    http_request->cacheValidatedWithOriginServer = FLB_FALSE;
                }
            }

            else {
                *extra_subfields += 1;
                flb_sds_destroy(sub_key_name);
            }
        }
    }
    
    return op_status == HTTPREQUEST_EXISTED;
}
                                                                                        
void pack_extra_http_request_subfields(msgpack_packer *mp_pck, 
                                       msgpack_object *http_request, 
                                       int extra_subfields) {
    msgpack_object_kv *p = http_request->via.map.ptr;
    msgpack_object_kv *const pend = http_request->via.map.ptr + http_request->via.map.size;
    flb_sds_t key_name;

    msgpack_pack_map(mp_pck, extra_subfields);

    for (; p < pend; ++p) {
        key_name = flb_sds_create_len(p->key.via.str.ptr, p->key.via.str.size);

        if (strcmp("requestMethod", key_name) != 0 
            && strcmp("requestUrl", key_name) != 0
            && strcmp("userAgent", key_name) != 0
            && strcmp("remoteIp", key_name) != 0
            && strcmp("serverIp", key_name) != 0
            && strcmp("referer", key_name) != 0
            && strcmp("latency", key_name) != 0
            && strcmp("protocol", key_name) != 0
            && strcmp("requestSize", key_name) != 0
            && strcmp("responseSize", key_name) != 0
            && strcmp("status", key_name) != 0
            && strcmp("cacheFillBytes", key_name) != 0
            && strcmp("cacheLookup", key_name) != 0
            && strcmp("cacheHit", key_name) != 0
            && strcmp("cacheValidatedWithOriginServer", key_name) != 0) {
                
            msgpack_pack_object(mp_pck, p->key);
            msgpack_pack_object(mp_pck, p->val);
        }

        flb_sds_destroy(key_name);
    }
}
