#include "pt_game_QandA.h"

const char* PTGameQandARecord::ClassName = "PTGameQandARecord";

PTGameQandA::PTGameQandA(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->_basic_coll = "pt_game_QandA";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);

}

PTGameQandA::~PTGameQandA()
{

    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(PTGameQandARecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

int PTGameQandA::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    PTGameQandARecord* detail = ObjectPoolFactory<PTGameQandARecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "enable"))
        {
            detail->enable = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.enable[%s]",
                     __FUNCTION__, detail->enable.c_str());
        }
        else if(0 == strcmp(key, "answer"))
        {
            detail->answer = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.answer[%s]",
                     __FUNCTION__, detail->answer.c_str());
        }
        else if(0 == strcmp(key, "status"))
        {
            detail->status = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.status[%s]",
                     __FUNCTION__, detail->status.c_str());
        }
        else if(0 == strcmp(key, "appid"))
        {
            detail->app_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.app_id[%s]",
                     __FUNCTION__, detail->app_id.c_str());
        }
        else if(0 == strcmp(key, "question_type"))
        {
            detail->question_type = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.question_type[%s]",
                     __FUNCTION__, detail->question_type.c_str());
        }
        else if(0 == strcmp(key, "phone"))
        {
            detail->phone = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.phone[%s]",
                     __FUNCTION__, detail->phone.c_str());
        }
        else if(0 == strcmp(key, "question"))
        {
            detail->question = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.question[%s]",
                     __FUNCTION__, detail->question.c_str());
        }
        else if(0 == strcmp(key, "date_q"))
        {
            detail->date_q = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.date_q[%s]",
                     __FUNCTION__, detail->date_q.c_str());
        }
        else if(0 == strcmp(key, "remarks"))
        {
            detail->remarks = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.remarks[%s]",
                     __FUNCTION__, detail->remarks.c_str());
        }
        else if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.id[%s]",
                     __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "date_a"))
        {
            detail->date_a = bson_iterator_string(it);


            LOGG("[GWJ] %s: set PTGameQandARecord.date_a[%s]",
                     __FUNCTION__, detail->date_a.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "enable"))
    {
        FILL_RESULT_BUCKET(detail, enable)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "answer"))
    {
        FILL_RESULT_BUCKET(detail, answer)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "status"))
    {
        FILL_RESULT_BUCKET(detail, status)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "app_id"))
    {
        FILL_RESULT_BUCKET(detail, app_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "question_type"))
    {
        FILL_RESULT_BUCKET(detail, question_type)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "phone"))
    {
        FILL_RESULT_BUCKET(detail, phone)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "question"))
    {
        FILL_RESULT_BUCKET(detail, question)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "date_q"))
    {
        FILL_RESULT_BUCKET(detail, date_q)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "remarks"))
    {
        FILL_RESULT_BUCKET(detail, remarks)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        FILL_RESULT_BUCKET(detail, id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "date_a"))
    {
        FILL_RESULT_BUCKET(detail, date_a)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
