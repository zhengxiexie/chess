#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"
#include "tools.h"
#include "UADicTree.h"

using namespace std;


extern UADicTree uaTree;

//#define REAL_TIME_DATA

#define PHONE_NAME_ST_TIMEOUT 600
//有效期
#define PHONE_INFO_ST_EXPIRED 60*24*60
//函数插入cache超时时间
#define PHONE_INFO_ST_TIMEOUT 100
//系列过期时间和超时时间
#define PHONE_SERIES_ST_EXPIRED 60*24*60
#define PHONE_SERIES_ST_TIMEOUT 100

//字典树过期时间 单位秒
#define DIC_TREE_EXPIRED 24*60*60

const int PHONE_INFO_CNT = 10;
enum  phone_baseinfo{
    PHONE_ID =   0,             //  机型ID 
    PHONE_BRAND ,               //  品牌 
    PHONE_MODEL,                //  型号 
    PHONE_OS_ID,                //  操作系统ID
    PHONE_OS_NAME,              //  操作系统名称 
    PHONE_SERIES_ID,            //  系列ID 
    PHONE_SERIES_NAME,          //  系列名称
    PHONE_RESOLU,               //  屏幕分辨率
    PHONE_SCREEN_WORDS,         //  屏幕字数
    PHONE_PAGE_VERSION,          //  页面版本
    PHONE_PLAT_VERSION
};

typedef struct {
    char id[12];
    char name[201];
    char brand_name[201];
}phone_name_st;

//手机基本信息结构体
typedef struct {
    char id[12];                //机型id
    char brand_name[201];          //品牌
    char name[201];             //型号
    char plat_id[12];           //操作系统(平台)id
    char plat_name[201];        //操作系统(平台)名称
    char res_info[19];          //屏幕分辨率
    char page_size[20];         //屏幕字数
    char page_type[21];         //页面版本 1简 2彩 4 触 位或的结果
    char plat_version[16];
}phone_info_st;

//手机系列信息结构体
typedef struct{
    char series_id[12];         //系列id
    char series_name[201];      //系列名称
}phone_series_st;

/**
  *根据PID获得手机系列总数(CACHE)
  *
  *@param [in]  pid               指定手机的PID
  *
  * @return    int                手机所属系列个数 小于 0 失败
  */
int get_phone_series_cnt_by_pid_fromCache(string pid)
{
    int ret = 0;
    //int cnt = 0;
    string mainkey( "mcp_series_count" );
    string subkey = pid;
    string valueout;
    
    mcp_session conn;

    ret = conn.session_search( mainkey, subkey, valueout );
    if ( 0 == ret )
    {
        UB_LOG_NOTICE("[FOUND IN CACHE] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str() , subkey.c_str());
        return atoi( valueout.c_str() );
    }
    else
    {
        UB_LOG_NOTICE("[ERROR IN CACHE] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str() , subkey.c_str() );
        return -1;
    }
}
/**
  *@brief 插入PID对应的手机系列系列总数到CACHE
  *
  *@param [in]  pid               指定手机的PID
  *@param [in]  cnt               该手机对应的系列总数
  *
  *@return int  0 成功 -1 失败
  */
int insert_phone_series_cnt_by_pid_toCache(string pid,int cnt)
{
    int ret = 0;
    if( cnt < 0 ){
        return -1;
    }
    string mainkey( "mcp_series_count" );
    string subkey = pid;
    char num[12];
    sprintf(num, "%d", cnt);
    string value = num;
    mcp_session conn;
    ret = conn.session_insert(mainkey, subkey, value, PHONE_SERIES_ST_EXPIRED );
    if (0 != ret)
    {
        UB_LOG_NOTICE("[INSERT REDIS ERROR] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str());
        return -1;
    }
    UB_LOG_NOTICE("[INSERT REDIS SUCCESS] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str());
    return 0;
}
/**
  *@brief  根据PID获得手机系列总数(DB)
  *
  *@param [in]  pid               指定手机的PID
  *
  * @return    int                手机所属系列个数 小于 0 失败
  */
int get_phone_series_cnt_by_pid_fromDB(string pid){

    MysqlConnection *database_connection;
    string sql_count = "SELECT count(*) FROM `mcp_phone_series_list` WHERE `mcp_phone_series_list`.`pid`  =  " + pid + " ";

    int err = 0;
    int ret = 0;
    
    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        UB_LOG_NOTICE("[GET PHONE_SERIES_CNT BY PID CONNECT DB ERROR] %s",__func__);
        return -1;
    }
    UB_LOG_NOTICE("%s { %s }",__func__,sql_count.c_str());
    ret = database_connection->selectCount(sql_count.c_str());
    database_pool->putBackConnection(database_connection, false);
    return ret;
}
/**
  *@brief  根据PID获得手机系列总数
  *
  *@param [in]  pid               指定手机的PID
  *
  * @return    int                手机所属系列个数 小于 0 失败
  */
int get_phone_series_cnt_by_pid(string pid){
    int cache_flag = 0;
    int cnt = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    cnt = get_phone_series_cnt_by_pid_fromCache(pid);
    if(cnt < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;  
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        cnt = get_phone_series_cnt_by_pid_fromDB(pid);
        if( cnt < 0 ){
            //DB query error
            UB_LOG_NOTICE("[GET PHONE SERIES CNT ERROR] %s pid = %s ", __func__,pid.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("[GET PHONE SERIES CNT OK] %s pid = %s CNT = %d", __func__,pid.c_str(),cnt);
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_series_cnt_by_pid_toCache(pid,cnt);
#endif
        }
    }
    return cnt;
}

/**
  *根据PID获得手机系列信息(CACHE)
  *
  *@param [in]  pid               指定手机的PID
  *@param [in]  cnt               该手机对应的系列总数
  *@param [out] result            指定手机的系列信息结构体
  *
  * @return    int         0 成功, 小于 0 失败
  */
int get_phone_series_by_pid_fromCache(string pid,int cnt,phone_series_st *result)
{
    int ret = 0;
    if( NULL == result || cnt < 0 ){
        return -1;
    }

    string mainkey = "mcp_series_info";
    string subkey = pid;
    string valueout;

    mcp_session conn;
    ret = conn.session_search( mainkey, subkey, valueout );
    if ( 0 == ret )
        UB_LOG_NOTICE("[FOUND IN SESSION] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str(), subkey.c_str() );
    else
    {
        UB_LOG_NOTICE("[ERROR IN session] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str(), subkey.c_str() );
        return -1;
    }

    Json::Reader json_reader;
    Json::Value json_value;
    string tmpmsg;
    size_t pos= 0;
    int k = 0;
    while( (pos = valueout.find_first_of( '{', pos )) < valueout.size() )
    {
        tmpmsg.assign( valueout, pos, valueout.find_first_of( '}', pos ) - pos +1 );
        ++pos;
        if(json_reader.parse(tmpmsg,json_value))
        {
            strlcpy( result[k].series_id, json_value["series_id"].asString().c_str(), sizeof(result[k].series_id) );
            strlcpy( result[k].series_name, json_value["series_name"].asString().c_str(), sizeof(result[k].series_name) );
        }
        k++;
    }
    return 0;
}

/**
  *@brief 插入PID对应的手机系列信息到CACHE
  *
  *@param [in]  pid               指定手机的PID
  *@param [in]  cnt               该手机对应的系列总数
  *@param [in] result             指定手机的相关信息结构体
  *
  *@return int  0 成功 -1 失败
  */
int insert_phone_series_by_pid_toCache(string pid,int cnt,phone_series_st *result)
{
    int ret = 0;
    if( NULL == result || cnt < 0 ){
        return -1;
    }

    string mainkey = "mcp_series_info";
    string subkey = pid;
    string value;
    string series_id;
    string series_name;
    
    value.clear();
    for( int i=0; i< cnt; i++)
    {
        series_id.clear();
        series_name.clear();
        series_id = result[i].series_id;
        series_name = result[i].series_name;
        value += "{\"series_id\":\"" + series_id + "\",\"series_name\":\"" + series_name + "\"}" ;
    }

    mcp_session conn;
    ret = conn.session_insert(mainkey, subkey, value, PHONE_SERIES_ST_EXPIRED);
    if (0 != ret)
    {
        UB_LOG_NOTICE("[INSERT SESSION ERROR] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str() );
        return -1;
    }
    UB_LOG_NOTICE("[INSERT SESSION SUCCESS] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str() );
    return 0;
}

/**
  *根据PID获得手机系列信息(DB)
  *
  *@param [in]  pid               指定手机的PID
  *@param [in]  cnt               该手机对应的系列总数
  *@param [out] result            指定手机的系列信息结构体
  *
  * @return    int         0 成功, 小于 0 失败
  *
  *@return int                  机型个数，小于0表示查询出错
  */
int get_phones_series_by_pid_fromDB(string pid,int cnt, phone_series_st* result){

    UB_LOG_NOTICE("[Start Search Series From DB] %s : pid = %s ", __func__,pid.c_str());
    int err = 0;
    int ret = 0;
    MysqlConnection *database_connection;
    MyclientRes data_res;
    string sql =  "SELECT  `mcp_phone_series_list`.`sid` ,  `mcp_phone_series`.`name`  FROM  `mcp_phone_series_list` LEFT JOIN  `mcp_phone_series` ON  `mcp_phone_series_list`.`sid` =  `mcp_phone_series`.`id` WHERE  `mcp_phone_series_list`.`enable` = 1 AND  `mcp_phone_series`.`enable` =1 AND `mcp_phone_series_list`.`pid` = " + pid;

    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        UB_LOG_NOTICE("[Search Series From DB Connect ERROR] %s : pid = %s ", __func__,pid.c_str());
        data_res.free();
        return -1;
    }

    UB_LOG_NOTICE("%s { %s }",__func__,sql.c_str());
    ret = database_connection->query(sql.c_str(), &data_res, true);
    
    if ( ret < 0 ){
        UB_LOG_NOTICE("[Search Series From DB ERROR] %s : pid = %s ", __func__,pid.c_str());
        ret = database_pool->putBackConnection(database_connection, false);
        if (ret != 0){
            UB_LOG_NOTICE("[Search Series PutBack Connection ERROR] %s : pid = %s ", __func__,pid.c_str());
        }
        data_res.free();
        return -1;
    }
    //分配数组的大小 大于查出来的大小
    if( cnt != ret ){
        UB_LOG_NOTICE("[Search Series CNT CACHE and DB DOES NOT MATCH ERROR] %s : pid = %s ", __func__,pid.c_str());
        if(cnt > ret){
            cnt = ret;
        }
    }
    //遍历结果
    UB_LOG_NOTICE("[Save Reuslt From DB INTO result] %s : pid = %s ", __func__,pid.c_str());
    for ( int i = 0; i < cnt ; i++){
        
//        UB_LOG_NOTICE("[Saving Reuslt From DB INTO result] index = %d function %s id = %s name = %s brand_name = %s  ",i,__func__,data_res[i][0],data_res[i][1],data_res[i][2]);
        copy_result(result[i].series_id,data_res[i][0]);
        copy_result(result[i].series_name,data_res[i][1]);
    }
    UB_LOG_NOTICE("[Save Reuslt From DB INTO result (OK)] %s : pid = %s ", __func__,pid.c_str());
    //释放资源
    database_pool->putBackConnection(database_connection, false);
    data_res.free();
    return cnt;
}

/**
  *@brief  根据PID获得手机系列所有名称和所有ID
  *
  *@param [in]  pid               指定手机的PID
  *
  * @return    int                手机所属系列个数 小于 0 失败
  */
int get_phone_series_by_pid(string pid,string *series_ids, string *series_names){
    int cache_flag = 0;
    int cnt = 0;
    int ret = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    cnt = get_phone_series_cnt_by_pid_fromCache(pid);
    if(cnt < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;  
       UB_LOG_NOTICE("[GET PHONE SERIES CNT OK] %s pid = %s CNT = %d", __func__,pid.c_str(),cnt);
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        cnt = get_phone_series_cnt_by_pid_fromDB(pid);
        if( cnt < 0 ){
            //DB query error
            UB_LOG_NOTICE("[GET PHONE SERIES CNT ERROR] %s pid = %s ", __func__,pid.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("[GET PHONE SERIES CNT OK] %s pid = %s CNT = %d", __func__,pid.c_str(),cnt);
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_series_cnt_by_pid_toCache(pid,cnt);
#endif
        }
    }
    if( cnt <= 0 ){
        return -1;
    }
    phone_series_st *result = (phone_series_st*) malloc( sizeof(phone_series_st) * cnt); 
    if( NULL == result ){
        return -1;
    }
    memset(result,0x00,sizeof(phone_series_st) * cnt);
    //重新初始化cache_flag
    cache_flag = 0;
#ifndef REAL_TIME_DATA
    ret = get_phone_series_by_pid_fromCache(pid,cnt,result);
    if(ret < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;  
    }
#endif
    if( 0 == cache_flag ){
        ret = get_phones_series_by_pid_fromDB(pid,cnt,result);
        if( ret < 0 ){
            //DB query error
            UB_LOG_NOTICE("[GET PHONE SERIES LIST ERROR] %s pid = %s ", __func__,pid.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("[GET PHONE SERIES LIST OK] %s pid = %s ", __func__,pid.c_str());
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_series_by_pid_toCache(pid,cnt,result);
#endif
        }
    }
    series_ids->clear();
    series_names->clear();
    *series_ids = *series_ids + result[0].series_id;
    *series_names = *series_names + result[0].series_name;
    for(int i = 1; i < cnt; i++){
        *series_ids = *series_ids + ","+ result[i].series_id;
        *series_names = *series_names + "," + result[i].series_name;
    }
    UB_LOG_NOTICE("[SET PHONE SERIES LIST OK] %s pid = %s", __func__,pid.c_str());
    free(result);
    return cnt;
}

/**
  *根据PID获得手机信息(CACHE)
  *
  *[in]  pid               指定手机的PID
  *[out] result            指定手机的相关信息结构体
  *
  * @return    int         0 成功, 小于 0 失败
  */
int get_phone_info_by_pid_fromCache(string pid,phone_info_st *result)
{
    int ret = 0;
    string mainkey = "mcp_phone_info";
    string subkey = pid;
    string valueout;
    mcp_session conn;
    ret = conn.session_search( mainkey, subkey, valueout );
    if ( valueout.length() <= 0 )
    {
        UB_LOG_NOTICE("[ NOT IN SESSION ] %s : pid = %s MAINKEY = %s SUBKEY = %s ", __func__,pid.c_str(),mainkey.c_str(),subkey.c_str() );
        return -1;
    }
    else if ( 0 == ret )
        UB_LOG_NOTICE("[ FOUND IN SESSION ] %s : pid = %s MAINKEY = %s SUBKEY = %s ", __func__,pid.c_str(),mainkey.c_str(),subkey.c_str() );
    else
    {
        UB_LOG_NOTICE("[ ERROR IN SESSION ] %s : pid = %s MAINKEY = %s SUBKEY = %s ", __func__,pid.c_str(),mainkey.c_str(),subkey.c_str() );
        return -1;
    }

    Json::Reader json_reader;
    Json::Value json_value;
    string tmpmsg;
    size_t pos= 0;
    while( (pos = valueout.find_first_of( '{', pos )) < valueout.size() )
    {
        tmpmsg.assign( valueout, pos, valueout.find_first_of( '}', pos ) - pos +1 );
        ++pos;
        if(json_reader.parse(tmpmsg,json_value))
        {
            strlcpy( result->id, json_value["id"].asString().c_str(), sizeof(result->id) );
            strlcpy( result->brand_name, json_value["brand_name"].asString().c_str(), sizeof(result->brand_name) );
            strlcpy( result->name, json_value["name"].asString().c_str(), sizeof(result->name) );
            strlcpy( result->plat_id, json_value["plat_id"].asString().c_str(), sizeof(result->plat_id) );
            strlcpy( result->plat_name, json_value["plat_name"].asString().c_str(), sizeof(result->plat_name) );
            strlcpy( result->res_info, json_value["res_info"].asString().c_str(), sizeof(result->res_info) );
            strlcpy( result->page_size, json_value["page_size"].asString().c_str(), sizeof(result->page_size) );
            strlcpy( result->page_type, json_value["page_type"].asString().c_str(), sizeof(result->page_type) );
            strlcpy( result->plat_version, json_value["plat_version"].asString().c_str(), sizeof(result->plat_version) );
        }
    }
    return 0; 
}

/**
  *@brief 插入PID对应的手机信息到CACHE
  *
  *@param [in]  pid               指定手机的PID
  *@param [out] result            指定手机的相关信息结构体
  *
  *@return int  0 成功 -1 失败
  */
int insert_phone_info_by_pid_toCache(string pid,phone_info_st *result)
{
    int ret = 0;
    string mainkey = "mcp_phone_info";
    string subkey = pid;

    string value;
    string id;
    string brand_name;
    string name;
    string plat_id;
    string plat_name;
    string res_info;
    string page_size;
    string page_type;
    string plat_version;
    if( result != NULL )
    {
        id.clear(); id = result->id;
	brand_name.clear(); brand_name = result->brand_name;
	name.clear(); name = result->name;
	plat_id.clear(); plat_id = result->plat_id;
	plat_name.clear(); plat_name = result->plat_name;
	res_info.clear(); res_info = result->res_info;
	page_size.clear(); page_size = result->page_size;
	page_type.clear(); page_type = result->page_type;
	plat_version.clear(); plat_version = result->plat_version;
        value += "{\"id\":\"" + id + "\",\"brand_name\":\"" + brand_name + "\",\"name\":\"" + name + "\",\"plat_id\":\""+ plat_id + "\",\"plat_name\":\"" + plat_name +"\",\"res_info\":\"" + res_info +"\",\"page_size\":\"" + page_size +"\",\"page_type\":\"" + page_type +"\",\"plat_version\":\"" + plat_version + "\"}" ;
    UB_LOG_WARNING("value is %s\n", value.c_str() );
    }
    mcp_session conn;
    ret = conn.session_insert(mainkey, subkey, value, PHONE_INFO_ST_EXPIRED);
    if (0 != ret)
    {
        UB_LOG_NOTICE("[INSERT SESSION ERROR] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(),subkey.c_str() );
        return -1;
    }
    UB_LOG_NOTICE("[INSERT CACHE SUCCESS] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str() );
    return 0;
}
/**
  *@brief 根据PID获得手机信息(DB)
  *
  *@param [in]  pid               指定手机的PID
  *@param [out] result            指定手机的相关信息结构体
  *
  * @return int                   0成功，小于0失败
  */
int get_phone_info_by_pid_fromDB(string pid,phone_info_st *result){

    MyclientRes data_res;
    MysqlConnection *database_connection;
    char query_sql[2048];
    int err = 0;
    int ret = 0;
    int rows = 0;
    int cols = 0;

    if(NULL == result ){
        return -1;
    }
    //所有结果数据清0
    memset(result,0x00,sizeof(phone_info_st));
    //sql语句清空
    memset(query_sql, 0x00, sizeof(query_sql));
    // 从连接池中获取一个连接
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        UB_LOG_NOTICE("[GET PHONE_INFO BY PID CONNECT DB ERROR] %s",__func__);
        data_res.free();
        return MCP_NG;
    }
    //get basic information
    sprintf(query_sql, "SELECT `mcp_phone_info`.`id` ,  `mcp_phone_brand`.`name`,  `mcp_phone_info`.`name` , "
            "`mcp_phone_plat`.`id` ,  `mcp_phone_plat`.`name` , `mcp_phone_res`.`name` ,  `mcp_phone_info`.`page_id`, `mcp_phone_plat`.`version` "
            "FROM  `mcp_phone_info` "
            "LEFT JOIN  `mcp_phone_plat` ON  `mcp_phone_info`.`plat_id` =  `mcp_phone_plat`.`id` "
            "LEFT JOIN  `mcp_phone_brand` ON  `mcp_phone_info`.`brand_id` =  `mcp_phone_brand`.`id` "
            "LEFT JOIN  `mcp_phone_res` ON  `mcp_phone_info`.`res_id` =  `mcp_phone_res`.`id` "
            "WHERE  `mcp_phone_info`.`id` = %s",pid.c_str());
    UB_LOG_NOTICE("%s { %s }",__func__,query_sql);
    ret = database_connection->query(query_sql, &data_res, true);
    if(ret > 0){
        //success
        copy_result(result->brand_name, data_res[0][1]);
        copy_result(result->name, data_res[0][2]);
        copy_result(result->plat_id, data_res[0][3]);
        copy_result(result->plat_name, data_res[0][4]);
        copy_result(result->res_info , data_res[0][5]);
        copy_result(result->page_type, data_res[0][6]);
        copy_result(result->plat_version, data_res[0][7]);
    }else{
        //error or empty
    }
//  data_res.free();
    //get the total characters of the phone per screen
    //rows 
    sprintf(query_sql,"SELECT  `value` "
            "FROM  `mcp_phone_property_list` "
            "WHERE  `propertyid` = 9 "
            "AND `pid` = %s "
            "AND `enable` = 1",pid.c_str());
    UB_LOG_NOTICE("%s { %s }",__func__,query_sql);
    ret = database_connection->query(query_sql, &data_res, true);
    if(ret > 0){
        //success
        if( NULL == data_res[0][0] || 0 == strcmp("",data_res[0][0])){
            rows = 0;
        }else{
            rows = atoi(data_res[0][0]);
        }
    }else{
        //error or empty
        rows = 0;
    }
//  data_res.free();
    //cols
    sprintf(query_sql,"SELECT  `value` "
            "FROM  `mcp_phone_property_list` "
            "WHERE  `propertyid` = 32 "
            "AND `pid` = %s "
            "AND `enable` = 1",pid.c_str());
    UB_LOG_NOTICE("%s { %s }",__func__,query_sql);
    ret = database_connection->query(query_sql, &data_res, true);
    if(ret > 0){
        //success
        if( NULL == data_res[0][0] || 0 == strcmp("",data_res[0][0])){
            cols = 0;
        }else{
            cols = atoi(data_res[0][0]);
        }
    }else{
        //error or empty
        cols = 0;
    }
    //保存屏幕字数
    char tmp_buff[256];
    snprintf(tmp_buff,256,"%d",rows * cols);
    copy_result(result->page_size,tmp_buff);
    ret = database_pool->putBackConnection(database_connection, false);
    if (ret != 0){
        ul_writelog(UL_LOG_FATAL, "putback connection is failed");
    }
    data_res.free();
    return 0;
}

/**
  *@brief 根据PID获得手机信息(DB)
  *
  *@param [in]  pid               指定手机的PID
  *@param [out] result            指定手机的相关信息结构体
  *
  * @return int                   0成功，小于0失败
  */
int get_phone_info_by_pid(string pid,phone_info_st *result){
    int cache_flag = 0;
    int ret = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    ret = get_phone_info_by_pid_fromCache(pid,result);
    if(ret < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;  
       UB_LOG_NOTICE("[GET PHONE INFO OK] %s pid = %s ", __func__,pid.c_str());
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        ret = get_phone_info_by_pid_fromDB(pid,result);
        if( ret < 0 ){
            //DB query error
            UB_LOG_NOTICE("[GET PHONE INFO ERROR] %s pid = %s ", __func__,pid.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("[GET PHONE INFO OK] %s pid = %s ", __func__,pid.c_str());
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_info_by_pid_toCache(pid,result);
#endif
        }
    }
    return ret;
}


int McpServlet::get_phone_info(const idl::mcp_get_phone_info_params& in, idl::mcp_get_phone_info_response& out){
    UB_LOG_NOTICE("+++++get_phone_info start, [%s]+++++", in.user_info());

    const string user_info = in.user_info();
    const uint32_t user_type = in.user_type();
//    const uint32_t phone_info_type = in.phone_info_type();

    if( user_info.size() > 1024 )
    {
        UB_LOG_WARNING( "user_info[%s] is too long, [%s:%d]", user_info.c_str(), __FILE__, __LINE__ );
        return 0;
    }

    if ( 0 ==  user_type ){
        //user_info 代表的是UA信息
        //if(uaTree.isTimeout(DIC_TREE_EXPIRED)){
        //    UB_LOG_NOTICE("[DIC TREE EXPIRED]");
        //    uaTree.delete_trie();
        //    uaTree.create_trie_list();
        //    uaTree.updateCreateTime();
        //}
        vector<string> pids;
        uaTree.Search(user_info,&pids);
        int cnt = pids.size();
        UB_LOG_NOTICE("[UA %s MATCH %d PIDS]",user_info.c_str(), cnt);
        
        for(int i = 0; i < cnt; i++){
            string pid = pids[i];
            phone_info_st pif;
            string phone_series_ids;
            string phone_series_names;
            //获得手机基本信息
            memset(&pif,0x00,sizeof(phone_info_st));
            get_phone_info_by_pid(pid,&pif);
            //获得手机系列信息 
            get_phone_series_by_pid(pid,&phone_series_ids,&phone_series_names);
            //设置返回值
            out.m_result_params()->set_phone_info(i,PHONE_ID,pid.c_str());
            out.m_result_params()->set_phone_info(i,PHONE_BRAND,pif.brand_name);
            out.m_result_params()->set_phone_info(i,PHONE_MODEL,pif.name);
            out.m_result_params()->set_phone_info(i,PHONE_OS_ID,pif.plat_id);
            out.m_result_params()->set_phone_info(i,PHONE_OS_NAME,pif.plat_name);
            out.m_result_params()->set_phone_info(i,PHONE_SERIES_ID,phone_series_ids.c_str());
            out.m_result_params()->set_phone_info(i,PHONE_SERIES_NAME,phone_series_names.c_str());
            out.m_result_params()->set_phone_info(i,PHONE_RESOLU,pif.res_info);
            out.m_result_params()->set_phone_info(i,PHONE_SCREEN_WORDS,pif.page_size);
            out.m_result_params()->set_phone_info(i,PHONE_PAGE_VERSION,pif.page_type);
            out.m_result_params()->set_phone_info(i,PHONE_PLAT_VERSION,pif.plat_version);
            phone_series_ids.clear();
            phone_series_names.clear();
        }

    }else if( 1 == user_type){
        //user_info 代表的是PID
        phone_info_st pif;
        string phone_series_ids;
        string phone_series_names;
        //获得手机基本信息
        memset(&pif,0x00,sizeof(phone_info_st));
        get_phone_info_by_pid(user_info,&pif);
        //获得手机系列信息 
        get_phone_series_by_pid(user_info,&phone_series_ids,&phone_series_names);
        //设置返回值
        out.m_result_params()->set_phone_info(0,PHONE_ID,user_info.c_str());
        out.m_result_params()->set_phone_info(0,PHONE_BRAND,pif.brand_name);
        out.m_result_params()->set_phone_info(0,PHONE_MODEL,pif.name);
        out.m_result_params()->set_phone_info(0,PHONE_OS_ID,pif.plat_id);
        out.m_result_params()->set_phone_info(0,PHONE_OS_NAME,pif.plat_name);
        out.m_result_params()->set_phone_info(0,PHONE_SERIES_ID,phone_series_ids.c_str());
        out.m_result_params()->set_phone_info(0,PHONE_SERIES_NAME,phone_series_names.c_str());
        out.m_result_params()->set_phone_info(0,PHONE_RESOLU,pif.res_info);
        out.m_result_params()->set_phone_info(0,PHONE_SCREEN_WORDS,pif.page_size);
        out.m_result_params()->set_phone_info(0,PHONE_PAGE_VERSION,pif.page_type);
        out.m_result_params()->set_phone_info(0,PHONE_PLAT_VERSION,pif.plat_version);
    }else{
        //未指定
    }
    UB_LOG_NOTICE("+++++get_phone_info end+++++");
    return MCP_OK;
}

/**
  *@brief 根据厂商ID和首字母获得手机个数(CACHE)
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *
  *@return int      该字母机型个数，小于0表示查询出错
  */
int get_phone_cnt_by_brand_id_and_head_char_fromCache(string brand_id,string head_char)
{ 
    int ret = 0;
    int count_num = 0;

    string valueout =  "";
    string mainkey = "brand_phone_count";
    string subkey;
    if( "" == head_char || "*" == head_char )
        subkey = brand_id;
    else
        subkey = brand_id + "_" + head_char + "_count";

    mcp_session conn;
    ret = conn.session_search( mainkey, subkey, valueout);
    if ( valueout.length() == 0 )
    {
        UB_LOG_NOTICE("%s : brand_id = %s head_char %s [not exist] ()", __func__,brand_id.c_str(),head_char.c_str());
        return -1;
    }else if ( 0 == ret ){
        count_num = atoi( valueout.c_str() );
        UB_LOG_NOTICE("%s : brand_id = %s head_char %s count_num is %d ", __func__,brand_id.c_str(),head_char.c_str(),count_num);
        return count_num;
    }else{
        UB_LOG_NOTICE("%s : brand_id = %s head_char %s [search failed]", __func__,brand_id.c_str(),head_char.c_str());
        return -1;
    }
}
/**
  *@brief 插入厂商ID和首字母获得手机个数(CACHE)
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *
  *@return int  0 成功 -1 失败
  */
int insert_phone_cnt_by_brand_id_and_head_char_toCache(string brand_id,string head_char,int cnt)
{
    int ret = 0;
    string mainkey = "brand_phone_count";
    string subkey;
    if( "" == head_char || "*" == head_char )
        subkey = brand_id;
    else
        subkey = brand_id + "_" + head_char + "_count";

    char num[16];
    sprintf(num , "%d", cnt);
    string value = num;
    mcp_session conn;
    ret = conn.session_insert(mainkey, subkey, value, 60000);
    if (0 != ret){
        UB_LOG_FATAL("%s insert fail!",__func__);
        return -1;
    }
    UB_LOG_NOTICE("%s insert success",__func__);
    return 0;

}


/**
  *@brief 根据厂商ID和首字母获得手机个数(DB)
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *
  *@return int      该字母机型个数，小于0表示查询出错
  */
int get_phone_cnt_by_brand_id_and_head_char_fromDB(string brand_id,string head_char){

    MysqlConnection *database_connection;
    int err = 0;
    int ret = 0;
    
    string sql_count = "SELECT count(DISTINCT  `mcp_phone_info`.`name`) FROM `mcp_phone_info` WHERE `mcp_phone_info`.`brand_id`  =  " + brand_id + " and `mcp_phone_info`.`status` = 1";
    if( "*" == head_char || "" == head_char ){
    }else{
        sql_count = sql_count + " and `mcp_phone_info`.`name` like '" + head_char + "%' ";
    }
    UB_LOG_NOTICE( "sql[%s], [%s:%d]", sql_count.c_str(), __FILE__, __LINE__ );
    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        return -1;
    }
    UB_LOG_NOTICE("%s %s",__func__,sql_count.c_str());
    ret = database_connection->selectCount(sql_count.c_str());
    database_pool->putBackConnection(database_connection, false);
    return ret;

}

/**
  *@brief 根据厂商ID和首字母获得手机个数
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *
  *@return int      该字母机型个数，小于0表示查询出错
  */
int get_phone_cnt_by_brand_id_and_head_char(string brand_id,string head_char){
    
    UB_LOG_NOTICE("%s : brand_id = %s head_char %s START)", __func__,brand_id.c_str(),head_char.c_str());
    int cache_flag = 0;
    int cnt = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    cnt = get_phone_cnt_by_brand_id_and_head_char_fromCache(brand_id,head_char);
    if(cnt < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;  
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        cnt = get_phone_cnt_by_brand_id_and_head_char_fromDB(brand_id,head_char);
        if( cnt < 0 ){
            //DB query error
            UB_LOG_NOTICE("%s : brand_id = %s head_char %s [not exist] (DB)", __func__,brand_id.c_str(),head_char.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("%s : brand_id = %s head_char %s cnt = %d [OK] (DB)", __func__,brand_id.c_str(),head_char.c_str(),cnt);
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_cnt_by_brand_id_and_head_char_toCache(brand_id,head_char,cnt);
#endif
        }
    }
    UB_LOG_NOTICE("%s : brand_id = %s head_char %s END)", __func__,brand_id.c_str(),head_char.c_str());
    return cnt;
}
/*
 *@brief    根据厂商ID,首字母，每页个数，页数返回手机
 *
 *@param    [in]     brand_id            厂商ID
 *@param    [in]     head_word           首字母
 *@param    [in]     unit_num            每页个数
 *@param    [in]     unit_times          页数
 *@param    [out]    phones_cnt          手机总个数
 *@param    [out]    phones_id           本页的所有手机ID
 *@param    [out]    phones_name         本页的所有手机名称
 *
 *@return   int    手机个数 < 0 错误
 */
int get_phones_cnt_by_brand_id(string brand_id,string head_word){
    UB_LOG_NOTICE("%s START", __func__);
    //所有首字母机型总数之和
    int total_cnt = 0;
    //每个字母的机型总数临时计数
    int cnt = 0;
    
    if( head_word == "" || head_word == "*" ){
        cnt = get_phone_cnt_by_brand_id_and_head_char(brand_id,head_word);
        if(cnt > 0){
            total_cnt = cnt;
        }
    }else{
        //首字母字符串长度
        int len = head_word.length();
        char *p;
        //the length add 1 to contain the char '\0'
        char *buff = (char *)malloc(sizeof(char) * len+1);
        char *buff_tmp ;

        if(NULL == buff){
            return MCP_NG;
        }

        memcpy(buff,head_word.c_str(),len+1);
        // 用 / 分开 取得每个首字母
        p = strtok_r(buff, "/",&buff_tmp);
        while(p){
            //p points each head_char seperates by /
			UB_LOG_TRACE("p is %s", p);
            string tmp_head_char = "";
            tmp_head_char = tmp_head_char + p;
            cnt =get_phone_cnt_by_brand_id_and_head_char(brand_id,tmp_head_char);
            if(cnt > 0){
                total_cnt = total_cnt + cnt;
            }
            p = strtok_r(NULL, "/",&buff_tmp);
        }
        free(buff);
    }
    UB_LOG_NOTICE("%s END", __func__);
    return total_cnt;
}

/**
  *@brief 根据厂商ID和首字母获得手机列表(DB)
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *@param [in]  cnt             该首字母、开头字母对应的手机个数,  >= 0,用cnt的值,<0 则实时查询
  *
  *@return int      该字母机型个数，小于0表示查询出错
  */
int get_phones_by_brand_id_and_head_char_fromDB(string brand_id,string head_char,int cnt, phone_name_st* result){

    if(cnt==0){
        UB_LOG_NOTICE( "!!!!!!!cnt[0]!!!!!!!!!, do not need to query" );
        return MCP_OK;
    }
    int err = 0;
    int ret = 0;
    int j = 0;
    MysqlConnection *database_connection;
    MyclientRes data_res;

    string sql = "SELECT `mcp_phone_info`.`id`, `mcp_phone_info`.`name`,`mcp_phone_brand`.`name` FROM `mcp_phone_info` LEFT JOIN `mcp_phone_brand` ON `mcp_phone_info`.`brand_id` = `mcp_phone_brand`.`id` WHERE `mcp_phone_info`.`brand_id` = " + brand_id + 
                   " and `mcp_phone_info`.`status` = 1";

    if( "" == head_char || "*" == head_char ){
    }else{
        sql = sql +  " AND `mcp_phone_info`.`name` like '"+head_char+"%'";
    }
    sql = sql + " ORDER BY  `mcp_phone_info`.`name` ASC ";
    UB_LOG_NOTICE( "sql[%s], [%s:%d]", sql.c_str(), __FILE__, __LINE__ );
    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        data_res.free();
        return -1;
    }

    UB_LOG_NOTICE("%s %s",__func__,sql.c_str());
    ret = database_connection->query(sql.c_str(), &data_res, true);
    
    if ( ret < 0 ){
        UB_LOG_FATAL("%s query data from database failed",__func__);
        ret = database_pool->putBackConnection(database_connection, false);
        if (ret != 0){
            ul_writelog(UL_LOG_FATAL, "putback connection is failed");
        }
        data_res.free();
        return -1;
    }
    //分配数组的大小 大于查出来的大小
/*
    if( cnt != ret ){
        UB_LOG_FATAL("%s cnt not equal ret",__func__);
        if(cnt > ret){
            cnt = ret;
        }
    }
*/
    //遍历结果
    for ( int i = 0; i < ret ; i++){
        if( i == cnt )
        {
//            UB_LOG_NOTICE("%s cnt not equal ret, i is %d, cnt is %d",__func__ ,i, cnt);
            UB_LOG_NOTICE("%s cnt not equal ret, i is %d, cnt is %d, j is %d ,phone_name is %s ",
                                                                  __func__ ,i, cnt, j, data_res[i][1]);
        }
        if( NULL != data_res[i][1])
        {
            if( j > 0 ){
//            UB_LOG_NOTICE( "strlen(data_res[%d][1]):[%d]", i, strlen(data_res[i][1]) );
                if( 0 == strcmp(result[j-1].name, data_res[i][1]))
                {
                    UB_LOG_NOTICE("phone name %s is same,i is %d, j is %d",result[j-1].name, i , j);
                    continue;
                }
            } 
            strcpy(result[j].name, data_res[i][1]);
        }else{
            strcpy(result[j].name, "");
        }
        if( NULL != data_res[i][0]){
            strcpy(result[j].id, data_res[i][0]);
        }else{
            strcpy(result[j].id, "");
        }
        if( NULL != data_res[i][2]){
            strcpy(result[j].brand_name, data_res[i][2]);
        }else{
            strcpy(result[j].brand_name, "");
        }
        UB_LOG_NOTICE("i is %d, j is %d", i , j);
        j++;

    }
    //释放资源
    database_pool->putBackConnection(database_connection, false);
    UB_LOG_NOTICE("loop is ok");
    data_res.free();
    UB_LOG_NOTICE("data_res free is ok");
    return MCP_OK;
}

/**
  *@brief 根据厂商ID和首字母获得手机列表(CACHE)
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *@param [in]  cnt             该首字母、开头字母对应的手机个数,  >= 0,用cnt的值,<0 则实时查询
  *
  *@return int                小于0表示出错，0表示成功
  */
int get_phones_by_brand_id_and_head_char_fromCache(string brand_id,string head_char,int cnt, phone_name_st* result)
{
    int ret = 0;

    string subkey =  "";
    string mainkey = "brand_phone_info";
    string valueout;
    if( "" == head_char || "*" == head_char )
        subkey = brand_id;
    else
        subkey = brand_id + "_" + head_char;

    mcp_session conn;
    ret = conn.session_search( mainkey, subkey, valueout);
    if ( valueout.length() == 0 )
    {
        UB_LOG_NOTICE("%s : brand_id = %s head_char %s [not exist] (SESSION)", __func__,brand_id.c_str(),head_char.c_str());
        return -1;
    }
    else if ( 0 == ret )
    {
        UB_LOG_NOTICE("%s : brand_id = %s head_char %s (SESSION)", __func__,brand_id.c_str(),head_char.c_str());
    }
    else
    {
        UB_LOG_NOTICE("%s : brand_id = %s head_char %s [search failed] (SESSION)", __func__,brand_id.c_str(),head_char.c_str());
        return -1;
    }

    Json::Reader json_reader;
    Json::Value json_value;
    string tmpmsg;
    size_t pos= 0;
    int k = 0;
    while( (pos = valueout.find_first_of( '{', pos )) < valueout.size() && k < cnt )
    {
       tmpmsg.assign( valueout, pos, valueout.find_first_of( '}', pos ) - pos +1 );
       ++pos;
       if(json_reader.parse(tmpmsg,json_value))
       {
	  strlcpy( result[k].id, json_value["id"].asString().c_str(), sizeof(result[k].id) );
	  strlcpy( result[k].name, json_value["name"].asString().c_str(), sizeof(result[k].name) );
	  strlcpy( result[k].brand_name, json_value["brand_name"].asString().c_str(), sizeof(result[k].brand_name) );
       }
       k++;
    }
    return 0;    
}
/**
  *@brief 根据厂商ID和首字母获得手机列表(CACHE)
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *@param [in]  cnt             该首字母、开头字母对应的手机个数,  >= 0,用cnt的值,<0 则实时查询
  *
  *@return int                  小于0表示出错，0表示成功
  */
int insert_phones_by_brand_id_and_head_char_toCache(string brand_id,string head_char,int cnt, phone_name_st* result)
{
    int ret = 0;

    string value;
    string subkey =  "";
    string mainkey = "brand_phone_info";
    
    if( "" == head_char || "*" == head_char )
        subkey = brand_id;
    else
        subkey = brand_id + "_" + head_char;
    
    mcp_session conn;
    
    ret = conn.session_delete( mainkey, subkey);
    if (0 != ret)
    {
        UB_LOG_FATAL("%s delete fail!",__func__);
        return -1;
    }

    string id;
    string name;
    string brand_name;
    for(int i=0; i<cnt; i++)
    {
       id.clear(); id = result[i].id;
       name.clear(); name = result[i].name;
       brand_name.clear(); brand_name = result[i].brand_name;
       value += "{\"id\":\"" + id + "\",\"name\":\"" + name +"\",\"brand_name\":\"" + brand_name +"\"}";
    }

    ret = conn.session_insert(mainkey, subkey, value, 60000);
    if (0 != ret)
    {
        UB_LOG_FATAL("%s insert fail!",__func__);
        return -1;
    }
    UB_LOG_NOTICE("%s insert success",__func__);
    return 0;
}
/**
  *@brief 根据厂商ID和首字母获得手机个数
  *
  *@param [in]  brand_id        厂商ID
  *@param [in]  head_char       开头字母
  *
  *@return int                  小于0表示查询出错
  */
int get_phones_by_brand_id_and_head_char(string brand_id,string head_char,int cnt, phone_name_st* result){
    int cache_flag = 0;
    int ret = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    ret = get_phones_by_brand_id_and_head_char_fromCache(brand_id,head_char,cnt,result);
    if(ret < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        ret = get_phones_by_brand_id_and_head_char_fromDB(brand_id,head_char,cnt,result);
        if( ret < 0 ){
            //DB query error
            UB_LOG_NOTICE("%s : brand_id = %s head_char %s [not exist] (DB)", __func__,brand_id.c_str(),head_char.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("%s : brand_id = %s head_char %s cnt = %d [OK] (DB)", __func__,brand_id.c_str(),head_char.c_str(),cnt);
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phones_by_brand_id_and_head_char_toCache(brand_id,head_char,cnt,result);
#endif
        }
    }
    return ret;
}
 /*
 *@brief    根据厂商ID,首字母，每页个数，页数返回手机
 *
 *@param    [in]     brand_id            厂商ID
 *@param    [in]     head_word           首字母
 *@param    [in]     unit_num            每页个数
 *@param    [in]     unit_times          页数
 *@param    [out]    phones_cnt          手机总个数
 *@param    [out]    phones_id           本页的所有手机ID
 *@param    [out]    phones_name         本页的所有手机名称
 */
int get_phones_by_brand_id(string brand_id,string head_word,int unit_num,int unit_times,int* phones_cnt,char** phones_id,char** phones_name){
    int ret = 0;
    int total_cnt = 0;
    int index = 0;
    int i = 0;
    int index_cnt = 0;
    int cnt = 0;
    //参数合理性检验
    if( unit_times < 1 ){
        unit_times = 1;
    }
    if( unit_num <= 0 ){
        unit_num = 10;
    }

    //获得满足条件的所有机型数量之和
    total_cnt = get_phones_cnt_by_brand_id(brand_id,head_word);
    UB_LOG_NOTICE("[GET PHONES CNT OK ] %s %s %s", __func__,brand_id.c_str(),head_word.c_str());
    if( 0 >= total_cnt ){
        *phones_cnt = 0;
        return 0;
    }
    *phones_cnt = total_cnt;
    //最后页 页码的最大值
    int last_valide_page = 0;
    if( 0 == (total_cnt % unit_num)){
        last_valide_page = total_cnt / unit_num;
    }
    else{
        last_valide_page = total_cnt / unit_num + 1;
    }
    if(unit_times > last_valide_page){
        unit_times = last_valide_page;
    }
    //计算出理论上起始 与 结束 index 从0开始
    int start_index = (unit_times - 1) * unit_num;
    //end_index = last_show_index + 1
    int end_index = unit_num * unit_times;
    if(end_index > total_cnt){
        end_index = total_cnt;
    }

    //分配空间
    phone_name_st *result = (phone_name_st *) malloc(sizeof(phone_name_st) * (total_cnt+1) );
    if( NULL == result ){
        return -1;
    }
    //初始化内存
    memset(result,0,sizeof(phone_name_st) * total_cnt);
    
    if( head_word == "" || head_word == "*" ){
        ret = get_phones_by_brand_id_and_head_char(brand_id,head_word,total_cnt,result);
    }else{
        //首字母字符串长度
        int len = head_word.length();
        char *p;
        //the length add 1 to contain the char '\0'
        char *buff = (char *)malloc(sizeof(char) * len+1);
        char *buff_tmp;
        if(NULL == buff){
            free(result);
            return MCP_NG;
        }
        memcpy(buff,head_word.c_str(),len+1);
        // 用 / 分开 取得每个首字母
        p = strtok_r(buff, "/",&buff_tmp);
        while(p){
            //p points each head_char seperates by /
            string tmp_head_char = "";
            tmp_head_char = tmp_head_char + p;
            cnt =get_phone_cnt_by_brand_id_and_head_char(brand_id,tmp_head_char);
            if(cnt<0){
                UB_LOG_FATAL("get_phone_cnt_by_brand_id_and_head_char failed, [%s:%d]", __FILE__, __LINE__ );
                return -1;
            }
            //分配空间
            phone_name_st *tmp_result = (phone_name_st *) malloc(sizeof(phone_name_st) * (cnt+1));
            if( NULL == tmp_result ){
                free(result);
                free(buff);
                return -1;
             }
            //初始化内存
            memset(tmp_result,0,sizeof(phone_name_st) * cnt);
            //查询每个字母的手机信息
            get_phones_by_brand_id_and_head_char(brand_id,tmp_head_char,cnt,tmp_result);
            //追加到后面 
            memcpy(&(result[index_cnt]),tmp_result,sizeof(phone_name_st) * cnt);
            index_cnt = index_cnt + cnt;
            //释放档袅偈蹦诖�
            free(tmp_result);
            //move to next 
            p = strtok_r(NULL, "/",(char**)&buff_tmp);
        }
        free(buff);
    }
    //返回每一页
    index = start_index;
    UB_LOG_NOTICE("%s unit_num %d unit_times %d start_index %d end_index %d", __func__,unit_num,unit_times,start_index,end_index);
    for( i = 0 ; index < end_index && i < unit_num ; index++,i++){
        //合成手机名 厂商-手机型号
        char temp_phone_name[1024] = {0};
        snprintf(temp_phone_name,1024,"%s-%s",result[index].brand_name,result[index].name);
        
        strcpy(phones_id[i],result[index].id);
        strcpy(phones_name[i],temp_phone_name);
    }
    //释放空间
    free(result);
    return 0;
}

/**
  *@brief 根据平台ID获得手机个数(DB)
  *
  *@param [in]  plat_id         厂商ID
  *
  *@return int               该平台机型个数，小于0表示查询出错
  */
int get_phone_cnt_by_plat_id_fromDB(string plat_id){

    MysqlConnection *database_connection;
    string sql_count = "SELECT count(distinct mcp_phone_info.name , mcp_phone_info.brand_id) FROM `mcp_phone_info` WHERE `mcp_phone_info`.`plat_id`  =  " + plat_id + " and `mcp_phone_info`.`status` = 1";

    int err = 0;
    int ret = 0;
    
    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        return -1;
    }
    UB_LOG_NOTICE("%s %s",__func__,sql_count.c_str());
    ret = database_connection->selectCount(sql_count.c_str());
    database_pool->putBackConnection(database_connection, false);
    return ret;
}

/**
  *@brief 根据平台ID获得手机个数(CACHE)
  *
  *@param [in]  plat_id         厂商ID
  *
  *@return int               该平台机型个数，小于0表示查询出错
  */
int get_phone_cnt_by_plat_id_fromCache(string plat_id)
{
    int ret = 0;
    int count_num = 0;
    string valueout;
    string mainkey = "plat_phone_count";
    string subkey = plat_id;

    mcp_session conn;
    ret = conn.session_search( mainkey, subkey, valueout);
    if ( valueout.length() <= 0 )
    {
        UB_LOG_NOTICE("%s : plat_id = %s  [not exist] (SESSION)", __func__,plat_id.c_str());
        return -1;
    }
    else if ( 0 == ret )
    {
        count_num = atoi( valueout.c_str() );
        UB_LOG_NOTICE("%s : plat_id = %s count_num is %d (SESSION)", __func__,plat_id.c_str(),count_num);
        return count_num;
    }
    else
    {
        UB_LOG_NOTICE("%s : plat_id = %s [search failed] (SESSION)", __func__,plat_id.c_str());
        return -1;
    }
}
/**
  *@brief 插入平台ID手机个数(CACHE)
  *
  *@param [in]  plat_id         厂商ID
  *
  *@return int  0 成功 -1 失败
  */
int insert_phone_cnt_by_plat_id_toCache(string plat_id,int cnt)
{
    int ret = 0;
    char num[16];
    sprintf(num, "%d", cnt);
    string value = num;
    string mainkey = "plat_phone_count";
    string subkey = plat_id;

    mcp_session redisconn;
    ret = redisconn.session_delete( mainkey, subkey);
    if (0 != ret)
    {
        UB_LOG_FATAL("%s delete fail!",__func__);
        return -1;
    }

    ret = redisconn.session_insert(mainkey, subkey, value, 60000);
    if (0 != ret)
    {
        UB_LOG_FATAL("%s insert fail!",__func__);
        return -1;
    }
    UB_LOG_NOTICE("%s insert success",__func__);
    return 0;
}

/**
  *@brief 根据平台ID获得手机个数
  *
  *@param [in]  plat_id         厂商ID
  *
  *@return int      该字母机型个数，小于0表示查询出错
  */
int get_phone_cnt_by_plat_id(string plat_id){
    int cache_flag = 0;
    int cnt = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    cnt = get_phone_cnt_by_plat_id_fromCache(plat_id);
    if(cnt < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;  
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        cnt = get_phone_cnt_by_plat_id_fromDB(plat_id);
        if( cnt < 0 ){
            //DB query error
            UB_LOG_NOTICE("%s : brand_id = %s [not exist] (DB)", __func__,plat_id.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("%s : brand_id = %s cnt = %d [OK] (DB)", __func__,plat_id.c_str(),cnt);
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_cnt_by_plat_id_toCache(plat_id,cnt);
#endif
        }
    }
    return cnt;
}

/**
  *@brief 根据平台ID获得手机列表(DB)
  *
  *@param [in]  plat_id         厂商ID
  *@param [in]  cnt             手机个数 
  *
  *@return int                  机型个数，小于0表示查询出错
  */
int get_phones_by_plat_id_fromDB(string plat_id,int cnt, phone_name_st* result){

    UB_LOG_NOTICE("[Start Search From DB] %s : plat_id = %s ", __func__,plat_id.c_str());
    int err = 0;
    int ret = 0;
    MysqlConnection *database_connection;
    MyclientRes data_res;

    string sql = "SELECT distinct `mcp_phone_info`.`id`,`mcp_phone_info`.`name`,`mcp_phone_brand`.`name` FROM `mcp_phone_info` LEFT JOIN `mcp_phone_brand` ON `mcp_phone_info`.`brand_id` = `mcp_phone_brand`.`id` WHERE `mcp_phone_info`.`plat_id` = " + plat_id + 
                " and `mcp_phone_info`.`status` = 1" ;

    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        data_res.free();
        return -1;
    }

    UB_LOG_NOTICE("%s %s",__func__,sql.c_str());
    ret = database_connection->query(sql.c_str(), &data_res, true);
    
    if ( ret < 0 ){
        UB_LOG_FATAL("%s query data from database failed",__func__);
        ret = database_pool->putBackConnection(database_connection, false);
        if (ret != 0){
            ul_writelog(UL_LOG_FATAL, "putback connection is failed");
        }
        data_res.free();
        return -1;
    }
    //分配数组的大小 大于查出来的大小
    if( cnt != ret ){
        UB_LOG_FATAL("%s cnt not equal ret",__func__);
        if(cnt > ret){
            cnt = ret;
        }
    }
    //遍历结果
    UB_LOG_NOTICE("[Save Reuslt From DB INTO result] %s : plat_id = %s ", __func__,plat_id.c_str());
    for ( int i = 0; i < cnt ; i++){
        
        UB_LOG_NOTICE("[Saving Reuslt From DB INTO result] index = %d function %s id = %s name = %s brand_name = %s  ",i,__func__,data_res[i][0],data_res[i][1],data_res[i][2]);
        if( NULL != data_res[i][0] ){
            strcpy(result[i].id, data_res[i][0]);
        }else{
            strcpy(result[i].id,"");
        }
        if( NULL != data_res[i][1] ){
            strcpy(result[i].name, data_res[i][1]);
        }else{
            strcpy(result[i].name, "");
        }
        if( NULL != data_res[i][2] ){
            strcpy(result[i].brand_name, data_res[i][2]);
        }else{
            strcpy(result[i].brand_name, "");
        }
    }
    UB_LOG_NOTICE("[Save Reuslt From DB INTO result (OK)] %s : plat_id = %s ", __func__,plat_id.c_str());
    //释放资源
    database_pool->putBackConnection(database_connection, false);
    data_res.free();
    return cnt;
}
/**
  *@brief 根据平台ID获得手机列表(CACHE)
  *
  *@param [in]  plat_id         厂商ID
  *@param [in]  cnt             手机个数 
  *@param [out] result          结果数组 
  *
  *@return int                  0成功 小于0表示查询出错
  */
int get_phones_by_plat_id_fromCache(string plat_id,int cnt, phone_name_st* result)
{
    int ret = 0;
    string mainkey = "plat_phone_info";
    string subkey = plat_id;
    string valueout;
    
    mcp_session redisconn;
    ret = redisconn.session_search( mainkey, subkey, valueout);
    if ( valueout.length() <= 0 )
    {
        UB_LOG_NOTICE("%s : plat_id = %s [not exist] (SESSION)", __func__,plat_id.c_str());
        return -1;
    }
    else if ( 0 == ret )
    {
        UB_LOG_NOTICE("%s : brand_id = %s (SESSION)", __func__,plat_id.c_str());
    }
    else
    {
        UB_LOG_NOTICE("%s : brand_id = %s [search failed] (SESSION)", __func__,plat_id.c_str());
        return -1;
    }

    Json::Reader json_reader;
    Json::Value json_value;
    string tmpmsg;
    size_t pos= 0; 
    int k = 0;
    while( (pos = valueout.find_first_of( '{', pos )) < valueout.size() && k < cnt )
    {   
       tmpmsg.assign( valueout, pos, valueout.find_first_of( '}', pos ) - pos +1 );
       ++pos;
       if(json_reader.parse(tmpmsg,json_value))
       {
	  strlcpy( result[k].id, json_value["id"].asString().c_str(), sizeof(result[k].id) );
	  strlcpy( result[k].name, json_value["name"].asString().c_str(), sizeof(result[k].name) );
	  strlcpy( result[k].brand_name, json_value["brand_name"].asString().c_str(), sizeof(result[k].brand_name) );
       }
       k++;
    }
    return 0;
}
/**
  *@brief 根据平台ID插入手机列表(CACHE)
  *
  *@param [in]  plat_id         厂商ID
  *@param [in]  cnt             手机个数 
  *@param [out] result          待插入结果数组 
  *
  *@return int                  小于0表示出错，0表示成功
  */
int insert_phones_by_plat_id_toCache(string plat_id,int cnt, phone_name_st* result)
{
    int ret = 0;
    string value;
    string subkey =  plat_id;
    string mainkey = "plat_phone_info";
    
    mcp_session redisconn;
    ret = redisconn.session_delete( mainkey, subkey);
    if (0 != ret)
    {
        UB_LOG_FATAL("%s delete fail!",__func__);
        return -1;
    }

    string id;
    string name;
    string brand_name;
    for(int i=0; i<cnt; i++)
    {
       id.clear(); id = result[i].id;
       name.clear(); name = result[i].name;
       brand_name.clear(); brand_name = result[i].brand_name;
       
       value += "{\"id\":\"" + id + "\",\"name\":\"" + name +"\",\"brand_name\":\"" + brand_name +"\"}";
    }
    
    ret = redisconn.session_insert(mainkey, subkey, value, 60000);
    if (0 != ret)
    {
        UB_LOG_FATAL("%s insert fail!",__func__);
        return -1;
    }
    UB_LOG_NOTICE("%s insert success",__func__);
    return 0;
}

/**
  *@brief 根据平台ID获得手机列表
  *
  *@param [in]  plat_id         厂商ID
  *@param [in]  cnt             手机个数 
  *@param [out] result          结果数组 
  *
  *@return int                  小于0表示查询出错
  */
int get_phones_by_plat_id(string plat_id,int cnt, phone_name_st* result){
    int cache_flag = 0;
    int ret = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    ret = get_phones_by_plat_id_fromCache(plat_id,cnt,result);
    if(ret < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        ret = get_phones_by_plat_id_fromDB(plat_id,cnt,result);
        if( ret < 0 ){
            //DB query error
            UB_LOG_NOTICE("%s : plat_id = %s  [not exist] (DB)", __func__,plat_id.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("%s : plat_id = %s  cnt = %d [OK] (DB)", __func__,plat_id.c_str(),cnt);
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phones_by_plat_id_toCache(plat_id,cnt,result);

#endif
        }
    }
    return ret;
}
 /*
 *@brief    根据平台ID，每页个数，页数返回手机
 *
 *@param    [in]     brand_id            平台ID
 *@param    [in]     unit_num            每页个数
 *@param    [in]     unit_times          页数
 *@param    [out]    phones_cnt          手机总个数
 *@param    [out]    phones_id           本页的所有手机ID
 *@param    [out]    phones_name         本页的所有手机名称
 */
int get_phones_by_plat_id_per_page(string plat_id,int unit_num,int unit_times,int* phones_cnt,char** phones_id,char** phones_name){
    int ret = 0;
    int total_cnt = 0;
    int index = 0;
    int i = 0;
    //参数合理性检验
    if( unit_times < 1 ){
        unit_times = 1;
    }
    if( unit_num <= 0 ){
        unit_num = 10;
    }
    //获得满足条件的所有机型数量之和
    total_cnt = get_phone_cnt_by_plat_id(plat_id);
    if( 0 >= total_cnt ){
        *phones_cnt = 0;
        return 0;
    }
    *phones_cnt = total_cnt;
    //最后页 页码的最大值
    int last_valide_page = 0;
    if( 0 == (total_cnt % unit_num)){
        last_valide_page = total_cnt / unit_num;
    }
    else{
        last_valide_page = total_cnt / unit_num + 1;
    }
    if(unit_times > last_valide_page){
        unit_times = last_valide_page;
    }
    //计算出理论上起始 与 结束 index 从0开始
    int start_index = (unit_times - 1) * unit_num;
    //end_index = last_show_index + 1
    int end_index = unit_num * unit_times;
    if(end_index > total_cnt){
        end_index = total_cnt;
    }
    
    //分配空间
    phone_name_st *result = (phone_name_st *) malloc(sizeof(phone_name_st) * total_cnt);
    if( NULL == result ){
        return -1;
    }
    //初始化内存
    memset(result,0,sizeof(phone_name_st) * total_cnt);
    
    *phones_cnt = total_cnt;
    ret = get_phones_by_plat_id(plat_id,total_cnt,result);
    UB_LOG_NOTICE("[GET PHONES BY PLAT_ID OK %s]", __func__);
    UB_LOG_NOTICE("%s unit_num %d unit_times %d start_index %d end_index %d", __func__,unit_num,unit_times,start_index,end_index);
    //返回每一页
    index = start_index;
    for( i = 0 ; index < end_index && i < unit_num ; index++,i++){
        //合成手机名 厂商-手机型号
        UB_LOG_NOTICE("%s %s %s  %s]", __func__,result[index].id,result[index].brand_name,result[index].name);
        char temp_phone_name[1024] = {0};
        snprintf(temp_phone_name,1024,"%s-%s",result[index].brand_name,result[index].name);
        
        strcpy(phones_id[i],result[index].id);
        strcpy(phones_name[i],temp_phone_name);
    }
    //释放空间
    free(result);
    return 0;
}


int McpServlet::get_phone_type_from_ID(const idl::mcp_get_phone_type_from_ID_params& in, idl::mcp_get_phone_type_from_ID_response& out){

    //输入参数
    const string info_id = in.info_id();
    const uint32_t id_type = in.id_type();
    const string head_word = in.head_word();
    const uint32_t unit_num  = in.unit_num();
    const uint32_t unit_times = in.unit_times();
    //输出参数
    int total_cnt = 0;

    int show_cnt = 0;


    //临时变量
    int i = 0;
    int ID_LEN   = 12;
    int FULL_NAME_LEN = 404;

    if( unit_num <= 0 || unit_times <= 0 ){
        return 0;
    }

    char **phones_id = (char**)malloc(sizeof(char *) * unit_num);
    if(NULL == phones_id){
        UB_LOG_NOTICE("[malloc fails] %s", __func__);
        return 0;
    }
    for( i = 0; i < (int)unit_num; i++){
        phones_id[i] = (char*)malloc(sizeof(char) * ID_LEN);
        memset(phones_id[i],0,sizeof(char) * ID_LEN);
        if(NULL == phones_id[i] ){
            UB_LOG_NOTICE("[malloc fails] %s", __func__);
            return 0;
        }
    }  
    char **phones_name = (char**)malloc(sizeof(char *) * unit_num);
    if(NULL == phones_name){
        UB_LOG_NOTICE("[malloc fails] %s", __func__);
        return 0;
    }
    for( i = 0; i < (int)unit_num; i++){
        phones_name[i] = (char*)malloc(sizeof(char) * FULL_NAME_LEN);
        memset(phones_name[i],0,sizeof(char) * FULL_NAME_LEN);
        if(NULL == phones_name[i] ){
            UB_LOG_NOTICE("[malloc fails] %s", __func__);
            return 0;
        }
    }

    if( 0 == id_type ){
        //info_id 是 厂商 ID
        get_phones_by_brand_id(info_id,head_word,unit_num,unit_times,&total_cnt,phones_id,phones_name);
        //返回结果
        out.m_result_params()->set_phone_count(total_cnt);
        
        show_cnt = get_cnt_this_page(total_cnt,unit_times,unit_num); 
        
        for( i = 0; i < (int)show_cnt; i++){
            out.m_result_params()->set_phone_id(i,phones_id[i]);
            out.m_result_params()->set_phone_name(i,phones_name[i]);
        }

    }else if( 1 == id_type ){
        //info_id 是 平台 ID
        UB_LOG_NOTICE("[FLAG 1 OK] %s", __func__);
        get_phones_by_plat_id_per_page(info_id,unit_num,unit_times,&total_cnt,phones_id,phones_name);
        //返回结果
        out.m_result_params()->set_phone_count(total_cnt);

        show_cnt = get_cnt_this_page(total_cnt,unit_times,unit_num);

        for( i = 0; i < (int)show_cnt; i++){
            out.m_result_params()->set_phone_id(i,phones_id[i]);
            out.m_result_params()->set_phone_name(i,phones_name[i]);
        }
        UB_LOG_NOTICE("[FLAG 2 OK %s", __func__);
    }else{
        //未指定info_id类型 默认用 厂商ID
        //info_id 是 厂商 ID
        get_phones_by_brand_id(info_id,head_word,unit_num,unit_times,&total_cnt,phones_id,phones_name);
        //返回结果
        out.m_result_params()->set_phone_count(total_cnt);

        show_cnt = get_cnt_this_page(total_cnt,unit_times,unit_num);

        for( i = 0; i < (int)show_cnt; i++){
            out.m_result_params()->set_phone_id(i,phones_id[i]);
            out.m_result_params()->set_phone_name(i,phones_name[i]);
        }
    }

    //释放资源
    for( i = 0; i < (int)unit_num; i++){
       free(phones_id[i]);
    }
    free(phones_id); 
    for( i = 0; i < (int)unit_num; i++){
       free(phones_name[i]);
    }
    free(phones_name);
    
    return MCP_OK;
}


