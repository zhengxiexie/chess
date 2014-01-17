/*
 * duoku_user_download.cpp
 *
 *  Created on: 2013-5-29
 *      Author: guoweijiang
 */

#include "duoku_user_download.h"

const char* DuokuUserDownloadInfo::KEY_SUFFIX = "user_download_";

int DuokuUserDownloadInfo::set_new_download(const char* u_id,
                                            const char* c_id)
{
    string key = string(DuokuUserDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], c_id[%s], key[%s]",
         __FUNCTION__, u_id, c_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.add_new_key_value(key.c_str(), c_id);
}

int DuokuUserDownloadInfo::remove_download(const char* u_id,
                                           const char* c_id)
{
    string key = string(DuokuUserDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], c_id[%s], key[%s]",
         __FUNCTION__, u_id, c_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.del_key_value(key.c_str(), c_id);
}

int DuokuUserDownloadInfo::get_user_download_games_count(const char* u_id)
{
    string key = string(DuokuUserDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
             __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_count_by_key(key.c_str());
}

int DuokuUserDownloadInfo::get_user_download_games(const char* u_id,
                                                   list<string>& vs)
{
    string key = string(DuokuUserDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
             __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_by_key(key.c_str(), vs);
}

int DuokuUserDownloadInfo::remove_key_value_list(const char* u_id, list<string>& values)
{
    string key = string(DuokuUserDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
                 __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.remove_key_value_list(key.c_str(), values);
}
