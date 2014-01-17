#include "mcp_content_qp_activity_award.h"

const char* MCPContentQPAwardRecord::ClassName = "MCPContentQPAwardRecord";

MCPContentQPAward::MCPContentQPAward(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->_basic_coll = "mcp_content_qp_activity_award";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);

}

MCPContentQPAward::~MCPContentQPAward()
{

    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPContentQPAwardRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

int MCPContentQPAward::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPContentQPAwardRecord* detail = ObjectPoolFactory<MCPContentQPAwardRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "pic_url"))
        {
            detail->pic_url = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPAwardRecord.pic_url[%s]",
                     __FUNCTION__, detail->pic_url.c_str());
        }
        else if(0 == strcmp(key, "activity_id"))
        {
            detail->activity_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPAwardRecord.activity_id[%s]",
                     __FUNCTION__, detail->activity_id.c_str());
        }
        else if(0 == strcmp(key, "name"))
        {
            detail->name = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPAwardRecord.name[%s]",
                     __FUNCTION__, detail->name.c_str());
        }
        else if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPAwardRecord.id[%s]",
                     __FUNCTION__, detail->id.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "pic_url"))
    {
        FILL_RESULT_BUCKET(detail, pic_url)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "activity_id"))
    {
        FILL_RESULT_BUCKET(detail, activity_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "name"))
    {
        FILL_RESULT_BUCKET(detail, name)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        FILL_RESULT_BUCKET(detail, id)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}