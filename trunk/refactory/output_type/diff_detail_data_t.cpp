/**
 * @file diff_detail_data_t.h
 * @brief 将更新信息记录输出
 * @author zhengxie
 * @version 1.0
 * @date 2013-08-03
 */

#include "diff_detail_data_t.h"
void diff_detail_data_t::set_diff_data_out(idl::game_ext_update_info_t* out,
                                      MCPDiffUpdateInfoRecord* record)
{   
	if(record != NULL)
    {
		out->set_c_id(record->c_id.c_str());
        out->set_update_url(record->diff_url.c_str());
		out->set_diff_pkg_size(record->diff_pkg_size.c_str());
        out->set_channel_info(record->channel_info.c_str());
        out->set_channel_id(record->channel_id.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: record NULL Empty!", __FUNCTION__);
    }

}
