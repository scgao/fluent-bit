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


#ifndef FLB_STD_TIMESTAMP_H
#define FLB_STD_TIEMSTAMP_H

#include "stackdriver.h"
#include <fluent-bit/flb_time.h>

typedef enum {
    TIMESTAMP_NOT_PRESENT = 0,
    FORMAT_TIMESTAMP = 1,
    FORMAT_TIMESTAMPSECONDS = 2,
    FORMAT_TIME = 3,

    /* used for removing subfield */
    FORMAT_INVALID_TIMESTAMP = 4,
    FORMAT_INVALID_TIMESTAMPSECONDS = 5,
    FORMAT_INVALID_TIME = 6
}timestamp_status;

timestamp_status extract_timestamp(msgpack_object *obj,
                                   struct flb_time *tms,
                                   flb_sds_t time);


#endif
