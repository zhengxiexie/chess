/*
 * redis_module.h
 *
 *  Created on: 2013-6-5
 *      Author: guoweijiang
 */

#ifndef REDIS_MODULE_H_
#define REDIS_MODULE_H_

#include "../../redis_connpool.h"
#include "../common.h"
#include <list>

#define ADD_MAPPING_REDIS_VALUE(key, c_id)\
    map<string, string> record;\
    string ts;\
    CommonInterface::get_time_now(ts);\
    record["c_id"] = string(c_id);\
    record["p_ts"] = ts;\
    return RedisModule::add_new_key_record(key.c_str(), record);

#define IS_REDIS_REPLY_ERROR(type)\
    (type == REDIS_REPLY_NIL || type == REDIS_REPLY_ERROR)

#define FREE_REDIS_REPLY(reply)\
    if(reply != NULL)\
    {\
        freeReplyObject(reply);\
        reply = NULL;\
    }

class RedisModule
{
private:
    redisContext* conn;
    redisReply*   reply;

public:

    RedisModule();
    ~RedisModule();

    int add_new_key_value(const char* key, const char* value, int minites_to_live=0);
    int add_new_tuple(const char* key, const char* value, int minites_to_live=0);
	int add_new_key_record(const char* key, const map<string, string>& record, int minites_to_live=0);
	int push_value_list(const char* key, const char* value, int minites_to_live=0);
	int get_value_list(const char* key, list<string>& values);
    int del_key_value(const char* key, const char* value);
    int del_key_record(const char* key);
    int get_value_by_key(const char* key, string& values);
    int get_values_by_key(const char* key, list<string>& values);
	int get_record_by_key(const char* key, map<string, string>& record);
	int get_keys(const char* like_key, list<string>& keys);
    int get_values_count_by_key(const char* key);
    int remove_key_value_list(const char* key, list<string>& values);
	int get_rank_by_key( const char* key, const char* set_name, int order_type=0 );
};

#endif /* REDIS_MODULE_H_ */
