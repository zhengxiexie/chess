/*
 * duoku_user_favorite.h
 *
 *  Created on: 2013-6-4
 *      Author: guoweijiang
 */

#ifndef DUOKU_USER_FAVORITE_H_
#define DUOKU_USER_FAVORITE_H_

#include "redis_module.h"
#include "../../duokoo_mysql.h"

class DuokuUserFavoriteInfo
{
    static const char* KEY_SUFFIX;

public:

    // 获取用户收藏的攻略ID列表
    static int get_user_favorites(const char* u_id,
                                  list<string> &fs);

    static int remove_favorite(const char* u_id, const char* c_id);

    static int set_new_favorite(const char* u_id, const char* c_id);

    static int get_user_favorite_games_count(const char* u_id);

    static int remove_key_value_list(const char* key, list<string>& values);
};

#endif /* DUOKU_USER_FAVORITE_H_ */
