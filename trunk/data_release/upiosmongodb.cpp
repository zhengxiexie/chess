#include <stdio.h>
#include <string.h>
#include "mongo.h"
#include "mysql.h"
#include <map>
#include <set>
#include <string>
#include "common.h"
using namespace std;

MYSQL mysql_conn;
mongo mongodb_conn[1];
set<string > s_cids;
time_t date_to_int(char *date);

int main(int argc, char *argv[])
{
    int ret;
     
    int mongodb_port;
    char mongodb_host[16];
    memset(mongodb_host, 0x00, sizeof(mongodb_host));
    
    if ( strlen(argv[1]) > 16 )
    {
        printf("parameter1 is too long\n");
    }
    if ( strlen(argv[2]) > 6 )
    {
        printf("parameter2 is too long\n");
    }
    strcpy(mongodb_host, argv[1]);
    mongodb_port = atoi(argv[2]);

    printf("mongodb_host is %s\n", mongodb_host);
    printf("mongodb_port is %d\n", mongodb_port);

     if( mongo_connect( mongodb_conn , mongodb_host, mongodb_port  ) != MONGO_OK )
     {
        printf( "mongodb connect is ng\n" );
        return(-1 );
     }

    if (mysql_init(&mysql_conn) == NULL)
    {
         printf("init is ng\n");
         mysql_close(&mysql_conn);
         return -1;
    }

    if (mysql_real_connect(&mysql_conn, argv[3], argv[5], argv[6],argv[7], atoi(argv[4]), NULL, 0) ==    NULL)
    {
        printf("mysql connection is ng\n");
        mysql_close(&mysql_conn);
        return -1;
    }

    mysql_set_character_set(&mysql_conn,"utf8");

	
	printf("set_mcp_ios_content start \n ");
    ret=set_mcp_ios_content();
    if(0!=ret){
        printf("set_mcp_ios_content is ng\n");
        mysql_close(&mysql_conn);
        return -1;
    }


    printf("set_mcp_content_column_list start \n ");
    ret = set_mcp_content_column_list();
    if ( 0 != ret)
    {
        printf("set_mcp_content_column_list is ng\n");
        mysql_close(&mysql_conn);
        return -1;
    }

    printf("set_mcp_content_class_list start \n ");
    ret = set_mcp_content_class_list();
    if ( 0 != ret)
    {
        printf("set_mcp_content_class_list is ng\n");
        mysql_close(&mysql_conn);
        return -1;
    }

    nowtime();
     
    mysql_close(&mysql_conn);
    mongo_destroy( mongodb_conn );
    printf("update is ok\n");

    return 0;
}
