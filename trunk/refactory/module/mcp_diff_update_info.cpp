#include "mcp_diff_update_info.h"

const char* MCPDiffUpdateInfoRecord::ClassName = "MCPDiffUpdateInfoRecord";

MCPDiffUpdateInfo::MCPDiffUpdateInfo(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    this->_basic_coll = "mcp_diff_update_info";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);

}

MCPDiffUpdateInfo::~MCPDiffUpdateInfo()
{

    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPDiffUpdateInfoRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

int MCPDiffUpdateInfo::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPDiffUpdateInfoRecord* detail = ObjectPoolFactory<MCPDiffUpdateInfoRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "priority"))
        {
            detail->priority = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.priority[%s]",
                     __FUNCTION__, detail->priority.c_str());
        }
        else if(0 == strcmp(key, "channel_id"))
        {
            detail->channel_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.channel_id[%s]",
                     __FUNCTION__, detail->channel_id.c_str());
        }
        else if(0 == strcmp(key, "enable"))
        {
            detail->enable = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.enable[%s]",
                     __FUNCTION__, detail->enable.c_str());
        }
        else if(0 == strcmp(key, "new_cdata_id"))
        {
            detail->	new_cdata_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.	new_cdata_id[%s]",
                     __FUNCTION__, detail->	new_cdata_id.c_str());
        }
        else if(0 == strcmp(key, "old_cdata_id"))
        {
            detail->	old_cdata_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.	old_cdata_id[%s]",
                     __FUNCTION__, detail->	old_cdata_id.c_str());
        }
        else if(0 == strcmp(key, "channel_info"))
        {
            detail->channel_info = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.channel_info[%s]",
                     __FUNCTION__, detail->channel_info.c_str());
        }
        else if(0 == strcmp(key, "diff_url"))
        {
            detail->	diff_url = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.	diff_url[%s]",
                     __FUNCTION__, detail->	diff_url.c_str());
        }
        else if(0 == strcmp(key, "old_file_md5"))
        {
            detail->	old_file_md5 = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.	old_file_md5[%s]",
                     __FUNCTION__, detail->	old_file_md5.c_str());
        }
        else if(0 == strcmp(key, "diff_pkg_size"))
        {
            detail->diff_pkg_size = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.diff_pkg_size[%s]",
                     __FUNCTION__, detail->diff_pkg_size.c_str());
        }
        else if(0 == strcmp(key, "c_id"))
        {
            detail->c_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPDiffUpdateInfoRecord.c_id[%s]",
                     __FUNCTION__, detail->c_id.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "priority"))
    {
        FILL_RESULT_BUCKET(detail, priority)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "channel_id"))
    {
        FILL_RESULT_BUCKET(detail, channel_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "enable"))
    {
        FILL_RESULT_BUCKET(detail, enable)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "new_cdata_id"))
    {
        FILL_RESULT_BUCKET(detail, 	new_cdata_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "old_cdata_id"))
    {
        FILL_RESULT_BUCKET(detail, 	old_cdata_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "channel_info"))
    {
        FILL_RESULT_BUCKET(detail, channel_info)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "diff_url"))
    {
        FILL_RESULT_BUCKET(detail, 	diff_url)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "old_file_md5"))
    {
        FILL_RESULT_BUCKET(detail, 	old_file_md5)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "diff_pkg_size"))
    {
        FILL_RESULT_BUCKET(detail, diff_pkg_size)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "c_id"))
    {
        FILL_RESULT_BUCKET(detail, c_id)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
