/*
 * game_content_download_info_t.h
 *
 *  Created on: 2013-6-18
 *      Author: guoweijiang
 */

#ifndef GAME_CONTENT_DOWNLOAD_INFO_T_H_
#define GAME_CONTENT_DOWNLOAD_INFO_T_H_

/*
struct game_content_download_info_t
{
    content_basic_info_t basic_info;
    download_detail_t download_info[];
};
*/

#include "common_header.h"
#include "../module/mcp_game_content.h"
#include "../module/mcp_content_data.h"
#include "../binder/compound_binder.h"
#include "../input_type/content_input.h"
#include "download_detail_data_t.h"
#include "game_basic_info.h"

class game_content_download_info_param
{
public:

    static void set_outparameter(idl::game_content_download_info_t* out,
                                 content_input_params& in,
                                 CompoundBinder& com_record,
                                 int index);

    static void set_outparameter(idl::game_content_download_info_t* out,
                                 content_input_params& in,
                                 MCPGameContent& mcp_content,
                                 MCPContentData& mcp_content_data,
                                 string& id_str);

    static void set_outparameter(idl::game_content_download_info_t* out,
                                 content_input_params& in,
                                 MCPGameInfo* game_data,
                                 MCPContentData& mcp_content_data,
                                 string& id_str);
};

#endif /* GAME_CONTENT_DOWNLOAD_INFO_T_H_ */
