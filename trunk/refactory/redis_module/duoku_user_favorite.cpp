/*
 * duoku_user_favorite.cpp
 *
 *  Created on: 2013-6-4
 *      Author: guoweijiang
 */

#include "duoku_user_favorite.h"

const char* DuokuUserFavoriteInfo::KEY_SUFFIX = "favorite_";

int DuokuUserFavoriteInfo::set_new_favorite(const char* u_id,
                                            const char* c_id)
{
    string key = string(DuokuUserFavoriteInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], c_id[%s], key[%s]",
         __FUNCTION__, u_id, c_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.add_new_key_value(key.c_str(), c_id);
}

int DuokuUserFavoriteInfo::remove_favorite(const char* u_id,
                                           const char* c_id)
{
    string key = string(DuokuUserFavoriteInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], c_id[%s], key[%s]",
         __FUNCTION__, u_id, c_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.del_key_value(key.c_str(), c_id);
}

int DuokuUserFavoriteInfo::get_user_favorites(const char* u_id,
                                              list<string>& fs)
{
    string key = string(DuokuUserFavoriteInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
            __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_by_key(key.c_str(), fs);
}

int DuokuUserFavoriteInfo::get_user_favorite_games_count(const char* u_id)
{
    string key = string(DuokuUserFavoriteInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
             __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_count_by_key(key.c_str());
}

int DuokuUserFavoriteInfo::remove_key_value_list(const char* u_id, list<string>& values)
{
    string key = string(DuokuUserFavoriteInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
                 __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.remove_key_value_list(key.c_str(), values);
}
