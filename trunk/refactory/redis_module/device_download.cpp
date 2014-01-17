/*
 * device_download.cpp
 *
 *  Created on: 2013-6-18
 *      Author: guoweijiang
 */

#include "device_download.h"

const char* DeviceDownloadInfo::KEY_SUFFIX = "device_download_";

int DeviceDownloadInfo::set_new_download(const char* u_id,
                                         const char* c_id)
{
    string key = string(DeviceDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], c_id[%s], key[%s]",
         __FUNCTION__, u_id, c_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.add_new_key_value(key.c_str(), c_id);
}

int DeviceDownloadInfo::remove_download(const char* u_id,
                                        const char* c_id)
{
    string key = string(DeviceDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], c_id[%s], key[%s]",
         __FUNCTION__, u_id, c_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.del_key_value(key.c_str(), c_id);
}

int DeviceDownloadInfo::get_device_download_games_count(const char* u_id)
{
    string key = string(DeviceDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
             __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_count_by_key(key.c_str());
}

int DeviceDownloadInfo::get_device_download_games(const char* u_id,
                                                  list<string>& vs)
{
    string key = string(DeviceDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
             __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.get_values_by_key(key.c_str(), vs);
}

int DeviceDownloadInfo::remove_key_value_list(const char* u_id, list<string>& values)
{
    string key = string(DeviceDownloadInfo::KEY_SUFFIX) + u_id;

    LOGD("[GWJ] %s: start. u_id[%s], key[%s]",
                 __FUNCTION__, u_id, key.c_str());

    RedisModule redis_processor;
    return redis_processor.remove_key_value_list(key.c_str(), values);
}

