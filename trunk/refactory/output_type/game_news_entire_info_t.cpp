/*
 * game_news_entire_info_t.cpp
 *
 *  Created on: 2013-8-3
 *      Author: guoweijiang
 */

#include "game_news_entire_info_t.h"
#include "game_info_t.h"
#include "game_basic_info.h"
#include "download_detail_data_t.h"

void game_news_entire_info_param::set_outparameter(idl::game_news_entire_info_t* out,
                                                   content_input_params& in,
                                                   MCPNewsInfo* news_record,
                                                   MCPGameInfo* game_basic,
                                                   MCPContentData& mcp_content_data,
                                                   const char* c_id)
{
    LOGD("[GWJ] %s:  start. game_record[%x], news_record[%x], c_id[%s]",
             __FUNCTION__, game_basic, news_record, c_id);

    if(news_record != NULL)
    {
        game_info_t::set_outparameter(out->m_news_info(), news_record);
    }

    if(game_basic != NULL)
    {
        game_basic_info_param::set_outparameter(out->m_game_basic_info(), in, game_basic);
    }

    if(IS_VALUED_STRING(c_id))
    {
        KeyBucket* key_bucket = mcp_content_data.get_key_bucket(c_id);

        if(key_bucket != NULL)
        {
            int bkt_size = key_bucket->size();

            LOGD("[GWJ] %s: download_bucket_size[%d] c_id[%s]",
                    __FUNCTION__, bkt_size, c_id);

            for(int i = 0; i < bkt_size; i++)
            {
                MCPContentDataInfo* download_record = (MCPContentDataInfo*)(*key_bucket)[i];
                download_detail_data_t::set_outparameter(out->m_game_download_list(i),
                                                         download_record);
            }
        }
        else
        {
            LOGD("[GWJ] %s: c_id[%s] download key_bucket not found",
                       __FUNCTION__, c_id);
        }
    }
}
