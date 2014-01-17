/*
 * duoku_user_guide.h
 *
 *  Created on: 2013-6-4
 *      Author: guoweijiang
 */

#ifndef DUOKU_USER_GUIDE_H_
#define DUOKU_USER_GUIDE_H_

#include "redis_module.h"
#include "../../duokoo_mysql.h"

class DuokuUserGuideInfo
{
    static const char* KEY_SUFFIX;

public:

    // 增加用户已收藏的攻略
    static int set_new_guide(const char* u_id, const char* gd_id);

    // 用户删除收藏的攻略
    static int remove_guide(const char* u_id, const char* gd_id);

    // 获取用户收藏的攻略总数
    static int get_user_guides_count(const char* u_id);

    // 获取用户收藏的攻略ID列表
    static int get_user_guides(const char* u_id,
                               list<string> &gds);

    static int remove_key_value_list(const char* key, list<string>& values);
};

#endif /* DUOKU_USER_GUIDE_H_ */
