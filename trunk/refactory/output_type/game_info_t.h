/*
 * game_brief_info_t.h
 *
 *  Created on: 2013-5-10
 *      Author: guoweijiang
 */

#ifndef GAME_BRIEF_INFO_T_H_
#define GAME_BRIEF_INFO_T_H_

#include "common_header.h"
#include "../module/mcp_content_news.h"

/*
struct game_info_t
{
    string info_type;           //资讯类型 1-公告 2-新闻 3-活动 4-攻略
    string info_id;
    string c_id;
    string info_title;
    string info_content;
    string info_date;
    string type_name;
    string game_type;           //游戏类型 0-网游 1-单机 2-无对应游戏
};
*/
class game_info_t
{
public:
    static void set_outparameter(idl::game_info_t* out,
                                 MCPNewsInfo* binded_record);
    static void set_outparameter_hall(idl::game_info_t_hall* out,
                                 MCPNewsInfo* binded_record);
};

#endif /* GAME_BRIEF_INFO_T_H_ */
