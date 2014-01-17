#include "mcp_content_appinfo_data.h"

const char* MCPContentAppInfoDataRecord::ClassName = "MCPContentAppInfoDataRecord";

MCPContentAppInfoData::MCPContentAppInfoData(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);
    this->_basic_coll = "mcp_content_appinfo_data";
    this->_basic_ns = string("mcp") + "." + this->_basic_coll;
    LOGD("[GWJ] %s: end", __FUNCTION__);

}

MCPContentAppInfoData::~MCPContentAppInfoData()
{
    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPContentAppInfoDataRecord)
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

int MCPContentAppInfoData::set_basic_info_from_bson_result(bson* bson_out,
                                                           const char* aggregate_name)
{

    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPContentAppInfoDataRecord* detail = ObjectPoolFactory<MCPContentAppInfoDataRecord>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "sdk_version"))
        {
            detail->sdk_version = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.sdk_version[%s]",
                     __FUNCTION__, detail->sdk_version.c_str());
        }
        else if(0 == strcmp(key, "app_id"))
        {
            detail->app_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.app_id[%s]",
                     __FUNCTION__, detail->app_id.c_str());
        }
        else if(0 == strcmp(key, "size"))
        {
            detail->size = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.size[%s]",
                     __FUNCTION__, detail->size.c_str());
        }
        else if(0 == strcmp(key, "to_baidu_url"))
        {
            detail->to_baidu_url = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.to_baidu_url[%s]",
                     __FUNCTION__, detail->to_baidu_url.c_str());
        }
        else if(0 == strcmp(key, "cdata_id"))
        {
            detail->cdata_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.cdata_id[%s]",
                     __FUNCTION__, detail->cdata_id.c_str());
        }
        else if(0 == strcmp(key, "app_name"))
        {
            detail->app_name = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.app_name[%s]",
                     __FUNCTION__, detail->app_name.c_str());
        }
        else if(0 == strcmp(key, "MD5"))
        {
            detail->MD5 = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.MD5[%s]",
                     __FUNCTION__, detail->MD5.c_str());
        }
        else if(0 == strcmp(key, "version_upcode"))
        {
            detail->version_upcode = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.version_upcode[%s]",
                     __FUNCTION__, detail->version_upcode.c_str());
        }
        else if(0 == strcmp(key, "c_id"))
        {
            detail->c_id = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.c_id[%s]",
                     __FUNCTION__, detail->c_id.c_str());
        }
        else if(0 == strcmp(key, "logdate"))
        {
            detail->logdate = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.logdate[%s]",
                     __FUNCTION__, detail->logdate.c_str());
        }
        else if(0 == strcmp(key, "enable"))
        {
            detail->enable = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.enable[%s]",
                     __FUNCTION__, detail->enable.c_str());
        }
        else if(0 == strcmp(key, "version"))
        {
            detail->version = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.version[%s]",
                     __FUNCTION__, detail->version.c_str());
        }
        else if(0 == strcmp(key, "package_name"))
        {
            detail->package_name = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.package_name[%s]",
                     __FUNCTION__, detail->package_name.c_str());
        }
        else if(0 == strcmp(key, "channel_info"))
        {
            detail->channel_info = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.channel_info[%s]",
                     __FUNCTION__, detail->channel_info.c_str());
        }
        else if(0 == strcmp(key, "path_url"))
        {
            detail->path_url = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.path_url[%s]",
                     __FUNCTION__, detail->path_url.c_str());
        }
        else if(0 == strcmp(key, "path_type"))
        {
            detail->path_type = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.path_type[%s]",
                     __FUNCTION__, detail->path_type.c_str());
        }
        else if(0 == strcmp(key, "version_code"))
        {
            detail->version_code = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.version_code[%s]",
                     __FUNCTION__, detail->version_code.c_str());
        }
        else if(0 == strcmp(key, "modify_date"))
        {
            detail->modify_date = bson_iterator_string(it);


            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.modify_date[%s]",
                     __FUNCTION__, detail->modify_date.c_str());
        }
        else if(0 == strcmp(key, "apk_md5"))
        {
            detail->apk_md5 = bson_iterator_string(it);
            LOGG("[GWJ] %s: set MCPContentAppInfoDataRecord.apk_md5[%s]",
                     __FUNCTION__, detail->apk_md5.c_str());
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "c_id"))
    {
        FILL_RESULT_BUCKET(detail, c_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "cdata_id"))
    {
        FILL_RESULT_BUCKET(detail, cdata_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "app_id"))
    {
        FILL_RESULT_BUCKET(detail, app_id)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "app_name"))
    {
        FILL_RESULT_BUCKET(detail, app_name)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "sdk_version"))
    {
        FILL_RESULT_BUCKET(detail, sdk_version)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "size"))
    {
        FILL_RESULT_BUCKET(detail, size)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "to_baidu_url"))
    {
        FILL_RESULT_BUCKET(detail, to_baidu_url)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "MD5"))
    {
        FILL_RESULT_BUCKET(detail, MD5)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "version_upcode"))
    {
        FILL_RESULT_BUCKET(detail, version_upcode)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "logdate"))
    {
        FILL_RESULT_BUCKET(detail, logdate)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "enable"))
    {
        FILL_RESULT_BUCKET(detail, enable)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "version"))
    {
        FILL_RESULT_BUCKET(detail, version)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "package_name"))
    {
        FILL_RESULT_BUCKET(detail, package_name)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "channel_info"))
    {
        FILL_RESULT_BUCKET(detail, channel_info)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "path_url"))
    {
        FILL_RESULT_BUCKET(detail, path_url)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "path_type"))
    {
        FILL_RESULT_BUCKET(detail, path_type)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "version_code"))
    {
        FILL_RESULT_BUCKET(detail, version_code)
    }
    else if(IS_SAME_VALUED_STRING(this->key_name, "modify_date"))
    {
        FILL_RESULT_BUCKET(detail, modify_date)
    }
    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}
