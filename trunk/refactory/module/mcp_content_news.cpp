/*
 * mcp_content_news.cpp
 *
 *  Created on: 2013-5-10
 *      Author: guoweijiang
 */

#include "mcp_content_news.h"

const char* MCPNewsInfo::ClassName = "MCPNewsInfo";

MCPContentNews::MCPContentNews(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    // 该子类专门处理 mongoDB 中的 mcp.mcp_content_news 表
    this->_basic_coll = "mcp_content_news";
    this->_basic_ns = MCP_MONGODB_NAME + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

MCPContentNews::~MCPContentNews()
{
    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPNewsInfo)
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

/*
 *  将从 mcp.mcp_content_news 表中查询到的一条 Record 保存到  id_s 和 result_set 中
 */
int MCPContentNews::set_basic_info_from_bson_result(bson* bson_out,
                                                    const char* aggregate_name)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPNewsInfo* detail = ObjectPoolFactory<MCPNewsInfo>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.id[%s]", __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "name"))
        {
            detail->name = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.name[%s]", __FUNCTION__, detail->name.c_str());
        }
        else if(0 == strcmp(key, "content"))
        {
            detail->content = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.content[%s]", __FUNCTION__, detail->content.c_str());
        }
        else if(0 == strcmp(key, "type_id"))
        {
            detail->type_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.type_id[%s]", __FUNCTION__, detail->type_id.c_str());
        }
        else if(0 == strcmp(key, "logdate"))
        {
            detail->logdate = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.logdate[%s]", __FUNCTION__, detail->logdate.c_str());
        }
        else if(0 == strcmp(key, "c_id"))
        {
            detail->c_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.c_id[%s]", __FUNCTION__, detail->c_id.c_str());
        }
        else if(0 == strcmp(key, "game_type"))
        {
            detail->game_type = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.game_type[%s]", __FUNCTION__, detail->game_type.c_str());
        }
		else if(0 == strcmp(key, "news_source"))
        {
            detail->news_source = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPNewsInfo.game_type[%s]", __FUNCTION__, detail->news_source.c_str());
        }

        else
        {
            LOGG("[GWJ] %s: Unknown KEY[%s]",
                    __FUNCTION__, key);
        }

    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        FILL_RESULT_BUCKET(detail, id)
    }
    if(IS_SAME_VALUED_STRING(this->key_name, "c_id"))
    {
        FILL_RESULT_BUCKET(detail, c_id)
    }
    else
    {
        FILL_RESULT_BUCKET(detail, id)
    }

    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
