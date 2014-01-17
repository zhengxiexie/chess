/*
 * =====================================================================================
 *
 *       Filename:  content_list.h
 *
 *    Description:  get mcp content list from MongoDB
 *
 *        Version:  1.0
 *        Created:  05/18/2012 10:50:42 
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  RD. Yong li (ly), liyong04@baidu.com
 *        Company:  baidu, Beijing, China
 *
 * =====================================================================================
 */

#ifndef _CONTENT_LIST_H_
#define _CONTENT_LIST_H_

#include <vector>
#include <string>
#include <map>
#include "content_common.h"
#include "mongo_connpool.h"
#include "ub_server.h"
using namespace std;

extern mongo_connpool* mongo_pool;

namespace content{
struct basic_info_t
{
    string id;
    string name;
    string class_id;
    string class_name;
    string code;
    string img_url;
    string charge_type;
    string charge_name;
    string charge_value;
    string logdate;

	string version;
	string package_name;
	string version_appinfo;
	string sdk_version;
	string version_code;
    string star;

    // [s] add by gwj for mcp_content_pub_prop
    string pub_prop_ids;
    // [e] add by gwj for mcp_content_pub_prop
	string show_highspeed;

    void clear()
    {
        id.clear();
        name.clear();
        class_id.clear();
        class_name.clear();
        code.clear();
        img_url.clear();
        charge_type.clear();
        charge_name.clear();
        charge_value.clear();
		package_name.clear();
		version.clear();
		sdk_version.clear();
		version_code.clear();
        star.clear();
        // [s] add by gwj for mcp_content_pub_prop
        pub_prop_ids.clear();
        // [e] add by gwj for mcp_content_pub_prop
		show_highspeed.clear();
    }
};


struct download_info_t
{
    string id;   //content_id
    string down_id;
    string down_url;
    string charge_id;
    string charge_name;
    string charge_value;
    void clear()
    {
        id.clear();
        down_id.clear();
        down_url.clear();
        charge_id.clear();
        charge_name.clear();
        charge_value = "0";
    }
};

struct content_info_t
{
    string id;
    string name;
    string class_id;
    string class_name;
    string code;
    string img_url;
    string down_id;
    string down_url;
    string charge_id;
    string charge_name;
    string charge_value;
    string logdate;
    string up_date;

	string c_id;
	string package_name;
	string version;
	string version_appinfo;
	string sdk_version;
	string version_code;
    string star;

    // [s] add by gwj for mcp_content_pub_prop
    string pub_prop_ids;
    // [e] add by gwj for mcp_content_pub_prop
	string show_highspeed;

    void clear()
    {
        id.clear();
        name.clear();
        class_id.clear();
        class_name.clear();
        code.clear();
        img_url.clear();
        down_id.clear();
        down_url.clear();
        charge_id.clear();
        charge_name.clear();
        charge_value = "0";
        logdate.clear();
		c_id.clear();
		package_name.clear();
		version.clear();
		version_appinfo.clear();
		sdk_version.clear();
		version_code.clear();
        star.clear();

        // [s] add by gwj for mcp_content_pub_prop
        pub_prop_ids.clear();
        // [e] add by gwj for mcp_content_pub_prop
		show_highspeed.clear();
    }
    void set_basic_info(const basic_info_t& basic_info)
    {
        id = basic_info.id;
        name = basic_info.name;
        class_id = basic_info.class_id;
        class_name = basic_info.class_name;
        code = basic_info.code;
        img_url = basic_info.img_url;
        charge_id = basic_info.charge_type;
        charge_name = basic_info.charge_name;
        charge_value = basic_info.charge_value;
        logdate = basic_info.logdate;
		package_name = basic_info.package_name;
        version = basic_info.version;
        version_appinfo = basic_info.version_appinfo;
        sdk_version = basic_info.sdk_version;
		version_code = basic_info.version_code;
        star = basic_info.star;

        // [s] add by gwj for mcp_content_pub_prop
        pub_prop_ids = basic_info.pub_prop_ids;
        // [e] add by gwj for mcp_content_pub_prop
		show_highspeed = basic_info.show_highspeed;
    }

    void set_download_info(const download_info_t& download_info)
    {
        if(!down_id.empty()||down_id.compare("")!=0){
            down_id += ",";
            down_url += ",";
            charge_id += ",";
            charge_name += ",";
            charge_value += ",";
        }
        else{
            charge_id.clear();
            charge_name.clear();
            charge_value.clear();
        }
        down_id += download_info.down_id;
        down_url += download_info.down_url;
        charge_id += download_info.charge_id;
        charge_name += download_info.charge_name;
        charge_value +=  download_info.charge_value;
    }
    content_info_t()
    {
    }
};

class ContentList
{
public:
    ContentList()
    {
        _conn = mongo_pool->getConnection();
    }
    ContentList( const string& content_type, uint32_t adapt )
    {
        _conn = mongo_pool->getConnection();
        set_content_type( content_type );
        _adapt = adapt;
    }
	ContentList( const string& content_type)
	{
		_conn = mongo_pool->getConnection();
        set_content_type( content_type );
	}
    virtual ~ContentList()
    {
        if( _conn != NULL ){
            mongo_pool->releaseConnection(_conn);
        }
    }

    void set_unit_info( const uint32_t unit_num, const uint32_t unit_times )
    {
        if( unit_num == 0 ){
            _unit_num = 10;
        }
        else{
            _unit_num = unit_num;
        }
        if( unit_times == 0 ){
            _unit_times = 1;
        }
        else{
            _unit_times = unit_times;
        }
        _start_pos = _unit_num*(_unit_times-1);
        UB_LOG_DEBUG( "unit_num[%d], unit_times[%d], start_pos[%d]", _unit_num, _unit_times, _start_pos );
    }

    void set_adapt_info( const uint32_t adapt_type, const string& adapt_id )
    {
        _adapt_type = adapt_type;
        _adapt_id = adapt_id;
    }

    void set_priority( const uint32_t priority )
    {
        _priority = priority;
    }

    void set_for_free( const uint32_t for_free )
    {
        _for_free = for_free;
    }

    //according to content type,set which collection to get the data from MongoDB
    void set_content_type( const string& content_type )
    {
        if( content_type == MCP_CONTENT_GAME ||
            content_type == MCP_ANDROID_GAME    ){
            _type = content_type;
            _basic_coll = MCP_MONGO_CONTENT_COL;
        }
        else if( content_type == MCP_CONTENT_THEME ){
            _type = content_type;
            _basic_coll = MCP_MONGO_CONTENT_COL;
        }
        else if( content_type == MCP_CONTENT_SOFT ){
            _type = content_type;
            _basic_coll = MCP_MONGO_SOFT_COL;
        }
        else if( content_type == MCP_ANDROID_THEME ){
            _type = content_type;
            _basic_coll = MCP_ANDROID_THEME_COL;
        }
        else if( content_type == MCP_IOS_GAME ||
                 content_type == MCP_IOS_SOFT     ){
            _type = content_type;
            _basic_coll = MCP_IOS_CONTENT_COL;
        }
        else if( content_type == MCP_ONLINE_GAME ){
            _type = content_type;
            _basic_coll = MCP_ONLINE_GAME_COL;
        }
		else if( content_type == MCP_GAME_NEWS){
			_type = content_type;
			_basic_coll = MCP_MONGO_NEWS_COL;
		}
        else if( content_type == MCP_MONGO_H5_MUSIC){
            _type = content_type;
            _basic_coll = MCP_MONGO_H5_MUSIC;
        }
        else{
            _type = MCP_CONTENT_GAME;
            _basic_coll = MCP_MONGO_CONTENT_COL;
        }
        _download_coll = MCP_MONGO_DATA_COL;
        _count_coll = MCP_MONGO_ADAPT_COL;
        _basic_ns = MCP_MONGODB_NAME + "." + _basic_coll;
        _download_ns = MCP_MONGODB_NAME + "." + _download_coll;
        _count_ns = MCP_MONGODB_NAME + "." + _count_coll;
    }

    virtual int get_content_list(int32_t& count, vector<content_info_t>& list);
    virtual int get_soft_list(int32_t& count, vector<soft_info_t>& soft_list);
    virtual int get_ios_content_list(int32_t& count, vector<ios_content_t>& list);
    virtual int get_android_theme_list(int32_t& count, vector<android_theme_t>& theme_list);
    virtual int get_H5_music_id_list(int32_t& count, vector<string>& id_list, string col_id);
    virtual int getOnlineGameList(int32_t& count, vector<online_game_t>& list);
    virtual int get_content_count(int32_t weight_type = 0);
    virtual int get_content_count(const map<string,vector<download_info_t> >& m_id_download_info);

    virtual int get_content_basic_info(vector<basic_info_t>& basic_list);
    virtual int get_content_basic_info(const map<string,vector<download_info_t> >& m_id_download_info, 
                                             vector<basic_info_t>& basic_list);
    virtual int get_content_download_info(const vector<basic_info_t>& basic_list, vector<download_info_t>& download_list);
    virtual int get_content_download_info(map<string,vector<download_info_t> >& m_id_download_info);

    virtual int combine_content_list(const vector<basic_info_t>& basic_list, const vector<download_info_t>& download_list);
    virtual int combine_content_list(const vector<basic_info_t>& basic_list, 
                                     map<string,vector<download_info_t> >& m_id_download_info);
    virtual int get_content_soft_list(vector<soft_info_t>& soft_list);
    virtual int get_ios_content_list(vector<ios_content_t>& list);
    virtual int get_android_theme_basic_list(vector<android_theme_t>& theme_list);
    virtual int get_H5_music_id_list(vector<string>& id_list, string col_id);
    virtual int getOnlineGameList(vector<online_game_t>& list);

    virtual int get_basic_info_from_cursor(mongo_cursor * cursor, basic_info_t& basic_info);
    virtual int get_download_info_from_cursor(mongo_cursor * cursor, download_info_t& download_info);
    virtual int get_soft_info_from_cursor(mongo_cursor * cursor, soft_info_t& soft_info);
    virtual int get_soft_info_from_bson_iterator(bson_iterator* it, soft_info_t& soft_info);
    virtual int get_android_theme_from_cursor(mongo_cursor * cursor, android_theme_t& theme_info);
    virtual int get_ios_content_from_cursor(mongo_cursor * cursor, ios_content_t& ios_content);
    virtual online_game_t getOnlineGameFromCursor(mongo_cursor *cursor);
    virtual void get_soft_class_info_from_bson(bson_iterator* it, soft_info_t& soft_info);
    virtual void get_theme_class_info_from_bson(bson_iterator* it, android_theme_t& theme_info);
    virtual void get_class_info_from_bson(bson_iterator* bson_it, basic_info_t& basic_info);
    virtual void get_img_info_from_bson(bson_iterator* bson_it, string& img_url);
    virtual void get_img_info_from_bson(bson_iterator* bson_it, soft_info_t& soft_info);
    virtual void get_img_info_from_bson(bson_iterator* bson_it, android_theme_t& theme_info);

    virtual void get_basic_query_bson(bson* query);
    virtual void get_basic_query_bson2(bson* query, const int32_t weight_type, const string class_id);
    virtual void get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_basic_query_bson2(const map<string,vector<download_info_t> >& m_id_download_info,bson* query, const int32_t weight_type, const string class_id);
    virtual void get_basic_fields_bson(bson* fields);
    virtual void get_download_query_bson(bson* query);
    virtual void get_download_query_bson(const vector<basic_info_t>& basic_list,bson* query);
    virtual void get_download_fields_bson(bson* fields);
    virtual void get_count_query_bson(bson* query, int32_t weight_type);
    virtual void get_count_query_bson(bson* query);
    virtual void get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_count_fields_bson(bson* fields);
    virtual void get_soft_query_bson(bson* query);
    virtual void get_soft_fields_bson(bson* fields);
    virtual void get_ios_query_bson(bson* query);
    virtual void get_ios_fields_bson(bson* fields);
    virtual void getOnlineGameQueryBson(bson *query);
    virtual void getOnlineGameFieldsBson(bson *fields);
    virtual void get_android_theme_query_bson(bson* query);
    virtual void get_android_theme_fields_bson(bson* fields);
	virtual int  get_game_brief_list(int32_t&count,vector<game_brief_t>& list);
	virtual int  get_game_brief_list(vector<game_brief_t>& list);
	virtual int  get_game_brief_from_cursor(mongo_cursor * cursor, game_brief_t& ios_content );
    virtual void get_game_brief_query_bson(bson* query);
    virtual void get_game_brief_fields_bson(bson* fields);
    virtual void get_H5_music_id_query_bson(bson* query, string col_id);
    virtual void get_H5_music_id_fields_bson(bson* fields);
    virtual int get_H5_music_id_from_cursor(mongo_cursor * cursor, vector<string>& id_list);
    virtual void get_H5_music_ids_from_bson(bson_iterator* bson_it, vector<string>& id_list);

    static void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
    static string get_mongo_namespace( string mongo_db_name, string mongo_collection_name );

protected:
    mongo *_conn;
    string _type;
    uint32_t _unit_times;
    uint32_t _unit_num;
    uint32_t _start_pos;
    uint32_t _adapt;
    uint32_t _adapt_type;
    uint32_t _priority;
    uint32_t _for_free;
    string _adapt_id;
    string _basic_coll;
    string _download_coll;
    string _basic_ns;
    string _download_ns;
    string _count_coll;
    string _count_ns;

    vector<content_info_t> _list;

    virtual int get_content_list_all();
    virtual int get_content_list_only_adapt();
    string convert_mongo_ts_to_string(bson_iterator* it);

private:
};

};

#endif
