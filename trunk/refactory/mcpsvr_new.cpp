/*
 * mcpsvr_new.cpp
 *
 *  Created on: 2013-5-14
 *      Author: zhengxie
 *          Ps: Please keep it clean and beautiful, for a better world.
 */

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <time.h>

#include "module/mcp_content_data.h"
#include "module/mcp_game_content.h"
#include "module/mcp_content_news.h"
#include "module/mcp_content_championship.h"
#include "module/mcp_diff_update_info.h"
#include "module/pt_game_user_QA_list.h"
#include "module/pt_game_QandA.h"
#include "module/mcp_content_qp_activity.h"
#include "module/mcp_content_qp_activity_award.h"
#include "module/mcp_content_qp_activity_person.h"
#include "module/mcp_content_person_award.h"
#include "redis_module/redis_module.h"
#include "decorator/single_decorator/is_decorator.h"
#include "decorator/single_decorator/or_decorator.h"
#include "decorator/single_decorator/orderby_decorator.h"
#include "decorator/single_decorator/weight_decorator.h"
#include "decorator/single_decorator/not_equal_decorator.h"
#include "decorator/single_decorator/smaller_decorator.h"
#include "decorator/single_decorator/bigger_decorator.h"
#include "decorator/single_decorator/bigger_decorator_int.h"
#include "decorator/compound_decorator/decorator_for_test.h"
#include "decorator/compound_decorator/logic_or_decorator.h"
#include "mysql_module/mysql_general_module.h"
#include "output_type/grab_info_t.h"
#include "output_type/user_grab_number_t.h"
#include "output_type/pt_game_basic_info_t.h"
#include "output_type/game_basic_download_info.h"
#include "output_type/game_info_t.h"
#include "output_type/game_update_download_info.h"
#include "output_type/championship_basic_download_info.h"
#include "input_type/json_extractor.h"
#include "./client/middle_client.h"
#include "../duokoo_tools.h"
#include "../duokoo_mysql.h"
#include "./client/community_client.h"

#include "../mcp_session.h"
#include "../mcp.idl.mcp.h"
#include "../mcpsvr.h"
#include "mcpsvr_baiduAppInfo.h"


using namespace std;
using namespace content;
using namespace DuokooTools;

extern CommunityClient community_client;

/**
 * @brief 获得抢号列表, 若有用户概念，需要显示用户哪些号抢过，哪些没抢
 *
 * @Param type_id, c_id, id, user_id, page, page_size
 * @Param grab_list
 *
 * @Returns
 */

int McpServlet::get_grab_list(const idl::mcp_get_grab_list_params& in,
									idl::mcp_get_grab_list_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
    string type_id(in.type_id());
    string c_id(in.c_id());
    uint32_t id(in.id());
	string user_id(in.user_id());
    uint32_t page(in.page());
    uint32_t page_size(in.page_size());

    LOGA( "get_grab_list start: type_id[%s], c_id[%s], id[%d], user_id[%s], page[%d], page_size[%d]",
          type_id.c_str(), c_id.c_str(), id, user_id.c_str(), page, page_size);

    MysqlTable mcp_content_grab("mcp_content_grab", "GRB");
    MysqlTable mcp_content_user_grab("mcp_content_user_grab", "CUGRB");
	mcp_content_grab.set_unit_info(page_size,page);

    mcp_content_grab.add_select_column("id");
    mcp_content_grab.add_select_column("name");
    mcp_content_grab.add_select_column("c_id");
    mcp_content_grab.add_select_column("info");
    mcp_content_grab.add_select_column("create_time");
    mcp_content_grab.add_select_column("start_time");
    mcp_content_grab.add_select_column("end_time");
    mcp_content_grab.add_select_column("amount");

	if( !user_id.empty() )
	{
		mcp_content_user_grab.add_select_column("num");
		string user_id_condition = "CUGRB.user_id=" + user_id;
		mcp_content_grab["id"]->add_left_join(&mcp_content_user_grab, "grab_id", user_id_condition.c_str());
	}
	if( !type_id.empty() )
	{
    	mcp_content_grab["content_type"]->add_and_condition("=", type_id.c_str() );
     	mcp_content_grab["enable"]->add_and_condition("=", 1);
     	mcp_content_grab["start_time"]->add_orderby_decrease();
	}
	else if( !c_id.empty() )
	{
    	mcp_content_grab["c_id"]->add_and_condition("=", c_id.c_str() );
     	mcp_content_grab["enable"]->add_and_condition("=", 1);
     	mcp_content_grab["start_time"]->add_orderby_decrease();
	}
	else if( id != 0 )
	{
    	mcp_content_grab["id"]->add_and_condition("=", id );
     	mcp_content_grab["enable"]->add_and_condition("=", 1);
	}

	mcp_content_grab.process_query_sql("grab_mysql", true);
    int result_size = mcp_content_grab.get_total_count_without_page();

	if( !user_id.empty() ){
    	grab_info_t::set_grab_info_out(out.m_result_params()->mutable_grab_list(), mcp_content_grab, 1);
	}else{
		grab_info_t::set_grab_info_out(out.m_result_params()->mutable_grab_list(), mcp_content_grab, 0);
	}
	out.m_result_params()->set_count(result_size);

    LOGD("[GWJ] %s: item_count[%hu]", __FUNCTION__, result_size);
    LOGA( "get_grab_list end item_count[%hu]", result_size);
    gettimeofday(&end, NULL);
    LOGT(start, end);
    return 0;
}


/**
 * @brief 只获得用户的抢号列表
 *
 * @Param user_id, page, page_size
 * @Param user_grab_num_list
 *
 * @Returns
 */

int McpServlet::get_user_grab_num_list(const idl::mcp_get_user_grab_num_list_params& in,
											 idl::mcp_get_user_grab_num_list_response& out)
{
    LOGA( "get_user_grab_num_list start" );

	string user_id(in.user_id());
	uint32_t page(in.page());
	uint32_t page_size(in.page_size());

	MysqlTable mcp_content_user_grab("mcp_content_user_grab", "CUGRB");
	MysqlTable mcp_content_grab("mcp_content_grab", "GRB");
	mcp_content_user_grab.set_unit_info(page_size,page);
	mcp_content_user_grab.add_select_column("grab_id");
	mcp_content_user_grab.add_select_column("num");
	mcp_content_grab.add_select_column("c_id");
	mcp_content_grab.add_select_column("name");
    mcp_content_grab.add_select_column("create_time");
	mcp_content_grab.add_select_column("start_time");
	mcp_content_grab.add_select_column("end_time");
	mcp_content_grab.add_select_column("info");

	mcp_content_user_grab["user_id"]->add_and_condition("=", user_id.c_str() );
	mcp_content_user_grab["grab_id"]->add_inner_join(&mcp_content_grab, "id", NULL);

	mcp_content_user_grab.process_query_sql("grab_mysql", true);
	int result_size = mcp_content_user_grab.get_total_count_without_page();

	user_grab_number_t::set_user_grab_number_out(out.m_result_params()->mutable_user_grab_num_list(), mcp_content_user_grab);
	out.m_result_params()->set_count(result_size);

	LOGD("[GWJ] %s: item_count[%d]", __FUNCTION__, result_size);
	LOGA( "get_user_grab_num_list end" );
	return 0;
}

int McpServlet::get_pt_game_basic_list_by_game_id(const idl::mcp_get_pt_game_basic_list_by_game_id_params& in,
														idl::mcp_get_pt_game_basic_list_by_game_id_response& out)
{
    string game_id(in.game_id());
    LOGA("[GWJ] %s: start. game_id[%s]", __FUNCTION__, game_id.c_str());

    if(!IS_VALUED_STRING(game_id.c_str()))
    {
        LOGA("[GWJ] %s: game_id Empty", __FUNCTION__);
        return 0;
    }

	/* 2013-07-13 add by zhengxie 先从redis获取 beg*/

	map<string, string> record;
	string game_id_key = "game_id:"+game_id;

	RedisModule redis_processor;
	int result = redis_processor.get_record_by_key(game_id_key.c_str(), record);
	if(0 == result)
	{
		pt_game_basic_info_t::set_pt_game_basic_info_out(out.m_result_params()->mutable_pt_game_basic_list()->mutable_get(0), record);
		out.m_result_params()->set_count(1);
		UB_LOG_TRACE( "from redis :get_pt_game_basic_list_by_app_key end" );
		return 0;
	}

	/* 2013-07-13 add by zhengxie 先从redis获取 end*/


	MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");
	pt_game_basic_info.add_select_column("name");
	pt_game_basic_info.add_select_column("app_keys");
	pt_game_basic_info.add_select_column("app_secret");
	pt_game_basic_info.add_select_column("pay_report_addr");
	pt_game_basic_info.add_select_column("app_id");

	SET_MYSQL_SELECT_CONDITION(game_id.c_str(), "app_id", pt_game_basic_info, "=");

	pt_game_basic_info.process_query_sql("dev_platform_mysql", true);
	int result_size = pt_game_basic_info.get_total_count_without_page();

	pt_game_basic_info_t::set_pt_game_basic_info_out(out.m_result_params()->mutable_pt_game_basic_list(), pt_game_basic_info);


	/* 2013-07-13 add by zhengxie 再存到redis中去 beg*/

	if(0 != result_size)
	{
		pt_game_basic_info_t::set_pt_game_basic_info_record(pt_game_basic_info, record);

		RedisModule redis_processor_2;
		redis_processor_2.add_new_key_record(game_id_key.c_str(), record, 15);
	}

	/* 2013-07-13 add by zhengxie 再存到redis中去 end*/


	out.m_result_params()->set_count(result_size);

	LOGD("[GWJ] %s: item_count[%d]", __FUNCTION__, result_size);
	UB_LOG_TRACE( "get_pt_game_basic_list_by_game_id end" );
	return 0;
}

int McpServlet::get_pt_game_basic_list_by_app_key(const idl::mcp_get_pt_game_basic_list_by_app_key_params& in,
														idl::mcp_get_pt_game_basic_list_by_app_key_response& out)
{
    string app_key(in.app_key());
    LOGA("[GWJ] %s: start. app_key[%s]", __FUNCTION__, app_key.c_str());

    if(!IS_VALUED_STRING(app_key.c_str()))
    {
        LOGA("[GWJ] %s: app_key Empty", __FUNCTION__);
        return 0;
    }


	/* 2013-07-13 add by zhengxie 先从redis获取 beg*/

	map<string, string> record;
	string app_key_key = "app_key:"+app_key;

	RedisModule redis_processor;
	int result = redis_processor.get_record_by_key(app_key_key.c_str(), record);
	if(0 == result)
	{
		pt_game_basic_info_t::set_pt_game_basic_info_out(out.m_result_params()->mutable_pt_game_basic_list()->mutable_get(0), record);
		out.m_result_params()->set_count(1);
		UB_LOG_TRACE( "from redis :get_pt_game_basic_list_by_app_key end" );
		return 0;
	}

	/* 2013-07-13 add by zhengxie 先从redis获取 end*/


	MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");

	pt_game_basic_info.add_select_column("name");
	pt_game_basic_info.add_select_column("app_keys");
	pt_game_basic_info.add_select_column("app_secret");
	pt_game_basic_info.add_select_column("pay_report_addr");
	pt_game_basic_info.add_select_column("app_id");

	SET_MYSQL_SELECT_CONDITION(app_key.c_str(), "app_keys", pt_game_basic_info, "=");

	pt_game_basic_info.process_query_sql("dev_platform_mysql", true);
	int result_size = pt_game_basic_info.get_total_count_without_page();

	pt_game_basic_info_t::set_pt_game_basic_info_out(out.m_result_params()->mutable_pt_game_basic_list(), pt_game_basic_info);


	/* 2013-07-13 add by zhengxie 再存到redis中去 beg*/

	if(0 != result_size)
	{
		pt_game_basic_info_t::set_pt_game_basic_info_record(pt_game_basic_info, record);

		RedisModule redis_processor_2;
		redis_processor_2.add_new_key_record(app_key_key.c_str(), record, 15);
	}

	/* 2013-07-13 add by zhengxie 再存到redis中去 end*/


	out.m_result_params()->set_count(result_size);

	LOGD("[GWJ] %s: item_count[%d]", __FUNCTION__, result_size);
	UB_LOG_TRACE( "get_pt_game_basic_list_by_app_key end" );
	return 0;
}

int McpServlet::get_pt_app_key_by_game_id(const idl::mcp_get_pt_app_key_by_game_id_params& in,
												idl::mcp_get_pt_app_key_by_game_id_response& out)
{
	string game_id(in.game_id());
    LOGA("[GWJ] %s: start. game_id[%s]", __FUNCTION__, game_id.c_str());

    if(!IS_VALUED_STRING(game_id.c_str()))
    {
        LOGA("[GWJ] %s: game_id Empty", __FUNCTION__);
        return 0;
    }


	/* 2013-07-13 add by zhengxie 先从redis获取 beg*/

	map<string, string> record;
	string game_id_key = "game_id:"+game_id;

	RedisModule redis_processor;
	int result = redis_processor.get_record_by_key(game_id_key.c_str(), record);
	if(0 == result)
	{
		map<string, string>::iterator it;
		for ( it=record.begin(); it != record.end(); it++ )
		{
			if( !strcmp("app_key", (it->first).c_str()) ) {
				out.m_result_params()->set_app_key( (it->second).c_str() );
				out.m_result_params()->set_count(1);
			}
		}
		out.m_result_params()->set_count(1);
		UB_LOG_TRACE( "from redis :get_pt_app_key_by_game_id end" );
		return 0;
	}

	/* 2013-07-13 add by zhengxie 先从redis获取 end*/


	MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");

	pt_game_basic_info.add_select_column("app_keys");

	SET_MYSQL_SELECT_CONDITION(game_id.c_str(), "app_id", pt_game_basic_info, "=");

	pt_game_basic_info.process_query_sql("dev_platform_mysql", true);
	int result_size = pt_game_basic_info.get_total_count_without_page();
	if(result_size > 0)
	{
		const char* app_key = pt_game_basic_info.get_result_set_value(0, 0, "PGBI.app_keys");
		out.m_result_params()->set_app_key( app_key );
	}
	out.m_result_params()->set_count(result_size);

	UB_LOG_TRACE( "get_pt_app_key_by_game_id end" );
	return 0;
}

int McpServlet::get_pt_game_id_by_app_key(const idl::mcp_get_pt_game_id_by_app_key_params& in,
												idl::mcp_get_pt_game_id_by_app_key_response& out)
{
	string app_key(in.app_key());
    LOGA("[GWJ] %s: start. app_key[%s]", __FUNCTION__, app_key.c_str());

    if(!IS_VALUED_STRING(app_key.c_str()))
    {
        LOGA("[GWJ] %s: app_key Empty", __FUNCTION__);
        return 0;
    }


	/* 2013-07-13 add by zhengxie 先从redis获取 beg*/

	map<string, string> record;
	string app_key_key = "app_key:"+app_key;

	RedisModule redis_processor;
	int result = redis_processor.get_record_by_key(app_key_key.c_str(), record);
	if(0 == result)
	{
		map<string, string>::iterator it;
		for ( it=record.begin(); it != record.end(); it++ )
		{
			if( !strcmp("game_id", (it->first).c_str()) ) {
				out.m_result_params()->set_game_id( (it->second).c_str() );
				out.m_result_params()->set_count(1);
			}
		}
		UB_LOG_TRACE( "from redis :get_pt_game_id_by_app_key end" );
		return 0;
	}

	/* 2013-07-13 add by zhengxie 先从redis获取 end*/


	MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");

	pt_game_basic_info.add_select_column("app_id");

	SET_MYSQL_SELECT_CONDITION(app_key.c_str(), "app_keys", pt_game_basic_info, "=");

	pt_game_basic_info.process_query_sql("dev_platform_mysql", true);
	int result_size = pt_game_basic_info.get_total_count_without_page();
	if(result_size > 0)
	{
		const char* game_id = pt_game_basic_info.get_result_set_value(0, 0, "PGBI.app_id");
		out.m_result_params()->set_game_id( game_id );
	}
	out.m_result_params()->set_count(result_size);

	UB_LOG_TRACE( "get_pt_game_id_by_app_key end" );
	return 0;
}

int McpServlet::exist_pt_game_by_game_id(const idl::mcp_exist_pt_game_by_game_id_params& in,
											   idl::mcp_exist_pt_game_by_game_id_response& out)
{
	string game_id(in.game_id());
    LOGA("[GWJ] %s: start. game_id[%s]", __FUNCTION__, game_id.c_str());

    if(!IS_VALUED_STRING(game_id.c_str()))
    {
        LOGA("[GWJ] %s: game_id Empty", __FUNCTION__);
        return 0;
    }


	/* 2013-07-13 add by zhengxie 先从redis获取 beg*/

	map<string, string> record;
	string game_id_key = "game_id:"+game_id;

	RedisModule redis_processor;
	int result = redis_processor.get_record_by_key(game_id_key.c_str(), record);
	if(0 == result)
	{
		out.m_result_params()->set_flag(1);
		UB_LOG_TRACE( "from redis :get_pt_game_basic_list_by_app_key end" );
		return 0;
	}

	/* 2013-07-13 add by zhengxie 先从redis获取 end*/


	MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");

	pt_game_basic_info.add_select_column("app_id");

	SET_MYSQL_SELECT_CONDITION(game_id.c_str(), "app_id", pt_game_basic_info, "=");

	pt_game_basic_info.process_query_sql("dev_platform_mysql", true);
	int result_size = pt_game_basic_info.get_total_count_without_page();

	out.m_result_params()->set_flag(result_size);

	LOGD("[GWJ] %s: item_count[%d]", __FUNCTION__, result_size);
	UB_LOG_TRACE( "exist_pt_game_by_game_id start" );
	return 0;
}

int McpServlet::is_same_pt_game(const idl::mcp_is_same_pt_game_params& in,
									  idl::mcp_is_same_pt_game_response& out)
{
	string game_id(in.game_id());
	string app_key(in.app_key());
	string app_secret(in.app_secret());

    LOGA("[GWJ] %s: start. app_key[%s], game_id[%s], app_secret[%s]", __FUNCTION__, app_key.c_str(), game_id.c_str(), app_secret.c_str());
    if(!IS_VALUED_STRING(game_id.c_str()) ||
       !IS_VALUED_STRING(app_key.c_str()) ||
       !IS_VALUED_STRING(app_secret.c_str()) )
    {
        LOGA("[GWJ] %s: game_id or app_key or app_secret Empty", __FUNCTION__);
        return 0;
    }


	/* 2013-07-13 add by zhengxie 先从redis获取 beg*/

	map<string, string> record;
	string game_id_key = "game_id:"+game_id;

	RedisModule redis_processor;
	int result = redis_processor.get_record_by_key(game_id_key.c_str(), record);
	if(0 == result)
	{
		map<string, string>::iterator it;
		bool tmp_flag = true;
		for ( it=record.begin(); it != record.end(); it++ )
		{
			if(  !strcmp("app_key", (it->first).c_str()) )
			{
				if( strcmp(app_key.c_str(), (it->second).c_str()) ) {
					tmp_flag = false;
					break;
				}
			}
			if( !strcmp("app_secret", (it->first).c_str()) )
			{
				if( strcmp(app_secret.c_str(), (it->second).c_str()) ) {
					tmp_flag = false;
					break;
				}
			}
		}
		if(tmp_flag)
		{
			out.m_result_params()->set_flag(1);
			UB_LOG_TRACE( "from redis :get_pt_game_basic_list_by_app_key end" );
			return 0;
		}
	}

	/* 2013-07-13 add by zhengxie 先从redis获取 end*/


	MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");

	pt_game_basic_info.add_select_column("app_id");

	SET_MYSQL_SELECT_CONDITION(game_id.c_str(), "app_id", pt_game_basic_info, "=");
	SET_MYSQL_SELECT_CONDITION(app_key.c_str(), "app_keys", pt_game_basic_info, "=");
	SET_MYSQL_SELECT_CONDITION(app_secret.c_str(), "app_secret", pt_game_basic_info, "=");

	pt_game_basic_info.process_query_sql("dev_platform_mysql", true);
	int result_size = pt_game_basic_info.get_total_count_without_page();

	out.m_result_params()->set_flag(result_size);

	UB_LOG_TRACE( "is_same_pt_game end" );
	return 0;
}


/**
 * @brief deprecated 此接口暂未使用
 *
 * @Param in
 * @Param out
 *
 * @Returns
 */

int McpServlet::remove_pt_game_basic_list_by_app_key(const idl::mcp_remove_pt_game_basic_list_by_app_key_params& in,
														   idl::mcp_remove_pt_game_basic_list_by_app_key_response& out)
{
	string app_key(in.app_key());
	LOGA("[GWJ] %s: start. app_key[%s]", __FUNCTION__, app_key.c_str());

    if( !IS_VALUED_STRING(app_key.c_str()))
    {
        LOGA("[GWJ] %s: app_key Empty", __FUNCTION__);
        return 0;
    }

	map<string, string> record;
	string game_id;

	RedisModule redis_processor;
	int result = redis_processor.get_record_by_key(app_key.c_str(), record);

	if(0 == result)
	{
		map<string, string>::iterator it;
		for ( it=record.begin(); it != record.end(); it++ )
		{
			if(  !strcmp("game_id", (it->first).c_str()) )
			{
				game_id = it->second;
			}
		}
		if( IS_VALUED_STRING(game_id.c_str()))
		{
			LOGA("[GWJ] %s: game_id Empty", __FUNCTION__);
			string game_id_key = "game_id:"+game_id;

			RedisModule redis_processor_2;
			result = redis_processor_2.del_key_record(game_id_key.c_str());
			if(0!=result)
			{
				LOGA("[GWJ] %s: remove redis fail,game_id_key:%s", __FUNCTION__, game_id_key.c_str());
				return 0;
			}
		}
	}

	string app_key_key = "app_key:"+app_key;

	RedisModule redis_processor_3;
	result = redis_processor_3.del_key_record(app_key_key.c_str());
	if(0!=result)
	{
		LOGA("[GWJ] %s: remove redis fail,app_key_key:%s", __FUNCTION__, app_key_key.c_str());
		return 0;
	}

	out.m_result_params()->set_flag(1);
	return 0;
}

/** 废弃，新接口见下
 * @brief 专门用于增量更新, 结果集分为三大块：基本信息、下载信息、增量信息
 *		  注意：
 *		  1.如果下载信息为空，说明可能有增量信息，增量信息获得后，必须根据c_id同时
 *		    把所有渠道的最新包的信息也输出, 防止客户端增量更新失败，从而能重新下载最新包信息.
 *		  2.如果要获取增量信息，则输入参数file_md5为必填字段.file_md5是差分表的主键.
 *		  3.如果下载信息和增量信息都没有，需要去除file_md5条件再查询一次，防止出现其他渠道对不上
 *		    file_md5的情况。
 *		  4.若基本信息不存在，即使下载信息和增量信息存在，也不会输出。
 *		  5.若下载信息和增量信息都不存在，则也不会输出。
 *
 * @Param in
 * @Param out
 * @Returns
 */

/*int McpServlet::get_game_download_app_info(const idl::mcp_get_game_download_app_info_params& in,*/
												 //idl::mcp_get_game_download_app_info_response& out)
//{
	//struct timeval start, end;
	//gettimeofday(&start, NULL);

    //JsonExtractor extractor;
    //extractor.parse_input_params(in.filter());

    //const char* c_id = in.c_id();
    //const char* channel_info = extractor.get_filter_condition("channel_info");
    //const char* channel_id = extractor.get_filter_condition("channel_id");
    //const char* not_class_id = extractor.get_filter_condition("no_class_id");

    //const char* package_name = in.package_name();
    //const char* dev_version_code = in.dev_version_code();
    //const char* dev_version = in.dev_version();
	//const char* apk_md5 = in.apk_md5();
	//const char* file_md5 = in.file_md5(); // 若想得到更新包，则必填，此字段唯一确定一个包

    //const char* game_img = in.game_img(); // 用于基本信息输出时的图片类型


    //LOGA("[ZX] %s: c_id[%s], package_name[%s], dev_version_code[%s], dev_version[%s],"
		 //"file_md5[%s], game_img[%s], channel_info[%s], channel_id[%s], not_class_id[%s]",
		  //__FUNCTION__, c_id, package_name, dev_version_code, dev_version,
		  //file_md5, game_img, channel_info, channel_id, not_class_id);

	//content_input_params game_imgs; // 统一使用的图片类型
									//// 注意：game_img并不是过滤条件，而是筛选条件，即使相应img_type不存在的，
									//// 记录也会输出，只不过没有game_img相关信息罢了

    //SET_INPUT_PARAMETERS_TEMPLATE(game_img, game_imgs.img_types)

	//AbstractCompoundDecorator content_diff_decor;
	//AbstractCompoundDecorator game_basic_decor;
	//AbstractCompoundDecorator content_data_decor;

    //OR_CONDITION_TEMPLATE(c_id, "c_id", content_data_decor)					  // 下载信息统一使用的游戏 ID 和渠道名称修饰器, 或逻辑
    //OR_CONDITION_JSON(channel_info, "channel_info", content_data_decor)
    //OR_CONDITION_JSON(channel_id, "channel_id", content_data_decor)

	//if(IS_VALUED_STRING(c_id)) {											  // c_id_is_dec
		//content_diff_decor.add_primary_table_decorator(&c_id_or_dec);         // 已经在上面的宏定义中定义
	//}

	//[>
	 //*  package_name, version_code, version, apk_md5, file_md5的筛选条件格式，比如:
		//package_name:cn.cngamexx.cos, com.comandroidbox.g9zhgdscnbaidu, com.comandroidbox.g8elsscnbaidu
		//version_code:2, 3, 4
		//file_md5:89bc99dc264f913a44778b46b6145997, d1e80c62251a0d8f0684f1321931938c, 377f7788fd94f5ed5eb0c86c35b067ee
		//必须保证一一对应(第一个package_named对应第一个version_code, 第二个对应第二个)
		//几个可能有可能没有，但有的话个数一定要一一对应
	 //*
	//*/

	//int max_size = 0;

    //vector<char*> pkg_list;
    //int package_name_size = CommonInterface::splic_string((char*)package_name, pkg_list, ",");
    //max_size = package_name_size;

    //vector<char*> version_code_list;
    //int version_code_size = CommonInterface::splic_string((char*)dev_version_code, version_code_list, ",");
    //max_size = (version_code_size > max_size) ? version_code_size:max_size;

    //vector<char*> version_list;
    //int version_size = CommonInterface::splic_string((char*)dev_version, version_list, ",");
    //max_size = (version_size > max_size) ? version_size:max_size;

	//vector<char*> apk_md5_list;
	//int apk_md5_size = CommonInterface::splic_string((char*)apk_md5, apk_md5_list, ",");
	//max_size = (apk_md5_size > max_size) ? apk_md5_size:max_size;


    //vector<char*> file_md5_list; // 如果要获得增量更新信息，则必填file_md5
    //int file_md5_size = CommonInterface::splic_string((char*)file_md5, file_md5_list, ",");
    //max_size = (file_md5_size > max_size) ? file_md5_size:max_size;

    //int out_record_index = 0; // 最终的count是根据基本信息的条数决定的
							  //// 按照基本信息的条数逐条输出总的结果集
    //for(int i = 0; i < max_size; i++)
    //{

        //char* pkg_name = NULL;
        //char* ver_code = NULL;
        //char* ver_str  = NULL;
		//char* apk_md5  = NULL;
        //char* file_md5 = NULL;

        //if(i < package_name_size) {
            //pkg_name = pkg_list[i];
        //}

        //if(i < version_code_size) {
            //ver_code = version_code_list[i];
        //}

        //if(i < version_size) {
            //ver_str = version_list[i];
        //}

		//if(i < apk_md5_size) {
			//apk_md5 = apk_md5_list[i];
		//}

        //if(i < file_md5_size) {
            //file_md5 = file_md5_list[i];
        //}


        //[> 查询下载信息 beg <]

        //IS_CONDITION_TEMPLATE(file_md5, "file_md5", content_data_decor)
        //IS_CONDITION_TEMPLATE(pkg_name, "package_name", content_data_decor)
        //IS_CONDITION_TEMPLATE(ver_code, "int_version", content_data_decor)
        //IS_CONDITION_TEMPLATE(ver_str, "string_version", content_data_decor)
		//IS_CONDITION_TEMPLATE(apk_md5, "apk_md5", content_data_decor)

        //MCPContentData mcp_content_data("c_id");
        //mcp_content_data.load_decorator(&content_data_decor);
        //mcp_content_data.general_fetch_list_records(false);

        //int data_rec_count = mcp_content_data.get_key_size();
		//if(data_rec_count == 0)
		//{
            ////LOGA("[ZX] %s: Game_Download_Info with file_md5 Not Found!", __FUNCTION__);
		//}

        //[> 查询下载信息 end <]


		//[> 查询更新包信息 beg <]

		//MCPDiffUpdateInfo mcp_diff_update_module("old_file_md5");


		//OrDecorator id_or_dec;			// 如果有下载信息，则需要用于基本信息的c_id修饰器
		//IsDecorator id_is_dec;			// 如果有差分信息，则需要用于基本信息的c_id修饰器
		//IsDecorator old_file_md5_decor; // 用于更新包的c_id修饰器
		//string new_cdata_id;			// 用于最新包的new_cdata_id字段
		//OrDecorator c_id_or_decorator;
		//vector<const char*> c_ids_p;

		//int diff_rec_count = 0;

		//if(data_rec_count == 0)			//如果下载信息为空，说明可能有差分文件存在
		//{
			//old_file_md5_decor.add_field_filter_condition("old_file_md5", file_md5);
			//content_diff_decor.add_primary_table_decorator(&old_file_md5_decor);
			//mcp_diff_update_module.load_decorator(&content_diff_decor);
			//mcp_diff_update_module.general_fetch_list_records(false);


			/**
			 * 如果下载信息和更新信息都没有，则尝试去除file_md5再查询下载信息一次
			 * 或者
			 * 如果有更新信息，则尝试去除file_md5根据c_id查询所有的下载信息
			 **/

			//REMOVE_IS_CONDITION_TEMPLATE(file_md5, content_data_decor)

			//diff_rec_count = mcp_diff_update_module.get_key_size();
			//if(diff_rec_count == 0)
			//{
				//LOGA("[ZX] %s: Game_Diff_Info Not Found!", __FUNCTION__);

				//[> 尝试去除file_md5再查询下载信息一次 beg<]

				//mcp_content_data.general_fetch_list_records(false);

				//REMOVE_IS_CONDITION_TEMPLATE(pkg_name, content_data_decor) 	// 方便下次循环时修饰器没有上次的修饰器
				//REMOVE_IS_CONDITION_TEMPLATE(ver_code, content_data_decor)
				//REMOVE_IS_CONDITION_TEMPLATE(ver_str, content_data_decor)
				//REMOVE_IS_CONDITION_TEMPLATE(apk_md5, content_data_decor)

				//int data_rec_count = mcp_content_data.get_key_size();
				//if(data_rec_count == 0)
				//{
					////LOGA("[ZX] %s: Game_Download_Info without file_md5 Not Found!", __FUNCTION__);
					//continue;
				//}
				//else
				//{
					//id_or_dec.set_same_field_filter_conditions(mcp_content_data.get_result_keys(),
															   //"id", true);
					//game_basic_decor.add_primary_table_decorator(&id_or_dec);
				//}

				//[> 尝试去除file_md5再查询下载信息一次 end<]

			//}
										//// 如果有差分文件，则需要选出对应的c_id便于下面查找基本信息,
			//else						// 还需要根据此c_id得到最新包的信息。
			//{
				//int bucket_count = mcp_diff_update_module.get_key_bucket_size(file_md5);
				//for(int i=0; i<bucket_count; i++)
				//{
					//MCPDiffUpdateInfoRecord *diff_record = (MCPDiffUpdateInfoRecord*)mcp_diff_update_module.get_record_by_key_vec(file_md5, i);
					//c_ids_p.push_back( diff_record->c_id.c_str());
				//}
				//id_or_dec.set_same_field_filter_conditions(&c_ids_p, "id", true);
				//game_basic_decor.add_primary_table_decorator(&id_or_dec);


				//[> 查询最新包的信息 beg<]

				//c_id_or_decorator.set_same_field_filter_conditions(&c_ids_p, "c_id", true);
				//content_data_decor.add_primary_table_decorator(&c_id_or_decorator);

				//mcp_content_data.general_fetch_list_records(false);
				//int new_data_rec_count = mcp_content_data.get_key_size();
				//if(new_data_rec_count == 0)
				//{
					////LOGA("[ZX] %s: NEW_Game_Download_Info Not Found!", __FUNCTION__);
				//}

				//content_data_decor.remove_primary_table_decorator(&c_id_or_decorator);

				//[> 查询最新包的信息 beg<]


			//}
		//}
		//else // 如果有下载信息，直接根据c_id查找基本信息即可
		//{
			//REMOVE_IS_CONDITION_TEMPLATE(pkg_name, content_data_decor) 	// 方便下次循环时修饰器没有上次的修饰器
			//REMOVE_IS_CONDITION_TEMPLATE(ver_code, content_data_decor)
			//REMOVE_IS_CONDITION_TEMPLATE(ver_str, content_data_decor)
			//REMOVE_IS_CONDITION_TEMPLATE(apk_md5, content_data_decor)

			//REMOVE_IS_CONDITION_TEMPLATE(file_md5, content_data_decor)

			//id_or_dec.set_same_field_filter_conditions(mcp_content_data.get_result_keys(),
												  //"id", true);
			//game_basic_decor.add_primary_table_decorator(&id_or_dec);
		//}

		//content_diff_decor.remove_primary_table_decorator(&old_file_md5_decor);

		//[> 查询更新包信息 end <]


        //[> 查询基本信息 beg <]

		//MCPGameContent content_game("id");
		//NOT_EQUAL_CONDITION_JSON(not_class_id, "class.id", game_basic_decor) // 统一过滤掉的 class.id

		//content_game.load_decorator(&game_basic_decor);
		//content_game.general_fetch_list_records(false);

		//game_basic_decor.remove_all_primary_table_decorator();
		//int basic_rec_count = content_game.get_key_size();
		//if(basic_rec_count == 0)
		//{
            //LOGA("[ZX] %s: Game_Basic_Info Not Found!", __FUNCTION__);
			//continue;		// 如果基本信息都没有，那就不用输出任何信息
		//}

		//[> 查询基本信息 end <]


		//[> 插入结果集 beg<]

		//game_update_info_param::set_outparameter(out.m_result_params()->mutable_result_list(out_record_index),
												 //game_imgs, mcp_content_data, content_game, mcp_diff_update_module, 0);
		//out_record_index++; //最终的count是根据基本信息的条数决定的

		//[> 插入结果集 end<]

    //}

    //out.mutable_result_params()->set_count(out_record_index);
    //LOGA("[ZX] %s: count [%d] OK!", __FUNCTION__, out_record_index);


	//gettimeofday(&end, NULL);
	//LOGT(start, end);
    //return 0;
/*}*/

/**
 * @brief 专门用于增量更新, 结果集分为三大块：基本信息、下载信息、增量信息
 *		  注意：
 *		  1.根据过滤条件和pkg_name查询下载信息，如果下载信息为空，不输出任何结果。
 *		  2.如果下载信息不为空，如果有apk_md5, 比较apk_md5, 相同输出，不相同不输出；
 *		    如果有file_md5, 比较file_md5,
 *		    如果相同，输出，不去查询查分文件，如果不相同，再去查询查分文件。
 *		  3.查分表如果有记录，则输出，没有则不输出。
 *		  4.如果要获取增量信息，则输入参数file_md5为必填字段.file_md5是差分表的主键.
 *		  5.若基本信息不存在，即使下载信息和增量信息存在，也不会输出。
 *		  6.若下载信息和增量信息都不存在，则也不会输出。
 *
 * @Param in
 * @Param out
 * @Returns
 */

int McpServlet::get_game_download_app_info(const idl::mcp_get_game_download_app_info_params& in,
												 idl::mcp_get_game_download_app_info_response& out)
{
	struct timeval start, middle, end;
	gettimeofday(&start, NULL);

	JsonExtractor extractor;
	extractor.parse_input_params(in.filter());

	const char* c_id = in.c_id();
	const char* channel_info = extractor.get_filter_condition("channel_info");
	const char* channel_id = extractor.get_filter_condition("channel_id");
	const char* not_class_id = extractor.get_filter_condition("no_class_id");

	const char* package_name = in.package_name();
	const char* dev_version_code = in.dev_version_code();
	const char* dev_version = in.dev_version();
	const char* apk_md5 = in.apk_md5();
	const char* file_md5 = in.file_md5(); // 若想得到更新包，则必填，此字段唯一确定一个包

	const char* game_img = in.game_img(); // 用于基本信息输出时的图片类型


	LOGA("[ZX] %s: c_id[%s], package_name[%s], dev_version_code[%s], dev_version[%s],"
		 "file_md5[%s], game_img[%s], channel_info[%s], channel_id[%s], not_class_id[%s]",
		  __FUNCTION__, c_id, package_name, dev_version_code, dev_version,
		  file_md5, game_img, channel_info, channel_id, not_class_id);

	content_input_params game_imgs; // 统一使用的图片类型
									// 注意：game_img并不是过滤条件，而是筛选条件，即使相应img_type不存在的，
									// 记录也会输出，只不过没有game_img相关信息罢了

	SET_INPUT_PARAMETERS_TEMPLATE(game_img, game_imgs.img_types)

	AbstractCompoundDecorator content_diff_decor;
	AbstractCompoundDecorator game_basic_decor;
	AbstractCompoundDecorator content_data_decor;

	OR_CONDITION_TEMPLATE(c_id, "c_id", content_data_decor)					  // 下载信息统一使用的游戏 ID 和渠道名称修饰器, 或逻辑
	OR_CONDITION_JSON(channel_info, "channel_info", content_data_decor)
	OR_CONDITION_JSON(channel_id, "channel_id", content_data_decor)

	if(IS_VALUED_STRING(c_id)) {											  // c_id_is_dec
		content_diff_decor.add_primary_table_decorator(&c_id_or_dec);         // 已经在上面的宏定义中定义
	}

	/*
	 *  package_name, version_code, version, apk_md5, file_md5的筛选条件格式，比如:
		package_name:cn.cngamexx.cos, com.comandroidbox.g9zhgdscnbaidu, com.comandroidbox.g8elsscnbaidu
		version_code:2, 3, 4
		file_md5:89bc99dc264f913a44778b46b6145997, d1e80c62251a0d8f0684f1321931938c, 377f7788fd94f5ed5eb0c86c35b067ee
		必须保证一一对应(第一个package_named对应第一个version_code, 第二个对应第二个)
		几个可能有可能没有，但有的话个数一定要一一对应
	 *
	*/

	int max_size = 0;

	vector<char*> pkg_list;
	int package_name_size = CommonInterface::splic_string((char*)package_name, pkg_list, ",");
	max_size = package_name_size;

	vector<char*> version_code_list;
	int version_code_size = CommonInterface::splic_string((char*)dev_version_code, version_code_list, ",");
	max_size = (version_code_size > max_size) ? version_code_size:max_size;

	vector<char*> version_list;
	int version_size = CommonInterface::splic_string((char*)dev_version, version_list, ",");
	max_size = (version_size > max_size) ? version_size:max_size;

	vector<char*> apk_md5_list;
	int apk_md5_size = CommonInterface::splic_string((char*)apk_md5, apk_md5_list, ",");
	max_size = (apk_md5_size > max_size) ? apk_md5_size:max_size;


	vector<char*> file_md5_list; // 如果要获得增量更新信息，则必填file_md5
	int file_md5_size = CommonInterface::splic_string((char*)file_md5, file_md5_list, ",");
	max_size = (file_md5_size > max_size) ? file_md5_size:max_size;

	int out_record_index = 0; // 最终的count是根据基本信息的条数决定的
							  // 按照基本信息的条数逐条输出总的结果集
	for(int i = 0; i < max_size; i++)
	{

		char* pkg_name = NULL;
		char* ver_code = NULL;
		char* ver_str  = NULL;
		char* apk_md5  = NULL;
		char* file_md5 = NULL;

		if(i < package_name_size) {
			pkg_name = pkg_list[i];
		}

		if(i < version_code_size) {
			ver_code = version_code_list[i];
		}

		if(i < version_size) {
			ver_str = version_list[i];
		}

		if(i < apk_md5_size) {
			apk_md5 = apk_md5_list[i];
		}

		if(i < file_md5_size) {
			file_md5 = file_md5_list[i];
		}


		/* 查询下载信息 beg */

		//IS_CONDITION_TEMPLATE(file_md5, "file_md5", content_data_decor)
		IS_CONDITION_TEMPLATE(pkg_name, "package_name", content_data_decor)
		IS_CONDITION_TEMPLATE(ver_code, "int_version", content_data_decor)
		IS_CONDITION_TEMPLATE(ver_str, "string_version", content_data_decor)
		//IS_CONDITION_TEMPLATE(apk_md5, "apk_md5", content_data_decor)

		MCPContentData mcp_content_data("c_id");
		mcp_content_data.load_decorator(&content_data_decor);
		mcp_content_data.general_fetch_list_records(false);

		REMOVE_IS_CONDITION_TEMPLATE(pkg_name, content_data_decor)
		REMOVE_IS_CONDITION_TEMPLATE(ver_code, content_data_decor)
		REMOVE_IS_CONDITION_TEMPLATE(ver_str, content_data_decor)

		int data_rec_count = mcp_content_data.get_key_size();
		if(data_rec_count == 0)
		{
			continue;
		}

		/* 查询下载信息 end */

		/* 查询基本信息 beg */

		OrDecorator id_or_dec;			// 如果有下载信息，则需要用于基本信息的c_id修饰器
		id_or_dec.set_same_field_filter_conditions(mcp_content_data.get_result_keys(),"id",true);
		game_basic_decor.add_primary_table_decorator(&id_or_dec);

		MCPGameContent content_game("id");
		NOT_EQUAL_CONDITION_JSON(not_class_id, "class.id", game_basic_decor) // 统一过滤掉的 class.id

		content_game.load_decorator(&game_basic_decor);
		content_game.general_fetch_list_records(false);

		game_basic_decor.remove_all_primary_table_decorator();
		int basic_rec_count = content_game.get_key_size();
		if(basic_rec_count == 0)
		{
			//LOGA("[ZX] %s: Game_Basic_Info Not Found!", __FUNCTION__);
			continue;		// 如果基本信息都没有，那就不用输出任何信息
		}

		/* 查询基本信息 end */

		/* 查询更新包信息 beg */

		MCPDiffUpdateInfo mcp_diff_update_module("old_file_md5");

		//IsDecorator id_is_dec;			// 如果有差分信息，则需要用于基本信息的c_id修饰器
		IsDecorator old_file_md5_decor; // 用于更新包的c_id修饰器
		//string new_cdata_id;			// 用于最新包的new_cdata_id字段
		//OrDecorator c_id_or_decorator;
		//vector<const char*> c_ids_p;


		int diff_rec_count = 0;

		vector<const char*>* v_c = mcp_content_data.get_result_keys();
		const char *c_id_temp = (*v_c)[0];
		MCPContentDataInfo* data_record_temp = (MCPContentDataInfo*)mcp_content_data.get_record_by_key_vec(c_id_temp, 0);
		if( NULL != apk_md5 ) //如果签名md5不为空，比较是否一致
		{
			const char *apk_md5_temp = data_record_temp->apk_md5.c_str();
			if(strcmp(apk_md5_temp, apk_md5)) // 不一致，直接返回
			{
				continue;
			}
		}
		if( NULL != file_md5 )
		{
			const char *file_md5_temp = data_record_temp->file_md5.c_str();
			if(strcmp(file_md5_temp, file_md5))
			{
				old_file_md5_decor.add_field_filter_condition("old_file_md5", file_md5);
				content_diff_decor.add_primary_table_decorator(&old_file_md5_decor);
				mcp_diff_update_module.load_decorator(&content_diff_decor);
				mcp_diff_update_module.general_fetch_list_records(false);
			}
		}

		/* 查询更新包信息 end */

		/* 插入结果集 beg*/

		game_update_info_param::set_outparameter(out.m_result_params()->mutable_result_list(out_record_index),
												 game_imgs, mcp_content_data, content_game, mcp_diff_update_module, 0);
		out_record_index++; //最终的count是根据基本信息的条数决定的

		/* 插入结果集 end*/

		gettimeofday(&middle, NULL);
		double process_time = CommonInterface::get_interface_process_time(start, middle);
		if(process_time > 200.0)
		{
			LOGA("[ZX] %s: break,too long! [TIME] %lf", __FUNCTION__, process_time);
			break;
		}
	}

	out.mutable_result_params()->set_count(out_record_index);
    //out.mutable_result_params()->set_count(0);
    LOGA("[ZX] %s: count [%d] OK!", __FUNCTION__, out_record_index);


    gettimeofday(&end, NULL);
    LOGT(start, end);
    return 0;
}

int McpServlet::get_game_dbapp_detail_info(const idl::mcp_get_game_dbapp_detail_info_params& in, idl::mcp_get_game_dbapp_detail_info_response& out)
{
    /*
    idl::mcp_get_game_download_app_info_params* iinn = idl::mcp_get_game_download_app_info_params::create(in._pool);;
    iinn->set_c_id(in.c_id());
    iinn->set_package_name(in.package_name());
    iinn->set_dev_version_code(in.dev_version_code());
    iinn->set_dev_version(in.dev_version());
    iinn->set_apk_md5(in.apk_md5());
    iinn->set_file_md5(in.file_md5());
    iinn->set_game_img(in.game_img());
    iinn->set_filter(in.filter());

    idl::mcp_get_game_download_app_info_response* ooutt = idl::mcp_get_game_download_app_info_response::create(in._pool);
    get_game_download_app_info(*iinn,*ooutt);
    if(ooutt->m_result_params()->count() < 1)
    {
        map<string,string> paramVec;
        string package_name = in.package_name();
        paramVec.insert(pair<string,string>("package_name", package_name));
        getBaiduAppInfoFromMongoDbByPackageName(paramVec, out);
    }
    else
    {
        idl::vector<idl::game_update_download_info>* pDownInfo = ooutt->m_result_params()->m_result_list();
        idl::vector<idl::game_update_download_info>* pCurDownInfo = out.m_result_params()->m_result_list();
        for(unsigned int i = 0; i < pDownInfo->size(); i++)
        {
            const idl::game_update_download_info* pDownloadInfoItem = pCurDownInfo->push_back();
            pDownloadInfoItem = &(pDownInfo->get(i));
        }
        //out.m_result_params()->set_count(ooutt->m_result_params()->count());
        out.m_result_params()->set_count(pDownInfo->size());
    }
    */

    map<string,string> paramVec;
    paramVec.clear();
    paramVec.insert(pair<string,string>("package_name", in.package_name()));
    paramVec.insert(pair<string,string>("game_img", in.game_img()));
    paramVec.insert(pair<string,string>("filter", in.filter()));

    int nResult = get_baiduApp_download_info(paramVec,out);
    if(nResult < 1)
    {
        nResult = 0;
        getBaiduAppInfoFromMongoDbByPackageName(paramVec,out,nResult);
    }

    return 0;
}
/**
 * @brief 获得争霸赛列表
 *
 * @Param in
 * @Param out
 *
 * @Returns
 */

int McpServlet::get_championship_info(const idl::mcp_get_championship_info_params& in, idl::mcp_get_championship_info_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

    int page = in.page();
    int page_size = in.page_size();

	int total_count = 0;

    LOGA( "[zx] %s start: page[%d], page_size[%d]", __FUNCTION__, page, page_size);

    AbstractCompoundDecorator champion_compound_decor;
    //AbstractCompoundDecorator basic_compound_decor;
    //AbstractCompoundDecorator data_compound_decor;

	MCPContentChampionship mcp_content_championship("id");
    mcp_content_championship.set_unit_info(page_size, page);

	//MCPGameContent content_game("id");
	//MCPContentData mcp_content_data("c_id");

	OrderbyDecorator logdate_order;
	logdate_order.order_type = -1;
	logdate_order.add_field_filter_condition("log_date", NULL);
	champion_compound_decor.add_primary_table_decorator(&logdate_order);

	SmallerThanDecorator smaller_logdate;
	smaller_logdate.add_field_filter_condition("log_date", "3000-09-20 15:05:00"); //logdate_order 是特殊的query，必须有常规的query，才能获取数据
	champion_compound_decor.add_primary_table_decorator(&smaller_logdate);

	IsDecorator enable_decor;
	enable_decor.add_field_filter_condition("enable", "1");
	champion_compound_decor.add_primary_table_decorator(&enable_decor);

	/* SmallerThanDecorator smaller_endtime; */
	/* smaller_endtime.add_field_filter_condition("end_time", ""); //logdate_order 是特殊的query，必须有常规的query，才能获取数据 */
	/* champion_compound_decor.add_primary_table_decorator(&smaller_endtime); */

    mcp_content_championship.load_decorator(&champion_compound_decor);

    mcp_content_championship.general_fetch_list_records(true);
    int size = mcp_content_championship.get_key_size();
	if(size == 0)
	{
		out.m_result_params()->set_count(0);
		LOGA("[zx] %s: count 0. No championsip record found", __FUNCTION__);
		return 0;
	}

	int j=0;
	for(int i = 0; i < size; i++)
	{
		MCPGameContent content_game("id");
		MCPContentData mcp_content_data("c_id");

		AbstractCompoundDecorator basic_compound_decor;
		AbstractCompoundDecorator data_compound_decor;

		MCPContentChampionshipRecord* championship_record =
				(MCPContentChampionshipRecord*)mcp_content_championship.get_record_by_key_vec(i, 0);

		const char* c_id = championship_record->c_id.c_str();

		/* 查询基本信息 beg*/

        IS_CONDITION_TEMPLATE(c_id, "id", basic_compound_decor)
		content_game.load_decorator(&basic_compound_decor);
		content_game.general_fetch_list_records(false);
		int basic_rec_count = content_game.get_key_size();
		if(basic_rec_count == 0)
		{
            LOGA("[zx] %s: Game_Basic_Info Not Found!", __FUNCTION__);
			continue;		// 如果基本信息都没有，那就不用输出任何信息
		}
		basic_compound_decor.remove_all_primary_table_decorator();

		/* 查询基本信息 end*/

		/* 查询下载信息 beg*/

		IsDecorator c_id_is_decorator;
		if(IS_VALUED_STRING(c_id))
		{
			c_id_is_decorator.add_field_filter_condition("c_id", c_id);
			data_compound_decor.add_primary_table_decorator(&c_id_is_decorator);
		}
		mcp_content_data.load_decorator(&data_compound_decor);
		mcp_content_data.general_fetch_list_records(false);
		int data_rec_count = mcp_content_data.get_key_size();
		if(data_rec_count == 0)
		{
            LOGA("[zx] %s: Game_Download_Info Not Found!", __FUNCTION__);
			continue;		// 如果下载信息都没有，那也不用输出任何信息
		}
		data_compound_decor.remove_all_primary_table_decorator();

		/* 查询下载信息 end*/

		total_count++;
		/* 插入结果集 beg*/

		championship_basic_download_info_param::set_outparameter(out.m_result_params()->mutable_championship_list(j),
												  mcp_content_data, content_game, mcp_content_championship,i, 0);
		j++;

		/* 插入结果集 end*/


	}

    int record_count = mcp_content_championship.get_total_result_count();
	out.m_result_params()->set_count(total_count);
	//out.m_result_params()->set_count(3);
    LOGA("[zx] %s: count [%d] OK!", __FUNCTION__, size);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}

int McpServlet::user_add_coins(const idl::mcp_user_add_coins_params& in, idl::mcp_user_add_coins_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	/* 获取当前到当天结束之间的时间差：秒 beg*/

	time_t current_time;
	time( &current_time );
	struct tm *p;

	p = localtime( &current_time );
	p->tm_hour = 23;
	p->tm_min = 59;
	p->tm_sec = 59;

	time_t end_time = mktime(p);
	int diff_time = end_time - current_time;

	/* 获取当前到当天结束之间的时间差：秒 beg*/

    string user_id(in.user_id());
    string game_id(in.game_id());
    uint32_t opt(in.opt());

    LOGA( "[zx] %s start: user_id[%s], game_id[%s], opt[%d]", __FUNCTION__, user_id.c_str(), game_id.c_str(), opt);
	if(opt == 60) // 用户安装游戏
	{

		/* 判断用户是否安装过这个游戏，先从redis获取 beg*/

		list<string> record;
		list<string>::iterator it;
		string user_id_key = "install_user_id:" + user_id;

		RedisModule redis_processor;
		int result = redis_processor.get_values_by_key(user_id_key.c_str(), record);
		for ( it=record.begin(); it != record.end(); it++ )
		{
			if( !strcmp(game_id.c_str(), it->c_str()) ) { // 该用户安装过这个游戏
				out.m_result_params()->set_result(0);
				out.m_result_params()->set_errorcode(1);
				return 0;
			}
		}

		/* 判断用户是否安装过这个游戏，先从redis获取 end*/


		/* 判断用户是否安装过这个游戏，后从mysql获取 beg*/

		MysqlTable user_game_install("user_game_install", "UGI");
		user_game_install.add_select_column("game_id");
		user_game_install["user_id"]->add_and_condition("=", user_id.c_str() );
		user_game_install["game_id"]->add_and_condition("=", game_id.c_str() );
		user_game_install.process_query_sql("default_mysql", true);
		int result_size = user_game_install.get_total_count_without_page();
		if(result_size != 0) // 该用户安装过这个游戏
		{

			out.m_result_params()->set_result(0);
			out.m_result_params()->set_errorcode(1);
			return 0;
		}

		/* 判断用户是否安装过这个游戏，后从mysql获取 end*/

		/* 安装过的游戏存到mysql beg*/

		MysqlTable add_user_game_install("user_game_install", "AUGI");
		add_user_game_install["user_id"]->set_value( user_id.c_str() );
		add_user_game_install["game_id"]->set_value( game_id.c_str() );

		add_user_game_install.add_new_insert_transaction();
		bool ret = add_user_game_install.process_insert_transaction("default_mysql");
		if(ret == false)
		{
			LOGA("[GWJ] %s: process_insert_transaction Error!", __FUNCTION__);
			return -1;
		}

		/* 安装过的游戏存到mysql end*/

		/* 安装过的游戏存到redis beg*/

		RedisModule redis_processor2;
		result = redis_processor2.add_new_key_value(user_id_key.c_str(), game_id.c_str());
		if(0 != result)
		{
			LOGA( "[zx] %s redis fault", __FUNCTION__);
			return -1;
		}

		/* 安装过的游戏存到redis end*/

		/* 判断用户当天安装游戏是否超过五个 beg*/

		RedisModule redis_processor3;
		string user_game_num_string;
		string user_game_num_key = "install_num_user_id:" + user_id;
		result = redis_processor3.get_value_by_key(user_game_num_key.c_str(), user_game_num_string);
		int user_game_num = atoi(user_game_num_string.c_str());
		if( user_game_num  > 4)
		{
			out.m_result_params()->set_result(0);
			out.m_result_params()->set_errorcode(2);
			return 0;
		}

		/* 判断用户当天安装游戏是否超过五个 beg*/

		/* 存到redis beg*/

		user_game_num++;
		stringstream ss;
		ss << user_game_num;
		string user_game_num_str;
		ss >> user_game_num_str;

		RedisModule redis_processor4;
		result = redis_processor4.add_new_tuple( user_game_num_key.c_str(), user_game_num_str.c_str(), diff_time/60 );
		if( 0 != result )
		{
			LOGA( "[zx] %s redis fault", __FUNCTION__);
			return -1;
		}

		/* 存到redis end*/

		/* 调用前端接口，获取积分 beg*/

	    int score = MiddleClient::instance().addGameHallScore( atoi( user_id.c_str() ), opt  );
		LOGA( "[zx]get install score from middle_client:%d", score);
		out.m_result_params()->set_result(score);
		if( 0 == score )
		{
			out.m_result_params()->set_errorcode(2);
		}
		else
		{
			out.m_result_params()->set_errorcode(0);
		}
		gettimeofday(&end, NULL);
		LOGT(start, end);
		return 0;

		/* 调用前端接口，获取积分 end*/

	}
	else if(opt == 61) // 游戏启动
	{
		string user_id_key = "lanuch_user_id:" + user_id;
		string user_id_num_key = "lanuch_num_user_id:" + user_id;

		/* 判断用户是否启动过这个游戏，先从redis获取 beg*/

		list<string> record;
		list<string>::iterator it;

		RedisModule redis_processor;
		int result = redis_processor.get_values_by_key(user_id_key.c_str(), record);
		for ( it=record.begin(); it != record.end(); it++ )
		{
			if( !strcmp(game_id.c_str(), it->c_str()) ) { // 该用户启动过这个游戏
				out.m_result_params()->set_result(0);
				out.m_result_params()->set_errorcode(1);
				return 0;
			}
		}

		/* 判断用户是否启动过这个游戏，先从redis获取 end*/

		/* 判断用户是否启动过的个数是否超过5?beg*/

		RedisModule redis_processor5;
		string user_game_num_string;
		result = redis_processor5.get_value_by_key(user_id_num_key.c_str(), user_game_num_string);
		int user_game_num = atoi(user_game_num_string.c_str());
		if( user_game_num  > 4)
		{
			out.m_result_params()->set_result(0);
			out.m_result_params()->set_errorcode(2);
			return 0;
		}

		/* 判断用户是否启动过的个数是否超过5个 beg*/

		/* 用户启动游戏的个数存到redis beg*/

		user_game_num++;
		stringstream ss;
		ss << user_game_num;
		string user_game_num_str;
		ss >> user_game_num_str;

		RedisModule redis_processor6;
		result = redis_processor6.add_new_tuple( user_id_num_key.c_str(), user_game_num_str.c_str(), diff_time/60 );
		if( 0 != result )
		{
			LOGA( "[zx] %s redis fault", __FUNCTION__);
			return -1;
		}

		/* 用户启动游戏的个数存到redis end*/

		/* 用户启动了哪些游戏存到redis beg*/

		RedisModule redis_processor7;
		result = redis_processor7.add_new_key_value(user_id_key.c_str(), game_id.c_str(), diff_time/60 );
		if(0 != result)
		{
			LOGA( "[zx] %s redis fault", __FUNCTION__);
			return -1;
		}

		/* 用户启动了哪些游戏存到redis end*/


		/* 调用前端接口，获取积分 beg*/

		int score = MiddleClient::instance().addGameHallScore( atoi( user_id.c_str() ), opt );
		LOGA( "[zx]get launch score from middle_client:%d", score);
		out.m_result_params()->set_result( score );
		if( 0 == score )
		{
			out.m_result_params()->set_errorcode(2);
		}
		else
		{
			out.m_result_params()->set_errorcode(0);
		}

		gettimeofday(&end, NULL);
		LOGT(start, end);
		return 0;

		/* 调用前端接口，获取积分 end*/
	}
	else
	{
		out.m_result_params()->set_result(0);
		out.m_result_params()->set_errorcode(3);
		gettimeofday(&end, NULL);
		LOGT(start, end);
		return 0;
	}
}

int McpServlet::get_game_news(const idl::mcp_get_game_news_params& in,
								    idl::mcp_get_game_news_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
    LOGD("[GWJ] %s: start", __FUNCTION__);

    const char* info_id = in.info_id();
    const char* c_id = in.c_id();
    const char* info_type = in.info_type();
    const char* game_type = in.game_type();
    const char* data_limit = in.date_limit();

    uint32_t page(in.page());
    uint32_t page_size(in.page_size());

    LOGA("[GWJ] %s: start. info_id[%s], c_id[%s], info_type[%s], game_type[%s], data_limit[%s], page[%d], page_size[%d]",
         __FUNCTION__, info_id, c_id, info_type, game_type, data_limit, page, page_size);

    AbstractCompoundDecorator decorator;

    OR_CONDITION_TEMPLATE(info_id, "id", decorator)
    OR_CONDITION_TEMPLATE(c_id, "c_id", decorator)
    OR_CONDITION_TEMPLATE(info_type, "type_id", decorator)
    OR_CONDITION_TEMPLATE(game_type, "game_type", decorator)
    SMALLER_CONDITION_TEMPLATE(data_limit, "logdate", decorator)
    DECREASE_ORDER_CONDITION_TEMPLATE(dec_date, "logdate", decorator)

    MCPContentNews mcp_content_news("id");
    mcp_content_news.load_decorator(&decorator);
    mcp_content_news.set_unit_info(page_size,page);
    mcp_content_news.general_fetch_list_records(true);

    int ret_size = mcp_content_news.get_key_size();

    for(int i = 0; i < ret_size; i++)
    {
        game_info_t::set_outparameter_hall(out.m_result_params()->mutable_game_info_list(i),
                                      (MCPNewsInfo*)mcp_content_news.get_record_by_key_vec(i, 0));
    }

    ret_size = mcp_content_news.get_total_result_count();
    out.m_result_params()->set_count(ret_size);

    LOGA("[GWJ] %s: end. Total[%d]", __FUNCTION__, ret_size);
    gettimeofday(&end, NULL);
    LOGT(start, end);
    return 0;
}

int McpServlet::get_ip_province(const idl::mcp_get_ip_province_params& in, idl::mcp_get_ip_province_response& out)
{
	LOGD("[GWJ] %s: start", __FUNCTION__);

	struct timeval start, end;
	gettimeofday(&start, NULL);

    string ip = in.ip();
	vector<char*> target_address_p;
	CommonInterface::splic_string((char*)ip.c_str(), target_address_p, ".");
	int target_address_0 = atoi(target_address_p[0]);
	int target_address_1 = atoi(target_address_p[1]);
	int target_address_2 = atoi(target_address_p[2]);
	int target_address_3 = atoi(target_address_p[3]);

    LOGA("[GWJ] %s: start. ip[%s]", __FUNCTION__, ip.c_str());

	//string key = "ip_list";
	//list<string> record;
	//list<string>::iterator it ;

	extern list<string> g_ip_scope;
	list<string>::iterator it ;

	/*RedisModule redis_processor;*/
	//int result = redis_processor.get_value_list(key.c_str(), record);
	//if( result <= 0 )
	//{
		//out.m_result_params()->set_province(0);
		//gettimeofday(&end, NULL);
		//LOGT(start, end);
		//return 0;
	/*}*/

	//for(it = record.begin(); it != record.end(); it++)
	for(it = g_ip_scope.begin(); it != g_ip_scope.end(); it++)
	{
		string current_record = it->c_str();
		vector<char*> record_p;
		vector<char*> start_address_p;
		vector<char*> end_address_p;

		CommonInterface::splic_string((char*)current_record.c_str(), record_p, ",");

		char* ip_start = record_p[0];
		char* ip_end = record_p[1];
		char* ip_province = record_p[2];

		CommonInterface::splic_string(ip_start, start_address_p, ".");
		CommonInterface::splic_string(ip_end, end_address_p, ".");

		int start_address_0 = atoi(start_address_p[0]);
		int end_address_0 = atoi(end_address_p[0]);
		if( target_address_0 >= start_address_0  &&  target_address_0 <= end_address_0)
		{
			int start_address_1 = atoi(start_address_p[1]);
			int end_address_1 = atoi(end_address_p[1]);

			if( target_address_1 >= start_address_1  &&  target_address_1 <= end_address_1)
			{
				int start_address_2 = atoi(start_address_p[2]);
				int end_address_2 = atoi(end_address_p[2]);
				if( target_address_2 >= start_address_2  &&  target_address_2 <= end_address_2)
				{
					int start_address_3 = atoi(start_address_p[3]);
					int end_address_3 = atoi(end_address_p[3]);
					if( target_address_3 >= start_address_3  &&  target_address_3 <= end_address_3)
					{
						out.m_result_params()->set_province(atoi(ip_province));
						out.m_result_params()->set_ip_scope(it->c_str());
						gettimeofday(&end, NULL);
						LOGT(start, end);
						return 0;
					}
					else{continue;}
				}
				else{continue;}
			}
			else{continue;}
		}
		else{continue;}
	}

	out.m_result_params()->set_province(0);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;

}

int McpServlet::get_question_type_name(const idl::mcp_get_question_type_name_params& in, idl::mcp_get_question_type_name_response& out)
{
	//uint32_t page(in.page());
	//uint32_t page_size(in.page_size());
    //LOGA("[zx] %s: start. page[%d] page_size[%d]", __FUNCTION__, page, page_size);
    LOGA("[zx] %s: start. ", __FUNCTION__);

	struct timeval start, end;
	gettimeofday(&start, NULL);

	list<string> id_and_name;
	list<string>::iterator it;
	string question_key("question");

	RedisModule redis_processor;
	int result = redis_processor.get_values_by_key(question_key.c_str(), id_and_name);
	if(0 < result)
	{
		vector<string> tmp;
		int i = 0;
		for( it=id_and_name.begin(); it != id_and_name.end(); it++ )
		{
			tmp.clear();
			string id_name = it->c_str();
			CommonInterface::splic_string(id_name, tmp, ",");
			string id = tmp[0];
			string name = tmp[1];
			out.m_result_params()->mutable_question_type_name_list()->mutable_get(i)->set_id(id.c_str());
			out.m_result_params()->mutable_question_type_name_list()->mutable_get(i)->set_name(name.c_str());
			i++;
		}
		out.m_result_params()->set_count(i);
		return 0;
	}

	RedisModule redis_processor2;

	MysqlTable pt_game_question_type("pt_game_question_type", "PGQT");
	//pt_game_question_type.set_unit_info(page_size, page);
	pt_game_question_type.add_select_column("question_type_id");
	pt_game_question_type.add_select_column("question_type_name");

	pt_game_question_type.process_query_sql("dev_platform_mysql", false);
	int result_size = pt_game_question_type.get_total_count_without_page();

	unsigned int key_size = pt_game_question_type.get_total_key_count();

	for(int i=0; i<key_size; i++)
	{
		const char* id = pt_game_question_type.get_result_set_value(i, 0, "question_type_id");
		const char* name = pt_game_question_type.get_result_set_value(i, 0, "question_type_name");
		out.m_result_params()->mutable_question_type_name_list()->mutable_get(i)->set_id(id);
		out.m_result_params()->mutable_question_type_name_list()->mutable_get(i)->set_name(name);
		string value = string(id) + "," + name;
		redis_processor2.add_new_key_value(question_key.c_str(), value.c_str(), 30);
	}
	out.m_result_params()->set_count(key_size);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}


int McpServlet::get_user_question(const idl::mcp_get_user_question_params& in, idl::mcp_get_user_question_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	const char* user_id = in.user_id();

    //int page = in.unit_times();
    //int page_num = in.unit_num();

    LOGA("[GWJ] %s: start. user_id[%s]", __FUNCTION__, user_id);
	AbstractCompoundDecorator user_compound_decor;
	AbstractCompoundDecorator pt_game_compound_decor;

	PTGameUserQandA pt_game_user_QandA("user_id");
	PTGameQandA pt_game_QandA("id");

	IsDecorator user_id_decor;
	user_id_decor.add_field_filter_condition("user_id", user_id);
	user_compound_decor.add_primary_table_decorator(&user_id_decor);

	OrDecorator question_id_or_dec;

	pt_game_user_QandA.load_decorator(&user_compound_decor);
	pt_game_user_QandA.general_fetch_list_records(false);

    int size = pt_game_user_QandA.get_key_size();
	if(size == 0)
	{
		out.m_result_params()->set_count(0);
		LOGA("[zx] %s: count 0. No question record found", __FUNCTION__);
		return 0;
	}

	PTGameUserQARecord* pt_game_user_QandA_record = (PTGameUserQARecord*)pt_game_user_QandA.get_record_by_key_vec(0,0);
	const char* question_id = pt_game_user_QandA_record->question_id.c_str();
	string question_id_s(question_id);
	vector<string> tmp;
	vector<const char*> condition_list;
	vector<string>::iterator it;
	tmp.clear();
	CommonInterface::splic_string(question_id_s, tmp, ",");
	for( it=tmp.begin(); it != tmp.end(); it++ )
	{
		condition_list.push_back(it->c_str());
	}

	question_id_or_dec.set_same_field_filter_conditions(&condition_list, "id", true);
	pt_game_compound_decor.add_primary_table_decorator(&question_id_or_dec);
	pt_game_QandA.load_decorator(&pt_game_compound_decor);

	pt_game_QandA.general_fetch_list_records(false);
	size = pt_game_QandA.get_key_size();
	if(size == 0)
	{
		out.m_result_params()->set_count(0);
		LOGA("[zx] %s: count 0. No question record found", __FUNCTION__);
		return 0;
	}

	for(int i=0; i<size; i++)
	{
		PTGameQandARecord* pt_game_QandA_record = (PTGameQandARecord*)pt_game_QandA.get_record_by_key_vec(i,0);
		const char* id = pt_game_QandA_record->id.c_str();
		const char* app_id = pt_game_QandA_record->app_id.c_str();
		const char* question = pt_game_QandA_record->question.c_str();
		const char* status = pt_game_QandA_record->status.c_str();
		const char* answer = pt_game_QandA_record->answer.c_str();
		const char* date_q = pt_game_QandA_record->date_q.c_str();
		const char* date_a = pt_game_QandA_record->date_a.c_str();
		/*const char* id = pt_game_QandA.get_result_set_value(i, 0, "id");*/
		//const char* app_id = pt_game_QandA.get_result_set_value(i, 0, "app_id");
		/*const char* question = pt_game_QandA.get_result_set_value(i, 0, "question");*/
		//const char* question_type = pt_game_QandA.get_result_set_value(i, 0, "question_type");
		//const char* answer = pt_game_QandA.get_result_set_value(i, 0, "answer");
		//const char* phone = pt_game_QandA.get_result_set_value(i, 0, "phone");
		//const char* status = pt_game_QandA.get_result_set_value(i, 0, "status");
		//const char* enable = pt_game_QandA.get_result_set_value(i, 0, "status");
		//const char* date_q = pt_game_QandA.get_result_set_value(i, 0, "date_q");
		//const char* date_a = pt_game_QandA.get_result_set_value(i, 0, "date_a");
		//const char* remarks = pt_game_QandA.get_result_set_value(i, 0, "remarks");
		out.m_result_params()->mutable_user_question_answer_list()->mutable_get(i)->set_id(id);
		out.m_result_params()->mutable_user_question_answer_list()->mutable_get(i)->set_app_id(app_id);
		out.m_result_params()->mutable_user_question_answer_list()->mutable_get(i)->set_question(question);
		out.m_result_params()->mutable_user_question_answer_list()->mutable_get(i)->set_answer(answer);
		out.m_result_params()->mutable_user_question_answer_list()->mutable_get(i)->set_status(status);
		out.m_result_params()->mutable_user_question_answer_list()->mutable_get(i)->set_date_q(date_q);
		out.m_result_params()->mutable_user_question_answer_list()->mutable_get(i)->set_date_a(date_a);
	}

	out.m_result_params()->set_count(size);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}

int McpServlet::submit_question(const idl::mcp_submit_question_params& in, idl::mcp_submit_question_response& out)
{
	LOGD("-------------%s start-------------", __FUNCTION__ );

	struct timeval start, end;
	gettimeofday(&start, NULL);

	LOGA("[zx] %s: c_id[%s] question_type[%s], question[%s], phone[%s]", __FUNCTION__, in.c_id(), in.question_type(), in.question(), in.phone());

	MysqlTable pt_game_QandA("pt_game_QandA", "PGQA");
	pt_game_QandA["user_id"]->set_value( in.user_id() );
	pt_game_QandA["appid"]->set_value( in.c_id() );
	string question(in.question());
	DuokooMysql duokoo_mysql;
	string content_escape = duokoo_mysql.escapeString(question);
	pt_game_QandA["question"]->set_value( content_escape.c_str() );
	pt_game_QandA["question_type"]->set_value( in.question_type() );
	pt_game_QandA["phone"]->set_value( in.phone() );
	pt_game_QandA["enable"]->set_value( "1" );

	pt_game_QandA.add_new_insert_transaction();
	bool ret = pt_game_QandA.process_insert_transaction("dev_platform_mysql");
	if(ret == false)
	{
		LOGA("[GWJ] %s: process_insert_transaction Error!", __FUNCTION__);
		out.m_result_params()->set_res(-1);
		return -1;
	}

	out.m_result_params()->set_res(0);

	gettimeofday(&end, NULL);
	LOGT(start, end);

	LOGD("-------------%s end-------------", __FUNCTION__ );
	return 0;

}

// 获取最热的活动
int McpServlet::get_qp_top_activity(const idl::mcp_get_qp_top_activity_params& in, idl::mcp_get_qp_top_activity_response& out)
{

	struct timeval start, end;
	gettimeofday(&start, NULL);

	//const char* type = in.type();
	//int page = in.page();
	//int page_size = in.page_size();

    LOGA("[GWJ] %s: start. ", __FUNCTION__);
	AbstractCompoundDecorator qp_compound_decor;
	//AbstractCompoundDecorator qp_award_compound_decor;

	MCPContentQP mcp_content_qp("id");
	//MCPContentQPAward mcp_content_qp_award("activity_id");

	//mcp_content_qp.set_unit_info(page_size,page);
	IsDecorator qp_enable_decor;
	qp_enable_decor.add_field_filter_condition("enable", "1" );
	qp_compound_decor.add_primary_table_decorator(&qp_enable_decor);

	//OrderbyDecorator weight_order;
	//weight_order.order_type = -1;
	//weight_order.add_field_filter_condition("weight", NULL);
	//qp_compound_decor.add_primary_table_decorator(&weight_order);

	//OrderbyDecorator end_time_order;
	//end_time_order.order_type = -1;
	//end_time_order.add_field_filter_condition("end_time", NULL);
	//qp_compound_decor.add_primary_table_decorator(&end_time_order);

	IsDecorator is_top_decor;
	is_top_decor.add_field_filter_condition( "is_top", "1" );
	qp_compound_decor.add_primary_table_decorator(&is_top_decor);

	mcp_content_qp.load_decorator(&qp_compound_decor);
	mcp_content_qp.general_fetch_list_records(true);

    int size = mcp_content_qp.get_key_size();
	if(size == 0)
	{
		out.m_result_params()->set_count(0);
		LOGA("[zx] %s: count 0. No question record found", __FUNCTION__);
		return 0;
	}
	for(int i=0; i<size; i++)
	{
		MCPContentQPRecord* mcp_content_qp_record = (MCPContentQPRecord*)mcp_content_qp.get_record_by_key_vec(i,0);

		const char* activity_id = mcp_content_qp_record->id.c_str();
		const char* c_id = mcp_content_qp_record->c_id.c_str();
		const char* title = mcp_content_qp_record->title.c_str();
		const char* info = mcp_content_qp_record->info.c_str();
		const char* act_info = mcp_content_qp_record->act_info.c_str();
		const char* rules = mcp_content_qp_record->rules.c_str();
		const char* start_time = mcp_content_qp_record->start_time.c_str();
		const char* end_time = mcp_content_qp_record->end_time.c_str();
		const char* icon_url = mcp_content_qp_record->icon_url.c_str();
		const char* status = mcp_content_qp_record->status.c_str();

		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_id(activity_id);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_c_id(c_id);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_title(title);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_info(info);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_act_info(act_info);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_rules(rules);
		int64_t t = stringToTimestamp(string(start_time));
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_start_time(t*1000);
		int64_t t2 = stringToTimestamp(string(end_time));
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_end_time(t2*1000);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_icon_url(icon_url);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_status(status);

   /*     IsDecorator activity_id_decor;*/
		//activity_id_decor.add_field_filter_condition("activity_id", activity_id);
		//qp_award_compound_decor.add_primary_table_decorator(&activity_id_decor);
		//mcp_content_qp_award.load_decorator(&qp_award_compound_decor);
		//mcp_content_qp_award.general_fetch_list_records(false);

		//int size2 = mcp_content_qp_award.get_key_size();
		//for(int j=0; j<size2; j++)
		//{
			//MCPContentQPAwardRecord* mcp_content_qp_award_record = (MCPContentQPAwardRecord*)mcp_content_qp_award.get_record_by_key_vec(j,0);
			//const char* id = mcp_content_qp_award_record->id.c_str();
			//const char* name = mcp_content_qp_award_record->name.c_str();
			//const char* pic_url = mcp_content_qp_award_record->pic_url.c_str();

			//out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->mutable_qp_activity_award_list()->mutable_get(j)->set_id(id);
			//out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->mutable_qp_activity_award_list()->mutable_get(j)->set_name(name);
			//out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->mutable_qp_activity_award_list()->mutable_get(j)->set_pic_url(pic_url);
		//}
   }

    int result_size = mcp_content_qp.get_total_result_count();
	out.m_result_params()->set_count(result_size);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}

// 棋牌大厅获取活动列表
int McpServlet::get_qp_activity(const idl::mcp_get_qp_activity_params& in, idl::mcp_get_qp_activity_response& out)
{

	struct timeval start, end;
	gettimeofday(&start, NULL);

	const char* id = in.id();
	int page = in.page();
	int page_size = in.page_size();

    LOGA("[GWJ] %s: start. page[%d], page_size[%d]", __FUNCTION__, page, page_size);
	AbstractCompoundDecorator qp_compound_decor;

	MCPContentQP mcp_content_qp("id");

	mcp_content_qp.set_unit_info(page_size,page);
	IsDecorator qp_enable_decor;
	qp_enable_decor.add_field_filter_condition("enable", "1" );
	qp_compound_decor.add_primary_table_decorator(&qp_enable_decor);

	OrderbyDecorator weight_order;
	weight_order.order_type = -1;
	weight_order.add_field_filter_condition("weight", NULL);
	qp_compound_decor.add_primary_table_decorator(&weight_order);

	OrderbyDecorator end_time_order;
	end_time_order.order_type = -1;
	end_time_order.add_field_filter_condition("end_time", NULL);
	qp_compound_decor.add_primary_table_decorator(&end_time_order);

	IsDecorator id_is_decor;
	if( IS_VALUED_STRING( id ) )
	{
		id_is_decor.add_field_filter_condition( "id", id );
		qp_compound_decor.add_primary_table_decorator(&id_is_decor);
	}

	mcp_content_qp.load_decorator(&qp_compound_decor);
	mcp_content_qp.general_fetch_list_records(true);

    int size = mcp_content_qp.get_key_size();
	if(size == 0)
	{
		out.m_result_params()->set_count(0);
		LOGA("[zx] %s: count 0. No question record found", __FUNCTION__);
		return 0;
	}
	for(int i=0; i<size; i++)
	{
		MCPContentQPRecord* mcp_content_qp_record = (MCPContentQPRecord*)mcp_content_qp.get_record_by_key_vec(i,0);

		const char* activity_id = mcp_content_qp_record->id.c_str();
		const char* c_id = mcp_content_qp_record->c_id.c_str();
		const char* title = mcp_content_qp_record->title.c_str();
		const char* info = mcp_content_qp_record->info.c_str();
		const char* act_info = mcp_content_qp_record->act_info.c_str();
		const char* rules = mcp_content_qp_record->rules.c_str();
		const char* start_time = mcp_content_qp_record->start_time.c_str();
		const char* end_time = mcp_content_qp_record->end_time.c_str();
		const char* icon_url = mcp_content_qp_record->icon_url.c_str();
		const char* status = mcp_content_qp_record->status.c_str();

		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_id(activity_id);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_c_id(c_id);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_title(title);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_info(info);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_act_info(act_info);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_rules(rules);
		int64_t t = stringToTimestamp(string(start_time));
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_start_time(t*1000);
		int64_t t2 = stringToTimestamp(string(end_time));
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_end_time(t2*1000);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_icon_url(icon_url);
		out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->set_status(status);

		MCPContentQPAward mcp_content_qp_award("id");
		AbstractCompoundDecorator qp_award_compound_decor;

		IsDecorator activity_id_decor;
		activity_id_decor.add_field_filter_condition("activity_id", activity_id);
		qp_award_compound_decor.add_primary_table_decorator(&activity_id_decor);
		mcp_content_qp_award.load_decorator(&qp_award_compound_decor);
		mcp_content_qp_award.general_fetch_list_records(false);


		int size2 = mcp_content_qp_award.get_key_size();
		for(int j=0; j<size2; j++)
		{
			MCPContentQPAwardRecord* mcp_content_qp_award_record = (MCPContentQPAwardRecord*)mcp_content_qp_award.get_record_by_key_vec(j,0);
			const char* id = mcp_content_qp_award_record->id.c_str();
			const char* name = mcp_content_qp_award_record->name.c_str();
			const char* pic_url = mcp_content_qp_award_record->pic_url.c_str();

			out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->mutable_qp_activity_award_list()->mutable_get(j)->set_id(id);
			out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->mutable_qp_activity_award_list()->mutable_get(j)->set_name(name);
			out.m_result_params()->mutable_qp_activity_list()->mutable_get(i)->mutable_qp_activity_award_list()->mutable_get(j)->set_pic_url(pic_url);
		}
	}

    int result_size = mcp_content_qp.get_total_result_count();
	out.m_result_params()->set_count(result_size);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}

// 棋牌大厅根据游戏获取在线人数
int McpServlet::get_qp_activity_person(const idl::mcp_get_qp_activity_person_params& in, idl::mcp_get_qp_activity_person_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

    const char* c_ids = in.c_ids();
	string c_ids_new = string(c_ids) + ",0";
	const char* c_id = c_ids_new.c_str();

    LOGA("[zx] %s: start. c_ids[%s]", __FUNCTION__, c_ids);

	MCPContentQPPerson mcp_content_qp_person("id");
	AbstractCompoundDecorator qp_person_compound_decor;

	OR_CONDITION_TEMPLATE(c_id, "c_id", qp_person_compound_decor)

	mcp_content_qp_person.load_decorator(&qp_person_compound_decor);

	mcp_content_qp_person.general_fetch_list_records(false);

	int size = mcp_content_qp_person.get_key_size();
	if( size == 0 ) {
		out.m_result_params()->set_res(-1);
	}

	int total_person_num = 0;
	int j = 0;
	for( int i = 0; i < size; i++ )
	{
		MCPContentQPPersonRecord* mcp_content_qp_person_record =
			(MCPContentQPPersonRecord*)mcp_content_qp_person.get_record_by_key_vec(i, 0);

		const char* c_id = mcp_content_qp_person_record->c_id.c_str();
		const char* person_num = mcp_content_qp_person_record->person_num.c_str();
		if( !strcmp( "0", c_id ) )
		{
			out.m_result_params()->set_total_person_num( atoi( person_num ) );
		}
		else
		{
			//total_person_num += atoi( (char*)person_num);
			out.m_result_params()->mutable_qp_activity_person_list()->mutable_get(j)->set_c_id(c_id);
			out.m_result_params()->mutable_qp_activity_person_list()->mutable_get(j)->set_person_num(person_num);
			j++;
		}
	}

	out.m_result_params()->set_res( 0 );
	//out.m_result_params()->set_total_person_num( total_person_num );

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}

int McpServlet::get_person_all_awards(const idl::mcp_get_person_all_awards_params& in, idl::mcp_get_person_all_awards_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int page = in.page();
	int page_size = in.page_size();

	const char* user_id = in.user_id();
    LOGA( "[zx] %s start: user_id[%s]", __FUNCTION__, user_id);

	AbstractCompoundDecorator award_compound_decor;

	MCPContentPersonAward mcp_content_person_award("id");
	mcp_content_person_award.set_unit_info(page_size, page);


    OrderbyDecorator date_order;
	date_order.order_type = -1;
	date_order.add_field_filter_condition("date", NULL);

	award_compound_decor.add_primary_table_decorator(&date_order);

	SmallerThanDecorator smaller_logdate;
	smaller_logdate.add_field_filter_condition("date", "3000-09-20 15:05:00"); //logdate_order 是特殊的query，必须有常规的query，才能获取数据
	award_compound_decor.add_primary_table_decorator(&smaller_logdate);

	IsDecorator user_id_decor;
	user_id_decor.add_field_filter_condition("user_id", user_id);
	award_compound_decor.add_primary_table_decorator(&user_id_decor);

	//SmallerThanDecorator smaller_endtime;
	//smaller_endtime.add_field_filter_condition("end_time", ""); //logdate_order 是特殊的query，必须有常规的query，才能获取数据
	//award_compound_decor.add_primary_table_decorator(&smaller_endtime);

	mcp_content_person_award.load_decorator(&award_compound_decor);

	mcp_content_person_award.general_fetch_list_records(true);
	int size = mcp_content_person_award.get_key_size();
	if(size == 0)
	{
		out.m_result_params()->set_res(-1);
		out.m_result_params()->set_count(0);
		LOGA("[zx] %s: count 0. No mcp_content_person_award record found", __FUNCTION__);
		return 0;
	}

	for(int i = 0; i < size; i++)
	{
		MCPContentPersonAwardRecord* person_award_record =
				(MCPContentPersonAwardRecord*)mcp_content_person_award.get_record_by_key_vec(i, 0);

		const char* id = person_award_record->id.c_str();
		const char* icon = person_award_record->icon.c_str();
		const char* name = person_award_record->name.c_str();
		const char* type = person_award_record->type.c_str();
		const char* date = person_award_record->date.c_str();

		out.m_result_params()->mutable_person_all_award_list()->mutable_get(i)->set_id(id);
		out.m_result_params()->mutable_person_all_award_list()->mutable_get(i)->set_icon(icon);
		out.m_result_params()->mutable_person_all_award_list()->mutable_get(i)->set_name(name);
		int64_t t = stringToTimestamp(string(date));
		out.m_result_params()->mutable_person_all_award_list()->mutable_get(i)->set_date(t*1000);
		out.m_result_params()->mutable_person_all_award_list()->mutable_get(i)->set_type(type);
	}

	int count = mcp_content_person_award.get_total_result_count();
	out.m_result_params()->set_res(0);
	out.m_result_params()->set_count(count);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}

// 获取特定用户的排行
int McpServlet::get_person_award(const idl::mcp_get_person_award_params& in, idl::mcp_get_person_award_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

   /* int page = in.page();*/
    //int page_size = in.page_size();

	const char* user_id = in.user_id();
    LOGA( "[zx] %s start: user_id[%s]", __FUNCTION__, user_id);

	RedisModule redis_processor;
	int rank = redis_processor.get_rank_by_key(user_id, "user_score", 0);
	if( rank < 0 )
	{
		out.m_result_params()->set_res(-1);
		out.m_result_params()->set_rank(-1);
		LOGA("[zx] %s: count 0. No mcp_content_person_award record found.user_id[%s]", __FUNCTION__, user_id);
		return 0;
	}

	RedisModule redis_processor_2;
	map<string, string> record;
	map<string, string>::iterator it;

	string key = "award_person_highest:" + string(user_id);
	int size = redis_processor_2.get_record_by_key(key.c_str(), record);
	if( size != 0 )
	{
		out.m_result_params()->set_rank(-1);
		out.m_result_params()->set_res(-1);
		out.m_result_params()->set_count(0);
	}
	string id="", commodity_id="", kudou="",  priority="", name="", icon="";
	for( it = record.begin(); it != record.end(); it++ )
	{
		const char* key = (it->first).c_str();
		const char* value = (it->second).c_str();
		if( !strcmp( "id", key ) ){ id = value; }
		else if( !strcmp( "commodity_id", key ) ){ commodity_id = value; }
		else if( !strcmp( "kudou", key ) ){ kudou = value; }
		else if( !strcmp( "priority", key ) ){ priority = value; }
		else if( !strcmp( "name", key ) ){ name = value; }
		else if( !strcmp( "icon", key ) ){ icon = value; }
	}
	out.m_result_params()->mutable_person_award_list()->mutable_get(0)->set_id(id.c_str());
	out.m_result_params()->mutable_person_award_list()->mutable_get(0)->set_user_id(user_id);
	out.m_result_params()->mutable_person_award_list()->mutable_get(0)->set_commodity_id(commodity_id.c_str());
	out.m_result_params()->mutable_person_award_list()->mutable_get(0)->set_name(name.c_str());
	out.m_result_params()->mutable_person_award_list()->mutable_get(0)->set_kudou(kudou.c_str());
	out.m_result_params()->mutable_person_award_list()->mutable_get(0)->set_priority(priority.c_str());
	out.m_result_params()->mutable_person_award_list()->mutable_get(0)->set_icon(icon.c_str());

	out.m_result_params()->set_rank(rank + 1);
	out.m_result_params()->set_res(0);
	out.m_result_params()->set_count(1);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
    //AbstractCompoundDecorator award_compound_decor;

	//MCPContentPersonAward mcp_content_person_award("user_id");
    //mcp_content_person_award.set_unit_info(page_size, page);


   /* OrderbyDecorator priority_order;*/
	//priority_order.order_type = -1;
	//priority_order.add_field_filter_condition("priority", NULL);
	//award_compound_decor.add_primary_table_decorator(&priority_order);

	//SmallerThanDecorator smaller_logdate;
	//smaller_logdate.add_field_filter_condition("date", "3000-09-20 15:05:00"); //logdate_order 是特殊的query，必须有常规的query，才能获取数据
	//award_compound_decor.add_primary_table_decorator(&smaller_logdate);

	//IsDecorator user_id_decor;
	//user_id_decor.add_field_filter_condition("user_id", user_id);
	//award_compound_decor.add_primary_table_decorator(&user_id_decor);

	/* SmallerThanDecorator smaller_endtime; */
	/* smaller_endtime.add_field_filter_condition("end_time", ""); //logdate_order 是特殊的query，必须有常规的query，才能获取数据 */
	/* champion_compound_decor.add_primary_table_decorator(&smaller_endtime); */

    /*mcp_content_person_award.load_decorator(&award_compound_decor);*/

    //mcp_content_person_award.general_fetch_list_records(true);
    //int size = mcp_content_person_award.get_key_size();
	//if(size == 0)
	//{
		//out.m_result_params()->set_res(-1);
		//out.m_result_params()->set_rank(-1);
		//LOGA("[zx] %s: count 0. No mcp_content_person_award record found", __FUNCTION__);
		//return 0;
	//}

	//for(int i = 0; i < size; i++)
	//{
		//MCPContentPersonAwardRecord* person_award_record =
				//(MCPContentPersonAwardRecord*)mcp_content_person_award.get_record_by_key_vec(i, 0);

		//const char* id = person_award_record->id.c_str();
		//const char* user_id = person_award_record->user_id.c_str();
		//const char* commodity_id = person_award_record->commodity_id.c_str();
		//const char* kudou = person_award_record->kudou.c_str();
		//const char* priority = person_award_record->priority.c_str();
		//const char* name = person_award_record->name.c_str();

		//out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_id(id);
		//out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_user_id(user_id);
		//out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_commodity_id(commodity_id);
		//out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_name(name);
		//out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_kudou(kudou);
		//out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_priority(priority);
	//}
}

// 获取奖品排行
int McpServlet::get_person_award_list(const idl::mcp_get_person_award_list_params& in, idl::mcp_get_person_award_list_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int page = in.page();
	int page_size = in.page_size();

	LOGA( "[zx] %s start: page_size[%d]", __FUNCTION__, page_size);

	AbstractCompoundDecorator award_compound_decor;

	LogicOrDecorator logic_or_decorator;

	MCPContentPersonAward mcp_content_person_award("id");
	mcp_content_person_award.set_unit_info(page_size, page);

	OrderbyDecorator order_by_condition;

	order_by_condition.order_type = -1;
	order_by_condition.add_field_filter_condition("priority", NULL);
	order_by_condition.add_field_filter_condition("kudou", NULL);
	award_compound_decor.add_primary_table_decorator(&order_by_condition);

	//OrderbyDecorator priority_order;
	//priority_order.order_type = -1;
	//priority_order.add_field_filter_condition("priority", NULL);
	//award_compound_decor.add_primary_table_decorator(&priority_order);

	//OrderbyDecorator kudou_order;
	//kudou_order.order_type = -1;
	//kudou_order.add_field_filter_condition("kudou", NULL);
	//award_compound_decor.add_primary_table_decorator(&kudou_order);

	//SmallerThanDecorator smaller_logdate;
	//smaller_logdate.add_field_filter_condition("date", "3000-09-20 15:05:00"); //logdate_order 是特殊的query，必须有常规的query，才能获取数据
	//award_compound_decor.add_primary_table_decorator(&smaller_logdate);

	BiggerThanIntDecorator bigger_priority;
	bigger_priority.add_field_filter_condition("priority", "0");
	logic_or_decorator.add_primary_table_decorator(&bigger_priority);

	BiggerThanIntDecorator bigger_kudou;
	bigger_kudou.add_field_filter_condition("kudou", "500000");
	logic_or_decorator.add_primary_table_decorator(&bigger_kudou);

	award_compound_decor.add_primary_table_decorator(&logic_or_decorator);

	mcp_content_person_award.load_decorator(&award_compound_decor);

	mcp_content_person_award.general_fetch_list_records(true);
	int size = mcp_content_person_award.get_key_size();
	if(size == 0)
	{
		out.m_result_params()->set_res(-1);
		LOGA("[zx] %s: count 0. No mcp_content_person_award record found", __FUNCTION__);
		return 0;
	}

	for(int i = 0; i < size; i++)
	{
		MCPContentPersonAwardRecord* person_award_record =
				(MCPContentPersonAwardRecord*)mcp_content_person_award.get_record_by_key_vec(i, 0);

		const char* id = person_award_record->id.c_str();
		const char* user_id = person_award_record->user_id.c_str();
		const char* commodity_id = person_award_record->commodity_id.c_str();
		const char* name = person_award_record->name.c_str();
		const char* kudou = person_award_record->kudou.c_str();
		const char* priority = person_award_record->priority.c_str();
		const char* icon = person_award_record->icon.c_str();

		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_id(id);
		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_user_id(user_id);
		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_commodity_id(commodity_id);
		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_name(name);
		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_kudou(kudou);
		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_priority(priority);
		if( page == 0 ){ page = 1; }
		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_rank( (page-1) * page_size + i + 1 );
		out.m_result_params()->mutable_person_award_list()->mutable_get(i)->set_icon( icon );
	}

	int count = mcp_content_person_award.get_total_result_count();
	out.m_result_params()->set_res(0);
	out.m_result_params()->set_count(count);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}


//获取大转盘奖品列表
int McpServlet::get_award_list(const idl::mcp_get_award_list_params& in, idl::mcp_get_award_list_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	const char* award_channel = in.award_channel(); // "1"-大转盘 "0"-刮刮乐
    LOGA("[zx] %s: start. award_channel[%s]", __FUNCTION__, award_channel);
	uint32_t page_size= in.page_size();

	int count=0;
	time_t t;
	t = time(0);

	list<string> keys;
	list<string>::iterator it;
	string key = "award_list";

	RedisModule redis_processor;
	int result = redis_processor.get_values_by_key( key.c_str(), keys );
	if(0 > result)
	{
		out.m_result_params()->set_count(0);
		out.m_result_params()->set_res(-1);
		return 0;
	}
	//for( int i = 0; i < result; i++ )
	for ( it=keys.begin(); it != keys.end(); it++ )
	{
		string award_id = it->c_str();
		//keys.pop_front();
		map<string, string> record;

		RedisModule redis_processor_2;
		int result2 = redis_processor_2.get_record_by_key(award_id.c_str(), record);

		string id, type, begin_time, end_time, store_num, sell_num, name,
			   prize_name, icon, auto_expired_type, is_slyder;

		map<string, string>::iterator it;
		for( it = record.begin(); it != record.end(); it++ )
		{
			const char* key = (it->first).c_str();
			const char* value = (it->second).c_str();
			if( !strcmp( "type", key ) ){ type = value; }//奖品类型
			else if( !strcmp( "id", key ) ){ id = value; }
			else if( !strcmp( "begin_time", key ) ){ begin_time = value; }
			else if( !strcmp( "end_time", key ) ){ end_time = value; }
			else if( !strcmp( "store_num", key ) ){ store_num = value; }
			else if( !strcmp( "sell_num", key ) ){ sell_num = value; }
			else if( !strcmp( "name", key ) ){ name = value; }
			//else if( !strcmp( "prize_name", key ) ){ prize_name = value; }
			else if( !strcmp( "icon", key ) ){ icon = value; }
			else if( !strcmp( "auto_expired_type", key ) ){ auto_expired_type = value; }
			else if( !strcmp( "is_slyder", key ) ){ is_slyder= value; }
			//else if( !strcmp( "prize_url", key ) ){ prize_url = value; }
		}

		if( !strcmp( "0", is_slyder.c_str() ) ) //如果是刮刮乐，不显示
		{
			continue;
		}

		if( t < stringToTimestamp(begin_time) || t > stringToTimestamp(end_time) ) //不在时间范围内，不显示
		{
			continue;
		}

		out.m_result_params()->mutable_award_list()->mutable_get(count)->set_type(type.c_str());
		out.m_result_params()->mutable_award_list()->mutable_get(count)->set_award_id(id.c_str());
		out.m_result_params()->mutable_award_list()->mutable_get(count)->set_name(name.c_str());
		//out.m_result_params()->mutable_award_list()->mutable_get(count)->set_prize_name(prize_name.c_str());
		out.m_result_params()->mutable_award_list()->mutable_get(count)->set_icon(icon.c_str());
		//out.m_result_params()->mutable_award_list()->mutable_get(count)->set_prize_url(prize_url.c_str());

		count++;
		if( count == page_size )
		{break;}

	}

	out.m_result_params()->set_count(count);
	out.m_result_params()->set_res(0);

	gettimeofday(&end, NULL);
	LOGT(start, end);
	return 0;
}

// 用户抢得奖品，大转盘或者刮刮乐
int McpServlet::grab_award(const idl::mcp_grab_award_params& in, idl::mcp_grab_award_response& out)
{
	struct timeval thread_start, thread_end;
	gettimeofday(&thread_start, NULL);

	const char* user_id = in.user_id();
	const char* award_channel = in.award_channel();
    LOGA("[zx] %s: start. user_id[%s], award_channel[%s]", __FUNCTION__, user_id, award_channel);

	int res = 0;

	redisContext *c;
do{

	//beg 得到随机数

	long random = DuokooTools::getRandomNum(1000000);

	//end 得到随机数

	//beg 根据随机数二分查找award_id

	//beg 先得到list长度

	const char* key = "award_probability";
	int length=0;
    c=redisConnPool::getInstance()->getConnection();
	if( NULL == c )
	{
        LOGA( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		res = -1;
		break;
	}
    redisReply *reply;

	reply = (redisReply*)redisCommand(c, "LLEN %s", key);
	if(reply!=NULL)
	{
		if(reply->type == REDIS_REPLY_INTEGER)
		{
			LOGD( "redis key[%s] length[%d]", key, reply->integer );
			length=reply->integer;
			freeReplyObject(reply);
			reply = NULL;
		}
		else
		{
			LOGA( "[zx] redis key[%s] not exist ", key );
			freeReplyObject(reply);
			reply = NULL;
			res = -1; // 键 award_probability 不存在
			break;
		}
	}
	else
	{
		LOGA( "[zx] redis key[%s] error ", key );
		res = -1;
		break;
	}

	//end 先得到list长度

	//beg二分查找到所在范围

	int start = 0;
	int end = length -1;
	bool found = false;
	string id = "";

	while(start <= end)
	{
		int middle = start + ((end - start) >> 1);
		reply = (redisReply*)redisCommand(c, "LINDEX %s %d", key, middle);
		string value = reply->str;
		vector<char*> id_probability;
		CommonInterface::splic_string((char*)value.c_str(), id_probability, ",");
		id = id_probability[0];
		int probability_begin = atoi(id_probability[1]);
		int probability_end = atoi(id_probability[2]);

		if(random >= probability_begin && random <= probability_end)  {
			found = true;
			break;
		} else if(random < probability_begin){
			end = middle -1;
		} else if(random > probability_end){
			start = middle + 1;
		}
		freeReplyObject(reply);
	}

	//end二分查找到所在范围

	//end 根据随机数二分查找award_id

	if( !found ) {
		res = -2; // 不满足概率
		LOGA("[zx] %s: probability not satisfy! random[%ld]", __FUNCTION__, random);
		break;
	}

	//beg 根据award_id查找award信息

	string award_id = "award_info_" + id;
	string type="", start_time="", end_time="", store_num_s="", sell_num_s="", name="", prize_name="",
		   prize_url="", auto_expired_type="", is_slyder="", icon="", func_desc="", kudou="";
	int store_num=0, sell_num=0, kudou_num=0;

	reply = (redisReply *)redisCommand(c, "watch %s ", award_id.c_str());
	FREE_REDIS_REPLY(reply)

	reply = (redisReply *)redisCommand(c, "HMGET %s %s %s %s %s %s %s %s %s %s %s %s %s",
			award_id.c_str(), "type", "begin_time", "end_time", "store_num", "sell_num",
			"name", "auto_expired_type", "is_slyder", "func_desc", "prize_url", "icon", "kudou");

    if(reply!=NULL)
	{
		type = reply->element[0]->str;
		start_time = reply->element[1]->str;
		end_time = reply->element[2]->str;
		store_num_s = reply->element[3]->str;
		sell_num_s = reply->element[4]->str;
		name = reply->element[5]->str;
		//prize_name = reply->element[6]->str;
		//prize_url = reply->element[7]->str;
		auto_expired_type = reply->element[6]->str;
		is_slyder = reply->element[7]->str;
		func_desc = reply->element[8]->str;
		prize_url = reply->element[9]->str;
		icon = reply->element[10]->str;
		kudou = reply->element[11]->str;
		kudou_num = atoi((char*)kudou.c_str());
		store_num = atoi((char*)store_num_s.c_str());
		sell_num = atoi((char*)sell_num_s.c_str());
		freeReplyObject(reply);
	}else {
		res = -16;
		LOGA("[zx] %s: award[%s] not found!", __FUNCTION__, award_id.c_str());
		break;
	}

	//beg 如果是大转盘，则必须保证is_slyder为1

	if( !strcmp( award_channel, "1" )) { //大转盘
		if( strcmp( is_slyder.c_str(), "1" ) ) { // 奖品不属于大转盘
			res = -14;
			LOGA("[zx] %s: award is not slyder!", __FUNCTION__);
			break;
		}
	}

	//end 如果是大转盘，则必须保证is_slyder为1

	//beg 如果资源是图片或者音乐，还需要取得名称和url及icon

	if( !strcmp(type.c_str(), "4") )
	{
		reply = (redisReply *)redisCommand(c, "SRANDMEMBER award_prize");
		if( reply != NULL )
		{
			if( reply->type != REDIS_REPLY_NIL )
			{
				vector<char*> value_list;
				string value = reply->str;
				int value_list_size = CommonInterface::splic_string((char*)value.c_str(),
				   	value_list, ",");
				if( value_list_size != 12 )
				{
					res = -15;
					LOGA("[zx] %s: award_prize field num err!", __FUNCTION__);
					break;
				}
				prize_name = value_list[7];
				icon = value_list[8];
				prize_url = value_list[9];
			}
		}
		freeReplyObject(reply);
	}

	//end 如果资源是图片或者音乐，还需要取得名称和url及icon

	//beg 如果奖品不是图片，则需要保证用户一天只抢一次奖品

   /* string user_id_type = "user_id_award_type:" + string(user_id) + type;*/
	//if( strcmp(type.c_str(), "4") ){
		//reply = (redisReply*)redisCommand(c, "get %s", user_id_type.c_str());
		//if( reply->type != REDIS_REPLY_NIL ) {
			//res = -13;
			//LOGA("[zx] %s: same user grab same award!", __FUNCTION__);
			//FREE_REDIS_REPLY(reply)
			//break;
		//}
		//FREE_REDIS_REPLY(reply)
	//}

	//end 如果奖品不是图片，则需要保证用户一天只抢一次奖品

	//beg比较开始时间、结束时间
	time_t t;
	t=time(0);
	if( t < stringToTimestamp(start_time) ){
		LOGA( "award[%s] does not start[%s]", award_id.c_str(), start_time.c_str() );
		res = -3; // 奖品还未开始
		break;
	}
	if( t > stringToTimestamp(end_time) ){
		LOGA( "award[%s] has end[%s]", award_id.c_str(), end_time.c_str() );
		res = -4; // 奖品已经结束
		break;
	}
	//end比较开始时间、结束时间

	//beg 比较奖品数量
	if( store_num <= sell_num ) {
		LOGA( "award[%s] has send over ", award_id.c_str() );
		res = -5; // 奖品发完
		break;
	}
	//end 比较奖品数量

	reply = (redisReply *)redisCommand(c, "multi");
	FREE_REDIS_REPLY(reply)



	reply = (redisReply *)redisCommand(c, "HINCRBY %s %s 1", award_id.c_str(), "sell_num" );
	FREE_REDIS_REPLY(reply)

	//beg如果是兑奖码还要对兑奖码进行操作

	if( !strcmp(type.c_str(), "1") ){
		reply = (redisReply*)redisCommand( c, "SPOP award_num_%s", id.c_str() );
	}
	FREE_REDIS_REPLY(reply)

	//end如果是兑奖码还要对兑奖码进行操作

	string num;
	redisReply* reply2;
	reply2 = (redisReply *)redisCommand(c, "exec");
	if( reply2 != NULL )
	{
       if(reply2->elements > 2 || reply2->elements == 0)
       {
           LOGA("[zx] %s: UNKNOW ERROR! ELEMNETS is %d", __FUNCTION__, reply2->elements);
           res = -18; // redis 错误
           FREE_REDIS_REPLY(reply2)
           break;
       }

       if( !strcmp(type.c_str(), "1") )
       {
           if( reply2 ->elements != 2 )
           {
               LOGA("[zx] %s: redis exec error!", __FUNCTION__);
               res = -6; // redis 错误
               FREE_REDIS_REPLY(reply2)
                   break;
           }
           if( reply2->element[1]->type != REDIS_REPLY_STRING )
           {
               LOGA("[zx] %s: num not exist!", __FUNCTION__);
               res = -7; // redis 错误
               FREE_REDIS_REPLY(reply2)
                   break;
           }
           num = reply2->element[1]->str;
           if( !IS_VALUED_STRING(num.c_str()) )
           {
               LOGA("[zx] %s: num not valid!", __FUNCTION__);
               res = -8; // 没有号码了
               FREE_REDIS_REPLY(reply2)
                   break;
           }
           if( reply2->element[0]->type != REDIS_REPLY_INTEGER )
           {
               res = -12;
               LOGA("[zx] %s: incre redis error!", __FUNCTION__);
               FREE_REDIS_REPLY(reply2)
                   break;
           }
       }
       else
       {
           if( reply2->element[0]->type != REDIS_REPLY_INTEGER )
           {
               res = -12;
               LOGA("[zx] %s: incre redis error!", __FUNCTION__);
               FREE_REDIS_REPLY(reply2)
                   break;
           }
       }
    }

    FREE_REDIS_REPLY(reply2)

        //end 得到奖品

        //beg将用户抢到的奖品信息入库

        if( !strcmp(type.c_str(), "2" ) || !strcmp(type.c_str(), "3" ) || !strcmp(type.c_str(), "1") )
        {
            MysqlTable props_user("props_user", "PU");
            props_user["user_id"]->set_value( user_id );
            props_user["commodity_id"]->set_value( id.c_str());
            props_user["award_channel"]->set_value( award_channel );
            props_user["kudou"]->set_value( kudou_num );

            props_user.add_new_insert_transaction();
            bool ret = props_user.process_insert_transaction("default_mysql");
            if(ret == false)
            {
                LOGA("[zx] %s: process_insert_transaction Error!", __FUNCTION__);
                res = -9;
                break;
            }
        }

    if( !strcmp(type.c_str(), "1" ))
    {
        DuokooMysql mysql("default_mysql");
        if(!mysql.startTransaction()){
            res = -10;
            break;
        }
        string query_sql="UPDATE `MCP`.`props_exchange_code` SET `type` = 1, `user_id` = \'" + string(user_id) + "\' WHERE `number` =\'" + num + "\' and commodity_id = " + id;
        UB_LOG_DEBUG("query_sql:[%s]", query_sql.c_str());
        int res=mysql.query(query_sql);
        if(res<0){
            LOGA("[zx] %s: update_transaction Error!", __FUNCTION__);
            mysql.rollback();
            res = -11;
            break;
        }
		mysql.commit();
    }

    DuokooMysql mysql("default_mysql");
    if(!mysql.startTransaction()){
        res = -10;
        break;
    }
    string query_sql = "UPDATE `MCP`.`props_commodity` SET `sell_num` = sell_num + 1  WHERE `id` = " + id;
    UB_LOG_DEBUG("query_sql:[%s]", query_sql.c_str());
    int res=mysql.query(query_sql);
    if(res<0){
        LOGA("[zx] %s: update_transaction Error!", __FUNCTION__);
        mysql.rollback();
        res = -11;
        break;
    }
	mysql.commit();
    //end将用户抢到的奖品信息入库

   /* if( strcmp(type.c_str(), "4") ){*/
        //reply = (redisReply*)redisCommand(c, "SETNX %s %s", user_id_type.c_str(), "true");
        //FREE_REDIS_REPLY(reply)

            //reply = (redisReply*)redisCommand(c, "EXPIRE %s %ld", user_id_type.c_str(), 24*60*60);
        //FREE_REDIS_REPLY(reply)
    //}

    //end 如果奖品不是图片，则需要保证用户一天只抢一次奖品

    //beg 通知中间层抢的奖品信息

    //CommunityClient communityClient;
    int32_t userId = atoi(user_id);
    int32_t commodity_id = atoi(award_id.c_str());
    int32_t id2middle = atoi(id.c_str());
    int32_t expiredType = atoi(auto_expired_type.c_str());
    bool bRes;

    if(strcmp(type.c_str(), "1"))
    {
        //非兑换码
        //res = communityClient.obtain_commodity(userId, commodity_id, 1, "0", bRes);
        res = community_client.obtain_commodity(userId, id2middle, 1, "0", bRes);
        if(res == 0)
        {
            LOGA("obtain_commodity success %d", res);
        }
        else
        {
            LOGA("obtain_commodity fail %d", res);
            res = -17;
            break;
        }
    }
    else
    {
        //res = communityClient.obtain_exchange_code(userId, num, commodity_id, end_time, expiredType, "0", bRes);
        time_t tt = DuokooTools::stringToTimestamp(end_time);
        std::stringstream ss;
        ss << tt;
        string time_stamp = ss.str();

        res = community_client.obtain_exchange_code(userId, num, id2middle, time_stamp, expiredType, "0", bRes);
        if(res == 0)
        {
            LOGA("obtain_exchange_code success %d num is %s", res, num.c_str());
        }
        else
        {
            LOGA("obtain_exchange_code fail %d num is %s", res, num.c_str());
            res = -17;
            break;
        }
    }

    //end 通知中间层抢的奖品信息

    out.m_result_params()->set_res(1);
    out.m_result_params()->set_user_id(user_id);
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_type(type.c_str());
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_award_id(id.c_str());
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_name(name.c_str());
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_prize_name(prize_name.c_str());
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_prize_url(prize_url.c_str());
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_func_desc(func_desc.c_str());
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_prize_url(prize_url.c_str());
    out.m_result_params()->mutable_award_list()->mutable_get(0)->set_icon(icon.c_str());

    redisConnPool::getInstance()->releaseConnection(c);

    LOGA("[zx] %s success! res[%d], user_id[%s], award_id[%s], type[%s], store_num[%d], sell_num[%d], num[%s], name[%s], prize_name[%s], prize_url[%s], icon[%s] ",
            __FUNCTION__, res, user_id, id.c_str(), type.c_str(), store_num, sell_num, num.c_str(), name.c_str(), prize_name.c_str(), prize_url.c_str(), icon.c_str());

	gettimeofday(&thread_end, NULL);
	LOGT(thread_start, thread_end);
    return 0;

}while (0);

	if( res == -1 || res == -16 || res == -15 ||
			res == -6 || res == -7 ||
			res == -12 )
	{
		res = -1; // redis 错误
	}
	else if( res == -13 || res == -2 || res == -14 || res == -17 ||
			res == -15 || res == -8 || res == -3 || res == -4 || res == -5 || res == -18)
	{
		res = 0;
	}
	else if( res == -9 || res == -10 || res == -11 )
	{
		res = -2; // 数据库错误
	}

	redisConnPool::getInstance()->releaseConnection(c);
	out.m_result_params()->set_res(res);
	gettimeofday(&thread_end, NULL);
	LOGT(thread_start, thread_end);
	return 0;
}

int McpServlet::test_redis(const idl::mcp_test_redis_params& in, idl::mcp_test_redis_response& out)
{
    redisContext *c;
    redisReply *reply;
    int res;
    c=redisConnPool::getInstance()->getConnection();
    reply = (redisReply *)redisCommand(c, "multi");

	FREE_REDIS_REPLY(reply)



    reply = (redisReply *)redisCommand(c, "HINCRBY award_info_331 %s 1", "sell_num" );
    FREE_REDIS_REPLY(reply)

        //beg如果是兑奖码还要对兑奖码进行操作

        //if( !strcmp(type.c_str(), "1") ){
    reply = (redisReply*)redisCommand( c, "SPOP award_num_331" );
        //}
    FREE_REDIS_REPLY(reply)

        //end如果是兑奖码还要对兑奖码进行操作

    string num;
    redisReply* reply2;
    reply2 = (redisReply *)redisCommand(c, "exec");
    LOGA("ELEMENTS IS %d", reply2->elements);
    if( reply2 != NULL )
    {
        if(reply2->elements > 2)
        {
            LOGA("[zx] %s: UNKNOW ERROR! ELEMNETS is %d", __FUNCTION__, reply2->elements);

            res = -18; // redis 错误
            FREE_REDIS_REPLY(reply2)
            return 0;

                //break;
        }
    }
    redisConnPool::getInstance()->releaseConnection(c);
    out.m_result_params()->set_res(reply2->elements);
    if(reply2->element[1] != NULL && reply2->element[1]->type == REDIS_REPLY_STRING)
        out.m_result_params()->set_num(reply2->element[1]->str);
    FREE_REDIS_REPLY(reply2)

    return 0;
}


int McpServlet::get_qp_rule(const idl::mcp_get_qp_rule_params& in, idl::mcp_get_qp_rule_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

    string type_id(in.type());
    //uint32_t page(in.page());
    //uint32_t page_size(in.page_size());

    LOGA( "get_qp_rule: type[%s]", type_id.c_str());

    MysqlTable mcp_content_qp_rules("mcp_content_qp_rules", "MCQR");
    //MysqlTable mcp_content_user_grab("mcp_content_user_grab", "CUGRB");
	//mcp_content_qp_rules.set_unit_info(page_size,page);

    mcp_content_qp_rules.add_select_column("id");
    mcp_content_qp_rules.add_select_column("rule");
    mcp_content_qp_rules.add_select_column("describe");
    mcp_content_qp_rules.add_select_column("button_text");
    mcp_content_qp_rules.add_select_column("free_num");

	mcp_content_qp_rules["enable"]->add_and_condition("=", 1 );

	if( !type_id.empty() )
	{
    	mcp_content_qp_rules["type"]->add_and_condition("=", type_id.c_str() );
	}

	mcp_content_qp_rules.process_query_sql("default_mysql", true);
    int result_size = mcp_content_qp_rules.get_total_count_without_page();

	const char* id = mcp_content_qp_rules.get_result_set_value(0, 0, "MCQR.id");
	const char* rule = mcp_content_qp_rules.get_result_set_value(0, 0, "MCQR.rule");
	const char* describe = mcp_content_qp_rules.get_result_set_value(0, 0, "MCQR.describe");
	const char* button_text = mcp_content_qp_rules.get_result_set_value(0, 0, "MCQR.button_text");
	const char* free_num = mcp_content_qp_rules.get_result_set_value(0, 0, "MCQR.free_num");

	out.m_result_params()->set_count(result_size);
	out.m_result_params()->set_id(id);
	out.m_result_params()->set_rule(rule);
	out.m_result_params()->set_describe(describe);
	out.m_result_params()->set_button_text(button_text);
	out.m_result_params()->set_free_num(free_num);

    LOGA( "get_qp_rule end item_count[%d]", result_size);

    gettimeofday(&end, NULL);
    LOGT(start, end);
    return 0;
}

int McpServlet::get_qp_basic_info(const idl::mcp_get_qp_basic_info_params& in , idl::mcp_get_qp_basic_info_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

    //string type_id(in.type());
    //uint32_t page(in.page());
    //uint32_t page_size(in.page_size());

    LOGA( "%s start", __FUNCTION__);

    MysqlTable mcp_content_qp_basic_info("mcp_content_qp_basic_info", "MCQBI");
    //MysqlTable mcp_content_user_grab("mcp_content_user_grab", "CUGRB");
	//mcp_content_qp_basic_info.set_unit_info(page_size,page);

    mcp_content_qp_basic_info.add_select_column("id");
    mcp_content_qp_basic_info.add_select_column("bg_url");
    mcp_content_qp_basic_info.add_select_column("bg_sound");
    mcp_content_qp_basic_info.add_select_column("type");
    mcp_content_qp_basic_info.add_select_column("in_bg_url");
    mcp_content_qp_basic_info.add_select_column("kudou_rank_rule");
    mcp_content_qp_basic_info.add_select_column("prize_rank_rule");

	mcp_content_qp_basic_info["enable"]->add_and_condition("=", 1 );

	mcp_content_qp_basic_info.process_query_sql("default_mysql", true);
    int result_size = mcp_content_qp_basic_info.get_total_count_without_page();

	const char* id = mcp_content_qp_basic_info.get_result_set_value(0, 0, "MCQBI.id");
	const char* bg_url = mcp_content_qp_basic_info.get_result_set_value(0, 0, "MCQBI.bg_url");
	const char* bg_sound = mcp_content_qp_basic_info.get_result_set_value(0, 0, "MCQBI.bg_sound");
	const char* type = mcp_content_qp_basic_info.get_result_set_value(0, 0, "MCQBI.type");
	const char* in_game_url = mcp_content_qp_basic_info.get_result_set_value(0, 0, "MCQBI.in_bg_url");
	const char* kudou_rank_rule = mcp_content_qp_basic_info.get_result_set_value(0, 0, "MCQBI.kudou_rank_rule");
	const char* prize_rank_rule = mcp_content_qp_basic_info.get_result_set_value(0, 0, "MCQBI.prize_rank_rule");

	out.m_result_params()->set_count(result_size);
	out.m_result_params()->set_id(id);
	out.m_result_params()->set_bg_url(bg_url);
	out.m_result_params()->set_bg_sound(bg_sound);
	out.m_result_params()->set_in_game_type(type);
	out.m_result_params()->set_in_game_url(in_game_url);
	out.m_result_params()->set_kudou_rank_rule(kudou_rank_rule);
	out.m_result_params()->set_prize_rank_rule(prize_rank_rule);


	MysqlTable mcp_content_qp_tieba( "mcp_content_qp_tieba", "MCQT" );

	mcp_content_qp_tieba.add_select_column("id");
	mcp_content_qp_tieba.add_select_column("name");
	mcp_content_qp_tieba.add_select_column("icon");

	mcp_content_qp_tieba["enable"]->add_and_condition("=", 1 );
	mcp_content_qp_tieba.process_query_sql("default_mysql", true);
    int result_size2 = mcp_content_qp_tieba.get_total_count_without_page();

	for( int i=0; i<result_size2; i++ )
	{
		const char* id = mcp_content_qp_tieba.get_result_set_value(i, 0, "MCQT.id");
		const char* name = mcp_content_qp_tieba.get_result_set_value(i, 0, "MCQT.name");
		const char* icon = mcp_content_qp_tieba.get_result_set_value(i, 0, "MCQT.icon");

		out.m_result_params()->mutable_tie_ba_list()->mutable_get(i)->set_id(id);
		out.m_result_params()->mutable_tie_ba_list()->mutable_get(i)->set_name(name);
		out.m_result_params()->mutable_tie_ba_list()->mutable_get(i)->set_icon(icon);
	}

    LOGA( "get_qp_rule end item_count[%d]", result_size);

    gettimeofday(&end, NULL);
    LOGT(start, end);
    return 0;
}
