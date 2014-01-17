/*
 * gamedev_svr.cpp
 *
 *  Created on: 2013-5-22
 *      Author: guoweijiang
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
#include "module/mcp_content_common_info.h"
#include "decorator/single_decorator/is_decorator.h"
#include "decorator/single_decorator/or_decorator.h"
#include "decorator/single_decorator/orderby_decorator.h"
#include "decorator/single_decorator/weight_decorator.h"
#include "decorator/compound_decorator/decorator_for_test.h"
#include "mysql_module/mysql_general_module.h"
#include "output_type/once_consume_goods.h"
#include "output_type/game_basic_download_info.h"
#include "output_type/game_info_t.h"
#include "output_type/vir_currency_info.h"
#include "input_type/input_extractor.h"

#include "../mcp_session.h"
#include "../mcp.idl.mcp.h"
#include "../mcpsvr.h"


using namespace std;
using namespace content;

int McpServlet::get_once_consume_goods_list(const idl::mcp_get_once_consume_goods_list_params& in,
                                            idl::mcp_get_once_consume_goods_list_response& out)
{
    const char* udid  = in.udid();
    const char* appid = in.app_id();

    LOGA("[GWJ] %s: start. udid[%s], appid[%s]",
         __FUNCTION__, udid, appid);

    // 设置游戏基本信息表格，找到指定 app_id
    MysqlTable game_basic("pt_game_basic_info", "GBI");
    game_basic["app_id"]->add_and_condition("=", appid);

    // 单机游戏商品表格，匿名为 GP
    MysqlTable goods_prop("pt_local_game_prop_info", "GP");

    // 一次性消费物品  is_consume_goods = 1
    goods_prop["is_consume_goods"]->add_and_condition("=", 1);
    goods_prop["game_id"]->add_left_join(&game_basic, "id", NULL);

    // 用户订购关系表格，匿名为 UOR
    MysqlTable usr_order_rel("pt_user_consume_rel", "UOR");

    // 从 UOR 表中选出如下列
    usr_order_rel.add_select_column("id");
    usr_order_rel.add_select_column("order_id");
    usr_order_rel.add_select_column("goods_id");
    usr_order_rel.add_select_column("consume_date");
    usr_order_rel.add_select_column("udid");

    // 指定设备并 JOIN 连接 GP 表格
    usr_order_rel["udid"]->add_and_condition("=", udid);
    usr_order_rel["goods_id"]->add_left_join(&goods_prop, "id", NULL);

    // 执行查询操作，从数据库 dev_platform_mysql 中执行
    usr_order_rel.process_query_sql("dev_platform_mysql", true);

    // 设置输出参数
    unsigned int result_size = once_consume_goods_t::set_once_goods_out(out.m_result_params()->mutable_items(), usr_order_rel);
    out.m_result_params()->set_itemcount(result_size);

    LOGA("[GWJ] %s: item_count[%hu]", __FUNCTION__, result_size);
    return 0;
}

int McpServlet::add_new_user_consume_record(const idl::mcp_add_new_user_consume_record_params& in,
                                            idl::mcp_add_new_user_consume_record_response& out)
{
    const char* imei = in.imei_id();
    const char* order_id = in.order_id();
    const char* goods_id = in.goods_id();
    const char* consume_date = in.consume_date();
    const char* udid = in.udid();
    int goods_id_value = atoi(goods_id);

    LOGA("[GWJ] %s: start. imei[%s], order_id[%s], goods_id[%s], consume_date[%s], udid[%s]",
         __FUNCTION__, imei, order_id, goods_id, consume_date, udid);

    MysqlTable user_consume_rel("pt_user_consume_rel");
    user_consume_rel["device_id"]->set_value(imei);
    user_consume_rel["order_id"]->set_value(order_id);
    user_consume_rel["goods_id"]->set_value(goods_id_value);
    user_consume_rel["consume_date"]->set_value(consume_date);
    user_consume_rel["udid"]->set_value(udid);

    user_consume_rel.add_new_insert_transaction();
    bool ret = user_consume_rel.process_insert_transaction("dev_platform_mysql");

    if(ret == false)
    {
        LOGA("[GWJ] %s: process_insert_transaction Error!", __FUNCTION__);
        out.m_result_params()->set_ret_code(-1);
    }
    else
    {
        out.m_result_params()->set_ret_code(1);
    }

    return 0;
}

int McpServlet::get_game_currency_rate(const idl::mcp_get_game_currency_rate_params& in,
                                       idl::mcp_get_game_currency_rate_response& out)
{
    InputExtractor extractor;
    extractor.parse_input_params(in.input());

    const char* app_id = extractor.get_filter_condition("app_id");
    LOGA("[GWJ] %s: start. app_id[%s]", __FUNCTION__, app_id);

    MysqlTable vir_currency_info("pt_vir_currency_basic_info", "PVCBI");
	//add by zhengxie
    MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");
    vir_currency_info.add_select_column("id");
    vir_currency_info.add_select_column("game_id");
    vir_currency_info.add_select_column("name");
    vir_currency_info.add_select_column("exchange_rate");
    vir_currency_info.add_select_column("icon");
    vir_currency_info.add_select_column("desc");

	//add by zhengxie
	pt_game_basic_info["app_id"]->add_and_condition("=", app_id );
	vir_currency_info["game_id"]->add_inner_join(&pt_game_basic_info, "id", NULL);

    bool ret = vir_currency_info.process_query_sql("dev_platform_mysql", false);
    vir_currency_info_t::set_outparameter(out.m_result_params()->m_currency_info(),
                                          vir_currency_info);

    if(ret == false)
    {
        LOGA("[GWJ] %s: end. Error Result!", __FUNCTION__);
    }
    else
    {
        LOGA("[GWJ] %s: end. OK!", __FUNCTION__);
    }

    return 0;
}
