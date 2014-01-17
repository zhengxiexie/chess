/*
 * =====================================================================================
 *
 *       Filename:  mcpsvr_mongo_contentapi.cpp
 *
 *    Description:  use mongoDB implent the content interface
 *
 *        Version:  1.0
 *        Created:  08/18/2011 05:42:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  RD. Hongming li (lhm), lihongming@baidu.com
 *        Company:  baidu, Beijing, China
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <time.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream> 
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"
#include "tools.h"
#include "mcpsvr_content_common.h"

#include "mongo.h"

using namespace std;
#define REAL_TIME_DATA

#define PHONE_TYPE 1



enum info_list
{
    CONTENT_ID =   0,     //  资源id 
    CONTENT_NAME ,        //  资源名称
    CONTENT_TYPE_ID,      //  资源类别id 
    CONTENT_TYPE_NAME,    //  资源类别名称
    WAP_PREVIEW_URL,      //  wap预览图的URL 
    EXT_INFO_ID,          //  扩展属性id 
    GAME_CONTENT_INFO,    //  游戏内容介绍
    VERSION,              //  版本号
    METHOD,               //  操作方法
    STAR,                 //  星级
    NET_WORK,             //  支持网络
    GAME_CODE ,           //  中国移动业务代码
    CP_ID,                //  cp_id 
    CP_NAME,              //  CP公司名称
    GRANT_ID,             //  授权id 
    GRANT_DATE,           //  授权时间
    SIGN_ENDDATE,         //  协议有效期
    CONTENT_LOG_DATE,     //  录入时间
    CONTENT_MOD_DATE,     //  修改日期
    DEFAULT_CHARGE_TYPEID,//  默认计费类型
    DEFAULT_CHARGE_VAL,   //  默认计费价格
    CONTENT_DATA_ID,      //  数据包ID 
    CONTENT_DATA_URL,     //  数据包URL
    CONTENT_DATA_CHARGE   //  数据付费ID
};


typedef struct{
    string c_id;
    string c_name;
    string log_date;
    string game_code;
    int priority;
}content_info;

//按天的优先级排序
bool compare_content_info(content_info c1,content_info c2){
    return c1.priority > c2.priority;
}
//按照日期排序
bool compare_content_info_by_logdate(content_info c1,content_info c2){
    if( strcmp(c1.log_date.c_str(),c2.log_date.c_str())  > 0 ){
        return true;
    }
    return false;
}

time_t from_mongo_change_to_unix(time_t mongo ){
    time_t timep = mongo;
    struct tm *t;
    t = localtime(&timep);
    t->tm_year = t->tm_year + 1900;
    t->tm_mon = t->tm_mon ;
    t->tm_hour = t->tm_hour - 8;
    time_t timep2;
    timep2 = mktime(t);
    return timep2;
}
time_t adjust_8_hours_seconds(time_t timep ){
    timep = timep - 8*60*60;
    /*
    struct tm *t;
    t = localtime(&timep);
    t->tm_year = t->tm_year;
    t->tm_mon = t->tm_mon ;
    t->tm_hour = t->tm_hour - 8;
    time_t timep2;
    timep2 = mktime(t);
    */
    return timep;
}

void change_everything_to_string(const bson_iterator* it,string& result){
    stringstream ss;

    result.clear();
    time_t timep;
    struct tm *t;
    char tmp_date_time[1024];

    switch( bson_iterator_type(it) ) {
    //    case BSON_DOUBLE:
    //        fprintf( stderr, "(double) %e\n", bson_iterator_double( &it ) );
    //        break;
        case BSON_INT:
            ss<<bson_iterator_int(it);
            result = ss.str();
            break;
        case BSON_STRING:
            result = string(bson_iterator_string(it));
            break;
    //    case BSON_OID:
    //        bson_oid_to_string( bson_iterator_oid( &it ), hex_oid );
    //        fprintf( stderr, "(oid) \"%s\"\n", hex_oid );
    //        break;
    //    case BSON_OBJECT:
    //        fprintf( stderr, "(subobject) {...}\n" );
    //        break;
    //    case BSON_ARRAY:
    //        fprintf( stderr, "(array) [...]\n" );
    //        break;
        case BSON_TIMESTAMP:
            timep = bson_iterator_time_t(it);
            timep = from_mongo_change_to_unix(timep);
            t = localtime(&timep); 
            snprintf(tmp_date_time,sizeof(tmp_date_time),"%d-%d-%d %d:%d:%d",t->tm_year , t->tm_mon + 1 , t->tm_mday, t->tm_hour , t->tm_min, t->tm_sec);
            result = string(tmp_date_time);
            break;
        case BSON_DATE:
            timep = bson_iterator_time_t(it);
            timep = from_mongo_change_to_unix(timep);
            t = localtime(&timep); 
            snprintf(tmp_date_time,sizeof(tmp_date_time),"%d-%d-%d %d:%d:%d",t->tm_year , t->tm_mon + 1 , t->tm_mday, t->tm_hour , t->tm_min, t->tm_sec);
            result = string(tmp_date_time);
            break;
        default:
            fprintf( stderr, "(type %d)\n", bson_iterator_type(it) );
            break;
        }
}

int get_class_name_by_class_id(string class_id,string& class_name){

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }

    string tmp_result;
    const char* key;
    bson query[1];
    bson fields[1];
    bson result[1];
               
    class_name.clear();
    
    //设置查询条件
    bson_init(query);
    bson_append_string(query,"id",class_id.c_str());
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"name",1);
    bson_finish(fields);

    if( MONGO_OK == mongo_find_one(conn,"mcp.mcp_content_class",query,fields,result) ){
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_class OK %s %d",__func__,__LINE__);
        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"name")){
                change_everything_to_string(it,tmp_result);
                class_name = tmp_result;
            }
        }
        bson_destroy(result);
    }else{
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_class Error %s %d",__func__,__LINE__);
    }

    bson_destroy(query);
    bson_destroy(fields);

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    return 0;
}


//
//return value 0 成功 -1，失败 -2 已经过期或者未到授权起始期
int get_permission_by_p_id(string p_id,string& grant_date,string& sign_enddate,string& modify_date){

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }

    const char* key;
    bson query[1];
    bson fields[1];
    bson result[1];

    string tmp_result;
    
    grant_date.clear();
    sign_enddate.clear();
    modify_date.clear();

     //获得当前时间
     time_t now;
     time_t end_date;
     time_t start_date;
     now = time(0);


    //设置查询条件
    bson_init(query);
    bson_append_string(query,"id",p_id.c_str());
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"grant_date",1);
    bson_append_int(fields,"sign_enddate",1);
    bson_append_int(fields,"modify_date",1);
    bson_finish(fields);

    if( MONGO_OK == mongo_find_one(conn,"mcp.mcp_content_grant",query,fields,result) ){
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_grant OK %s %d",__func__,__LINE__);
        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"grant_date")){
                start_date = bson_iterator_time_t(it);
                change_everything_to_string(it,tmp_result);
                grant_date = tmp_result;
            }else if( 0 == strcmp(key,"sign_enddate") ){
                end_date = bson_iterator_time_t(it);
                change_everything_to_string(it,tmp_result);
                sign_enddate = tmp_result;
            }else if( 0 == strcmp(key,"modify_date") ){
                change_everything_to_string(it,tmp_result);
                modify_date = tmp_result;
            }
        }
        bson_destroy(result);
    }else{
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_grant Error %s %d",__func__,__LINE__);
    }

    bson_destroy(query);
    bson_destroy(fields);


    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    if( now >= adjust_8_hours_seconds(start_date) && now <= adjust_8_hours_seconds(end_date) ){
        return 0;
    }else{
        return -2;
    }

}

int get_cp_name_by_cp_id(string cp_id,string& cp_name){

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }

    const char* key;
    bson query[1];
    bson fields[1];
    bson result[1];
    string tmp_result;
    cp_name.clear();
    //设置查询条件
    bson_init(query);
    bson_append_string(query,"id",cp_id.c_str());
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"name",1);
    bson_finish(fields);

    if( MONGO_OK == mongo_find_one(conn,"mcp.mcp_content_cp",query,fields,result) ){
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_cp OK %s %d",__FILE__,__LINE__);
        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"name")){
                change_everything_to_string(it,tmp_result);
                cp_name = tmp_result;
            }
        }
        bson_destroy(result);
    }else{
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_cp Error %s %d",__FILE__,__LINE__);
    }
    bson_destroy(query);
    bson_destroy(fields);

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    return 0;
}


int get_image_by_content_id_iamge_type(string cid,string itype,string& url){

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }


    const char* key;
    bson query[1];
    bson fields[1];
    bson result[1];
    url.clear();
    string tmp_result;
    //设置查询条件
    bson_init(query);
    bson_append_string(query,"c_id",cid.c_str());
    bson_append_string(query,"image_type_id",itype.c_str());
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"image_url",1);
    bson_finish(fields);

   // bson_print(query);
   // bson_print(fields);

    if( MONGO_OK == mongo_find_one(conn,"mcp.mcp_content_preview",query,fields,result) ){
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_preview OK %s %d",__func__,__LINE__);
        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"image_url")){
                change_everything_to_string(it,tmp_result);
                url = tmp_result;
            }
        }
        UB_LOG_NOTICE(" bson_iterator_next is ok");
        bson_destroy(result);
    }else{
        UB_LOG_NOTICE(" Find Data Info From MCP.mcp_content_preview Error %s %d",__func__,__LINE__);
    }
    UB_LOG_NOTICE(" bson_destroy is ok");

    bson_destroy(query);
    bson_destroy(fields);

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    return 0;
}

/*s.10/18/2011 liyong */
int get_content_id_list_by_class_id(string content_type_id, string class_id,vector<content_info>& id_list){
/*e.10/18/2011 liyong */


    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }

    string c_id;
    const char* key;
    bson query[1];
    bson fields[1];
    bson *result;
    mongo_cursor cursor[1];
               
    id_list.clear();
    
/*s.10/21/2011 liyong */
    if( content_type_id == MCP_CONTENT_GAME || content_type_id == MCP_ANDROID_GAME )
    {
           //设置查询条件
        bson_init(query);
        bson_append_string(query,"content_type",content_type_id.c_str());
        bson_append_string(query,"class_id",class_id.c_str());
        bson_finish(query);
        //设置返回域
        bson_init(fields);
        bson_append_int(fields,"id",1);
        bson_append_int(fields,"logdate",1);
        bson_append_int(fields,"name",1);
        bson_append_int(fields,"priority",1);
        bson_append_int(fields,"game_code",1);
        bson_finish(fields);
        mongo_cursor_init( cursor, conn, "mcp.mcp_game_content");
    }
    else if( content_type_id == MCP_CONTENT_THEME )
    {
        //设置查询条件
        bson_init(query);
        bson_append_string(query,"content_type",content_type_id.c_str());
        bson_append_string(query,"class_id",class_id.c_str());
        bson_finish(query);
        //设置返回域
        bson_init(fields);
        bson_append_int( fields, "c_id", 1 );
        bson_append_int( fields, "priority", 1 );
        bson_finish(fields);
        mongo_cursor_init( cursor, conn, "mcp.mcp_content_class_list" );
    }
    else
    {
        UB_LOG_FATAL( "Unknown content_type_id[%s], [%s:%d]", content_type_id.c_str(), __FILE__, __LINE__ );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);

        return 0;
    }
/*e.10/21/2011 liyong */

    mongo_cursor_set_query(cursor, query);
    mongo_cursor_set_fields(cursor, fields);

    while( mongo_cursor_next(cursor) == MONGO_OK ){

        string log_date = "";
        string tmp_result = "";
        string name = "";
        string game_code = "";
        int priority;
        
        result = &cursor->current;

        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id")){
                change_everything_to_string(it,tmp_result);
                c_id = tmp_result;
            }else if( 0 == strcmp(key,"logdate")){
                change_everything_to_string(it,tmp_result);
                log_date = tmp_result;
            }else if( 0 == strcmp(key,"name")){
                change_everything_to_string(it,tmp_result);
                name = tmp_result;
            }else if( 0 == strcmp(key,"priority")){
                priority = bson_iterator_int(it);
            }else if( 0 == strcmp(key,"game_code")){
                change_everything_to_string(it, tmp_result);
                game_code = tmp_result;
            }else if( 0 == strcmp(key,"c_id")){
                change_everything_to_string(it,tmp_result);
                c_id = tmp_result;
            }
        }

/*s.10/21/2011 liyong */
        int res;
        if( content_type_id == MCP_CONTENT_THEME )
        {
            res = get_theme_basic_info_by_id( conn, c_id, name, log_date );
            if( res != MCP_OK )
            {
                UB_LOG_FATAL( "get theme basic info by id[%s] failed, [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
                continue;
            }
        }
/*e.10/21/2011 liyong */

        content_info c;
        c.c_id = c_id;
        c.log_date = log_date;
        c.priority = priority;
        c.c_name = name;
        c.game_code = game_code;
        //cout<<" cid "<<c_id<<" log_date "<<log_date<<" priority "<<priority<<" name "<<name<<endl;
        UB_LOG_NOTICE( "cid[%s], logdate[%s], priority[%d], name[%s]", c_id.c_str(), log_date.c_str(), priority, name.c_str() );
        id_list.push_back(c);
    }

    bson_destroy(query);
    bson_destroy(fields);
    mongo_cursor_destroy(cursor);

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    return 0;
}

int get_cdata_id_list_by_stype_stypeid(string stype,string stype_id, string c_id, vector<string>& id_list){

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }
    
    int ret = 0;

    ret = get_cdata_id_by_type( conn, c_id, stype, stype_id, id_list );

    if ( ret < 0 )
    {
        UB_LOG_FATAL("get_cdata_id_by_type is failed, c_id is %s, stype is %s, stype_id is %s", 
                               c_id.c_str(), stype.c_str(), stype_id.c_str() );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_NG;
    }
/*
    string tmp_result;
    const char* key;
    bson query[1];
    bson fields[1];
    bson *result;
    mongo_cursor cursor[1];
               
    id_list.clear();
    
    //设置查询条件
    bson_init(query);
    bson_append_string(query,"c_id",c_id.c_str());
    bson_append_string(query,"stype",stype.c_str());
    bson_append_string(query,"stype_id",stype_id.c_str());
    bson_finish(query);

    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"cdata_id",1);
    bson_finish(fields);

    
   // bson_print(query);
   // bson_print(fields);


    mongo_cursor_init( cursor, conn, "mcp.mcp_content_format");
    mongo_cursor_set_query(cursor, query);
    mongo_cursor_set_fields(cursor, fields);

    while( mongo_cursor_next(cursor) == MONGO_OK ){
        result = &cursor->current;

        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"cdata_id")){
                change_everything_to_string(it,tmp_result);
                id_list.push_back(tmp_result);

                //cout<<__func__<<" c_id "<<tmp_result<<endl;
            }
        }
    }

    

    bson_destroy(query);
    bson_destroy(fields);
    mongo_cursor_destroy(cursor);
*/

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    return MCP_OK;
}

int get_content_id_list_by_image_type_id(string image_type_id,vector<string>& id_list){

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }

    string tmp_result;
    const char* key;
    bson query[1];
    bson fields[1];
    bson *result;
    mongo_cursor cursor[1];
               
    id_list.clear();
    
    //设置查询条件
    bson_init(query);
    bson_append_string(query,"image_type_id",image_type_id.c_str());
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"c_id",1);
    bson_finish(fields);

    mongo_cursor_init( cursor, conn, "mcp.mcp_content_preview");
    mongo_cursor_set_query(cursor, query);
    mongo_cursor_set_fields(cursor, fields);

    while( mongo_cursor_next(cursor) == MONGO_OK ){
        result = &cursor->current;

        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"c_id")){
                change_everything_to_string(it,tmp_result);
                id_list.push_back(tmp_result);
                //cout<<__func__<<" c_id "<<tmp_result<<endl;
            }
        }
    }

    bson_destroy(query);
    bson_destroy(fields);
    mongo_cursor_destroy(cursor);

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    return 0;
}

/*s.10/18/2011 liyong */
int get_down_load_info_by_cdata_id(vector<string> id,vector<string>& path_url,
                                   vector<string>& charge_typeid,vector<string>& charge_typename,
                                   string default_charge_typeid, string default_charge_typename ){
/*e.10/18/2011 liyong */

    mongo *conn;
    vector<string>::iterator vec_it;
    int j = 0;
    char index[4];
    memset(index, 0x00, sizeof(index));
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_NG;
    }

    const char* key;
    bson query[1];
    bson fields[1];
    const bson *result;
    string tmp_result;
/*s.10/18/2011 liyong */
    string tmp_charge_typename;
/*e.10/18/2011 liyong */
    mongo_cursor *cursor = NULL;
    
//    id.clear();
    path_url.clear();
    charge_typeid.clear();

    //设置查询条件
    bson_init(query);
//    bson_append_string(query,"c_id",c_id.c_str());
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    for(vec_it = id.begin(); vec_it < id.end(); vec_it++)
    {
        sprintf(index,"%d", j);
        bson_append_string( query, index, vec_it->c_str() );
        UB_LOG_NOTICE("cdata id %d is %s",j,vec_it->c_str() );
        j++;
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_finish(query);
    //设置返回域
    bson_init(fields);
//    bson_append_int(fields,"id",1);
    bson_append_int(fields,"path_url",1);
    bson_append_int(fields,"charge_typeid",1);
//    bson_append_int(fields,"value",1);
    bson_finish(fields);

//    if( MONGO_OK == mongo_find_one(conn,"mcp.mcp_content_data",query,fields,result) ){
    cursor = mongo_find(conn, "mcp.mcp_content_data",query,fields, 100, 0, 0 );
    
    if( cursor == NULL )
    {
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        bson_destroy(query);
        bson_destroy(fields);
        UB_LOG_FATAL( "query content info from [mcp_content_data] faied"  );
        return MCP_NG;
    }
    UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content_data OK %s %d",__FILE__,__LINE__);
/*s.10/18/2011 liyong */
    int res;
/*e.10/18/2011 liyong */
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        result = mongo_cursor_bson( cursor );
        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
//            if( 0 == strcmp(key,"id")){
//                change_everything_to_string(it,tmp_result);
//                id.push_back(tmp_result);
            if( 0 == strcmp(key,"path_url")){
                change_everything_to_string(it,tmp_result);
                path_url.push_back( tmp_result);
            }else if( 0 == strcmp(key,"charge_typeid")){
                change_everything_to_string(it,tmp_result);
/*s.10/18/2011 liyong */
                if( tmp_result == "" )
                {
                    charge_typeid.push_back( default_charge_typeid );
                    charge_typename.push_back( default_charge_typename ); 
                }
                else
                {
                    charge_typeid.push_back( tmp_result);
                    res = get_charge_type_name_by_id( conn, tmp_result, tmp_charge_typename );
                    if( res != MCP_OK )
                    {
                        UB_LOG_FATAL( "get_charge_type_name_by_id by id[%s], [%s:%d]", tmp_result.c_str(),__FILE__,__LINE__);
                        tmp_charge_typename = "";
                    }
                    charge_typeid.push_back( tmp_charge_typename );
                }
/*e.10/18/2011 liyong */
            }
        }
                
    }

    if( path_url.empty() )
    {
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        bson_destroy(query);
        bson_destroy(fields);
        UB_LOG_FATAL( "query content info from [mcp_content_data] faied"  );
        return MCP_NG; 
    }

    bson_destroy(query);
    bson_destroy(fields);

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);

    return 0;
}

/*
int McpServlet::get_content_list_from_classID(const idl::mcp_get_content_list_from_classID_params& in,
                       idl::mcp_get_content_list_from_classID_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_content_list_from_classID is start");



//s.10/18/2011 liyong 
    const string class_id = in.class_id();
//e.10/18/2011 liyong 
    string content_type_id = in.content_type_id();
    if( content_type_id != MCP_CONTENT_THEME )
    {
        content_type_id = MCP_CONTENT_GAME;
    }
    const uint32_t priority = in.priority();
    const string info_id=in.info_id();
    const uint32_t info_type = in.info_type();
    const string img_size = in.img_size();
    const uint32_t unit_num = in.unit_num();
    const uint32_t unit_times = in.unit_times();
    int width;
    int image_type;
    int ret = 0;
    stringstream ss;

    //满足的条件个数
    int need_times = 0;
    
    //根据类别ID获得所有CID
    vector<content_info> v1;
    //根据适配信息得所有CID
    vector<string> v2;
    //根据图像大小获得CID
    vector<string> v3;

    get_content_id_list_by_class_id( content_type_id, class_id, v1);
    need_times++;

//    if( !info_id.empty() ){
//        ss.str("");
//        ss<<info_type;
//        get_content_id_list_by_stype_stypeid(ss.str(),info_id,v2);
//        need_times++;
//    }

    if( !img_size.empty()){
        //size_t pos;
        //pos = img_size.find("*");
        //string img_x = img_size.substr(0,pos);
        width = atoi(img_size.c_str());
        width = (width * 8) / 10;
        if( width <= 0 ){
            image_type = 3;
        }else if ( width > 160 ){
            image_type = 2;
        }else if ( width > 120 ){
            image_type = 3;
        }else if ( width > 50 ){
            image_type = 4;
        }else {
            image_type = 4;
        }
    }else{
        image_type = 3;
    }
   
    ss.str(""); 
    ss<<image_type;
    get_content_id_list_by_image_type_id(ss.str(),v3);
  
//    map<string,int> m;
//    map<string,int>::iterator iter;
//    int i = 0;
//    int cnt = 0;
//    for(i = 0 ; i < (int)v1.size(); i++){
//        m[v1[i].c_id] = 1;
       // cout<<"In v1 "<<i<<" "<<v1[i].c_id<<endl;
//    }

//    for(i = 0 ; i < (int)v2.size(); i++){
     //   cout<<"finding in map "<<v2[i]<<endl;
//        iter = m.find(v2[i]);
//        if( iter != m.end() ){
            //找到
//            cnt = iter->second;
//            cnt++;
//            m[v2[i]] = cnt;
//        }else{
            //没找到
//        }
//    }

    //整理结果
    int i = 0;
    vector<content_info> result;
    for(i = 0 ; i < (int)v1.size(); i++){
            result.push_back(v1[i]); 
    }
    //排序
    if( 1 == priority ){
        sort(result.begin(),result.end(),compare_content_info);
    }else{
        sort(result.begin(),result.end(),compare_content_info_by_logdate);
    }

    int start_index = (unit_times - 1) * unit_num;
    if(start_index >= (int)result.size()){
        return 0;
    }

    //cout<<"total count "<<result.size()<<endl;
    out.m_result_params()->set_count(result.size());

    mongo conn[1];
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return 0;
    }

    int page_cnt = get_cnt_this_page(result.size(),unit_times,unit_num);
    for( i = 0; i < page_cnt && start_index < (int)result.size() ; i++){
        string c_id = result[start_index].c_id;
        string c_name = result[start_index].c_name;
        string game_code = result[start_index].game_code;
        vector<string> path_url;
        vector<string> cdata_id;
        vector<string> charge_typeid;
//s.10/18/2011 liyong 
        vector<string> charge_typename;
//e.10/18/2011 liyong 
        string path_url_out;
        string cdata_id_out;
        string charge_typeid_out;
//s.10/18/2011 liyong 
        string charge_typename_out;
//e.10/18/2011 liyong 
        string wap_img_url;
        out.m_result_params()->set_content_list(i,0,c_id.c_str());
        out.m_result_params()->set_content_list(i,1,c_name.c_str());
        out.m_result_params()->set_content_list(i,2,game_code.c_str());
        if( !info_id.empty() ){
            ss.str("");
            ss<<info_type;
            ret = get_cdata_id_list_by_stype_stypeid(ss.str(),info_id,c_id, cdata_id);
            if ( MCP_OK != ret )
            {
                UB_LOG_FATAL("get_cdata_id_list_by_stype_stypeid is failed");
            }

//s.10/18/2011 liyong 
            string default_charge_typeid, default_charge_typename;
            ret = get_default_charge_type_by_cid( conn, content_type_id, c_id, default_charge_typeid );
            if( ret != MCP_OK )
            {
                UB_LOG_FATAL( "get default_charge_type_id failed by c_id[%s], [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
                default_charge_typeid = default_charge_typename = "";
            }
            else
            {
                ret = get_charge_type_name_by_id( conn, default_charge_typeid, default_charge_typename );
                if( ret != MCP_OK )
                {
                    UB_LOG_FATAL("get charge_type_name failed by id[%s], [%s:%d]",default_charge_typeid.c_str(),__FILE__,__LINE__); 
                    default_charge_typename = "";
                }
            }
            ret = get_down_load_info_by_cdata_id(cdata_id,path_url,charge_typeid,charge_typename,
                                                          default_charge_typeid,default_charge_typename);
//e.10/18/2011 liyong 
            if ( MCP_OK != ret )
            {
                UB_LOG_FATAL("get_down_load_info_by_cdata_id is failed");
            }
            else
            {
                for ( int j = 0; j < (int)cdata_id.size(); j++)
                {
                    if( !cdata_id_out.empty() )
                    {
                        cdata_id_out.append(","); 
                        path_url_out.append(",");
                        charge_typeid_out.append(",");
//s.10/18/2011 liyong 
                        charge_typename_out.append(",");
//e.10/18/2011 liyong 
                    }
                    cdata_id_out.append(cdata_id[j]);
                    path_url_out.append(path_url[j]);
                    charge_typeid_out.append(charge_typeid[j]);
//s.10/18/2011 liyong 
                    charge_typeid_out.append(charge_typename[j]);
//e.10/18/2011 liyong/
                }
            }
	    out.m_result_params()->set_content_list(i,3,cdata_id_out.c_str());
            out.m_result_params()->set_content_list(i,4,path_url_out.c_str());
	    out.m_result_params()->set_content_list(i,5,charge_typeid_out.c_str());
//s.10/18/2011 liyong 
            out.m_result_params()->set_content_list(i,7,charge_typename_out.c_str());
//e.10/18/2011 liyong 
        }

        //wap预览图
        vector<string>::iterator v_it = find(v3.begin(),v3.end(),c_id );
        if( v_it != v3.end() ){
            //找到
            ss.str("");
            ss<<image_type;
            get_image_by_content_id_iamge_type(c_id,ss.str(),wap_img_url);
        }else{
            //没找到
            get_image_by_content_id_iamge_type(c_id,"3",wap_img_url);
        }
        UB_LOG_NOTICE(" get_image_by_content_id_iamge_type is  OK");
        out.m_result_params()->set_content_list(i,6,wap_img_url.c_str());

        start_index++;
    }

    ul_writelog(UL_LOG_NOTICE,"get_content_list_from_classID is end");
    mongo_destroy( conn );
    return MCP_OK;
    
};
*/

int McpServlet::get_content_class(const idl::mcp_get_content_class_params& in,
                                 idl::mcp_get_content_class_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_content_class is start");


    const string content_type_id = in.content_type_id();

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    { 
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    const char* key;
    mongo_cursor cursor[1];
    bson query[1];
    bson fields[1];
    bson *result;
    int cnt = 0;
    string tmp_result;
    //设置查询条件
    bson_init(query);
    bson_append_string(query,"type_id",content_type_id.c_str());
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"id",1);
    bson_append_int(fields,"name",1);
    bson_finish(fields);

    mongo_cursor_init( cursor, conn, "mcp.mcp_content_class");
    mongo_cursor_set_query(cursor, query);
    mongo_cursor_set_fields(cursor, fields);

    while( mongo_cursor_next(cursor) == MONGO_OK ){
        result = &cursor->current;

        bson_iterator it[1];
        bson_iterator_init( it, result);

        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_content_class_id(cnt,tmp_result.c_str());
            }else if( 0 == strcmp(key,"name")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_content_class_name(cnt,tmp_result.c_str());
            }
        }
        cnt++;
    }


    bson_destroy(query);
    bson_destroy(fields);
    mongo_cursor_destroy(cursor);

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    ul_writelog(UL_LOG_NOTICE,"get_content_class is end");
    return 0;
}

/*
int McpServlet::get_content_class_name(const idl::mcp_get_content_class_name_params& in, 
                                                 idl::mcp_get_content_class_name_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_content_class_name is start");
    const string class_id = in.class_id();

    ul_writelog(UL_LOG_NOTICE,"get_content_class_name is end");
    return MCP_OK;
    


}

*/

/*
int McpServlet::get_content_info(const idl::mcp_get_content_info_params& in, 
                                                 idl::mcp_get_content_info_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_content_info is start");

//    if( check_mongo_conn( conn )!= MCP_OK )
//    {
//        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
//        return MCP_NG;
//    }

//    const string content_id = in.content_id();
//    const string info_id=in.info_id();
//    const uint32_t info_type = in.info_type();

    int if_out_permission = 0;

    bson result[1];
    stringstream ss;
    string tmp_result;

    const char* key;
//s.10/19/2011 liyong 
    string content_type_id = "";
//e.10/19/2011 liyong 
    string content_name = "";
    string class_id = "";
    string class_name = "";
    string wap_img_url = "";
    string ext_id = "";
    string game_info = "";
    string game_vserion = "";
    string game_method = "";
    string star = "";
    string net_work = "";
    string game_code = "";
    string cp_id = "";
    string cp_name = "";
    string p_id = "";
    string p_time = "";
    string p_end_date = "";
    string p_log_date = "";
    string p_modify_date = "";

    mongo_cursor *cursor = NULL;
    const bson *result_data;
    vector<string> cdata_id;
    vector<string> data_id;
    vector<string> data_url;
    vector<string> charge_id;
    string data_id_out;
    string data_url_out;
    string charge_id_out;
    vector<string>::iterator vec_it;
    char index[10];
    int j=0;
    int ret=0;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    map<string, string> m_class_id_name;
    get_content_class_id_name_map( conn, m_class_id_name );

//s.10/19/2011 liyong 
    ret = get_content_type_by_id( conn, content_id, content_type_id );
    if( ret != MCP_OK )
    {
        UB_LOG_FATAL( "get content_type_id by content_id[%s] failed, [%s;%d]", content_id.c_str(), __FILE__, __LINE__ );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }
//e.10/19/2011 liyong 

    bson query[1];
    bson fields[1];

    //设置查询条件
    bson_init(query);
    bson_append_string(query,"id",content_id.c_str());
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields,"id",1);
    bson_append_int(fields,"name",1);
    bson_append_int(fields,"class_id",1);
    bson_append_int(fields,"logdate",1);
    bson_append_int(fields,"ext_id",1);
    bson_append_int(fields,"info",1);
    bson_append_int(fields,"version",1);
    bson_append_int(fields,"method",1);
    bson_append_int(fields,"star",1);
    bson_append_int(fields,"network_id",1);
    bson_append_int(fields,"game_code",1);
    bson_append_int(fields,"cp_id",1);
    bson_append_int(fields,"grant_id",1);
    bson_append_int(fields,"default_charge_typeid",1);
    bson_append_int(fields,"default_charge_value",1);
    bson_append_int(fields,"modify_date",1);
    bson_finish(fields);

    bson_print(query);
    bson_print(fields);

//s.10/19/2011 liyong 

    if( content_type_id == MCP_CONTENT_GAME || content_type_id == MCP_ANDROID_GAME ) 
    {
        ret = mongo_find_one(conn,"mcp.mcp_game_content",query,fields,result);
    }
    else if( content_type_id == MCP_CONTENT_THEME )
    {
        ret = mongo_find_one(conn,"mcp.mcp_theme_content",query,fields,result);
    }
    else
    {
        UB_LOG_FATAL( "Unknown content_type %s, [%s:%d]", content_type_id.c_str(),__FILE__, __LINE__ );
        //mongo_destroy( conn );
        ret = mongo_find_one(conn,"mcp.mcp_game_content",query,fields,result);
    }
    //if( MONGO_OK == mongo_find_one(conn,"mcp.mcp_game_content",query,fields,result) ){
    if( ret == MONGO_OK ){
//e.10/19/2011 liyong 
        UB_LOG_NOTICE(" Find Data Info From mcp.mcp_content OK %s %d",__func__,__LINE__);
        bson_iterator it[1], sub[1];
        bson_iterator_init( it, result);
        string tmp_class_id, tmp_class_name;
        
        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if (0 == strcmp(key,"id")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(CONTENT_ID,tmp_result.c_str());
            }else if( 0 == strcmp(key,"name")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(CONTENT_NAME,tmp_result.c_str());
            }else if( 0 == strcmp(key,"class_id")){
                if( bson_iterator_type( it ) == BSON_STRING )
                {
                    change_everything_to_string(it,tmp_result);
                    out.m_result_params()->set_info_list(CONTENT_TYPE_ID,tmp_result.c_str());
                
                    get_class_name_by_class_id(tmp_result,class_name);
                    out.m_result_params()->set_info_list(CONTENT_TYPE_NAME,class_name.c_str());
                }
                else if( bson_iterator_type( it ) == BSON_ARRAY )
                {
                    bson_iterator_subiterator( it, sub );
                    while( bson_iterator_next( sub ) )
                    {
                        tmp_class_id = bson_iterator_string( it );

                        get_class_name_by_class_id(tmp_class_id,tmp_class_name);

                        if( !class_id.empty() )
                        {
                            class_id += ",";
                            class_name += ",";
                        }
                        class_id += tmp_class_id;
                        class_name += tmp_class_name;
                    }
                    out.m_result_params()->set_info_list(CONTENT_TYPE_ID,class_id.c_str());
                    out.m_result_params()->set_info_list(CONTENT_TYPE_NAME,class_name.c_str());
                }
            }else if( 0 == strcmp(key,"logdate")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(CONTENT_LOG_DATE,tmp_result.c_str());
            }else if( 0 == strcmp(key,"ext_id")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(EXT_INFO_ID,tmp_result.c_str());
            }else if( 0 == strcmp(key,"info")){
                out.m_result_params()->set_info_list(GAME_CONTENT_INFO,bson_iterator_string(it));
               // cout<<"info = "<<bson_iterator_string(it)<<endl;
            }else if( 0 == strcmp(key,"version")){
                out.m_result_params()->set_info_list(VERSION,bson_iterator_string(it));
                //cout<<"version = "<<bson_iterator_string(it)<<endl;
            }else if( 0 == strcmp(key,"method")){
                out.m_result_params()->set_info_list(METHOD,bson_iterator_string(it));
                //cout<<"method = "<<bson_iterator_string(it)<<endl;
            }else if( 0 == strcmp(key,"star")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(STAR,tmp_result.c_str());
                //cout<<"STAR = "<<tmp_result<<endl;
            }else if( 0 == strcmp(key,"network_id")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(NET_WORK,tmp_result.c_str());
            }else if( 0 == strcmp(key,"game_code")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(GAME_CODE,tmp_result.c_str());
            }else if( 0 == strcmp(key,"default_charge_typeid")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(DEFAULT_CHARGE_TYPEID,tmp_result.c_str());
            }else if( 0 == strcmp(key,"default_charge_value")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(DEFAULT_CHARGE_VAL,tmp_result.c_str());
            }else if( 0 == strcmp(key,"modify_date")){
                change_everything_to_string(it,tmp_result);
                UB_LOG_NOTICE("modify_date is %s", tmp_result.c_str());
                out.m_result_params()->set_info_list(CONTENT_MOD_DATE,tmp_result.c_str());
            }else if( 0 == strcmp(key,"cp_id")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(CP_ID,tmp_result.c_str());

                get_cp_name_by_cp_id(tmp_result,cp_name);
                out.m_result_params()->set_info_list(CP_NAME,cp_name.c_str());
            }else if( 0 == strcmp(key,"grant_id")){
                change_everything_to_string(it,tmp_result);
                out.m_result_params()->set_info_list(GRANT_ID,tmp_result.c_str());

                if_out_permission = get_permission_by_p_id(tmp_result,p_time,p_end_date,p_modify_date);
                out.m_result_params()->set_info_list(GRANT_DATE,p_time.c_str());
                out.m_result_params()->set_info_list(SIGN_ENDDATE,p_end_date.c_str());
//                out.m_result_params()->set_info_list(CONTENT_MOD_DATE,p_modify_date.c_str());

            }
        }
        if(  content_type_id == MCP_CONTENT_THEME )
        {
            ret = get_theme_class_info( conn, content_id, m_class_id_name, class_id, class_name );
            if( ret != MCP_OK )
            {
                UB_LOG_FATAL( "get_theme_class_info failed, [%s:%d]", __FILE__, __LINE__ );
            }
            out.m_result_params()->set_info_list(CONTENT_TYPE_ID,class_id.c_str());
            out.m_result_params()->set_info_list(CONTENT_TYPE_NAME,class_name.c_str());
        }
        bson_destroy(result);
    }
    
    bson_destroy(query);
    bson_destroy(fields);

    //查询WAP预览图
    get_image_by_content_id_iamge_type(content_id,"3",wap_img_url);
    out.m_result_params()->set_info_list(WAP_PREVIEW_URL,wap_img_url.c_str());

    //查询下载信息
    if( !info_id.empty() )
    {
          
        //设置查询条件
//        bson_init(query);
//        ss.str("");
//        ss<<info_type;
//        bson_append_string(query,"stype",ss.str().c_str());
//        bson_append_string(query,"stype_id",info_id.c_str());
//        bson_append_string(query,"c_id",content_id.c_str());
//        bson_finish(query);
//        //设置返回域
//        bson_init(fields);
//        bson_append_int(fields,"cdata_id",1);
//        bson_finish(fields);
//        cursor = mongo_find(conn, "mcp.mcp_content_format",query,fields, 100, 0, 0 );
//        if( cursor == NULL )
//        {
 //           //mongo_destroy( conn );
//            bson_destroy(query);
//            bson_destroy(fields);
//  /          UB_LOG_FATAL( "query content info from [mcp_content_data] faied"  );
//            return MCP_NG;
//        }
//        UB_LOG_NOTICE(" Find Data Info From mcp_content_format OK %s %d",__func__,__LINE__);
//        while( mongo_cursor_next( cursor ) == MONGO_OK )
//  /      {
//	    result_data = mongo_cursor_bson( cursor );
//	    bson_iterator it[1];
//	    bson_iterator_init( it, result_data);
//
//            while( bson_iterator_next(it) )
//            {
//	        key = bson_iterator_key(it);
//	        if( 0 == strcmp(key,"cdata_id"))
//                {
//                    UB_LOG_NOTICE(" Find Document From mcp.mcp_content_data OK %s %d",__func__,__LINE__);
//		    change_everything_to_string(it,tmp_result);
//    cdata_id.push_back(tmp_result);
//设置返回值信息
//                        out.m_result_params()->set_info_list(CONTENT_DATA_ID,data_id.c_str());
//                        out.m_result_params()->set_info_list(CONTENT_DATA_URL,data_url.c_str());
//                        out.m_result_params()->set_info_list(CONTENT_DATA_CHARGE,charge_id.c_str());
//        }
//    }
//        }
//        bson_destroy(query);
//        bson_destroy(fields);

        ss.str("");
        ss<<info_type;
        ret = get_cdata_id_by_type( conn, content_id, ss.str(), info_id, cdata_id );
        
        if ( ret < 0 )
        {   
            UB_LOG_FATAL("get_cdata_id_by_type is failed, c_id is %s, stype is %s, stype_id is %s",
                               content_id.c_str(), ss.str().c_str(), info_id.c_str() );
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            return MCP_OK;
        }  

        bson_init(query);
        bson_append_start_object(query,"id");
        bson_append_start_array(query,"$in");
        for(vec_it = cdata_id.begin(); vec_it < cdata_id.end(); vec_it++)
        {
            sprintf(index,"%d", j);
            bson_append_string( query, index, vec_it->c_str() ); 
            j++;
        }
        bson_append_finish_object( query );
        bson_append_finish_object( query );
        bson_finish(query);
        //设置返回域
        bson_init(fields);
        bson_append_int(fields,"id",1);
        bson_append_int(fields,"path_url",1);
        bson_append_int(fields,"charge_typeid",1);
//        bson_append_int(fields,"path_type",1);
//        bson_append_int(fields,"media_format_id",1);
//        bson_append_int(fields,"size",1);
//        bson_append_int(fields,"mtype",1);
        bson_finish(fields);
        //打印过滤条件和返回列名称
//        mongo_cursor_init( cursor, conn, "MCP.mcp_content_data" );
//        mongo_cursor_set_query(cursor,query);
        cursor = mongo_find(conn, "mcp.mcp_content_data",query,fields, 100, 0, 0 );
        if( cursor == NULL )
        {
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            bson_destroy(query);
            bson_destroy(fields);
            UB_LOG_NOTICE( "query content info from [mcp_content_data] faied\n"  );
            return 0;
        }
        while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
            UB_LOG_NOTICE(" Find Data Info from mcp.mcp_content_data  OK %s",__func__);
            result_data = mongo_cursor_bson( cursor );
            bson_iterator it[1];
            bson_iterator_init( it, result_data);

            while( bson_iterator_next(it) ){
                key = bson_iterator_key(it);
                if( 0 == strcmp(key,"id")){
                    data_id.push_back(bson_iterator_string(it));
                    //cout<<"data_id "<<data_id<<endl;
                }else if( 0 == strcmp(key,"path_url")){
                    data_url.push_back(bson_iterator_string(it));
                    //cout<<"data_url "<<data_url<<endl;
                }else if( 0 == strcmp(key,"charge_typeid")){
                    charge_id.push_back( bson_iterator_string(it));
                    //cout<<"charge_id "<<charge_id<<endl;
                }
            }
            //bson_destroy(result);
            bson_destroy(query);
            bson_destroy(fields);

        }
        for (  j = 0; j < (int)data_id.size(); j++)
        {
            if( !data_id_out.empty() )
            {
                data_id_out.append(",");
                data_url_out.append(",");
                charge_id_out.append(",");
            }
            data_id_out.append(data_id[j]);
            data_url_out.append(data_url[j]);
            charge_id_out.append(charge_id[j]);
        }
		//设置返回值信息
        out.m_result_params()->set_info_list(CONTENT_DATA_ID,data_id_out.c_str());
        out.m_result_params()->set_info_list(CONTENT_DATA_URL,data_url_out.c_str());
        out.m_result_params()->set_info_list(CONTENT_DATA_CHARGE,charge_id_out.c_str());
    }


    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    mongo_cursor_destroy( cursor );

    //授权已经过期
   // if( if_out_permission == -2 ){
     //   for(int i = 0; i < 22; i++){
       //     out.m_result_params()->set_info_list(i,"");
       // }
   // }
    ul_writelog(UL_LOG_NOTICE,"get_content_info is end");
    return MCP_OK;

}
*/

/*
int McpServlet::get_download_info(const idl::mcp_get_download_info_params& in, 
                                                 idl::mcp_get_download_info_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_download_info is start");
    const string content_id = in.content_id();
    const string info_id=in.info_id();
    const uint32_t info_type = in.info_type();

    ul_writelog(UL_LOG_NOTICE,"get_download_info is end");
    return MCP_OK;

}



int McpServlet::get_content_list(const idl::mcp_get_content_list_params& in, 
                                                 idl::mcp_get_content_list_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_content_list is start");
    const uint32_t priority = in.priority();
    const string info_id=in.info_id();
    const uint32_t info_type = in.info_type();
    const uint32_t unit_num = in.unit_num();
    const uint32_t unit_times = in.unit_times();

    ul_writelog(UL_LOG_NOTICE,"get_content_list is end");
    return MCP_OK;

}


int McpServlet::get_content_list_from_part(const idl::mcp_get_content_list_from_part_params& in, 
                                                 idl::mcp_get_content_list_from_part_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_content_list_from_part is start");
    const string part_id = in.part_id();
    const string info_id=in.info_id();
    const uint32_t info_type = in.info_type();
    const uint32_t unit_num = in.unit_num();
    const uint32_t unit_times = in.unit_times();

    ul_writelog(UL_LOG_NOTICE,"get_content_list_from_part is end");
    return MCP_OK;

}

*/
