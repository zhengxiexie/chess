/**
 * @file championship_basic_download_info.cpp
 * @brief 输出争霸信息、部分下载信息和更新信息
 * @author zhengxie
 * @version 1.0
 * @date 2013-08-03
 */

#include "championship_basic_download_info.h"

void championship_basic_download_info_param::set_outparameter(idl::championship_info_t* out,
												 MCPContentData& download_module,
												 MCPGameContent& game_basic_module,
												 MCPContentChampionship& mcp_content_championship_module,
												 int champion_index, int index)
{
    MCPGameInfo* game_record = (MCPGameInfo*)game_basic_module.get_record_by_key_vec(index, 0);
	MCPContentChampionshipRecord* champion_record = (MCPContentChampionshipRecord*)mcp_content_championship_module.get_record_by_key_vec(champion_index, 0);
	
	if(game_record == NULL)
    {
        LOGD("[GWJ] %s: game_record NULL", __FUNCTION__);
        return;
    }
	const char* game_id = game_record->id.c_str();

	/* 争霸信息 beg*/

	if(champion_record == NULL)
	{
		LOGD("[zx] %s: No Champion Info in mcp_content_championship_module", __FUNCTION__);
		return;
	}
	else
	{
		const char* name = champion_record->name.c_str();
		const char* log_date = champion_record->log_date.c_str();
		const char* channel_info = champion_record->channel_info.c_str();
		string start_time = (champion_record->start_time).substr(0,10);
		string end_time = (champion_record->end_time).substr(0,10);
		const char* join_num = champion_record->join_num.c_str();
		const char* award = champion_record->award.c_str();
		const char* rule = champion_record->rule.c_str();
		const char* total_coins = champion_record->total_coins.c_str();
		const char* pic_url = champion_record->pic_url.c_str();
		const char* enable = champion_record->enable.c_str();
		const char* top_award = champion_record->top_award.c_str();

		out->set_name(name);
		out->set_log_date(log_date);
		out->set_channel_info(channel_info);
		out->set_start_time(start_time.c_str());
		out->set_end_time(end_time.c_str());
		out->set_join_num(join_num);
		out->set_award(award);
		out->set_rule(rule);
		out->set_total_coins(total_coins);
		out->set_pic_url(pic_url);
		out->set_enable(enable);
		out->set_top_award(top_award);
	}

	/* 争霸信息 end*/

	/* 基本信息 beg*/

	const char* game_name = game_record->name.c_str();
	const char* union_start_action = game_record->union_start_action.c_str();
	const char* has_account = game_record->has_account.c_str();

	LOGD("[GWJ] %s: game_record name[%s]", __FUNCTION__, game_name);
	out->set_game_name(game_name);
	out->set_union_start_action(union_start_action);
	out->set_has_account(has_account);
	out->set_c_id(game_id);


	/* 基本信息 end*/

	/* 下载信息 beg */

	/*KeyBucket* download_bucket = download_module.get_key_bucket(game_id);*/

	//if(download_bucket == NULL)
	//{
		//LOGD("[GWJ] %s: No DownLoad Info in [%s]", __FUNCTION__, game_id);
	//}
	//else
	//{
		//int download_size = download_bucket->size();
		//LOGD("[GWJ] %s: download_size[%d]", __FUNCTION__, download_size);

		//for(int i = 0; i < download_size; i++)
		//{
			//out->mutable_championship_download_list(i)->set_package_name( ((MCPContentDataInfo*)(*download_bucket)[i])->package_name.c_str() );
			//out->mutable_championship_download_list(i)->set_version_code( ((MCPContentDataInfo*)(*download_bucket)[i])->int_version.c_str() );
			//out->mutable_championship_download_list(i)->set_download_url( ((MCPContentDataInfo*)(*download_bucket)[i])->path_url.c_str() );
			//out->mutable_championship_download_list(i)->set_channel_info( ((MCPContentDataInfo*)(*download_bucket)[i])->channel_info.c_str() );
		//}
	//}

	/* 下载信息 beg */

	/* 下载信息 beg */

        KeyBucket* download_bucket = download_module.get_key_bucket(game_id);

        if(download_bucket == NULL)
        {
            LOGD("[GWJ] %s: No DownLoad Info in [%s]", __FUNCTION__, game_id);
        }
        else
        {
            int download_size = download_bucket->size();
            LOGD("[GWJ] %s: download_size[%d]", __FUNCTION__, download_size);

            for(int i = 0; i < download_size; i++)
            {
                download_detail_data_t::set_outparameter(out->mutable_championship_download_list(i),
                                                         (MCPContentDataInfo*)(*download_bucket)[i]);
            }
        }

	/* 下载信息 beg */


}
