/*
 * mcp_content_common_info.h
 *
 *  Created on: 2013-5-17
 *      Author: guoweijiang
 */

#ifndef MCP_CONTENT_COMMON_INFO_H_
#define MCP_CONTENT_COMMON_INFO_H_

#include "abstract_mcp_content.h"

class MCPCommonAdvInfoRecord : public AbstractMCPRecord
{
public:

    static const char* ClassName;

    string id;
    string info;

    void clear()
    {
        this->id.clear();
        this->info.clear();
    }

    MCPCommonAdvInfoRecord():AbstractMCPRecord()
    {
    }

    virtual ~MCPCommonAdvInfoRecord()
    {
        this->clear();
    }
};

class MCPContentCommonInfo : public AbstractMCPContent
{
protected:

    /*
     *  将从 mcp.mcp_content 表中查询到的一条 Record 保存到  id_s 和 result_set 中
     */
    int set_basic_info_from_bson_result(bson* bson_out,
                                        const char* aggregate_name);

public:
    MCPContentCommonInfo(const char* key_name);
    virtual ~MCPContentCommonInfo();
};

#endif /* MCP_CONTENT_COMMON_INFO_H_ */
