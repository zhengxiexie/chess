/*
 * game_brief_info_t.cpp
 *
 *  Created on: 2013-5-10
 *      Author: guoweijiang
 */
#include "game_info_t.h"

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
void game_info_t::set_outparameter(idl::game_info_t* out_parameter,
                                   MCPNewsInfo* binded_record)
{
    if(binded_record != NULL)
    {
        out_parameter->set_info_type(binded_record->type_id.c_str());
        out_parameter->set_info_id(binded_record->id.c_str());
        out_parameter->set_c_id(binded_record->c_id.c_str());
        out_parameter->set_info_title(binded_record->name.c_str());
        out_parameter->set_info_content(binded_record->content.c_str());
        out_parameter->set_info_date(binded_record->logdate.c_str());
        out_parameter->set_game_type(binded_record->game_type.c_str());
        SET_NEWS_TYPE_NAME(binded_record, out_parameter, type_name)
        LOGD("[GWJ] %s: set_type_name[%s]", __FUNCTION__, out_parameter->type_name());
    }
    else
    {
        LOGD("[GWJ] %s: record NULL Empty!", __FUNCTION__);
    }
}

void game_info_t::set_outparameter_hall(idl::game_info_t_hall* out_parameter,
                                   MCPNewsInfo* binded_record)
{
    if(binded_record != NULL)
    {
        out_parameter->set_info_type(binded_record->type_id.c_str());
        out_parameter->set_info_id(binded_record->id.c_str());
        out_parameter->set_c_id(binded_record->c_id.c_str());
        out_parameter->set_info_title(binded_record->name.c_str());
        out_parameter->set_info_content(binded_record->content.c_str());
        out_parameter->set_info_date(binded_record->logdate.c_str());
        out_parameter->set_game_type(binded_record->game_type.c_str());
        out_parameter->set_news_source(binded_record->news_source.c_str());
        SET_NEWS_TYPE_NAME(binded_record, out_parameter, type_name)
        LOGD("[GWJ] %s: set_type_name[%s]", __FUNCTION__, out_parameter->type_name());
    }
    else
    {
        LOGD("[GWJ] %s: record NULL Empty!", __FUNCTION__);
    }
}
