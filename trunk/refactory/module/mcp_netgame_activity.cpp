#include "mcp_netgame_activity.h"

const char* MCPNetGameActivityInfoRecord::ClassName = "MCPNetGameActivityInfoRecord";

MCPNetGameActivityInfo::MCPNetGameActivityInfo(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);
    this->_basic_coll = "mcp_netgame_activity";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

MCPNetGameActivityInfo::~MCPNetGameActivityInfo()
{
    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPNetGameActivityInfoRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

int MCPNetGameActivityInfo::set_basic_info_from_bson_result(bson* bson_out,
                                                            const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPNetGameActivityInfoRecord* detail = ObjectPoolFactory<MCPNetGameActivityInfoRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "img_info"))
        {
            this->set_tuple_data_from_bson(it,
                                           (void*)&detail->img_info,
                                           true,
                                           "img_type",         // KEY
                                           "img_url");   // VALUE


            LOGG("[GWJ] %s: set MCPNetGameActivityInfoRecord.img_info[size:%hu]",
                     __FUNCTION__, detail->img_info.size());
        }
        else if(0 == strcmp(key, "content"))
        {
            detail->content = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPNetGameActivityInfoRecord.content[%s]",
                     __FUNCTION__, detail->content.c_str());
        }
        else if(0 == strcmp(key, "logdate"))
        {
            detail->logdate = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPNetGameActivityInfoRecord.logdate[%s]",
                     __FUNCTION__, detail->logdate.c_str());
        }
        else if(0 == strcmp(key, "channel"))
        {
            detail->channel = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPNetGameActivityInfoRecord.channel[%s]",
                     __FUNCTION__, detail->channel.c_str());
        }
        else if(0 == strcmp(key, "news_id"))
        {
            this->set_vector_data_from_bson(it, detail->news_id);


            LOGG("[GWJ] %s: set MCPNetGameActivityInfoRecord.news_id[size:%hu]",
                     __FUNCTION__, detail->news_id.size());
        }
        else if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPNetGameActivityInfoRecord.id[%s]",
                     __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "title"))
        {
            detail->title = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPNetGameActivityInfoRecord.title[%s]",
                     __FUNCTION__, detail->title.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        FILL_RESULT_BUCKET(detail, id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "content"))
    {
        FILL_RESULT_BUCKET(detail, content)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "channel"))
    {
        FILL_RESULT_BUCKET(detail, channel)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "title"))
    {
        FILL_RESULT_BUCKET(detail, title)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
