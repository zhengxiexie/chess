#ifndef mcp_content_appinfo_data_H_
#define mcp_content_appinfo_data_H_


#include "abstract_mcp_content.h"


class MCPContentAppInfoDataRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string sdk_version;
    string app_id;
    string size;
    string to_baidu_url;
    string cdata_id;
    string app_name;
    string MD5;
    string version_upcode;
    string c_id;
    string logdate;
    string enable;
    string version;
    string package_name;
    string channel_info;
    string path_url;
    string path_type;
    string version_code;
    string modify_date;
    string apk_md5;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        sdk_version.clear();
        app_id.clear();
        size.clear();
        to_baidu_url.clear();
        cdata_id.clear();
        app_name.clear();
        MD5.clear();
        version_upcode.clear();
        c_id.clear();
        logdate.clear();
        enable.clear();
        version.clear();
        package_name.clear();
        channel_info.clear();
        path_url.clear();
        path_type.clear();
        version_code.clear();
        modify_date.clear();
        apk_md5.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPContentAppInfoDataRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPContentAppInfoDataRecord()
    {
        this->clear();
    }
};




class MCPContentAppInfoData : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out,
                                        const char* aggregate_name);


public:
    MCPContentAppInfoData(const char* key_name);
    virtual ~MCPContentAppInfoData();
};


#endif /* mcp_content_appinfo_data_H_ */
