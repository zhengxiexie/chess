#ifndef mcp_content_championship_H_
#define mcp_content_championship_H_


#include "abstract_mcp_content.h"


class MCPContentChampionshipRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string enable;
    string join_num;
    string start_time;
    string name;
    string rule;
    string total_coins;
    string award;
    string channel_info;
    string pic_url;
    string end_time;
    string 	id;
    string log_date;
    string c_id;
	string top_award;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        enable.clear();
        join_num.clear();
        start_time.clear();
        name.clear();
        rule.clear();
        total_coins.clear();
        award.clear();
        channel_info.clear();
        pic_url.clear();
        end_time.clear();
		id.clear();
        log_date.clear();
        c_id.clear();
        top_award.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPContentChampionshipRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPContentChampionshipRecord()
    {
        this->clear();
    }
};




class MCPContentChampionship : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    MCPContentChampionship(const char* key_name);
    virtual ~MCPContentChampionship();
};


#endif /* mcp_content_championship_H_ */
