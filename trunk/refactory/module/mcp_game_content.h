/*
 * mcp_game_content.h
 *
 *  专门负责操作  MongoDB 中  mcp.mcp_content 表的类
 *
 *  Created on: 2013-4-25
 *      Author: guoweijiang
 */

#ifndef MCP_GAME_CONTENT_H_
#define MCP_GAME_CONTENT_H_

#include "abstract_mcp_content.h"

/*
 * AbstractMCPRecord 类针对 mcp.mcp_content 表的具体实现
 * 是 mcp.mcp_content 表的一个全集结构
 * 作为 MCPGameContent 子类的返回基础类型
 *
 *  要通过对象池管理
 *  所以要实现 ClassName 和 clear 方法
 */
class MCPGameInfo : public AbstractMCPRecord
{
public:

    static const char* ClassName;

    string id;
    string name;
    string content_type;
    vector<MCPTupleItem> classes;
    string logdate;
    string ext_id;
    string info;
    string version;
    string method;
    string star;
    string network_id;
    string game_code;
    string cp_id;
    string grant_id;
    string priority;
    string default_charge_typeid;
    string default_charge_value;
    string modify_date;
    string package_name;
    string version_appinfo;
    string sdk_version;
    string version_code;
    string ptype_1;
    string ptype_2;
    string ptype_3;
    string ptype_4;
    string ptype_5;
    vector<string> label_ids;
    vector<MCPTupleItem> column;
    vector<MCPTupleItem> img_info;
	//add by zhengxie
	//vector<MCPTupleItem> pub_prop_info;
	vector<MCPStructItem> pub_prop_info;
    string enName;
    string language;
    string adapt;
    string class_weight;
    string latest_weight;
    string download_weight;
    string pop_weight;
    string udate;
    string pub_prop_ids;
    string client_class_weight;
    string client_latest_weight;
    string client_download_weight;
    string client_pop_weight;
    string union_start_action;
    string has_account;
    string apk_md5;
	//add by zhengxie
	string editor_recom;
	string show_highspeed;
	string bind_resource_id;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        id.clear();
        name.clear();
        content_type.clear();
        logdate.clear();
        ext_id.clear();
        info.clear();
        version.clear();
        method.clear();
        star.clear();
        network_id.clear();
        game_code.clear();
        cp_id.clear();
        grant_id.clear();
        priority.clear();
        default_charge_typeid.clear();
        default_charge_value.clear();
        modify_date.clear();
        package_name.clear();
        version_appinfo.clear();
        sdk_version.clear();
        version_code.clear();
        ptype_1.clear();
        ptype_2.clear();
        ptype_3.clear();
        ptype_4.clear();
        ptype_5.clear();
        enName.clear();
        language.clear();
        adapt.clear();
        class_weight.clear();
        latest_weight.clear();
        download_weight.clear();
        pop_weight.clear();
        udate.clear();
        pub_prop_ids.clear();
        client_class_weight.clear();
        client_latest_weight.clear();
        client_download_weight.clear();
        client_pop_weight.clear();
        union_start_action.clear();
        has_account.clear();
        apk_md5.clear();
		editor_recom.clear();
		show_highspeed.clear();
        int size = label_ids.size();
        for(int i = 0; i < size; i++)
        {
            label_ids[i].clear();
        }
        label_ids.clear();

        size = classes.size();
        for(int i = 0; i < size; i++)
        {
            classes[i].key.clear();
            classes[i].value.clear();
        }
        classes.clear();

        size = column.size();
        for(int i = 0; i < size; i++)
        {
            column[i].key.clear();
            column[i].value.clear();
        }
        column.clear();

        size = img_info.size();
        for(int i = 0; i < size; i++)
        {
            img_info[i].key.clear();
            img_info[i].value.clear();
        }
        img_info.clear();

		//add by zhengxie
		pub_prop_info.clear();

    }

    MCPGameInfo():AbstractMCPRecord()
    {
    }

    virtual ~MCPGameInfo()
    {
        this->clear();
    }
};

class MCPGameContent : public AbstractMCPContent
{
protected:

    /*
     *  将从 mcp.mcp_content 表中查询到的一条 Record 保存到  id_s 和 result_set 中
     */
    int set_basic_info_from_bson_result(bson* bson_out,
                                        const char* aggregate_name);

public:
    MCPGameContent(const char* key_name);
    virtual ~MCPGameContent();
};

#endif /* MCP_GAME_CONTENT_H_ */
