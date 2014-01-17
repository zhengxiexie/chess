/*
 * netgame_activity_info_t.cpp
 *
 *  Created on: 2013-6-7
 *      Author: guoweijiang
 */

#include "netgame_activity_info_t.h"

void netgame_act_info_t::set_outparameter(idl::netgame_activity_info_t* out_parameter,
                                          game_news_info_input_params& in,
                                          MCPNetGameActivityInfoRecord* netgame_record,
                                          CompoundBinder* game_news_record)
{
    if(netgame_record == NULL || game_news_record == NULL)
    {
        LOGD("[GWJ] %s:  netgame_record[%x], game_news_record[%x]. Has NULL Record",
                __FUNCTION__, netgame_record, game_news_record);

        return;
    }

    const char* act_id = netgame_record->id.c_str();
    const char* act_title = netgame_record->title.c_str();
    const char* act_content = netgame_record->content.c_str();
    const char* act_logdate = netgame_record->logdate.c_str();

    out_parameter->set_act_id(act_id);
    out_parameter->set_act_title(act_title);
    out_parameter->set_act_content(act_content);
    out_parameter->set_act_logdate(act_logdate);

    vector<MCPTupleItem>::const_iterator itr = netgame_record->img_info.begin();
    vector<MCPTupleItem>::const_iterator end_itr = netgame_record->img_info.end();
    for(int i = 0; itr != end_itr; itr++)
    {
        const char* img_type = (*itr).key.c_str();
        int index = CommonInterface::find_key_index(img_type, in.activity_img_types);

        if(index < 0)
        {
            LOGD("[GWJ] %s: No need img_type[%s]", __FUNCTION__, img_type);
        }
        else
        {
            LOGD("[GWJ] %s: need img_type[%s]", __FUNCTION__, img_type);
            out_parameter->mutable_act_img_info(i)->set_type(img_type);
            out_parameter->mutable_act_img_info(i)->set_url((*itr).value.c_str());
            i++;
        }
    }

    int record_count = game_news_record->get_result_count();
    int j = 0;
    for(int i = 0; i < record_count; i++)
    {
        int bucket_size = game_news_record->get_bucket_size(i, 0);
        int sec_bucket_size = game_news_record->get_bucket_size(i, 1);
        MCPGameInfo* game_record = (MCPGameInfo*)game_news_record->get_record_value(i, 1, 0);

        LOGA("[GWJ] %s: [Compound-NetGameActivity] news_bucket size[%d], game_bucket_size[%d]",
             __FUNCTION__, bucket_size, sec_bucket_size);

        if(bucket_size > 0)
        {
            for(int x = 0; x < bucket_size; x++, j++)
            {
                MCPNewsInfo* news_record = (MCPNewsInfo*)game_news_record->get_record_value(i, 0, x);
                const char* news_id = news_record->id.c_str();

                int index = CommonInterface::find_key_index(news_id, netgame_record->news_id);

                if(index < 0)
                {
                    j--;

                    LOGA("[GWJ] %s:  news_record[%s] not in NetGameActivity Not Insert. game_news_list_index[%d]!",
                         __FUNCTION__, news_id, j);
                    continue;
                }
                else
                {
                    LOGA("[GWJ] %s:  news_record[%s] Valid and Insert. game_news_list_index[%d]!",
                          __FUNCTION__, news_id, j);

                    game_news_info_param::set_outparameter(out_parameter->mutable_game_news_list(j), in,
                                                           news_record, game_record);
                }
            }
        }
    }
}
