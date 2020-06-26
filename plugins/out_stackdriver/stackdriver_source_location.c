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
#include "stackdriver_source_location.h"

typedef enum {
    NO_SOURCELOCATION = 1,
    SOURCELOCATION_EXISTED = 2
} source_location_status;


void add_source_location_field(flb_sds_t *source_location_file, 
                               int64_t source_location_line, 
                               flb_sds_t *source_location_function, 
                               msgpack_packer *mp_pck)
{    
    msgpack_pack_str(mp_pck, 14);
    msgpack_pack_str_body(mp_pck, "sourceLocation", 14);
    msgpack_pack_map(mp_pck, 3);

    msgpack_pack_str(mp_pck, 4);
    msgpack_pack_str_body(mp_pck, "file", 4);
    msgpack_pack_str(mp_pck, flb_sds_len(*source_location_file));
    msgpack_pack_str_body(mp_pck, *source_location_file, 
                          flb_sds_len(*source_location_file));

    msgpack_pack_str(mp_pck, 4);
    msgpack_pack_str_body(mp_pck, "line", 4);
    msgpack_pack_int64(mp_pck, source_location_line);

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck, "function", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(*source_location_function));
    msgpack_pack_str_body(mp_pck, *source_location_function, 
                          flb_sds_len(*source_location_function));
}

/* Return FLB_TRUE if sourceLocation extracted */
int extract_source_location(flb_sds_t *source_location_file, 
                            int64_t *source_location_line,
                            flb_sds_t *source_location_function, 
                            msgpack_object *obj, int *extra_subfields)
{
    source_location_status op_status = NO_SOURCELOCATION;
    flb_sds_t key_name;
    flb_sds_t sub_key_name;

    if (obj->via.map.size == 0) {   
        return FLB_FALSE;
    } 	
    msgpack_object_kv *p = obj->via.map.ptr;
    msgpack_object_kv *const pend = obj->via.map.ptr + obj->via.map.size;

    for (; p < pend && op_status == NO_SOURCELOCATION; ++p) {
        key_name = flb_sds_create_len(p->key.via.str.ptr, p->key.via.str.size);

        if (p->val.type != MSGPACK_OBJECT_MAP 
            || p->key.type != MSGPACK_OBJECT_STR
            || strcmp(SOURCELOCATION_FIELD_IN_JSON, key_name) != 0) {

            flb_sds_destroy(key_name);
            continue;
        }
        flb_sds_destroy(key_name);

        op_status = SOURCELOCATION_EXISTED;
        msgpack_object sub_field = p->val;
        
        msgpack_object_kv *tmp_p = sub_field.via.map.ptr;
        msgpack_object_kv *const tmp_pend = sub_field.via.map.ptr + sub_field.via.map.size;

        /* Validate the subfields of sourceLocation */
        for (; tmp_p < tmp_pend; ++tmp_p) {
            if (tmp_p->key.type != MSGPACK_OBJECT_STR) {
                continue;
            }
            sub_key_name = flb_sds_create_len(tmp_p->key.via.str.ptr, tmp_p->key.via.str.size);
            if (strcmp("file", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);

                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                *source_location_file = flb_sds_copy(*source_location_file, 
                                                     tmp_p->val.via.str.ptr, 
                                                     tmp_p->val.via.str.size);
            }
            else if (strcmp("function", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);

                if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                    continue;
                }
                *source_location_function = flb_sds_copy(*source_location_function, 
                                                         tmp_p->val.via.str.ptr, 
                                                         tmp_p->val.via.str.size);
            }
            else if (strcmp("line", sub_key_name) == 0) {
                flb_sds_destroy(sub_key_name);

                if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                    *source_location_line = atoll(tmp_p->val.via.str.ptr);
                }
                else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    *source_location_line = tmp_p->val.via.i64;
                }
            }
            else {
                *extra_subfields += 1;
                flb_sds_destroy(sub_key_name);
            }

        }
    }
    
    
    return op_status == SOURCELOCATION_EXISTED;
}

void pack_extra_source_location_subfields(msgpack_packer *mp_pck, 
                                          msgpack_object *source_location, 
                                          int extra_subfields) {
    msgpack_object_kv *p = source_location->via.map.ptr;
    msgpack_object_kv *const pend = source_location->via.map.ptr + source_location->via.map.size;
    flb_sds_t key_name;

    msgpack_pack_map(mp_pck, extra_subfields);

    for (; p < pend; ++p) {
        key_name = flb_sds_create_len(p->key.via.str.ptr, p->key.via.str.size);

        if (strcmp("file", key_name) != 0 
            && strcmp("line", key_name) != 0
            && strcmp("function", key_name) != 0) {
            msgpack_pack_object(mp_pck, p->key);
            msgpack_pack_object(mp_pck, p->val);
        }

        flb_sds_destroy(key_name);
    }
}
