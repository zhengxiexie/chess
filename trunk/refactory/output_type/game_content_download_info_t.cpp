/*
 * game_content_download_info_t.cpp
 *
 *  Created on: 2013-6-18
 *      Author: guoweijiang
 */

#include "game_content_download_info_t.h"

void game_content_download_info_param::set_outparameter(idl::game_content_download_info_t* out,
                                                        content_input_params& in,
                                                        CompoundBinder& com_record,
                                                        int index)
{
    MCPGameInfo* game_data = (MCPGameInfo*)com_record.get_record_value(index, 0, 0);
    game_basic_info_param::set_outparameter(out->mutable_basic_info(), in, game_data);

    int download_size = com_record.get_bucket_size(index, 1);
    for(int i = 0; i < download_size; i++)
    {
        MCPContentDataInfo* download_info = (MCPContentDataInfo*)com_record.get_record_value(index, 1, i);
        download_detail_data_t::set_outparameter(out->mutable_download_info(i),
                                                 download_info);
    }

    LOGD("[GWJ] %s:  start. game_record[%x], download_size[%d]",
         __FUNCTION__, game_data, download_size);
}

/*
 *   进入到此处的   MCPGameInfo* game_data 必须不能是 NULL
 */
void game_content_download_info_param::set_outparameter(idl::game_content_download_info_t* out,
                                                        content_input_params& in,
                                                        MCPGameInfo* game_data,
                                                        MCPContentData& mcp_content_data,
                                                        string& id_str)
{
    game_basic_info_param::set_outparameter(out->mutable_basic_info(), in, game_data);
    int download_size = mcp_content_data.get_key_bucket_size(id_str.c_str());

    for(int i = 0; i < download_size; i++)
    {
        MCPContentDataInfo* download_info = (MCPContentDataInfo*)mcp_content_data.get_record_by_key_vec(id_str.c_str(), i);
        download_detail_data_t::set_outparameter(out->mutable_download_info(i), download_info);
    }

    LOGD("[GWJ] %s: id[%s]: game_record[%x], download_size[%d]",
            __FUNCTION__, id_str.c_str(), game_data, download_size);
}

void game_content_download_info_param::set_outparameter(idl::game_content_download_info_t* out,
                                                        content_input_params& in,
                                                        MCPGameContent& mcp_content,
                                                        MCPContentData& mcp_content_data,
                                                        string& id_str)
{
    MCPGameInfo* game_data = (MCPGameInfo*)mcp_content.get_record_by_key_vec(id_str.c_str(), 0);

    if(game_data != NULL)
    {
        game_basic_info_param::set_outparameter(out->mutable_basic_info(), in, game_data);
        int download_size = mcp_content_data.get_key_bucket_size(id_str.c_str());

        for(int i = 0; i < download_size; i++)
        {
            MCPContentDataInfo* download_info = (MCPContentDataInfo*)mcp_content_data.get_record_by_key_vec(id_str.c_str(), i);
            download_detail_data_t::set_outparameter(out->mutable_download_info(i), download_info);
        }

        LOGD("[GWJ] %s: id[%s]: game_record[%x], download_size[%d]",
             __FUNCTION__, id_str.c_str(), game_data, download_size);
    }
    else
    {
        LOGD("[GWJ] %s: id[%s] Not Found", __FUNCTION__, id_str.c_str());
    }
}
