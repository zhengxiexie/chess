/*
 * =====================================================================================
 *
 *       Filename:  mcpsvr_phone_headword.cpp
 *
 *    Description:  实现接口get_phone_headword_from_maker
 *
 *        Version:  1.0
 *        Created:  08/11/2011 06:38:42 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  RD. Hongming li (lhm), lihongming@baidu.com
 *        Company:  baidu, Beijing, China
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"
#include "tools.h"

#define PHONE_HEAD_WORD_ST_TIMEOUT 100
#define PHONE_HEAD_WORD_ST_EXPIRED 60*60*24

typedef struct {
    char head_word[256];
    int total;
}phone_head_word_st;

//根据phone_head_word_st中的total字段排序
int compar(const void *arg1, const void *arg2){
	phone_head_word_st *a = (phone_head_word_st *)arg1;
	phone_head_word_st *b = (phone_head_word_st *)arg2;
	
	return a->total - b->total;
}

/**
  *@brief 根据厂商ID获得该手机厂商所有不同开头字母总数(CACHE)
  *
  *@param [in]  brand_id          指定厂商ID
  *
  * @return    int                该手机厂商所有不同开头字母总数,小于0失败
  */
int get_phone_headword_cnt_by_brand_id_fromCache(string brand_id)
{
    int ret = 0;
    string  valueout("0");
    //主KEY
    string mainkey = "brand_head_word_info";
    string subkey = brand_id+"_count";
    
    mcp_session conn;
    ret = conn.session_search( mainkey, subkey, valueout );
    if ( valueout.length() <= 0 )
    {
        UB_LOG_NOTICE("[NOT IN REDIS] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str() );
        return -1;
    }
    else if ( 0 == ret )
    {
        UB_LOG_NOTICE("[FOUND IN REDIS] %s MAINKEY %s SUBKEY %s ",__func__,mainkey.c_str(), subkey.c_str() );
        return atoi( valueout.c_str() );
    }
    else
    {
        UB_LOG_NOTICE("[ERROR IN REDIS] %s MAINKEY %s SUBKEY %s ",__func__,mainkey.c_str(), subkey.c_str() );
        return -1;
    }
}
/**
  *@brief 插入厂商ID厂商所有不同开头字母总数(CACHE)
  *
  *@param [in]  brand_id          指定厂商ID
  *@param [in]  cnt               该手机厂商所有不同开头字母总数
  *
  *@return int  0 成功 -1 失败
  */
int insert_phone_headword_cnt_by_brand_id_toCache(string brand_id,int cnt)
{
    int ret = 0;
    if( cnt < 0 ){
        return -1;
    }
    //主KEY
    string mainkey = "brand_head_word_info";
    //子KEY
    string subkey = brand_id+"_count";
    //设置要插入的值
    char num[16];
    sprintf(num, "%d", cnt);
    string value = num;

    mcp_session conn;
    //先删除
    ret = conn.session_delete( mainkey, subkey);
    if (0 != ret)
    {
        UB_LOG_NOTICE("[DELETE SESSION ERROR] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str() );
        return -1;
    }
    //插入
    ret = conn.session_insert(mainkey, subkey, value, PHONE_HEAD_WORD_ST_EXPIRED );
    if (0 != ret)
    {
        UB_LOG_NOTICE("[INSERT REDIS ERROR] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str(), subkey.c_str() );
        return -1;
    }
    UB_LOG_NOTICE("[INSERT REDIS SUCCESS] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str(), subkey.c_str() );
    return 0;
}
/**
  *@brief  根据厂商ID获得该手机厂商所有不同开头字母总数(DB)
  *
  *@param [in]  brand_id          指定厂商ID
  *
  * @return    int                该手机厂商所有不同开头字母总数,小于0失败
  */
int get_phone_headword_cnt_by_brand_id_fromDB(string brand_id){

    MysqlConnection *database_connection;
    string sql_count = "SELECT count(*) FROM `mcp_phone_brand_sw` WHERE `mcp_phone_brand_sw`.`brand_id`  =  " + brand_id + " ";

    int err = 0;
    int ret = 0;
    
    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        UB_LOG_NOTICE("[GET PHONE_HEADWORD_CNT BY BRAND_ID CONNECT DB ERROR] %s",__func__);
        return -1;
    }
    UB_LOG_NOTICE("%s { %s }",__func__,sql_count.c_str());
    ret = database_connection->selectCount(sql_count.c_str());
    database_pool->putBackConnection(database_connection, false);
    return ret;
}
/**
  *@brief  根据厂商ID获得该手机厂商所有不同开头字母总数
  *
  *@param [in]  brand_id          指定厂商ID
  *
  * @return    int                该手机厂商所有不同开头字母总数,小于0失败
  */
int get_phone_headword_cnt_by_brand_id(string brand_id){
    int cache_flag = 0;
    int cnt = 0;
#ifndef REAL_TIME_DATA
    //Search from cache
    cnt = get_phone_headword_cnt_by_brand_id_fromCache(brand_id);
    if(cnt < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;
       UB_LOG_NOTICE("[GET PHONE_HEADWORD_CNT  OK] %s brand_id = %s CNT = %d", __func__,brand_id.c_str(),cnt);
    }
#endif
    if( 0 == cache_flag ){
        //not in cache or doese use cache
        cnt = get_phone_headword_cnt_by_brand_id_fromDB(brand_id);
        if( cnt < 0 ){
            //DB query error
            UB_LOG_NOTICE("[GET PHONE_HEADWORD_CNT  ERROR] %s brand_id = %s ", __func__,brand_id.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("[GET PHONE_HEADWORD_CNT  OK] %s brand_id = %s CNT = %d", __func__,brand_id.c_str(),cnt);
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_headword_cnt_by_brand_id_toCache(brand_id,cnt);
#endif
        }
    }
    return cnt;
}





/**
  *@brief  根据厂商ID获得该手机厂商所有不同开头字母列表(CAHCE)
  *
  *@param [in]  brand_id          指定厂商ID
  *@param [in]  cnt               该手机厂商所有不同开头字母总数
  *@param [out] result            厂商首字母信息结构体
  *
  * @return    int                0 成功, 小于 0 失败
  */
int get_phone_headword_list_by_brand_id_fromCache(string brand_id,int cnt,phone_head_word_st *result)
{
    int ret = 0;
    //cache信息
    if( NULL == result || cnt < 0 ){
        return -1;
    }
    //主KEY
    string mainkey = "brand_head_word_info";
    //子KEY
    string subkey = brand_id;
    //设置返回值大小
    string valueout;

    mcp_session conn;
    ret = conn.session_search( mainkey, subkey, valueout );
    if ( valueout.length() <= 0 )
    {
        UB_LOG_DEBUG("[NOT IN REDIS] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str(), subkey.c_str() );
        return -1;
    }
    else if ( 0 == ret )
    {
        UB_LOG_DEBUG("[FOUND IN REDIS] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str(), subkey.c_str() );
        UB_LOG_DEBUG("valueout[%s]", valueout.c_str());
    }
    else
    {
        UB_LOG_NOTICE("[ERROR IN REDIS] %s MAINKEY %s SUBKEY %s",__func__, mainkey.c_str(), subkey.c_str() );
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
	  strlcpy( result[k].head_word, json_value["head_word"].asString().c_str(), sizeof(result[k].head_word) );
	  result[k].total = atoi( json_value["total"].asString().c_str() );
       }
       k++;
    }
    return cnt;
}

/**
  *@brief 插入厂商所有不同开头字母列表
  *
  *@param [in]  brand_id          指定厂商ID
  *@param [in]  cnt               该手机厂商所有不同开头字母总数
  *@param [in] result             厂商首字母信息结构体
  *
  *@return int  									0 成功 -1 失败
  */
int insert_phone_headword_list_by_brand_id_toCache(string brand_id,int cnt,phone_head_word_st *result)
{
    int ret = 0;
    //cache信息
    if( NULL == result || cnt < 0 )
    {
        return -1;
    }
    //主KEY
    string mainkey = "brand_head_word_info";
    //子KEY
    string subkey = brand_id;
    //设置要插入的值
    string value;
    string head_word;
    string total;
    char num[16];
    for(int i=0; i<cnt; i++)
    {
       memset(num, 0x00, sizeof(num));
       head_word.clear(); head_word = result[i].head_word;
       total.clear();
       sprintf(num, "%d", result[i].total);
       total = num;
       value += "{\"head_word\":\""+ head_word + "\",\"total\":\"" + total +"\"}";
    }
    mcp_session conn;
    //先删除
    ret = conn.session_delete( mainkey, subkey);
    if (0 != ret)
    {
        UB_LOG_NOTICE("[DELETE REDIS ERROR] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str() );
        return -1;
    }
    //插入
    ret = conn.session_insert( mainkey, subkey, value, PHONE_HEAD_WORD_ST_EXPIRED );
    if (0 != ret)
    {
        UB_LOG_NOTICE("[INSERT REDIS ERROR] %s MAINKEY %s SUBKEY %s ",__func__,mainkey.c_str(), subkey.c_str() );
        return -1;
    }
    UB_LOG_NOTICE("[INSERT REDIS SUCCESS] %s MAINKEY %s SUBKEY %s",__func__,mainkey.c_str(), subkey.c_str() );
    return 0;
}

/**
  *根据brand_id获得手机系列信息(DB)
  *
  *@param [in]  brand_id               指定手机的brand_id
  *@param [in]  cnt               该手机对应的系列总数
  *@param [out] result            指定手机的系列信息结构体
  *
  * @return    int         0 成功, 小于 0 失败
  *
  *@return int                  机型个数，小于0表示查询出错
  */
int get_phone_headword_list_by_brand_id_fromDB(string brand_id,int cnt, phone_head_word_st* result){

    UB_LOG_NOTICE("[Start Search Series From DB] %s : brand_id = %s ", __func__,brand_id.c_str());
    int err = 0;
    int ret = 0;
    MysqlConnection *database_connection;
    MyclientRes data_res;
    string sql =  "SELECT  `mcp_phone_brand_sw`.`startwith`,`mcp_phone_brand_sw`.`total`  FROM  `mcp_phone_brand_sw` WHERE  `mcp_phone_brand_sw`.`brand_id` = " + brand_id;

    //得到connection对象
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        UB_LOG_NOTICE("[Search Series From DB Connect ERROR] %s : brand_id = %s ", __func__,brand_id.c_str());
        data_res.free();
        return -1;
    }

    UB_LOG_DEBUG("%s { %s }",__func__,sql.c_str());
    ret = database_connection->query(sql.c_str(), &data_res, true);
    
    if ( ret < 0 ){
        UB_LOG_NOTICE("[Search Series From DB ERROR] %s : brand_id = %s ", __func__,brand_id.c_str());
        ret = database_pool->putBackConnection(database_connection, false);
        if (ret != 0){
            UB_LOG_NOTICE("[Search Series PutBack Connection ERROR] %s : brand_id = %s ", __func__,brand_id.c_str());
        }
        data_res.free();
        return -1;
    }
    
    //保存离散首字母
    phone_head_word_st *tmp = (phone_head_word_st *)malloc(sizeof(phone_head_word_st) * ret);
    phone_head_word_st *tmp_result = (phone_head_word_st *)malloc(sizeof(phone_head_word_st) * ret);
    
    for ( int i = 0; i < ret ; i++){    
//      UB_LOG_NOTICE("[Saving Reuslt From DB INTO result] index = %d function %s id = %s name = %s brand_name = %s  ",i,__func__,data_res[i][0],data_res[i][1],data_res[i][2]);
        copy_result(tmp[i].head_word,data_res[i][0]);
        tmp[i].total = atoi(data_res[i][1]);
    }
    //根据total数量从小到大排序
    qsort(tmp,ret,sizeof(phone_head_word_st),compar);
    //根据规则合并数组
    int group_cnt = 0;
    int combine_total = 5;
    int combine_group_cnt = 0;
    int max_group_items_cnt = 4;
    //新的每组信息
    //新组名
    string group_name("");
   	//新组机型数
    int group_total = 0;
    
    int i = 0;
    //循环每个元素
    for( ; i < ret; i++){
    	//总数小于合并要求
    	if(tmp[i].total <= combine_total){
    		//本组合并数小于最大要求
    		if( combine_group_cnt < max_group_items_cnt){
    			//合并
    			group_name = group_name + tmp[i].head_word + "/";
    			group_total = group_total + tmp[i].total;
    			combine_group_cnt++;
    		}else{
    			strcpy(tmp_result[group_cnt].head_word,group_name.c_str());
    			tmp_result[group_cnt].total = group_total;
    			group_name.clear();
    			group_name = group_name + tmp[i].head_word + "/";
    			group_total = tmp[i].total;
    			combine_group_cnt = 1;
    			group_cnt++;
    		}
    	}else{
    		break;
    	}
    }
    //存在不足max_group_items_cnt的组没保存
    if(combine_group_cnt > 0){
    	strcpy(tmp_result[group_cnt].head_word,group_name.c_str());
    	tmp_result[group_cnt].total = group_total;
    	group_cnt++;
    }
    //将所有大于combine_total的组追加到末尾
    for( ;i < ret; i++){
    	strcpy(tmp_result[group_cnt].head_word,tmp[i].head_word);
    	tmp_result[group_cnt].total = tmp[i].total;
    	group_cnt++;
    }
    
    //查看要返回的结果空间是否足够大
    if( group_cnt > cnt ){
    	group_cnt = cnt;
    }
    //保存结果到result
    memcpy(result,tmp_result,sizeof(phone_head_word_st)* cnt);
    free(tmp);
    free(tmp_result);
    UB_LOG_DEBUG("[Save Reuslt From DB INTO result (OK)] %s : brand_id = %s ", __func__,brand_id.c_str());
    //释放资源
    database_pool->putBackConnection(database_connection, false);
    data_res.free();
    return group_cnt;
}

/**
  *根据brand_id获得手机系列信息(DB)
  *
  *@param [in]  brand_id               指定手机的brand_id
  *@param [in]  cnt               该手机对应的系列总数
  *@param [out] result            指定手机的系列信息结构体
  *
  * @return    int              0 成功, 小于 0 失败
  *
  *@return int                  机型个数，小于0表示查询出错
  */
int get_phone_headword_list_by_brand_id(string brand_id,int cnt,phone_head_word_st* result){
    int cache_flag = 0;

    int ret = 0;
    if( cnt <= 0 ){
        return -1;
    }
    cache_flag = 0;
#ifndef REAL_TIME_DATA
    ret = get_phone_headword_list_by_brand_id_fromCache(brand_id,cnt,result);
    if(ret < 0){
        //not in cache
        cache_flag = 0;
    }else{
       //in cache
       cache_flag = 1;  
    }
#endif
    if( 0 == cache_flag ){
        ret = get_phone_headword_list_by_brand_id_fromDB(brand_id,cnt,result);
        if( ret < 0 ){
            //DB query error
            UB_LOG_NOTICE("[GET PHONE SERIES LIST ERROR] %s brand_id = %s ", __func__,brand_id.c_str());
        }else{
            //DB query OK
            UB_LOG_NOTICE("[GET PHONE SERIES LIST OK] %s brand_id = %s ", __func__,brand_id.c_str());
#ifndef REAL_TIME_DATA
            //insert into cache
            insert_phone_headword_list_by_brand_id_toCache(brand_id,ret,result);
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
int get_phone_headword_by_brand_id_per_page(string brand_id,int unit_num,int unit_times,int* phones_cnt,char** head_word){
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
    total_cnt = get_phone_headword_cnt_by_brand_id(brand_id);
    if( 0 >= total_cnt ){
        *phones_cnt = 0;
        return 0;
    }
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
    phone_head_word_st *result = (phone_head_word_st *) malloc(sizeof(phone_head_word_st) * total_cnt);
    if( NULL == result ){
        return -1;
    }
    //初始化内存
    memset(result,0,sizeof(phone_head_word_st) * total_cnt);
    
    ret = get_phone_headword_list_by_brand_id(brand_id,total_cnt,result);
    //合并后的总数
    *phones_cnt = ret;
#ifndef REAL_TIME_DATA
    insert_phone_headword_cnt_by_brand_id_toCache(brand_id,ret);
#endif
    UB_LOG_NOTICE("[GET PHONES BY PLAT_ID OK %s]", __func__);
    UB_LOG_NOTICE("%s unit_num %d unit_times %d start_index %d end_index %d", __func__,unit_num,unit_times,start_index,end_index);
    //返回每一页
    index = start_index;
    for( i = 0 ; index < end_index && i < unit_num ; index++,i++){
        //合成手机名 厂商-手机型号
        //copy_result(head_word[i],result[index].head_word);
        size_t len = strlen(result[index].head_word);
        strncpy(head_word[i],result[index].head_word,len);
        head_word[i][255] = '\0';
    }
    //释放空间
    free(result);
    return 0;
}

int McpServlet::get_phone_headword_from_maker(const idl::mcp_get_phone_headword_from_maker_params& in, idl::mcp_get_phone_headword_from_maker_response& out){

    //输入参数
    const string brand_id = in.maker_id();
    const uint32_t unit_num  = in.unit_num();
    const uint32_t unit_times = in.unit_times();
    //输出参数
    int total_cnt = 0;

    uint32_t show_cnt = 0;


    //临时变量
    uint32_t i = 0;
    int FULL_NAME_LEN = 256;

    if( unit_num <= 0 || unit_times <= 0){
        return 0;
    }

    //char **head_word = malloc2d(unit_num,FULL_NAME_LEN);
    char **head_word = new char *[unit_num];
    for( i = 0; i < unit_num; ++i )
    {
        head_word[i] = new char[FULL_NAME_LEN];
        memset(head_word[i],0x00,FULL_NAME_LEN);
    }
    if(NULL == head_word){
        return 0;
    }

    //info_id 是 厂商 ID
    get_phone_headword_by_brand_id_per_page(brand_id,unit_num,unit_times,&total_cnt,head_word);
    //返回结果
    out.m_result_params()->set_headword_count(total_cnt);

    show_cnt = get_cnt_this_page(total_cnt,unit_times,unit_num);

    for( i = 0; i < show_cnt; i++){
        out.m_result_params()->set_head_word(i,head_word[i]);
    }

    //释放资源
    //free2d(head_word,unit_num); 
    for( i = 0; i < unit_num; ++i )
    {
        delete head_word[i];
    } 
    delete[] head_word;
    
    return MCP_OK;
}

