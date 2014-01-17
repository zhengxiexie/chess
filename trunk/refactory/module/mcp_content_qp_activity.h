#ifndef mcp_content_qp_activity_H_
#define mcp_content_qp_activity_H_


#include "abstract_mcp_content.h"


class MCPContentQPRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string info;
    string icon_url;
    string status;
    string start_time;
    string rules;
    string end_time;
    string id;
    string title;
    string c_id;
    string act_info;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        info.clear();
        icon_url.clear();
        status.clear();
        start_time.clear();
        rules.clear();
        end_time.clear();
        id.clear();
        title.clear();
        c_id.clear();
		act_info.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPContentQPRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPContentQPRecord()
    {
        this->clear();
    }
};




class MCPContentQP : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    MCPContentQP(const char* key_name);
    virtual ~MCPContentQP();
};


#endif /* mcp_content_qp_activity_H_ */
