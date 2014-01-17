/*
 * common_simple_buf.h
 *
 *  Created on: 2013-5-15
 *      Author: guoweijiang
 */

#ifndef COMMON_SIMPLE_BUF_H_
#define COMMON_SIMPLE_BUF_H_

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "common.h"
#include "mysql_module/mysql_general_module.h"

class CommonSimpleBuffer
{
    // 单例模式
    NSP_SINGLETONIZE(CommonSimpleBuffer)

    // 游戏公共属性
    MysqlTable* m_public_propertys;

    // 游戏资讯类型
    MysqlTable* m_game_info_type;

    // 获取公共属性
    int fill_public_property();

    // 获取游戏资讯类型名称
    int fill_game_info_type_name();

    int get_public_property_url(const char* id, const char* prop_url);

public:
    ~CommonSimpleBuffer();

    int init_buffer();
    int get_game_info_type_name(const char* id, string& type_name);
};

#endif /* COMMON_SIMPLE_BUF_H_ */
