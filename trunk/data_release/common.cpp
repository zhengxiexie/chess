#include "common.h"
#include <set>
#include <string>
#include <vector>
#include "redis_connpool.h"
#include <stdio.h>
#include "ub_server.h"
#include <stdlib.h>
using namespace std;

map<string, pub_prop_record>  pub_prop_record_map;
// [s] add by gwj for opt_update
TimeStampProcessor::TimeStampProcessor()
{
    this->ts_fd = fopen("/home/work/upmongodb/last_update_timestamp.txt", "rb");

    if(this->ts_fd == NULL)
    {
        this->get_time_now(this->last_update_ts);
        printf("TS File Open Error! Use Now[%s]\n", this->last_update_ts.c_str());
        return;
    }

    FILEShell fshell(this->ts_fd);

    char timestamp[128];
    memset(timestamp, 0x00, sizeof(timestamp));

    fgets(timestamp, sizeof(timestamp), this->ts_fd);
    printf("Get TS Line[%s]\n", timestamp);

    int line_len = strlen(timestamp);
    if(line_len == 0)
    {
        this->get_time_now(this->last_update_ts);
        printf("Empty Time. Use Now[%s]\n", this->last_update_ts.c_str());
    }
    else
    {
        this->last_update_ts = timestamp;
        printf("Use File Time[%s]\n", this->last_update_ts.c_str());
    }
}

void TimeStampProcessor::get_time_now(string& tm_now)
{
    time_t cur = time(NULL);
    time(&cur);

    struct tm *curtm = localtime(&cur);

    char time_str[64];
    memset(time_str, 0x00, sizeof(time_str));

    sprintf(time_str, "%d-%02d-%02d %02d:%02d:%02d",
                curtm->tm_year+1900, curtm->tm_mon+1,
                curtm->tm_mday, curtm->tm_hour,
                curtm->tm_min,  curtm->tm_sec);

    tm_now = time_str;
}

TimeStampProcessor::~TimeStampProcessor()
{
    this->get_time_now(this->last_update_ts);

    this->ts_fd = fopen("/home/work/upmongodb/last_update_timestamp.txt", "w");
    FILEShell fshell(this->ts_fd);

    if(this->ts_fd != NULL)
    {
        fputs(this->last_update_ts.c_str(), this->ts_fd);
        printf("Set UpdateTime[%s]\n", this->last_update_ts.c_str());
    }
    else
    {
        printf("TS File Error!\n");
    }
}

const char* TimeStampProcessor::get_last_update_ts()
{
    return this->last_update_ts.c_str();
}
// [e] add by gwj opt_update

// [s] add by gwj for rename_mongodb_collection
void rename_mongodb_collection(mongo* mongodb_conn,
                               string db_name,
                               string real_name,
                               string bak_name)
{
    mongo_cmd_drop_collection(mongodb_conn, db_name.c_str(), real_name.c_str(), NULL);

    string bak_entire_name = db_name + "." + bak_name;
    string real_entire_name = db_name + "." + real_name;

    bson rename;
    bson_init(&rename);
    bson_append_string( &rename , "renameCollection", bak_entire_name.c_str());
    bson_append_string( &rename , "to", real_entire_name.c_str());
    bson_finish(&rename);

    mongo_run_command(mongodb_conn, "admin", &rename, NULL);
}
// [e] add by gwj for rename_mongodb_collection

// [s] add by gwj for random_num
int get_random_number(int min, int max)
{
    int range = max - min;
    srand((unsigned int)time(0));
    int random = (rand()%range) + min;

    //printf("get random[%d]", random);
    return random;
}
// [e] add by gwj for random_num

void nowtime()
{
	time_t cur = time(NULL);
	time(&cur);
	struct tm *curtm = localtime(&cur);
	printf("%d-%02d-%02d %02d:%02d:%02d 星期 %d\n",
			curtm->tm_year+1900, curtm->tm_mon+1,
			curtm->tm_mday, curtm->tm_hour,
			curtm->tm_min,  curtm->tm_sec,
			curtm->tm_wday);
	fflush(stdout);
}


/* --------------------------------------------------------------------------*/
/**
 * @brief
 *	过滤出符合条件的c_id, 在mcp_content和mcp_content_data表存在记录的所有渠道的id
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
int get_channel_info()
{
    MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1,  num_row ;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	int num_col;

    sprintf(query_sql,
            "select C.id,D.channel_info from mcp_content AS C INNER JOIN mcp_content_data AS D ON C.`id` = D.`c_id` where C.`enable`=1");
    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
            printf("query is not ok\n");
            return -1;
    }
    mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
            printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
            return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);
    if ( 2 != num_col )
    {
            printf(" colum count %d is err\n",num_col);
            mysql_free_result(mysql_result);
            return -1;
    }

    string id;
    for (f1 = 0; f1 < num_row; f1++)
    {
            mysql_row = mysql_fetch_row(mysql_result);
            id = mysql_row[1];
            /* if(id.compare("DuoKu") == 0 || id.compare("TEMP") == 0 ) */
            /* { */
                    s_cids.insert(mysql_row[0]);
            /* } */
    }
    printf("id set count is %d\n", s_cids.size());
    return 0;
}

int get_channel_info2()
{
    MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1,  num_row ;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	int num_col;

    sprintf(query_sql,
            "select C.id from mcp_content AS C where C.`enable`=1 and C.`type_id`=11");
    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
            printf("query is not ok\n");
            return -1;
    }
    mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
            printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
            return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);
    if ( 1 != num_col )
    {
            printf(" colum count %d is err\n",num_col);
            mysql_free_result(mysql_result);
            return -1;
    }

    string id;
    for (f1 = 0; f1 < num_row; f1++)
    {
            mysql_row = mysql_fetch_row(mysql_result);
			s_cids.insert(mysql_row[0]);
    }
    printf("id set count is %d\n", s_cids.size());
    return 0;
}


/* --------------------------------------------------------------------------*/
/**
 * @brief
 *	从redis获取用户的平均分（小数）.
 *
 * @Param c_id
 *
 * @Steps
 *	1.根据标志位选择是按照计算获得star, 还是直接从库中获取star
 *	2.redis中有两种键,
 *		一种是hash键值对，键是package_name+user_id, field是rating的形式, 如:
 *		com.wanmei.mini.condor.DK:UserId:32602014 rating => "5"
 *		另一种是hash键值对，键是score, field是package_name+version_code+imei, 如:
 *		score com.sxiaoao.moto3dOnline_27_351554058427924 => "3"
 *	3.分数只可能在1-5分之间，因此把每个分数对应的人数放进数组中去
 *	4.计算平均分，返回小数
 */
/* ----------------------------------------------------------------------------*/
/* 2013.05.28 add by zhengxie 修改用户评分逻辑 beg*/
double get_star_by_c_id(const char *c_id)
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int num_col, num_row ;
	char query_sql[2048];

	/* 根据标志位选择是按照计算获得star, 还是直接从库中获取star beg*/
	memset(query_sql, 0x00, sizeof(query_sql));
    sprintf(query_sql,
            "select star_flag, star from mcp_content_game_ext where `c_id`=%s", c_id);

    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
            printf("query is not ok\n");
            return 0.0;
    }
    mysql_result = mysql_store_result(&mysql_conn);
    if( mysql_result==NULL ){
            printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
            return 0.0;
    }
	num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);
    if ( 2 != num_col) {
            mysql_free_result(mysql_result);
            return 0.0;
    }
	int star_flag;
    double db_star;
	if ( 0 != num_row) {
       //     printf(" 游戏id:%s 没有对应的包名。\n",c_id);
		    mysql_row = mysql_fetch_row(mysql_result);
			star_flag = atoi(mysql_row[0]);
			if(1 == star_flag) {
				db_star = atof(mysql_row[1]);
                //printf("db_star:%ld\n", db_star);
				mysql_free_result(mysql_result);
				return (double)db_star;
			}
    }
	/* 根据标志位选择是按照计算获得star, 还是直接从库中获取star end*/

	memset(query_sql, 0x00, sizeof(query_sql));
    sprintf(query_sql,
            "select package_name, version_code from mcp_content_appinfo where `c_id`=%s", c_id);

    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
            printf("query is not ok\n");
            return 0.0;
    }
    mysql_result = mysql_store_result(&mysql_conn);
    if( mysql_result==NULL ){
            printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
            return 0.0;
    }

	num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);
    if ( 2 != num_col) {
            mysql_free_result(mysql_result);
            return 0.0;
    }
	string package_name, version_code;
	if ( 0 != num_row) {
       //     printf(" 游戏id:%s 没有对应的包名。\n",c_id);
		    mysql_row = mysql_fetch_row(mysql_result);
			package_name = mysql_row[0];
			version_code = mysql_row[1];
			mysql_free_result(mysql_result);
    }

	vector<string> key_v, key_v2, key_v3;
	double score_person[5] = {0.0,0.0,0.0,0.0,0.0};

    redisContext *c;
    c=redisConnPool::getInstance()->getConnection();
    if(c==NULL){
        printf( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
        return 0.0;
    }

    redisReply *reply=NULL, *reply2=NULL, *reply3=NULL;
	if ( 0 != num_row)
	{
       //     printf(" 游戏id:%s 没有对应的包名。\n",c_id);
	   if(strcmp(package_name.c_str(),""))
	   {
			reply = (redisReply*)redisCommand( c, "KEYS *%s*", package_name.c_str() );
			if(reply!=NULL){
			   for (uint32_t i = 0; i < reply->elements; ++i) {
					redisReply* childReply = reply->element[i];
					key_v.push_back( childReply->str );
			   }
			   freeReplyObject(reply);
			}

			reply2 = (redisReply*)redisCommand( c, "HKEYS score" );
			if(reply2!=NULL)
			{
			   for (uint32_t i = 0; i < reply2->elements; ++i) {
					redisReply* childReply = reply2->element[i];
					string key = childReply->str;
					size_t pos = key.rfind("_");
					string tmp = key.substr(0,pos);
					string pattern = package_name + "_" + version_code ;
					if( !strcmp(tmp.c_str(), pattern.c_str()) ){
						key_v2.push_back( childReply->str );
					}
			   }
			   freeReplyObject(reply2);
			}
	   }
    }

    reply3 = (redisReply*)redisCommand( c, "KEYS Cid:%s*",c_id );

    //redisConnPool::getInstance()->releaseConnection(c);

	if(reply3!=NULL)
	{
	   for (uint32_t i = 0; i < reply3->elements; ++i) {
			redisReply* childReply = reply3->element[i];
			key_v3.push_back( childReply->str );
	   }
       freeReplyObject(reply3);
    }
	/* else{ */
       /* printf( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ ); */
       /* return 0.0; */
    /* } */

	for(uint32_t i=0; i<key_v.size(); i++)
	{
		reply = (redisReply*)redisCommand( c, "HGET %s rating", key_v[i].c_str() );
		//redisConnPool::getInstance()->releaseConnection(c);
		if( reply->type!=REDIS_REPLY_ERROR) {
			score_person[atoi(reply->str)-1]++;
		}
		freeReplyObject(reply);
	}

	for(uint32_t i=0; i<key_v3.size(); i++)
	{
		reply3 = (redisReply*)redisCommand( c, "HGET %s rating", key_v3[i].c_str() );
		//redisConnPool::getInstance()->releaseConnection(c);
		if( reply3->type!=REDIS_REPLY_ERROR )
		{
			score_person[atoi(reply3->str)-1]++;
		}
		/* else{ */
		/* 	freeReplyObject(reply); */
		/* 	printf( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ ); */
		/* 	return 0.0; */
		/* } */
		freeReplyObject(reply3);
	}

	for(uint32_t i=0; i<key_v2.size(); i++)
	{
		reply2 = (redisReply*)redisCommand( c, "HGET score %s", key_v2[i].c_str() );
		//redisConnPool::getInstance()->releaseConnection(c);
		if( reply->type!=REDIS_REPLY_ERROR )
		{
			score_person[atoi(reply2->str)-1]++;
		}
		/* else{ */
		/* 	freeReplyObject(reply2); */
		/* 	printf( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ ); */
		/* 	return 0.0; */
		/* } */
		freeReplyObject(reply2);
	}
	redisConnPool::getInstance()->releaseConnection(c);

	double total_person = (double)(score_person[0]+score_person[1]+score_person[2]+score_person[3]+score_person[4]);
	double total_score = (double)(1.0*score_person[0]+2.0*score_person[1]+3.0*score_person[2]+4.0*score_person[3]+5.0*score_person[4]);
	double star = 0.0;
	if(total_person != 0.0){
		star = total_score / total_person;
	}
//    printf("star:%lf", star);
    //UB_LOG_DEBUG( "c_id:[%s],star:[%lf]\n", c_id, star);
	/* 洪涛说线上暂时还是用int */
	return (int)star;
}
/* 2013.05.28 add by zhengxie 修改用户评分逻辑 end*/

/* 2013.10.14 add by zhengxie 把mcp_content_pub_prop信息放到map中 beg*/
int get_pub_prop_info()
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int num_col, num_row ;
	char query_sql[2048];
	pub_prop_record pub_prop;

	memset(query_sql, 0x00, sizeof(query_sql));
    sprintf(query_sql,
            "select id, name, color, prop_url from mcp_content_pub_prop order by id");

    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
            printf("query is not ok\n");
            return 0.0;
    }
    mysql_result = mysql_store_result(&mysql_conn);
    if( mysql_result==NULL ){
            printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
            return -1;
    }
	num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);
    if ( 4 != num_col) {
            mysql_free_result(mysql_result);
			return -1;
    }
	for (int i=0; i<num_row; i++) {
		    mysql_row = mysql_fetch_row(mysql_result);
			pub_prop.id = mysql_row[0];
			pub_prop.name = mysql_row[1];
			pub_prop.color = mysql_row[2];
			pub_prop.prop_url = mysql_row[3];
			pub_prop_record_map[mysql_row[0]] = pub_prop;
    }
	return 0;
}


/* 2013.10.14 add by zhengxie 把mcp_content_pub_prop信息放到map中 end*/

int set_mcp_game_content()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

// [s] add by gwj for opt_update
        const char* update_ts = TimeStampProcessor::instance().get_last_update_ts();
// [e] add by gwj for opt_update

        sprintf(col,"mcp_content_bak");
        sprintf(ns,"mcp.mcp_content_bak");
        sprintf(query_sql,
                        "SELECT C.id, C.name, C.type_id, GE.updatedate, C.ext_id, GE.info, GE.version, GE.method, GE.star, GE.network_id, "
                        "GE.game_code, GE.cp_id, GE.grant_id, CL.priority, C.charge_typeid, C.charge_value, GE.updatedate, "
                        "GROUP_CONCAT(DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`class_id` ), "
                        "GROUP_CONCAT( DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
                        "GROUP_CONCAT( DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`priority` DESC ), "
                        "GROUP_CONCAT( DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
                        "GROUP_CONCAT( DISTINCT R.`ptype_id`, \",\", R.`priority` ORDER BY R.`ptype_id` ), "
                        "GE.`enName`, GE.`language`,  GE.`adapt`, "
                        "A.`package_name`, A.`version`, A.`sdk_version`, A.`version_code`, "
                        "GE.`class_weight`, GE.`latest_weight`, GE.`download_weight`, GE.`pop_weight`, GE.`updatedate` "
                // [s] add by gwj for game_public_property
                        ", GE.`pub_props`, GE.`client_class_weight`, GE.`client_latest_weight`, GE.`client_download_weight`, GE.`client_pop_weight` "
                // [e] add by gwj for game_public_property
                // [s] add by gwj for union_action_game
                        ", UACT.`start_action`, UACT.`has_account` "
                // [e] add by gwj for union_action_game
                // [s] add by gwj for apk_md5
                        ", A.`apk_md5` "
                // [e] add by gwj for apk_md5
                // [s] add by zhengxie for search
						", GE.`search_weight` "

                        "FROM  mcp_content AS C "

                // [s] add by gwj for eliminate BaiduApp
                        "INNER JOIN mcp_content_data AS CDATA ON (CDATA.c_id = C.id AND CDATA.channel_info != 'BaiduApp') "
                // [e] add by gwj for eliminate BaiduApp

                        "INNER JOIN (mcp_content_game_ext AS GE "
                        "INNER JOIN mcp_content_cp AS CP ON CP.id = GE.cp_id AND CP.enable = 1) "
                        "ON GE.id = C.ext_id AND GE.enable = 1 AND GE.status = 1 "
                        "AND (C.type_id = 1 OR C.type_id = 2 OR C.type_id = 11 OR C.type_id = 14 OR C.type_id = 25 OR C.type_id = 26 OR C.type_id = 33) "
                        "INNER JOIN  mcp_content_class_list AS CL ON CL.c_id = C.id AND CL.enable = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` "
                        "LEFT JOIN `MCP`.`mcp_content_appinfo` AS A ON A.`c_id` = C.`id` "
                        // [s] add by gwj for union_action_game
                        "LEFT JOIN `MCP`.`mcp_union_action_game_info` AS UACT ON UACT.`game_id` = C.`id` "
                        // [e] add by gwj for union_action_game
                        //"LEFT JOIN `MCP`.`mcp_content_data` AS D ON C.`id` = D.`c_id` "
                        //"LEFT JOIN `MCP`.`mcp_content_data` AS D ON C.`id` = D.`c_id` "
                        //"WHERE C.enable = 1 AND C.status = 1 "
                        "WHERE C.enable = 1 AND C.status = 1 "

                // [s] add by gwj for opt_update
//                        "AND GE.gdate > \"%s\" "
                // [e] add by gwj for opt_update

                        "GROUP BY C.id "
                        "ORDER BY GE.gdate DESC "

                // [s] add by gwj for opt_update
//                      ,update_ts
                // [e] add by gwj for opt_update
                        );

                     printf("sql[%s]\n", query_sql);
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }

        // [s] delete by gwj for opt_update
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        // [e] delete by gwj for opt_update

        mysql_result = mysql_store_result(&mysql_conn);
        if(mysql_result==NULL){
                printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
                return 0;
        }
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        // [s] modify by gwj for game_public_property
        if ( 43 != num_col )
        // [s] modify by gwj for game_public_property
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        printf("game count [%d] \n",num_row);
        string channel_info;

        bson key[1];

        // 建立 ID 索引
        bson_init( key );
        bson_append_int( key, "id", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        // 建立 class.id 索引
        bson_init( key );
        bson_append_int( key, "class.id", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        // 建立  column.id 索引
        bson_init( key );
        bson_append_int( key, "column.id", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        redisContext *c;
        c=redisConnPool::getInstance()->getConnection();
        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                if( s_cids.find(mysql_row[0]) == s_cids.end())
                {
                        continue;
                }
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );
                if( strcmp(mysql_row[2], "2") == 0 ){
                        bson_append_string( &b, "content_type", "1" );
                }else if( strcmp(mysql_row[2], "25") == 0 ){
                        bson_append_string( &b, "content_type", "11" );
                }else{
                        bson_append_string( &b, "content_type", mysql_row[2] );
                }

                string class_list(mysql_row[17]);
                bson_append_class_info(&b,class_list);

                // [s] modify by gwj for use_same_format
                bson_append_time_t( &b , "logdate" , date_to_int(mysql_row[3]) );
                // [e] modify by gwj for use_same_format

                bson_append_string( &b , "ext_id" , mysql_row[4] );
                bson_append_string( &b , "info" , mysql_row[5] );
                bson_append_string( &b , "version" , mysql_row[6] );
                bson_append_string( &b , "method" , mysql_row[7] );

				/* 修改星级逻辑 */
                /* bson_append_int( &b , "star" , atoi(mysql_row[8]) ); */
				//double star = get_star_by_c_id(mysql_row[0]);
                double star = getStarFromRedis(c, mysql_row[0]);
                bson_append_double( &b , "star" , star );

                bson_append_int( &b , "network_id" , atoi(mysql_row[9]) );
                bson_append_string( &b , "game_code" , mysql_row[10] );
                bson_append_string( &b , "cp_id" , mysql_row[11] );
                bson_append_string( &b , "grant_id" , mysql_row[12] );
                bson_append_int( &b , "priority" , atoi(mysql_row[13]) );
                bson_append_string( &b , "default_charge_typeid" , mysql_row[14] );
                bson_append_string( &b , "default_charge_value" , mysql_row[15] );
                bson_append_string( &b , "modify_date" , mysql_row[16] );
                if( NULL != mysql_row[25] )
                {
                        bson_append_string( &b , "package_name" , mysql_row[25] );
                        bson_append_string( &b , "version_appinfo" , mysql_row[26] );
                        bson_append_int( &b , "sdk_version" , atoi(mysql_row[27]) );
                        bson_append_string( &b , "version_code" , mysql_row[28] );
                        // [s] add by gwj for apk_md5
                        SET_BSON_VALUE_STRING_B(&b, mysql_row[41], "apk_md5")
                        // [e] add by gwj for apk_md5
						// [s] add by zhengxie for search_weight
                        SET_BSON_VALUE_STRING_B(&b, mysql_row[42], "search_weight")
                        // [e] add by zhengxie for search_weight

                }


                char index[8];
                int j = 0;
                size_t pos;
                string ptype_list;
                if( mysql_row[21] != NULL  )
                {
                        ptype_list = mysql_row[21];
                        //printf("%s %s\n",mysql_row[0],ptype_list.c_str());
                }
                string ptype,ptype6;
                int priority,priority6=0;
                int tmp;
                j = 1;
                //printf( "id[%s], ptype_list:[%s], priority_list[%s]\n", mysql_row[0], ptype_list.c_str(), priority_list.c_str() );
                size_t pos1(0), pos2(0);
                while( !ptype_list.empty() && j<=5 )
                {
                        pos1=ptype_list.find(',');
                        if( pos1 != string::npos )
                        {
                                ptype = ptype_list.substr( 0, pos1 );
                                ptype_list.erase( 0, pos1+1 );
                        }
                        else
                        {
                                break;
                        }

                        pos2=ptype_list.find(',');
                        priority = atoi(ptype_list.substr(0,pos2).c_str());
                        ptype_list.erase( 0, pos2+1 );

                        if(j == 5 && !ptype_list.empty())
                        {
                                pos1=ptype_list.find(',');
                                if( pos1 != string::npos )
                                {
                                        ptype6 = ptype_list.substr( 0, pos1 );
                                        ptype_list.erase( 0, pos1+1 );
                                }
                                else
                                {
                                        goto out;
                                }

                                pos2=ptype_list.find(',');
                                priority6 = atoi(ptype_list.substr(0,pos2).c_str());
//                                printf("priority 6 is %d", priority6);
                                ptype_list.erase( 0, pos2+1 );
                        }
out:

                        tmp = atoi(ptype.c_str());
                        if( tmp < j )
                        {
                                continue;
                        }
                        else if( tmp == j )
                        {
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                if(j == 5)
                                {
                                        priority += priority6;
                                        //printf("priority is %d", priority);
                                }
                                bson_append_int( &b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        else
                        {
                                while(j<tmp)
                                {
                                        memset( index, 0x00, sizeof(index) );
                                        sprintf( index, "ptype_%d", j );
                                        bson_append_int( &b, index, 0 );
                                        //   printf( "%s:0 ", index );
                                        ++j;
                                }
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                if(j == 5)
                                {
                                        priority += priority6;
                                        //printf("priority is %d", priority);
                                }
                                bson_append_int( &b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }

                        ++j;
                }
                while( j<= 5 )
                {
                        memset( index, 0x00, sizeof(index) );
                        sprintf( index, "ptype_%d", j );
                        //printf( "%s:0 ", index );

                        // [s] mod by gwj for random download
                        //bson_append_int( &b, index, 0 );
                        int random_dl_cnt = get_random_number(500, 1000);
                        bson_append_int( &b, index, random_dl_cnt );
                        // [s] mod by gwj for random download

                        ++j;
                }
                // printf( "\n" );


                string label_list;
                if( mysql_row[18] != NULL )
                {
                        label_list = mysql_row[18];
                }

                bson_append_start_array( &b , "label_id" );
                j = 0;
                string label_id;
                while( !label_list.empty() )
                {
                        pos = label_list.find(',');
                        label_id = label_list.substr( 0, pos );
                        //printf( "label_list:[%s], label_id:[%s]\n", label_list.c_str(), label_id.c_str() );
                        if( pos != string::npos )
                        {
                                label_list.erase( 0, pos+1 );
                        }
                        else
                        {
                                label_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf( index, "%d", j );
                        bson_append_string( &b, index, label_id.c_str() );
                        ++j;
                }
                bson_append_finish_object( &b );

                string column_list;
                if( mysql_row[19] != NULL )
                {
                        column_list = mysql_row[19];
                }
                bson_append_column_info(&b, column_list);

                string img_info_list;
                if( mysql_row[20] != NULL  )
                {
                        img_info_list = mysql_row[20];
                }
                bson_append_start_array( &b , "img_info" );
                j = 0;
                string img_type, img_url;
                //printf( "img_info_list:[%s]\n", img_info_list.c_str() );
                while( !img_info_list.empty() )
                {
                        pos1 = img_info_list.find(',');
                        img_type = img_info_list.substr( 0, pos1 );
                        if( pos1 != string::npos )
                        {
                                img_info_list.erase( 0, pos1+1 );
                        }
                        else
                        {
                                img_info_list.clear();
                                break;
                        }
                        pos2 = img_info_list.find(',');
                        img_url = img_info_list.substr( 0, pos2 );
                        if( pos2 !=  string::npos )
                        {
                                img_info_list.erase( 0, pos2+1 );
                        }
                        else
                        {
                                img_info_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf( index, "%d", j );
                        bson_append_start_object( &b, index );
                        bson_append_string( &b, "img_type", img_type.c_str() );
                        bson_append_string( &b, "img_url", img_url.c_str() );
                        bson_append_finish_object( &b );
                        //printf( "{img_type:%s, img_url:%s} ", img_type.c_str(), img_url.c_str() );
                        ++j;
                }
                bson_append_finish_object( &b );

                bson_append_string( &b, "enName",  mysql_row[22] );
                bson_append_string( &b, "language",  mysql_row[23] );
                bson_append_string( &b, "adapt",  mysql_row[24] );

                /*
                   printf(" id is %s\n", mysql_row[0]);
                   printf(" name is %s\n", mysql_row[1]);
                   printf(" class_id is %s\n", mysql_row[2]);
                   printf(" logdate is %s\n", mysql_row[3]);
                   printf(" ext_id is %s\n", mysql_row[4]);
                   printf(" info is %s\n", mysql_row[5]);
                   printf(" version is %s\n", mysql_row[6]);
                   printf(" method is %s\n", mysql_row[7]);
                   printf(" star is %s\n", mysql_row[8]);
                   printf(" network_id is %s\n", mysql_row[9]);
                   printf(" game_code is %s\n", mysql_row[10]);
                   printf(" cp_id is %s\n", mysql_row[11]);
                   printf(" grant_id is %s\n", mysql_row[12]);
                   */
                bson_append_int(&b,"class_weight",atoi(mysql_row[29]));
                bson_append_int(&b,"latest_weight",atoi(mysql_row[30]));
                bson_append_int(&b,"download_weight",atoi(mysql_row[31]));
                bson_append_int(&b,"pop_weight",atoi(mysql_row[32]));
                bson_append_time_t(&b,"udate",date_to_int(mysql_row[33]));

                // [s] add by gwj for game_public_proerty
                bson_append_string( &b, "pub_prop_ids",  mysql_row[34] );
				bson_append_pub_prop_info(&b, mysql_row[34]);
                bson_append_int(&b,"client_class_weight",atoi(mysql_row[35]));
                bson_append_int(&b,"client_latest_weight",atoi(mysql_row[36]));
                bson_append_int(&b,"client_download_weight",atoi(mysql_row[37]));
                bson_append_int(&b,"client_pop_weight",atoi(mysql_row[38]));
                // [e] add by gwj for game_public_proerty

                // [s] add by gwj for union_action_game
                SET_BSON_VALUE_STRING_B(&b, mysql_row[39], "union_start_action")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[40], "has_account")
                // [e] add by gwj for union_action_game

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp", "mcp_content", "mcp_content_bak");
        // [e] add by gwj for rename collection name

        return 0;
}

int set_mcp_game_content_new()
{
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;
    int f1,  num_row ;
    char query_sql[2048];
    memset(query_sql, 0x00, sizeof(query_sql));
    bson b, cond[1];
    mongo_write_concern write_concern[1];
    int num_col;
    int res;
    char col[64];
    char ns[64];
    memset( col, 0x00, sizeof(col));
    memset(ns,0x00,sizeof(ns));

    const char* update_ts = TimeStampProcessor::instance().get_last_update_ts();

    sprintf(col,"mcp_content_bak");
    sprintf(ns,"mcp.mcp_content_bak");
    // [s] delete by gwj for opt_update
    mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
    // [e] delete by gwj for opt_update"

    mongo_write_concern_init( write_concern );
    write_concern->w = 1;
    mongo_write_concern_finish( write_concern );

    //printf("before first query\n");
    nowtime();
    ////-------------------sql query part1 start-----------------
    sprintf(query_sql,
            "SELECT DISTINCT(C.id), C.name, C.type_id, GE.updatedate, C.ext_id, GE.info, GE.version, GE.method, GE.star, GE.network_id, "//0~9
            "GE.game_code, GE.cp_id, GE.grant_id, C.charge_typeid, C.charge_value, GE.updatedate, "//10~15
            "GE.`enName`, GE.`language`,  GE.`adapt`, "//16~18
            "GE.`pub_props`, GE.`client_class_weight`, GE.`client_latest_weight`, GE.`client_download_weight`, GE.`client_pop_weight`, "//19~23
			// [e] add by zhengxie for editor_recom
            "GE.`class_weight`, GE.`latest_weight`, GE.`download_weight`, GE.`pop_weight`, GE.`updatedate`, GE.`editor_recom`, GE.`search_weight`, GE.`show_highspeed`, GE.`bind_resource_id` "//24~29
            "FROM  mcp_content AS C "
            "INNER JOIN  mcp_content_class_list AS CL ON CL.c_id = C.id AND CL.enable = 1 "
            "INNER JOIN mcp_content_data AS CDATA ON (CDATA.c_id = C.id AND CDATA.channel_info != 'BaiduApp') "
            "INNER JOIN (mcp_content_game_ext AS GE "
            "INNER JOIN mcp_content_cp AS CP ON CP.id = GE.cp_id AND CP.enable = 1) "
            "ON GE.id = C.ext_id AND GE.enable = 1 AND GE.status = 1 "
            "WHERE C.enable = 1 AND C.status = 1 "
            "AND (C.type_id = 1 OR C.type_id = 2 OR C.type_id = 11 OR C.type_id = 14 OR C.type_id = 25 OR C.type_id = 26 OR C.type_id = 31 OR C.type_id = 33) "
            //"AND (GE.is_dev_game = 0 OR GE.is_dev_game = 2) "
           );
    //printf("sql[%s]\n", query_sql);
    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if ( 33 != num_col )
    {
        printf(" colum count1 %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    //printf("game first query count [%d] \n",num_row);
    string channel_info;

    bson key[1];

    // 建立 ID 索引
    bson_init( key );
    bson_append_int( key, "id", 1 );
    bson_finish( key);
    mongo_create_index( mongodb_conn, ns, key, 0, NULL );
    bson_destroy( key );

    // 建立 class.id 索引
    bson_init( key );
    bson_append_int( key, "class.id", 1 );
    bson_finish( key);
    mongo_create_index( mongodb_conn, ns, key, 0, NULL );
    bson_destroy( key );

    // 建立  column.id 索引
    bson_init( key );
    bson_append_int( key, "column.id", 1 );
    bson_finish( key);
    mongo_create_index( mongodb_conn, ns, key, 0, NULL );
    bson_destroy( key );

    redisContext *c = redisConnPool::getInstance()->getConnection();
    for (f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        if( s_cids.find(mysql_row[0]) == s_cids.end())
        {
            continue;
        }
        bson_init( &b );
        bson_append_new_oid( &b, "_id" );
        bson_append_string( &b , "id" , mysql_row[0] );
        bson_append_string( &b , "name" , mysql_row[1] );
        if( strcmp(mysql_row[2], "2") == 0 ){
            bson_append_string( &b, "content_type", "1" );
        }else if( strcmp(mysql_row[2], "25") == 0 ){
            bson_append_string( &b, "content_type", "11" );
        }else{
            bson_append_string( &b, "content_type", mysql_row[2] );
        }

        bson_append_time_t( &b , "logdate" , date_to_int(mysql_row[3]) );
        bson_append_string( &b , "ext_id" , mysql_row[4] );
        bson_append_string( &b , "info" , mysql_row[5] );
        bson_append_string( &b , "version" , mysql_row[6] );
        bson_append_string( &b , "method" , mysql_row[7] );

        //double star = 2.0;//get_star_by_c_id(mysql_row[0]);
        double star = getStarFromRedis(c, mysql_row[0]);//get_star_by_c_id(mysql_row[0]);
        //printf("%s  %lf\n", mysql_row[0], star);
        //double star = get_star_by_c_id(mysql_row[0]);
        bson_append_double( &b , "star" , star );

        bson_append_int( &b , "network_id" , atoi(mysql_row[9]) );
        bson_append_string( &b , "game_code" , mysql_row[10] );
        bson_append_string( &b , "cp_id" , mysql_row[11] );
        bson_append_string( &b , "grant_id" , mysql_row[12] );

        bson_append_string( &b , "default_charge_typeid" , mysql_row[13] );
        bson_append_string( &b , "default_charge_value" , mysql_row[14] );
        bson_append_string( &b , "modify_date" , mysql_row[15] );

        bson_append_string( &b, "enName",  mysql_row[16] );
        bson_append_string( &b, "language",  mysql_row[17] );
        bson_append_string( &b, "adapt",  mysql_row[18] );

        bson_append_string( &b, "pub_prop_ids",  mysql_row[19] );
		bson_append_pub_prop_info(&b, mysql_row[19]);
        bson_append_int(&b,"class_weight",atoi(mysql_row[20]));
        bson_append_int(&b,"latest_weight",atoi(mysql_row[21]));
        bson_append_int(&b,"download_weight",atoi(mysql_row[22]));
        bson_append_int(&b,"pop_weight",atoi(mysql_row[23]));

        bson_append_int(&b,"client_class_weight",atoi(mysql_row[24]));
        bson_append_int(&b,"client_latest_weight",atoi(mysql_row[25]));
        bson_append_int(&b,"client_download_weight",atoi(mysql_row[26]));
        bson_append_int(&b,"client_pop_weight",atoi(mysql_row[27]));
        bson_append_time_t(&b,"udate",date_to_int(mysql_row[28]));
		//add by zhengxie
		if(NULL != mysql_row[29]){
			bson_append_string( &b,"editor_recom",mysql_row[29] );
		}else{
			bson_append_string( &b,"editor_recom","" );
		}
		SET_BSON_VALUE_STRING_B(&b, mysql_row[30], "search_weight")
		SET_BSON_VALUE_STRING_B(&b, mysql_row[31], "show_highspeed")
		SET_BSON_VALUE_STRING_B(&b, mysql_row[32], "bind_resource_id")

        bson_finish( &b );
        mongo_insert( mongodb_conn , ns , &b, NULL );
        bson_destroy( &b );
    }
    ////------------------sql query part1 end--------------------

    //printf("before second query\n");
    nowtime();
    ////------------------sql query part2 start------------------
    memset(query_sql, 0, sizeof(query_sql));
    sprintf(query_sql,
            "SELECT DISTINCT(C.id), CL.priority, "
            "GROUP_CONCAT(DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`class_id` ), "
            "GROUP_CONCAT( DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
            "GROUP_CONCAT( DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`priority` DESC ), "
            "GROUP_CONCAT( DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
            "GROUP_CONCAT( DISTINCT R.`ptype_id`, \",\", R.`priority` ORDER BY R.`ptype_id` ) "
            "FROM  mcp_content AS C "
            "INNER JOIN  mcp_content_class_list AS CL ON CL.c_id = C.id AND CL.enable = 1 "
            "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
            "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
            "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
            "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` "
            "WHERE C.enable = 1 AND C.status = 1 "
            "AND (C.type_id = 1 OR C.type_id = 2 OR C.type_id = 11 OR C.type_id = 14 OR C.type_id = 25 OR C.type_id = 26 OR C.type_id = 31) "
            "GROUP BY C.id"
        );

    //printf("sql[%s]\n", query_sql);
    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if ( 7!= num_col )
    {
        printf(" colum count2 %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    string ptype_list, lable_list, column_list, img_info_list, class_list;
    for (f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        bson_init(cond);
        bson_append_string(cond, "id", mysql_row[0]);
        bson_finish(cond);

        bson_init(&b);
        bson_append_start_object(&b, "$set");
        bson_append_int( &b , "priority" , atoi(mysql_row[1]) );
        class_list = mysql_row[2]==NULL?"":mysql_row[2];
        lable_list = mysql_row[3]==NULL?"":mysql_row[3];
        column_list = mysql_row[4]==NULL?"":mysql_row[4];
        img_info_list = mysql_row[5]==NULL?"":mysql_row[5];
        ptype_list=mysql_row[6]==NULL?"":mysql_row[6];
        bson_append_ptype_info(&b, ptype_list);
        bson_append_lable_info(&b, lable_list);
        bson_append_column_info(&b, column_list);
        bson_append_img_info2(&b, img_info_list);
        bson_append_class_info(&b,class_list);
        bson_append_finish_object(&b);
        bson_finish(&b);

        mongo_update(mongodb_conn, "mcp.mcp_content_bak", cond, &b, MONGO_UPDATE_BASIC, write_concern);
    }
    //printf("game second query count [%d] \n",num_row);
    ////------------------sql query part2 end--------------------

    //printf("before third query\n");
    nowtime();
    ////------------------sql query part3 start------------------
    memset(query_sql, 0, sizeof(query_sql));
    sprintf(query_sql,
        "SELECT DISTINCT(C.id), A.`package_name`, A.`version`, A.`sdk_version`, A.`version_code`, A.`apk_md5`, "
        "UACT.`start_action`, UACT.`has_account` "
        "FROM  mcp_content AS C "
        "INNER JOIN mcp_content_data AS CDATA ON (CDATA.c_id = C.id AND CDATA.channel_info != 'BaiduApp' AND CDATA.enable = 1) "
        "LEFT JOIN `MCP`.`mcp_content_appinfo` AS A ON A.`c_id` = C.`id` AND A.cdata_id = CDATA.id "
        "AND (C.type_id = 1 OR C.type_id = 2 OR C.type_id = 11 OR C.type_id = 14 OR C.type_id = 25 OR C.type_id = 26 OR C.type_id = 31) "
        "LEFT JOIN `MCP`.`mcp_union_action_game_info` AS UACT ON UACT.`game_id` = C.`id` "
        "WHERE C.enable = 1 AND C.status = 1 "
        //"AND (C.type_id = 1 OR C.type_id = 2 OR C.type_id = 11 OR C.type_id = 14 OR C.type_id = 25 OR C.type_id = 26) "
    );
    //printf("sql[%s]\n", query_sql);
    if (mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if ( 8!= num_col )
    {
        printf(" colum count %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }
    //printf("game third query count [%d] \n",num_row);

    for (f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        bson_init(cond);
        bson_append_string(cond, "id", mysql_row[0]);
        bson_finish(cond);

        bson_init(&b);
        bson_append_start_object(&b, "$set");
        bson_append_string( &b , "package_name" , mysql_row[1]==NULL?"":mysql_row[1] );
        bson_append_string( &b , "version_appinfo" , mysql_row[2]==NULL?"":mysql_row[2] );
        bson_append_int( &b , "sdk_version" , atoi(mysql_row[3]==NULL?"":mysql_row[3]) );
        bson_append_string( &b , "version_code" , mysql_row[4]==NULL?"":mysql_row[4] );
        bson_append_string( &b , "apk_md5" , mysql_row[5]==NULL?"":mysql_row[5] );
        bson_append_string( &b , "union_start_action", mysql_row[6]==NULL?"":mysql_row[6]);
        bson_append_string( &b , "has_account", mysql_row[7]==NULL?"":mysql_row[7]);
        bson_append_finish_object(&b);

        bson_finish(&b);

        mongo_update(mongodb_conn, "mcp.mcp_content_bak", cond, &b, MONGO_UPDATE_BASIC, write_concern);
    }

    ////------------------sql query part3 end--------------------
    //printf("after third query\n");
    nowtime();

    bson_destroy(&b);
    bson_destroy(cond);
    bson_destroy(key);
    mysql_free_result(mysql_result);
    rename_mongodb_collection(mongodb_conn, "mcp", "mcp_content", "mcp_content_bak");
    return 0;

}

/* --------------------------------------------------------------------------*/
/**
 * @brief 内部函数，将增量更新的每条记录插入到mcp_content_diff_data中
 *
 * @Param c_id
 * @Param bp
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
/* add by zhengxie for 增量更新*/
int set_mcp_diff_update_info_by_id(const char *c_id, bson *bp)
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_diff_data");
        sprintf(ns,"mcp.mcp_diff_update_info");
        sprintf(query_sql,
                    "select c_id, old_file_md5, old_cdata_id, new_cdata_id, diff_url, diff_pkg_size, channel_info, channel_id,  priority, enable "
                    "from mcp_content_diff_data where c_id=%s order by channel_info ", c_id);

		bson key[1];
        bson_init( key );
        bson_append_int( key, "old_file_md5", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 10 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        for (f1 = 0; f1 < num_row; f1++)
        {
                bson_init( &(*bp) );
                mysql_row = mysql_fetch_row(mysql_result);

                bson_append_new_oid( &(*bp), "_id" );
                bson_append_string( &(*bp), "c_id" , mysql_row[0] );
                bson_append_string( &(*bp), "old_file_md5" , mysql_row[1] );
                bson_append_string( &(*bp), "old_cdata_id" , mysql_row[2] );
                bson_append_string( &(*bp), "new_cdata_id" , mysql_row[3] );
                bson_append_string( &(*bp), "diff_url" , mysql_row[4] );
                bson_append_string( &(*bp), "diff_pkg_size" , mysql_row[5] );
                bson_append_string( &(*bp), "channel_info" , mysql_row[6] );
                bson_append_string( &(*bp), "channel_id" , mysql_row[7] );
                bson_append_string( &(*bp), "priority" , mysql_row[8] );
                bson_append_string( &(*bp), "enable" , mysql_row[9] );

                bson_finish(&(*bp));
                mongo_insert( mongodb_conn , ns , &(*bp), NULL );
                bson_destroy(&(*bp));
        }
        mysql_free_result(mysql_result);
        return 0;
}


/* --------------------------------------------------------------------------*/
/**
 * @brief 同步增量更新
 *
 * @Returns
 */
/* ----------------------------------------------------------------------------*/
/* add by zhengxie for 增量更新*/
int set_mcp_diff_update_info()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char ns[64];
        char col[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_diff_update_info");
        sprintf(query_sql,
                    "select distinct c_id from mcp_content_diff_data ");

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 1 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                string c_id = mysql_row[0];
                int result = set_mcp_diff_update_info_by_id(c_id.c_str(), &b);
                if(0!=result)
                {
                    printf(" colum count %d is err\n",num_col);
                    return -1;
                }
        }
        mysql_free_result(mysql_result);
        return 0;
}

int set_mcp_content_format()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));
        bson key[1];

        sprintf(col,"mcp_content_format_bak");
        sprintf(ns,"mcp.mcp_content_format_bak");

        sprintf(query_sql,
                        "select mcp_content_format.stype, mcp_content_format.stype_id, mcp_content_data.c_id, "
                        "mcp_content_format.cdata_id ,mcp_content_format.id from mcp_content_format "
                        "INNER JOIN  mcp_content_data "
                        //"INNER JOIN mcp_content_class_list ON  mcp_content_class_list.c_id = mcp_content_data.c_id ) "
                        "ON mcp_content_data.id = mcp_content_format.cdata_id "
                        "WHERE mcp_content_format.enable = 1 AND mcp_content_data.enable = 1 AND mcp_content_data.visible = 1 AND mcp_content_data.channel_info = 'DuoKu' "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }

        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 5 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "stype" , mysql_row[0] );
                bson_append_string( &b , "stype_id" , mysql_row[1] );
                bson_append_string( &b , "c_id" , mysql_row[2] );
                bson_append_string( &b , "cdata_id" , mysql_row[3] );
                bson_append_string( &b , "id" , mysql_row[4] );
                // bson_append_string( &b , "class_id" , mysql_row[4] );

                /*
                   printf(" id is %s\n", mysql_row[0]);
                   printf(" name is %s\n", mysql_row[1]);
                   printf(" class_id is %s\n", mysql_row[2]);
                   printf(" logdate is %s\n", mysql_row[3]);
                   printf(" ext_id is %s\n", mysql_row[4]);
                   printf(" info is %s\n", mysql_row[5]);
                   printf(" version is %s\n", mysql_row[6]);
                   printf(" method is %s\n", mysql_row[7]);
                   printf(" star is %s\n", mysql_row[8]);
                   printf(" network_id is %s\n", mysql_row[9]);
                   printf(" game_code is %s\n", mysql_row[10]);
                   printf(" cp_id is %s\n", mysql_row[11]);
                   printf(" grant_id is %s\n", mysql_row[12]);
                   */

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        bson_init( key );
        bson_append_int( key, "stype", 1 );
        bson_append_int( key, "stype_id", 1 );
        bson_append_int( key, "c_id", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp", "mcp_content_format", "mcp_content_format_bak");
        // [e] add by gwj for rename collection name

        return 0;
}


int set_mcp_content_class()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_class_bak");
        sprintf(ns,"mcp.mcp_content_class_bak");
        sprintf(query_sql,
                        "select mcp_content_class.id, mcp_content_class.name, "
                        "mcp_content_class.type_id from mcp_content_class"
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 3 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );
                bson_append_string( &b , "type_id" , mysql_row[2] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp", "mcp_content_class", "mcp_content_class_bak");
        // [e] add by gwj for rename collection name

        return 0;
}


int set_mcp_content_data()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_data_bak");
        sprintf(ns,"mcp.mcp_content_data_bak");

        // [s] add by gwj for opt_update
//        const char* update_ts = TimeStampProcessor::instance().get_last_update_ts();
        // [e] add by gwj for opt_update

            sprintf(
                query_sql,
                "SELECT D.`id`, D.`path_url`, D.`path_type`, D.`media_format_id`, "
                    "D.`size`, MF.`mtype`, CC.`charge_typeid`, D.`c_id`, CC.`value`, "
                    "D.`modify_date`, GROUP_CONCAT(CF.`stype`), GROUP_CONCAT(CF.`stype_id`), "
                    "D.`channel_info` , DTBD.`to_baidu_url` , D.`channel_id` , "
                    "APP.`package_name`, APP.`version_code`, APP.`version`, APP.`sdk_version` , "
                    "APP.`permissions`, APP.`features`, APP.`apk_info`, APP.`apk_md5`, "
					//add by zhengxie for 增量更新 file_md5唯一确定一个包
					"D.MD5 "
                    "FROM `MCP`.`mcp_content_appinfo` AS APP "
                    "LEFT JOIN `MCP`.`mcp_content_data` AS D on ( APP.`cdata_id` = D.`id` AND D.`enable` = 1 and D.`visible` = 1 AND "

                    // 渠道选择同步
                    "  D.`channel_info` != 'BaiduApp' ) "
                    // 渠道选择同步

                    "LEFT JOIN `MCP`.`mcp_duoku_to_baidu_url` AS DTBD ON DTBD.`cdata_id` = D.`id` "
                    "LEFT JOIN `MCP`.`mcp_media_format` AS MF ON D.`media_format_id` = MF.`id` "
                    "LEFT JOIN `MCP`.`mcp_content_format` AS CF ON (CF.`cdata_id` = D.`id` AND CF.`visible` = 1  AND CF.`enable` = 1) "
                    "LEFT JOIN `MCP`.`mcp_content_charge` AS CC ON CC.`cdata_id` = D.`id` "

                    // [s] add by gwj for opt_update
                    // "WHERE D.modify_date > \"%s\" "
                    // [e] add by gwj for opt_update

                    "GROUP BY D.`id` "

                    // [s] add by gwj for opt_update
                    // ,update_ts
                    // [e] add by gwj for opt_update
            );

        //printf("sql[%s]\n", query_sql);
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }

        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );

        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        // [s] add by gwj for multiple channel package
        if ( 24 != num_col )
        // [e] add by gwj for multiple channel package
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        printf("num_row is %d \n",num_row);

		bson key[1];
        bson_init( key );
        bson_append_int( key, "file_md5", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

		bson_init( key );
        bson_append_int( key, "package_name", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

		bson_init( key );
        bson_append_int( key, "apk_md5", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

		bson_init( key );
        bson_append_int( key, "string_version", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

		bson_init( key );
        bson_append_int( key, "int_version", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        bson_init( key );
        bson_append_int( key, "channel_id", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        bson_init( key );
        bson_append_int( key, "channel_info", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        bson_init( key );
        bson_append_int( key, "c_id", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        bson_init( key );
        bson_append_int( key, "c_id", 1 );
        bson_append_int( key, "adapt_info", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        bson_init( key );
        bson_append_int( key, "adapt_info", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );

        for (f1 = 0; f1 < num_row; f1++)
        {
            // [s] add by gwj for multiple channel package
            mysql_row = mysql_fetch_row(mysql_result);
            if(mysql_row[0] == NULL)
            {
                printf("num_row %d:%d id NULL \n", f1, num_row);
                continue;
            }
            // [s] add by gwj for multiple channel package

                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );

                // [s] add by gwj for change channel to Baidu
                if(mysql_row[13] == NULL)
                {
                    bson_append_string( &b , "baidu_path_url" , "");
                }
                else
                {
                    bson_append_string( &b , "baidu_path_url" , mysql_row[13]);
                    //printf("baidu_path_url:%s\n", mysql_row[13]);
                }
                // [e] add by gwj for change channel to Baidu

                SET_BSON_VALUE_STRING_B(&b, mysql_row[1], "path_url")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[2], "path_type")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[3], "media_format_id")
                SET_BSON_VALUE_INT_B(&b, mysql_row[4], "size")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[5], "mtype")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[6], "charge_typeid")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[7], "c_id")
                SET_BSON_VALUE_INT_B(&b, mysql_row[8], "value")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[9], "modify_date")

//                bson_append_string( &b , "path_url" , mysql_row[1] );
//                bson_append_string( &b , "path_type" , mysql_row[2] );
//                bson_append_string( &b , "media_format_id" , mysql_row[3] );
//                bson_append_int( &b , "size" , atoi(mysql_row[4]) );
//                bson_append_string( &b , "mtype" , mysql_row[5] );
//                bson_append_string( &b , "charge_typeid" , mysql_row[6] );
//                bson_append_string( &b , "c_id" , mysql_row[7] );
//                bson_append_int( &b , "value", atoi(mysql_row[8]));
//                bson_append_string( &b , "modify_date" , mysql_row[9] );

                bson_append_start_array( &b, "adapt_info" );

                char* slist = "";
                if(mysql_row[10] != NULL)
                {
                    slist = mysql_row[10];
                }
                string stype_list(slist);

                char* s_idlist = "";
                if(mysql_row[11] != NULL)
                {
                    s_idlist = mysql_row[11];
                }
                string stype_id_list(s_idlist);

                //bson_append_string( &b , "package_name" , mysql_row[12] );
                //bson_append_string( &b , "version_code" , mysql_row[13] );
                //bson_append_string( &b , "version" , mysql_row[14] );

                string stype, stype_id;
                char index[8];
                int j = 0;
                size_t pos1, pos2;
                //printf(  "id[%s],stype:[%s],stype_id:[%s]\n", mysql_row[0],stype_list.c_str(), stype_id_list.c_str() );
                //printf( "[" );
                while( (pos1=stype_list.find(','))!=string::npos && (pos2=stype_id_list.find(','))!=string::npos )
                {
                        stype = stype_list.substr( 0, pos1 );
                        stype_id = stype_id_list.substr( 0, pos2 );
                        stype_list.erase( 0, pos1+1 );
                        stype_id_list.erase( 0, pos2+1 );
                        memset(index, 0x00, sizeof(index));
                        sprintf( index, "%d", j );
                        bson_append_start_object( &b, index );
                        bson_append_int( &b, "stype", atoi(stype.c_str()) );
                        bson_append_string( &b, "stype_id", stype_id.c_str() );
                        bson_append_finish_object( &b );
                        ++j;
                        //printf( "{stype:%s,stype_id:%s},", stype.c_str(), stype_id.c_str() );
                }
                memset(index, 0x00, sizeof(index));
                sprintf( index, "%d", j );
                bson_append_start_object( &b, index );
                bson_append_int( &b, "stype", atoi(stype_list.c_str()) );
                bson_append_string( &b, "stype_id", stype_id_list.c_str() );
                bson_append_finish_object( &b );
                //printf( "{stype:%s,stype_id:%s}", stype_list.c_str(), stype_id_list.c_str() );
                //printf( "]\n" );

                bson_append_finish_object( &b );

				/* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 beg*/
                bson_append_string( &b , "channel_info" , mysql_row[12] );
                /* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 end*/

                /* 20130613 zhengxie mongodb.mcp.mcp_content_data新增channel_id字段 beg*/
                if(NULL == mysql_row[14]){
                    bson_append_string( &b , "channel_id" , "" );
                }else{
                    bson_append_string(&b, "channel_id", mysql_row[14] );
                }
                /* 20130613 zhengxie mongodb.mcp.mcp_content_data新增channel_id字段 end*/

                // [s] add by gwj for multiple channel package
                SET_BSON_VALUE_STRING_B(&b, mysql_row[15], "package_name")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[16], "int_version")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[17], "string_version")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[18], "sdk_version")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[19], "permissions")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[20], "features")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[21], "apk_info")
                SET_BSON_VALUE_STRING_B(&b, mysql_row[22], "apk_md5")
				//add by zhengxie for 增量更新 file_md5唯一确定一个包
                SET_BSON_VALUE_STRING_B(&b, mysql_row[23], "file_md5")
                // [e] add by gwj for multiple channel package

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp", "mcp_content_data", "mcp_content_data_bak");
        // [e] add by gwj for rename collection name

        return 0;
}

int set_mcp_content_data_kjava()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_data");
        sprintf(ns,"mcp.mcp_content_data");

//        sprintf(query_sql,
//                        "SELECT D.`id`, D.`path_url`, D.`path_type`, D.`media_format_id`, "
//                        "D.`size`, MF.`mtype`, CC.`charge_typeid`, D.`c_id`,CC.`value`, "
//                        "D.`modify_date`, GROUP_CONCAT(CF.`stype`), GROUP_CONCAT(CF.`stype_id`) "
//                        "FROM mcp_media_format AS MF "
//                        "INNER JOIN ( mcp_content_data AS D "
//                        "INNER JOIN mcp_content_format AS CF ON CF.`cdata_id` = D.`id` AND CF.`visible` = 1 AND "
//                        "CF.`enable` = 1 "
//                        "INNER JOIN  mcp_content_charge AS CC ON CC.`cdata_id` = D.`id`  )"
//                        "ON D.`media_format_id` = MF.`id` AND D.`enable` = 1 and D.`visible` = 1  and D.`channel_info` = 'DuoKu'"
//                        "GROUP BY D.`id` "
//               );
		/* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 beg*/
		sprintf(query_sql,
                        "SELECT D.`id`, D.`path_url`, D.`path_type`, D.`media_format_id`, "
                        "D.`size`, MF.`mtype`, CC.`charge_typeid`, D.`c_id`, CC.`value`, "
                        "D.`modify_date`, GROUP_CONCAT(CF.`stype`), GROUP_CONCAT(CF.`stype_id`), "
                        "D.`channel_info` "

		                // [s] add by gwj for change channel to Baidu
                        ", DTBD.`to_baidu_url` "
                        // [e] add by gwj for change channel to Baidu

        /* 20130613 zhengxie mongodb.mcp.mcp_content_data新增channel_id字段 beg*/
                        ", D.`channel_id` "
        /* 20130613 zhengxie mongodb.mcp.mcp_content_data新增channel_id字段 end*/

                        "FROM mcp_media_format AS MF "
                        "INNER JOIN ( mcp_content_data AS D "
                        "INNER JOIN mcp_content_format AS CF ON CF.`cdata_id` = D.`id` AND CF.`visible` = 1  AND "
                        "CF.`enable` = 1 "
                        "INNER JOIN  mcp_content_charge AS CC ON CC.`cdata_id` = D.`id`  "

                        // [s] add by gwj for change channel to Baidu
                        "LEFT JOIN `MCP`.`mcp_duoku_to_baidu_url` AS DTBD ON DTBD.`cdata_id` = D.`id` "
                        // [e] add by gwj for change channel to Baidu

                        ")"
                        /* "ON D.`media_format_id` = MF.`id` AND D.`enable` = 1 and D.`visible` = 1 AND D.`channel_info` = 'Duoku' OR D.`channel_info` = 'TEMP' " */
                        "ON (D.`media_format_id` = MF.`id` AND D.`enable` = 1 and D.`visible` = 1 AND "
                        "MF.`mtype`!= 'apk' ) "
                        "GROUP BY D.`id` "
               );

        /* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 end*/

        //printf("sql[%s]\n", query_sql);
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
//        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        // [s] modify by gwj for change channel to Baidu
//        if ( 13 != num_col )
        if ( 15 != num_col )
        // [e] modify by gwj for change channel to Baidu
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        //printf("num_row is %d \n",num_row);

        for (f1 = 0; f1 < num_row; f1++)
        {
                if( f1==0 ){
                        bson key[1];
                        bson_init( key );
                        bson_append_int( key, "c_id", 1 );
                        bson_finish( key);
                        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
                        bson_destroy( key );

                        bson_init( key );
                        bson_append_int( key, "c_id", 1 );
                        bson_append_int( key, "adapt_info", 1 );
                        bson_finish( key);
                        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
                        bson_destroy( key );

                        bson_init( key );
                        bson_append_int( key, "adapt_info", 1 );
                        bson_finish( key);
                        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
                        bson_destroy( key );
                }
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );

                // [s] add by gwj for change channel to Baidu
                if(mysql_row[13] == NULL)
                {
                    bson_append_string( &b , "baidu_path_url" , "");
                }
                else
                {
                    bson_append_string( &b , "baidu_path_url" , mysql_row[13]);
                    //printf("baidu_path_url:%s\n", mysql_row[13]);
                }
                // [e] add by gwj for change channel to Baidu

                bson_append_string( &b , "path_url" , mysql_row[1] );
                bson_append_string( &b , "path_type" , mysql_row[2] );
                bson_append_string( &b , "media_format_id" , mysql_row[3] );
                bson_append_int( &b , "size" , atoi(mysql_row[4]) );
                bson_append_string( &b , "mtype" , mysql_row[5] );
                bson_append_string( &b , "charge_typeid" , mysql_row[6] );
                bson_append_string( &b , "c_id" , mysql_row[7] );
                bson_append_int( &b , "value", atoi(mysql_row[8]));
                bson_append_string( &b , "modify_date" , mysql_row[9] );


                bson_append_start_array( &b, "adapt_info" );

                string stype_list(mysql_row[10]);
                string stype_id_list(mysql_row[11]);

                //bson_append_string( &b , "package_name" , mysql_row[12] );
                //bson_append_string( &b , "version_code" , mysql_row[13] );
                //bson_append_string( &b , "version" , mysql_row[14] );

                string stype, stype_id;
                char index[8];
                int j = 0;
                size_t pos1, pos2;
                //printf(  "id[%s],stype:[%s],stype_id:[%s]\n", mysql_row[0],stype_list.c_str(), stype_id_list.c_str() );
                //printf( "[" );
                while( (pos1=stype_list.find(','))!=string::npos && (pos2=stype_id_list.find(','))!=string::npos )
                {
                        stype = stype_list.substr( 0, pos1 );
                        stype_id = stype_id_list.substr( 0, pos2 );
                        stype_list.erase( 0, pos1+1 );
                        stype_id_list.erase( 0, pos2+1 );
                        memset(index, 0x00, sizeof(index));
                        sprintf( index, "%d", j );
                        bson_append_start_object( &b, index );
                        bson_append_int( &b, "stype", atoi(stype.c_str()) );
                        bson_append_string( &b, "stype_id", stype_id.c_str() );
                        bson_append_finish_object( &b );
                        ++j;
                        //printf( "{stype:%s,stype_id:%s},", stype.c_str(), stype_id.c_str() );
                }
                memset(index, 0x00, sizeof(index));
                sprintf( index, "%d", j );
                bson_append_start_object( &b, index );
                bson_append_int( &b, "stype", atoi(stype_list.c_str()) );
                bson_append_string( &b, "stype_id", stype_id_list.c_str() );
                bson_append_finish_object( &b );
                //printf( "{stype:%s,stype_id:%s}", stype_list.c_str(), stype_id_list.c_str() );
                //printf( "]\n" );

                bson_append_finish_object( &b );

				/* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 beg*/
                bson_append_string( &b , "channel_info" , mysql_row[12] );
                /* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 end*/

                /* 20130613 zhengxie mongodb.mcp.mcp_content_data新增channel_id字段 beg*/
                if(NULL == mysql_row[14]){
                    bson_append_string( &b , "channel_id" , "" );
                }else{
                    bson_append_string(&b, "channel_id", mysql_row[14] );
                }
                /* 20130613 zhengxie mongodb.mcp.mcp_content_data新增channel_id字段 end*/

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        /*
           bson key[1];
           bson_init( key );
           bson_append_int( key, "c_id", 1 );
           bson_finish( key);
           mongo_create_index( mongodb_conn, ns, key, 0, NULL );
           bson_destroy( key );

           bson_init( key );
           bson_append_int( key, "c_id", 1 );
           bson_append_int( key, "stype_info", 1 );
           bson_finish( key);
           mongo_create_index( mongodb_conn, ns, key, 0, NULL );
           bson_destroy( key );

           bson_init( key );
           bson_append_int( key, "stype_info", 1 );
           bson_finish( key);
           mongo_create_index( mongodb_conn, ns, key, 0, NULL );
           bson_destroy( key );
           */

        return 0;
}

int set_mcp_content_diy_column()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_diy_column_bak");
        sprintf(ns,"mcp.mcp_content_diy_column_bak");
        sprintf(query_sql,
                        "select mcp_content_diy_column.id, mcp_content_diy_column.name, mcp_content_diy_column.type_id "
                        "from mcp_content_diy_column"
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 3 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );
                if( strcmp( mysql_row[2], "2" ) == 0 )
                {
                        bson_append_string( &b , "type_id" , "1" );
                }
                else
                {
                        bson_append_string( &b , "type_id" , mysql_row[2] );
                }

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_diy_column", "mcp_content_diy_column_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_column_list()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_column_list_bak");
        sprintf(ns,"mcp.mcp_content_column_list_bak");
        sprintf(query_sql,
                        "SELECT COL.column_id, COL.c_id, COL.priority "
                        "FROM `MCP`.`mcp_content_column_list` AS COL "
                        "INNER JOIN  `MCP`.`mcp_content` AS C "
                        "ON C.`id` = COL.`c_id` AND C.`enable` = 1 AND C.`visible` = 1 AND C.`status` = 1 "
                        "WHERE COL.`enable` = 1 "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 3 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                if ( 0  != check_cid( mysql_row[1] ))
                {
                        continue;
                }
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "column_id" , mysql_row[0] );
                bson_append_string( &b , "c_id" , mysql_row[1] );
                bson_append_int( &b , "priority" , atoi(mysql_row[2]) );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_column_list", "mcp_content_column_list_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_cp()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_cp_bak");
        sprintf(ns,"mcp.mcp_content_cp_bak");
        sprintf(query_sql,
                        "select mcp_content_cp.id, mcp_content_cp.name from mcp_content_cp"
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 2 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                          "mcp_content_cp", "mcp_content_cp_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_grant()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_grant_bak");
        sprintf(ns,"mcp.mcp_content_grant_bak");
        sprintf(query_sql,
                        "select mcp_content_grant.id, mcp_content_grant.grant_date, "
                        "mcp_content_grant.sign_enddate, mcp_content_grant.modify_date from mcp_content_grant "
               );

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 4 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_time_t( &b , "grant_date" , date_to_int(mysql_row[1]) );
                bson_append_time_t( &b , "sign_enddate" , date_to_int(mysql_row[2]) );
                bson_append_time_t( &b , "modify_date" , date_to_int(mysql_row[3]) );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_grant", "mcp_content_grant_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_preview()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_preview_bak");
        sprintf(ns,"mcp.mcp_content_preview_bak");
        sprintf(query_sql,
                        "select mcp_content_preview.id, mcp_content_preview.image_type_id, "
                        "mcp_content_preview.image_url,mcp_content_preview.c_id,mcp_content_preview.priority from mcp_content_preview "
                        "where mcp_content_preview.enable= 1 "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 5 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "image_type_id" , mysql_row[1] );
                bson_append_string( &b , "image_url" , mysql_row[2] );
                bson_append_string( &b , "c_id" , mysql_row[3] );
                bson_append_int( &b , "priority" , atoi(mysql_row[4]) );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_preview", "mcp_content_preview_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_rank()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_rank_bak");
        sprintf(ns,"mcp.mcp_content_rank_bak");
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        for( int i=1; i<=5; ++i )
        {
                sprintf(query_sql,
                                "SELECT C.id, GROUP_CONCAT( DISTINCT CL.class_id ORDER BY CL.class_id ), R.ptype_id, TB.`id`, R.priority "
                                "FROM `MCP`.`mcp_content` AS C "
                                "INNER JOIN (`MCP`.`mcp_content_type` AS CT "
                                "INNER JOIN `MCP`.`mcp_content_type_base` AS TB ON CT.`type_base_id` = TB.`id`) "
                                "ON C.`type_id` = CT.`id` "
                                "INNER JOIN `MCP`.`mcp_content_class_list` AS CL ON C.`id` = CL.`c_id` AND CL.`enable` = 1 "
                                "LEFT JOIN  `MCP`.`mcp_content_rank` AS R ON C.`id` = R.`c_id` AND R.ptype_id =%d "
                                "WHERE R.ptype_id =%d OR R.ptype_id IS NULL "
                                "GROUP BY C.id", i, i
                       );

                if (mysql_query(&mysql_conn, query_sql) != 0)
                {
                        printf("query is not ok\n");
                        return -1;
                }
                mysql_result = mysql_store_result(&mysql_conn);
                num_row = mysql_num_rows(mysql_result);
                num_col = mysql_num_fields(mysql_result);
                if ( 5 != num_col )
                {
                        printf(" colum count %d is err\n",num_col);
                        mysql_free_result(mysql_result);
                        return -1;
                }

                for (f1 = 0; f1 < num_row; f1++)
                {
                        mysql_row = mysql_fetch_row(mysql_result);
                        if ( 0  != check_cid( mysql_row[0] ))
                        {
                                continue;
                        }
                        bson_init( &b );

                        bson_append_new_oid( &b, "_id" );
                        bson_append_string( &b , "c_id" , mysql_row[0] );
                        //bson_append_string( &b , "class_id" , mysql_row[1] );

                        bson_append_start_array( &b , "class_id" );
                        string class_list(mysql_row[1]);
                        string class_id;
                        char index[8];
                        int j = 0;
                        size_t pos;
                        while( (pos = class_list.find(',')) != string::npos )
                        {
                                class_id = class_list.substr( 0, pos );
                                class_list.erase( 0, pos+1 );
                                memset(index, 0x00, sizeof(index));
                                sprintf( index, "%d", j );
                                bson_append_string( &b, index, class_id.c_str() );
                                ++j;
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf( index, "%d", j );
                        bson_append_string( &b, index, class_list.c_str() );
                        bson_append_finish_object( &b );

                        bson_append_int( &b , "ptype_id" , i );
                        bson_append_int( &b , "type_base_id" , atoi(mysql_row[3]) );

                        if( mysql_row[4] == NULL || strcmp( mysql_row[4], "" ) == 0 )
                        {
                                bson_append_int( &b , "priority" , 0 );
                        }
                        else
                        {
                                bson_append_int( &b , "priority" , atoi(mysql_row[4]) );
                        }

                        bson_finish( &b );
                        mongo_insert( mongodb_conn , ns , &b, NULL );
                        bson_destroy( &b );
                        //         printf(" one row insert to mongodb ok\n");
                }
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_rank", "mcp_content_rank_bak");
        // [e] add by gwj for rename collection name
        return 0;
}



int set_mcp_phone_info()
{
        MYSQL_RES *mysql_result;
        MYSQL_RES *mysql_result2;
        MYSQL_ROW mysql_row;
        MYSQL_ROW mysql_row2;
        int f1, f2, num_row, num_row2 ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        char query_sql2[1024];
        memset(query_sql2, 0x00, sizeof(query_sql2));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        char index[4];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));
        memset(index, 0x00,sizeof(index));

        sprintf(col,"mcp_phone_info_bak");
        sprintf(ns,"mcp.mcp_phone_info_bak");
        sprintf(query_sql,
                        "SELECT mcp_phone_info.id, mcp_phone_info.plat_id, mcp_phone_info.res_id "
                        "FROM mcp_phone_info where mcp_phone_info.status = 1 "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 3 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "plat_id" , mysql_row[1] );
                bson_append_string( &b , "res_id" , mysql_row[2] );
                //         bson_append_string( &b , "sid" , mysql_row[3] );

                sprintf(query_sql2,
                                "select mcp_phone_series_list.sid from mcp_phone_series_list "
                                "INNER JOIN mcp_phone_series ON mcp_phone_series.id = mcp_phone_series_list.sid "
                                "where mcp_phone_series_list.pid = %s AND mcp_phone_series.enable = 1", mysql_row[0] );
                if (mysql_query(&mysql_conn, query_sql2) != 0)
                {
                        printf("query is not ok\n");
                        return -1;
                }
                mysql_result2 = mysql_store_result(&mysql_conn);
                num_row2 = mysql_num_rows(mysql_result2);

                bson_append_start_array(  &b , "sid" );
                for( f2 =0 ; f2 < num_row2; f2++)
                {
                        mysql_row2 = mysql_fetch_row(mysql_result2);
                        sprintf(index,"%d",f2);
                        bson_append_string( &b, index, mysql_row2[0]);
                }
                bson_append_finish_object( &b );
                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                mysql_free_result(mysql_result2);
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_phone_info", "mcp_phone_info_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


time_t date_to_int(char *date)
{
        char year[5];
        char month[3];
        char day[3];
        char hour[3];
        char minte[3];
        char sec[3];
        char* p;
        struct tm t;
        //    tm *it;
        time_t timep;
        p = date;

        memset(year, 0x00, sizeof(year));
        memset(month, 0x00, sizeof(month));
        memset(day, 0x00, sizeof(day));
        memset(hour, 0x00, sizeof(hour));
        memset(minte, 0x00, sizeof(minte));
        memset(sec, 0x00, sizeof(sec));

        strncpy(year, date, 4);
        p= p+5;
        strncpy(month, p, 2);
        p = p+3;
        strncpy(day, p, 2);
        p = p+3;
        strncpy(hour, p , 2);
        p = p+3;
        strncpy(minte, p , 2);
        p = p+3;
        strncpy(sec, p , 2);

        t.tm_sec = atoi(sec);
        t.tm_min = atoi(minte) ;
        t.tm_hour = atoi(hour) + 8 ;
        t.tm_mday= atoi(day);
        t.tm_mon = atoi(month) -1 ;
        t.tm_year = atoi(year) - 1900;

        timep = mktime(&t);
        //    printf("date is %s, timep is %ld\n",date , timep);

        //    it = localtime(&timep);

        //    printf("%s\n", date);
        //    printf("%s-%s-%s %s:%s:%s\n", year, month, day, hour, minte, sec);
        //    printf("year is %d, month is %d, day is %d,hour is %d, min is %d, sec is %d\n",
        //            t.tm_year, t.tm_mon +1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

        return ( timep);
}


int set_mcp_theme_content()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1, num_row  ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col ;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));
        char index[4];
        memset(index, 0x00,sizeof(index));

        sprintf(col,"mcp_content");
        sprintf(ns,"mcp.mcp_content");

        // [s] add by gwj for opt_update
//        const char* update_ts = TimeStampProcessor::instance().get_last_update_ts();
        // [e] add by gwj for opt_update

        sprintf(query_sql,
                        "SELECT C.`id`, C.`name`, C.`type_id`, TE.`gdate`, C.`ext_id`, TE.`info`, TE.`network_id`, "
                        "TE.`cp_id`, TE.`grant_id`, TE.`star`, C.`charge_typeid`, C.`charge_value`, C.`modify_date`,  "
                        "GROUP_CONCAT( DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`class_id` ), "
                        "GROUP_CONCAT( R.`ptype_id` ORDER BY R.`ptype_id` ), "
                        "GROUP_CONCAT( R.`priority` ORDER BY R.`ptype_id` ), "
                        "GROUP_CONCAT( DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
                        "GROUP_CONCAT( DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`column_id` ), "
                        "GROUP_CONCAT( DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ) "
                        "FROM `MCP`.`mcp_content` AS C "
                        "INNER JOIN  `MCP`.`mcp_content_theme_ext` AS TE ON TE.`id` = C.`ext_id` "
                        "AND TE.`enable` = 1 AND TE.`status` = 1 AND C.`enable` = 1 AND C.`status` = 1 AND C.`type_id` = 5 "
                        "INNER JOIN `MCP`.`mcp_content_class_list` AS CL  ON CL.`c_id` = C.`id` AND CL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` "
                        "LEFT JOIN `MCP`.`mcp_content_data` AS D ON C.`id` = D.`c_id` "
                        "WHERE D.`channel_info` = 'DuoKu' "

                        // [s] add by gwj for opt_update
//                        "AND TE.gdate > \"%s\" "
                        // [e] add by gwj for opt_update

                        "GROUP BY C.`id` "
                        "ORDER BY TE.`gdate` DESC "

                // [s] add by gwj for opt_update
//                      ,update_ts
                // [e] add by gwj for opt_update

                        );

        //             printf("connection is ok\n");
        int res=mysql_query(&mysql_conn, query_sql);
        if (res != 0)
        {
                printf("query is not ok, res[%d], [%s:%d]\n", res,__FILE__, __LINE__ );
                printf("%s\n", mysql_error(&mysql_conn));
                printf( "query_sql:[%s]\n", query_sql );
                return -1;
        }
        //mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 19 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        redisContext *c;
        c=redisConnPool::getInstance()->getConnection();
        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );
                bson_append_string( &b , "content_type" , mysql_row[2] );

                string class_list(mysql_row[13]);
                bson_append_class_info(&b,class_list);
                char index[8];
                int j = 0;
                size_t pos;

                bson_append_time_t( &b , "logdate" , date_to_int(mysql_row[3]) );
                bson_append_string( &b , "ext_id" , mysql_row[4] );
                bson_append_string( &b , "info" , mysql_row[5] );
                bson_append_int( &b , "network_id" , atoi(mysql_row[6]) );
                bson_append_string( &b , "cp_id" , mysql_row[7] );
                bson_append_string( &b , "grant_id" , mysql_row[8] );

				/* 修改星级逻辑 */
                /* bson_append_int( &b , "star" , atoi(mysql_row[9]) ); */
				//double star = get_star_by_c_id(mysql_row[0]);
                double star = getStarFromRedis(c, mysql_row[0]);
                bson_append_double( &b , "star" , star );

                bson_append_string( &b , "default_charge_typeid" , mysql_row[10] );
                bson_append_string( &b , "default_charge_value" , mysql_row[11] );
                bson_append_string( &b , "modify_date" , mysql_row[12] );

                string ptype_list, priority_list;
                if( mysql_row[14] != NULL &&  mysql_row[15] != NULL )
                {
                        ptype_list = mysql_row[14];
                        priority_list = mysql_row[15];
                }
                string ptype;
                int priority;
                int tmp;
                j = 1;
                size_t pos1(0), pos2(0);
                //printf( "id[%s], ptype_list:[%s], priority_list[%s]\n", mysql_row[0], ptype_list.c_str(), priority_list.c_str() );
                while( !ptype_list.empty() && !priority_list.empty() && j<=5 )
                {
                        pos1=ptype_list.find(',');
                        pos2=priority_list.find(',');
                        ptype = ptype_list.substr( 0, pos1 );
                        ptype_list.erase( 0, pos1+1 );
                        priority = atoi(priority_list.substr(0,pos2).c_str());
                        priority_list.erase( 0, pos2+1 );

                        tmp = atoi(ptype.c_str());
                        if( tmp < j )
                        {
                                continue;
                        }
                        else if( tmp == j )
                        {
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( &b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        else
                        {
                                while(j<tmp)
                                {
                                        memset( index, 0x00, sizeof(index) );
                                        sprintf( index, "ptype_%d", j );
                                        bson_append_int( &b, index, 0 );
                                        //   printf( "%s:0 ", index );
                                        ++j;
                                }
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( &b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }

                        ++j;
                }
                while( j<= 5 )
                {
                        memset( index, 0x00, sizeof(index) );
                        sprintf( index, "ptype_%d", j );
                        //printf( "%s:0 ", index );
                        bson_append_int( &b, index, 0 );
                        ++j;
                }
                // printf( "\n" );


                string label_list;
                if( mysql_row[16] != NULL )
                {
                        label_list = mysql_row[16];
                }

                bson_append_start_array( &b , "label_id" );
                j = 0;
                string label_id;
                while( !label_list.empty() )
                {
                        pos = label_list.find(',');
                        label_id = label_list.substr( 0, pos );
                        label_list.erase( 0, pos+1 );
                        memset(index, 0x00, sizeof(index));
                        sprintf( index, "%d", j );
                        bson_append_string( &b, index, label_id.c_str() );
                        ++j;
                }
                bson_append_finish_object( &b );

                string column_list;
                if( mysql_row[17] != NULL )
                {
                        column_list = mysql_row[17];
                }
                bson_append_column_info(&b,column_list);

                string img_info_list;
                if( mysql_row[18] != NULL  )
                {
                        img_info_list = mysql_row[18];
                }
                bson_append_start_array( &b , "img_info" );
                j = 0;
                string img_type, img_url;
                //printf( "img_info_list:[%s]\n", img_info_list.c_str() );
                while( !img_info_list.empty() )
                {
                        pos1 = img_info_list.find(',');
                        img_type = img_info_list.substr( 0, pos1 );
                        if( pos1 != string::npos )
                        {
                                img_info_list.erase( 0, pos1+1 );
                        }
                        else
                        {
                                img_info_list.clear();
                                break;
                        }
                        pos2 = img_info_list.find(',');
                        img_url = img_info_list.substr( 0, pos2 );
                        if( pos2 !=  string::npos )
                        {
                                img_info_list.erase( 0, pos2+1 );
                        }
                        else
                        {
                                img_info_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf( index, "%d", j );
                        bson_append_start_object( &b, index );
                        bson_append_string( &b, "img_type", img_type.c_str() );
                        bson_append_string( &b, "img_url", img_url.c_str() );
                        bson_append_finish_object( &b );
                        //printf( "{img_type:%s, img_url:%s} ", img_type.c_str(), img_url.c_str() );
                        ++j;
                }
                bson_append_finish_object( &b );
                //printf( "\n" );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);
        return 0;
}


int set_mcp_type_list()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_type_list_bak");
        sprintf(ns,"mcp.mcp_type_list_bak");
        sprintf(query_sql,
                        "select mcp_content.id, mcp_content.type_id "
                        "from mcp_content"
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 2 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "type_id" , mysql_row[1] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_type_list", "mcp_type_list_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_chargetype()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_chargetype_bak");
        sprintf(ns,"mcp.mcp_content_chargetype_bak");
        sprintf(query_sql,
                        "select mcp_content_chargetype.id, mcp_content_chargetype.name "
                        "from mcp_content_chargetype"
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 2 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_chargetype", "mcp_content_chargetype_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_class_list()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_class_list_bak");
        sprintf(ns,"mcp.mcp_content_class_list_bak");
        sprintf(query_sql,
                        "SELECT CL.`c_id`, CL.`class_id`, CL.`priority`,CL.`id` "
                        "FROM `MCP`.`mcp_content_class_list` AS CL "
                        "INNER JOIN `MCP`.`mcp_content` AS C "
                        "ON C.`id` = CL.`c_id` AND C.`visible` = 1 AND C.`enable` = 1 AND C.`status` = 1 "
                        "WHERE CL.`enable` = 1 "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 4 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                //printf( "c_id[%s],class_id[%s],priority[%s],enable[%s]\n", mysql_row[0],mysql_row[1],mysql_row[2],mysql_row[3] );
                if ( 0  != check_cid( mysql_row[0] ))
                {
                        continue;
                }
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "c_id" , mysql_row[0] );
                bson_append_string( &b , "class_id" , mysql_row[1] );
                bson_append_int( &b , "priority" , atoi(mysql_row[2]) );
                bson_append_int( &b , "id" , atoi(mysql_row[3]) );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_class_list", "mcp_content_class_list_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_url_content()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_url_content_bak");
        sprintf(ns,"mcp.mcp_url_content_bak");
        sprintf(query_sql,
                        //             "select mcp_content.id, mcp_content.name, mcp_content_class_list.class_id, "
                        "select mcp_content.id, mcp_content.name,  "
                        "mcp_content.ext_id, mcp_content_url_ext.ad, mcp_content_url_ext.url, "
                        "mcp_content_url_ext.baiduurl, mcp_content_url_ext.mark, "
                        "mcp_content_url_ext.logdate, mcp_content_url_ext.modify_date "
                        "FROM  mcp_content_url_ext "
                        "INNER JOIN   mcp_content "
                        "ON mcp_content_url_ext.id = mcp_content.ext_id "
                        "LEFT JOIN `MCP`.`mcp_content_data` AS D ON mcp_content.`id` = D.`c_id` AND D.`channel_info`='DuoKu' "
                        "WHERE mcp_content_url_ext.enable = 1 "
                        "AND mcp_content.enable = 1 "
                        "AND mcp_content.status = 1 "
                        "AND mcp_content.type_id = 6 "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 9 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        printf("url content num_row is %d\n", num_row);
        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );
                //         bson_append_string( &b , "class_id" , mysql_row[2] );
                bson_append_string( &b , "ext_id" , mysql_row[2] );
                bson_append_string( &b , "ad" , mysql_row[3] );
                bson_append_string( &b , "url" , mysql_row[4] );
                bson_append_string( &b , "baiduurl" , mysql_row[5] );
                bson_append_string( &b , "mark" , mysql_row[6] );
                bson_append_string( &b , "logdate" , mysql_row[7] );
                bson_append_string( &b , "modify_date" , mysql_row[8] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                //printf("mongo_insert res is %d\n", res);
                if( f1%1000 == 0 )
                {
                        usleep(10);
                }
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        printf("url content insert num is %d\n", f1);
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_url_content", "mcp_url_content_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int check_cid(char c_id[20])
{
        MYSQL_RES *mysql_result;
        MYSQL_RES *mysql_result2;
        MYSQL_ROW mysql_row;
        int  num_row=0 ;
        char query_sql[1024];
        char data_cid[20];
        memset(query_sql, 0x00, sizeof(query_sql));
        memset(data_cid, 0x00, sizeof(data_cid));

        sprintf(query_sql,
                        "select mcp_content.type_id from mcp_content where mcp_content.id = \"%s\"",
                        c_id);

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        if(mysql_result!=NULL){
                mysql_row = mysql_fetch_row(mysql_result);
        }else{
                //printf( "!!!! c_id:%s have no type !!!!\n", c_id );
                return -1;
        }
        if(mysql_row!=NULL&&mysql_row[0]!=NULL){
                sprintf(data_cid, mysql_row[0]);
        }else{
                //printf( "!!!! c_id:%s have no type !!!!\n", c_id );
                return -1;
        }
        mysql_free_result(mysql_result);
        //content is game type
        //    printf("cid %s type is %s\n", c_id, data_cid );
        if ( 0==strcmp( data_cid,"1") || 0 == strcmp( data_cid,"2") || 0==strcmp( data_cid,"11") || 0==strcmp( data_cid,"14") )
        {
                sprintf(query_sql, "select mcp_content_cp.id FROM mcp_content_cp "
                                "INNER JOIN ( mcp_content_game_ext "
                                "INNER JOIN (  mcp_content "
                                "INNER JOIN  mcp_content_class_list ON mcp_content_class_list.c_id = "
                                "mcp_content.id ) "
                                "ON mcp_content_game_ext.id = mcp_content.ext_id )"
                                "ON mcp_content_cp.id = mcp_content_game_ext.cp_id "
                                "WHERE mcp_content_game_ext.enable = 1 "
                                "AND mcp_content_game_ext.status = 1 "
                                "AND mcp_content.enable = 1 "
                                "AND mcp_content.status = 1 "
                                "AND mcp_content_cp.enable = 1 "
                                "AND mcp_content.id = \"%s\"", c_id);
                if (mysql_query(&mysql_conn, query_sql) != 0)
                {
                        printf("query is not ok\n");
                        return -1;
                }
                mysql_result2 = mysql_store_result(&mysql_conn);
                num_row = mysql_num_rows(mysql_result2);
                if(0 == num_row )
                {
                        //            printf("c_id %s is not ok \n", c_id);
                        mysql_free_result(mysql_result2);
                        return  -1;
                }
                else
                {
                        //            printf("c_id %s is  ok \n", c_id);
                        mysql_free_result(mysql_result2);
                        return 0;
                }
        }
        // theme
        else if ( (0 == strcmp( data_cid,"5")) )
        {
                sprintf(query_sql, "select mcp_content_theme_ext.id "
                                "FROM  mcp_content_theme_ext "
                                "INNER JOIN   mcp_content "
                                "ON mcp_content_theme_ext.id = mcp_content.ext_id "
                                "WHERE mcp_content_theme_ext.enable = 1 "
                                "AND mcp_content_theme_ext.status = 1 "
                                "AND mcp_content.enable = 1 "
                                "AND mcp_content.status = 1 "
                                "AND mcp_content.id = \"%s\"", c_id);
                if (mysql_query(&mysql_conn, query_sql) != 0)
                {
                        printf("query is not ok\n");
                        return -1;
                }
                mysql_result2 = mysql_store_result(&mysql_conn);
                num_row = mysql_num_rows(mysql_result2);
                if(0 == num_row )
                {
                        //             printf("c_id %s is not ok \n", c_id);
                        mysql_free_result(mysql_result2);
                        return  -1;
                }
                else
                {
                        //            printf("c_id %s is ok \n", c_id);
                        mysql_free_result(mysql_result2);
                        return 0;
                }

        }
        // URL
        else if ( (0 == strcmp( data_cid,"6")) )
        {
                sprintf(query_sql, "select mcp_content_url_ext.id "
                                "FROM  mcp_content_url_ext "
                                "INNER JOIN   mcp_content "
                                "ON mcp_content_url_ext.id = mcp_content.ext_id "
                                "WHERE mcp_content_url_ext.enable = 1 "
                                "AND mcp_content.enable = 1 "
                                "AND mcp_content.status = 1 "
                                "AND mcp_content.id = \"%s\"", c_id);
                if (mysql_query(&mysql_conn, query_sql) != 0)
                {
                        printf("query is not ok\n");
                        return -1;
                }
                mysql_result2 = mysql_store_result(&mysql_conn);
                num_row = mysql_num_rows(mysql_result2);
                if(0 == num_row )
                {
                        //                printf("c_id %s is not ok \n", c_id);
                        mysql_free_result(mysql_result2);
                        return  -1;
                }
                else
                {
                        //                printf("c_id %s is ok \n", c_id);
                        mysql_free_result(mysql_result2);
                        return 0;
                }
        }
        return 0;
}


int set_mcp_content_bookmark()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_bookmark_bak");
        sprintf(ns,"mcp.mcp_content_bookmark_bak");
        sprintf(query_sql,
                        "select mcp_content_bookmark.id, mcp_content_bookmark.name, "
                        "mcp_content_bookmark.type_id, mcp_content_bookmark.logdate "
                        "from mcp_content_bookmark "
                        "where mcp_content_bookmark.enable=1 "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 4 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );
                bson_append_int( &b , "type_id" , atoi(mysql_row[2]) );
                bson_append_string( &b , "logdate" , mysql_row[3] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_bookmark", "mcp_content_bookmark_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_content_bookmark_list()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_bookmark_list_bak");
        sprintf(ns,"mcp.mcp_content_bookmark_list_bak");
        sprintf(query_sql,
                        "select mcp_content_bookmark_list.c_id, mcp_content_bookmark_list.cb_id, "
                        "mcp_content_bookmark_list.priority, mcp_content_bookmark_list.logdate ,mcp_content_bookmark_list.id "
                        "from mcp_content_bookmark_list "
                        "where mcp_content_bookmark_list.enable=1 "
               );

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 5 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                if ( 0  != check_cid( mysql_row[0] ))
                {
                        continue;
                }

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "c_id" , mysql_row[0] );
                bson_append_string( &b , "cb_id" , mysql_row[1] );
                bson_append_int( &b , "priority" , atoi(mysql_row[2]) );
                bson_append_string( &b , "logdate" , mysql_row[3] );
                bson_append_string( &b , "id" , mysql_row[4] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_bookmark_list", "mcp_content_bookmark_list_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_class_adapt_count()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_class_adapt_count_bak");
        sprintf(ns,"mcp.mcp_class_adapt_count_bak");
        sprintf(query_sql,
                        "SELECT F.`stype`, F.`stype_id`, CL.`class_id`, count(distinct D.`c_id`) "
                        "FROM `MCP`.`mcp_content_format` AS F "
                        "INNER JOIN (`MCP`.`mcp_content_data` AS D "
                        "INNER JOIN `MCP`.`mcp_content_class_list` AS CL ON  D.`c_id` = CL.`c_id` AND CL.`enable` = 1 "
                        "INNER JOIN (`MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_game_ext` AS GE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP ON CP.`id` = GE.`cp_id` ) "
                        "ON GE.`id` = C.`ext_id` ) "
                        "ON D.`c_id` = C.`id` ) "
                        "ON D.`id` = F.`cdata_id` "
                        "WHERE F.`enable` = 1 AND C.`enable` = 1 AND C.`status` = 1 "
                        "AND D.`enable` = 1 AND D.`visible` = 1 "
                        "AND GE.`enable` = 1 AND GE.`status` = 1 AND CP.`enable` = 1 AND D.`channel_info` = 'DuoKu' "
                        "GROUP BY F.`stype`, F.`stype_id`, CL.`class_id` "
               );
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok, [%s:%d]\n", __FILE__, __LINE__ );
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 4 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_int( &b , "stype" , atoi(mysql_row[0]) );
                bson_append_string( &b , "stype_id" , mysql_row[1] );
                bson_append_string( &b , "class_id" , mysql_row[2] );
                bson_append_int( &b , "count" , atoi(mysql_row[3]) );

                //printf( "%s\t%s\t%s\t%s\n", mysql_row[0],mysql_row[1],mysql_row[2],mysql_row[3] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
        }

        memset(query_sql, 0x00, sizeof(query_sql));
        sprintf(query_sql,
                        "SELECT CL.`class_id`, count(*) "
                        "FROM `MCP`.`mcp_content_class_list` AS CL "
                        "INNER JOIN (`MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_game_ext` AS GE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP ON GE.`cp_id` = CP.`id` ) "
                        "ON C.`ext_id`=GE.`id` ) "
                        "ON CL.`c_id` = C.`id` "
                        "LEFT JOIN `MCP`.`mcp_content_data` AS D ON C.`id` = D.`c_id` AND D.`channel_info` = 'DuoKu' "
                        "WHERE C.`enable` = 1 AND C.`status` = 1 AND CL.`enable` = 1 "
                        "AND GE.`enable` = 1 AND GE.`status` = 1 AND CP.`enable` = 1 "
                        "GROUP BY CL.`class_id` " );

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok, [%s:%d]\n", __FILE__, __LINE__ );
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 2 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_int( &b , "stype" , 0 );
                bson_append_string( &b , "stype_id" , "0" );
                bson_append_string( &b , "class_id" , mysql_row[0] );
                bson_append_int( &b , "count" , atoi(mysql_row[1]) );

                //printf( "0\t0\t%s\t%s\n", mysql_row[0],mysql_row[1]);

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
        }

        memset(query_sql, 0x00, sizeof(query_sql));
        sprintf(query_sql,
                        "SELECT F.`stype`, F.`stype_id`,  count(distinct D.`c_id`) "
                        "FROM `MCP`.`mcp_content_format` AS F "
                        "INNER JOIN (`MCP`.`mcp_content_data` AS D "
                        "INNER JOIN (`MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_game_ext` AS GE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP "
                        "ON GE.`cp_id` = CP.`id`) "
                        "ON GE.`id` = C.`ext_id`) "
                        "ON C.`id` = D.`c_id`) "
                        "ON D.`id` = F.`cdata_id` "
                        "WHERE F.`enable` = 1 AND C.`enable` = 1 AND C.`status` = 1 "
                        "AND D.`enable` = 1 AND D.`visible` = 1 "
                        "AND GE.`enable` = 1 AND GE.`status` = 1 AND CP.`enable` = 1 "
                        "GROUP BY F.`stype`, F.`stype_id` " );

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok, [%s:%d]\n", __FILE__, __LINE__ );
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 3 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_int( &b , "stype" , atoi(mysql_row[0]) );
                bson_append_string( &b , "stype_id" , mysql_row[1] );
                bson_append_string( &b , "class_id" , "0" );
                bson_append_int( &b , "count" , atoi(mysql_row[2]) );

                //printf( "%s\t%s\t0\t%s\n", mysql_row[0],mysql_row[1],mysql_row[2]);

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
        }

        memset(query_sql, 0x00, sizeof(query_sql));
        sprintf( query_sql,
                        "SELECT count(C.`id`) FROM `mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_game_ext` AS GE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP "
                        "ON GE.`cp_id` = CP.`id` AND CP.`enable` = 1 ) "
                        "ON GE.`id` = C.`ext_id` AND GE.`enable` = 1 AND GE.`status` = 1 "
                        "WHERE C.`enable` = 1 AND C.`status` = 1 "
                        "AND (C.`type_id` = '1' OR  `type_id` = '2' OR `type_id` = '11' OR `type_id` = '14') " );

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok, [%s:%d]\n", __FILE__, __LINE__);
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 1 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_int( &b , "stype" , 0 );
                bson_append_string( &b , "stype_id" , "0" );
                bson_append_string( &b , "class_id" , "0" );
                bson_append_int( &b , "count" , atoi(mysql_row[0]) );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
        }


        memset(query_sql, 0x00, sizeof(query_sql));
        sprintf(query_sql,
                        "SELECT P.`id` , GROUP_CONCAT(distinct SL.`sid` ORDER BY SL.`sid`) "
                        "FROM `MCP`.`mcp_phone_info` AS P "
                        "INNER JOIN `MCP`.`mcp_phone_series_list` AS SL ON P.`id` = SL.`Pid` "
                        "INNER JOIN `MCP`.`mcp_phone_series` AS PS ON PS.`id` = SL.`sid` AND PS.`enable` = 1 "
                        "WHERE P.`status` =  1 AND P.`visible` = 1  "
                        "GROUP BY P.`id` " );

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok, [%s:%d]\n", __FILE__, __LINE__);
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 2 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        map<string,string> pid_series_map;
        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                pid_series_map[mysql_row[0]] = mysql_row[1];
                //printf( "map[%s]:[%s]\n", mysql_row[0],mysql_row[1] );
        }

        memset(query_sql, 0x00, sizeof(query_sql));
        sprintf(query_sql,
                        "SELECT P.`id`, GROUP_CONCAT(distinct SL.`sid` ORDER BY SL.`sid`), count(distinct D.`c_id`) "
                        "FROM `MCP`.`mcp_phone_info` AS P "
                        "INNER JOIN (`MCP`.`mcp_phone_series_list` AS SL "
                        "INNER JOIN  `MCP`.`mcp_phone_series` AS PS ON PS.`id` = SL.`sid` AND PS.`enable` = 1 ) "
                        "ON P.`id` = SL.`Pid` "
                        "INNER JOIN  `MCP`.`mcp_content_format` AS F ON SL.`sid` = F.`stype_id` AND F.`stype` = 2 AND SL.`enable` = 1 "
                        "INNER JOIN (`MCP`.`mcp_content_data` AS D "
                        "INNER JOIN (`MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_game_ext` AS GE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP "
                        "ON GE.`cp_id` = CP.`id`) "
                        "ON GE.`id` = C.`ext_id`) "
                        "ON C.`id` = D.`c_id`) "
                        "ON D.`id` = F.`cdata_id` AND D.`enable` = 1 AND D.`visible` = 1 "
                        "WHERE F.`enable` = 1 AND C.`enable` = 1 AND C.`status` = 1 "
                        "AND GE.`enable` = 1 AND GE.`status` = 1 AND CP.`enable` = 1 AND P.`status` =  1 AND P.`visible` = 1 "
                        "GROUP BY P.`id` "
                        "ORDER BY P.`id`, SL.`sid` ");

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok, [%s:%d]\n", __FILE__, __LINE__);
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 3 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_int( &b , "stype" , 2 );
                //bson_append_string( &b , "stype_id" , mysql_row[1] );
                bson_append_string( &b, "stype_id",  pid_series_map[mysql_row[0]].c_str() );
                bson_append_string( &b , "class_id" , "0" );
                bson_append_int( &b , "count" , atoi(mysql_row[2]) );

                //printf( "%s\t%s\t0\t%s\n", mysql_row[0],mysql_row[1],mysql_row[2]);
                //printf( "%s\t%s\t0\t%s\n", mysql_row[0],pid_series_map[mysql_row[0]].c_str(),mysql_row[2]);

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
        }

        memset(query_sql, 0x00, sizeof(query_sql));
        sprintf(query_sql,
                        "SELECT P.`id`, GROUP_CONCAT(distinct SL.`sid` ORDER BY SL.`sid`), CL.`class_id`, count(distinct D.`c_id`) "
                        "FROM `MCP`.`mcp_phone_info` AS P "
                        "LEFT JOIN  (`MCP`.`mcp_phone_series_list` AS SL "
                        "INNER JOIN `MCP`.`mcp_phone_series` AS PS ON PS.`id` = SL.`sid` AND PS.`enable` =1 ) "
                        "ON P.`id` = SL.`Pid` AND SL.`enable` = 1 "
                        "LEFT JOIN  (`MCP`.`mcp_content_format` AS F "
                        "INNER JOIN (`MCP`.`mcp_content_data` AS D "
                        "INNER JOIN `MCP`.`mcp_content_class_list` AS CL ON  D.`c_id` = CL.`c_id` AND CL.`enable` = 1 "
                        "INNER JOIN (`MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_game_ext` AS GE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP "
                        "ON GE.`cp_id` = CP.`id` AND CP.`enable` = 1 ) "
                        "ON GE.`id` = C.`ext_id` AND GE.`enable` = 1 AND GE.`status` = 1 ) "
                        "ON C.`id` = D.`c_id`  AND C.`enable` = 1 AND C.`status` = 1 ) "
                        "ON D.`id` = F.`cdata_id` AND D.`enable` = 1 AND D.`visible` = 1 ) "
                        "ON SL.`sid` = F.`stype_id` AND F.`stype` = 2 "
                        "WHERE P.`status` =  1 AND P.`visible` = 1 "
                        "GROUP BY P.`id`, CL.`class_id` "
                        "ORDER BY P.`id`, SL.`sid`" );

        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok, [%s:%d]\n", __FILE__, __LINE__);
                return -1;
        }
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 4 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);

                if( atoi(mysql_row[3]) == 0 )
                {
                        continue;
                }
                if( pid_series_map[mysql_row[0]].find( ',' ) == string::npos )
                {
                        continue;
                }

                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_int( &b , "stype" , 2 );
                bson_append_string( &b, "stype_id",  pid_series_map[mysql_row[0]].c_str() );
                if(  mysql_row[2] != NULL )
                {
                        bson_append_string( &b , "class_id" , mysql_row[2] );
                }
                else
                {
                        bson_append_string( &b , "class_id" , "0" );
                }
                bson_append_int( &b , "count" , atoi(mysql_row[3]) );

                //printf( "pid:[%s]\n",  mysql_row[0] );
                //printf( "2\t%s\t%s\t%s\n", mysql_row[1],mysql_row[2],mysql_row[3]);
                //printf( "2\t%s\t%s\t%s\n", pid_series_map[mysql_row[0]].c_str(),mysql_row[2],mysql_row[3]);

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
        }

        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_class_adapt_count", "mcp_class_adapt_count_bak");
        // [e] add by gwj for rename collection name

        return 0;
}

int set_mcp_software_content()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_software_content_bak");
        sprintf(ns,"mcp.mcp_software_content_bak");
        sprintf(query_sql,
                        "SELECT  C.`id`, C.`name`, C.`type_id`, SE.`info`, SE.`version`, SE.`cp_id`, SE.`grant_id`, "
                        "SE.`releasedate`, SE.`froms`, SE.`adapt`, SE.`pop`, D.`id`,D.`path_url`, D.`size`, D.`jump_url`, "
                        "GROUP_CONCAT(DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
                        "GROUP_CONCAT(DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`column_id` ), "
                        "GROUP_CONCAT(DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT R.`ptype_id`, \",\", R.`priority` ORDER BY R.`ptype_id` ), "
                        "SE.`star`, SE.`show_highspeed` "
                        "FROM `MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_software_ext` AS SE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP ON CP.`id` = SE.`cp_id` AND CP.`enable` = 1 ) "
                        "ON  SE.`id` = C.`ext_id` AND SE.`enable` = 1 AND C.`type_id` = 15 "
                        "INNER JOIN  `MCP`.`mcp_content_class_list` AS CL ON CL.c_id = C.id AND CL.enable = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_data` AS D ON D.`c_id` = C.`id` AND D.`enable` = 1 AND D.`visible` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` AND R.`type_base_id` = 0 "
                        "WHERE C.enable = 1 AND C.status = 1 AND C.visible = 1 AND D.`channel_info` = 'DuoKu' "
                        "GROUP BY C.id "
                        "ORDER BY SE.`releasedate` DESC "
                        );

        if (mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 22 != num_col ){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);
                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"name",mysql_row[1]);
                bson_append_string(b,"content_type",mysql_row[2]);
                bson_append_string(b,"info",mysql_row[3]);
                bson_append_string(b,"version",mysql_row[4]);
                bson_append_string(b,"cp_id",mysql_row[5]);
                bson_append_string(b,"grant_id",mysql_row[6]);
                bson_append_string(b,"releasedate",mysql_row[7]);
                bson_append_string(b,"froms",mysql_row[8]);
                bson_append_string(b,"adapt",mysql_row[9]);
                bson_append_string(b,"pop",mysql_row[10]);
                if(mysql_row[11]!=NULL){
                        bson_append_string(b,"data_id",mysql_row[11]);
                        bson_append_string(b,"path_url",mysql_row[12]);
                        bson_append_string(b,"size",mysql_row[13]);
                        bson_append_string(b,"jump_url",mysql_row[14]==NULL?"":mysql_row[14]);
                }
                else{
                        bson_append_string(b,"data_id","");
                        bson_append_string(b,"path_url","");
                        bson_append_string(b,"size","0");
                        bson_append_string(b,"jump_url","");
                }

                size_t pos;
                int j = 0;
                size_t pos1(0), pos2(0);
                char index[8];

                string class_list(mysql_row[15]);
                bson_append_class_info(b,class_list);

                string label_list, label_id;
                if( mysql_row[16] != NULL )
                {
                        label_list = mysql_row[16];
                }
                bson_append_start_array(b, "label_id");
                j = 0;
                while( !label_list.empty() ){
                        pos = label_list.find(',');
                        label_id = label_list.substr(0, pos);
                        //printf( "label_list:[%s], label_id:[%s]\n", label_list.c_str(), label_id.c_str() );
                        if( pos != string::npos ){
                                label_list.erase(0, pos+1);
                        }
                        else{
                                label_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf(index, "%d", j);
                        bson_append_string(b, index, label_id.c_str());
                        ++j;
                }
                bson_append_finish_object(b);

                string column_list;
                if(mysql_row[17] != NULL){
                        column_list = mysql_row[17];
                }
                bson_append_column_info(b,column_list);

                string img_info_list,img_type,img_url;
                if(mysql_row[18]!=NULL){
                        img_info_list = mysql_row[18];
                }
                bson_append_start_array( b , "img_info" );
                j = 0;
                while(!img_info_list.empty()){
                        pos1 = img_info_list.find(',');
                        img_type = img_info_list.substr( 0, pos1 );
                        if(pos1!=string::npos){
                                img_info_list.erase( 0, pos1+1 );
                        }
                        else{
                                img_info_list.clear();
                                break;
                        }
                        pos2 = img_info_list.find(',');
                        img_url = img_info_list.substr( 0, pos2 );
                        if(pos2!=string::npos){
                                img_info_list.erase( 0, pos2+1 );
                        }
                        else{
                                img_info_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf(index, "%d", j);
                        bson_append_start_object(b, index);
                        bson_append_string(b, "img_type", img_type.c_str());
                        bson_append_string(b, "img_url", img_url.c_str());
                        bson_append_finish_object(b);
                        //printf( "{img_type:%s, img_url:%s} ", img_type.c_str(), img_url.c_str() );
                        ++j;
                }
                bson_append_finish_object( b );

                string ptype_list;
                if(mysql_row[19]!=NULL){
                        ptype_list = mysql_row[19];
                }
                string ptype;
                int priority,tmp;
                j = 1;
//                printf( "id[%s], ptype_list:[%s]\n", mysql_row[0], ptype_list.c_str() );
                while( !ptype_list.empty() && j<=5 ){
                        pos1=ptype_list.find(',');
                        if(pos1 != string::npos){
                                ptype = ptype_list.substr( 0, pos1 );
                                ptype_list.erase( 0, pos1+1 );
                        }
                        else{
                                break;
                        }
                        pos2=ptype_list.find(',');
                        priority = atoi(ptype_list.substr(0,pos2).c_str());
                        ptype_list.erase( 0, pos2+1 );
                        tmp = atoi(ptype.c_str());
                        //if(tmp<j){
                        //        continue;
                        //}

                        sprintf(index,"ptype_%d", tmp);
                        bson_append_int(b,index,priority);
                        /*
                        else if(tmp==j){
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        else{
                                while(j<tmp){
                                        memset( index, 0x00, sizeof(index) );
                                        sprintf( index, "ptype_%d", j );
                                        bson_append_int( b, index, 0 );
                                        //   printf( "%s:0 ", index );
                                        ++j;
                                }
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        ++j;
                        */
                }
                /*
                while(j<=5){
                        memset( index, 0x00, sizeof(index) );
                        sprintf( index, "ptype_%d", j );
                        //printf( "%s:0 ", index );
                        bson_append_int( b, index, 0 );
                        ++j;
                }
                */
                // printf( "\n" );

                bson_append_int(b,"star",atoi(mysql_row[20]));
				SET_BSON_VALUE_STRING_B(b, mysql_row[21], "show_highspeed")
                //bson_append_int(b,"class_weight",atoi(mysql_row[21]));
                //bson_append_int(b,"latest_weight",atoi(mysql_row[22]));
                //bson_append_int(b,"download_weight",atoi(mysql_row[23]));
                //bson_append_int(b,"pop_weight",atoi(mysql_row[24]));
                //bson_append_time_t(b,"udate",date_to_int(mysql_row[25]));
                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_software_content", "mcp_software_content_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_content_topic()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_topic_bak");
        sprintf(ns,"mcp.mcp_content_topic_bak");
        sprintf(query_sql,
                        "SELECT T.`id`, T.`name`, T.`type_id`, T.`img_url`, T.`intro` "
                        "FROM `MCP`.`mcp_content_topic` AS T "
                        "WHERE T.`enable` = 1 " );

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if(5!=num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"name",mysql_row[1]);
                if(strcmp(mysql_row[2],"2")==0){
                        bson_append_string(b,"type_id","1");
                }
                else{
                        bson_append_string(b,"type_id",mysql_row[2]);
                }
                bson_append_string(b,"img_url",mysql_row[3]);
                bson_append_string(b,"intro",mysql_row[4]);

                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_topic", "mcp_content_topic_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_topic_col_map()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_topic_col_map_bak");
        sprintf(ns,"mcp.mcp_topic_col_map_bak");
        sprintf(query_sql,
                        "SELECT TM.`topic_id`, TM.`column_id`, TM.`priority` "
                        "FROM `MCP`.`mcp_content_topic_map` AS TM "
                        "INNER JOIN `MCP`.`mcp_content_topic` AS T ON T.`id` = TM.`topic_id` AND T.`enable` = 1 "
                        "INNER JOIN `MCP`.`mcp_content_diy_column` AS COL ON COL.`id` = TM.`column_id` AND COL.`enable` = 1 "
                        "WHERE TM.`enable` = 1 "
               );
        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if(3!=num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"topic",mysql_row[0]);
                bson_append_string(b,"column",mysql_row[1]);
                bson_append_int(b,"priority",atoi(mysql_row[2]));
                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_topic_col_map", "mcp_topic_col_map_bak");
        // [e] add by gwj for rename collection name
        return 0;
}


int set_mcp_class_class_map()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_class_class_map_bak");
        sprintf(ns,"mcp.mcp_class_class_map_bak");
        sprintf(query_sql,
                        "SELECT CM.`father_class_id`, CM.`child_class_id` "
                        "FROM `MCP`.`mcp_content_class_map` AS CM "
                        "INNER JOIN `MCP`.`mcp_content_class` AS C ON C.`id` = CM.`father_class_id` AND C.`enable` = 1 "
                        "INNER JOIN `MCP`.`mcp_content_class` AS C1  ON C1.`id` = CM.`child_class_id` AND C1.`enable` = 1 "
                        "WHERE CM.`enable` = 1 "
               );
        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok,sql[%s]\n",query_sql);
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if(2!=num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"father",mysql_row[0]);
                bson_append_string(b,"child",mysql_row[1]);
                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_class_class_map", "mcp_class_class_map_bak");
        // [e] add by gwj for rename collection name

        return 0;
}

int set_mcp_android_theme()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_android_theme_bak");
        sprintf(ns,"mcp.mcp_android_theme_bak");
        sprintf(query_sql,
                        "SELECT  C.`id`, C.`name`, C.`type_id`, ATE.`info`, ATE.`cp_id`, ATE.`grant_id`, "
                        "ATE.`releasedate`, ATE.`froms`, ATE.`adapt`, ATE.`pop`, D.`id`,D.`path_url`, D.`size`, D.`jump_url`, "
                        "GROUP_CONCAT(DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
                        "GROUP_CONCAT(DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`column_id` ), "
                        "GROUP_CONCAT(DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT R.`ptype_id`, \",\", R.`priority` ORDER BY R.`ptype_id` ) "
                        "FROM `MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_theme_android_ext` AS ATE "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP ON CP.`id` = ATE.`cp_id` AND CP.`enable` = 1 ) "
                        "ON  ATE.`id` = C.`ext_id` AND ATE.`enable` = 1 AND C.`type_id` = 16 "
                        "INNER JOIN  `MCP`.`mcp_content_class_list` AS CL ON CL.c_id = C.id AND CL.enable = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_data` AS D ON D.`c_id` = C.`id` AND D.`enable` = 1 AND D.`visible` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` "
                        "WHERE C.enable = 1 AND C.status = 1 AND C.visible = 1 AND D.`channel_info` = 'DuoKu' "
                        "GROUP BY C.id "
                        "ORDER BY ATE.`releasedate` DESC "
                        );


        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 19 != num_col ){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);
                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"name",mysql_row[1]);
                bson_append_string(b,"content_type",mysql_row[2]);
                bson_append_string(b,"info",mysql_row[3]);
                bson_append_string(b,"cp_id",mysql_row[4]);
                bson_append_string(b,"grant_id",mysql_row[5]);
                bson_append_string(b,"releasedate",mysql_row[6]);
                bson_append_string(b,"froms",mysql_row[7]);
                bson_append_string(b,"adapt",mysql_row[8]);
                bson_append_string(b,"pop",mysql_row[9]);
                if(mysql_row[10]!=NULL){
                        bson_append_string(b,"data_id",mysql_row[10]);
                        bson_append_string(b,"path_url",mysql_row[11]);
                        bson_append_string(b,"size",mysql_row[12]);
                        bson_append_string(b,"jump_url",mysql_row[13]==NULL?"":mysql_row[13]);
                }
                else{
                        bson_append_string(b,"data_id","");
                        bson_append_string(b,"path_url","");
                        bson_append_string(b,"size","0");
                        bson_append_string(b,"jump_url","");
                }

                char index[8];
                int j = 0;
                size_t pos;

                string class_list(mysql_row[14]);
                bson_append_class_info(b, class_list);

                string label_list, label_id;
                if( mysql_row[15] != NULL )
                {
                        label_list = mysql_row[15];
                }
                bson_append_start_array(b, "label_id");
                j = 0;
                while( !label_list.empty() ){
                        pos = label_list.find(',');
                        label_id = label_list.substr(0, pos);
                        //printf( "label_list:[%s], label_id:[%s]\n", label_list.c_str(), label_id.c_str() );
                        if( pos != string::npos ){
                                label_list.erase(0, pos+1);
                        }
                        else{
                                label_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf(index, "%d", j);
                        bson_append_string(b, index, label_id.c_str());
                        ++j;
                }
                bson_append_finish_object(b);

                string column_list;
                if(mysql_row[16] != NULL){
                        column_list = mysql_row[16];
                }
                bson_append_column_info(b,column_list);

                string img_info_list,img_type,img_url;
                size_t pos1(0), pos2(0);
                if(mysql_row[17]!=NULL){
                        img_info_list = mysql_row[17];
                }
                bson_append_start_array( b , "img_info" );
                j = 0;
                while(!img_info_list.empty()){
                        pos1 = img_info_list.find(',');
                        img_type = img_info_list.substr( 0, pos1 );
                        if(pos1!=string::npos){
                                img_info_list.erase( 0, pos1+1 );
                        }
                        else{
                                img_info_list.clear();
                                break;
                        }
                        pos2 = img_info_list.find(',');
                        img_url = img_info_list.substr( 0, pos2 );
                        if(pos2!=string::npos){
                                img_info_list.erase( 0, pos2+1 );
                        }
                        else{
                                img_info_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf(index, "%d", j);
                        bson_append_start_object(b, index);
                        bson_append_string(b, "img_type", img_type.c_str());
                        bson_append_string(b, "img_url", img_url.c_str());
                        bson_append_finish_object(b);
                        //printf( "{img_type:%s, img_url:%s} ", img_type.c_str(), img_url.c_str() );
                        ++j;
                }
                bson_append_finish_object( b );

                string ptype_list;
                if(mysql_row[18]!=NULL){
                        ptype_list = mysql_row[18];
                }
                string ptype;
                int priority,tmp;
                j = 1;
                //printf( "id[%s], ptype_list:[%s], priority_list[%s]\n", mysql_row[0], ptype_list.c_str(), priority_list.c_str() );
                while( !ptype_list.empty() && j<=5 ){
                        pos1=ptype_list.find(',');
                        if(pos1 != string::npos){
                                ptype = ptype_list.substr( 0, pos1 );
                                ptype_list.erase( 0, pos1+1 );
                        }
                        else{
                                break;
                        }
                        pos2=ptype_list.find(',');
                        priority = atoi(ptype_list.substr(0,pos2).c_str());
                        ptype_list.erase( 0, pos2+1 );
                        tmp = atoi(ptype.c_str());
                        if(tmp<j){
                                continue;
                        }
                        else if(tmp==j){
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        else{
                                while(j<tmp){
                                        memset( index, 0x00, sizeof(index) );
                                        sprintf( index, "ptype_%d", j );
                                        bson_append_int( b, index, 0 );
                                        //   printf( "%s:0 ", index );
                                        ++j;
                                }
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        ++j;
                }
                while(j<=5){
                        memset( index, 0x00, sizeof(index) );
                        sprintf( index, "ptype_%d", j );
                        //printf( "%s:0 ", index );
                        bson_append_int( b, index, 0 );
                        ++j;
                }
                // printf( "\n" );

                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_android_theme", "mcp_android_theme_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_ios_content()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_ios_content_bak");
        sprintf(ns,"mcp.mcp_ios_content_bak");
        sprintf(query_sql,
                        "SELECT  C.`id`, C.`name`, C.`type_id`, IOS.`itunes_id`, IOS.`info`, IOS.`version`, "
                        "IOS.`cp_id`, IOS.`star`, IOS.`gdate`, IOS.`lmtfree_date`, IOS.`system_req`,  "
                        "IOS.`history_count`, IOS.`total_score`, IOS.`current_count`, IOS.`new_score`, "
                        "D.`id`,D.`path_url`, D.`jump_url`, D.`size`, CC.`value`, IOS.`original_price`, IOS.`language`, "
                        "GROUP_CONCAT(DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
                        "GROUP_CONCAT(DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`column_id` ), "
                        "GROUP_CONCAT(DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT R.`ptype_id`, \",\", R.`priority` ORDER BY R.`ptype_id` ) "
                        "FROM `MCP`.`mcp_content` AS C "
                        "INNER JOIN (`MCP`.`mcp_content_ios_ext` AS IOS "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP ON CP.`id` = IOS.`cp_id` AND CP.`enable` = 1 ) "
                        "ON  IOS.`id` = C.`ext_id`  AND (C.`type_id` = 23 OR C.`type_id` = 22 ) "
                        "LEFT JOIN  `MCP`.`mcp_content_class_list` AS CL ON CL.c_id = C.id AND CL.enable = 1 "
                        "LEFT JOIN ( `MCP`.`mcp_content_data` AS D "
                        "LEFT JOIN  `MCP`.`mcp_content_charge` AS CC ON CC.`cdata_id` = D.`id` AND CC.`enable` = 1 ) "
                        "ON D.`c_id` = C.`id` AND D.`enable` = 1 AND D.`visible` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` "
                        "WHERE C.enable = 1 AND (C.status = 4 OR C.status = 1 )AND C.visible = 1 AND D.`channel_info` = 'DuoKu' "
                        "GROUP BY C.id "
                        "ORDER BY IOS.`modify_date` DESC " );

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok:[%s]\n",mysql_error(&mysql_conn));
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 27 != num_col ){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        redisContext *c;
        c=redisConnPool::getInstance()->getConnection();
        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);
                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"name",mysql_row[1]);
                bson_append_string(b,"content_type",mysql_row[2]);
                bson_append_string(b,"itunes_id",mysql_row[3]);
                bson_append_string(b,"info",mysql_row[4]);
                bson_append_string(b,"version",mysql_row[5]);
                bson_append_string(b,"cp_id",mysql_row[6]);
                //printf( "id:[%s], cp_id:[%s]\n", mysql_row[0], mysql_row[6] );

				/* 修改星级逻辑 */
                /* bson_append_int( b , "star" , atoi(mysql_row[7]) ); */
				//double star = get_star_by_c_id(mysql_row[0]);
                double star = getStarFromRedis(c, mysql_row[0]);
                bson_append_double( b , "star" , star );


                bson_append_time_t(b,"gdate",date_to_int(mysql_row[8]));
                bson_append_time_t(b,"mdate",date_to_int(mysql_row[9]));
                //printf( "%s:%s:%s\n", mysql_row[0], mysql_row[8], mysql_row[9] );
                bson_append_string(b,"system_req",mysql_row[10]);
                bson_append_int(b,"count",atoi(mysql_row[11]));
                bson_append_int(b,"rating",atoi(mysql_row[12]));
                bson_append_int(b,"crt_count",atoi(mysql_row[13]));
                bson_append_int(b,"crt_rating",atoi(mysql_row[14]));
                if(mysql_row[15]!=NULL){
                        bson_append_string(b,"data_id",mysql_row[15]);
                        bson_append_string(b,"path_url",mysql_row[16]);
                        bson_append_string(b,"jump_url",mysql_row[17]==NULL?"":mysql_row[17]);
                        bson_append_string(b,"size",mysql_row[18]);
                        if(mysql_row[19]!=NULL){
                                bson_append_double(b,"value",atof(mysql_row[19]));
                        }else{
                                bson_append_double(b,"value",0.0);
                        }
                }
                else{
                        bson_append_string(b,"data_id","");
                        bson_append_string(b,"path_url","");
                        bson_append_string(b,"jump_url","");
                        bson_append_string(b,"size","0");
                        bson_append_double(b,"value",0.0);
                }
                bson_append_double(b,"original_value",atof(mysql_row[20]));
                bson_append_string(b,"language",mysql_row[21]);

                char index[8];
                int j = 0;
                size_t pos;

                string class_list(mysql_row[22]);
                bson_append_class_info(b, class_list);

                string label_list, label_id;
                if( mysql_row[23] != NULL ){
                        label_list = mysql_row[23];
                }
                bson_append_start_array(b, "label_id");
                j = 0;
                while( !label_list.empty() ){
                        pos = label_list.find(',');
                        label_id = label_list.substr(0, pos);
                        //printf( "label_list:[%s], label_id:[%s]\n", label_list.c_str(), label_id.c_str() );
                        if( pos != string::npos ){
                                label_list.erase(0, pos+1);
                        }
                        else{
                                label_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf(index, "%d", j);
                        bson_append_string(b, index, label_id.c_str());
                        ++j;
                }
                bson_append_finish_object(b);

                string column_list;
                if(mysql_row[24] != NULL){
                        column_list = mysql_row[24];
                }
                bson_append_column_info(b,column_list);

                string img_info_list,img_type,img_url;
                size_t pos1(0), pos2(0);
                if(mysql_row[25]!=NULL){
                        img_info_list = mysql_row[25];
                }
                bson_append_start_array( b , "img_info" );
                j = 0;
                while(!img_info_list.empty()){
                        pos1 = img_info_list.find(',');
                        img_type = img_info_list.substr( 0, pos1 );
                        if(pos1!=string::npos){
                                img_info_list.erase( 0, pos1+1 );
                        }
                        else{
                                img_info_list.clear();
                                break;
                        }
                        pos2 = img_info_list.find(',');
                        img_url = img_info_list.substr( 0, pos2 );
                        if(pos2!=string::npos){
                                img_info_list.erase( 0, pos2+1 );
                        }
                        else{
                                img_info_list.clear();
                        }
                        memset(index, 0x00, sizeof(index));
                        sprintf(index, "%d", j);
                        bson_append_start_object(b, index);
                        bson_append_string(b, "img_type", img_type.c_str());
                        bson_append_string(b, "img_url", img_url.c_str());
                        bson_append_finish_object(b);
                        //printf( "{img_type:%s, img_url:%s}\n", img_type.c_str(), img_url.c_str() );
                        ++j;
                }
                bson_append_finish_object( b );

                string ptype_list;
                if(mysql_row[26]!=NULL){
                        ptype_list = mysql_row[26];
                }
                string ptype;
                int priority,tmp;
                j = 1;
                //printf( "id[%s], ptype_list:[%s], priority_list[%s]\n", mysql_row[0], ptype_list.c_str(), priority_list.c_str() );
                while( !ptype_list.empty() && j<=5 ){
                        pos1=ptype_list.find(',');
                        if(pos1 != string::npos){
                                ptype = ptype_list.substr( 0, pos1 );
                                ptype_list.erase( 0, pos1+1 );
                        }
                        else{
                                break;
                        }
                        pos2=ptype_list.find(',');
                        priority = atoi(ptype_list.substr(0,pos2).c_str());
                        ptype_list.erase( 0, pos2+1 );
                        tmp = atoi(ptype.c_str());
                        if(tmp<j){
                                continue;
                        }
                        else if(tmp==j){
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        else{
                                while(j<tmp){
                                        memset( index, 0x00, sizeof(index) );
                                        sprintf( index, "ptype_%d", j );
                                        bson_append_int( b, index, 0 );
                                        //   printf( "%s:0 ", index );
                                        ++j;
                                }
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( b, index, priority );
                                // printf( "%s:%d ", index, priority );
                        }
                        ++j;
                }
                while(j<=5){
                        memset( index, 0x00, sizeof(index) );
                        sprintf( index, "ptype_%d", j );
                        bson_append_int( b, index, 0 );
                        ++j;
                }

                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_ios_content", "mcp_ios_content_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int bson_append_class_info(bson* b, string class_list)
{
        string class_id, char_p;
        size_t pos1, pos2;
        char index[8];
        bson_append_start_array(b, "class");
        int j = 0;
        while(!class_list.empty()){
                pos1 = class_list.find(',');
                class_id = class_list.substr( 0, pos1 );
                if(pos1!=string::npos){
                        class_list.erase( 0, pos1+1 );
                }
                else{
                        class_list.clear();
                        break;
                }
                pos2 = class_list.find(',');
                char_p = class_list.substr( 0, pos2 );
                if(pos2!=string::npos){
                        class_list.erase( 0, pos2+1 );
                }
                else{
                        class_list.clear();
                }

                memset(index, 0x00, sizeof(index));
                sprintf( index, "%d", j );
                bson_append_start_object(b, index);
                bson_append_string(b, "id", class_id.c_str());
                bson_append_int(b, "p", atoi(char_p.c_str()));
                bson_append_finish_object(b);
                ++j;
        }
        bson_append_finish_object(b);
        return 0;
}

int bson_append_column_info(bson* b, string column_list)
{
        string column_id, char_p;
        size_t pos1, pos2;
        char index[8];
        bson_append_start_array(b, "column");
        int j = 0;
        while(!column_list.empty()){
                pos1 = column_list.find(',');
                column_id = column_list.substr( 0, pos1 );
                if(pos1!=string::npos){
                        column_list.erase( 0, pos1+1 );
                }
                else{
                        column_list.clear();
                        break;
                }
                pos2 = column_list.find(',');
                char_p = column_list.substr( 0, pos2 );
                if(pos2!=string::npos){
                        column_list.erase( 0, pos2+1 );
                }
                else{
                        column_list.clear();
                }

                memset(index, 0x00, sizeof(index));
                sprintf( index, "%d", j );
                bson_append_start_object(b, index);
                bson_append_string(b, "id", column_id.c_str());
                bson_append_int(b, "p", atoi(char_p.c_str()));
                bson_append_finish_object(b);
                ++j;
        }
        bson_append_finish_object(b);
        return 0;
}

int bson_append_pub_prop_info(bson* b, string pub_prop_info)
{
	string id, name, color;
	size_t pos1, pos2;
	char index[8];
	bson_append_start_array( b , "pub_prop_infos" );
	int j = 0;
	while(!pub_prop_info.empty())
	{
		pos1 = pub_prop_info.find(',');
		if(pos1!=string::npos)
		{
			id = pub_prop_info.substr( 0, pos1 );
			pub_prop_info.erase( 0, pos1+1 );
		}
		else
		{
			id = pub_prop_info;
			pub_prop_info.clear();
		}
		memset(index, 0x00, sizeof(index));
		sprintf(index, "%d", j);
		bson_append_start_object(b, index);
//		bson_append_string(b, "id", id.c_str());
		bson_append_string(b, "name", pub_prop_record_map[id].name.c_str());
		bson_append_string(b, "color", pub_prop_record_map[id].color.c_str());
		bson_append_string(b, "prop_url", pub_prop_record_map[id].prop_url.c_str());
		bson_append_finish_object(b);
		++j;

	}
	bson_append_finish_object( b );
	return 0;
}

int bson_append_img_info(bson* b, string img_list)
{
        string img_type, img_url;
        size_t pos1, pos2;
        char index[8];
        bson_append_start_array( b , "img_info" );
        int j = 0;
        while(!img_list.empty()){
                pos1 = img_list.find(',');
                img_type = img_list.substr( 0, pos1 );
                if(pos1!=string::npos){
                        img_list.erase( 0, pos1+1 );
                }
                else{
                        img_list.clear();
                        break;
                }
                pos2 = img_list.find(',');
                img_url = img_list.substr( 0, pos2 );
                if(pos2!=string::npos){
                        img_list.erase( 0, pos2+1 );
                }
                else{
                        img_list.clear();
                }
                memset(index, 0x00, sizeof(index));
                sprintf(index, "%d", j);
                bson_append_start_object(b, index);
                bson_append_string(b, "img_type", img_type.c_str());
                bson_append_string(b, "img_url", img_url.c_str());
                bson_append_finish_object(b);
                ++j;
        }
        bson_append_finish_object( b );
        return 0;
}

int bson_append_label_info(bson* b, string label_list)
{
        string label_id;
        size_t pos;
        char index[8];
        bson_append_start_array(b, "label_id");
        int j = 0;
        while( !label_list.empty() ){
                pos = label_list.find(',');
                label_id = label_list.substr(0, pos);
                //printf( "label_list:[%s], label_id:[%s]\n", label_list.c_str(), label_id.c_str() );
                if( pos != string::npos ){
                        label_list.erase(0, pos+1);
                }
                else{
                        label_list.clear();
                }
                memset(index, 0x00, sizeof(index));
                sprintf(index, "%d", j);
                bson_append_string(b, index, label_id.c_str());
                ++j;
        }
        bson_append_finish_object(b);
        return 0;
}

int bson_append_ptype_info(bson* b, string ptype_list)
{
        string ptype;
        int priority,tmp;
        size_t pos1(0), pos2(0);
        int j = 1;
        char index[8];
        while( !ptype_list.empty() && j<=5 ){
                pos1=ptype_list.find(',');
                if(pos1 != string::npos){
                        ptype = ptype_list.substr( 0, pos1 );
                        ptype_list.erase( 0, pos1+1 );
                }
                else{
                        break;
                }
                pos2=ptype_list.find(',');
                priority = atoi(ptype_list.substr(0,pos2).c_str());
                ptype_list.erase( 0, pos2+1 );
                tmp = atoi(ptype.c_str());
                if(tmp<j){
                        continue;
                }
                else if(tmp==j){
                        memset( index, 0x00, sizeof(index) );
                        sprintf( index, "ptype_%d", j );
                        bson_append_int( b, index, priority );
                }
                else{
                        while(j<tmp){
                                memset( index, 0x00, sizeof(index) );
                                sprintf( index, "ptype_%d", j );
                                bson_append_int( b, index, 0 );
                                ++j;
                        }
                        memset( index, 0x00, sizeof(index) );
                        sprintf( index, "ptype_%d", j );
                        bson_append_int( b, index, priority );
                }
                ++j;
        }
        while(j<=5){
                memset( index, 0x00, sizeof(index) );
                sprintf( index, "ptype_%d", j );
                bson_append_int( b, index, 0 );
                ++j;
        }
        return 0;
}

int bson_append_attribute_info(bson* b, string attribute_list)
{
        int type;
        string content,str;
        size_t pos1, pos2;
        char index[8];
        bson_append_start_array( b , "attribute" );
        int j = 0;

        pos1 = attribute_list.find('^');
        type = atoi(attribute_list.substr(0, pos1).c_str());
        attribute_list.erase( 0, pos1+1 );

        while(!attribute_list.empty()){
                pos1 = attribute_list.find('^');
                str = attribute_list.substr( 0, pos1 );
                pos2 = str.find_last_of(',');
                content  = str.substr( 0, pos2 );
                if(pos1!=string::npos){
                        attribute_list.erase( 0, pos1+1 );
                }
                else{
                        attribute_list.clear();
                }
                memset(index, 0x00, sizeof(index));
                sprintf(index, "%d", j);

                bson_append_start_object(b, index);
                bson_append_int(b, "type", type);
                bson_append_string(b, "content", content.c_str());
                bson_append_finish_object(b);
                ++j;
                type = atoi(str.substr(pos2+1).c_str());
        }
        bson_append_finish_object( b );
        return 0;
}

int del_class_list_by_type( const string& content_type )
{
        return 0;
}

int set_mcp_content_news()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_news_bak");
        sprintf(ns,"mcp.mcp_content_news_bak");
        sprintf(query_sql,
                        "SELECT N.`id`, N.`name`, N.`content`, N.`type_id`, N.`logdate`, N.`c_id`, N.`game_type`"

                // [s] add by gwj for news_source flag
                        ", N.`news_source` "
                // [e] add by gwj for news_source flag

                        "FROM `MCP`.`mcp_content_news` AS N "
                // [s] add by gwj for news_game_rel
                        "LEFT JOIN `MCP`.`mcp_content` AS C ON N.`c_id` = C.`id` "
                // [e] add by gwj for news_game_rel

                        "WHERE N.`enable` = 1 "

                // [s] add by gwj for news_game_rel
                        "AND C.`enable` = 1 "
                // [e] add by gwj for news_game_rel

                        "ORDER BY logdate DESC");

        //printf("sql_query[%s]", query_sql);
        if(mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if(8 != num_col)
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"name",mysql_row[1]);
                bson_append_string(b,"content",mysql_row[2]);
                bson_append_string(b,"type_id",mysql_row[3]);
                bson_append_string(b,"logdate",mysql_row[4]);
                bson_append_string(b,"c_id",mysql_row[5]);
                bson_append_string(b,"game_type",mysql_row[6]);

                // [s] add by gwj for news_source flag
                bson_append_string(b,"news_source", mysql_row[7]);
                // [e] add by gwj for news_source flag

                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_news", "mcp_content_news_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

// [s] add by gwj for empty_game_news
int set_mcp_content_empty_game_news()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_news");
        sprintf(ns,"mcp.mcp_content_news");
        sprintf(query_sql,
                        "SELECT N.`id`, N.`name`, N.`content`, N.`type_id`, N.`logdate`, N.`c_id`, N.`game_type` , N.`news_source` "
                        "FROM `MCP`.`mcp_content_news` AS N "
                        "WHERE N.`c_id` = \"0\"  AND N.`enable` = 1 "
                        "ORDER BY logdate DESC");

        printf("sql_query[%s]", query_sql);

        if(mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }

        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        if(8 != num_col)
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for(f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"name",mysql_row[1]);
                bson_append_string(b,"content",mysql_row[2]);
                bson_append_string(b,"type_id",mysql_row[3]);
                bson_append_string(b,"logdate",mysql_row[4]);
                bson_append_string(b,"c_id",mysql_row[5]);
                bson_append_string(b,"game_type",mysql_row[6]);
                bson_append_string(b,"news_source", mysql_row[7]);

                bson_finish(b);
                mongo_insert(mongodb_conn, ns, b, NULL);
                bson_destroy(b);
        }

        mysql_free_result(mysql_result);
        return 0;
}
// [e] add by gwj for empty_game_news
int set_mcp_appscore()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_appscore_bak");
        sprintf(ns,"mcp.mcp_content_appscore_bak");
        sprintf(query_sql,
                        "SELECT S.`id`, S.`imei`, S.`c_id`, S.`package_name`, S.`u_id`, S.`score`, S.`version_code` "
                        "FROM `MCP`.`mcp_content_appscore` AS S "
                        "WHERE S.`enable` = 1 " );

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if(7 != num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"imei",mysql_row[1]);
                bson_append_string(b,"c_id",mysql_row[2]);
                bson_append_string(b,"package_name",mysql_row[3]);
                bson_append_string(b,"u_id",mysql_row[4]);
                bson_append_string(b,"score",mysql_row[5]);
                bson_append_int(b,"version_code",mysql_row[6]==NULL?0:atoi(mysql_row[6]));


                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_appscore", "mcp_content_appscore_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_appinfo()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_appinfo_bak");
        sprintf(ns,"mcp.mcp_content_appinfo_bak");
        sprintf(query_sql,
                        "SELECT S.`id`, S.`c_id`, S.`app_name`, S.`package_name`, S.`version_code`, S.`version`, S.`sdk_version`, S.`permissions`, S.`features`, S.`apk_info` "
                        "FROM `MCP`.`mcp_content_appinfo` AS S LEFT JOIN `MCP`.`mcp_content` AS C on S.`c_id`=`C`.id "
                        " LEFT JOIN `MCP`.`mcp_content_data` AS D ON S.`c_id` = D.`c_id` AND C.enable=1 AND C.status=1 AND D.`channel_info`='DuoKu' ");

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if(10 != num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"c_id",mysql_row[1]);
                bson_append_string(b,"app_name",mysql_row[2]);
                bson_append_string(b,"package_name",mysql_row[3]);
                //bson_append_string(b,"version_code",mysql_row[4]);
                bson_append_int(b,"version_code",mysql_row[4]==NULL?0:atoi(mysql_row[4]));
                bson_append_string(b,"version",mysql_row[5]);
                bson_append_string(b,"sdk_version",mysql_row[6]);
                bson_append_string(b,"permission",mysql_row[7]);
                bson_append_string(b,"features",mysql_row[8]);
                bson_append_string(b,"apk_info",mysql_row[9]);


                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_appinfo", "mcp_content_appinfo_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_online_game()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b[1];
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_online_game_bak");
        sprintf(ns,"mcp.mcp_online_game_bak");

//        sprintf(query_sql,
//                        "SELECT  C.`id`, C.`name`, C.`type_id`, GE.`info`, GE.`version`, "
//                        "GE.`adapt`, GE.`cp_id`, GE.`star`, GE.`enName`, GE.`language`, "
//                        "GE.`gdate`, GE.`logdate`, GE.`modify_date`, "
//                        "D.`id`,D.`path_url`, D.`jump_url`, D.`size`, CC.`value`, "
//                        "GROUP_CONCAT(DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`priority` DESC ), "
//                        "GROUP_CONCAT(DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
//                        "GROUP_CONCAT(DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`column_id` ), "
//                        "GROUP_CONCAT(DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
//                        "GROUP_CONCAT(DISTINCT R.`ptype_id`, \",\", R.`priority` ORDER BY R.`ptype_id` ), "
//                        "GROUP_CONCAT(DISTINCT A.`type`, \"^\", A.`content` ORDER BY A.`type` ) "
//                        "FROM `MCP`.`mcp_content_game_ext` AS GE "
//                        "INNER JOIN `MCP`.`mcp_content` AS C "
//                        "ON  C.`id` = GE.`c_id` AND C.`type_id` = 25 AND C.enable = 1 AND C.status = 1 AND C.visible = 1 "
//                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP ON CP.`id` = GE.`cp_id` AND CP.`enable` = 1 "
//                        "LEFT JOIN  `MCP`.`mcp_content_class_list` AS CL ON CL.c_id = C.id AND CL.enable = 1 "
//                        "LEFT JOIN ( `MCP`.`mcp_content_data` AS D "
//                        "LEFT JOIN  `MCP`.`mcp_content_charge` AS CC ON CC.`cdata_id` = D.`id` AND CC.`enable` = 1 ) "
//                        "ON D.`c_id` = C.`id` AND D.`enable` = 1 AND D.`visible` = 1 AND D.`channel_info` = 'DuoKu' "
//                        "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
//                        "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
//                        "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
//                        "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` "
//                        "LEFT JOIN `MCP`.`mcp_content_attribute` AS A ON A.`c_id` = C.`id` AND A.`enable` = 1 "
//                        "GROUP BY C.id "
//                        "ORDER BY GE.`gdate` DESC " );
		/* 20130328 zhengxie mongodb.mcp.mcp_online_game新增channel_info字段 beg*/
        sprintf(query_sql,
                        "SELECT  C.`id`, C.`name`, C.`type_id`, GE.`info`, GE.`version`, "
                        "GE.`adapt`, GE.`cp_id`, GE.`star`, GE.`enName`, GE.`language`, "
                        "GE.`updatedate`, GE.`updatedate`, GE.`updatedate`, "
                        "D.`id`,D.`path_url`, D.`jump_url`, D.`size`, CC.`value`, "
                        "GROUP_CONCAT(DISTINCT CL.`class_id`, \",\", CL.`priority` ORDER BY CL.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT BL.`cb_id` ORDER BY BL.`cb_id` ), "
                        "GROUP_CONCAT(DISTINCT COL.`column_id`, \",\", COL.`priority` ORDER BY COL.`column_id` ), "
                        "GROUP_CONCAT(DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
                        "GROUP_CONCAT(DISTINCT R.`ptype_id`, \",\", R.`priority` ORDER BY R.`ptype_id` ), "
                        "GROUP_CONCAT(DISTINCT A.`type`, \"^\", A.`content` ORDER BY A.`type` ), "
                        "D.`channel_info` "

                        // [s] add by gwj for mcp_content_pub_prop
                        ", GE.`pub_props` "
                        // [e] add by gwj for mcp_content_pub_prop
						// [s] add by zhengxie for editor_recom
                        ", GE.`editor_recom` "
						// [s] add by zhengxie for editor_recom

                        "FROM `MCP`.`mcp_content_game_ext` AS GE "
                        "INNER JOIN `MCP`.`mcp_content` AS C "
                        "ON  C.`id` = GE.`c_id` AND (C.`type_id` = 25 OR C.`type_id` = 31) AND C.enable = 1 AND C.status = 1 AND C.visible = 1 "
                        "INNER JOIN `MCP`.`mcp_content_cp` AS CP ON CP.`id` = GE.`cp_id` AND CP.`enable` = 1 "
                        "LEFT JOIN  `MCP`.`mcp_content_class_list` AS CL ON CL.c_id = C.id AND CL.enable = 1 "
                        "INNER JOIN ( `MCP`.`mcp_content_data` AS D "
                        "LEFT JOIN  `MCP`.`mcp_content_charge` AS CC ON CC.`cdata_id` = D.`id` AND CC.`enable` = 1 ) "
                        "ON D.`c_id` = C.`id` AND D.`enable` = 1 AND D.`visible` = 1 AND (D.`channel_info` = 'Duoku' OR D.`channel_info` = 'TEMP') "
                        "LEFT JOIN `MCP`.`mcp_content_preview` AS PV ON PV.`c_id` = C.`id` AND PV.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_bookmark_list` AS BL ON BL.`c_id` = C.`id` AND BL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_column_list` AS COL ON COL.`c_id` = C.`id` AND COL.`enable` = 1 "
                        "LEFT JOIN `MCP`.`mcp_content_rank` AS R ON R.`c_id` = C.`id` "
                        "LEFT JOIN `MCP`.`mcp_content_attribute` AS A ON A.`c_id` = C.`id` AND A.`enable` = 1 "
                        "GROUP BY C.id "
                        "ORDER BY GE.`gdate` DESC " );
        /* 20130328 zhengxie mongodb.mcp.mcp_online_game新增channel_info字段 end*/

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok:[%s]\n",mysql_error(&mysql_conn));
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        // [s] modify by gwj for mcp_content_pub_prop
        //if ( 25 != num_col ){
        if ( 27 != num_col ){
        // [e] modify by gwj for mcp_content_pub_prop
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }
        redisContext *c;
        c=redisConnPool::getInstance()->getConnection();
        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);
                bson_append_new_oid(b,"_id");
                bson_append_string(b,"id",			mysql_row[0]);
                bson_append_string(b,"name",		mysql_row[1]);
                bson_append_string(b,"content_type",mysql_row[2]);
                bson_append_string(b,"info",		mysql_row[3]);
                bson_append_string(b,"version",		mysql_row[4]);
                bson_append_string(b,"system_req",	mysql_row[5]);
                bson_append_string(b,"cp_id",		mysql_row[6]);
//                bson_append_int   (b,"star",        atoi(mysql_row[7]));

				/* 修改星级逻辑 */
                /* bson_append_int( b , "star" , atoi(mysql_row[8]) ); */
				//double star = get_star_by_c_id(mysql_row[0]);
                double star = getStarFromRedis(c, mysql_row[0]);
                bson_append_double( b , "star" , star );

                //bson_append_int   (b,"popularity",	atoi(mysql_row[8]));
                bson_append_string(b,"language",	mysql_row[9]);
                bson_append_time_t(b,"gdate",date_to_int(mysql_row[10]));
                bson_append_time_t(b,"ldate",date_to_int(mysql_row[11]));
                bson_append_time_t(b,"mdate",date_to_int(mysql_row[12]));

                bson_append_string(b,"data_id", mysql_row[13]==NULL?"":mysql_row[13]);
                bson_append_string(b,"path_url",mysql_row[14]==NULL?"":mysql_row[14]);
                bson_append_string(b,"jump_url",mysql_row[15]==NULL?"":mysql_row[15]);
                bson_append_int   (b,"size",    mysql_row[16]==NULL?0:atoi(mysql_row[16]));
                bson_append_double(b,"value",   mysql_row[17]==NULL?0.0:atof(mysql_row[17]));

                bson_append_class_info(b, mysql_row[18]==NULL?"":mysql_row[18]);
                bson_append_label_info(b, mysql_row[19]==NULL?"":mysql_row[19]);
                bson_append_column_info(b, mysql_row[20]==NULL?"":mysql_row[20]);
                bson_append_img_info(b, mysql_row[21]==NULL?"":mysql_row[21]);
                bson_append_ptype_info(b, mysql_row[22]==NULL?"":mysql_row[22]);
                bson_append_attribute_info(b, mysql_row[23]==NULL?"":mysql_row[23]);

                /* 20130328 zhengxie mongodb.mcp.mcp_online_game新增channel_info字段 beg*/
        		if(NULL == mysql_row[24]){
                	bson_append_string(b,"channel_info","NULL");
            	}else{
            		bson_append_string(b,"channel_info",mysql_row[24]);
            	}
            	/* 20130328 zhengxie mongodb.mcp.mcp_online_game新增channel_info字段 beg*/

                // [s] add by gwj for mcp_content_pub_prop
                if(NULL == mysql_row[25] )
                {
                    bson_append_string(b,"pub_prop_ids","");
                    bson_append_string(b,"pub_prop_infos","");
                }
                else
                {
                    bson_append_string(b,"pub_prop_ids",mysql_row[25]);
                    bson_append_pub_prop_info(b, mysql_row[25]);
                }
                // [e] add by gwj for mcp_content_pub_prop
				// [s] add by zhengxie for editor_recom
                if(NULL == mysql_row[26])
                {
                    bson_append_string(b,"editor_recom","");
                }
                else
                {
                    bson_append_string(b,"editor_recom",mysql_row[26]);
                }
				// [e] add by zhengxie for editor_recom

                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_online_game", "mcp_online_game_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_mixed_topic_music()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;

        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));

        bson b[1];
        int num_col;
        char col[64];
        char ns[64];

        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_mixed_topic_music_bak");
        sprintf(ns,"mcp.mcp_mixed_topic_music_bak");
        sprintf(query_sql, "SELECT T.`mix_topic_id`,T.`music_id`,M.`song_name`,M.`area_id`,A.`name`,M.`cp_id`,C.`name`,M.`language_id`,L.`name`, "
                        "M.`publish_date`, M.`intro`, M.`lyric_writer`, M.`composer`, M.`price`, M.`discount`, M.`download_base`, M.`lyrictxt` "
                        "FROM `MCP`.`mcp_music_mixed_topic` AS T  "
                        "LEFT JOIN `MCP`.`mcp_music_ext` AS M on T.`music_id`=M.`id` "
                        "LEFT JOIN `MCP`.`mcp_music_song_area` AS A on M.`area_id` = A.`id` "
                        "LEFT JOIN `MCP`.`mcp_content_cp` AS C on M.`cp_id` = C.`id` "
                        "LEFT JOIN `MCP`.`mcp_music_song_language` as L on M.`language_id` = L.`id` "
                        "WHERE T.`enable`=1 AND M.`enable`=1 ");

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }

        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        if(17 != num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }


        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);

                bson_append_new_oid(b,"_id");
                bson_append_string(b,"topic_id",mysql_row[0]);
                bson_append_string(b,"music_id",mysql_row[1]);
                bson_append_string(b,"music_name",mysql_row[2]);
                bson_append_string(b,"area_id",mysql_row[3]);
                bson_append_string(b,"area_name",mysql_row[4]);
                bson_append_string(b,"cp_id",mysql_row[5]);
                bson_append_string(b,"cp_name",mysql_row[6]);
                bson_append_string(b,"language_id",mysql_row[7]);
                bson_append_string(b,"language",mysql_row[8]);
                bson_append_string(b,"publish_date",mysql_row[9]);
                bson_append_string(b,"intro",mysql_row[10]);
                bson_append_string(b,"lyric_writer",mysql_row[11]);
                bson_append_string(b,"composer",mysql_row[12]);
                bson_append_string(b,"price",mysql_row[13]);
                bson_append_string(b,"discount",mysql_row[14]);
                bson_append_string(b,"download_base",mysql_row[15]);
                bson_append_string(b,"lyric_txt",mysql_row[16]);


                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_mixed_topic_music", "mcp_mixed_topic_music_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_recently_played()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;

        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));

        bson b[1];
        int num_col;
        char col[64];
        char ns[64];

        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_recently_played_bak");
        sprintf(ns,"mcp.mcp_content_recently_played_bak");

        sprintf(query_sql,"SELECT R.`u_id`, GROUP_CONCAT(R.`c_id` ORDER BY R.`priority`) FROM mcp_content_recently_played AS R where R.`enable` =1 GROUP BY R.`u_id`");

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }

        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        if(2 != num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        char index[16];
        size_t pos;
        string uid;
        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                uid=mysql_row[0];
                bson_init(b);
                bson_append_string(b,"u_id",mysql_row[0]);

                string cids = mysql_row[1];
                string id;
                int cnt = 0;

                printf("cids is %s\n", cids.c_str());
                while(!cids.empty())
                {
                        printf("cids is %s\n", cids.c_str());
                        pos = cids.find(",");

                        if(pos != string::npos)
                        {
                                id = cids.substr(0,pos);
                                cids.erase(0,pos+1);
                        }
                        else
                        {
                                id = cids;
                                cids.clear();
                        }

                        sprintf(index,"%d", cnt);

                        bson_append_string(b,index, id.c_str());
                        cnt++;
                }
                bson_finish( b );
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }

        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_recently_played", "mcp_content_recently_played_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_favorite_list()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;

        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));

        bson b[1];
        int num_col;
        char col[64];
        char ns[64];

        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_favorite_list_bak");
        sprintf(ns,"mcp.mcp_content_favorite_list_bak");

        sprintf(query_sql,"SELECT F.`u_id`, GROUP_CONCAT(F.`c_id` ORDER BY F.`priority`) FROM mcp_content_favorite_list AS F where F.`enable` =1 GROUP BY F.`u_id`");

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }

        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        if(2 != num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        char index[16];
        size_t pos;
        string uid;
        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                uid=mysql_row[0];
                bson_init(b);
                bson_append_string(b,"u_id",mysql_row[0]);

                string cids = mysql_row[1];
                string id;
                int cnt = 0;

                bson_append_start_array( b ,"list");
//                printf("cids is %s\n", cids.c_str());
                while(!cids.empty())
                {
//                        printf("cids is %s\n", cids.c_str());
                        pos = cids.find(",");

                        if(pos != string::npos)
                        {
                                id = cids.substr(0,pos);
                                cids.erase(0,pos+1);
                        }
                        else
                        {
                                id = cids;
                                cids.clear();
                        }

                        sprintf(index,"%d", cnt);

                        bson_append_start_object( b, index );
                        bson_append_string(b,"priority", index);
                        bson_append_string(b,"c_id", id.c_str());
                        bson_append_finish_object(b);
                        cnt++;
                }
                bson_append_finish_object(b);
                bson_finish( b );
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }

        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_favorite_list", "mcp_content_favorite_list_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_content_calender()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;

        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));

        bson b[1];
        int num_col;
        char col[64];
        char ns[64];

        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_calender_bak");
        sprintf(ns,"mcp.mcp_content_calender_bak");

        sprintf(query_sql,"SELECT C.`id`,C.`enable_date`,C.`title`,C.`content_id`,C.`type_id` FROM mcp_content_calender AS C "
                        "where C.`enable` =1");

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }

        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        if(5 != num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        string uid;
        for(f1 = 0; f1 < num_row; f1++){
                mysql_row = mysql_fetch_row(mysql_result);
                uid=mysql_row[0];
                bson_init(b);
                bson_append_string(b,"id",mysql_row[0]);
                bson_append_string(b,"enable_date",mysql_row[1]);
                bson_append_string(b,"title",mysql_row[2]);
                bson_append_string(b,"content_id",mysql_row[3]);
                bson_append_string(b,"type_id",mysql_row[4]);

                bson_finish( b );
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }

        mysql_free_result(mysql_result);
        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_calender", "mcp_content_calender_bak");
        // [e] add by gwj for rename collection name
        return 0;
}

int set_mcp_H5_music_id()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;

        int f1,  num_row ;
        char query_sql[2048];
        memset(query_sql, 0x00, sizeof(query_sql));

        bson b[1];
        int num_col;
        char col[64];
        char ns[64];

        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mcp_content_H5_music_id_bak");
        sprintf(ns,"mcp.mcp_content_H5_music_id_bak");

        sprintf(query_sql,"SELECT C.`column_id`,GROUP_CONCAT(C.`c_id` ORDER BY C.`priority`) FROM mcp_content_column_list AS C "
                        "LEFT JOIN mcp_content_diy_column AS D ON C.`column_id` = D.`id` where D.`type_id` = 27 "
                        "and C.`enable` =1 GROUP BY C.`column_id`" );

        if(mysql_query(&mysql_conn, query_sql) != 0){
                printf("query is not ok\n");
                return -1;
        }

        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);

        if(2 != num_col){
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

        size_t pos1;
        string cids,cid;
        for(f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init(b);
                bson_append_string(b,"column_id",mysql_row[0]);
                cids = mysql_row[1];
                bson_append_start_array(b, "cid_list");

                int j=0;
                char index[16];
                while(!cids.empty())
                {
                        pos1=cids.find(",");
                        if(pos1 != string::npos)
                        {
                                cid = cids.substr(0,pos1);
                                cids.erase(0,pos1+1);
                        }
                        else
                        {
                                cid=cids;
                                cids.clear();
                        }
                        sprintf(index,"%d",j);
                        bson_append_string(b,index,cid.c_str());
                        j++;
                }

                bson_append_finish_object(b);
                bson_finish(b);
                mongo_insert(mongodb_conn,ns,b, NULL);
                bson_destroy(b);
        }

        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mcp_content_H5_music_id", "mcp_content_H5_music_id_bak");
        // [e] add by gwj for rename collection name

        return 0;
}

// [s] add by gwj for common_adv_info
int set_mcp_content_common_info()
{
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;

    int f1, num_row;
    char query_sql[2048];
    memset(query_sql, 0x00, sizeof(query_sql));

    bson b[1];
    int num_col;
    char col[64];
    char ns[64];

    memset(col, 0x00, sizeof(col));
    memset(ns, 0x00, sizeof(ns));

    sprintf(col, "mcp_content_common_info_bak");
    sprintf(ns, "mcp.mcp_content_common_info_bak");

    sprintf(query_sql,
            "SELECT CC.`id`, CC.`info`, CC.`type`, CC.`name` FROM mcp_content_common_info AS CC ");

    if(mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mongo_cmd_drop_collection(mongodb_conn, "mcp", col, NULL);
    mysql_result = mysql_store_result(&mysql_conn);
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if(4 != num_col)
    {
        printf(" colum count %d is err\n", num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    for(f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);
        bson_append_string(b, "id", mysql_row[0]);
        bson_append_string(b, "info", mysql_row[1]);
        bson_append_string(b, "name", mysql_row[2]);
        bson_append_string(b, "type", mysql_row[3]);
        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
    }

    mysql_free_result(mysql_result);

    // [s] add by gwj for rename collection name
    rename_mongodb_collection(mongodb_conn, "mcp",
                              "mcp_content_common_info", "mcp_content_common_info_bak");
    // [e] add by gwj for rename collection name

    return 0;
}
// [e] add by gwj for common_adv_info

// [s] add by gwj for common_interface
CommonInterface::CommonInterface()
{
}

CommonInterface::~CommonInterface()
{
}

// 找到 value 在 list 中的索引，不存在时返回 -1
int CommonInterface::find_key_index(const char* value, vector<const char*>& list)
{
    if(!IS_VALUED_STRING(value))
    {
        return -1;
    }

    int size = list.size();

    for(int i = 0; i < size; i++)
    {
        if(IS_SAME_VALUED_STRING(value, list[i]))
        {
            return i;
        }
    }

    return -1;
}

int CommonInterface::find_key_index(const char* value, vector<string>& list)
{
    if(!IS_VALUED_STRING(value))
    {
        return -1;
    }

    int size = list.size();

    for(int i = 0; i < size; i++)
    {
        if(IS_SAME_VALUED_STRING(value, list[i].c_str()))
        {
            return i;
        }
    }

    return -1;
}

// 数组赋值拷贝
void CommonInterface::copy_vector_values(vector<string>& src, vector<string>& des)
{
    int size = src.size();

    for(int i = 0; i < size; i++)
    {
        des[i] = src[i];
    }
}

// 删除字符串数组数据
void CommonInterface::clear_vector_string(vector<string>& list)
{
    int size = list.size();

    for(int i = 0; i < size; i++)
    {
        list[i].clear();
    }

    list.clear();
}

int CommonInterface::splic_string(string& orig_string,
                                  vector<string>& split_result,
                                  const char* seperator)
{
    if(orig_string.empty())
    {
        return 0;
    }

    int pos = 0;

    while(!orig_string.empty())
    {
        pos = orig_string.find(seperator);
        if(pos != string::npos)
        {
            string ss = orig_string.substr(0,pos);
            split_result.push_back(ss);

            orig_string.erase(0,pos+1);
        }
        else
        {
            split_result.push_back(orig_string);
            orig_string="";
        }
    }

    return split_result.size();
}

void CommonInterface::set_struct_mongodb_value(bson* b,
                                               const char* field_name,
                                               vector<const char*>& struct_cols,
                                               string& mysql_value)
{
    char index[8];

    int col_size = struct_cols.size();
    if(col_size == 0)
    {
        return;
    }

    vector<string> values;
    CommonInterface::splic_string(mysql_value, values, ",");

    bson_append_start_array(b, field_name);

    int valuse_size = values.size();
    int index_i = 0;
    for(int i = 0; i < valuse_size; i += col_size, index_i++)
    {
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", index_i);
        bson_append_start_object(b, index);

        for(int j = 0; j < col_size; j++)
        {
            bson_append_string(b, struct_cols[j], values[i + j].c_str());
        }

        bson_append_finish_object(b);
    }

    bson_append_finish_array(b);
    return;
}

void CommonInterface::set_array_mongodb_value(bson* b,
                                              const char* field_name,
                                              string& mysql_value)
{
    char index[8];
    vector<string> values;
    CommonInterface::splic_string(mysql_value, values, ",");

    bson_append_start_array(b, field_name);

    int valuse_size = values.size();
    for(int i = 0; i < valuse_size; i++)
    {
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(b, index, values[i].c_str());
    }

    bson_append_finish_array(b);
    return;
}
// [e] add by gwj for common_interface

// [s] add by gwj for mcp_activity
int set_mcp_activity_info()
{
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;

    int f1, num_row;
    char query_sql[2048];
    memset(query_sql, 0x00, sizeof(query_sql));

    bson b[1];
    int num_col;
    char col[64];
    char ns[64];

    memset(col, 0x00, sizeof(col));
    memset(ns, 0x00, sizeof(ns));

    sprintf(col, "mcp_activity_info_bak");
    sprintf(ns, "mcp.mcp_activity_info_bak");

    sprintf(query_sql,
            "SELECT ACT.id, ACT.title, ACT.content_des, ACT.defination, ACT.c_id, ACT.start_time, ACT.end_time, "
            "ACTT.type_name, ACTJR.join_reg_type, ACT.qualifier_regular, "
            "GROUP_CONCAT( DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ), "
            "GROUP_CONCAT( DISTINCT ACTAWD.`id`, \",\", ACTAWD.`award_info` ORDER BY ACTAWD.`award_rank` ASC ) "
            "FROM mcp_activity_basic_info AS ACT "
            "LEFT JOIN mcp_activity_type AS ACTT ON (ACT.act_type_id = ACTT.id AND ACTT.enable = 1) "
            "LEFT JOIN mcp_activity_join_regular AS ACTJR ON (ACT.join_regular_id = ACTJR.id AND ACTJR.enable = 1) "
            "LEFT JOIN mcp_activity_award_basic_info AS ACTAWD ON (ACT.id = ACTAWD.act_id AND ACTAWD.enable = 1) "
            "LEFT JOIN mcp_content_preview AS PV ON (PV.c_id = ACT.id AND PV.enable = 1) "
            "WHERE ACT.enable = 1 "
            "GROUP BY ACT.id "
            "ORDER BY ACT.start_time DESC ");

    //printf("sql : [%s]\n", query_sql);

    if(mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mongo_cmd_drop_collection(mongodb_conn, "mcp", col, NULL);
    mysql_result = mysql_store_result(&mysql_conn);
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if(12 != num_col)
    {
        printf(" colum count %d is err\n", num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    for(f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        if(mysql_row[0] == NULL)
        {
            bson_append_string(b, "id", "");
        }
        else
        {
            bson_append_string(b, "id", mysql_row[0]);
        }

        if(mysql_row[1] == NULL)
        {
            bson_append_string(b, "title", "");
        }
        else
        {
            bson_append_string(b, "title", mysql_row[1]);
        }

        if(mysql_row[2] == NULL)
        {
            bson_append_string(b, "content_des", "");
        }
        else
        {
            bson_append_string(b, "content_des", mysql_row[2]);
        }

        if(mysql_row[3] == NULL)
        {
            bson_append_string(b, "defination", "");
        }
        else
        {
            bson_append_string(b, "defination", mysql_row[3]);
        }

        if(mysql_row[4] == NULL)
        {
            bson_append_string(b, "c_id", "");
        }
        else
        {
            bson_append_string(b, "c_id", mysql_row[4]);
        }

        if(mysql_row[5] == NULL)
        {
            bson_append_string(b, "start_time", "");
        }
        else
        {
            bson_append_string(b, "start_time", mysql_row[5]);
        }

        if(mysql_row[6] == NULL)
        {
            bson_append_string(b, "end_time", "");
        }
        else
        {
            bson_append_string(b, "end_time", mysql_row[6]);
        }

        if(mysql_row[7] == NULL)
        {
            bson_append_string(b, "type_name", "");
        }
        else
        {
            bson_append_string(b, "type_name", mysql_row[7]);
        }

        if(mysql_row[8] == NULL)
        {
            bson_append_string(b, "join_reg_type", "");
        }
        else
        {
            bson_append_string(b, "join_reg_type", mysql_row[8]);
        }

        if(mysql_row[9] == NULL)
        {
            bson_append_string(b, "qualifier_regular", "");
        }
        else
        {
            bson_append_string(b, "qualifier_regular", mysql_row[9]);
        }

        string img_info = mysql_row[10];
        vector<const char*> columns;
        columns.push_back("img_type");
        columns.push_back("img_url");
        CommonInterface::set_struct_mongodb_value(b, "img_info", columns, img_info);

        string awd_info = mysql_row[11];
        columns.clear();
        columns.push_back("id");
        columns.push_back("award");
        CommonInterface::set_struct_mongodb_value(b, "award_info", columns, awd_info);

        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
    }

    mysql_free_result(mysql_result);

    // [s] add by gwj for rename collection name
    rename_mongodb_collection(mongodb_conn, "mcp",
                              "mcp_activity_info", "mcp_activity_info_bak");
    // [e] add by gwj for rename collection name
    return 0;
}

int set_mcp_activity_award_info()
{
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;

    int f1, num_row;
    char query_sql[2048];
    memset(query_sql, 0x00, sizeof(query_sql));

    bson b[1];
    int num_col;
    char col[64];
    char ns[64];

    memset(col, 0x00, sizeof(col));
    memset(ns, 0x00, sizeof(ns));

    sprintf(col, "mcp_activity_award_info_bak");
    sprintf(ns, "mcp.mcp_activity_award_info_bak");

    sprintf(query_sql,
            "SELECT ACTAWD.id, ACTAWD.act_id, ACTAWD.award_rank, ACTAWD.award_info, ACTAWD.total_items_cnt, "
            "ACTAWD.hit_ratio "
            "FROM mcp_activity_award_basic_info AS ACTAWD "
            "WHERE ACTAWD.enable = 1 ");

    //printf("sql : [%s]\n", query_sql);

    if(mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mongo_cmd_drop_collection(mongodb_conn, "mcp", col, NULL);
    mysql_result = mysql_store_result(&mysql_conn);
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if(6 != num_col)
    {
        printf(" colum count %d is err\n", num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    for(f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")
        SET_BSON_VALUE_STRING(mysql_row[1], "act_id")
        SET_BSON_VALUE_STRING(mysql_row[2], "award_rank")
        SET_BSON_VALUE_STRING(mysql_row[3], "award_info")
        SET_BSON_VALUE_INT(mysql_row[4], "total_items_cnt")
        SET_BSON_VALUE_INT(mysql_row[5], "hit_ratio")

        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
    }

    mysql_free_result(mysql_result);

    // [s] add by gwj for rename collection name
    rename_mongodb_collection(mongodb_conn, "mcp",
                              "mcp_activity_award_info", "mcp_activity_award_info_bak");
    // [e] add by gwj for rename collection name
    return 0;
}

int set_mcp_netgame_activity_info()
{
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;

    int f1, num_row;
    char query_sql[2048];
    memset(query_sql, 0x00, sizeof(query_sql));

    bson b[1];
    int num_col;
    char col[64];
    char ns[64];

    memset(col, 0x00, sizeof(col));
    memset(ns, 0x00, sizeof(ns));

    sprintf(col, "mcp_netgame_activity_bak");
    sprintf(ns, "mcp.mcp_netgame_activity_bak");

    sprintf(query_sql,
            "SELECT NGACT.id, NGACT.act_news_ids, NGACT.channel, "
            "NGACT.logdate, NGACT.title, NGACT.content, "
            "GROUP_CONCAT( DISTINCT PV.`image_type_id`, \",\", PV.`image_url` ORDER BY PV.`priority` DESC ) "
            "FROM mcp_netgame_activity_info AS NGACT "
            "LEFT JOIN mcp_netgame_activity_preview AS PV ON (PV.act_id = NGACT.id AND PV.enable = 1) "
            "WHERE NGACT.enable = 1 "
            "GROUP BY NGACT.id ");

    //printf("sql : [%s]\n", query_sql);

    if(mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mongo_cmd_drop_collection(mongodb_conn, "mcp", col, NULL);
    mysql_result = mysql_store_result(&mysql_conn);
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if(7 != num_col)
    {
        printf(" colum count %d is err\n", num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    for(f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")

        string value_string;

        if(IS_VALUED_STRING(mysql_row[1]))
        {
            value_string = mysql_row[1];
        }
        else
        {
            value_string = "";
        }

        CommonInterface::set_array_mongodb_value(b, "news_id", value_string);

        SET_BSON_VALUE_STRING(mysql_row[2], "channel")
        SET_BSON_VALUE_STRING(mysql_row[3], "logdate")
        SET_BSON_VALUE_STRING(mysql_row[4], "title")
        SET_BSON_VALUE_STRING(mysql_row[5], "content")

        string value_string_img;

        if(IS_VALUED_STRING(mysql_row[6]))
        {
            value_string_img = mysql_row[6];
        }
        else
        {
            value_string_img = "";
        }

        vector<const char*> columns;
        columns.push_back("img_type");
        columns.push_back("img_url");
        CommonInterface::set_struct_mongodb_value(b, "img_info", columns, value_string_img);

        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
    }

    mysql_free_result(mysql_result);

    // [s] add by gwj for rename collection name
    rename_mongodb_collection(mongodb_conn, "mcp",
                              "mcp_netgame_activity", "mcp_netgame_activity_bak");
    // [e] add by gwj for rename collection name
    return 0;
}
// [e] add by gwj for mcp_activity

// [s] add by gwj for app_download_info
int set_mcp_appInfo_data_info()
{
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;

    int f1, num_row;
    char query_sql[2048];
    memset(query_sql, 0x00, sizeof(query_sql));

    bson b[1];
    int num_col;
    char col[64];
    char ns[64];

    memset(col, 0x00, sizeof(col));
    memset(ns, 0x00, sizeof(ns));

    sprintf(col, "mcp_content_appinfo_data_bak");
    sprintf(ns, "mcp.mcp_content_appinfo_data_bak");

    sprintf(query_sql,
            "SELECT CADR.c_id, CAPI.id, CD.id, CAPI.app_name, CAPI.package_name, CAPI.version_code, "
            "CAPI.version, CAPI.sdk_version, CADR.version_upcode, CD.path_url, BDURL.to_baidu_url, CD.path_type, CD.size, CD.modify_date, CD.logdate, "
            "CD.enable, CD.MD5, CD.channel_info, CD.channel_id, CAPI.apk_md5 "
            "FROM mcp_content_appinfo_data_rel AS CADR "
            "LEFT JOIN mcp_content_appinfo AS CAPI ON CAPI.id = CADR.app_info_id  "
            "LEFT JOIN mcp_content_data AS CD ON CD.id = CADR.cdata_id "
            "LEFT JOIN mcp_duoku_to_baidu_url AS BDURL ON BDURL.cdata_id = CD.id "
            "ORDER BY CADR.c_id ");

    printf("sql : [%s]\n", query_sql);

    if(mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mongo_cmd_drop_collection(mongodb_conn, "mcp", col, NULL);
    mysql_result = mysql_store_result(&mysql_conn);
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if(20 != num_col)
    {
        printf(" colum count %d is err\n", num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    for(f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);

        bson_init(b);
        SET_BSON_VALUE_STRING(mysql_row[0], "c_id")
        SET_BSON_VALUE_STRING(mysql_row[1], "app_id")
        SET_BSON_VALUE_STRING(mysql_row[2], "cdata_id")
        SET_BSON_VALUE_STRING(mysql_row[3], "app_name")
        SET_BSON_VALUE_STRING(mysql_row[4], "package_name")
        SET_BSON_VALUE_STRING(mysql_row[5], "version_code")
        SET_BSON_VALUE_STRING(mysql_row[6], "version")
        SET_BSON_VALUE_STRING(mysql_row[7], "sdk_version")
        SET_BSON_VALUE_STRING(mysql_row[8], "version_upcode")
        SET_BSON_VALUE_STRING(mysql_row[9], "path_url")
        SET_BSON_VALUE_STRING(mysql_row[10], "to_baidu_url")
        SET_BSON_VALUE_STRING(mysql_row[11], "path_type")
        SET_BSON_VALUE_STRING(mysql_row[12], "size")
        SET_BSON_VALUE_STRING(mysql_row[13], "modify_date")
        SET_BSON_VALUE_STRING(mysql_row[14], "logdate")
        SET_BSON_VALUE_STRING(mysql_row[15], "enable")
        SET_BSON_VALUE_STRING(mysql_row[16], "MD5")
        SET_BSON_VALUE_STRING(mysql_row[17], "channel_info")
        SET_BSON_VALUE_STRING(mysql_row[18], "channel_id")
        SET_BSON_VALUE_STRING(mysql_row[19], "apk_md5")
        bson_finish(b);

        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
    }

    mysql_free_result(mysql_result);

    // [s] add by gwj for rename collection name
    rename_mongodb_collection(mongodb_conn, "mcp",
                              "mcp_content_appinfo_data", "mcp_content_appinfo_data_bak");
    // [e] add by gwj for rename collection name

    return 0;
}
// [e] add by gwj for app_download_info

// [s] add by zhengxie for mobile_music_info
int set_mb_music_content()
{
        MYSQL_RES *mysql_result;
        MYSQL_ROW mysql_row;
        int f1,  num_row ;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        bson b;
        int num_col;
        char col[64];
        char ns[64];
        memset( col, 0x00, sizeof(col));
        memset(ns,0x00,sizeof(ns));

        sprintf(col,"mb_music_content_bak");
        sprintf(ns,"mcp.mb_music_content_bak");
        sprintf(query_sql,
					"select * from mb_music_content ");

        //             printf("connection is ok\n");
        if (mysql_query(&mysql_conn, query_sql) != 0)
        {
                printf("query is not ok\n");
                return -1;
        }
        mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );
        mysql_result = mysql_store_result(&mysql_conn);
        num_row = mysql_num_rows(mysql_result);
        num_col = mysql_num_fields(mysql_result);
        if ( 9 != num_col )
        {
                printf(" colum count %d is err\n",num_col);
                mysql_free_result(mysql_result);
                return -1;
        }

		bson key[1];
        bson_init( key );
        bson_append_int( key, "ring_id", 1 );
        bson_finish( key);
        mongo_create_index( mongodb_conn, ns, key, 0, NULL );
        bson_destroy( key );


        for (f1 = 0; f1 < num_row; f1++)
        {
                mysql_row = mysql_fetch_row(mysql_result);
                bson_init( &b );

                bson_append_new_oid( &b, "_id" );
                bson_append_string( &b , "ring_id" , mysql_row[0] );
                bson_append_string( &b , "name" , mysql_row[1] );
                bson_append_string( &b , "singer" , mysql_row[2] );
                bson_append_string( &b , "bell_id" , mysql_row[3] );
                bson_append_string( &b , "song_id" , mysql_row[4] );
                bson_append_string( &b , "cp_id" , mysql_row[5] );
                bson_append_string( &b , "date" , mysql_row[6] );
                bson_append_string( &b , "dolby_flag" , mysql_row[7] );
                bson_append_string( &b , "status" , mysql_row[8] );

                bson_finish( &b );
                mongo_insert( mongodb_conn , ns , &b, NULL );
                bson_destroy( &b );
                //         printf(" one row insert to mongodb ok\n");
        }
        mysql_free_result(mysql_result);

        // [s] add by gwj for rename collection name
        rename_mongodb_collection(mongodb_conn, "mcp",
                                  "mb_music_content", "mb_music_content_bak");
        // [e] add by gwj for rename collection name

        return 0;
}
// [e] add by zhengxie for mobile_music_info

// add by zhengxie for 争霸赛
int set_mcp_content_championship()
{
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row;

    int f1, num_row;
    char query_sql[2048];
    memset(query_sql, 0x00, sizeof(query_sql));

    bson b[1];
    int num_col;
    char col[64];
    char ns[64];

    memset(col, 0x00, sizeof(col));
    memset(ns, 0x00, sizeof(ns));

    sprintf(col, "mcp_content_championship_bak");
    sprintf(ns, "mcp.mcp_content_championship_bak");

    time_t t;
    t=time(0);
    tm* vtm;
    vtm=localtime(&t);
    char str[32];
    memset(str,0x00, sizeof(str));
    sprintf( str, "%04d-%02d-%02d %02d:%02d:%02d",
            vtm->tm_year+1900, vtm->tm_mon+1, vtm->tm_mday, vtm->tm_hour, vtm->tm_min, vtm->tm_sec);
    string sdate(str);

    sprintf(query_sql,
            "SELECT CH.id, CH.name, CH.c_id, CH.log_date, CH.channel_info, CH.start_time, CH.end_time, CH.join_num, CH.award, CH.rule, CH.total_coins, CH.pic_url, CH.enable, CH.top_award "
            "from mcp_content_championship as CH where CH.enable = 1 and CH.end_time >'%s'", sdate.c_str());

	printf("sql : [%s]\n", query_sql);

    if(mysql_query(&mysql_conn, query_sql) != 0)
    {
        printf("query is not ok\n");
        return -1;
    }

    mongo_cmd_drop_collection(mongodb_conn, "mcp", col, NULL);
    mysql_result = mysql_store_result(&mysql_conn);
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if(14 != num_col)
    {
        printf(" colum count %d is err\n", num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

    for(f1 = 0; f1 < num_row; f1++)
    {
        mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")
        SET_BSON_VALUE_STRING(mysql_row[1], "name")
        SET_BSON_VALUE_STRING(mysql_row[2], "c_id")
        SET_BSON_VALUE_STRING(mysql_row[3], "log_date")
        SET_BSON_VALUE_STRING(mysql_row[4], "channel_info")
        SET_BSON_VALUE_STRING(mysql_row[5], "start_time")
        SET_BSON_VALUE_STRING(mysql_row[6], "end_time")
        SET_BSON_VALUE_STRING(mysql_row[7], "join_num")
        SET_BSON_VALUE_STRING(mysql_row[8], "award")
        SET_BSON_VALUE_STRING(mysql_row[9], "rule")
        SET_BSON_VALUE_STRING(mysql_row[10], "total_coins")
        SET_BSON_VALUE_STRING(mysql_row[11], "pic_url")
        SET_BSON_VALUE_STRING(mysql_row[12], "enable")
        SET_BSON_VALUE_STRING(mysql_row[13], "top_award")


        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
    }

    mysql_free_result(mysql_result);

    rename_mongodb_collection(mongodb_conn, "mcp",
                              "mcp_content_championship", "mcp_content_championship_bak");
    return 0;

}

int set_user_question_id()
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1, num_row;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	bson b[1];
	int num_col;
	int res;
	char col[64];
	char ns[64];
    memset( col, 0x00, sizeof(col));
    memset(ns,0x00,sizeof(ns));

	sprintf(col, "pt_game_user_QA_list_bak");
	sprintf(ns, "mcp.pt_game_user_QA_list_bak");
    mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );

    nowtime();
	sprintf(query_sql,
			"select user_id, GROUP_CONCAT(id ORDER BY id) from pt_game_QandA "
			"where enable=1 group by user_id "
			);
	printf("sql:%s", query_sql);
	if(mysql_query(&mysql_conn, query_sql) != 0)
	{
		printf("query is not ok\n");
		return -1;
	}

	mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if ( 2!= num_col )
    {
        printf(" colum count %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }
	for(f1 = 0; f1 < num_row; f1++)
	{
		mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "user_id")
        SET_BSON_VALUE_STRING(mysql_row[1], "question_id")
        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
	}
	mysql_free_result(mysql_result);
	rename_mongodb_collection(mongodb_conn, "mcp",
							  "pt_game_user_QA_list", "pt_game_user_QA_list_bak");
	return 0;
}

int set_user_question()
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1, num_row;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	bson b[1];
	int num_col;
	int res;
	char col[64];
	char ns[64];
    memset( col, 0x00, sizeof(col));
    memset(ns,0x00,sizeof(ns));

	sprintf(col, "pt_game_QandA_bak");
	sprintf(ns, "mcp.pt_game_QandA_bak");
    mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );

    nowtime();
	sprintf(query_sql,
			"select * from pt_game_QandA where enable=1 "
			);
	if(mysql_query(&mysql_conn, query_sql) != 0)
	{
		printf("query is not ok\n");
		return -1;
	}

	mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if ( 12!= num_col )
    {
        printf(" colum count %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }
	for(f1 = 0; f1 < num_row; f1++)
	{
		mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")
        SET_BSON_VALUE_STRING(mysql_row[1], "user_id")
        SET_BSON_VALUE_STRING(mysql_row[2], "appid")
        SET_BSON_VALUE_STRING(mysql_row[3], "question")
        SET_BSON_VALUE_STRING(mysql_row[4], "question_type")
        SET_BSON_VALUE_STRING(mysql_row[5], "answer")
        SET_BSON_VALUE_STRING(mysql_row[6], "phone")
        SET_BSON_VALUE_STRING(mysql_row[7], "status")
        SET_BSON_VALUE_STRING(mysql_row[8], "enable")
        SET_BSON_VALUE_STRING(mysql_row[9], "date_q")
        SET_BSON_VALUE_STRING(mysql_row[10], "date_a")
        SET_BSON_VALUE_STRING(mysql_row[11], "remarks")
        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
	}
	mysql_free_result(mysql_result);

	rename_mongodb_collection(mongodb_conn, "mcp",
							  "pt_game_QandA", "pt_game_QandA_bak");
	return 0;


}

int set_qp_activity_person()
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1, num_row;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	bson b[1];
	int num_col;
	int res;
	char col[64];
	char ns[64];
    memset( col, 0x00, sizeof(col));
    memset(ns,0x00,sizeof(ns));

	sprintf(col, "mcp_content_qp_activity_person_bak");
	sprintf(ns, "mcp.mcp_content_qp_activity_person_bak");
    mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );

    nowtime();
	sprintf(query_sql,
			"select id, c_id, num "
			"from mcp_content_qp_activity_person "
			);
	printf("sql:[%s]", query_sql);
	if(mysql_query(&mysql_conn, query_sql) != 0)
	{
		printf("query is not ok\n");
		return -1;
	}

	mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

	bson key[1];

	bson_init( key );
	bson_append_int( key, "c_id", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

    if ( 3!= num_col )
    {
        printf(" colum count %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }
	for(f1 = 0; f1 < num_row; f1++)
	{
		mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")
        SET_BSON_VALUE_STRING(mysql_row[1], "c_id")
        SET_BSON_VALUE_STRING(mysql_row[2], "num")
        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
	}
	mysql_free_result(mysql_result);

	rename_mongodb_collection(mongodb_conn, "mcp",
							  "mcp_content_qp_activity_person", "mcp_content_qp_activity_person_bak");
	return 0;
}

int set_qp_activity()
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1, num_row;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	bson b[1];
	int num_col;
	int res;
	char col[64];
	char ns[64];
    memset( col, 0x00, sizeof(col));
    memset(ns,0x00,sizeof(ns));

	sprintf(col, "mcp_content_qp_activity_bak");
	sprintf(ns, "mcp.mcp_content_qp_activity_bak");
    mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );

    nowtime();
	sprintf(query_sql,
			"select Q.id, Q.cid, Q.appid, Q.title, Q.info, Q.rules, Q.start_time, Q.end_time, Q.icon_url, Q.status, Q.enable, Q.act_info, Q.weight, Q.is_top "
			"from mcp_content_qp_activity as Q, mcp_content as C where Q.enable=1 and Q.cid=C.id and C.enable=1 "
			);
	printf("sql:[%s]", query_sql);
	if(mysql_query(&mysql_conn, query_sql) != 0)
	{
		printf("query is not ok\n");
		return -1;
	}

	mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

    if ( 14!= num_col )
    {
        printf(" colum count %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }

	bson key[1];

	// 建立 ID 索引
	bson_init( key );
	bson_append_int( key, "id", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

	bson_init( key );
	bson_append_int( key, "enable", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

	bson_init( key );
	bson_append_int( key, "is_top", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

	bson_init( key );
	bson_append_int( key, "weight", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

	bson_init( key );
	bson_append_int( key, "end_time", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

	for(f1 = 0; f1 < num_row; f1++)
	{
		mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")
        SET_BSON_VALUE_STRING(mysql_row[1], "cid")
        SET_BSON_VALUE_STRING(mysql_row[2], "appid")
        SET_BSON_VALUE_STRING(mysql_row[3], "title")
        SET_BSON_VALUE_STRING(mysql_row[4], "info")
        SET_BSON_VALUE_STRING(mysql_row[5], "rules")
        SET_BSON_VALUE_STRING(mysql_row[6], "start_time")
        SET_BSON_VALUE_STRING(mysql_row[7], "end_time")
        SET_BSON_VALUE_STRING(mysql_row[8], "icon_url")
        SET_BSON_VALUE_STRING(mysql_row[9], "status")
        SET_BSON_VALUE_STRING(mysql_row[10], "enable")
        SET_BSON_VALUE_STRING(mysql_row[11], "act_info")
        SET_BSON_VALUE_INT(mysql_row[12], "weight")
        SET_BSON_VALUE_STRING(mysql_row[13], "is_top")

        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
	}
	mysql_free_result(mysql_result);

	rename_mongodb_collection(mongodb_conn, "mcp",
							  "mcp_content_qp_activity", "mcp_content_qp_activity_bak");
	return 0;


}

int set_person_award()
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1, num_row;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	bson b[1];
	int num_col;
	int res;
	char col[64];
	char ns[64];
    memset( col, 0x00, sizeof(col));
    memset(ns,0x00,sizeof(ns));

	sprintf(col, "mcp_content_person_award_bak");
	sprintf(ns, "mcp.mcp_content_person_award_bak");
    mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );

    nowtime();
	sprintf(query_sql,
			"select U.id, U.user_id, U.commodity_id, U.kudou, U.priority, U.date, P.name, P.icon, U.award_channel "
			"from props_user as U, props_commodity as P "
		    "where U.commodity_id = P.id"
			);
	printf("sql:[%s]", query_sql);
	if(mysql_query(&mysql_conn, query_sql) != 0)
	{
		printf("query is not ok\n");
		return -1;
	}

	mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

	bson key[1];

	bson_init( key );
	bson_append_int( key, "user_id", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

	bson_init( key );
	bson_append_int( key, "priority", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

    if ( 9!= num_col )
    {
        printf(" colum count %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }
	for(f1 = 0; f1 < num_row; f1++)
	{
		mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")
        SET_BSON_VALUE_STRING(mysql_row[1], "user_id")
        SET_BSON_VALUE_STRING(mysql_row[2], "commodity_id")
        SET_BSON_VALUE_INT(mysql_row[3], "kudou")
        SET_BSON_VALUE_INT(mysql_row[4], "priority")
        SET_BSON_VALUE_STRING(mysql_row[5], "date")
        SET_BSON_VALUE_STRING(mysql_row[6], "name")
        SET_BSON_VALUE_STRING(mysql_row[7], "icon")
        SET_BSON_VALUE_STRING(mysql_row[8], "type")
        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
	}
	mysql_free_result(mysql_result);

	rename_mongodb_collection(mongodb_conn, "mcp",
							  "mcp_content_person_award", "mcp_content_person_award_bak");
	return 0;
}

int set_qp_activity_award()
{
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	int f1, num_row;
	char query_sql[2048];
	memset(query_sql, 0x00, sizeof(query_sql));
	bson b[1];
	int num_col;
	int res;
	char col[64];
	char ns[64];
    memset( col, 0x00, sizeof(col));
    memset(ns,0x00,sizeof(ns));

	sprintf(col, "mcp_content_qp_activity_award_bak");
	sprintf(ns, "mcp.mcp_content_qp_activity_award_bak");
    mongo_cmd_drop_collection( mongodb_conn, "mcp", col, NULL );

    nowtime();
	sprintf(query_sql,
			"select * from mcp_content_qp_activity_award where enable=1 "
			);
	printf("sql:[%s]", query_sql);
	if(mysql_query(&mysql_conn, query_sql) != 0)
	{
		printf("query is not ok\n");
		return -1;
	}

	mysql_result = mysql_store_result(&mysql_conn);
    if(mysql_result==NULL){
        printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
        return 0;
    }
    num_row = mysql_num_rows(mysql_result);
    num_col = mysql_num_fields(mysql_result);

	bson key[1];

	bson_init( key );
	bson_append_int( key, "id", 1 );
	bson_finish( key);
	mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	bson_destroy( key );

	/*bson_init( key );*/
	//bson_append_int( key, "priority", 1 );
	//bson_finish( key);
	//mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	//bson_destroy( key );

	//bson_init( key );
	//bson_append_int( key, "user_id", 1 );
	//bson_finish( key);
	//mongo_create_index( mongodb_conn, ns, key, 0, NULL );
	//bson_destroy( key );

    if ( 6!= num_col )
    {
        printf(" colum count %d is err\n",num_col);
        mysql_free_result(mysql_result);
        return -1;
    }
	for(f1 = 0; f1 < num_row; f1++)
	{
		mysql_row = mysql_fetch_row(mysql_result);
        bson_init(b);

        SET_BSON_VALUE_STRING(mysql_row[0], "id")
        SET_BSON_VALUE_STRING(mysql_row[1], "name")
        SET_BSON_VALUE_STRING(mysql_row[2], "activity_id")
        SET_BSON_VALUE_STRING(mysql_row[3], "pic_url")
        SET_BSON_VALUE_STRING(mysql_row[4], "enable")
        SET_BSON_VALUE_STRING(mysql_row[5], "create_time")
        bson_finish(b);
        mongo_insert(mongodb_conn, ns, b, NULL);
        bson_destroy(b);
	}
	mysql_free_result(mysql_result);

	rename_mongodb_collection(mongodb_conn, "mcp",
							  "mcp_content_qp_activity_award", "mcp_content_qp_activity_award_bak");
	return 0;
}

void bson_append_ptype_info(bson* b, string& ptype1)
{
    char index[8];
    int j = 0;
    size_t pos;
    string ptype_list = ptype1;

    string ptype,ptype6;
    int priority,priority6=0;
    int tmp;

    size_t pos1(0), pos2(0);
    while( !ptype_list.empty() && j<=5 )
    {
        pos1=ptype_list.find(',');
        if( pos1 != string::npos )
        {
            ptype = ptype_list.substr( 0, pos1 );
            ptype_list.erase( 0, pos1+1 );
        }
        else
        {
            break;
        }

        pos2=ptype_list.find(',');
        priority = atoi(ptype_list.substr(0,pos2).c_str());
        ptype_list.erase( 0, pos2+1 );

        if(j == 5 && !ptype_list.empty())
        {
            pos1=ptype_list.find(',');
            if( pos1 != string::npos )
            {
                ptype6 = ptype_list.substr( 0, pos1 );
                ptype_list.erase( 0, pos1+1 );
            }
            else
            {
                goto out;
            }

            pos2=ptype_list.find(',');
            priority6 = atoi(ptype_list.substr(0,pos2).c_str());
            //                                printf("priority 6 is %d", priority6);
            ptype_list.erase( 0, pos2+1 );
        }
out:
        tmp = atoi(ptype.c_str());
        if( tmp < j )
        {
            continue;
        }
        else if( tmp == j )
        {
            memset( index, 0x00, sizeof(index) );
            sprintf( index, "ptype_%d", j );
            if(j == 5)
            {
                priority += priority6;
                //printf("priority is %d", priority);
            }
            bson_append_int( b, index, priority );
            // printf( "%s:%d ", index, priority );
        }
        else
        {
            while(j<tmp)
            {
                memset( index, 0x00, sizeof(index) );
                sprintf( index, "ptype_%d", j );
                bson_append_int( b, index, 0 );
                //   printf( "%s:0 ", index );
                ++j;
            }
            memset( index, 0x00, sizeof(index) );
            sprintf( index, "ptype_%d", j );
            if(j == 5)
            {
                priority += priority6;
                //printf("priority is %d", priority);
            }
            bson_append_int( b, index, priority );
            // printf( "%s:%d ", index, priority );
        }
        ++j;
    }
    while( j<= 5 )
    {
        memset( index, 0x00, sizeof(index) );
        sprintf( index, "ptype_%d", j );
        //printf( "%s:0 ", index );

        // [s] mod by gwj for random download
        //bson_append_int( &b, index, 0 );
        int random_dl_cnt = get_random_number(500, 1000);
        //bson_append_int( b, index, random_dl_cnt );
        bson_append_int( b, index, random_dl_cnt );
        // [s] mod by gwj for random download

        ++j;
    }
}

void bson_append_lable_info(bson* b, string& lable_list1)
{
    bson_append_start_array( b , "label_id" );
    int j = 0;
    size_t pos(0);
    string label_id, label_list = lable_list1;
    char index[16];
    while( !label_list.empty() )
    {
        pos = label_list.find(',');
        label_id = label_list.substr( 0, pos );
        //printf( "label_list:[%s], label_id:[%s]\n", label_list.c_str(), label_id.c_str() );
        if( pos != string::npos )
        {
            label_list.erase( 0, pos+1 );
        }
        else
        {
            label_list.clear();
        }
        memset(index, 0x00, sizeof(index));
        sprintf( index, "%d", j );
        bson_append_string( b, index, label_id.c_str() );
        ++j;
    }
    bson_append_finish_object( b );
}

void bson_append_img_info2(bson* b, string& img_info_list)
{
    bson_append_start_array( b , "img_info" );
    int j = 0;
    string img_type, img_url;
    size_t pos1, pos2;
    char index[16];
    //printf( "img_info_list:[%s]\n", img_info_list.c_str() );
    while( !img_info_list.empty() )
    {
        pos1 = img_info_list.find(',');
        img_type = img_info_list.substr( 0, pos1 );
        if( pos1 != string::npos )
        {
            img_info_list.erase( 0, pos1+1 );
        }
        else
        {
            img_info_list.clear();
            break;
        }
        pos2 = img_info_list.find(',');
        img_url = img_info_list.substr( 0, pos2 );
        if( pos2 !=  string::npos )
        {
            img_info_list.erase( 0, pos2+1 );
        }
        else
        {
            img_info_list.clear();
        }
        memset(index, 0x00, sizeof(index));
        sprintf( index, "%d", j );
        bson_append_start_object( b, index );
        bson_append_string( b, "img_type", img_type.c_str() );
        bson_append_string( b, "img_url", img_url.c_str() );
        bson_append_finish_object( b );
        //printf( "{img_type:%s, img_url:%s} ", img_type.c_str(), img_url.c_str() );
        ++j;
    }
    bson_append_finish_array( b );
}

double getStarFromRedis(redisContext* &c, char* c_id)
{
    redisReply *reply = NULL;
    double star = 0.0;
    reply = (redisReply*)redisCommand( c, "GET star:%s", c_id );
    if(reply != NULL)
    {
        if(!IS_REDIS_REPLY_ERROR(reply->type))
        {
            if(IS_VALUED_STRING(reply->str))
            {
                star = atof(reply->str);
            }

        }
    }
    FREE_REDIS_REPLY(reply);
    return star;
}

