/**
 * @file championship_basic_download_info.h
 * @brief 输出争霸信息、部分下载信息和基本信息
 * @author zhengxie
 * @version 1.0
 * @date 2013-08-03
 */

#ifndef CHAMPIONSHIP_BASIC_DOWNLOAD_INFO_H_
#define CHAMPIONSHIP_BASIC_DOWNLOAD_INFO_H_

#include "common_header.h"
#include "../module/mcp_game_content.h"
#include "../module/mcp_content_data.h"
#include "../module/mcp_content_championship.h"
#include "../input_type/content_input.h"
#include "download_detail_data_t.h"
#include "game_basic_info.h"

class championship_basic_download_info_param
{
public:
    static void set_outparameter(idl::championship_info_t* out,
                                 MCPContentData& download_module,
                                 MCPGameContent& game_basic_module,
								 MCPContentChampionship& mcp_content_championship_module,
                                 int champion_index, int index);

};

#endif /* CHAMPIONSHIP_BASIC_DOWNLOAD_INFO_H_ */
