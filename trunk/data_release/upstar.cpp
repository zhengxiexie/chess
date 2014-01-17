#include "common.h"
#include "redis_connpool.h"
#include <stdio.h>
#include <string.h>
#include <set>
using namespace std;

#define MONGODB_PORT 27020
#define NG -1
#define OK 0

MYSQL mysql_conn;
mongo mongodb_conn[1];

set<string> s_cids;

int main(int argc, char *argv[])
{
	if (mysql_init(&mysql_conn) == NULL)
    {
         printf("init is ng\n");
         mysql_close(&mysql_conn);
         return -1;
    }

    if (mysql_real_connect(&mysql_conn, argv[1], argv[3], argv[4],argv[5], atoi(argv[2]), NULL, 0) == NULL)
    {
        printf("mysql connection is ng\n");
        mysql_close(&mysql_conn);
        return -1;
    }

    mysql_set_character_set(&mysql_conn,"utf8");

	redisContext *c = redisConnPool::getInstance()->getConnection();

	printf("get_channel_info start \n ");
	int ret = get_channel_info2();
	if ( 0 != ret)
	{
		printf("get_channel_info is ng\n");
		mysql_close(&mysql_conn);
		return -1;
	}

	//nowtime();
	printf("updatestar start \n ");
    if(c==NULL){
        printf( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
        return -1;
    }
	
    redisReply *reply=NULL;

	set<string>::iterator it;
    for(it=s_cids.begin(); it!=s_cids.end(); it++)
	{
		string c_id = (*it);
		double star = get_star_by_c_id(c_id.c_str());
		if( 0.0==star ){
			printf("c_id:%s star is null\n", c_id.c_str());
			continue;
		}
		reply = (redisReply*)redisCommand( c, "SET star:%s %f", c_id.c_str(), star );
		printf("set star:%s %f\n", c_id.c_str(), star);
		freeReplyObject(reply);

		/* add by zhengxie 将计算出的星级插入数据库 beg*/

		MYSQL_RES *mysql_result;
		MYSQL_ROW mysql_row;
		char query_sql[2048];
		memset(query_sql, 0x00, sizeof(query_sql));
		int num_col, num_row;

		sprintf(query_sql,
				"select star_flag from mcp_content_game_ext where `c_id`=%s", c_id.c_str());
		if (mysql_query(&mysql_conn, query_sql) != 0)
		{
				printf("query is not ok\n");
				return -1;
		}
		mysql_result = mysql_store_result(&mysql_conn);
		if(mysql_result==NULL){
				printf("%d:%s\n", mysql_errno(&mysql_conn), mysql_error(&mysql_conn));
				return -1;
		}
		num_row = mysql_num_rows(mysql_result);
		num_col = mysql_num_fields(mysql_result);
		if ( 1 != num_col )
		{
				printf(" colum count %d is err\n",num_col);
				mysql_free_result(mysql_result);
				return -1;
		}
		if( num_row == 0)
		{
			continue;
		}

		string star_flag;
		mysql_row = mysql_fetch_row(mysql_result);
		star_flag = mysql_row[0];
		if(!strcmp(star_flag.c_str(), "0"))
		{
			sprintf(query_sql,
				"update mcp_content_game_ext set `star`=%lf where `c_id`=%s", star, c_id.c_str());
			if (mysql_query(&mysql_conn, query_sql) != 0)
			{
					printf("update query is not ok\n");
					return -1;
			}
			printf("update c_id:%s'star query is ok\n", c_id.c_str());
		}

	}
		/* add by zhengxie 将计算出的星级插入数据库 end*/

    redisConnPool::getInstance()->releaseConnection(c);
    return 0;
}
