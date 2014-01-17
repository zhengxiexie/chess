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
    
//    mongodb_port = MONGODB_PORT;
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

//    strcpy(mongodb_host,MONGODB_HOST );
    printf("mongodb_host is %s\n", mongodb_host);
    printf("mongodb_port is %d\n", mongodb_port);

     if( mongo_connect( mongodb_conn , mongodb_host, mongodb_port  ) != MONGO_OK )
     {
        printf( "mongodb connect is ng\n" );
        return(-1 );
     }

     /*
     if( mongo_cmd_authenticate( mongodb_conn, "mcp", "mcp", "1q2w3e4r" ) != MONGO_OK )
     {
         printf( "check authenticate ng" ); 
         return -1;
     }
     */


     if (mysql_init(&mysql_conn) == NULL)
     {
          printf("init is ng\n");
          mysql_close(&mysql_conn);
          return -1;
     }
//         printf("init is ok\n"); 
    // if (mysql_real_connect(&mysql_conn, "10.40.27.57", "mi_yl_mcp_w", "3i9s8f4A0E7e", "MCP", 4018, NULL, 0) ==    NULL)
    if (mysql_real_connect(&mysql_conn, argv[3], argv[5], argv[6],argv[7], atoi(argv[4]), NULL, 0) ==    NULL)
//     if (mysql_real_connect(&mysql_conn, "10.23.245.118", "root", "1q2w3e4r", "MCP", MYSQL_PORT, NULL, 0) ==    NULL)
//     if (mysql_real_connect(&mysql_conn, "10.23.245.155", "root", "root", "MCP", MYSQL_PORT, NULL, 0) ==    NULL)
//     if (mysql_real_connect(&mysql_conn, "10.40.27.57", "yl_mcp_front_w", "3i9s8f4A0E7e", "MCP", 4018, NULL, 0) ==    NULL)
     {
         printf("mysql connection is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }
//     printf("connect mysql is ok \n");
//     return 1;
     mysql_set_character_set(&mysql_conn,"utf8");
//         mysql_set_character_set(&mysql_conn,"utf8");

	nowtime();
	printf("set_qp_activity start \n ");
	ret = set_qp_activity();
     if ( 0 != ret)
     {
         printf("set_qp_activity is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }

	nowtime();
	printf("set_qp_activity_award start \n ");
	ret = set_qp_activity_award();
     if ( 0 != ret)
     {
         printf("set_qp_activity_award is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }

	nowtime();
	printf("set_person_award start \n ");
	ret = set_person_award();
     if ( 0 != ret)
     {
         printf("set_person_award is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }

	nowtime();
	printf("set_qp_activity_person start \n ");
	ret = set_qp_activity_person();
     if ( 0 != ret)
     {
         printf("set_qp_activity_person is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }
	//add by zhengxie
	nowtime();
	printf("set_user_question_id start \n ");
	ret = set_user_question_id();
     if ( 0 != ret)
     {
         printf("set_user_question_id is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }

	nowtime();
	printf("set_user_question start \n ");
	ret = set_user_question();
     if ( 0 != ret)
     {
         printf("set_user_question is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }

	//add by zhengxie
    nowtime();
    printf("get_pub_prop_info start \n ");
	ret = get_pub_prop_info();
     if ( 0 != ret)
     {
         printf("get_pub_prop_info is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }


     nowtime();
      printf("get_channel_info start \n ");
     ret = get_channel_info();
     if ( 0 != ret)
     {
         printf("get_channel_info is ng\n");
         mysql_close(&mysql_conn);
         return -1;
     }

	  nowtime();
	  printf("set_mcp_game_content_new start \n ");
	 ret = set_mcp_game_content_new();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_game_content_new is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }
	 
	  nowtime();
	  printf("set_mcp_theme_content start \n ");
	 ret = set_mcp_theme_content();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_theme_content is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }
	 
	  nowtime();
	  printf("set_mcp_content_format start \n ");
	 ret = set_mcp_content_format();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_format is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }
	 
	  nowtime();
	  printf("set_mcp_content_class start \n ");
	 ret = set_mcp_content_class();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_class is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_data duobao start \n ");

	  // 先同步 android 多包
	 ret = set_mcp_content_data();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_data channel_info is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	 nowtime();
	 printf("set_mcp_content_data kjava start \n ");

	 // 后同步 kjava
	 ret = set_mcp_content_data_kjava();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_data kjava is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	  }
	// [s] add by zhengxie for 增量更新
	nowtime();
	printf("set mcp_diff_update_info start \n ");
	ret = set_mcp_diff_update_info();
	if(0!=ret)
	{
		printf("set mcp_diff_update_info failed \n ");
		mysql_close(&mysql_conn);
		return -1;
	}
	// [e] add by zhengxie for 增量更新
	
	// [s] add by zhengxie for 争霸赛
	nowtime();
	printf("set mcp_content_championship start \n ");
	ret = set_mcp_content_championship();
	if(0!=ret)
	{
		printf("set mcp_content_championship failed \n ");
		mysql_close(&mysql_conn);
		return -1;
	}
	// [e] add by zhengxie for 争霸赛


	  nowtime();
	  printf("set_mcp_content_diy_column start \n ");
	 ret = set_mcp_content_diy_column();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_diy_column is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_column_list start \n ");
	  ret = set_mcp_content_column_list();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_column_list is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_cp start \n ");
	  
	 ret = set_mcp_content_cp();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_cp is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_grant start \n ");
	  
	 ret = set_mcp_content_grant();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_grant is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_preview start \n ");
	  
	 ret = set_mcp_content_preview();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_preview is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_rank start \n ");
	  
	 ret = set_mcp_content_rank();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_rank is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_phone_info start \n ");
	  
	 ret = set_mcp_phone_info();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_phone_info is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_type_list start \n ");
	  
	 ret = set_mcp_type_list();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_type_list is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_chargetype start \n ");
	  
	 ret = set_mcp_content_chargetype();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_chargetype is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_class_list start \n ");
	  
	 ret = set_mcp_content_class_list();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_class_list is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_url_content start \n ");
	  
	 ret = set_mcp_url_content();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_url_content is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_bookmark start \n ");
	  
	 ret = set_mcp_content_bookmark();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_bookmark is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_bookmark_list start \n ");
	  
	 ret = set_mcp_content_bookmark_list();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_content_bookmark_list is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }
 
	  nowtime();
	  printf("set_mcp_class_adapt_count start \n ");
		  
	 ret = set_mcp_class_adapt_count();
	 if ( 0 != ret)
	 {
		 printf("set_mcp_class_adapt_count is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_software_content start \n ");
	  
	 ret = set_mcp_software_content();
	 if ( 0 != ret)
	 {
		  printf("set_mcp_software_content is ng\n");
		  mysql_close(&mysql_conn);
		  return -1;
	 }

	  nowtime();
	  printf("set_mcp_android_theme start \n ");
	  
	 ret=set_mcp_android_theme();
	 if(0!=ret){
		 printf("set_mcp_android_theme is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }


	  nowtime();
	  printf("set_mcp_ios_content start \n ");

	 ret=set_mcp_ios_content();
	 if(0!=ret){
		 printf("set_mcp_ios_content is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_content_topic start \n ");
	  
	 ret = set_mcp_content_topic();
	 if(0!=ret)
	 {
		 printf("set_mcp_content_topic is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_topic_col_map start \n ");
	  
	 ret = set_mcp_topic_col_map();
	 if(0!=ret)
	 {
		 printf("set_mcp_topic_col_map is ng\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	  nowtime();
	  printf("set_mcp_class_class_map start \n ");
	  
	 ret=set_mcp_class_class_map();
	 if(0!=ret)
	 {
	 printf("set_mcp_class_class_map failed\n");
	 mysql_close(&mysql_conn);
		 return -1;
	 }

	 nowtime();
	 printf("set_mcp_content_news start \n ");
	 ret = set_mcp_content_news();

	 if(0!=ret)
	 {
		 printf("set_mcp_content_news failed\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	nowtime();
	printf("set_mcp_content_empty_game_news start \n ");
	ret = set_mcp_content_empty_game_news();

	if(0!=ret)
	{
		printf("set_mcp_content_empty_game_news failed\n");
		mysql_close(&mysql_conn);
		return -1;
	}

	 nowtime();
	 printf("set_mcp_appinfo start \n ");
	 ret = set_mcp_appinfo();

	 if(0!=ret)
	 {
		 printf("set_mcp_appinfo failed\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

    nowtime();
	 printf("set_mcp_online_game start \n ");
	 ret = set_mcp_online_game();

	 if(0!=ret)
	 {
		 printf("set_mcp_online_game failed\n");
		 mysql_close(&mysql_conn);
		 return -1;
	 }

	 nowtime();

	printf("set_mcp_mixed_topic_music start \n ");
	ret = set_mcp_mixed_topic_music();
	if(0!=ret)
	{
		printf("set_mixed_topic_music failed\n");
		mysql_close(&mysql_conn);
		return -1;
	}

	nowtime();

	printf("set_mcp_content_calender start \n ");
	ret = set_mcp_content_calender();
	if(0!=ret)
	{
		printf("set_mcp_content_calender failed\n");
		mysql_close(&mysql_conn);
		return -1;
	}

	nowtime();

    printf("set_mcp_H5_music_id start \n ");
	ret = set_mcp_H5_music_id();
	if(0!=ret)
	{
		printf("set_mcp_H5_music_id failed\n");
		mysql_close(&mysql_conn);
		return -1;
	}

	nowtime();

	// [s] add by gwj for common_adv_info
    printf("set mcp_content_common_info start \n ");
    ret = set_mcp_content_common_info();
    if(0!=ret)
    {
        printf("set_mcp_content_common_info failed\n");
        mysql_close(&mysql_conn);
        return -1;
    }
    // [e] add by gwj for common_adv_info

    // [s] add by gwj for activity
    nowtime();
    printf("set set_mcp_netgame_activity_info start \n ");
    ret = set_mcp_netgame_activity_info();
    if(0!=ret)
    {
        printf("set_mcp_netgame_activity_info failed\n");
        mysql_close(&mysql_conn);
        return -1;
    }

    //nowtime();
    //printf("set set_mcp_activity_award_info start \n ");
    //ret = set_mcp_activity_award_info();
	/*if(0!=ret)*/
	//{
		//printf("set_mcp_activity_award_info failed\n");
		//mysql_close(&mysql_conn);
		//return -1;
	/*}*/

    //nowtime();
    //printf("set set_mcp_activity_info start \n ");
   /* ret = set_mcp_activity_info();*/
	//if(0!=ret)
	//{
		//printf("set_mcp_activity_info failed\n");
		//mysql_close(&mysql_conn);
		//return -1;
	/*}*/
	// [e] add by gwj for activity

    // [s] add by gwj for app_download_info
    //nowtime();
    //printf("set mcp_content_appinfo_data start \n ");
    //ret = set_mcp_appInfo_data_info();
	//if(0!=ret)
	//{
		//printf("mcp_content_appinfo_data failed\n");
		//mysql_close(&mysql_conn);
		//return -1;
	/*}*/
    // [e] add by gwj for app_download_info

	// [s] add by zhengxie for mobile_music_info
	nowtime();
	printf("set mb_music_content start \n ");
	ret = set_mb_music_content();
	if(0!=ret)
	{
		printf("set mb_music_content failed\n");
		mysql_close(&mysql_conn);
		return -1;
	}
	// [e] add by zhengxie for mobile_music_info
	
	nowtime();


	mysql_close(&mysql_conn);
	mongo_destroy( mongodb_conn );
	printf("update is ok\n");

	return 0;

}

