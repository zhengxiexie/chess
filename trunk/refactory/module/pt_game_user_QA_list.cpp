#include "pt_game_user_QA_list.h"

const char* PTGameUserQARecord::ClassName = "PTGameUserQARecord";

PTGameUserQandA::PTGameUserQandA(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->_basic_coll = "pt_game_user_QA_list";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);

}

PTGameUserQandA::~PTGameUserQandA()
{

    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(PTGameUserQARecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

int PTGameUserQandA::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    PTGameUserQARecord* detail = ObjectPoolFactory<PTGameUserQARecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "question_id"))
        {
            detail->question_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameUserQARecord.question_id[%s]",
                     __FUNCTION__, detail->question_id.c_str());
        }
        else if(0 == strcmp(key, "user_id"))
        {
            detail->user_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameUserQARecord.user_id[%s]",
                     __FUNCTION__, detail->user_id.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "question_id"))
    {
        FILL_RESULT_BUCKET(detail, question_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "user_id"))
    {
        FILL_RESULT_BUCKET(detail, user_id)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}