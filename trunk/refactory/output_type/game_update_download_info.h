/**
 * @file game_update_download_info.h
 * @brief 输出基本信息、下载信息、更新信息
 * @author zhengxie
 * @version 1.0
 * @date 2013-08-03
 */

#ifndef GAME_UPDATE_DOWNLOAD_INFO_H_
#define GAME_UPDATE_DOWNLOAD_INFO_H_

#include "common_header.h"
#include "../module/mcp_game_content.h"
#include "../module/mcp_content_data.h"
#include "../module/mcp_diff_update_info.h"
#include "../input_type/content_input.h"
#include "download_detail_data_t.h"
#include "game_basic_info.h"
#include "diff_detail_data_t.h"

class game_update_info_param
{
public:
    static void set_outparameter(idl::game_update_download_info* out,
                                 content_input_params& in,
                                 MCPContentData& download_module,
                                 MCPGameContent& game_basic_module,
								 MCPDiffUpdateInfo& mcp_diff_update_module,
                                 int index);

};

#endif /* GAME_UPDATE_DOWNLOAD_INFO_H_ */
