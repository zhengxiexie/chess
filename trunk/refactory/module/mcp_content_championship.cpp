#include "mcp_content_championship.h"

const char* MCPContentChampionshipRecord::ClassName = "MCPContentChampionshipRecord";

MCPContentChampionship::MCPContentChampionship(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->_basic_coll = "mcp_content_championship";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);

}

MCPContentChampionship::~MCPContentChampionship()
{

    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPContentChampionshipRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

int MCPContentChampionship::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPContentChampionshipRecord* detail = ObjectPoolFactory<MCPContentChampionshipRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "enable"))
        {
            detail->enable = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.enable[%s]",
                     __FUNCTION__, detail->enable.c_str());
        }
        else if(0 == strcmp(key, "join_num"))
        {
            detail->join_num = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.join_num[%s]",
                     __FUNCTION__, detail->join_num.c_str());
        }
        else if(0 == strcmp(key, "start_time"))
        {
            detail->start_time = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.start_time[%s]",
                     __FUNCTION__, detail->start_time.c_str());
        }
        else if(0 == strcmp(key, "name"))
        {
            detail->name = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.name[%s]",
                     __FUNCTION__, detail->name.c_str());
        }
        else if(0 == strcmp(key, "rule"))
        {
            detail->rule = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.rule[%s]",
                     __FUNCTION__, detail->rule.c_str());
        }
        else if(0 == strcmp(key, "total_coins"))
        {
            detail->total_coins = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.total_coins[%s]",
                     __FUNCTION__, detail->total_coins.c_str());
        }
        else if(0 == strcmp(key, "award"))
        {
            detail->award = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.award[%s]",
                     __FUNCTION__, detail->award.c_str());
        }
        else if(0 == strcmp(key, "channel_info"))
        {
            detail->channel_info = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.channel_info[%s]",
                     __FUNCTION__, detail->channel_info.c_str());
        }
        else if(0 == strcmp(key, "pic_url"))
        {
            detail->pic_url = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.pic_url[%s]",
                     __FUNCTION__, detail->pic_url.c_str());
        }
        else if(0 == strcmp(key, "end_time"))
        {
            detail->end_time = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.end_time[%s]",
                     __FUNCTION__, detail->end_time.c_str());
        }
        else if(0 == strcmp(key, "id"))
        {
            detail->id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.	id[%s]",
                     __FUNCTION__, detail->	id.c_str());
        }
        else if(0 == strcmp(key, "log_date"))
        {
            detail->log_date = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.log_date[%s]",
                     __FUNCTION__, detail->log_date.c_str());
        }
        else if(0 == strcmp(key, "c_id"))
        {
            detail->c_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.c_id[%s]",
                     __FUNCTION__, detail->c_id.c_str());
        }
		else if(0 == strcmp(key, "top_award"))
        {
            detail->top_award = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentChampionshipRecord.top_award[%s]",
                     __FUNCTION__, detail->top_award.c_str());
        }

    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "enable"))
    {
        FILL_RESULT_BUCKET(detail, enable)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "join_num"))
    {
        FILL_RESULT_BUCKET(detail, join_num)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "start_time"))
    {
        FILL_RESULT_BUCKET(detail, start_time)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "name"))
    {
        FILL_RESULT_BUCKET(detail, name)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "rule"))
    {
        FILL_RESULT_BUCKET(detail, rule)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "total_coins"))
    {
        FILL_RESULT_BUCKET(detail, total_coins)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "award"))
    {
        FILL_RESULT_BUCKET(detail, award)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "channel_info"))
    {
        FILL_RESULT_BUCKET(detail, channel_info)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "pic_url"))
    {
        FILL_RESULT_BUCKET(detail, pic_url)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "end_time"))
    {
        FILL_RESULT_BUCKET(detail, end_time)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "id"))
    {
        FILL_RESULT_BUCKET(detail,id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "log_date"))
    {
        FILL_RESULT_BUCKET(detail,log_date)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "c_id"))
    {
        FILL_RESULT_BUCKET(detail,c_id)
    }
	else if(IS_SAME_VALUED_STRING(this->key_name, "top_award"))
    {
        FILL_RESULT_BUCKET(detail,top_award)
    }

    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
