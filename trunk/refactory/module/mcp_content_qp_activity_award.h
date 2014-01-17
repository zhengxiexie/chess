#ifndef mcp_content_qp_activity_award_H_
#define mcp_content_qp_activity_award_H_


#include "abstract_mcp_content.h"


class MCPContentQPAwardRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string pic_url;
    string activity_id;
    string name;
    string id;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        pic_url.clear();
        activity_id.clear();
        name.clear();
        id.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPContentQPAwardRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPContentQPAwardRecord()
    {
        this->clear();
    }
};




class MCPContentQPAward : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    MCPContentQPAward(const char* key_name);
    virtual ~MCPContentQPAward();
};


#endif /* mcp_content_qp_activity_award_H_ */