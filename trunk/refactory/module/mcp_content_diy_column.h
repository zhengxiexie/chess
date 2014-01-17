#ifndef mcp_content_diy_column_H_
#define mcp_content_diy_column_H_


#include "abstract_mcp_content.h"


class MCPDIYColumnRecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string type_id;
    string name;
    string id;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        type_id.clear();
        name.clear();
        id.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    MCPDIYColumnRecord():AbstractMCPRecord()
    {
    }


    virtual ~MCPDIYColumnRecord()
    {
        this->clear();
    }
};




class MCPDIYColumnInfo : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out,
                                        const char* aggregate_name);


public:
    MCPDIYColumnInfo(const char* key_name);
    virtual ~MCPDIYColumnInfo();
};

#endif /* mcp_content_diy_column_H_ */
