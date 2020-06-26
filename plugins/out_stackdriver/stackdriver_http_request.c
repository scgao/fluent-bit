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
#include "stackdriver_helper.h"
#include "stackdriver_http_request.h"

typedef enum {
    NO_HTTPREQUEST = 1,
    HTTPREQUEST_EXISTS = 2
} http_request_status;

void init_http_request(struct http_request_field *http_request)
{
    http_request->latency = flb_sds_create("");
    http_request->protocol = flb_sds_create("");
    http_request->referer = flb_sds_create("");
    http_request->remoteIp = flb_sds_create("");
    http_request->requestMethod = flb_sds_create("");
    http_request->requestUrl = flb_sds_create("");
    http_request->serverIp = flb_sds_create("");
    http_request->userAgent = flb_sds_create("");

    http_request->cacheFillBytes = 0;
    http_request->requestSize = 0;
    http_request->responseSize = 0;
    http_request->status = 0;

    http_request->cacheHit = FLB_FALSE;
    http_request->cacheLookup = FLB_FALSE;
    http_request->cacheValidatedWithOriginServer = FLB_FALSE;
}

void destroy_http_request(struct http_request_field *http_request)
{
    flb_sds_destroy(http_request->latency);
    flb_sds_destroy(http_request->protocol);
    flb_sds_destroy(http_request->referer);
    flb_sds_destroy(http_request->remoteIp);
    flb_sds_destroy(http_request->requestMethod);
    flb_sds_destroy(http_request->requestUrl);
    flb_sds_destroy(http_request->serverIp);
    flb_sds_destroy(http_request->userAgent);
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

        msgpack_pack_str(mp_pck, HTTP_REQUEST_LATENCY_SIZE);
        msgpack_pack_str_body(mp_pck, HTTP_REQUEST_LATENCY, 
                              HTTP_REQUEST_LATENCY_SIZE);
        msgpack_pack_str(mp_pck, flb_sds_len(http_request->latency));
        msgpack_pack_str_body(mp_pck, http_request->latency, 
                              flb_sds_len(http_request->latency));
    }

    /* String sub-fields */
    msgpack_pack_str(mp_pck, HTTP_REQUEST_REQUEST_METHOD_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_REQUEST_METHOD, 
                          HTTP_REQUEST_REQUEST_METHOD_SIZE);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->requestMethod));
    msgpack_pack_str_body(mp_pck, http_request->requestMethod, 
                          flb_sds_len(http_request->requestMethod));

    msgpack_pack_str(mp_pck, HTTP_REQUEST_REQUEST_URL_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_REQUEST_URL, 
                          HTTP_REQUEST_REQUEST_URL_SIZE);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->requestUrl));
    msgpack_pack_str_body(mp_pck, http_request->requestUrl, 
                          flb_sds_len(http_request->requestUrl));

    msgpack_pack_str(mp_pck, HTTP_REQUEST_USER_AGENT_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_USER_AGENT, 
                          HTTP_REQUEST_USER_AGENT_SIZE);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->userAgent));
    msgpack_pack_str_body(mp_pck, http_request->userAgent, 
                          flb_sds_len(http_request->userAgent));

    msgpack_pack_str(mp_pck, HTTP_REQUEST_REMOTE_IP_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_REMOTE_IP, 
                          HTTP_REQUEST_REMOTE_IP_SIZE);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->remoteIp));
    msgpack_pack_str_body(mp_pck, http_request->remoteIp, 
                          flb_sds_len(http_request->remoteIp));

    msgpack_pack_str(mp_pck, HTTP_REQUEST_SERVER_IP_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_SERVER_IP, 
                          HTTP_REQUEST_SERVER_IP_SIZE);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->serverIp));
    msgpack_pack_str_body(mp_pck, http_request->serverIp, 
                          flb_sds_len(http_request->serverIp));

    msgpack_pack_str(mp_pck, HTTP_REQUEST_REFERER_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_REFERER, 
                          HTTP_REQUEST_REFERER_SIZE);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->referer));
    msgpack_pack_str_body(mp_pck, http_request->referer, 
                          flb_sds_len(http_request->referer));

    msgpack_pack_str(mp_pck, HTTP_REQUEST_PROTOCOL_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_PROTOCOL, 
                          HTTP_REQUEST_PROTOCOL_SIZE);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->protocol));
    msgpack_pack_str_body(mp_pck, http_request->protocol, 
                          flb_sds_len(http_request->protocol));

    /* Integer sub-fields */
    msgpack_pack_str(mp_pck, HTTP_REQUEST_REQUESTSIZE_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_REQUESTSIZE, 
                          HTTP_REQUEST_REQUESTSIZE_SIZE);
    msgpack_pack_int64(mp_pck, http_request->requestSize);

    msgpack_pack_str(mp_pck, HTTP_REQUEST_RESPONSESIZE_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_RESPONSESIZE, 
                          HTTP_REQUEST_RESPONSESIZE_SIZE);
    msgpack_pack_int64(mp_pck, http_request->responseSize);

    msgpack_pack_str(mp_pck, HTTP_REQUEST_STATUS_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_STATUS, HTTP_REQUEST_STATUS_SIZE);
    msgpack_pack_int64(mp_pck, http_request->status);

    msgpack_pack_str(mp_pck, HTTP_REQUEST_CACHE_FILL_BYTES_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_CACHE_FILL_BYTES, 
                          HTTP_REQUEST_CACHE_FILL_BYTES_SIZE);
    msgpack_pack_int64(mp_pck, http_request->cacheFillBytes);

    /* Boolean sub-fields */
    msgpack_pack_str(mp_pck, HTTP_REQUEST_CACHE_LOOKUP_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_CACHE_LOOKUP, 
                          HTTP_REQUEST_CACHE_LOOKUP_SIZE);
    if (http_request->cacheLookup == FLB_TRUE) {
        msgpack_pack_true(mp_pck);
    }
    else {
        msgpack_pack_false(mp_pck);
    }
                                                                                    
    msgpack_pack_str(mp_pck, HTTP_REQUEST_CACHE_HIT_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_CACHE_HIT, 
                          HTTP_REQUEST_CACHE_HIT_SIZE);
    if (http_request->cacheLookup == FLB_TRUE) {
        msgpack_pack_true(mp_pck);
    }
    else {
        msgpack_pack_false(mp_pck);
    }

    msgpack_pack_str(mp_pck, HTTP_REQUEST_CACHE_VALIDATE_WITH_ORIGIN_SERVER_SIZE);
    msgpack_pack_str_body(mp_pck, HTTP_REQUEST_CACHE_VALIDATE_WITH_ORIGIN_SERVER, 
                          HTTP_REQUEST_CACHE_VALIDATE_WITH_ORIGIN_SERVER_SIZE);
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
    flb_sds_t pattern = flb_sds_create("^\\s*\\d+(.\\d+)?\\s*s\\s*$");
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
    msgpack_object_kv *p;
    msgpack_object_kv *pend;
    msgpack_object_kv *tmp_p;
    msgpack_object_kv *tmp_pend;

    if (obj->via.map.size == 0) {
        return FLB_FALSE;
    }
                                                                                        
    p = obj->via.map.ptr;
    pend = obj->via.map.ptr + obj->via.map.size;
                                                                                        
    for (; p < pend && op_status == NO_HTTPREQUEST; ++p) {
        if (p->val.type != MSGPACK_OBJECT_MAP
            || p->key.type != MSGPACK_OBJECT_STR
            || !cmp_str_helper(p->key, HTTPREQUEST_FIELD_IN_JSON, 
                           sizeof(HTTPREQUEST_FIELD_IN_JSON) - 1)) {
            
            continue;
        }

        op_status = HTTPREQUEST_EXISTS;
        msgpack_object sub_field = p->val;
        
        tmp_p = sub_field.via.map.ptr;
        tmp_pend = sub_field.via.map.ptr + sub_field.via.map.size;
                                                                                        
        /* Validate the subfields of httpRequest */
        for (; tmp_p < tmp_pend; ++tmp_p) {
            if (tmp_p->key.type != MSGPACK_OBJECT_STR) {
                continue;
            }

            if (cmp_str_helper(tmp_p->key, HTTP_REQUEST_LATENCY, 
                           HTTP_REQUEST_LATENCY_SIZE)) {                
                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                validate_latency(tmp_p->val.via.str, http_request);
            }
            else if (assign_subfield_str(tmp_p, HTTP_REQUEST_PROTOCOL, 
                                         HTTP_REQUEST_PROTOCOL_SIZE, 
                                         &http_request->protocol) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_str(tmp_p, HTTP_REQUEST_REFERER, 
                                         HTTP_REQUEST_REFERER_SIZE, 
                                         &http_request->referer) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_str(tmp_p, HTTP_REQUEST_REMOTE_IP, 
                                         HTTP_REQUEST_REMOTE_IP_SIZE, 
                                         &http_request->remoteIp) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_str(tmp_p, HTTP_REQUEST_REQUEST_METHOD, 
                                         HTTP_REQUEST_REQUEST_METHOD_SIZE, 
                                         &http_request->requestMethod) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_str(tmp_p, HTTP_REQUEST_REQUEST_URL, 
                                         HTTP_REQUEST_REQUEST_URL_SIZE, 
                                         &http_request->requestUrl) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_str(tmp_p, HTTP_REQUEST_SERVER_IP, 
                                         HTTP_REQUEST_SERVER_IP_SIZE, 
                                         &http_request->serverIp) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_str(tmp_p, HTTP_REQUEST_USER_AGENT, 
                                         HTTP_REQUEST_USER_AGENT_SIZE, 
                                         &http_request->userAgent) == FLB_TRUE) {
                continue;
            }

            else if (assign_subfield_int(tmp_p, HTTP_REQUEST_CACHE_FILL_BYTES, 
                                         HTTP_REQUEST_CACHE_FILL_BYTES_SIZE, 
                                         &http_request->cacheFillBytes) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_int(tmp_p, HTTP_REQUEST_REQUESTSIZE, 
                                         HTTP_REQUEST_REQUESTSIZE_SIZE, 
                                         &http_request->requestSize) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_int(tmp_p, HTTP_REQUEST_RESPONSESIZE, 
                                         HTTP_REQUEST_RESPONSESIZE_SIZE, 
                                         &http_request->responseSize) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_int(tmp_p, HTTP_REQUEST_STATUS, 
                                         HTTP_REQUEST_STATUS_SIZE, 
                                         &http_request->status) == FLB_TRUE) {
                continue;
            }

            else if (assign_subfield_bool(tmp_p, HTTP_REQUEST_CACHE_HIT, 
                                          HTTP_REQUEST_CACHE_HIT_SIZE, 
                                          &http_request->cacheHit) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_bool(tmp_p, HTTP_REQUEST_CACHE_LOOKUP, 
                                          HTTP_REQUEST_CACHE_LOOKUP_SIZE, 
                                          &http_request->cacheLookup) == FLB_TRUE) {
                continue;
            }
            else if (assign_subfield_bool(tmp_p, HTTP_REQUEST_CACHE_VALIDATE_WITH_ORIGIN_SERVER, 
                                          HTTP_REQUEST_CACHE_VALIDATE_WITH_ORIGIN_SERVER_SIZE, 
                                          &http_request->cacheValidatedWithOriginServer) == FLB_TRUE) {
                continue;
            }

            else {
                *extra_subfields += 1;
            }
        }
    }
    
    return op_status == HTTPREQUEST_EXISTS;
}
                                                                                        
void pack_extra_http_request_subfields(msgpack_packer *mp_pck, 
                                       msgpack_object *http_request, 
                                       int extra_subfields) {
    msgpack_object_kv *p = http_request->via.map.ptr;
    msgpack_object_kv *const pend = http_request->via.map.ptr + http_request->via.map.size;

    msgpack_pack_map(mp_pck, extra_subfields);

    for (; p < pend; ++p) {
        if (cmp_str_helper(p->key, HTTP_REQUEST_LATENCY, 
                       HTTP_REQUEST_LATENCY_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_PROTOCOL, 
                          HTTP_REQUEST_PROTOCOL_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_REFERER, 
                          HTTP_REQUEST_REFERER_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_REMOTE_IP, 
                          HTTP_REQUEST_REMOTE_IP_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_REQUEST_METHOD, 
                          HTTP_REQUEST_REQUEST_METHOD_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_REQUEST_URL, 
                          HTTP_REQUEST_REQUEST_URL_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_SERVER_IP, 
                          HTTP_REQUEST_SERVER_IP_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_USER_AGENT, 
                          HTTP_REQUEST_USER_AGENT_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_CACHE_FILL_BYTES, 
                          HTTP_REQUEST_CACHE_FILL_BYTES_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_REQUESTSIZE, 
                          HTTP_REQUEST_REQUESTSIZE_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_RESPONSESIZE, 
                          HTTP_REQUEST_RESPONSESIZE_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_STATUS, 
                          HTTP_REQUEST_STATUS_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_CACHE_HIT, 
                          HTTP_REQUEST_CACHE_HIT_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_CACHE_LOOKUP, 
                          HTTP_REQUEST_CACHE_LOOKUP_SIZE)
            || cmp_str_helper(p->key, HTTP_REQUEST_CACHE_VALIDATE_WITH_ORIGIN_SERVER, 
                          HTTP_REQUEST_CACHE_VALIDATE_WITH_ORIGIN_SERVER_SIZE)) {
            
            continue;
        }

        msgpack_pack_object(mp_pck, p->key);
        msgpack_pack_object(mp_pck, p->val);
    }
}
