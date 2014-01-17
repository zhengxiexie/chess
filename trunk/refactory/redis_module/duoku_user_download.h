/*
 * duoku_user_download.h
 *
 *  Created on: 2013-5-29
 *      Author: guoweijiang
 */

#ifndef DUOKU_USER_DOWNLOAD_H_
#define DUOKU_USER_DOWNLOAD_H_

#include "../../duokoo_mysql.h"
#include "redis_module.h"

class DuokuUserDownloadInfo
{
    static const char* KEY_SUFFIX;

public:

    // 增加用户已下载的游戏
    static int set_new_download(const char* u_id, const char* c_id);

    // 用户删除下载过的游戏
    static int remove_download(const char* u_id, const char* c_id);

    // 获取用户下载的游戏个数
    static int get_user_download_games_count(const char* u_id);

    // 获取用户下载的游戏ID列表
    static int get_user_download_games(const char* u_id,
                                       list<string> &vs);

    static int remove_key_value_list(const char* key, list<string>& values);
};

#endif /* DUOKU_USER_DOWNLOAD_H_ */
