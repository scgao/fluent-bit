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
#include "stackdriver_httpRequest.h"

typedef enum {
    NO_HTTPREQUEST = 1,
    HTTPREQUEST_EXISTED = 2
} httpRequest_status;

void init_httpRequest(struct httpRequest *http_request)
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

    http_request->cacheLookup = false;
    http_request->cacheHit = false;
    http_request->cacheValidatedWithOriginServer = false;
}

void destroy_httpRequest(struct httpRequest *http_request)
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

void add_httpRequest_field(struct httpRequest *http_request, msgpack_packer *mp_pck)
{    
    msgpack_pack_str(mp_pck, 11);
    msgpack_pack_str_body(mp_pck, "httpRequest", 11);

    if (flb_sds_is_empty(http_request->latency) == FLB_TRUE) {
        msgpack_pack_map(mp_pck, 14);
    }
    else {
        msgpack_pack_map(mp_pck, 15);

        msgpack_pack_str(mp_pck, 7);
        msgpack_pack_str_body(mp_pck,"latency", 7);
        msgpack_pack_str(mp_pck, flb_sds_len(http_request->latency));
        msgpack_pack_str_body(mp_pck, http_request->latency, flb_sds_len(http_request->latency));
    }

    /* String sub-fields */
    msgpack_pack_str(mp_pck, 13);
    msgpack_pack_str_body(mp_pck,"requestMethod", 13);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->requestMethod));
    msgpack_pack_str_body(mp_pck, http_request->requestMethod, flb_sds_len(http_request->requestMethod));

    msgpack_pack_str(mp_pck, 10);
    msgpack_pack_str_body(mp_pck,"requestUrl", 10);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->requestUrl));
    msgpack_pack_str_body(mp_pck, http_request->requestUrl, flb_sds_len(http_request->requestUrl));

    msgpack_pack_str(mp_pck, 9);
    msgpack_pack_str_body(mp_pck,"userAgent", 9);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->userAgent));
    msgpack_pack_str_body(mp_pck, http_request->userAgent, flb_sds_len(http_request->userAgent));

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck,"remoteIp", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->remoteIp));
    msgpack_pack_str_body(mp_pck, http_request->remoteIp, flb_sds_len(http_request->remoteIp));

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck,"serverIp", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->serverIp));
    msgpack_pack_str_body(mp_pck, http_request->serverIp, flb_sds_len(http_request->serverIp));

     msgpack_pack_str(mp_pck, 7);
    msgpack_pack_str_body(mp_pck,"referer", 7);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->referer));
    msgpack_pack_str_body(mp_pck, http_request->referer, flb_sds_len(http_request->referer));

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck,"protocol", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->protocol));
    msgpack_pack_str_body(mp_pck, http_request->protocol, flb_sds_len(http_request->protocol));

    /* Integer sub-fields */
    msgpack_pack_str(mp_pck, 11);
    msgpack_pack_str_body(mp_pck,"requestSize", 11);
    msgpack_pack_int64(mp_pck, http_request->requestSize);

    msgpack_pack_str(mp_pck, 12);
    msgpack_pack_str_body(mp_pck,"responseSize", 12);
    msgpack_pack_int64(mp_pck, http_request->responseSize);

    msgpack_pack_str(mp_pck, 6);
    msgpack_pack_str_body(mp_pck,"status", 6);
    msgpack_pack_int64(mp_pck, http_request->status);

    msgpack_pack_str(mp_pck, 14);
    msgpack_pack_str_body(mp_pck,"cacheFillBytes", 14);
    msgpack_pack_int64(mp_pck, http_request->cacheFillBytes);

    /* Boolean sub-fields */
    msgpack_pack_str(mp_pck, 11);
    msgpack_pack_str_body(mp_pck,"cacheLookup", 11);
    if (http_request->cacheLookup) {
        msgpack_pack_true(mp_pck);
    }
    else {
        msgpack_pack_false(mp_pck);
    }

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck,"cacheHit", 8);
    if (http_request->cacheLookup) {
        msgpack_pack_true(mp_pck);
    }
    else {
        msgpack_pack_false(mp_pck);
    }

    msgpack_pack_str(mp_pck, 30);
    msgpack_pack_str_body(mp_pck,"cacheValidatedWithOriginServer", 30);
    if (http_request->cacheValidatedWithOriginServer) {
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
static bool validate_latency(msgpack_object_str latency) {
    return false;
}

/* Return true if httpRequest extracted */
bool extract_httpRequest(struct httpRequest *http_request, msgpack_object *obj, int *extra_subfields)
{
    httpRequest_status op_status = NO_HTTPREQUEST;

    if (obj->via.map.size != 0) {    	
        msgpack_object_kv* p = obj->via.map.ptr;
        msgpack_object_kv* const pend = obj->via.map.ptr + obj->via.map.size;

        for (; p < pend && op_status == NO_HTTPREQUEST; ++p) {
            if (p->val.type == MSGPACK_OBJECT_MAP 
                && strncmp(HTTPREQUEST_FIELD_IN_JSON, p->key.via.str.ptr, p->key.via.str.size) == 0) {
                
                op_status = HTTPREQUEST_EXISTED;
                msgpack_object sub_field = p->val;
                
                msgpack_object_kv* tmp_p = sub_field.via.map.ptr;
                msgpack_object_kv* const tmp_pend = sub_field.via.map.ptr + sub_field.via.map.size;

                /* Validate the subfields of httpRequest */
                for (; tmp_p < tmp_pend; ++tmp_p) {
                    if (strncmp("requestMethod", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        http_request->requestMethod = flb_sds_copy(http_request->requestMethod, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("requestUrl", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        http_request->requestUrl = flb_sds_copy(http_request->requestUrl, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("userAgent", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        http_request->userAgent = flb_sds_copy(http_request->userAgent, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("remoteIp", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        http_request->remoteIp = flb_sds_copy(http_request->remoteIp, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("serverIp", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        http_request->serverIp = flb_sds_copy(http_request->serverIp, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("referer", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        http_request->referer = flb_sds_copy(http_request->referer, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("protocol", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        http_request->protocol = flb_sds_copy(http_request->protocol, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("latency", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        if(validate_latency(tmp_p->key.via.str)) {
                            http_request->latency = flb_sds_copy(http_request->latency, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                        }
                    }

                    else if (strncmp("requestSize", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                            http_request->requestSize = atoll(tmp_p->val.via.str.ptr);
                        }
                        else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                            http_request->requestSize = tmp_p->val.via.i64;
                        }
                    }
                    else if (strncmp("responseSize", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                            http_request->responseSize = atoll(tmp_p->val.via.str.ptr);
                        }
                        else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                            http_request->responseSize = tmp_p->val.via.i64;
                        }
                    }
                    else if (strncmp("status", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                            http_request->status = atoll(tmp_p->val.via.str.ptr);
                        }
                        else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                            http_request->status = tmp_p->val.via.i64;
                        }
                    }
                    else if (strncmp("cacheFillBytes", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                            http_request->cacheFillBytes = atoll(tmp_p->val.via.str.ptr);
                        }
                        else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                            http_request->cacheFillBytes = tmp_p->val.via.i64;
                        }
                    }

                    else if (strncmp("cacheLookup", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_BOOLEAN) {
                            continue;
                        }
                        http_request->cacheLookup = tmp_p->val.via.boolean;
                    }
                    else if (strncmp("cacheHit", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_BOOLEAN) {
                            continue;
                        }
                        http_request->cacheHit = tmp_p->val.via.boolean;
                    }
                    else if (strncmp("cacheValidatedWithOriginServer", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_BOOLEAN) {
                            continue;
                        }
                        http_request->cacheValidatedWithOriginServer = tmp_p->val.via.boolean;
                    }

                    else {
                        *extra_subfields += 1;
                    }
                }
            }
        }
    }
    
    return op_status == HTTPREQUEST_EXISTED;
}

void pack_extra_httpRequest_subfields(msgpack_packer *mp_pck, msgpack_object *http_request, int extra_subfields) {
    msgpack_pack_map(mp_pck, extra_subfields);

    msgpack_object_kv* p = http_request->via.map.ptr;
    msgpack_object_kv* const pend = http_request->via.map.ptr + http_request->via.map.size;

    for (; p < pend; ++p) {
        if(strncmp("requestMethod", p->key.via.str.ptr, p->key.via.str.size) != 0 
            && strncmp("requestUrl", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("userAgent", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("remoteIp", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("serverIp", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("referer", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("latency", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("protocol", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("requestSize", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("responseSize", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("status", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("cacheFillBytes", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("cacheLookup", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("cacheHit", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("cacheValidatedWithOriginServer", p->key.via.str.ptr, p->key.via.str.size) != 0) {
            msgpack_pack_object(mp_pck, p->key);
            msgpack_pack_object(mp_pck, p->val);
        }
    }

}
