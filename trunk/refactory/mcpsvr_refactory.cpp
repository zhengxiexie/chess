/*
 * mcpsvr_refactory.cpp
 *
 *  Created on: 2013-4-27
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
#include "module/mcp_content_diy_column.h"
#include "module/mcp_netgame_activity.h"
#include "module/mcp_diff_update_info.h"
#include "decorator/single_decorator/is_decorator.h"
#include "decorator/single_decorator/or_decorator.h"
#include "decorator/single_decorator/orderby_decorator.h"
#include "decorator/single_decorator/weight_decorator.h"
#include "decorator/single_decorator/bigger_decorator.h"
#include "decorator/single_decorator/smaller_decorator.h"
#include "decorator/single_decorator/not_equal_decorator.h"
#include "decorator/compound_decorator/decorator_for_test.h"
#include "output_type/once_consume_goods.h"
#include "output_type/game_basic_download_info.h"
#include "output_type/game_info_t.h"
#include "output_type/game_news_info_t.h"
#include "output_type/column_info_t.h"
#include "output_type/netgame_activity_info_t.h"
#include "output_type/game_update_download_info.h"
#include "output_type/game_content_download_info_t.h"
#include "output_type/game_news_entire_info_t.h"
#include "input_type/game_news_info_input.h"
#include "input_type/content_input.h"
#include "input_type/input_extractor.h"
#include "input_type/json_extractor.h"
#include "redis_module/duoku_user_download.h"
#include "redis_module/device_download.h"
#include "redis_module/duoku_user_guide.h"
#include "redis_module/duoku_user_favorite.h"
#include "binder/compound_binder.h"
#include "mysql_module/mysql_general_module.h"

#include "../mcp_session.h"
#include "../mcp.idl.mcp.h"
#include "../mcpsvr.h"
#include "mcpsvr_baiduAppInfo.h"

using namespace std;
using namespace content;

/*
 *  重构代码实现业务接口范例：
 *
 *  weight_type : 0-最新, 1-最热, 2-好评
 *  type_mode     : 0-全部, 1-单机(1:基地game; 11:android游戏; 14:基地NQ; 26:H5乐园游戏), 2-网游
 *  class_id    : 游戏类别id. (97:动作冒险; 98:角色扮演; 99:休闲益智; 100:飞行射击; 101:卡片棋牌; 102:策略经营; 104:体育竞技; 108:网络游戏(将来要细分);)
 *                默认如果传入的是 -1 则显示所有类别, 如果是 -2 则表示只显示单机类型, 如果是 -3 则表示只显示网络游戏
 *                注意，这些 class.id 对应的 type.id 都是 11(android 游戏)，其余 type 下的 class 没有列出
 */
int McpServlet::get_content_with_download_info(const idl::mcp_get_content_with_download_info_params& in,
                                               idl::mcp_get_content_with_download_info_response& out)
{
    IsDecorator type_decor;
    OrDecorator class_or_decor;
    OrDecorator label_or_dec;
    OrDecorator column_or_dec;
    OrDecorator content_or_dec;
    AbstractCompoundDecorator compound_decor;

    /*
     *  【注意】
     *  保存 class_id 的实体
     *  因为在 Decorator 中只保存指针
     *  真正的实体只有这一份
     *
     *  在附表  mcp.mcp_content_class_list 中筛选  class_id 项
     */
    vector<string> class_ids;
    content_input_params in_conditions;

    int unit_num = in.unit_num();
    int unit_times = in.unit_times();

    //////////////////////////////////////////////////////////// 处理 content_type 类型 ////////////////////////////////////////////////////////////
    const char* type_id = in.in_param().type_id();

    if(IS_VALUED_STRING(type_id))
    {
        type_decor.add_field_filter_condition("content_type", type_id);
        compound_decor.add_primary_table_decorator(&type_decor);
    }

    //////////////////////////////////////////////////////////// 处理 content_id 游戏ID //////////////////////////////////////////////////////////
    const char* content_ids = in.in_param().content_id();

    if(IS_VALUED_STRING(content_ids))
    {
        string contents_str = content_ids;
        int size = CommonInterface::splic_string(contents_str,
                                                 in_conditions.content_ids, ",");

        if(size > 0)
        {
            for(int i = 0; i < size; i++)
            {
                content_or_dec.add_field_filter_condition("id",
                                                          in_conditions.content_ids[i].c_str());
            }

            compound_decor.add_primary_table_decorator(&content_or_dec);
        }
    }

    //////////////////////////////////////////////////////////// 处理 Class.id 类型 //////////////////////////////////////////////////////////////
    const char* class_id = in.in_param().class_id();
    LOGD("[GWJ] %s: class.id[%s]", __FUNCTION__, class_id);

    if(IS_VALUED_STRING(class_id))
    {
        if(strcmp(class_id, "-1") == 0)
        {
            // 筛选所有游戏类型
            char i_str[8];

            for(int i = 97; i < 103; i++)
            {
                memset(i_str, 0x00, sizeof(i_str));
                sprintf(i_str, "%d", i);
                class_ids.push_back(i_str);

                class_or_decor.add_field_filter_condition("class.id",
                                                          class_ids[i - 97].c_str());
            }

            class_or_decor.add_field_filter_condition("class.id", "104");
            class_or_decor.add_field_filter_condition("class.id", "108");
        }
        else if(strcmp(class_id, "-2") == 0)
        {
            // 筛选出所有单机游戏
            char i_str[8];

            for(int i = 97; i < 103; i++)
            {
                memset(i_str, 0x00, sizeof(i_str));
                sprintf(i_str, "%d", i);
                class_ids.push_back(i_str);

                class_or_decor.add_field_filter_condition("class.id",
                                                          class_ids[i - 97].c_str());
            }

            class_or_decor.add_field_filter_condition("class.id", "104");
        }
        else if(strcmp(class_id, "-3") == 0)
        {
            // 筛选出所有网络游戏
            class_or_decor.add_field_filter_condition("class.id", "108");
        }
        else
        {
            // 按照指定的 class.id 进行筛选
            class_or_decor.add_field_filter_condition("class.id", class_id);
        }

        compound_decor.add_primary_table_decorator(&class_or_decor);
    }

    //////////////////////////////////////////////////////////// 处理 Lable 标签 //////////////////////////////////////////////////////////////
    const char* label_ids = in.in_param().label_id();

    if(IS_VALUED_STRING(label_ids))
    {
        string lables_str = label_ids;
        int size = CommonInterface::splic_string(lables_str,
                                                 in_conditions.lable_ids, ",");

        if(size > 0)
        {
            for(int i = 0; i < size; i++)
            {
                label_or_dec.add_field_filter_condition("label_id",
                                                        in_conditions.lable_ids[i].c_str());
            }

            compound_decor.add_primary_table_decorator(&label_or_dec);
        }
    }

    //////////////////////////////////////////////////////////// 处理 img_type 图片类型 //////////////////////////////////////////////////////////////
    const char* img_types = in.in_param().img_type();

    if(IS_VALUED_STRING(img_types))
    {
        /*
         *  img_types 要在输出时进行判断过滤，所以没有 Decorator
         */
        string img_types_str = img_types;
        CommonInterface::splic_string(img_types_str, in_conditions.img_types, ",");
    }

    //////////////////////////////////////////////////////////// 处理 column.id 栏目类型 ////////////////////////////////////////////////////////////
    const char* column_ids = in.in_param().column_id();

    if(IS_VALUED_STRING(column_ids))
    {
        string columns_str = column_ids;
        int size = CommonInterface::splic_string(columns_str,
                                                 in_conditions.column_ids, ",");

        if(size > 0)
        {
            for(int i = 0; i < size; i++)
            {
                column_or_dec.add_field_filter_condition("column.id",
                                                        in_conditions.column_ids[i].c_str());
            }

            compound_decor.add_primary_table_decorator(&column_or_dec);
        }
    }

    //////////////////////////////////////////////////////////// 处理排序//////////////////////////////////////////////////////////////
    AbstractDecorator* ultimate_decorator;
    const char* w_order = in.in_param().weight_order();
    WEIGHT_ORDER_CONDITION(column_ids, class_id, w_order, compound_decor, ultimate_decorator)

    LOGA("[GWJ] %s: start. type_id[%s], content_ids[%s], class_id[%s], label_ids[%s], img_types[%s], column_ids[%s], weight[%d], unit_num[%d], unit_times[%d]",
         __FUNCTION__, type_id, content_ids, class_id, label_ids, img_types, column_ids, weight_decor.weight_type, unit_num, unit_times);
    //////////////////////////////////////////////////////////// 处理 module 获取数据  ////////////////////////////////////////////////////////////
    /*
     *  设置 module
     *  查询 mcp.mcp_content 表，以 id 为 KEY 索引
     *  并与 mcp.mcp_content_data(以 c_id 为 KEY 索引) 表进行数据黏合
     */
    MCPGameContent mcp_content("id");
    MCPContentData mcp_content_data("c_id");

    // 给 module 装备装饰器
    mcp_content.load_decorator(ultimate_decorator);
    mcp_content.set_unit_info(unit_num, unit_times);

    /*
     *  执行装配好的 module
     *  从 mongoDB 获取数据，并按照指定页面规格获取
     *  保存到 mcp_content内部的 result_set 中
     */
    int ret = mcp_content.general_fetch_list_records(true);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
    }

    int record_count = mcp_content.get_key_size();

    LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
         __FUNCTION__, record_count);

    if(record_count == 0)
    {
        out.m_result_params()->set_count(0);
        return 0;
    }

    /*
     *  GAME_DOWNLOAD_INFO 要从数据库中找到  c_id 为  GAME_BASIC_INFO.id 的 Record
     */
    const char* channel_id = in.in_param().channel_id();
    const char* channel_info = in.in_param().channel_info();

    AbstractCompoundDecorator sub_decor;
    OR_CONDITION_TEMPLATE(channel_id, "channel_id", sub_decor)
    OR_CONDITION_TEMPLATE(channel_info, "channel_info", sub_decor)

    OrDecorator id_filter;
    id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(), "c_id", true);
    sub_decor.add_primary_table_decorator(&id_filter);

    // 装配  module
    mcp_content_data.load_decorator(&sub_decor);

    /*
     *  执行装配好的 GAME_DOWNLOAD_INFO
     *  不必考虑页面大小，获取所有数据即可
     */
    mcp_content_data.general_fetch_list_records(false);

    /*
     *  执行黏合工作
     *  注意顺序，前面的是黏合主表，后面的是附表
     */
    AbstractBinder content_data_binder;
    content_data_binder.bind_two_modules(&mcp_content, &mcp_content_data);

    record_count = mcp_content.get_key_size();

    for(int i = 0; i < record_count; i++)
    {
        game_basic_download_t::set_outparameter(out.m_result_params()->mutable_result_list(i),
                                                in_conditions,
                                                content_data_binder.get_binded_record(i));

    }

    /*
     *  将符合条件的所有数据个数返回
     *  不是分页后的结果个数
     */
    out.m_result_params()->set_count(mcp_content.get_total_result_count());
    return 0;
}

int McpServlet::get_game_info_list_ext(const idl::mcp_get_game_info_list_ext_params& in,
                                       idl::mcp_get_game_info_list_ext_response& out)
{
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
    SMALLER_EQUAL_CONDITION_TEMPLATE(data_limit, "logdate", decorator)
    DECREASE_ORDER_CONDITION_TEMPLATE(dec_date, "logdate", decorator)

    MCPContentNews mcp_content_news("id");
    mcp_content_news.load_decorator(&decorator);
    mcp_content_news.set_unit_info(page_size,page);
    mcp_content_news.general_fetch_list_records(true);

    int ret_size = mcp_content_news.get_key_size();

    for(int i = 0; i < ret_size; i++)
    {
        game_info_t::set_outparameter(out.m_result_params()->mutable_game_info_list(i),
                                      (MCPNewsInfo*)mcp_content_news.get_record_by_key_vec(i, 0));
    }

    ret_size = mcp_content_news.get_total_result_count();
    out.m_result_params()->set_count(ret_size);

    LOGA("[GWJ] %s: end. Total[%d]", __FUNCTION__, ret_size);
    return 0;
}

int McpServlet::get_common_adv_info(const idl::mcp_get_common_adv_info_params& in,
                                    idl::mcp_get_common_adv_info_response& out)
{
    const char* id = in.id();

    LOGA("[GWJ] %s: start. id[%s]", __FUNCTION__, id);

    IsDecorator condition;
    condition.add_field_filter_condition("id", id);

    MCPContentCommonInfo module("id");
    module.load_decorator(&condition);
    module.general_fetch_list_records(false);

    int k_size = module.get_key_size();

    if(k_size > 0)
    {
        MCPCommonAdvInfoRecord* record = (MCPCommonAdvInfoRecord*)module.get_record_by_key_vec(0, 0);
        out.m_result_params()->set_info(record->info.c_str());
    }

    return 0;
}

int McpServlet::add_new_download_game_by_uid(const idl::mcp_add_new_download_game_by_uid_params& in,
                                             idl::mcp_add_new_download_game_by_uid_response& out)
{
    const char* u_id = in.u_id();
    const char* c_id = in.game_id();
    LOGA("[GWJ] %s: start. u_id[%s], c_id[%s]", __FUNCTION__, u_id, c_id);

    int ret = DuokuUserDownloadInfo::set_new_download(u_id, c_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: set_new_download Error[%d]", __FUNCTION__, ret);
    }

    out.m_result_params()->set_ret_code(ret);
    return 0;
}

int McpServlet::get_download_games_count_by_uid(const idl::mcp_get_download_games_count_by_uid_params& in,
                                                idl::mcp_get_download_games_count_by_uid_response& out)
{
    const char* u_id = in.u_id();
    LOGA("[GWJ] %s: start. u_id[%s]", __FUNCTION__, u_id);

    int ret = DuokuUserDownloadInfo::get_user_download_games_count(u_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: get_user_download_games_count Error[%d]", __FUNCTION__, ret);
        out.m_result_params()->set_count(0);
    }
    else
    {
        LOGA("[GWJ] %s: get_user_download_games_count[%d]", __FUNCTION__, ret);
        out.m_result_params()->set_count(ret);
    }

    return 0;
}


int McpServlet::get_download_games_info_by_uid(const idl::mcp_get_download_games_info_by_uid_params& in,
                                               idl::mcp_get_download_games_info_by_uid_response& out)
{
    JsonExtractor extractor;
    extractor.parse_input_params(in.filter());

    const char* u_id = in.u_id();
    const char* img_types = in.img_types();
    const char* channel_info = extractor.get_filter_condition("channel_info");
    const char* channel_id = extractor.get_filter_condition("channel_id");
    int page = in.unit_times();
    int page_num = in.unit_num();
    content_input_params in_conditions;

    LOGA("[GWJ] %s: start. u_id[%s]. img_types[%s], channel_info[%s], channel_id[%s], page[%d], page_num[%d]",
            __FUNCTION__, u_id, img_types, channel_info, channel_id, page, page_num);

    list<string> c_ids;
    int ret = DuokuUserDownloadInfo::get_user_download_games(u_id, c_ids);
    int size = c_ids.size();

    if(ret < 0 || size == 0)
    {
        LOGA("[GWJ] %s: download_games Error[%d]. Count[%d]", __FUNCTION__, ret, size);
        out.m_result_params()->set_count(0);
        return 0;
    }
    else
    {
        LOGA("[GWJ] %s: download_games[%d], size[%d]", __FUNCTION__, ret, size);
    }

    if(IS_VALUED_STRING(img_types))
    {
        string img_types_str = img_types;
        CommonInterface::splic_string(img_types_str, in_conditions.img_types, ",");
    }

	//2013.01.09 laixiaokang support baiduApp collection
    list<string> baiduAppList;
    baiduAppList.clear();
    string baiduContentIdList = "";
    OrDecorator id_or;
    list<string>::iterator it = c_ids.begin();
    for(int i = 0; it != c_ids.end(); i++, it++)
    {
		string strAppId = *it;
        int nLen = strAppId.length();
        if(nLen >= BAIDUAPPIDMINILEN)
        {
            if(baiduContentIdList == "")
            {
                baiduContentIdList = strAppId;
            }
            else
            {
                baiduContentIdList = baiduContentIdList + ";" + strAppId;
            }
            baiduAppList.push_back(*it);
            //LOGA("baiduContentIdList= %s",baiduContentIdList.data());
            continue;
        }
        id_or.add_field_filter_condition("id", (*it).c_str());
    }
    list<string>::iterator itt = baiduAppList.begin();
    for(;itt != baiduAppList.end(); itt++)
    {
        c_ids.remove(*itt); //把百度id从原来的逻辑中除去，防止后面在原来的逻辑中没找到被删除
    }
	bool bIsPageFull = false;
    map<string,string> appInfoMap;
    appInfoMap.clear();
    appInfoMap.insert(pair<string,string>("content_id",baiduContentIdList));
    char appInfoPage[5];
    memset(appInfoPage, 0x00, sizeof(appInfoPage));
    sprintf(appInfoPage,"%d",page);
    char appInfoPageNum[5];
    memset(appInfoPageNum,0x00,sizeof(appInfoPageNum));
    sprintf(appInfoPageNum,"%d",page_num);
    char duokuAppNum[6];
    memset(duokuAppNum,0x00,sizeof(duokuAppNum));
    sprintf(duokuAppNum,"%d",c_ids.size());
    appInfoMap.insert(pair<string,string>("page",appInfoPage));
    appInfoMap.insert(pair<string,string>("page_num",appInfoPageNum));
    appInfoMap.insert(pair<string,string>("duokuAppNum", duokuAppNum));
    appInfoMap.insert(pair<string,string>("fuction_flags", "yesPage"));
    //LOGA("baiduContentIdList= %s",baiduContentIdList.data());

    MCPGameContent mcp_content("id");
    MCPContentData mcp_content_data("c_id");

    mcp_content.load_decorator(&id_or);
    mcp_content.general_fetch_list_records(false);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
        out.m_result_params()->set_count(0);
        DuokuUserDownloadInfo::remove_key_value_list(u_id, c_ids);
        return 0;
    }

    int record_count = mcp_content.get_key_size();

    LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
            __FUNCTION__, record_count);

    if(record_count == 0)
    {
		int nBaiduIndex = 0;
        int nBaiduAppCount = getBaiduAppInfoFromMongoDb(appInfoMap, out.m_result_params()->mutable_result_list(),nBaiduIndex);
        LOGA("[GWJ] %s: general_fetch_list_records. Count[%d]", __FUNCTION__, record_count);
        out.m_result_params()->set_count(nBaiduAppCount);
        //LOGA("[GWJ] %s: general_fetch_list_records. Count[%d]", __FUNCTION__, record_count);
        //out.m_result_params()->set_count(0);
        DuokuUserDownloadInfo::remove_key_value_list(u_id, c_ids);
        return 0;
    }

    AbstractCompoundDecorator sub_decorator;
    OrDecorator id_filter;
    id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(),
                                               "c_id", true);

    sub_decorator.add_primary_table_decorator(&id_filter);
    OR_CONDITION_JSON(channel_info, "channel_info", sub_decorator)
    OR_CONDITION_JSON(channel_id, "channel_id", sub_decorator)

    mcp_content_data.load_decorator(&sub_decorator);
    mcp_content_data.general_fetch_list_records(false);

    it = c_ids.begin();
    int start = (page - 1) * page_num;
    int out_it = 0;

    for(int i = 0; it != c_ids.end() && out_it < page_num; i++, it++)
    {
        if(i < start)
        {
            continue;
        }

        MCPGameInfo* game_data = (MCPGameInfo*)mcp_content.get_record_by_key_vec((*it).c_str(), 0);
        LOGD("[GWJ] %s: id[%s], game_info[%x], out_it[%d], i[%d]",
             __FUNCTION__, (*it).c_str(), game_data, out_it, i);

        if(game_data != NULL)
        {
            game_content_download_info_param::set_outparameter(out.m_result_params()->mutable_result_list(out_it),
                                                               in_conditions, game_data, mcp_content_data, (*it));
            out_it++;
			if(out_it == page_num)
			{
				bIsPageFull = true;				
			}
        }
        else
        {
			//2013.01.09 laixiaokang 百度应用在redis里的删除，有另外的逻辑
            string strAppId = (*it).c_str();
            int nLen = strAppId.length();
            if(nLen >= BAIDUAPPIDMINILEN)
            {
                continue;
            }
            LOGD("[GWJ] %s: id[%s] need remove from REDIS!", __FUNCTION__, (*it).c_str());
            DuokuUserDownloadInfo::remove_download(u_id, (*it).c_str());
        }
    }
	int nBaiduAppCount = 0;
    if(!bIsPageFull)
    {
        int nBaiduIndex = out_it;
        nBaiduAppCount = getBaiduAppInfoFromMongoDb(appInfoMap, out.m_result_params()->mutable_result_list(),nBaiduIndex);
    }

    record_count = mcp_content.get_total_result_count();
	record_count += baiduAppList.size();
    out.m_result_params()->set_count(record_count);
    LOGA("[GWJ] %s: end. Total_Count[%d]", __FUNCTION__, record_count);
    return 0;
}

int McpServlet::add_new_guide_by_uid(const idl::mcp_add_new_guide_by_uid_params& in,
                                     idl::mcp_add_new_guide_by_uid_response& out)
{
    const char* u_id = in.u_id();
    const char* gd_id = in.guide_id();
    LOGA("[GWJ] %s: start. u_id[%s], gd_id[%s]", __FUNCTION__, u_id, gd_id);

    int ret = DuokuUserGuideInfo::set_new_guide(u_id, gd_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: set_new_download Error[%d]", __FUNCTION__, ret);
    }

    out.m_result_params()->set_ret_code(ret);
    return 0;
}

int McpServlet::get_guides_count_by_uid(const idl::mcp_get_guides_count_by_uid_params& in,
                                        idl::mcp_get_guides_count_by_uid_response& out)
{
    const char* u_id = in.u_id();
    LOGA("[GWJ] %s: start. u_id[%s]", __FUNCTION__, u_id);

    int ret = DuokuUserGuideInfo::get_user_guides_count(u_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: get_user_download_games_count Error[%d]", __FUNCTION__, ret);
        out.m_result_params()->set_count(0);
    }
    else
    {
        LOGA("[GWJ] %s: get_user_download_games_count[%d]", __FUNCTION__, ret);
        out.m_result_params()->set_count(ret);
    }

    return 0;
}

int McpServlet::get_guides_info_by_uid(const idl::mcp_get_guides_info_by_uid_params& in,
                                       idl::mcp_get_guides_info_by_uid_response& out)
{
    const char* u_id = in.u_id();
    int page = in.unit_times();
    int page_num = in.unit_num();

    LOGA("[GWJ] %s: start. u_id[%s]. page[%d], page_num[%d]",
            __FUNCTION__, u_id, page, page_num);

    list<string> gd_ids;
    int ret = DuokuUserGuideInfo::get_user_guides(u_id, gd_ids);
    int size = gd_ids.size();

    if(ret < 0 || size == 0)
    {
        LOGA("[GWJ] %s: get_user_guides Error[%d]. Count[%d]", __FUNCTION__, ret, size);
        out.m_result_params()->set_count(0);
        return 0;
    }
    else
    {
        LOGA("[GWJ] %s: user_guides[%d], size[%d]", __FUNCTION__, ret, size);
    }

    OrDecorator id_or;
    list<string>::iterator it = gd_ids.begin();
    for(; it != gd_ids.end(); it++)
    {
        id_or.add_field_filter_condition("id", (*it).c_str());
    }

    MCPContentNews mcp_content_news("id");
    mcp_content_news.load_decorator(&id_or);
    mcp_content_news.general_fetch_list_records(false);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: [mcp_content_news] fetch list Error! ", __FUNCTION__);
        out.m_result_params()->set_count(0);
        DuokuUserGuideInfo::remove_key_value_list(u_id, gd_ids);
        return 0;
    }

    it = gd_ids.begin();
    int start = (page - 1) * page_num;
    int out_it = 0;
    for(int i = 0; it != gd_ids.end() && out_it < page_num; i++, it++)
    {
        if(i < start)
        {    
            continue;
        }

        MCPNewsInfo* news_info = (MCPNewsInfo*)mcp_content_news.get_record_by_key_vec((*it).c_str(), 0);

        if(news_info != NULL)
        {
            game_info_t::set_outparameter(out.m_result_params()->mutable_result_list(out_it), news_info);
            out_it++;
        }
        else
        {
            LOGD("[GWJ] %s: id[%s] need remove from REDIS!", __FUNCTION__, (*it).c_str());
            DuokuUserGuideInfo::remove_guide(u_id, (*it).c_str());
        }
    }

    int ret_size = mcp_content_news.get_total_result_count();
    out.m_result_params()->set_count(ret_size);
    LOGA("[GWJ] %s: OK! count[%d]", __FUNCTION__, ret_size);
    return 0;
}

/* 开服列表 */
int McpServlet::get_game_news_info_list(const idl::mcp_get_game_news_info_list_params& in,
                                        idl::mcp_get_game_news_info_list_response& out)
{
    const char* game_id = in.game_id();
    const char* ov_id = in.ov_id();
    const char* ov_type = in.news_type();
    const char* date_limit = in.date_limit();
    const char* game_imgtypes = in.game_imgtypes();
    int page = in.page();
    int page_num = in.page_size();

    LOGA("[GWJ] %s: start. game_id[%s], ov_id[%s], ov_type[%s], date_limit[%s], "
            "game_imgtypes[%s], page[%d], page_num[%d]",
            __FUNCTION__, game_id, ov_id, ov_type, date_limit,
            game_imgtypes, page, page_num);

    AbstractCompoundDecorator compound_decor;
    game_news_info_input_params in_conditions;

    OR_CONDITION_TEMPLATE(game_id, "c_id", compound_decor)
    OR_CONDITION_TEMPLATE(ov_id, "id", compound_decor)
    OR_CONDITION_TEMPLATE(ov_type, "type_id", compound_decor)
    SMALLER_CONDITION_TEMPLATE(date_limit, "logdate", compound_decor)
    DECREASE_ORDER_CONDITION_TEMPLATE(dec_date, "logdate", compound_decor)
    SET_INPUT_PARAMETERS_TEMPLATE(game_imgtypes, in_conditions.game_img_types)

    MCPContentNews mcp_content_news("c_id");
    mcp_content_news.load_decorator(&compound_decor);
    mcp_content_news.set_unit_info(page_num, page);
    mcp_content_news.general_fetch_list_records(true);

    int news_key_size = mcp_content_news.get_key_size();

    if(news_key_size == 0)
    {
        LOGA("[GWJ] %s: No Record Return!!", __FUNCTION__);
        out.m_result_params()->set_count(0);
        return 0;
    }

    OrDecorator id_filter;
    id_filter.set_same_field_filter_conditions(mcp_content_news.get_result_keys(),
                                               "id", true);

    MCPGameContent mcp_content("id");
    mcp_content.load_decorator(&id_filter);
    mcp_content.general_fetch_list_records(false);

    CompoundBinder content_data_binder;
    content_data_binder.left_join(&mcp_content_news);
    content_data_binder.left_join(&mcp_content);

    int record_count = content_data_binder.get_result_count();
    int j = 0;
    for(int i = 0; i < record_count; i++)
    {
        int bucket_size = content_data_binder.get_bucket_size(i, 0);
        int sec_bucket_size = content_data_binder.get_bucket_size(i, 1);

        LOGA("[GWJ] %s: [Compound] news_bucket size[%d], game_bucket_size[%d], record_count[%d]",
              __FUNCTION__, bucket_size, sec_bucket_size, record_count);

        if(bucket_size > 0 && sec_bucket_size > 0)
        {
            for(int x = 0; x < bucket_size; x++, j++)
            {
                game_news_info_param::set_outparameter(out.m_result_params()->mutable_game_news_info_list(j), in_conditions,
                                                       (MCPNewsInfo*)content_data_binder.get_record_value(i, 0, x),
                                                       (MCPGameInfo*)content_data_binder.get_record_value(i, 1, 0));
            }
        }
    }

    out.m_result_params()->set_count(mcp_content_news.get_total_result_count());
    return 0;
}

int McpServlet::is_user_has_relation_with(const idl::mcp_is_user_has_relation_with_params& in,
                                          idl::mcp_is_user_has_relation_with_response& out)
{
    const char* u_id = in.u_id();
    const char* c_id = in.c_id();
    const char* c_type = in.c_type();

    LOGA("[GWJ] %s: u_id[%s], c_id[%s], c_type[%s]",
         __FUNCTION__, u_id, c_id, c_type);

    if(!IS_VALUED_STRING(u_id) ||
       !IS_VALUED_STRING(c_id) ||
       !IS_VALUED_STRING(c_type))
    {
        LOGA("[GWJ] %s: u_id[%s], c_id[%s], c_type[%s] has NULL Error!!",
             __FUNCTION__, u_id, c_id, c_type);

        out.m_result_params()->set_has_relation(-1);
        return 0;
    }

    if(IS_SAME_VALUED_STRING(c_type, "0"))
    {
        list<string> vs;
        DuokuUserFavoriteInfo::get_user_favorites(u_id, vs);

        int index = CommonInterface::find_key_index(c_id, vs);

        if(index < 0)
        {
            LOGA("[GWJ] %s: u_id[%s], c_id[%s] Has No Relation!",
                 __FUNCTION__, u_id, c_id);

            out.m_result_params()->set_has_relation(0);
            return 0;
        }
        else
        {
            LOGA("[GWJ] %s: u_id[%s], c_id[%s] Has Relation!",
                 __FUNCTION__, u_id, c_id);

            out.m_result_params()->set_has_relation(1);
            return 0;
        }
    }
    else if(IS_SAME_VALUED_STRING(c_type, "1"))
    {
        list<string> vs;
        DuokuUserDownloadInfo::get_user_download_games(u_id, vs);

        int index = CommonInterface::find_key_index(c_id, vs);

        if(index < 0)
        {
            LOGA("[GWJ] %s: u_id[%s], c_id[%s] Has No Relation!",
                 __FUNCTION__, u_id, c_id);

            out.m_result_params()->set_has_relation(0);
            return 0;
        }
        else
        {
            LOGA("[GWJ] %s: u_id[%s], c_id[%s] Has Relation!",
                 __FUNCTION__, u_id, c_id);

            out.m_result_params()->set_has_relation(1);
            return 0;
        }
    }
    else if(IS_SAME_VALUED_STRING(c_type, "2"))
    {
        list<string> vs;
        DuokuUserGuideInfo::get_user_guides(u_id, vs);

        int index = CommonInterface::find_key_index(c_id, vs);

        if(index < 0)
        {
            LOGA("[GWJ] %s: u_id[%s], c_id[%s] Has No Relation!",
                 __FUNCTION__, u_id, c_id);

            out.m_result_params()->set_has_relation(0);
            return 0;
        }
        else
        {
            LOGA("[GWJ] %s: u_id[%s], c_id[%s] Has Relation!",
                 __FUNCTION__, u_id, c_id);

            out.m_result_params()->set_has_relation(1);
            return 0;
        }
    }

    out.m_result_params()->set_has_relation(0);
    return 0;
}

int McpServlet::add_new_favorite_by_uid(const idl::mcp_add_new_favorite_by_uid_params& in,
                                        idl::mcp_add_new_favorite_by_uid_response& out)
{
    const char* u_id = in.u_id();
    const char* c_id = in.favorites_id();
    LOGA("[GWJ] %s: start. u_id[%s], c_id[%s]", __FUNCTION__, u_id, c_id);

    int ret = DuokuUserFavoriteInfo::set_new_favorite(u_id, c_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: set_new_favorite Error[%d]", __FUNCTION__, ret);
    }

    out.m_result_params()->set_ret_code(ret);
    return 0;
}

int McpServlet::get_favorites_count_by_uid(const idl::mcp_get_favorites_count_by_uid_params& in,
                                           idl::mcp_get_favorites_count_by_uid_response& out)
{
    const char* u_id = in.u_id();
    LOGA("[GWJ] %s: start. u_id[%s]", __FUNCTION__, u_id);

    int ret = DuokuUserFavoriteInfo::get_user_favorite_games_count(u_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: get_user_favorite_games_count Error[%d]", __FUNCTION__, ret);
        out.m_result_params()->set_count(0);
    }
    else
    {
        LOGA("[GWJ] %s: get_user_favorite_games_count[%d]", __FUNCTION__, ret);
        out.m_result_params()->set_count(ret);
    }

    return 0;
}

int McpServlet::get_favorites_info_by_uid(const idl::mcp_get_favorites_info_by_uid_params& in,
                                          idl::mcp_get_favorites_info_by_uid_response& out)
{
    JsonExtractor extractor;
    extractor.parse_input_params(in.filter());

    const char* u_id = in.u_id();
    const char* img_types = in.img_types();
    const char* channel_info = extractor.get_filter_condition("channel_info");
    const char* channel_id = extractor.get_filter_condition("channel_id");

    int page = in.unit_times();
    int page_num = in.unit_num();
    content_input_params in_conditions;

    LOGA("[GWJ] %s: start. u_id[%s]. img_types[%s], channel_info[%s], channel_id[%s], page[%d], page_num[%d]",
            __FUNCTION__, u_id, img_types, channel_info, channel_id, page, page_num);

    list<string> c_ids;
    int ret = DuokuUserFavoriteInfo::get_user_favorites(u_id, c_ids);
    int size = c_ids.size();

    if(ret < 0 || size == 0)
    {
        LOGA("[GWJ] %s: get_user_favorites Error[%d]. Count[%d]", __FUNCTION__, ret, size);
        out.m_result_params()->set_count(0);
        return 0;
    }
    else
    {
        LOGA("[GWJ] %s: get_user_favorites[%d], size[%d]", __FUNCTION__, ret, size);
    }

    if(IS_VALUED_STRING(img_types))
    {
        string img_types_str = img_types;
        CommonInterface::splic_string(img_types_str, in_conditions.img_types, ",");
    }

    OrDecorator id_or;
    list<string>::iterator it = c_ids.begin();
    //2013.12.3 laixiaokang support baiduApp collection
    list<string> baiduAppList;
    baiduAppList.clear();
    string baiduContentIdList = "";
    for(; it != c_ids.end(); it++)
    {
		string strAppId = *it;
        int nLen = strAppId.length();
        if(nLen >= BAIDUAPPIDMINILEN)
        {
            if(baiduContentIdList == "")
            {
                baiduContentIdList = strAppId;
            }
            else
            {
                baiduContentIdList = baiduContentIdList + ";" + strAppId;
            }
            baiduAppList.push_back(*it);
            //LOGA("baiduContentIdList= %s",baiduContentIdList.data());
            continue;
        }
        id_or.add_field_filter_condition("id", (*it).c_str());
    }
    list<string>::iterator itt = baiduAppList.begin();
    for(;itt != baiduAppList.end(); itt++)
    {
        c_ids.remove(*itt); //把百度id从原来的逻辑中除去，防止后面在原来的逻辑中没找到被删除
    }
	bool bIsPageFull = false;

    map<string,string> appInfoMap;
    appInfoMap.clear();
    appInfoMap.insert(pair<string,string>("content_id",baiduContentIdList));
    char appInfoPage[5];
    memset(appInfoPage, 0x00, sizeof(appInfoPage));
    sprintf(appInfoPage,"%d",page);
    char appInfoPageNum[5];
    memset(appInfoPageNum,0x00,sizeof(appInfoPageNum));
    sprintf(appInfoPageNum,"%d",page_num);
    char duokuAppNum[6];
    memset(duokuAppNum,0x00,sizeof(duokuAppNum));
    sprintf(duokuAppNum,"%d",c_ids.size());
    appInfoMap.insert(pair<string,string>("page",appInfoPage));
    appInfoMap.insert(pair<string,string>("page_num",appInfoPageNum));
    appInfoMap.insert(pair<string,string>("duokuAppNum", duokuAppNum));
    appInfoMap.insert(pair<string,string>("fuction_flags", "yesPage"));
    //LOGA("baiduContentIdList= %s",baiduContentIdList.data());
    MCPGameContent mcp_content("id");
    MCPContentData mcp_content_data("c_id");

    mcp_content.load_decorator(&id_or);
    mcp_content.general_fetch_list_records(false);

    if(ret < 0)
    {
		/*
        int nBaiduIndex = 0;
        int nBaiduAppCount = getBaiduAppInfoFromMongoDb(appInfoMap, out.m_result_params()->mutable_result_list(),nBaiduIndex);
        LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
        out.m_result_params()->set_count(nBaiduAppCount);
		*/
        out.m_result_params()->set_count(0);
        DuokuUserFavoriteInfo::remove_key_value_list(u_id, c_ids);
        return 0;
    }

    int record_count = mcp_content.get_key_size();

    LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
            __FUNCTION__, record_count);

    if(record_count == 0)
    {
        int nBaiduIndex = 0;
        int nBaiduAppCount = getBaiduAppInfoFromMongoDb(appInfoMap, out.m_result_params()->mutable_result_list(),nBaiduIndex);
        LOGA("[GWJ] %s: general_fetch_list_records. Count[%d]", __FUNCTION__, record_count);
        out.m_result_params()->set_count(nBaiduAppCount);
        DuokuUserFavoriteInfo::remove_key_value_list(u_id, c_ids);
        return 0;
    }

    AbstractCompoundDecorator sub_decorator;
    OrDecorator id_filter;
    id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(),
                                               "c_id", true);

    sub_decorator.add_primary_table_decorator(&id_filter);
    OR_CONDITION_JSON(channel_info, "channel_info", sub_decorator)
    OR_CONDITION_JSON(channel_id, "channel_id", sub_decorator)

    mcp_content_data.load_decorator(&sub_decorator);
    mcp_content_data.general_fetch_list_records(false);

    it = c_ids.begin();
    int start = (page - 1) * page_num;
    int out_it = 0;
    for(int i = 0; it != c_ids.end() && out_it < page_num; i++, it++)
    {
        if(i < start)
        {
            continue;
        }

        // 给定的 game_id 有可能已经下架，要判断一下是否为 NULL
        MCPGameInfo* game_data = (MCPGameInfo*)mcp_content.get_record_by_key_vec((*it).c_str(), 0);
        LOGD("[GWJ] %s: id[%s], game_info[%x], out_it[%d], i[%d], record_count[%d]",
             __FUNCTION__, (*it).c_str(), game_data, out_it, i, record_count);

        if(game_data != NULL)
        {
            game_content_download_info_param::set_outparameter(out.m_result_params()->mutable_result_list(out_it),
                                                               in_conditions, game_data, mcp_content_data, (*it));
            out_it++;
			if(out_it == page_num)
			{
				bIsPageFull = true;				
			}
        }
        else
        {
            LOGD("[GWJ] %s: id[%s] need remove from REDIS!", __FUNCTION__, (*it).c_str());
            //2013.12.04 laixiaokang 百度应用在redis里的删除，有另外的逻辑
            string strAppId = (*it).c_str();
            int nLen = strAppId.length();
            if(nLen >= BAIDUAPPIDMINILEN)
            {
                continue;
            }

            DuokuUserFavoriteInfo::remove_favorite(u_id, (*it).c_str());
        }
    }
    int nBaiduAppCount = 0;
    if(!bIsPageFull)
    {
        int nBaiduIndex = out_it;
        nBaiduAppCount = getBaiduAppInfoFromMongoDb(appInfoMap, out.m_result_params()->mutable_result_list(),nBaiduIndex);
    }

    record_count = mcp_content.get_total_result_count();
	record_count += baiduAppList.size();
    out.m_result_params()->set_count(record_count);
    LOGA("[GWJ] %s: end. Total_Count[%d]", __FUNCTION__, record_count);
    return 0;
}

int McpServlet::remove_favorite_by_uid(const idl::mcp_remove_favorite_by_uid_params& in,
                                       idl::mcp_remove_favorite_by_uid_response& out)
{
    const char* u_id = in.u_id();
    const char* c_id = in.favorites_id();
    LOGA("[GWJ] %s: start. u_id[%s], c_id[%s]", __FUNCTION__, u_id, c_id);

    int ret = DuokuUserFavoriteInfo::remove_favorite(u_id, c_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: remove_favorite Error[%d]", __FUNCTION__, ret);
    }

    out.m_result_params()->set_ret_code(ret);
    return 0;
}

int McpServlet::remove_guide_by_uid(const idl::mcp_remove_guide_by_uid_params& in,
                                    idl::mcp_remove_guide_by_uid_response& out)
{
    const char* u_id = in.u_id();
    const char* c_id = in.guide_id();
    LOGA("[GWJ] %s: start. u_id[%s], c_id[%s]", __FUNCTION__, u_id, c_id);

    int ret = DuokuUserGuideInfo::remove_guide(u_id, c_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: remove_guide Error[%d]", __FUNCTION__, ret);
    }

    out.m_result_params()->set_ret_code(ret);
    return 0;
}

int McpServlet::get_game_contents_count(const idl::mcp_get_game_contents_count_params& in,
                                        idl::mcp_get_game_contents_count_response& out)
{
    const char* type_id = in.type_id();
    const char* class_id = in.class_id();
    const char* label_id = in.label_id();
    const char* column_id = in.column_id();

    LOGA("[GWJ] %s: type_id[%s], class_id[%s], label_id[%s], column_id[%s]",
         __FUNCTION__, type_id, class_id, label_id, column_id);

    AbstractCompoundDecorator compound_decor;
    OR_CONDITION_TEMPLATE(type_id, "content_type", compound_decor)
    OR_CONDITION_TEMPLATE(class_id, "class.id", compound_decor)
    OR_CONDITION_TEMPLATE(label_id, "label_id", compound_decor)
    OR_CONDITION_TEMPLATE(column_id, "column.id", compound_decor)

    MCPGameContent mcp_content("id");
    mcp_content.load_decorator(&compound_decor);
    int ret = mcp_content.general_get_list_records_count();

    if(ret < 0)
    {
        LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
        out.m_result_params()->set_count(0);
        return 0;
    }

    out.m_result_params()->set_count(ret);
    LOGA("[GWJ] %s: [mcp_content] count is[%d]", __FUNCTION__, ret);
    return 0;
}

int McpServlet::get_columns_info(const idl::mcp_get_columns_info_params& in,
                                 idl::mcp_get_columns_info_response& out)
{
    const char* type_id = in.type_id();
    const char* col_id = in.column_id();

    LOGA("[GWJ] %s: type_id[%s], col_id[%s]",
         __FUNCTION__, type_id, col_id);

    AbstractCompoundDecorator compound_decor;
    OR_CONDITION_TEMPLATE(type_id, "type_id", compound_decor)
    OR_CONDITION_TEMPLATE(col_id, "id", compound_decor)

    MCPDIYColumnInfo mcp_column("id");
    mcp_column.load_decorator(&compound_decor);
    mcp_column.general_fetch_list_records(false);

    int count = mcp_column.get_key_size();

    for(int i = 0; i < count; i++)
    {
        diy_column_info_t::set_outparameter(out.m_result_params()->mutable_result_list(i),
                                            (MCPDIYColumnRecord*)mcp_column.get_record_by_key_vec(i, 0));
    }

    out.m_result_params()->set_count(count);
    LOGA("[GWJ] %s: count[%d]", __FUNCTION__, count);
    return 0;
}

/* 活动 */
int McpServlet::get_netgame_activity_info(const idl::mcp_get_netgame_activity_info_params& in,
                                          idl::mcp_get_netgame_activity_info_response& out)
{
    const char* game_id = in.game_id();
    const char* act_id = in.act_id();
    const char* channel = in.channel();
    const char* logdate = in.date_limit();
    const char* game_img = in.game_img_type();
    const char* act_img = in.act_img_type();
    int page = in.page();
    int page_size = in.page_size();

    LOGA("[GWJ] %s: game_id[%s], act_id[%s], channel[%s], logdate[%s], game_img[%s], act_img[%s], page[%d], page_size[%d]",
         __FUNCTION__, game_id, act_id, channel, logdate, game_img, act_img, page, page_size);

    // 先设置图片参数
    game_news_info_input_params input_param;
    SET_INPUT_PARAMETERS_TEMPLATE(game_img, input_param.game_img_types)
    SET_INPUT_PARAMETERS_TEMPLATE(act_img, input_param.activity_img_types)

    AbstractCompoundDecorator compound_decor;
    MCPNetGameActivityInfo netgame_activity("id");

    // 当有输入 GameID 参数时, 先选出指定游戏所含有的所有资讯
    MCPContentNews news_module_by_gameId("c_id");
    OrDecorator activity_or_dec_by_game_id;

    // 当有输入 GameID 参数时
    if(IS_VALUED_STRING(game_id))
    {
        OrDecorator game_id_or_dec;
        game_id_or_dec.add_field_filter_condition("c_id", game_id);

        // 先选出指定游戏所含有的所有资讯
        news_module_by_gameId.load_decorator(&game_id_or_dec);
        news_module_by_gameId.general_fetch_list_records(false);

        int size = news_module_by_gameId.get_key_bucket_size(0);

        if(size == 0)
        {
            out.m_result_params()->set_count(0);
            LOGA("[GWJ] %s: count 0. No News_Record By GameId", __FUNCTION__);
            return 0;
        }

        for(int i = 0; i < size; i++)
        {
            MCPNewsInfo* news_record =
                    (MCPNewsInfo*)news_module_by_gameId.get_record_by_key_vec(0, i);

            activity_or_dec_by_game_id.add_field_filter_condition("news_id",
                                                       news_record->id.c_str());
        }

        compound_decor.add_primary_table_decorator(&activity_or_dec_by_game_id);
    }

    OR_CONDITION_TEMPLATE(act_id, "id", compound_decor)
    OR_CONDITION_TEMPLATE(channel, "channel", compound_decor)
    SMALLER_CONDITION_TEMPLATE(logdate, "logdate", compound_decor)
    DECREASE_ORDER_CONDITION_TEMPLATE(logdate, "logdate", compound_decor)

    netgame_activity.load_decorator(&compound_decor);
    netgame_activity.set_unit_info(page_size, page);
    netgame_activity.general_fetch_list_records(true);

    int key_size = netgame_activity.get_key_size();

    if(key_size == 0)
    {
        out.m_result_params()->set_count(0);
        LOGA("[GWJ] %s: count 0", __FUNCTION__);
        return 0;
    }

    for(int act_index = 0; act_index < key_size; act_index++)
    {
        MCPNetGameActivityInfoRecord * act_record =
                        (MCPNetGameActivityInfoRecord*)netgame_activity.get_record_by_key_vec(act_index,
                                                                                              0);

        MCPContentNews news_module("c_id");
        OrDecorator news_or_dec;

        int news_size = act_record->news_id.size();
        for(int n = 0; n < news_size; n++)
        {
            news_or_dec.add_field_filter_condition("id",
                                                   act_record->news_id[n].c_str());
        }

        news_module.load_decorator(&news_or_dec);
        news_module.general_fetch_list_records(false);

        MCPGameContent game_module("id");
        OrDecorator game_or_dec;
        game_or_dec.set_same_field_filter_conditions(news_module.get_result_keys(),
                                                     "id", true);

        game_module.load_decorator(&game_or_dec);
        game_module.general_fetch_list_records(false);

        CompoundBinder binder;
        binder.left_join(&news_module);
        binder.left_join(&game_module);

        netgame_act_info_t::set_outparameter(out.m_result_params()->mutable_result_list(act_index),
                                             input_param, act_record, &binder);
    }

    key_size = netgame_activity.get_total_result_count();
    out.m_result_params()->set_count(key_size);
    LOGA("[GWJ] %s: count %d", __FUNCTION__, key_size);
    return 0;
}


int McpServlet::get_device_download_games_info_by_udid(const idl::mcp_get_device_download_games_info_by_udid_params& in,
                                                       idl::mcp_get_device_download_games_info_by_udid_response& out)
{
    JsonExtractor extractor;
    extractor.parse_input_params(in.filter());

    const char* udid = in.udid();
    const char* img_types = in.img_types();
    const char* channel_info = extractor.get_filter_condition("channel_info");
    const char* channel_id = extractor.get_filter_condition("channel_id");

    LOGA("[GWJ] %s: udid[%s], img_types[%s], channel_info[%s], channel_id[%s]",
         __FUNCTION__, udid, img_types, channel_info, channel_id);

    list<string> games_id;
    int ret = DeviceDownloadInfo::get_device_download_games(udid, games_id);

    int size = games_id.size();

    if(ret < 0 || size == 0)
    {
        LOGA("[GWJ] %s: download_games Error[%d]. Count[%d]", __FUNCTION__, ret, size);
        out.m_result_params()->set_count(0);
        return 0;
    }
    else
    {
        LOGA("[GWJ] %s: download_games[%d], size[%d]", __FUNCTION__, ret, size);
    }

    content_input_params in_conditions;
    SET_INPUT_PARAMETERS_TEMPLATE(img_types, in_conditions.img_types)

    OrDecorator id_or;
    list<string>::iterator it = games_id.begin();
    for(; it != games_id.end(); it++)
    {
        id_or.add_field_filter_condition("id", (*it).c_str());
    }

    MCPGameContent mcp_content("id");
    mcp_content.load_decorator(&id_or);
    ret = mcp_content.general_fetch_list_records(false);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
        out.m_result_params()->set_count(0);
        DeviceDownloadInfo::remove_key_value_list(udid, games_id);
        return 0;
    }

    int record_count = mcp_content.get_key_size();

    LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
         __FUNCTION__, record_count);

    if(record_count == 0)
    {
        LOGA("[GWJ] %s: general_fetch_list_records. Count[%d]", __FUNCTION__, record_count);
        out.m_result_params()->set_count(0);
        DeviceDownloadInfo::remove_key_value_list(udid, games_id);
        return 0;
    }

    AbstractCompoundDecorator sub_decorator;

    OrDecorator id_filter;
    id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(),
                                               "c_id", true);

    sub_decorator.add_primary_table_decorator(&id_filter);
    OR_CONDITION_JSON(channel_info, "channel_info", sub_decorator)
    OR_CONDITION_JSON(channel_id, "channel_id", sub_decorator)

    MCPContentData mcp_content_data("c_id");
    mcp_content_data.load_decorator(&sub_decorator);
    mcp_content_data.general_fetch_list_records(false);

    it = games_id.begin();
    int out_it = 0;
    for(int i = 0; it != games_id.end(); i++, it++)
    {
        MCPGameInfo* game_data = (MCPGameInfo*)mcp_content.get_record_by_key_vec((*it).c_str(), 0);
        LOGD("[GWJ] %s: id[%s], game_info[%x], out_it[%d], i[%d], record_count[%d]",
              __FUNCTION__, (*it).c_str(), game_data, out_it, i, record_count);

        if(game_data != NULL)
        {
            game_content_download_info_param::set_outparameter(out.m_result_params()->mutable_result_list(out_it),
                                                               in_conditions, game_data, mcp_content_data, (*it));
            out_it++;
        }
        else
        {
            LOGD("[GWJ] %s: id[%s] need remove from REDIS!", __FUNCTION__, (*it).c_str());
            DeviceDownloadInfo::remove_download(udid, (*it).c_str());
        }
    }

    record_count = mcp_content.get_total_result_count();
    out.m_result_params()->set_count(record_count);
    LOGA("[GWJ] %s: end. Count[%d]", __FUNCTION__, record_count);
    return 0;
}

int McpServlet::add_device_download_game(const idl::mcp_add_device_download_game_params& in,
                                         idl::mcp_add_device_download_game_response& out)
{
    const char* udid = in.udid();
    const char* game_id = in.game_id();
    LOGA("[GWJ] %s: udid[%s], game_id[%s]", __FUNCTION__, udid, game_id);

    int ret = DeviceDownloadInfo::set_new_download(udid, game_id);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: set_new_download Error[%d]", __FUNCTION__, ret);
    }

    out.m_result_params()->set_ret_code(ret);
    LOGA("[GWJ] %s: result[%d]", __FUNCTION__, ret);
    return 0;
}

int McpServlet::get_game_content_info_list_ext(const idl::mcp_get_game_content_info_list_ext_params& in,
                                               idl::mcp_get_game_content_info_list_ext_response& out)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

    JsonExtractor extractor;
    extractor.parse_input_params(in.input());

    int page = in.page();
    int page_size = in.page_size();

    const char* type_id = extractor.get_filter_condition("type_id");
    const char* class_id = extractor.get_filter_condition("class_id");
    const char* label_id = extractor.get_filter_condition("label_id");
    const char* column_id = extractor.get_filter_condition("column_id");
    const char* content_id = extractor.get_filter_condition("content_id");
    const char* channel_id = extractor.get_filter_condition("channel_id");
    const char* channel_info = extractor.get_filter_condition("channel_info");
    const char* weight_order = extractor.get_filter_condition("weight_order");
    const char* img_type = extractor.get_filter_condition("img_type");
    map<string,string> paramVec;
    paramVec.insert(make_pair<string,string>("type_id",type_id));
    paramVec.insert(make_pair<string,string>("class_id",class_id));
    paramVec.insert(make_pair<string,string>("label_id",label_id));
    paramVec.insert(make_pair<string,string>("column_id",column_id));
    //paramVec.insert(make_pair<string,string>("content_id",content_id));
    paramVec.insert(make_pair<string,string>("channel_id",channel_id));
    paramVec.insert(make_pair<string,string>("channel_info",channel_info));
    paramVec.insert(make_pair<string,string>("weight_order",weight_order));
    paramVec.insert(make_pair<string,string>("img_type",img_type));
    paramVec.insert(make_pair<string,string>("type_id",type_id));
    paramVec.insert(make_pair<string,string>("fuction_flags","noPage"));
    char pageChar[20];
    memset(pageChar,0x00,sizeof(pageChar));
    sprintf(pageChar,"%d",page);
    paramVec.insert(pair<string,string>("page",pageChar));
    char pageSizeChar[20];
    memset(pageSizeChar,0x00,sizeof(pageSizeChar));
    sprintf(pageSizeChar,"%d",page_size);
    paramVec.insert(pair<string,string>("page_size",pageSizeChar));
    vector<string> idVector;
    string strDuokuList = "";
    string strBaiduList = "";
    bool bJudge = parseContentId(BAIDUAPPIDMINILEN,content_id, idVector, strDuokuList,strBaiduList);
    paramVec.insert(make_pair<string,string>("content_id",strBaiduList.data()));
    content_id = strDuokuList.data();

    LOGA("[GWJ] %s: type_id[%s], class_id[%s], weight_order[%s], label_id[%s], "
         "img_type[%s], column_id[%s], content_id[%s], channel_id[%s], channel_info[%s]",
          __FUNCTION__, type_id, class_id, weight_order, label_id,
          img_type, column_id, content_id, channel_id, channel_info);
    if(bJudge)
    {
    // 最后放到 module 里的装饰器
    AbstractDecorator* ultimate_decorator;

    AbstractCompoundDecorator compound_decor;
    OR_CONDITION_JSON(type_id, "content_type", compound_decor)
    OR_CONDITION_JSON(label_id, "label_id", compound_decor)
    OR_CONDITION_JSON(column_id, "column.id", compound_decor)
    OR_CONDITION_JSON(content_id, "id", compound_decor)
    CLASS_ID_OR_CONDITION(class_id, "class.id", compound_decor)
    WEIGHT_ORDER_CONDITION(column_id, class_id, weight_order, compound_decor, ultimate_decorator)

    content_input_params game_imgs;
    SET_INPUT_PARAMETERS_JSON(img_type, game_imgs.img_types)

    AbstractCompoundDecorator sub_compound_decor;
    OR_CONDITION_JSON(channel_id, "channel_id", sub_compound_decor)
    OR_CONDITION_JSON(channel_info, "channel_info", sub_compound_decor)

    //////////////////////////////////////////////////////////////////////////////////////////////

    MCPGameContent mcp_content("id");
    mcp_content.load_decorator(ultimate_decorator);
    mcp_content.set_unit_info(page_size, page);

    int ret = mcp_content.general_fetch_list_records(true);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
    }

    int record_count = mcp_content.get_key_size();

    LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
         __FUNCTION__, record_count);

    if(record_count == 0)
    {
        out.m_result_params()->set_count(0);
    }

    OrDecorator id_filter;
    id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(), "c_id", true);
    sub_compound_decor.add_primary_table_decorator(&id_filter);

    MCPContentData mcp_content_data("c_id");
    mcp_content_data.load_decorator(&sub_compound_decor);
    mcp_content_data.general_fetch_list_records(false);

    CompoundBinder content_data_binder;
    content_data_binder.left_join(&mcp_content);
    content_data_binder.left_join(&mcp_content_data);

    record_count = content_data_binder.get_result_count();

    for(int i = 0; i < record_count; i++)
    {
        game_content_download_info_param::set_outparameter(out.m_result_params()->mutable_result_list(i),
                                                           game_imgs,
                                                           content_data_binder, i);
    }

        record_count = mcp_content.get_total_result_count();
        if(idVector.size() > 0)
        {
            int nResultSize = getBaiduAppInfoFromMongoDb(paramVec,out.m_result_params()->mutable_result_list(),record_count);
            record_count += nResultSize;
        }
        out.m_result_params()->set_count(record_count);

        LOGA("[GWJ] %s: end. Total_Count[%d]", __FUNCTION__, record_count);
		gettimeofday(&end, NULL);
		LOGT(start, end);
        return 0;
    }
    else
    {
        //2013.11.20 laixiaokang 增加百度应用信息的详情信息,cId > 一千万 是百度的id,当多酷游戏信息为空的时候
        int nBaiduIndex = 0;
        int record_count = getBaiduAppInfoFromMongoDb(paramVec, out.m_result_params()->mutable_result_list(), nBaiduIndex);
        out.m_result_params()->set_count(record_count);
		gettimeofday(&end, NULL);
		LOGT(start, end);
        return 0;
    }
}

int McpServlet::get_game_download_info(const idl::mcp_get_game_download_info_params& in,
                                       idl::mcp_get_game_download_info_response& out)
{
    const char* c_id = in.c_id();
    const char* channel_id = in.channel_id();
    const char* channel_info = in.channel_info();

    LOGA("[GWJ] %s: content_id[%s], channel_id[%s], channel_info[%s]",
              __FUNCTION__, c_id, channel_id, channel_info);

    AbstractCompoundDecorator content_data_decor;
    OR_CONDITION_TEMPLATE(c_id, "c_id", content_data_decor)
    OR_CONDITION_TEMPLATE(channel_id, "channel_id", content_data_decor)
    OR_CONDITION_TEMPLATE(channel_info, "channel_info", content_data_decor)

    MCPContentData mcp_content_data("c_id");
    mcp_content_data.load_decorator(&content_data_decor);
    mcp_content_data.general_fetch_list_records(false);

    int count = mcp_content_data.get_key_size();

    if(count == 0)
    {
        LOGA("[GWJ] %s: No Download Info Found", __FUNCTION__);
        out.m_result_params()->set_count(0);
        return 0;
    }

    for(int i = 0; i < count; i++)
    {
        MCPContentDataInfo* download_info = (MCPContentDataInfo*)mcp_content_data.get_record_by_key_vec(i, 0);
        download_detail_data_t::set_outparameter(out.m_result_params()->mutable_download_list(i), download_info);
    }

    LOGA("[GWJ] %s: end. Total Download Info Count[%d]", __FUNCTION__, count);
    out.m_result_params()->set_count(count);
    return 0;
}

int McpServlet::get_game_news_entire_info(const idl::mcp_get_game_news_entire_info_params& in,
                                          idl::mcp_get_game_news_entire_info_response& out)
{
    const char* news_id = in.news_id();
    const char* game_id = in.game_id();
    const char* news_type = in.news_type();
    const char* date_limit = in.date_limit();
    const char* order_type = in.order_type();
    const char* game_imgtypes = in.game_imgtypes();
    const char* channel_info = in.channel_info();
    const char* channel_id = in.channel_id();
    unsigned int page = in.page();
    unsigned int page_size = in.page_size();

    LOGA("[GWJ] %s: news_id[%s], game_id[%s], news_type[%s], date_limit[%s], order_type[%s], "
                   "game_imgtypes[%s], channel_info[%s], channel_id[%s], page[%u], page_size[%u]",
                    __FUNCTION__, news_id, game_id, news_type, date_limit, order_type,
                    game_imgtypes, channel_info, channel_id, page, page_size);

    AbstractCompoundDecorator comp_decor;
    OR_CONDITION_TEMPLATE(news_id, "id", comp_decor)
    OR_CONDITION_TEMPLATE(game_id, "c_id", comp_decor)
    OR_CONDITION_TEMPLATE(news_type, "type_id", comp_decor)

    content_input_params in_conditions;
    SET_INPUT_PARAMETERS_TEMPLATE(game_imgtypes, in_conditions.img_types)

    OrderbyDecorator logdate_order;
    BiggerThanDecorator bigger_logdate;
    SmallerThanDecorator smaller_logdate;


    if(IS_VALUED_STRING(order_type) && IS_SAME_VALUED_STRING(order_type, "asc"))
    {
        // 按日期升序
        logdate_order.order_type = 1;
        logdate_order.add_field_filter_condition("logdate", NULL);
        comp_decor.add_primary_table_decorator(&logdate_order);

        if(IS_VALUED_STRING(date_limit))
        {
            bigger_logdate.is_bigger_and_equal = true;
            bigger_logdate.add_field_filter_condition("logdate", date_limit);
            comp_decor.add_primary_table_decorator(&bigger_logdate);
        }
    }
    else
    {
        // 按日期降序
        logdate_order.order_type = -1;
        logdate_order.add_field_filter_condition("logdate", NULL);
        comp_decor.add_primary_table_decorator(&logdate_order);

        if(IS_VALUED_STRING(date_limit))
        {
            smaller_logdate.is_smaller_and_equal = true;
            smaller_logdate.add_field_filter_condition("logdate", date_limit);
            comp_decor.add_primary_table_decorator(&smaller_logdate);
        }
    }

    /*
     *   根据 comp_decor 条件获取 mcp_content_news 信息
     */
    MCPContentNews mcp_content_news("id");
    mcp_content_news.set_unit_info(page_size, page);
    mcp_content_news.load_decorator(&comp_decor);
    mcp_content_news.general_fetch_list_records(true);

    int size = mcp_content_news.get_key_size();

    if(size == 0)
    {
        LOGA("[GWJ] %s: No News Info Found", __FUNCTION__);
        out.m_result_params()->set_count(0);
        return 0;
    }

    // 根据选出的资讯找到对应的【游戏基本信息】，【游戏下载信息】
    OrDecorator game_basic_ids;
    OrDecorator game_download_ids;
    for(int i = 0; i < size; i++)
    {
        MCPNewsInfo* news_record = (MCPNewsInfo*)mcp_content_news.get_record_by_key_vec(i, 0);
        const char* c_id = news_record->c_id.c_str();
        game_basic_ids.add_field_filter_condition("id", c_id);
        game_download_ids.add_field_filter_condition("c_id", c_id);
    }

    // 获取【游戏基本信息】
    MCPGameContent mcp_content("id");
    mcp_content.load_decorator(&game_basic_ids);
    mcp_content.general_fetch_list_records(false);

    // 获取【游戏下载信息】
    AbstractCompoundDecorator download_decor;
    OR_CONDITION_TEMPLATE(channel_info, "channel_info", download_decor)
    OR_CONDITION_TEMPLATE(channel_id, "channel_id", download_decor)

    MCPContentData mcp_content_data("c_id");
    download_decor.add_primary_table_decorator(&game_download_ids);
    mcp_content_data.load_decorator(&download_decor);
    mcp_content_data.general_fetch_list_records(false);

    int basic_size = mcp_content.get_key_size();
    int download_size = mcp_content_data.get_key_size();

    if(basic_size == 0)
    {
        LOGA("[GWJ] %s: No Game Basic Info Found", __FUNCTION__);
    }

    if(download_size == 0)
    {
        LOGA("[GWJ] %s: No Game DownLoad Info Found", __FUNCTION__);
    }

    // 将结果输出返回
    int out_index = 0;
    for(int i = 0; i < size; i++)
    {
        MCPNewsInfo* news_record = (MCPNewsInfo*)mcp_content_news.get_record_by_key_vec(i, 0);

        if(news_record != NULL)
        {
            MCPGameInfo* game_basic = (MCPGameInfo*)mcp_content.get_record_by_key_vec(news_record->c_id.c_str(), 0);
            game_news_entire_info_param::set_outparameter(out.mutable_result_params()->mutable_game_news_list(out_index),
                                                          in_conditions, news_record, game_basic, mcp_content_data,
                                                          news_record->c_id.c_str());

            out_index++;
        }
    }

    size = mcp_content_news.get_total_result_count();
    out.mutable_result_params()->set_count(size);
    LOGA("[GWJ] %s: Total Count[%d]", __FUNCTION__, size);
    return 0;
}

int McpServlet::get_game_news_without_pbus(const idl::mcp_get_game_news_without_pbus_params& in,
                                           idl::mcp_get_game_news_without_pbus_response& out)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    const char* info_id = in.info_id();
    const char* c_id = in.c_id();
    const char* info_type = in.info_type();
    const char* game_type = in.game_type();
    const char* data_limit = in.date_limit();
    const char* news_source = "0";

    uint32_t page(in.page());
    uint32_t page_size(in.page_size());

    LOGA("[GWJ] %s: start. info_id[%s], c_id[%s], info_type[%s], game_type[%s], data_limit[%s], page[%d], page_size[%d]",
         __FUNCTION__, info_id, c_id, info_type, game_type, data_limit, page, page_size);

    AbstractCompoundDecorator decorator;

    OR_CONDITION_TEMPLATE(info_id, "id", decorator)
    OR_CONDITION_TEMPLATE(c_id, "c_id", decorator)
    OR_CONDITION_TEMPLATE(info_type, "type_id", decorator)
    OR_CONDITION_TEMPLATE(game_type, "game_type", decorator)
    OR_CONDITION_TEMPLATE(news_source, "news_source", decorator)
    SMALLER_CONDITION_TEMPLATE(data_limit, "logdate", decorator)
    DECREASE_ORDER_CONDITION_TEMPLATE(dec_date, "logdate", decorator)

    MCPContentNews mcp_content_news("id");
    mcp_content_news.load_decorator(&decorator);
    mcp_content_news.set_unit_info(page_size,page);
    mcp_content_news.general_fetch_list_records(true);

    int ret_size = mcp_content_news.get_key_size();

    for(int i = 0; i < ret_size; i++)
    {
        game_info_t::set_outparameter(out.m_result_params()->mutable_game_info_list(i),
                                      (MCPNewsInfo*)mcp_content_news.get_record_by_key_vec(i, 0));
    }

    ret_size = mcp_content_news.get_total_result_count();
    out.m_result_params()->set_count(ret_size);

    LOGA("[GWJ] %s: end. Total[%d]", __FUNCTION__, ret_size);
    return 0;
}
