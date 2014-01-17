/*
 * game_basic_download_info.cpp
 *
 *  Created on: 2013-5-10
 *      Author: guoweijiang
 */

#include "game_basic_download_info.h"

void game_basic_download_t::set_outparameter(idl::content_detail_t* out,
                                             content_input_params& in,
                                             TwoBindedRecord* binded_record)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    if(binded_record == NULL || binded_record->get_primary_record(0) == NULL)
    {
        LOGD("[GWJ] %s: binded_record Or Basic Info is NULL", __FUNCTION__);
        return;
    }

    /*
     *  一般来说只去该索引下的第一条数据即可
     *  因为在设置游戏基本信息时一般只有一条数据对应一个索引
     */
    MCPGameInfo* record = (MCPGameInfo*)binded_record->get_primary_record(0);

    if(record != NULL)
    {
        out->set_id(record->id.c_str());
        out->set_name(record->name.c_str());
        out->set_type(record->content_type.c_str());
        out->set_ext_id(record->ext_id.c_str());
        out->set_abstract_(record->info.c_str());
        out->set_version(record->version.c_str());
        out->set_method(record->method.c_str());
        out->set_star(record->star.c_str());
        out->set_network(record->network_id.c_str());
        out->set_code(record->game_code.c_str());
        out->set_log_date(record->logdate.c_str());
        out->set_mod_date(record->modify_date.c_str());
        out->set_dft_charge_type(record->default_charge_typeid.c_str());
        out->set_dft_charge_val(record->default_charge_value.c_str());
        out->set_enName(record->enName.c_str());
        out->set_language(record->language.c_str());
        out->set_adapt(record->adapt.c_str());
        out->set_package_name(record->package_name.c_str());
        out->set_version_appinfo(record->version_appinfo.c_str());
        out->set_sdk_version(record->sdk_version.c_str());
        out->set_version_code(record->version_code.c_str());
        out->set_ptype_0(record->ptype_1.c_str());
        out->set_ptype_1(record->ptype_2.c_str());
        out->set_ptype_2(record->ptype_3.c_str());
        out->set_ptype_3(record->ptype_4.c_str());
        out->set_ptype_4(record->ptype_5.c_str());
        out->set_union_start_action(record->union_start_action.c_str());
        out->set_has_account(record->has_account.c_str());
		//add by zhengxie
        out->set_editor_recom(record->editor_recom.c_str());
        out->set_show_highspeed(record->show_highspeed.c_str());
		

        string cp_name;
        mcp_content_map::get_cp_name(record->cp_id, cp_name);
        out->mutable_cp_info()->set_name(cp_name.c_str());
        out->mutable_cp_info()->set_id(record->cp_id.c_str());

        grant_detail_t grant_info;
        mcp_content_map::get_grant_info(record->grant_id, grant_info);
        out->mutable_grant_info()->set_id(record->grant_id.c_str());
        out->mutable_grant_info()->set_modify_time(grant_info.modify_time.c_str());
        out->mutable_grant_info()->set_grant_time(grant_info.grant_time.c_str());
        out->mutable_grant_info()->set_valid_time(grant_info.valid_time.c_str());

        vector<MCPTupleItem>::const_iterator itr = record->classes.begin();
        vector<MCPTupleItem>::const_iterator end_itr = record->classes.end();

        for(int i = 0; itr != end_itr; itr++, i++)
        {
            class_info_t class_info;
            mcp_content_map::get_class_info((*itr).key, class_info);

            out->mutable_class_info(i)->set_name(class_info.name.c_str());
            out->mutable_class_info(i)->set_id((*itr).key.c_str());

            LOGD("[GWJ] %s: id[%s], class-id[%s]", __FUNCTION__, record->id.c_str(), (*itr).key.c_str());
        }

        itr = record->img_info.begin();
        end_itr = record->img_info.end();
        for(int i = 0; itr != end_itr; itr++)
        {
            const char* img_type = (*itr).key.c_str();

            int index = CommonInterface::find_key_index(img_type, in.img_types);

            if(index < 0)
            {
                LOGD("[GWJ] %s: No need img_type[%s]", __FUNCTION__, img_type);
            }
            else
            {
                LOGD("[GWJ] %s: need img_type[%s]", __FUNCTION__, img_type);
                out->mutable_img_info(i)->set_type(img_type);
                out->mutable_img_info(i)->set_url((*itr).value.c_str());
                i++;
            }
        }

        vector<string> pub_prop_vec;
        int p_size = CommonInterface::splic_string(record->pub_prop_ids, pub_prop_vec, ",");

        if(p_size > 0)
        {
            for(int i = 0; i < p_size; i++)
            {
                out->set_pub_props(i, pub_prop_vec[i].c_str());
            }
        }

        if(binded_record->get_secondary_record(0) == NULL)
        {
            LOGD("[GWJ] %s: DownLoad Info is NULL", __FUNCTION__);
            return;
        }

        int download_size = binded_record->get_secondary_record_size();

        for(int i = 0; i < download_size; i++)
        {
            MCPContentDataInfo* download_info =
                    (MCPContentDataInfo*)binded_record->get_secondary_record(i);

            download_detail_data_t::set_outparameter(out->mutable_download_info(i),
                                                     download_info);
        }

        CommonInterface::clear_vector_string(pub_prop_vec);
    }
    else
    {
        LOGD("[GWJ] %s: record NULL Empty!", __FUNCTION__);
    }

    LOGD("[GWJ] %s:  end", __FUNCTION__);
}
