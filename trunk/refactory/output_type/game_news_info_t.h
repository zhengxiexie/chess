/*
 * open_server_info_t.h
 *
 *  Created on: 2013-6-4
 *      Author: guoweijiang
 */

#ifndef OPEN_SERVER_INFO_T_H_
#define OPEN_SERVER_INFO_T_H_

#include "common_header.h"
#include "../module/mcp_content_news.h"
#include "../module/mcp_game_content.h"
#include "../input_type/game_news_info_input.h"

class game_news_info_param
{
public:

    static void set_outparameter(idl::game_news_info_t* out,
                                 game_news_info_input_params& in,
                                 MCPNewsInfo* news_record,
                                 MCPGameInfo* game_record
                                 /* 将来还要增加活动的 Key_Bucket */);
};

#endif /* OPEN_SERVER_INFO_T_H_ */
