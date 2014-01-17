#ifndef mcp_diff_update_info_H_
#define mcp_diff_update_info_H_


#include "abstract_mcp_content.h"


class MCPDiffUpdateInfoRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string priority;
    string channel_id;
    string enable;
    string new_cdata_id;
    string old_cdata_id;
    string channel_info;
    string diff_url;
    string old_file_md5;
    string diff_pkg_size;
    string c_id;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        priority.clear();
        channel_id.clear();
        enable.clear();
		new_cdata_id.clear();
		old_cdata_id.clear();
        channel_info.clear();
		diff_url.clear();
		old_file_md5.clear();
        diff_pkg_size.clear();
        c_id.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPDiffUpdateInfoRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPDiffUpdateInfoRecord()
    {
        this->clear();
    }
};




class MCPDiffUpdateInfo : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    MCPDiffUpdateInfo(const char* key_name);
    virtual ~MCPDiffUpdateInfo();
};


#endif /* mcp_diff_update_info_H_ */
