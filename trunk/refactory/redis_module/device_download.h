/*
 * device_download.h
 *
 *  Created on: 2013-6-18
 *      Author: guoweijiang
 */

#ifndef DEVICE_DOWNLOAD_H_
#define DEVICE_DOWNLOAD_H_

#include "../../duokoo_mysql.h"
#include "redis_module.h"

class DeviceDownloadInfo
{
    static const char* KEY_SUFFIX;

public:

    // 增加设备已下载的游戏
    static int set_new_download(const char* u_id, const char* c_id);

    // 设备删除下载过的游戏
    static int remove_download(const char* u_id, const char* c_id);

    // 获取设备下载的游戏个数
    static int get_device_download_games_count(const char* u_id);

    // 获取设备下载的游戏ID列表
    static int get_device_download_games(const char* u_id,
                                         list<string> &vs);

    static int remove_key_value_list(const char* key, list<string>& values);
};


#endif /* DEVICE_DOWNLOAD_H_ */
