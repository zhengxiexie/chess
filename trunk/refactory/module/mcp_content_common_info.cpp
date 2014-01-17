/*
 * mcp_content_common_info.cpp
 *
 *  Created on: 2013-5-17
 *      Author: guoweijiang
 */

#include "mcp_content_common_info.h"

const char* MCPCommonAdvInfoRecord::ClassName = "MCPCommonAdvInfoRecord";

MCPContentCommonInfo::MCPContentCommonInfo(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    // 该子类专门处理 mongoDB 中的 mcp.mcp_content_common_info 表
    this->_basic_coll = "mcp_content_common_info";
    this->_basic_ns = MCP_MONGODB_NAME + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

MCPContentCommonInfo::~MCPContentCommonInfo()
{
    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPCommonAdvInfoRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

int MCPContentCommonInfo::set_basic_info_from_bson_result(bson* bson_out,
                                                          const char* aggregate_name)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];

    MCPCommonAdvInfoRecord* detail = ObjectPoolFactory<MCPCommonAdvInfoRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.id[%s]", __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "info"))
        {
            detail->info = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.name[%s]", __FUNCTION__, detail->info.c_str());
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
