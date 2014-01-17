#include <stdio.h>
#include <string.h>
#include "mongo.h"
#include "mysql.h"
#include <map>
#include <set>
#include <string>
#include "common.h"
using namespace std;

//#define MONGODB_HOST "10.40.22.14"
//#define MONGODB_PORT 27020
#define NG -1
#define OK 0

MYSQL mysql_conn;
mongo mongodb_conn[1];

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


int main(int argc, char *argv[])
{
    if(argc < 8)
    {
        printf("argc < 8, input the params like this ./syncBaiduAppInfo2Mongodb 10.10.0.218 27020 10.10.0.137 3306 read 1syhl9t MCP\n");
        exit(0);
    }
    int mongodb_port;
    char mongodb_host[16];
    memset(mongodb_host, 0x00, sizeof(mongodb_host));
    
    strcpy(mongodb_host, argv[1]);
    mongodb_port = atoi(argv[2]);
    //strcpy(mongodb_host, "10.10.0.218");
    //mongodb_port = atoi("27020");

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
     //if (mysql_real_connect(&mysql_conn, "10.10.0.141", "root", "123456", "statusxkdb", 3306, NULL, 0) ==    NULL)
     if (mysql_real_connect(&mysql_conn, argv[3], argv[5], argv[6],argv[7], atoi(argv[4]), NULL, 0) ==    NULL)
     {
         printf("mysql connection is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }
     mysql_set_character_set(&mysql_conn,"utf8");
     MYSQL_RES *mysql_result;
     MYSQL_ROW mysql_row;
     int f1,  num_row ;
     char query_sql[1024];
     memset(query_sql, 0x00, sizeof(query_sql));
     bson b;
     int num_col;
     char colPreName[64];
     char colNormalName[64];
     memset( colPreName, 0x00, sizeof(colPreName));
     memset(colNormalName,0x00,sizeof(colNormalName));

     sprintf(colPreName,"mcp.mcp_content_baiduAppInfoPredecessor");
     sprintf(colNormalName,"mcp_content_baiduAppInfo");
     sprintf(query_sql,"select id,apkName,packageformat,versioncode,language,cateid,fee,description,packagesize,platform,platform_version,docid,versionname,releasedate,catename,type,icon,sname,url,site,bigmaplink1,smallmaplink1,bigmaplink2,smallmaplink2,all_download,score,sourcename,insertDate,screenshot from BaiduAndroidInfo where enable = 1");
     //id,baidu_path_url,path_url,path_type(1),media_format_id(3),size,mtype(apk),charge_typeid(6),c_id,value,modify_date,adapt_info(stype,stype_id==array),channel_info,channel_id,package_name,int_version,string_version,sdk_version,permissions,features,apk_info,apk_md5,file_md5 ======>mcp_content_data
     //_id,adapt,apk_md5,class(id,p ==array),class_weight,client_class_weight,client_download_weight,client_latest_weight,client_pop_weight,column,content_type,cp_id,default_charge_typeid,default_charge_value,download_weight,editor_recom,enName,ext_id,game_code,grant_id,has_account,id,img_info(img_type,img_url==array),info,label_id,language,latest_weight,logdate,method,modify_date,name,network_id,package_name,pop_weight
     if (mysql_query(&mysql_conn, query_sql) != 0)
     {
             printf("query is not ok\n");
             return -1;
     }
     mongo_cmd_drop_collection( mongodb_conn, "mcp", colPreName, NULL );
     mysql_result = mysql_store_result(&mysql_conn);
     num_row = mysql_num_rows(mysql_result);
     num_col = mysql_num_fields(mysql_result);

     bson keyIndex[1];
     //id索引
     bson_init( keyIndex );
     bson_append_int( keyIndex, "id", 10000000);
     bson_finish( keyIndex);
     mongo_create_index( mongodb_conn, colPreName, keyIndex, 0, NULL );
     bson_destroy(keyIndex);

    //apkName索引
     bson_init( keyIndex );
     bson_append_string( keyIndex, "apkName", "baiduApkName");
     bson_finish( keyIndex);
     mongo_create_index( mongodb_conn, colPreName, keyIndex, 0, NULL );
     bson_destroy(keyIndex);


    for (f1 = 0; f1 < num_row; f1++)
    {
            printf("BaiduAndroidInfo data %d \n",f1);
            mysql_row = mysql_fetch_row(mysql_result);
            if(num_col != 29)
            {
                printf("column number is not correct\n");
                mysql_free_result(mysql_result);
                exit(-1);
            }

            bson_init( &b );
            bson_append_new_oid( &b, "_id" );
            bson_append_int( &b , "id" , atol(mysql_row[0]));
            bson_append_string( &b , "apkName" , mysql_row[1] );
            bson_append_string( &b , "packageformat" , mysql_row[2]);
            bson_append_string( &b , "versioncode" , mysql_row[3]);
            bson_append_string( &b , "language" , mysql_row[4]);
            bson_append_string( &b , "cateid" , mysql_row[5]);
            bson_append_string( &b , "fee" , mysql_row[6]);
            bson_append_string( &b , "description" , mysql_row[7]);
            bson_append_string( &b , "packagesize" , mysql_row[8]);
            bson_append_string( &b , "platform" , mysql_row[9]);
            bson_append_string( &b , "platform_version" , mysql_row[10]);
            bson_append_string( &b , "docid" , mysql_row[11]);
            bson_append_string( &b , "versionname" , mysql_row[12]);
            bson_append_string( &b , "releasedate" , mysql_row[13]);
            bson_append_string( &b , "catename" , mysql_row[14]);
            bson_append_string( &b , "type" , mysql_row[15]);
            bson_append_string( &b , "icon" , mysql_row[16]);
            bson_append_string( &b , "sname" , mysql_row[17]);
            bson_append_string( &b , "url" , mysql_row[18]);
            bson_append_string( &b , "site" , mysql_row[19]);
            bson_append_string( &b , "bigmaplink1" , mysql_row[20]);
            bson_append_string( &b , "smallmaplink1" , mysql_row[21]);
            bson_append_string( &b , "bigmaplink2" , mysql_row[22]);
            bson_append_string( &b , "smallmaplink2" , mysql_row[23]);
            bson_append_string( &b , "all_download" , mysql_row[24]);
            bson_append_string( &b , "score" , mysql_row[25]);
            bson_append_string( &b , "sourcename" , mysql_row[26]);
            bson_append_string( &b , "insertDate" , mysql_row[27]);
            bson_append_string( &b , "screenshot" , mysql_row[28]);
    
            bson_finish( &b );
            mongo_insert( mongodb_conn , colPreName , &b, NULL );
            bson_destroy( &b );
            //printf("colPreName=%s,insertDate=%s,colNormalName=%s\n",colPreName,mysql_row[27],colNormalName);
    }
    sprintf(colPreName,"mcp_content_baiduAppInfoPredecessor");
    sprintf(colNormalName,"mcp_content_baiduAppInfo");
    mysql_free_result(mysql_result);
    rename_mongodb_collection(mongodb_conn, "mcp", colNormalName,colPreName);

	mysql_close(&mysql_conn);
	mongo_destroy( mongodb_conn );
    printf("update is ok\n");

    return 0;
}


