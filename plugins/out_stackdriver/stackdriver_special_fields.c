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
#include "stackdriver_special_fields.h"

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
    
    return op_status == OPERATION_EXISTED;
}

typedef enum {
    NO_SOURCELOCATION = 1,
    SOURCELOCATION_EXISTED = 2
} sourceLocation_status;

static bool isInteger(char *s, int64_t size) {
    int i = 0;

    for(; i < size; ++ i) {
        if(s[i] > '9' || s[i] < '0') {
            return false;
        }
    }
    return true;
}

/* Return true if sourceLocation extracted */
bool extract_sourceLocation(msgpack_object *sourceLocation_obj, msgpack_object *obj)
{
    sourceLocation_status srcLoc_status = NO_SOURCELOCATION;

    if (obj->via.map.size != 0) {    	
        msgpack_object_kv* p = obj->via.map.ptr;
        msgpack_object_kv* const pend = obj->via.map.ptr + obj->via.map.size;

        for (; p < pend && srcLoc_status == NO_SOURCELOCATION; ++p) {
            if (p->val.type == MSGPACK_OBJECT_MAP 
                && strncmp(SOURCELOCATION_IN_JSON, p->key.via.str.ptr, p->key.via.str.size) == 0) {

                msgpack_object sub_field = p->val;
                srcLoc_status = SOURCELOCATION_EXISTED;
        
                msgpack_object_kv* tmp_p = sub_field.via.map.ptr;
                msgpack_object_kv* const tmp_pend = sub_field.via.map.ptr + sub_field.via.map.size;

                // validate the sourceLocation field
                for (; tmp_p < tmp_pend; ++tmp_p) {
                    if (tmp_p->val.type == MSGPACK_OBJECT_STR) {
                        if (strncmp("file", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0
                            || strncmp("function", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0) {
                            continue;
                        }
                        else if (strncmp("line", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0 
                                 && isInteger(tmp_p->val.via.str.ptr, tmp_p->val.via.str.size)) {
                            continue;
                        }
                        else {
                            return false;
                        }
                    }
                    else if (strncmp("line", tmp_p->key.via.str.ptr, tmp_p->key.via.str.size) == 0
                             && tmp_p->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                        continue;
                    }
                    else {
                        return false;
                    }
                }
                *sourceLocation_obj = p->val;
                break;
            }
        }
    }
    
    return srcLoc_status == SOURCELOCATION_EXISTED;
}

