/*
 * duoku_user_guide.cpp
 *
 *  Created on: 2013-6-4
 *      Author: guoweijiang
 */

#include "duoku_user_guide.h"

const char* DuokuUserGuideInfo::KEY_SUFFIX = "user_guide_";

int DuokuUserGuideInfo::set_new_guide(const char* u_id, const char* gd_id)
{
    string key = string(DuokuUserGuideInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], gd_id[%s], key[%s]",
         __FUNCTION__, u_id, gd_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.add_new_key_value(key.c_str(), gd_id);
}

int DuokuUserGuideInfo::remove_guide(const char* u_id, const char* gd_id)
{
    string key = string(DuokuUserGuideInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], gd_id[%s], key[%s]",
         __FUNCTION__, u_id, gd_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.del_key_value(key.c_str(), gd_id);
}

int DuokuUserGuideInfo::get_user_guides_count(const char* u_id)
{
    string key = string(DuokuUserGuideInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
             __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_count_by_key(key.c_str());
}


int DuokuUserGuideInfo::get_user_guides(const char* u_id,
                                        list<string>& gds)
{
    string key = string(DuokuUserGuideInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
             __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_by_key(key.c_str(), gds);
}

int DuokuUserGuideInfo::remove_key_value_list(const char* u_id, list<string>& values)
{
    string key = string(DuokuUserGuideInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
                 __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.remove_key_value_list(key.c_str(), values);
}
