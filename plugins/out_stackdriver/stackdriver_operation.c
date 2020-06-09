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
#include "stackdriver_operation.h"

typedef enum {
    NO_OPERATION = 1,
    OPERATION_EXISTED = 2
} operation_status;


/* Return true if operation extracted */
bool extract_operation(msgpack_object *operation, msgpack_object *obj)
{
    operation_status op_status = NO_OPERATION;

    if (obj->via.map.size != 0) {    	
        msgpack_object_kv* p = obj->via.map.ptr;
        msgpack_object_kv* const pend = obj->via.map.ptr + obj->via.map.size;

        for (; p < pend && op_status == NO_OPERATION; ++p) {
            if (p->val.type == MSGPACK_OBJECT_MAP 
                && strncmp(OPERATION_FIELD_IN_JSON, p->key.via.str.ptr, p->key.via.str.size) == 0) {

                op_status = OPERATION_EXISTED;
                msgpack_object sub_field = p->val;
                
                msgpack_object_kv* tmp_p = sub_field.via.map.ptr;
                msgpack_object_kv* const tmp_pend = sub_field.via.map.ptr + sub_field.via.map.size;

                /* Validate the operation field */
                for (; tmp_p < tmp_pend; ++tmp_p) {
                    if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                        if (strncmp("id", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) != 0 
                            && strncmp("producer", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) != 0) {
                            return false;
                        }
                    }
                    else if (tmp_p->val.type == MSGPACK_OBJECT_BOOLEAN) {
                        if (strncmp("first", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) != 0 
                            && strncmp("last", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) != 0) {
                            return false;
                        }
                    }
                    else {
                        return false;
                    }
                }
                *operation = p->val;
                break;
            }
        }
    }
    
    /* Invalid if id/producer is empty */
    return op_status == OPERATION_EXISTED;
}

int pack_object_except_operation(msgpack_packer *mp_pck, msgpack_object *obj){
	/* obj type must be MSGPACK_OBJECT_MAP */
    int ret = msgpack_pack_map(mp_pck, obj->via.map.size - 1);
    if(ret < 0) {
        return ret;
    }
    else {
        msgpack_object_kv* kv = obj->via.map.ptr;
        msgpack_object_kv* const kvend = obj->via.map.ptr + obj->via.map.size;
        for(; kv != kvend; ++kv) {
            flb_sds_t cur_key = flb_sds_create_len(kv->key.via.str.ptr, kv->key.via.str.size);
            if (strcmp(cur_key, OPERATION_FIELD_IN_JSON) == 0 && kv->val.type == MSGPACK_OBJECT_MAP) {
                flb_sds_destroy(cur_key);
                continue;
            }
            
            flb_sds_destroy(cur_key);
            ret = msgpack_pack_object(mp_pck, kv->key);
            if(ret < 0) { return ret; }
            ret = msgpack_pack_object(mp_pck, kv->val);
            if(ret < 0) { return ret; }
        }
        return 0;
    }
}
