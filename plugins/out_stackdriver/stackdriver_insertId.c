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
#include "stackdriver_insertId.h"

void add_insertId_field(flb_sds_t *insertId, msgpack_packer *mp_pck) {
	msgpack_pack_str(mp_pck, 8);
    msgpack_pack_str_body(mp_pck, "insertId", 8);

    msgpack_pack_str(mp_pck, flb_sds_len(*insertId));
    msgpack_pack_str_body(mp_pck, *insertId, flb_sds_len(*insertId));
}

bool extract_insertId(flb_sds_t *insertId, msgpack_object *obj) {
    bool insertId_existed = false;

    if (obj->via.map.size != 0) {
        flb_sds_t field_name = flb_sds_create("");
    	
        msgpack_object_kv* p = obj->via.map.ptr;
        msgpack_object_kv* const pend = obj->via.map.ptr + obj->via.map.size;

        for (; p < pend; ++p) {
            if (p->key.type == MSGPACK_OBJECT_STR && p->val.type == MSGPACK_OBJECT_STR) {
                field_name = flb_sds_copy(field_name, p->key.via.str.ptr, p->key.via.str.size);
                
                if (strcmp(field_name, "insertId") == 0) {
                	*insertId = flb_sds_copy(*insertId, p->val.via.str.ptr, p->val.via.str.size);
                    insertId_existed = true;
                    break;
                }
            }
        }
        flb_sds_destroy(field_name); 
    }
    
    /* Invalid if insertId is empty */
	return insertId_existed && flb_sds_is_empty(*insertId) == FLB_FALSE;
}

int pack_object_except_insertId(msgpack_packer *mp_pck, msgpack_object *obj) {
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
            if (strcmp(cur_key, "insertId") == 0 && kv->val.type == MSGPACK_OBJECT_STR) {
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