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

void add_httpRequest_field(httpRequest *http_request, msgpack_packer *mp_pck) {
    msgpack_pack_str(mp_pck, 1);
    msgpack_pack_str_body(mp_pck, "httpRequest", 11);
    msgpack_pack_map(mp_pck, 14);

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
    msgpack_pack_str_body(mp_pck,"latency", 7);
    msgpack_pack_str(mp_pck, flb_sds_len(http_request->latency));
    msgpack_pack_str_body(mp_pck, http_request->latency, flb_sds_len(http_request->latency));

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

bool extract_httpRequest(httpRequest *http_request, msgpack_object *obj) {
    return false;
}