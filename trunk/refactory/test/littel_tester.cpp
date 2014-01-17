/*
 * littel_tester.cpp
 *
 *  Created on: 2013-4-28
 *      Author: guoweijiang
 */
#include "littel_tester.h"

using namespace std;

typedef struct test_struct
{
    string id;
    string value;
    string date;
    string user;
}test_struct;

map<char*, test_struct> g_map_set;
vector<char*> g_list;

map<char*, test_struct> g_map_set_2;
vector<char*> g_list_2;

map<char*, vector<test_struct> > g_map_v;
vector<char*> g_list_v;

int find_key_index_(char* value, vector<char*>& list)
{
    int size = list.size();

    for(int i = 0; i < size; i++)
    {
        if(strcmp(value, list[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}

void set_map_v()
{
    char index[8];

    for(int j = 0; j < 3; j++)
    {
        for(int i = 0; i < 3; i++)
        {
            test_struct detail;

            memset(index, 0x00, sizeof(index));
            sprintf(index, "%d", j);
            detail.id = string(index);
            g_list.push_back((char*)detail.id.c_str());

            memset(index, 0x00, sizeof(index));
            sprintf(index, "%d_%d_%s", j, i, "value");
            detail.value = string(index);

            memset(index, 0x00, sizeof(index));
            sprintf(index, "%d_%d_%s", j, i, "date");
            detail.date = string(index);

            memset(index, 0x00, sizeof(index));
            sprintf(index, "%d_%d_%s", j, i, "user");
            detail.user = string(index);

            int index = find_key_index_((char*)detail.id.c_str(), g_list_v);

            if(index < 0)
            {
                g_list_v.push_back((char*)detail.id.c_str());
                g_map_v[(char*)detail.id.c_str()].push_back(detail);
            }
            else
            {
                g_map_v[(char*)g_list_v[index]].push_back(detail);
            }
        }
    }
}

void set_map_set()
{
    char index[8];

    for(int i = 0; i < 3; i++)
    {
        test_struct detail;

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        detail.id = string(index);
        g_list.push_back((char*)detail.id.c_str());

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d_%s", i, "value");
        detail.value = string(index);

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d_%s", i, "date");
        detail.date = string(index);

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d_%s", i, "user");
        detail.user = string(index);

        g_map_set[(char*)detail.id.c_str()] = detail;
    }
}

void set_map_set_2()
{
    char index[8];

    for(int i = 0; i < 3; i++)
    {
        test_struct detail;

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i+1);
        detail.id = string(index);
        g_list_2.push_back((char*)detail.id.c_str());

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d_%s_2", i+1, "value");
        detail.value = string(index);

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d_%s_2", i+1, "date");
        detail.date = string(index);

        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d_%s_2", i+1, "user");
        detail.user = string(index);

        g_map_set_2[(char*)detail.id.c_str()] = detail;
    }
}

char* get_select_query(vector<char*>& select_fields)
{
    string query = "SELECT ";
    int size = select_fields.size();

    for(int i = 0; i < size; i++)
    {
        if(i == 0)
        {
            query = query + "`" + string(select_fields[i]) + "`";
        }
        else
        {
            query = query + ", `" + string(select_fields[i]) + "`";
        }
    }

    return (char*)query.c_str();
}

char* get_rest_query()
{
    char* value = "1";
    string query;
    query = "From table where id = " + string(value);
    return (char*)query.c_str();
}

char* get_mysql_query(vector<char*>& select_fields)
{
    string query;
    query = string(get_select_query(select_fields)) + string(get_rest_query());
    return (char*)query.c_str();
}

vector<char*> id_s;
char* key_name;
vector<char*> select_fields;
map<char*, vector<MySQLRecord> > result_set;

void set_mysql_result_set(vector<vector<string> >& records_data, int count)
{
    int size = select_fields.size();

    for(int i = 0; i < count; i++)
    {
        MySQLRecord record;

        for(int j = 0; j < size; j++)
        {
            record.record[select_fields[j]] = records_data[i][j];
        }

        int index = find_key_index_((char*)record.record[key_name].c_str(), id_s);

        if(index < 0)
        {
            id_s.push_back((char*)record.record[key_name].c_str());
            result_set[(char*)record.record[key_name].c_str()].push_back(record);
        }
        else
        {
            result_set[(char*)id_s[index]].push_back(record);
        }
    }
}

int littel_test_main()
{
    set_map_set();
    set_map_set_2();
    set_map_v();

    select_fields.push_back("id");
    select_fields.push_back("value");
    select_fields.push_back("name");
    select_fields.push_back("date");

    key_name = select_fields[0];

    vector<vector<string> > records_data;

    char istr[3];
    char jstr[3];

    for(int i = 0; i < 3; i++)
    {
        memset(istr, 0, 3);
        sprintf(istr, "%d", i);
        
        vector<string> row;

        for(int j = 0; j < 4; j++)
        {
            memset(jstr, 0, 3);
            sprintf(jstr, "%d", j);

            if(j == 0)
            {
                row.push_back(string(istr));
            }
            else
            {
                row.push_back(string(istr) + "-" + string(jstr));
            }
        }

        records_data.push_back(row);
    }

    set_mysql_result_set(records_data, 3);
    char* ret = get_mysql_query(select_fields);

    for(int i = 0; i < id_s.size(); i++)
    {
        vector<MySQLRecord> id_rows = result_set[id_s[i]];

        for(int j = 0; j < id_rows.size(); j++)
        {
            MySQLRecord row_0 = id_rows[j];

            for(int x = 0; x < select_fields.size(); x++)
            {
                string value = row_0.record[select_fields[x]];

                printf("id[%s], column_name[%s], value[%s]\n", id_s[i], select_fields[x], value.c_str());
            }
        }
    }

    //char* value = (char*)(g_map_set["34"].value.c_str());
    printf("value:%s", ret);

    return 0;
}

void print_result(TwoBindedRecord* binded_record)
{
    if(binded_record == NULL || binded_record->get_primary_record(0) == NULL)
    {
        LOGXX("[GWJ] %s: binded_record Or Basic Info is NULL", __FUNCTION__);
        return;
    }

    /*
     *  一般来说只去该索引下的第一条数据即可
     *  因为在设置游戏基本信息时一般只有一条数据对应一个索引
     */
    MCPGameInfo* record = (MCPGameInfo*)binded_record->get_primary_record(0);

    const char* id = record->id.c_str();
    LOGXX("[GWJ] %s: id[%s]", __FUNCTION__, id);

    const char* name = record->name.c_str();
    const char* type = record->content_type.c_str();
    const char* ext_id = record->ext_id.c_str();
    const char* abstract_ = record->info.c_str();
    const char* version = record->version.c_str();
    const char* method = record->method.c_str();
    const char* star = record->star.c_str();
    const char* network = record->network_id.c_str();
    const char* code = record->game_code.c_str();
    const char* log_date = record->logdate.c_str();
    const char* mod_date = record->modify_date.c_str();
    const char* dft_charge_type = record->default_charge_typeid.c_str();
    const char* dft_charge_val = record->default_charge_value.c_str();
    const char* _enName = record->enName.c_str();
    const char* language = record->language.c_str();
    const char* adapt = record->adapt.c_str();
    const char* package_name = record->package_name.c_str();
    const char* version_appinfo = record->version_appinfo.c_str();
    const char* sdk_version = record->sdk_version.c_str();
    const char* version_code = record->version_code.c_str();
    const char* ptype_0 = record->ptype_1.c_str();
    const char* ptype_1 = record->ptype_2.c_str();
    const char* ptype_2 = record->ptype_3.c_str();
    const char* ptype_3 = record->ptype_4.c_str();
    const char* ptype_4 = record->ptype_5.c_str();

    string cp_name;
    mcp_content_map::get_cp_name(record->cp_id, cp_name);
    const char* cp_name_ptr = cp_name.c_str();
    const char* cp_id_ptr = record->cp_id.c_str();

    grant_detail_t grant_info;
    mcp_content_map::get_grant_info(record->grant_id, grant_info);
    const char* grant_id = record->grant_id.c_str();
    const char* g_modify_time = grant_info.modify_time.c_str();
    const char* ggrant_time = grant_info.grant_time.c_str();
    const char* gvalid_time = grant_info.valid_time.c_str();

    vector<MCPTupleItem>::const_iterator itr = record->classes.begin();
    vector<MCPTupleItem>::const_iterator end_itr = record->classes.end();

    for(int i = 0; itr != end_itr; itr++, i++)
    {
        class_info_t class_info;
        mcp_content_map::get_class_info((*itr).key, class_info);

        const char* cname = class_info.name.c_str();
        const char* cid = (*itr).key.c_str();

        LOGXX("[GWJ] %s: c_id[key:%s, name:%s]", __FUNCTION__, cid, cname);
    }

    itr = record->img_info.begin();
    end_itr = record->img_info.end();
    for(int i = 0; itr != end_itr; itr++, i++)
    {
        const char* itype = (*itr).key.c_str();
        const char* iurl = (*itr).value.c_str();
    }

    if(binded_record->get_secondary_record(0) == NULL)
    {
        LOGD("[GWJ] %s: DownLoad Info is NULL", __FUNCTION__);
        return;
    }

    int download_size = binded_record->get_secondary_record_size();

    for(int i = 0; i < download_size; i++)
    {
        MCPContentDataInfo* download_info =
                (MCPContentDataInfo*)binded_record->get_secondary_record(i);

        const char* did = download_info->id.c_str();
        const char* durl = download_info->path_url.c_str();
        const char* durl_type = download_info->path_type.c_str();
        const char* dmedia_format = download_info->media_format_id.c_str();
        const char* dsize = download_info->size.c_str();
        const char* dmtype = download_info->mtype.c_str();
        const char* dcharge_id = download_info->charge_typeid.c_str();
        const char* dcharge_value = download_info->value.c_str();
    }
}

int test_fun()
{
//    IsDecorator type_decor;
//    OrDecorator class_or_decor;
//    OrDecorator label_or_dec;
//    OrDecorator column_or_dec;
//    OrDecorator content_or_dec;
//    WeightDecorator weight_decor;
//    AbstractCompoundDecorator compound_decor;
//
//    /*
//     *  【注意】
//     *  保存 class_id 的实体
//     *  因为在 Decorator 中只保存指针
//     *  真正的实体只有这一份
//     *
//     *  在附表  mcp.mcp_content_class_list 中筛选  class_id 项
//     */
//    vector<string> class_ids;
//    content_input_params in_conditions;
//
//    //////////////////////////////////////////////////////////// 处理 content_type 类型 ////////////////////////////////////////////////////////////
//    const char* type_id = "11";
//
//    if(IS_VALUED_STRING(type_id))
//    {
//
//        type_decor.add_field_filter_condition("content_type", type_id);
//        compound_decor.add_primary_table_decorator(&type_decor);
//    }
//
//    //////////////////////////////////////////////////////////// 处理 content_id 游戏ID //////////////////////////////////////////////////////////
//    const char* content_ids = "";
//
//    if(IS_VALUED_STRING(content_ids))
//    {
//        string contents_str = content_ids;
//        int size = CommonInterface::splic_string(contents_str,
//                                                 in_conditions.content_ids, ",");
//
//        if(size > 0)
//        {
//            for(int i = 0; i < size; i++)
//            {
//                content_or_dec.add_field_filter_condition("id",
//                                                        in_conditions.content_ids[i].c_str());
//            }
//
//            compound_decor.add_primary_table_decorator(&content_or_dec);
//        }
//    }
//
//    //////////////////////////////////////////////////////////// 处理 Class.id 类型 //////////////////////////////////////////////////////////////
//    const char* class_id = "-2";
//    LOGD("[GWJ] %s: class.id[%s]", __FUNCTION__, class_id);
//
//    if(IS_VALUED_STRING(class_id))
//    {
//        if(strcmp(class_id, "-1") == 0)
//        {
//            // 筛选所有游戏类型
//            char i_str[8];
//
//            for(int i = 97; i < 103; i++)
//            {
//                memset(i_str, 0x00, sizeof(i_str));
//                sprintf(i_str, "%d", i);
//                class_ids.push_back(i_str);
//
//                class_or_decor.add_field_filter_condition("class.id",
//                                                          class_ids[i - 97].c_str());
//            }
//
//            class_or_decor.add_field_filter_condition("class.id", "104");
//            class_or_decor.add_field_filter_condition("class.id", "108");
//        }
//        else if(strcmp(class_id, "-2") == 0)
//        {
//            // 筛选出所有单机游戏
//            char i_str[8];
//
//            for(int i = 97; i < 103; i++)
//            {
//                memset(i_str, 0x00, sizeof(i_str));
//                sprintf(i_str, "%d", i);
//                class_ids.push_back(i_str);
//
//                class_or_decor.add_field_filter_condition(
//                                                          "class.id",
//                                                          class_ids[i - 97].c_str());
//            }
//
//            class_or_decor.add_field_filter_condition("class.id", "104");
//        }
//        else if(strcmp(class_id, "-3") == 0)
//        {
//            // 筛选出所有网络游戏
//            class_or_decor.add_field_filter_condition("class.id", "108");
//        }
//        else
//        {
//            // 按照指定的 class.id 进行筛选
//            class_or_decor.add_field_filter_condition("class.id", class_id);
//        }
//
//        compound_decor.add_primary_table_decorator(&class_or_decor);
//    }
//
//    //////////////////////////////////////////////////////////// 处理 Lable 标签 //////////////////////////////////////////////////////////////
//    const char* label_ids = "";
//
//    if(IS_VALUED_STRING(label_ids))
//    {
//        string lables_str = label_ids;
//        int size = CommonInterface::splic_string(lables_str,
//                                                 in_conditions.lable_ids, ",");
//
//        if(size > 0)
//        {
//            for(int i = 0; i < size; i++)
//            {
//                label_or_dec.add_field_filter_condition("label_id",
//                                                        in_conditions.lable_ids[i].c_str());
//            }
//
//            compound_decor.add_primary_table_decorator(&label_or_dec);
//        }
//    }
//
//    //////////////////////////////////////////////////////////// 处理 权重//////////////////////////////////////////////////////////////
//    weight_decor.weight_type = WeightDecorator::get_weight_type("4");
//    compound_decor.add_primary_table_decorator(&weight_decor);
//
//    //////////////////////////////////////////////////////////// 处理 img_type 图片类型 //////////////////////////////////////////////////////////////
//    const char* img_types = "5";
//
//    if(IS_VALUED_STRING(img_types))
//    {
//        /*
//         *  img_types 要在输出时进行判断过滤，所以没有 Decorator
//         */
//        string img_types_str = img_types;
//        CommonInterface::splic_string(img_types_str, in_conditions.img_types, ",");
//    }
//
//    //////////////////////////////////////////////////////////// 处理 column.id 栏目类型 ////////////////////////////////////////////////////////////
//    const char* column_ids = "377";
//
//    if(IS_VALUED_STRING(column_ids))
//    {
//        string columns_str = column_ids;
//        int size = CommonInterface::splic_string(columns_str,
//                                                 in_conditions.column_ids, ",");
//
//        if(size > 0)
//        {
//            for(int i = 0; i < size; i++)
//            {
//                column_or_dec.add_field_filter_condition("column.id",
//                                                        in_conditions.column_ids[i].c_str());
//            }
//
//            compound_decor.add_primary_table_decorator(&column_or_dec);
//        }
//    }
//
//    //////////////////////////////////////////////////////////// 处理 module 获取数据  ////////////////////////////////////////////////////////////
//    /*
//     *  设置 module
//     *  查询 mcp.mcp_content 表，以 id 为 KEY 索引
//     *  并与 mcp.mcp_content_data(以 c_id 为 KEY 索引) 表进行数据黏合
//     */
//    MCPGameContent mcp_content("id");
//    MCPContentData mcp_content_data("c_id");
//
//    // 给 module 装备装饰器
//    mcp_content.load_decorator(&compound_decor);
//    mcp_content.set_unit_info(10, 1);
//
//    /*
//     *  执行装配好的 module
//     *  从 mongoDB 获取数据，并按照指定页面规格获取
//     *  保存到 mcp_content内部的 result_set 中
//     */
//    int ret = mcp_content.general_fetch_list_records(true);
//
//    if(ret < 0)
//    {
//        LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
//    }
//
//    int record_count = mcp_content.get_key_size();
//
//    LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
//         __FUNCTION__, record_count);
//
//    if(record_count == 0)
//    {
////        out.m_result_params()->set_count(0);
//        return 0;
//    }
//
//    /*
//     *  GAME_DOWNLOAD_INFO 要从数据库中找到  c_id 为  GAME_BASIC_INFO.id 的 Record
//     */
//    OrDecorator id_filter;
//    id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(), "c_id", true);
//
//    // 装配  module
//    mcp_content_data.load_decorator(&id_filter);
//
//    /*
//     *  执行装配好的 GAME_DOWNLOAD_INFO
//     *  不必考虑页面大小，获取所有数据即可
//     */
//    mcp_content_data.general_fetch_list_records(false);
//
//    /*
//     *  执行黏合工作
//     *  注意顺序，前面的是黏合主表，后面的是附表
//     */
//    AbstractBinder content_data_binder;
//    content_data_binder.bind_two_modules(&mcp_content, &mcp_content_data);
//
//    int k_size = mcp_content.get_key_size();
//
//    for(int i = 0; i < k_size; i++)
//    {
//        print_result(content_data_binder.get_binded_record(i));
//    }
//
//    k_size = mcp_content.get_total_result_count();
//    LOGD("[GWJ] %s: total count:%d", __FUNCTION__, k_size);
//
//    return 0;
}

void print_news_result(AbstractMCPRecord* record)
{
    MCPNewsInfo* news_rec = (MCPNewsInfo*)record;

    const char* type_id = news_rec->type_id.c_str();
    string type_name;

    CommonSimpleBuffer::instance().get_game_info_type_name(type_id, type_name);
    LOGD("[GWJ] %s: type_name[%s]", __FUNCTION__, type_name.c_str());
}

void test_upmongodb()
{
//        const char* c_id = "32936";
//        const char* channel_id = "";
//        const char* channel_info = "DuoKu,TEMP";
//
//        LOGA("[GWJ] %s: content_id[%s], channel_id[%s], channel_info[%s]",
//                  __FUNCTION__, c_id, channel_id, channel_info);
//
//        AbstractCompoundDecorator content_data_decor;
//        OR_CONDITION_JSON(c_id, "c_id", content_data_decor)
//        OR_CONDITION_JSON(channel_id, "channel_id", content_data_decor)
//        OR_CONDITION_JSON(channel_info, "channel_info", content_data_decor)
//
//        MCPContentData mcp_content_data("c_id");
//        mcp_content_data.load_decorator(&content_data_decor);
//        mcp_content_data.general_fetch_list_records(false);
//
//        int count = mcp_content_data.get_key_size();
//
//        if(count == 0)
//        {
//            LOGA("[GWJ] %s: No Download Info Found", __FUNCTION__);
//           //out.m_result_params()->set_count(0);
//            return;
//        }
//
//        for(int i = 0; i < count; i++)
//        {
//            MCPContentDataInfo* download_info = (MCPContentDataInfo*)mcp_content_data.get_record_by_key_vec(i, 0);
//            //download_detail_data_t::set_outparameter(out.m_result_params()->mutable_download_list(i), download_info);
//        }
//
//        LOGA("[GWJ] %s: end. Total Download Info Count[%d]", __FUNCTION__, count);
//        return;

//    string game_id("100");
//
//            if(!IS_VALUED_STRING(game_id.c_str()))
//            {
//                return ;
//            }
//
//            /* 2013-07-13 add by zhengxie 先从redis获取 beg*/
//            map<string, string> record;
//            string game_id_key = "game_id:"+game_id;
//
//            RedisModule rp;
//            int result = rp.get_record_by_key(game_id_key.c_str(), record);
//            if(0 == result)
//            {
//    //            out.m_result_params()->set_flag(1);
//                return ;
//            }
//            /* 2013-07-13 add by zhengxie 先从redis获取 end*/
//
//            MysqlTable pt_game_basic_info("pt_game_basic_info", "PGBI");
//
//            pt_game_basic_info.add_select_column("app_id");
//
//            SET_MYSQL_SELECT_CONDITION(game_id.c_str(), "app_id", pt_game_basic_info, "=");
//
//            pt_game_basic_info.process_query_sql("dev_platform_mysql", true);
//            int result_size = pt_game_basic_info.get_total_count_without_page();

    //        out.m_result_params()->set_flag(result_size);
//            return ;

//////////////////////////////////////////////////////////

//            string app_key("5t6y7u8i");
//            LOGA("[GWJ] %s: start. app_key[%s]", __FUNCTION__, app_key.c_str());
//
//            if( !IS_VALUED_STRING(app_key.c_str()))
//            {
//                LOGA("[GWJ] %s: app_key Empty", __FUNCTION__);
//                return ;
//            }
//
//            map<string, string> record;
//            string game_id;
//
//            RedisModule redis_processor;
//            int result = redis_processor.get_record_by_key(app_key.c_str(), record);
//
//            if(0 == result)
//            {
//                map<string, string>::iterator it;
//                for ( it=record.begin(); it != record.end(); it++ )
//                {
//                    if(  !strcmp("game_id", (it->first).c_str()) )
//                    {
//                        game_id = it->second;
//                    }
//                }
//                if( IS_VALUED_STRING(game_id.c_str()))
//                {
//                    LOGA("[GWJ] %s: game_id Empty", __FUNCTION__);
//                    string game_id_key = "game_id:"+game_id;
//
//                    RedisModule redis_processor_2;
//                    result = redis_processor_2.del_key_record(game_id_key.c_str());
//                    if(0!=result)
//                    {
//                        LOGA("[GWJ] %s: remove redis fail,game_id_key:%s", __FUNCTION__, game_id_key.c_str());
//                        return;
//                    }
//                }
//            }
//
//            string app_key_key = "app_key:"+app_key;
//
//            RedisModule redis_processor_3;
//            result = redis_processor_3.del_key_record(app_key_key.c_str());
//            if(0!=result)
//            {
//                LOGA("[GWJ] %s: remove redis fail,app_key_key:%s", __FUNCTION__, app_key_key.c_str());
//                return ;
//            }
//
////            out.m_result_params()->set_flag(1);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        const char* ptag = "51743";
        string ptag_key = "MBMUSIC:"+string(ptag);
        string ring_id;

        RedisModule redis_proc;
        int result = redis_proc.get_value_by_key(ptag_key.c_str(), ring_id);
        if(0!=result)
        {
            LOGA("[GWJ] %s: get redis fail,ptag_key:%s", __FUNCTION__, ptag_key.c_str());
//            out.m_result_params()->set_result(0);
            return ;
        }

//        out.m_result_params()->set_ring_id(ring_id.c_str());
//        out.m_result_params()->set_result(0);
        return ;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    LOGD("[GWJ] %s: start", __FUNCTION__);
//
//    string c_id("");
//    string info_type("");
//    string game_type("");
//    string date_lim("2013-05-30 00:00:00");
//
//    uint32_t page(1);
//    uint32_t page_size(10);
//
//    LOGA("[GWJ] %s: start. c_id[%s], info_type[%s], game_type[%s], page[%d], page_size[%d]",
//         __FUNCTION__, c_id.c_str(), info_type.c_str(), game_type.c_str(), page, page_size);
//
//    AbstractCompoundDecorator decorator;
//
//    OrDecorator cid_or_logic;
//    OrDecorator info_or_logic;
//    OrDecorator game_or_logic;
//    OrderbyDecorator date_order;
//    BiggerThanDecorator bigger_date;
//
//    vector<string> id_vec;
//    vector<string> info_vec;
//    vector<string> game_vec;
//
//    int id_size   = CommonInterface::splic_string(c_id, id_vec, ",");
//    int info_size = CommonInterface::splic_string(info_type, info_vec, ",");
//    int game_size = CommonInterface::splic_string(game_type, game_vec, ",");
//
//    if(id_size > 0)
//    {
//        int size = id_vec.size();
//
//        for(int i = 0; i < size; i++)
//        {
//            cid_or_logic.add_field_filter_condition("c_id", id_vec[i].c_str());
//        }
//
//        decorator.add_primary_table_decorator(&cid_or_logic);
//    }
//
//    if(info_size > 0)
//    {
//        int size = info_vec.size();
//
//        for(int i = 0; i < size; i++)
//        {
//            info_or_logic.add_field_filter_condition("type_id", info_vec[i].c_str());
//        }
//
//        decorator.add_primary_table_decorator(&info_or_logic);
//    }
//
//    if(game_size > 0)
//    {
//        int size = game_vec.size();
//
//        for(int i = 0; i < size; i++)
//        {
//            game_or_logic.add_field_filter_condition("game_type", game_vec[i].c_str());
//        }
//
//        decorator.add_primary_table_decorator(&game_or_logic);
//    }
//
//    date_order.order_type = -1;
//    date_order.add_field_filter_condition("logdate", "");
//    decorator.add_primary_table_decorator(&date_order);
//
//    bigger_date.add_field_filter_condition("logdate", date_lim.c_str());
//    decorator.add_primary_table_decorator(&bigger_date);
//
//    MCPContentNews mcp_content_news("id");
//    mcp_content_news.set_unit_info(page_size,page);
//    mcp_content_news.load_decorator(&decorator);
//    mcp_content_news.general_fetch_list_records(true);
//
//    int ret_size = mcp_content_news.get_key_size();
//
//    for(int i = 0; i < ret_size; i++)
//    {
//        print_news_result(mcp_content_news.get_record_by_key_vec(i, 0));
//    }

//
//    const char* u_id = "315128";
//    const char* c_id = "51124";
//    int page = 0;
//    int page_num = 10;
//    LOGA("[GWJ] %s: start. u_id[%s], c_id[%s]", __FUNCTION__, u_id, c_id);
//
//    int ret = DuokuUserDownloadInfo::set_new_download(u_id, c_id);
//
//    if(ret < 0)
//    {
//        LOGA("[GWJ] %s: set_new_download Error[%d]", __FUNCTION__, ret);
//    }
//
//    c_id = "51154";
//    LOGA("[GWJ] %s: start. u_id[%s], c_id[%s]", __FUNCTION__, u_id, c_id);
//
//    ret = DuokuUserDownloadInfo::set_new_download(u_id, c_id);
//
//    if(ret < 0)
//    {
//        LOGA("[GWJ] %s: set_new_download Error[%d]", __FUNCTION__, ret);
//    }
//
//    c_id = "51155";
//    LOGA("[GWJ] %s: start. u_id[%s], c_id[%s]", __FUNCTION__, u_id, c_id);
//
//    ret = DuokuUserDownloadInfo::set_new_download(u_id, c_id);
//
//    if(ret < 0)
//    {
//        LOGA("[GWJ] %s: set_new_download Error[%d]", __FUNCTION__, ret);
//    }
//
//    LOGA("[GWJ] %s: start. u_id[%s]", __FUNCTION__, u_id);
//    ret = DuokuUserDownloadInfo::get_user_download_games_count(u_id);
//    LOGA("[GWJ] %s: ret[%d]", __FUNCTION__, ret);
//
//        content_input_params in_conditions;
//
//        LOGA("[GWJ] %s: start. u_id[%s]. page[%d], page_num[%d]",
//                __FUNCTION__, u_id, page, page_num);
//
//        vector<string> c_ids;
//        ret = DuokuUserDownloadInfo::get_user_download_games(u_id, c_ids);
//        int size = c_ids.size();
//
//        if(ret < 0 || size == 0)
//        {
//            LOGA("[GWJ] %s: download_games Error[%d]. Count[%d]", __FUNCTION__, ret, size);
//            return ;
//        }
//        else
//        {
//            LOGA("[GWJ] %s: download_games[%d], size[%d]", __FUNCTION__, ret, size);
//        }
//
//        OrDecorator id_or;
//
//        for(int i = 0; i < size; i++)
//        {
//            id_or.add_field_filter_condition("id", c_ids[i].c_str());
//        }
//
//        MCPGameContent mcp_content("id");
//        MCPContentData mcp_content_data("c_id");
//
//        mcp_content.load_decorator(&id_or);
//        mcp_content.set_unit_info(page_num, page);
//
//        ret = mcp_content.general_fetch_list_records(true);
//
//        if(ret < 0)
//        {
//            LOGA("[GWJ] %s: [mcp_content] fetch list Error! ", __FUNCTION__);
//            return ;
//        }
//
//        int record_count = mcp_content.get_key_size();
//
//        LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
//                __FUNCTION__, record_count);
//
//        if(record_count == 0)
//        {
//            LOGA("[GWJ] %s: general_fetch_list_records. Count[%d]", __FUNCTION__, record_count);
//            return ;
//        }
//
//        OrDecorator id_filter;
//        id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(),
//                                                   "c_id", true);
//
//        mcp_content_data.load_decorator(&id_filter);
//        mcp_content_data.general_fetch_list_records(false);
//
//        AbstractBinder content_data_binder;
//        content_data_binder.bind_two_modules(&mcp_content, &mcp_content_data);
//
//        record_count = mcp_content.get_key_size();
//
//        for(int i = 0; i < record_count; i++)
//        {
//            print_result(content_data_binder.get_binded_record(i));
//        }
//
//        record_count = mcp_content.get_total_result_count();

}

void mysql_test()
{
    /////////////////////////   /////////////////////////   /////////////////////////

    UB_LOG_TRACE( "get_user_grab_num_list start" );

        string user_id("1");
        uint32_t page(0);
        uint32_t page_size(10);

        MysqlTable mcp_content_user_grab("mcp_content_user_grab", "CUGRB");
        MysqlTable mcp_content_grab("mcp_content_grab", "GRB");
        mcp_content_user_grab.set_unit_info(page_size,page);
        mcp_content_user_grab.add_select_column("grab_id");
        mcp_content_user_grab.add_select_column("num");

        mcp_content_grab.add_select_column("c_id");
        mcp_content_grab.add_select_column("name");
        mcp_content_grab.add_select_column("start_time");
        mcp_content_grab.add_select_column("end_time");
        mcp_content_grab.add_select_column("info");

        mcp_content_user_grab["user_id"]->add_and_condition("=", user_id.c_str() );
        mcp_content_user_grab["grab_id"]->add_inner_join(&mcp_content_grab, "id", NULL);

        mcp_content_user_grab.process_query_sql("default_mysql", true);
        int result_size = mcp_content_user_grab.get_total_count_without_page();

        int k_size = mcp_content_user_grab.get_total_key_count();

        for(int i = 0; i < k_size; i++)
        {
            const char* c_id = mcp_content_user_grab.get_result_set_value(i, 0, "GRB.c_id");
            const char* _name = mcp_content_user_grab.get_result_set_value(i, 0, "GRB.name");
            const char* _start_time = mcp_content_user_grab.get_result_set_value(i, 0, "GRB.start_time");
            const char* _end_time = mcp_content_user_grab.get_result_set_value(i, 0, "GRB.end_time");
            const char* _info = mcp_content_user_grab.get_result_set_value(i, 0, "GRB.info");
        }

    //NoticeRPC::instance().notice_grab("123145", "空战英豪", "2453252423");
        const char* udid  = "Imei id 1123";
        const char* appid = "1";
        int game_id = atoi(appid);

        // 单机游戏商品表格，匿名为 GP
        MysqlTable goods_prop("pt_local_game_prop_info", "GP");

        // 一次性消费物品  is_consume_goods = 1
        goods_prop["is_consume_goods"]->add_and_condition("=", 1);
        goods_prop["game_id"]->add_and_condition("=", game_id);

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

        int key_size = usr_order_rel.get_total_key_count();
        int col_size = usr_order_rel.get_total_columns_count();

        for(int i = 0; i < key_size; i++)
        {
            const char* value = usr_order_rel.get_result_set_value(i, 0, "order_id");
            const char* consume_date = usr_order_rel.get_result_set_value(i, 0, "consume_date");
            const char* udid_ret = usr_order_rel.get_result_set_value(i, 0, "udid");
            const char* goods_id = usr_order_rel.get_result_set_value(i, 0, "goods_id");
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////
}
