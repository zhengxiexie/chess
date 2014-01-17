/*
 * netgame_activity_info_t.h
 *
 *  Created on: 2013-6-7
 *      Author: guoweijiang
 */

#ifndef NETGAME_ACTIVITY_INFO_T_H_
#define NETGAME_ACTIVITY_INFO_T_H_

#include "common_header.h"
#include "../module/mcp_game_content.h"
#include "../module/mcp_content_news.h"
#include "../module/mcp_netgame_activity.h"
#include "../binder/compound_binder.h"
#include "game_info_t.h"
#include "../input_type/game_news_info_input.h"
#include "../output_type/game_news_info_t.h"

class netgame_act_info_t
{
public:
    static void set_outparameter(idl::netgame_activity_info_t* out_parameter,
                                 game_news_info_input_params& in,
                                 MCPNetGameActivityInfoRecord* netgame_record,
                                 CompoundBinder* game_news_record);
};

#endif /* NETGAME_ACTIVITY_INFO_T_H_ */
