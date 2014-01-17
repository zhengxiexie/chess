#include "mcp_content_qp_activity_person.h"

const char* MCPContentQPPersonRecord::ClassName = "MCPContentQPPersonRecord";

MCPContentQPPerson::MCPContentQPPerson(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->_basic_coll = "mcp_content_qp_activity_person";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);

}

MCPContentQPPerson::~MCPContentQPPerson()
{

    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPContentQPPersonRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

int MCPContentQPPerson::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPContentQPPersonRecord* detail = ObjectPoolFactory<MCPContentQPPersonRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "num"))
        {
            detail->person_num = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPPersonRecord.person_num[%s]",
                     __FUNCTION__, detail->person_num.c_str());
        }
        else if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPPersonRecord.id[%s]",
                     __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "c_id"))
        {
            detail->c_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPPersonRecord.c_id[%s]",
                     __FUNCTION__, detail->c_id.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "person_num"))
    {
        FILL_RESULT_BUCKET(detail, person_num)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        FILL_RESULT_BUCKET(detail, id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "c_id"))
    {
        FILL_RESULT_BUCKET(detail, c_id)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
