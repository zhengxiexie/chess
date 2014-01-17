/*
 * game_news_entire_info_t.h
 *
 *  Created on: 2013-8-3
 *      Author: guoweijiang
 */

#ifndef GAME_NEWS_ENTIRE_INFO_T_H_
#define GAME_NEWS_ENTIRE_INFO_T_H_

/*
struct game_news_entire_info_t
{
    game_info_t news_info;
    content_basic_info_t game_basic_info;
    download_detail_t game_download_list[];
};
 */

#include "common_header.h"
#include "../module/mcp_content_news.h"
#include "../module/mcp_game_content.h"
#include "../module/mcp_content_data.h"
#include "../input_type/content_input.h"

class game_news_entire_info_param
{
public:
    static void set_outparameter(idl::game_news_entire_info_t* out,
                                 content_input_params& in,
                                 MCPNewsInfo* news_record,
                                 MCPGameInfo* game_basic,
                                 MCPContentData& mcp_content_data,
                                 const char* c_id);
};

#endif /* GAME_NEWS_ENTIRE_INFO_T_H_ */
