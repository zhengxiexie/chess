/*
 * mcp_game_content.cpp
 *
 *  Created on: 2013-4-25
 *      Author: guoweijiang
 */

#include "mcp_game_content.h"

using namespace std;
using namespace content;

const char* MCPGameInfo::ClassName = "MCPGameInfo";

MCPGameContent::MCPGameContent(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    // 该子类专门处理 mongoDB 中的 mcp.mcp_content 表
    this->_basic_coll = "mcp_content";
    this->_basic_ns = MCP_MONGODB_NAME + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

MCPGameContent::~MCPGameContent()
{
    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPGameInfo)
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

/*
 *  将从 mcp.mcp_content 表中查询到的一条 Record 保存到  id_s 和 result_set 中
 */
int MCPGameContent::set_basic_info_from_bson_result(bson* bson_out,
                                                    const char* aggregate_name)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPGameInfo* detail = ObjectPoolFactory<MCPGameInfo>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.id[%s]",
                 __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "name"))
        {
            detail->name = string(bson_iterator_string(it));
            LOGG("[GWJ] %s: set MCPGameInfo.name[%s]",
                 __FUNCTION__, detail->name.c_str());
        }
        else if(0 == strcmp(key, "content_type"))
        {
            detail->content_type = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.content_type[%s]",
                 __FUNCTION__, detail->content_type.c_str());
        }
        else if(0 == strcmp(key, "class"))
        {
            if((aggregate_name == NULL || !IS_SAME_VALUED_STRING(aggregate_name, key)))
            {
                this->set_tuple_data_from_bson(it,
                                               (void*)&detail->classes,
                                               true,   // 使用的是数组
                                               "id",   // KEY
                                               "p");   // VALUE

                LOGG("[GWJ] %s: set MCPGameInfo.class[size:%hu]",
                     __FUNCTION__, detail->classes.size());
            }
        }
        else if(0 == strcmp(key, "logdate"))
        {
            detail->logdate = this->convert_mongo_ts_to_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.logdate[%s]",
                 __FUNCTION__, detail->logdate.c_str());
        }
        else if(0 == strcmp(key, "ext_id"))
        {
            detail->ext_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.ext_id[%s]",
                 __FUNCTION__, detail->ext_id.c_str());
        }
        else if(0 == strcmp(key, "info"))
        {
            detail->info = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.info[%s]",
                 __FUNCTION__, detail->info.c_str());
        }
        else if(0 == strcmp(key, "version"))
        {
            detail->version = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.version[%s]",
                 __FUNCTION__, detail->version.c_str());
        }
        else if(0 == strcmp(key, "method"))
        {
            detail->method = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.method[%s]",
                 __FUNCTION__, detail->method.c_str());
        }
        else if(0 == strcmp(key, "star"))
        {
            ss.clear();
            ss << bson_iterator_double(it);
            ss >> detail->star;
            LOGG("[GWJ] %s: set MCPGameInfo.star[%s]",
                 __FUNCTION__, detail->star.c_str());
        }
        else if(0 == strcmp(key, "network_id"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->network_id;
            LOGG("[GWJ] %s: set MCPGameInfo.network_id[%s]",
                 __FUNCTION__, detail->network_id.c_str());
        }
        else if(0 == strcmp(key, "game_code"))
        {
            detail->game_code = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.game_code[%s]",
                 __FUNCTION__, detail->game_code.c_str());
        }
        else if(0 == strcmp(key, "cp_id"))
        {
            detail->cp_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.cp_id[%s]",
                 __FUNCTION__, detail->cp_id.c_str());
        }
   /*     else if(0 == strcmp(key, "cp_name"))*/
		//{
			//detail->cp_name = bson_iterator_string(it);
			//LOGG("[GWJ] %s: set MCPGameInfo.cp_name[%s]",
				 //__FUNCTION__, detail->cp_name.c_str());
		/*}*/

        else if(0 == strcmp(key, "grant_id"))
        {
            detail->grant_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.grant_id[%s]",
                 __FUNCTION__, detail->grant_id.c_str());
        }
        else if(0 == strcmp(key, "priority"))
        {
            detail->priority = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPGameInfo.priority[%s]", __FUNCTION__, detail->priority.c_str());
        }
        else if(0 == strcmp(key, "default_charge_typeid"))
        {
            detail->default_charge_typeid = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.default_charge_typeid[%s]",
                 __FUNCTION__, detail->default_charge_typeid.c_str());
        }
        else if(0 == strcmp(key, "default_charge_value"))
        {
            detail->default_charge_value = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.default_charge_value[%s]",
                 __FUNCTION__, detail->default_charge_value.c_str());
        }
        else if(0 == strcmp(key, "modify_date"))
        {
            detail->modify_date = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.modify_date[%s]",
                 __FUNCTION__, detail->modify_date.c_str());
        }
        else if(0 == strcmp(key, "package_name"))
        {
            detail->package_name = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.package_name[%s]",
                 __FUNCTION__, detail->package_name.c_str());
        }
        else if(0 == strcmp(key, "version_appinfo"))
        {
            detail->version_appinfo = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.version_appinfo[%s]",
                 __FUNCTION__, detail->version_appinfo.c_str());
        }
        else if(0 == strcmp(key, "sdk_version"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->sdk_version;

            LOGG("[GWJ] %s: set MCPGameInfo.sdk_version[%s]",
                 __FUNCTION__, detail->sdk_version.c_str());
        }
        else if(0 == strcmp(key, "version_code"))
        {
            detail->version_code = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.version_code[%s]",
                 __FUNCTION__, detail->version_code.c_str());
        }
        else if(0 == strcmp(key, "ptype_1"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->ptype_1;

            LOGG("[GWJ] %s: set MCPGameInfo.ptype_1[%s]",
                  __FUNCTION__, detail->ptype_1.c_str());
        }
        else if(0 == strcmp(key, "ptype_2"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->ptype_2;

            LOGG("[GWJ] %s: set MCPGameInfo.ptype_2[%s]",
                  __FUNCTION__, detail->ptype_2.c_str());
        }
        else if(0 == strcmp(key, "ptype_3"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->ptype_3;

            LOGG("[GWJ] %s: set MCPGameInfo.ptype_3[%s]",
                  __FUNCTION__, detail->ptype_3.c_str());
        }
        else if(0 == strcmp(key, "ptype_4"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->ptype_4;

            LOGG("[GWJ] %s: set MCPGameInfo.ptype_4[%s]",
                  __FUNCTION__, detail->ptype_4.c_str());
        }
        else if(0 == strcmp(key, "ptype_5"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->ptype_5;

            LOGG("[GWJ] %s: set MCPGameInfo.ptype_5[%s]",
                  __FUNCTION__, detail->ptype_5.c_str());
        }
        else if(0 == strcmp(key, "label_id"))
        {
            this->set_vector_data_from_bson(it, detail->label_ids);

            LOGG("[GWJ] %s: set MCPGameInfo.label_id[size:%hu]",
                  __FUNCTION__, detail->label_ids.size());
        }
        else if(0 == strcmp(key, "column"))
        {
            if((aggregate_name == NULL || !IS_SAME_VALUED_STRING(aggregate_name, key)))
            {
                this->set_tuple_data_from_bson(it,
                                               (void*)&detail->column,
                                               true,       // 使用的是数组
                                               "id",       // KEY
                                               "p");       // VALUE

                LOGG("[GWJ] %s: set MCPGameInfo.column[size:%hu]",
                     __FUNCTION__, detail->column.size());
            }
        }
        else if(0 == strcmp(key, "img_info"))
        {
            this->set_tuple_data_from_bson(it,
                                           (void*)&detail->img_info,
                                           true,             // 使用的是数组
                                           "img_type",       // KEY
                                           "img_url");       // VALUE

            LOGG("[GWJ] %s: set MCPGameInfo.img_info[size:%hu]",
                 __FUNCTION__, detail->img_info.size());
        }
		
		//add by zhengxie
		else if(0 == strcmp(key, "pub_prop_infos"))
        {
            /*this->set_tuple_data_from_bson(it,*/
                                           //(void*)&detail->pub_prop_info,
                                           //true,             // 使用的是数组
                                           //"name",       // KEY
			 vector<const char*> key_vector;
			 key_vector.push_back("name");
			 key_vector.push_back("prop_url");
			 key_vector.push_back("color");
			 this->set_struct_data_from_bson(it,
										   (vector<MCPStructItem>*)&detail->pub_prop_info,
										   key_vector);                                    /*"color");       // VALUE*/

            LOGG("[GWJ] %s: set MCPGameInfo.pub_prop_info[size:%d]",
                 __FUNCTION__, detail->pub_prop_info.size());
        }

        else if(0 == strcmp(key, "enName"))
        {
            detail->enName = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.enName[%s]",
                 __FUNCTION__, detail->enName.c_str());
        }
        else if(0 == strcmp(key, "language"))
        {
            detail->language = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.language[%s]",
                 __FUNCTION__, detail->language.c_str());
        }
        else if(0 == strcmp(key, "adapt"))
        {
            detail->adapt = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.adapt[%s]",
                 __FUNCTION__, detail->adapt.c_str());
        }
        else if(0 == strcmp(key, "class_weight"))
        {
            detail->class_weight = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.class_weight[%s]",
                 __FUNCTION__, detail->class_weight.c_str());
        }
        else if(0 == strcmp(key, "latest_weight"))
        {
            detail->latest_weight = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.latest_weight[%s]",
                 __FUNCTION__, detail->latest_weight.c_str());
        }
        else if(0 == strcmp(key, "download_weight"))
        {
            detail->download_weight = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.download_weight[%s]",
                 __FUNCTION__, detail->download_weight.c_str());
        }
        else if(0 == strcmp(key, "pop_weight"))
        {
            detail->pop_weight = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.pop_weight[%s]",
                 __FUNCTION__, detail->pop_weight.c_str());
        }
        else if(0 == strcmp(key, "udate"))
        {
            detail->udate = this->convert_mongo_ts_to_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.udate[%s]",
                 __FUNCTION__, detail->udate.c_str());
        }
        else if(0 == strcmp(key, "pub_prop_ids"))
        {
            detail->pub_prop_ids = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.pub_prop_ids[%s]",
                  __FUNCTION__, detail->pub_prop_ids.c_str());
        }
        else if(0 == strcmp(key, "client_class_weight"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->client_class_weight;

            LOGG("[GWJ] %s: set MCPGameInfo.client_class_weight[%s]",
                  __FUNCTION__, detail->client_class_weight.c_str());
        }
        else if(0 == strcmp(key, "client_latest_weight"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->client_latest_weight;

            LOGG("[GWJ] %s: set MCPGameInfo.client_latest_weight[%s]",
                  __FUNCTION__, detail->client_latest_weight.c_str());
        }
        else if(0 == strcmp(key, "client_download_weight"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->client_download_weight;

            LOGG("[GWJ] %s: set MCPGameInfo.client_download_weight[%s]",
                  __FUNCTION__, detail->client_download_weight.c_str());
        }
        else if(0 == strcmp(key, "client_pop_weight"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->client_pop_weight;

            LOGG("[GWJ] %s: set MCPGameInfo.client_pop_weight[%s]",
                  __FUNCTION__, detail->client_pop_weight.c_str());
        }
        else if(0 == strcmp(key, "union_start_action"))
        {
            detail->union_start_action = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.union_start_action[%s]",
                  __FUNCTION__, detail->union_start_action.c_str());
        }
        else if(0 == strcmp(key, "has_account"))
        {
            detail->has_account = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.has_account[%s]",
                  __FUNCTION__, detail->has_account.c_str());
        }
        else if(0 == strcmp(key, "apk_md5"))
        {
            detail->apk_md5 = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.apk_md5[%s]",
                  __FUNCTION__, detail->apk_md5.c_str());
        }
		//add by zhengxie
		else if(0 == strcmp(key, "editor_recom"))
        {
            detail->editor_recom = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.editor_recom[%s]",
                  __FUNCTION__, detail->editor_recom.c_str());
        }
		else if(0 == strcmp(key, "show_highspeed"))
        {
            detail->show_highspeed = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.show_highspeed[%s]",
                  __FUNCTION__, detail->show_highspeed.c_str());
        }
		else if(0 == strcmp(key, "bind_resource_id"))
        {
            detail->bind_resource_id = bson_iterator_string(it);

            LOGG("[GWJ] %s: set MCPGameInfo.bind_resource_id[%s]",
                  __FUNCTION__, detail->bind_resource_id.c_str());
        }

        else
        {
            LOGG("[GWJ] %s: Unknown KEY[%s]",
                 __FUNCTION__, key);
        }

    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        // 以 detail->id 作为索引将查询到的数据存储到 result_set 和 id_s 中
        FILL_RESULT_BUCKET(detail, id)
    }
    else
    {
        FILL_RESULT_BUCKET(detail, id)
    }

    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}


