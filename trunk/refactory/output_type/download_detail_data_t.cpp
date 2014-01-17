/*
 * download_data_t.cpp
 *
 *  Created on: 2013-6-14
 *      Author: guoweijiang
 */

#include "download_detail_data_t.h"

void download_detail_data_t::set_outparameter(idl::download_detail_t* out,
                                              MCPContentDataInfo* download_info)
{

    LOGD("[GWJ] %s:  start. download_info[%x]",
          __FUNCTION__, download_info);

    if(download_info != NULL)
    {
        out->set_id(download_info->c_id.c_str());
        out->set_url(download_info->path_url.c_str());
        out->set_baidu_url(download_info->baidu_path_url.c_str());
        out->set_url_type(download_info->path_type.c_str());
        out->set_media_format(download_info->media_format_id.c_str());
        out->set_size(download_info->size.c_str());
        out->set_mtype(download_info->mtype.c_str());
        out->set_charge_id(download_info->charge_typeid.c_str());
        out->set_charge_value(download_info->value.c_str());
        out->set_channel_info(download_info->channel_info.c_str());
        out->set_channel_id(download_info->channel_id.c_str());
        out->set_package_name(download_info->package_name.c_str());
        out->set_int_version(download_info->int_version.c_str());
        out->set_string_version(download_info->string_version.c_str());
        out->set_sdk_version(download_info->sdk_version.c_str());
        out->set_apk_md5(download_info->apk_md5.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: record NULL Empty!", __FUNCTION__);
    }
}
