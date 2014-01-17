/*
 * game_basic_info.cpp
 *
 *  Created on: 2013-6-13
 *      Author: guoweijiang
 */

#include "game_basic_info.h"

void game_basic_info_param::set_outparameter(idl::content_basic_info_t* out,
                                             content_input_params& in,
                                             MCPGameInfo* record)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    if(record == NULL)
    {
        LOGD("[GWJ] %s:  game_data is NULL Return!", __FUNCTION__);
        return ;
    }

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
    out->set_bind_resource_id(record->bind_resource_id.c_str());

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

    vector<MCPTupleItem>::const_iterator claz_itr = record->classes.begin();
    vector<MCPTupleItem>::const_iterator claz_end_itr = record->classes.end();

    for(int i = 0; claz_itr != claz_end_itr; claz_itr++, i++)
    {
        class_info_t class_info;
        mcp_content_map::get_class_info((*claz_itr).key, class_info);

        out->mutable_class_info(i)->set_name(class_info.name.c_str());
        out->mutable_class_info(i)->set_id((*claz_itr).key.c_str());

        LOGD("[GWJ] %s: id[%s], class-id[%s]", __FUNCTION__, record->id.c_str(), (*claz_itr).key.c_str());
    }

    vector <string> pub_prop_vec;
    int p_size = CommonInterface::splic_string(record->pub_prop_ids,
                                               pub_prop_vec, ",");

    if(p_size > 0)
    {
        for(int i = 0; i < p_size; i++)
        {
            out->set_pub_props(i, pub_prop_vec[i].c_str());

        }
    }

	//add by zhengxie
   //SET_OUTPUT_PUB_PROP_INFO(record->pub_prop_info, out->mutable_pub_prop_info)
   vector<MCPStructItem>::iterator it;
	LOGD("[zx] %s: vector size[%d]",__FUNCTION__, (record->pub_prop_info).size());

	int i = 0;
   for( it=(record->pub_prop_info).begin(); it != (record->pub_prop_info).end(); it++ )
   {
	   map<string, string>::iterator m_it;
		LOGD("[zx] %s: map size[%d]",__FUNCTION__, (it->record).size());
	   for( m_it=(it->record).begin(); m_it != (it->record).end(); m_it++ )
	   {
            string key = (*m_it).first;
            string value = (*m_it).second;
			//SET_OUTPUT_PUB_PROP_INFO_NEW(i, key, value)
			if( !strcmp(key.c_str(), "name") ) {
					out->mutable_pub_prop_info(i)->set_name(value.c_str());
			}
			else if( !strcmp(key.c_str(), "color") ) {
					out->mutable_pub_prop_info(i)->set_color(value.c_str());
			}
			else if( !strcmp(key.c_str(), "prop_url") ) {
					out->mutable_pub_prop_info(i)->set_prop_url(value.c_str());
			}
			LOGD("[zx] %s: set struct [%s %s]",__FUNCTION__, key.c_str(), value.c_str());
	   }
	   i++;
   }

    SET_OUTPUT_IMG_INFO(record->img_info, in.img_types, out->mutable_img_info)
    CommonInterface::clear_vector_string(pub_prop_vec);
}
