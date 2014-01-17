/*
 * mcp_content_news.h
 *
 *  Created on: 2013-5-10
 *      Author: guoweijiang
 */

#ifndef MCP_CONTENT_NEWS_H_
#define MCP_CONTENT_NEWS_H_

#include "abstract_mcp_content.h"

/*
 *  要通过对象池管理
 *  所以要实现 ClassName 和 clear 方法
 */
class MCPNewsInfo : public AbstractMCPRecord
{
public:

    static const char* ClassName;

    string id;
    string name;
    string content;
    string type_id;
    string logdate;
    string c_id;
    string game_type;
	string news_source;

    MCPNewsInfo():AbstractMCPRecord(){}

    virtual ~MCPNewsInfo()
    {
        this->clear();
    }

    void clear()
    {
        id.clear();
        name.clear();
        content.clear();
        type_id.clear();
        logdate.clear();
        c_id.clear();
        game_type.clear();
		news_source.clear();
    }
};

class MCPContentNews : public AbstractMCPContent
{
protected:

    /*
     *  将从 mcp.mcp_content_news 表中查询到的一条 Record 保存到  id_s 和 result_set 中
     */
    int set_basic_info_from_bson_result(bson* bson_out,
                                        const char* aggregate_name);

public:
    MCPContentNews(const char* key_name);
    virtual ~MCPContentNews();
};

#endif /* MCP_CONTENT_NEWS_H_ */
