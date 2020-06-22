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


#ifndef FLB_STD_HTTPREQUEST_H
#define FLB_STD_HTTPREQUEST_H

#include "stackdriver.h"

struct httpRequest {
    flb_sds_t requestMethod;
    flb_sds_t requestUrl;
    flb_sds_t userAgent;
    flb_sds_t remoteIp;
    flb_sds_t serverIp;
    flb_sds_t referer;
    flb_sds_t latency;
    flb_sds_t protocol;

    int64_t requestSize;
    int64_t responseSize;
    int64_t status;
    int64_t cacheFillBytes;

    int cacheLookup;
    int cacheHit;
    int cacheValidatedWithOriginServer;
};

void init_httpRequest(struct httpRequest *http_request);
void destroy_httpRequest(struct httpRequest *http_request);
void add_httpRequest_field(struct httpRequest *http_request, msgpack_packer *mp_pck);
int extract_httpRequest(struct httpRequest *http_request, msgpack_object *obj, int *extra_subfields);
void pack_extra_httpRequest_subfields(msgpack_packer *mp_pck, msgpack_object *http_request, int extra_subfields);

#endif