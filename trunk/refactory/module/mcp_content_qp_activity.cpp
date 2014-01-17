#include "mcp_content_qp_activity.h"

const char* MCPContentQPRecord::ClassName = "MCPContentQPRecord";

MCPContentQP::MCPContentQP(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->_basic_coll = "mcp_content_qp_activity";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);

}

MCPContentQP::~MCPContentQP()
{

    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPContentQPRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

int MCPContentQP::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPContentQPRecord* detail = ObjectPoolFactory<MCPContentQPRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "info"))
        {
            detail->info = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.info[%s]",
                     __FUNCTION__, detail->info.c_str());
        }
        else if(0 == strcmp(key, "icon_url"))
        {
            detail->icon_url = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.icon_url[%s]",
                     __FUNCTION__, detail->icon_url.c_str());
        }

        else if(0 == strcmp(key, "status"))
        {
            detail->status = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.status[%s]",
                     __FUNCTION__, detail->status.c_str());
        }
        else if(0 == strcmp(key, "start_time"))
        {
            detail->start_time = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.start_time[%s]",
                     __FUNCTION__, detail->start_time.c_str());
        }
        else if(0 == strcmp(key, "rules"))
        {
            detail->rules = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.rules[%s]",
                     __FUNCTION__, detail->rules.c_str());
        }
        else if(0 == strcmp(key, "end_time"))
        {
            detail->end_time = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.end_time[%s]",
                     __FUNCTION__, detail->end_time.c_str());
        }
        else if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.id[%s]",
                     __FUNCTION__, detail->id.c_str());
        }
        else if(0 == strcmp(key, "title"))
        {
            detail->title = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.title[%s]",
                     __FUNCTION__, detail->title.c_str());
        }
        else if(0 == strcmp(key, "cid"))
        {
            detail->c_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.c_id[%s]",
                     __FUNCTION__, detail->c_id.c_str());
        }
		else if(0 == strcmp(key, "act_info"))
        {
            detail->act_info = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentQPRecord.act_info[%s]",
                     __FUNCTION__, detail->act_info.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "info"))
    {
        FILL_RESULT_BUCKET(detail, info)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "icon_url"))
    {
        FILL_RESULT_BUCKET(detail, icon_url)
    }

    else if(IS_SAME_VALUED_STRING(this->key_name, "status"))
    {
        FILL_RESULT_BUCKET(detail, status)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "start_time"))
    {
        FILL_RESULT_BUCKET(detail, start_time)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "rules"))
    {
        FILL_RESULT_BUCKET(detail, rules)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "end_time"))
    {
        FILL_RESULT_BUCKET(detail, end_time)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        FILL_RESULT_BUCKET(detail, id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "title"))
    {
        FILL_RESULT_BUCKET(detail, title)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "c_id"))
    {
        FILL_RESULT_BUCKET(detail, c_id)
    }
	else if(IS_SAME_VALUED_STRING(this->key_name, "act_info"))
    {
        FILL_RESULT_BUCKET(detail, act_info)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
