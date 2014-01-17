#ifndef mcp_content_qp_activity_person_H_
#define mcp_content_qp_activity_person_H_


#include "abstract_mcp_content.h"


class MCPContentQPPersonRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string person_num;
    string id;
    string c_id;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        person_num.clear();
        id.clear();
        c_id.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPContentQPPersonRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPContentQPPersonRecord()
    {
        this->clear();
    }
};




class MCPContentQPPerson : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    MCPContentQPPerson(const char* key_name);
    virtual ~MCPContentQPPerson();
};


#endif /* mcp_content_qp_activity_person_H_ */