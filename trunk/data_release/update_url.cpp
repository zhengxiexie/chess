#include <stdio.h>
#include <string.h>
#include "mongo.h"
#include <mysql.h>
#include <map>
#include <string>
#include "common.h"
using namespace std;

//#define MONGODB_HOST "10.40.22.14"
#define MONGODB_PORT 27020
#define NG -1
#define OK 0

MYSQL mysql_conn;
mongo mongodb_conn[1];
set<string> s_cids;

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
     if (mysql_real_connect(&mysql_conn, "10.10.0.9", "root", "duoku2012", "MCP", 4051, NULL, 0) ==  NULL)
     {
         printf("mysql connection is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }
     mysql_set_character_set(&mysql_conn,"utf8");

     ret = set_mcp_url_content();
     if ( 0 != ret)
     {
         printf("set_mcp_url_content is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }

     mysql_close(&mysql_conn);
     mongo_destroy( mongodb_conn );
     printf("update is ok\n");

     return 0;

}

