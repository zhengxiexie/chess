#ifndef _MCPSVR_CONTENT_COMMON_H_
#define _MCPSVR_CONTENT_COMMON_H_

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"
#include "tools.h"
#include "mongo_connpool.h"
#include <time.h>

using namespace std;

#define MCP_MONGODB_NAME      "mcp"
#define MCP_MONGO_GAME_COL    "mcp_game_content"
#define MCP_MONGO_FORMAT_COL  "mcp_content_format"
#define MCP_MONGO_CLASS_COL   "mcp_content_class"
#define MCP_MONGO_DATA_COL    "mcp_content_data"
#define MCP_MONGO_PREVIEW_COL "mcp_content_preview"
#define MCP_MONGO_RANK_COL    "mcp_content_rank"
#define MCP_MONGO_COLUMN_COL  "mcp_content_column_list"
#define MCP_MONGO_DIY_COL     "mcp_content_diy_column"
#define MCP_MONGO_PHONE_COL   "mcp_phone_info"
#define MCP_MONGO_TYPE_COL    "mcp_type_list"
#define MCP_MONGO_THEME_COL   "mcp_theme_content"
#define MCP_MONGO_CHARGETYPE_COL "mcp_content_chargetype"
#define MCP_MONGO_CLASS_LIST_COL "mcp_content_class_list"
#define MCP_MONGO_URL_COL     "mcp_url_content"
#define MCP_MONGO_ADAPT_COL     "mcp_class_adapt_count"
#define MCP_MONGO_LABEL_COL     "mcp_content_bookmark"
#define MCP_MONGO_LABEL_LIST_COL "mcp_content_bookmark_list"

const string MCP_CONTENT_GAME( "1" );
const string MCP_CONTENT_THEME( "5" );
const string MCP_ANDROID_GAME( "11" );

const string MCP_FORMAT_TYPE_PHONE("1");

extern mongo_connpool* mongo_pool;

/*
enum content_basic_info_index
{   
    CONTENT_ID = 0,
    CONTENT_NAME,
    CONTENT_CLASS_ID,
    CONTENT_CLASS_NAME,
    CONTENT_WAP_URL,
    CONTENT_EXT_ID,
    CONTENT_INFO,
    CONTENT_VERSION = 7, 
    GAME_METHOD     = 8,
    CONTENT_STAR    = 9,
    SUPPORT_NETWORK_ID,
    CONTENT_CP_CODE = 11,
    CONTENT_CP_ID,
    CONTENT_CP_NAME,
    CONTENT_GRANT_ID,
    CONTENT_GRANT_DATE,
    CONTENT_LOGDATE,
    CONTENT_SIGN_ENDDATE,
    CONTENT_MODIFY_DATE,
    CONTENT_DEFAULT_CHARGE_TYPE,
    CONTENT_DEFAULT_CHARGE_VALUE,
    CONTENT_DOWNLOAD_ID,
    CONTENT_DOWNLOAD_URL,
    CONTENT_DOWNLOAD_CHARGE_TYPE_ID = 23
}; 
*/


struct content_basic_info_t
{
    string content_id;
    string content_name;
    string class_id;
    string class_name;
    string game_code;
    string wap_url;

    string data_id;
    string data_path;
    string charge_type_id;
    string charge_type_name;
};

struct download_info_t
{
    string id;
    string path_url;
    string path_type;
    string media_format;
    string size;
    string mtype;
    string charge_type;
    string value;

    void clear()
    {
        id.clear();
        path_url.clear();
        path_type.clear();
        media_format.clear();
        size.clear();
        mtype.clear();
        charge_type.clear();
        value.clear();
    }
};

struct url_info_t
{
    string id;
    string name;
    string class_id;
    string class_name;
    string ext_id;
    string ad;
    string url;
    string baiduurl;
    string mark;
    string logdate;
    string modify_date;

    void clear()
    {
        id.clear();
        name.clear();
        class_id.clear();
        class_name.clear();
        ext_id.clear();
        ad.clear();
        url.clear();
        baiduurl.clear();
        mark.clear();
        logdate.clear();
        modify_date.clear();
    }

};

struct content_info_t
{
    string id;
    string name;
    string class_id;
    string class_name;
    string code;
    string down_id;
    string down_url;
    string charge_id;
    string charge_name;
    string charge_value;
    string wap_url;
    string dft_charge_id;
    string dft_charge_name;
    //int priority;
    //string logdate;
    void clear()
    {
        id.clear();
        name.clear();
        class_id.clear();
        class_name.clear();
        code.clear();
        down_id.clear();
        down_url.clear();
        charge_id.clear();
        charge_name.clear();
        charge_value = "0";
        //priority = 0;
        wap_url.clear();
        dft_charge_id.clear();
        dft_charge_name.clear();
        //logdate.clear();
    }
    content_info_t()
    {
    }
};

struct topic_info_t
{
    string id;
    string name;
    void clear()
    {
        id.clear();
        name.clear();
    }
};

time_t from_mongo_change_to_unix(time_t mongo );
int mcp_connect_mongo_hosts( mongo* &conn );
int check_mongo_conn( mongo* conn );
void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
string get_mongo_namespace( string mongo_db_name, string mongo_collection_name );
int get_content_type_by_id( mongo *conn, const string id, string& type_id );
int get_default_charge_type_by_cid( mongo* conn, const string content_type_id, const string c_id, string& default_charge_type_id );
int get_charge_type_name_by_id(  mongo* conn, const string id, string& name );
void change_everything_to_string(const bson_iterator* it,string& result);
int get_theme_basic_info_by_id( mongo *conn, const string id, string& name, string logdate );
int get_class_name( mongo *conn, const string class_id, string& class_name );
int get_theme_class_info( mongo *conn, const string c_id, map<string, string>& m_class_id_name,
                                             string& class_id, string& class_name );
int get_content_id_list_by_typeID( mongo *conn, const string type_id, vector<string>& content_id_list );
int get_img_url_by_content_id( mongo *conn, const string c_id, const string image_type_id, string& image_url );
int get_content_count_by_classID( mongo* conn, const string class_id, uint32_t& count );
int get_content_count_by_typeID( mongo* conn, const string class_id, uint32_t& count );
int get_content_info_by_id_list( mongo *conn, const string content_type_id, const uint32_t priority, const uint32_t unit_num,
                                 const uint32_t unit_times, const string info_id, const uint32_t info_type, const string img_size,
                           vector<string>& content_id_list, uint32_t& count, vector<content_basic_info_t>& content_info_list );
int get_download_basic_info( mongo *conn, const string c_id, const int stype, const string stype_id,
                             const string default_charge_typeid, map<string, string>& m_charge_id_name,
                             string& cdata_id, string& path_url, string& charge_typeid, string& charge_typename );
int get_column_name( mongo *conn, const string column_id, string& column_name );
int get_content_count_by_columnID( mongo* conn, const string column_id, uint32_t& count );
int get_content_charge_id_name_map( mongo* conn, map<string, string>& m_charge_id_name );
int get_content_class_id_name_map( mongo* conn, map<string, string>& m_charge_id_name );
int get_label_id_name_map( map<string,string>& m_label_id_name );
int mcp_get_url_info_by_id( mongo* conn, const string& id, url_info_t& url_info );
int mcp_get_url_count_by_column_id( mongo* conn, const string column_id, uint32_t& count );
int mcp_get_url_count_by_class_id( mongo* conn, const string class_id, uint32_t& count );
int mcp_get_url_info_by_column_id(  mongo* conn, const string column_id, const uint32_t unit_num, const uint32_t unit_time,
                                                     vector<url_info_t>& url_list );
int mcp_get_url_info_by_class_id(  mongo* conn, const string class_id, const uint32_t unit_num, const uint32_t unit_time,
                                                     vector<url_info_t>& url_list );

int get_content_id_list_by_class_id( mongo* conn, const string& content_type_id, const string& class_id,
                                     const uint32_t priority, const uint32_t unit_num, const uint32_t unit_times,
                                     const string& info_id, const uint32_t info_type, const string& img_size,
                                     uint32_t& count, vector<content_info_t>& content_info_list);

int get_content_list_by_phone_id(mongo* conn, const string p_id, const uint32_t priority, 
                                 const uint32_t type, const uint32_t unit_num, const uint32_t unit_times, 
                                 int64_t *count, vector<content_info_t>* content_info_list);

int get_adapt_count_by_class_id( const vector<string>& class_id, const string& info_id, const uint32_t info_type,
                                       vector<int>& count );
int get_count_by_rank( const string& content_type, const string& class_id, const string& info_id, const uint32_t info_type,
                       const uint32_t for_adapt );
int get_content_info_by_rank( const string& content_type, const string& class_id, const uint32_t p_type, const uint32_t unit_num,
                              const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list );

int get_adapt_content_info_by_rank( const string& content_type, const string& class_id, const uint32_t p_type,
                              const uint32_t unit_num, const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list );

int get_adapt_content_info_by_class( const string& content_type, const string& class_id, const uint32_t priority,
                              const uint32_t unit_num, const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list );

int get_label_by_c_id( const string& c_id, vector<string>& label_id_list );

int get_count_by_label( const string& label_id );
int get_content_info_by_label( const string& label_id, const uint32_t unit_num, const uint32_t unit_times, const string& info_id,
                               const uint32_t info_type, vector<content_info_t>& content_info );
int get_content_image( const string& content_id, const uint32_t img_type, vector<string>& img_url_list );

int get_topic_count_by_type( const string& content_type );
int get_topic_info_by_type( const string& content_type, const uint32_t unit_num, const uint32_t unit_times, 
                            vector<topic_info_t>& topic_info_list );

int get_label_name_by_label_id( const string& label_id, string& label_name );
int get_content_info_by_date( const string& content_type, const uint32_t unit_num,
                              const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list );
int get_adapt_content_info_by_date( const string& content_type,
                              const uint32_t unit_num, const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list );


int get_download_info_by_cid( const string& c_id, const uint32_t info_type, const string& info_id,
                              vector<download_info_t>& download_info_list );
int create_phone_invalid_list();

#endif
