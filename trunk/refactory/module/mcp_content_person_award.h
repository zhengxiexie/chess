#ifndef mcp_content_person_award_H_
#define mcp_content_person_award_H_


#include "abstract_mcp_content.h"


class MCPContentPersonAwardRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string priority;
    string kudou;
    string commodity_id;
	string name;
	string icon;
	string date;
	string type;
    string user_id;
    string id;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        kudou.clear();
        commodity_id.clear();
		name.clear();
		icon.clear();
		date.clear();
		type.clear();
        user_id.clear();
        id.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPContentPersonAwardRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPContentPersonAwardRecord()
    {
        this->clear();
    }
};




class MCPContentPersonAward : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    MCPContentPersonAward(const char* key_name);
    virtual ~MCPContentPersonAward();
};


#endif /* mcp_content_person_award_H_ */
