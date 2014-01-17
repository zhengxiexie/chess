/*
 * open_server_info_t.cpp
 *
 *  Created on: 2013-6-4
 *      Author: guoweijiang
 */

#include "game_news_info_t.h"

void game_news_info_param::set_outparameter(idl::game_news_info_t* out,
                                            game_news_info_input_params& in,
                                            MCPNewsInfo* news_record,
                                            MCPGameInfo* game_record)
{
    LOGD("[GWJ] %s:  start. game_record[%x], news_record[%x]",
         __FUNCTION__, game_record, news_record);

    if(news_record == NULL && game_record == NULL)
    {
        LOGD("[GWJ] %s: game_record[%x], news_record[%x] has NULL!!",
             __FUNCTION__, game_record, news_record);
        return;
    }

    if(news_record != NULL)
    {
        LOGD("[GWJ] %s: news_record[id:%s, c_id:%s]",
             __FUNCTION__, news_record->id.c_str(), news_record->c_id.c_str());

        out->set_news_id(news_record->id.c_str());
        out->set_news_type(news_record->type_id.c_str());
        out->set_news_time(news_record->logdate.c_str());
        out->set_news_content(news_record->content.c_str());
        out->set_news_title(news_record->name.c_str());
        SET_NEWS_TYPE_NAME(news_record, out, news_type_name)
        out->set_game_type(news_record->game_type.c_str());
    }

    if(game_record != NULL)
    {
        LOGD("[GWJ] %s: game_record[%s]",
             __FUNCTION__, game_record->id.c_str());

        out->set_game_id(game_record->id.c_str());
        out->set_game_name(game_record->name.c_str());

        vector<MCPTupleItem>::const_iterator itr = game_record->img_info.begin();
        vector<MCPTupleItem>::const_iterator end_itr = game_record->img_info.end();
        for(int i = 0; itr != end_itr; itr++)
        {
            const char* img_type = (*itr).key.c_str();

            int index = CommonInterface::find_key_index(img_type,
                                                        in.game_img_types);

            if(index < 0)
            {
                LOGD("[GWJ] %s: No need img_type[%s]", __FUNCTION__, img_type);
            }
            else
            {
                LOGD("[GWJ] %s: need img_type[%s]", __FUNCTION__, img_type);
                out->mutable_game_img_info(i)->set_type(img_type);
                out->mutable_game_img_info(i)->set_url((*itr).value.c_str());
                i++;
            }
        }
    }

    LOGD("[GWJ] %s:  end", __FUNCTION__);
}
