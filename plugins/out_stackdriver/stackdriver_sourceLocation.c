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
#include "stackdriver_sourceLocation.h"

typedef enum {
    NO_SOURCELOCATION = 1,
    SOURCELOCATION_EXISTED = 2
} sourceLocation_status;


void add_sourceLocation_field(flb_sds_t *sourceLocation_file, int64_t sourceLocation_line, 
                                flb_sds_t *sourceLocation_function, msgpack_packer *mp_pck)
{    
    msgpack_pack_str(mp_pck, 14);
    msgpack_pack_str_body(mp_pck, "sourceLocation", 14);
    msgpack_pack_map(mp_pck, 3);

    msgpack_pack_str(mp_pck, 4);
    msgpack_pack_str_body(mp_pck,"file", 4);
    msgpack_pack_str(mp_pck, flb_sds_len(*sourceLocation_file));
    msgpack_pack_str_body(mp_pck,*sourceLocation_file, flb_sds_len(*sourceLocation_file));

    msgpack_pack_str(mp_pck, 4);
    msgpack_pack_str_body(mp_pck,"line", 4);
    msgpack_pack_int64(mp_pck, sourceLocation_line);

    msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck,"function", 8);
    msgpack_pack_str(mp_pck, flb_sds_len(*sourceLocation_function));
    msgpack_pack_str_body(mp_pck,*sourceLocation_function, flb_sds_len(*sourceLocation_function));
}

/* Return true if sourceLocation extracted */
bool extract_sourceLocation(flb_sds_t *sourceLocation_file, int64_t *sourceLocation_line,
                              flb_sds_t *sourceLocation_function, msgpack_object *obj, int *extra_subfields)
{
    sourceLocation_status op_status = NO_SOURCELOCATION;

    if (obj->via.map.size != 0) {    	
        msgpack_object_kv* p = obj->via.map.ptr;
        msgpack_object_kv* const pend = obj->via.map.ptr + obj->via.map.size;

        for (; p < pend && op_status == NO_SOURCELOCATION; ++p) {
            if (p->val.type == MSGPACK_OBJECT_MAP 
                && strncmp(SOURCELOCATION_FIELD_IN_JSON, p->key.via.str.ptr, p->key.via.str.size) == 0) {
                
                op_status = SOURCELOCATION_EXISTED;
                msgpack_object sub_field = p->val;
                
                msgpack_object_kv* tmp_p = sub_field.via.map.ptr;
                msgpack_object_kv* const tmp_pend = sub_field.via.map.ptr + sub_field.via.map.size;

                /* Validate the subfields of sourceLocation */
                for (; tmp_p < tmp_pend; ++tmp_p) {
                    if (strncmp("file", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if(tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        *sourceLocation_file = flb_sds_copy(*sourceLocation_file, tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("function", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if (tmp_p->val.type != MSGPACK_OBJECT_STR) {
                            continue;
                        }
                        *sourceLocation_function = flb_sds_copy(*sourceLocation_function, 
                                                    tmp_p->val.via.str.ptr, tmp_p->val.via.str.size);
                    }
                    else if (strncmp("line", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                        if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                            *sourceLocation_line = atoll(tmp_p->val.via.str.ptr);
                        }
                        else if (tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                            *sourceLocation_line = tmp_p->val.via.i64;
                        }
                    }
                    else {
                        *extra_subfields += 1;
                    }

                }
            }
        }
    }
    
    return op_status == SOURCELOCATION_EXISTED;
}

void pack_extra_sourceLocation_subfields(msgpack_packer *mp_pck, msgpack_object *sourceLocation, int extra_subfields) {
    msgpack_pack_map(mp_pck, extra_subfields);

    msgpack_object_kv* p = sourceLocation->via.map.ptr;
    msgpack_object_kv* const pend = sourceLocation->via.map.ptr + sourceLocation->via.map.size;

    for (; p < pend; ++p) {
        if(strncmp("file", p->key.via.str.ptr, p->key.via.str.size) != 0 
            && strncmp("line", p->key.via.str.ptr, p->key.via.str.size) != 0
            && strncmp("function", p->key.via.str.ptr, p->key.via.str.size) != 0) {
            msgpack_pack_object(mp_pck, p->key);
            msgpack_pack_object(mp_pck, p->val);
        }
    }

}
