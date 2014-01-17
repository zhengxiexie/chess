#ifndef mcp_netgame_activity_H_
#define mcp_netgame_activity_H_


#include "abstract_mcp_content.h"


class MCPNetGameActivityInfoRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    vector<MCPTupleItem> img_info;
    string content;
    string logdate;
    string channel;
    vector<string> news_id;
    string id;
    string title;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);


        int size = img_info.size();
        for(int i = 0; i < size; i++)
        {
            img_info[i].key.clear();
            img_info[i].value.clear();
        }
        img_info.clear();

        content.clear();
        logdate.clear();
        channel.clear();


        size = news_id.size();
        for(int i = 0; i < size; i++)
        {
            news_id[i].clear();
        }
        news_id.clear();

        id.clear();
        title.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPNetGameActivityInfoRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPNetGameActivityInfoRecord()
    {
        this->clear();
    }
};




class MCPNetGameActivityInfo : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out,
                                        const char* aggregate_name);


public:
    MCPNetGameActivityInfo(const char* key_name);
    virtual ~MCPNetGameActivityInfo();
};


#endif /* mcp_netgame_activity_H_ */
