/**
 * @file game_update_download_info.h
 * @brief 输出基本信息、下载信息、更新信息
 * @author zhengxie
 * @version 1.0
 * @date 2013-08-03
 */

#include "game_update_download_info.h"

void game_update_info_param::set_outparameter(idl::game_update_download_info* out,
                                 content_input_params& in,
                                 MCPContentData& download_module,
                                 MCPGameContent& game_basic_module,
								 MCPDiffUpdateInfo& mcp_diff_update_module,
                                 int index)
{
    MCPGameInfo* game_record = (MCPGameInfo*)game_basic_module.get_record_by_key_vec(index, 0);
    //MCPContentDataInfo* data_record = (MCPContentDataInfo*)download_module.get_record_by_key_vec(index, 0);
	MCPDiffUpdateInfoRecord* diff_record = (MCPDiffUpdateInfoRecord*)mcp_diff_update_module.get_record_by_key_vec(index, 0);
    if(game_record == NULL)
    {
        LOGD("[GWJ] %s: game_record NULL", __FUNCTION__);
        return;
    }
    else
    {
		/* 基本信息 beg*/

        const char* game_id = game_record->id.c_str();
        LOGD("[GWJ] %s: game_record id[%s]", __FUNCTION__, game_id);
        game_basic_info_param::set_outparameter(out->mutable_basic_info(), in, game_record);

		/* 基本信息 end*/


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
                download_detail_data_t::set_outparameter(out->mutable_download_info(i),
                                                         (MCPContentDataInfo*)(*download_bucket)[i]);
            }
        }

		/* 下载信息 beg */


		/* 增量更新 beg*/

		if(diff_record == NULL)
        {
            LOGD("[GWJ] %s: No Diff Info in MCPDiffUpdateInfo", __FUNCTION__);
        }
		else
		{
			const char* old_file_md5 = diff_record->old_file_md5.c_str();
			LOGD("[GWJ] %s: diff_record old_file_md5[%s]", __FUNCTION__, old_file_md5);

			KeyBucket* diff_bucket = mcp_diff_update_module.get_key_bucket(old_file_md5);

			if(diff_bucket == NULL)
			{
				LOGD("[GWJ] %s: No Diff Info by old_file_md5", __FUNCTION__);
			}
			else
			{
				int diff_size = diff_bucket->size();
				LOGD("[GWJ] %s: diff_size[%d]", __FUNCTION__, diff_size);

				for(int i = 0; i < diff_size; i++) // 正常情况下，old_file_md5只可能对应一条记录
				{
					diff_detail_data_t::set_diff_data_out(out->mutable_update_info(i),(MCPDiffUpdateInfoRecord*)(*diff_bucket)[i]);
				}
			}
		}

		/* 增量更新 end*/

    }
}
