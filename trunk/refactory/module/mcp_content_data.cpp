/*
 * mcp_content_data.cpp
 *
 *  专门负责操作  MongoDB 中  mcp.mcp_content_data 表的类
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#include "mcp_content_data.h"

const char* MCPContentDataInfo::ClassName = "MCPContentDataInfo";

MCPContentData::MCPContentData(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    // 该子类专门处理 mongoDB 中的 mcp.mcp_content_data 表
    this->_basic_coll = MCP_MONGO_DATA_COL;
    this->_basic_ns = MCP_MONGODB_NAME + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

MCPContentData::~MCPContentData()
{
    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPContentDataInfo)
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

/*
 *  将从 mcp.mcp_content_data 表中查询到的一条 Record 保存到  id_s 和 result_set 中
 */
int MCPContentData::set_basic_info_from_bson_result(bson* bson_out,
                                                    const char* aggregate_name)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPContentDataInfo* detail = ObjectPoolFactory<MCPContentDataInfo>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.id[%s]", __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "path_url"))
        {
            detail->path_url = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.path_url[%s]", __FUNCTION__, detail->path_url.c_str());
        }
        else if(0 == strcmp(key, "baidu_path_url"))
        {
            detail->baidu_path_url = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.baidu_path_url[%s]", __FUNCTION__, detail->baidu_path_url.c_str());
        }
        else if(0 == strcmp(key, "path_type"))
        {
            detail->path_type = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.path_type[%s]", __FUNCTION__, detail->path_type.c_str());
        }
        else if(0 == strcmp(key, "media_format_id"))
        {
            detail->media_format_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.media_format_id[%s]", __FUNCTION__, detail->media_format_id.c_str());
        }
        else if(0 == strcmp(key, "size"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->size;
            LOGG("[GWJ] %s: set MCPContentDataInfo.size[%s]", __FUNCTION__, detail->size.c_str());
        }
        else if(0 == strcmp(key, "mtype"))
        {
            detail->mtype = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.mtype[%s]", __FUNCTION__, detail->mtype.c_str());
        }
        else if(0 == strcmp(key, "charge_typeid"))
        {
            detail->charge_typeid = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.charge_typeid[%s]", __FUNCTION__, detail->charge_typeid.c_str());
        }
        else if(0 == strcmp(key, "c_id"))
        {
            detail->c_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.c_id[%s]", __FUNCTION__, detail->c_id.c_str());
        }
        else if(0 == strcmp(key, "value"))
        {
            ss.clear();
            ss << bson_iterator_int(it);
            ss >> detail->value;
            LOGG("[GWJ] %s: set MCPContentDataInfo.value[%s]", __FUNCTION__, detail->value.c_str());
        }
        else if(0 == strcmp(key, "modify_date"))
        {
            detail->modify_date = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.modify_date[%s]", __FUNCTION__, detail->modify_date.c_str());
        }
        else if(0 == strcmp(key, "adapt_info"))
        {
            this->set_tuple_data_from_bson_integer_value(it,
                                                         (void*)&detail->adapt_info,
                                                         true,             // 使用的是数组
                                                         "stype_id",       // KEY
                                                         "stype");         // VALUE

            LOGG("[GWJ] %s: set MCPContentDataInfo.adapt_info[size:%hu]",
                             __FUNCTION__, detail->adapt_info.size());
        }
        else if(0 == strcmp(key, "channel_info"))
        {
            detail->channel_info = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.channel_info[%s]", __FUNCTION__, detail->channel_info.c_str());
        }
        else if(0 == strcmp(key, "channel_id"))
        {
            detail->channel_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.channel_id[%s]", __FUNCTION__, detail->channel_id.c_str());
        }
        else if(0 == strcmp(key, "package_name"))
        {
            detail->package_name = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.package_name[%s]", __FUNCTION__, detail->package_name.c_str());
        }
        else if(0 == strcmp(key, "int_version"))
        {
            detail->int_version = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.int_version[%s]", __FUNCTION__, detail->int_version.c_str());
        }
        else if(0 == strcmp(key, "string_version"))
        {
            detail->string_version = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.string_version[%s]", __FUNCTION__, detail->string_version.c_str());
        }
        else if(0 == strcmp(key, "sdk_version"))
        {
            detail->sdk_version = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.sdk_version[%s]", __FUNCTION__, detail->sdk_version.c_str());
        }
        else if(0 == strcmp(key, "apk_md5"))
        {
            detail->apk_md5 = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.apk_md5[%s]", __FUNCTION__, detail->apk_md5.c_str());
        }
        else if(0 == strcmp(key, "file_md5"))
        {
            detail->file_md5 = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentDataInfo.apk_md5[%s]", __FUNCTION__, detail->file_md5.c_str());
        }
        else
        {
            LOGG("[GWJ] %s: Unknown KEY[%s]",
                 __FUNCTION__, key);
        }

    }// while


    if(IS_SAME_VALUED_STRING(this->key_name, "c_id"))
    {
        // 以 detail->c_id 作为索引将查询到的数据存储到 result_set 和 id_s 中
        FILL_RESULT_BUCKET(detail,c_id)
    }
    else
    {
        FILL_RESULT_BUCKET(detail,c_id)
    }

    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
