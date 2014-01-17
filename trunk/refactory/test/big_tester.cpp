/*
 * big_tester.cpp
 *
 *  Created on: 2013-4-28
 *      Author: zhengxie
 */
#include "big_tester.h"

using namespace std;

//typedef struct test_struct
//{
//    string id;
//    string value;
//    string date;
//    string user;
//}test_struct;
//
//map<char*, test_struct> g_map_set;
//vector<char*> g_list;
//
//map<char*, test_struct> g_map_set_2;
//vector<char*> g_list_2;
//
//map<char*, vector<test_struct> > g_map_v;
//vector<char*> g_list_v;
//
//int find_key_index_(char* value, vector<char*>& list)
//{
//    int size = list.size();
//
//    for(int i = 0; i < size; i++)
//    {
//        if(strcmp(value, list[i]) == 0)
//        {
//            return i;
//        }
//    }
//
//    return -1;
//}
//
//void set_map_v()
//{
//    char index[8];
//
//    for(int j = 0; j < 3; j++)
//    {
//        for(int i = 0; i < 3; i++)
//        {
//            test_struct detail;
//
//            memset(index, 0x00, sizeof(index));
//            sprintf(index, "%d", j);
//            detail.id = string(index);
//            g_list.push_back((char*)detail.id.c_str());
//
//            memset(index, 0x00, sizeof(index));
//            sprintf(index, "%d_%d_%s", j, i, "value");
//            detail.value = string(index);
//
//            memset(index, 0x00, sizeof(index));
//            sprintf(index, "%d_%d_%s", j, i, "date");
//            detail.date = string(index);
//
//            memset(index, 0x00, sizeof(index));
//            sprintf(index, "%d_%d_%s", j, i, "user");
//            detail.user = string(index);
//
//            int index = find_key_index_((char*)detail.id.c_str(), g_list_v);
//
//            if(index < 0)
//            {
//                g_list_v.push_back((char*)detail.id.c_str());
//                g_map_v[(char*)detail.id.c_str()].push_back(detail);
//            }
//            else
//            {
//                g_map_v[(char*)g_list_v[index]].push_back(detail);
//            }
//        }
//    }
//}
//
//void set_map_set()
//{
//    char index[8];
//
//    for(int i = 0; i < 3; i++)
//    {
//        test_struct detail;
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d", i);
//        detail.id = string(index);
//        g_list.push_back((char*)detail.id.c_str());
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d_%s", i, "value");
//        detail.value = string(index);
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d_%s", i, "date");
//        detail.date = string(index);
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d_%s", i, "user");
//        detail.user = string(index);
//
//        g_map_set[(char*)detail.id.c_str()] = detail;
//    }
//}
//
//void set_map_set_2()
//{
//    char index[8];
//
//    for(int i = 0; i < 3; i++)
//    {
//        test_struct detail;
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d", i+1);
//        detail.id = string(index);
//        g_list_2.push_back((char*)detail.id.c_str());
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d_%s_2", i+1, "value");
//        detail.value = string(index);
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d_%s_2", i+1, "date");
//        detail.date = string(index);
//
//        memset(index, 0x00, sizeof(index));
//        sprintf(index, "%d_%s_2", i+1, "user");
//        detail.user = string(index);
//
//        g_map_set_2[(char*)detail.id.c_str()] = detail;
//    }
//}
//
//char* get_select_query(vector<char*>& select_fields)
//{
//    string query = "SELECT ";
//    int size = select_fields.size();
//
//    for(int i = 0; i < size; i++)
//    {
//        if(i == 0)
//        {
//            query = query + "`" + string(select_fields[i]) + "`";
//        }
//        else
//        {
//            query = query + ", `" + string(select_fields[i]) + "`";
//        }
//    }
//
//    return (char*)query.c_str();
//}
//
//char* get_rest_query()
//{
//    char* value = "1";
//    string query;
//    query = "From table where id = " + string(value);
//    return (char*)query.c_str();
//}
//
//char* get_mysql_query(vector<char*>& select_fields)
//{
//    string query;
//    query = string(get_select_query(select_fields)) + string(get_rest_query());
//    return (char*)query.c_str();
//}
//
//vector<char*> id_s;
//char* key_name;
//vector<char*> select_fields;
//map<char*, vector<MySQLRecord> > result_set;
//
//void set_mysql_result_set(vector<vector<string> >& records_data, int count)
//{
//    int size = select_fields.size();
//
//    for(int i = 0; i < count; i++)
//    {
//        MySQLRecord record;
//
//        for(int j = 0; j < size; j++)
//        {
//            record.record[select_fields[j]] = records_data[i][j];
//        }
//
//        int index = find_key_index_((char*)record.record[key_name].c_str(), id_s);
//
//        if(index < 0)
//        {
//            id_s.push_back((char*)record.record[key_name].c_str());
//            result_set[(char*)record.record[key_name].c_str()].push_back(record);
//        }
//        else
//        {
//            result_set[(char*)id_s[index]].push_back(record);
//        }
//    }
//}
//
//int littel_test_main()
//{
//    set_map_set();
//    set_map_set_2();
//    set_map_v();
//
//    select_fields.push_back("id");
//    select_fields.push_back("value");
//    select_fields.push_back("name");
//    select_fields.push_back("date");
//
//    key_name = select_fields[0];
//
//    vector<vector<string> > records_data;
//
//    char istr[3];
//    char jstr[3];
//
//    for(int i = 0; i < 3; i++)
//    {
//        memset(istr, 0, 3);
//        sprintf(istr, "%d", i);
//        
//        vector<string> row;
//
//        for(int j = 0; j < 4; j++)
//        {
//            memset(jstr, 0, 3);
//            sprintf(jstr, "%d", j);
//
//            if(j == 0)
//            {
//                row.push_back(string(istr));
//            }
//            else
//            {
//                row.push_back(string(istr) + "-" + string(jstr));
//            }
//        }
//
//        records_data.push_back(row);
//    }
//
//    set_mysql_result_set(records_data, 3);
//    char* ret = get_mysql_query(select_fields);
//
//    for(int i = 0; i < id_s.size(); i++)
//    {
//        vector<MySQLRecord> id_rows = result_set[id_s[i]];
//
//        for(int j = 0; j < id_rows.size(); j++)
//        {
//            MySQLRecord row_0 = id_rows[j];
//
//            for(int x = 0; x < select_fields.size(); x++)
//            {
//                string value = row_0.record[select_fields[x]];
//
//                printf("id[%s], column_name[%s], value[%s]\n", id_s[i], select_fields[x], value.c_str());
//            }
//        }
//    }
//
//    //char* value = (char*)(g_map_set["34"].value.c_str());
//    printf("value:%s", ret);
//
//    return 0;
//}
//
//void print_result(TwoBindedRecord* binded_record)
//{
//    if(binded_record == NULL || binded_record->get_primary_record(0) == NULL)
//    {
//        LOGXX("[GWJ] %s: binded_record Or Basic Info is NULL", __FUNCTION__);
//        return;
//    }
//
//    /*
//     *  涓�埇鏉ヨ鍙幓璇ョ储寮曚笅鐨勭涓�潯鏁版嵁鍗冲彲
//     *  鍥犱负鍦ㄨ缃父鎴忓熀鏈俊鎭椂涓�埇鍙湁涓�潯鏁版嵁瀵瑰簲涓�釜绱㈠紩
//     */
//    MCPGameInfo* record = (MCPGameInfo*)binded_record->get_primary_record(0);
//
//    const char* id = record->id.c_str();
//    LOGXX("[GWJ] %s: id[%s]", __FUNCTION__, id);
//
//    const char* name = record->name.c_str();
//    const char* type = record->content_type.c_str();
//    const char* ext_id = record->ext_id.c_str();
//    const char* abstract_ = record->info.c_str();
//    const char* version = record->version.c_str();
//    const char* method = record->method.c_str();
//    const char* star = record->star.c_str();
//    const char* network = record->network_id.c_str();
//    const char* code = record->game_code.c_str();
//    const char* log_date = record->logdate.c_str();
//    const char* mod_date = record->modify_date.c_str();
//    const char* dft_charge_type = record->default_charge_typeid.c_str();
//    const char* dft_charge_val = record->default_charge_value.c_str();
//    const char* _enName = record->enName.c_str();
//    const char* language = record->language.c_str();
//    const char* adapt = record->adapt.c_str();
//    const char* package_name = record->package_name.c_str();
//    const char* version_appinfo = record->version_appinfo.c_str();
//    const char* sdk_version = record->sdk_version.c_str();
//    const char* version_code = record->version_code.c_str();
//    const char* ptype_0 = record->ptype_1.c_str();
//    const char* ptype_1 = record->ptype_2.c_str();
//    const char* ptype_2 = record->ptype_3.c_str();
//    const char* ptype_3 = record->ptype_4.c_str();
//    const char* ptype_4 = record->ptype_5.c_str();
//
//    string cp_name;
//    mcp_content_map::get_cp_name(record->cp_id, cp_name);
//    const char* cp_name_ptr = cp_name.c_str();
//    const char* cp_id_ptr = record->cp_id.c_str();
//
//    grant_detail_t grant_info;
//    mcp_content_map::get_grant_info(record->grant_id, grant_info);
//    const char* grant_id = record->grant_id.c_str();
//    const char* g_modify_time = grant_info.modify_time.c_str();
//    const char* ggrant_time = grant_info.grant_time.c_str();
//    const char* gvalid_time = grant_info.valid_time.c_str();
//
//    vector<MCPTupleItem>::const_iterator itr = record->classes.begin();
//    vector<MCPTupleItem>::const_iterator end_itr = record->classes.end();
//
//    for(int i = 0; itr != end_itr; itr++, i++)
//    {
//        class_info_t class_info;
//        mcp_content_map::get_class_info((*itr).key, class_info);
//
//        const char* cname = class_info.name.c_str();
//        const char* cid = (*itr).key.c_str();
//
//        LOGXX("[GWJ] %s: c_id[key:%s, name:%s]", __FUNCTION__, cid, cname);
//    }
//
//    itr = record->img_info.begin();
//    end_itr = record->img_info.end();
//    for(int i = 0; itr != end_itr; itr++, i++)
//    {
//        const char* itype = (*itr).key.c_str();
//        const char* iurl = (*itr).value.c_str();
//    }
//
//    if(binded_record->get_secondary_record(0) == NULL)
//    {
//        LOGD("[GWJ] %s: DownLoad Info is NULL", __FUNCTION__);
//        return;
//    }
//
//    int download_size = binded_record->get_secondary_record_size();
//
//    for(int i = 0; i < download_size; i++)
//    {
//        MCPContentDataInfo* download_info =
//                (MCPContentDataInfo*)binded_record->get_secondary_record(i);
//
//        const char* did = download_info->id.c_str();
//        const char* durl = download_info->path_url.c_str();
//        const char* durl_type = download_info->path_type.c_str();
//        const char* dmedia_format = download_info->media_format_id.c_str();
//        const char* dsize = download_info->size.c_str();
//        const char* dmtype = download_info->mtype.c_str();
//        const char* dcharge_id = download_info->charge_typeid.c_str();
//        const char* dcharge_value = download_info->value.c_str();
//    }
//}

//int test_fun2()
//{
//    IsDecorator type_decor;
//    OrDecorator class_or_decor;
//    OrDecorator label_or_dec;
//    WeightDecorator weight_decor;
//    AbstractCompoundDecorator compound_decor;
//
//    content_input_params in_conditions;
//
//    //////////////////////////////////////////////////////////// 澶勭悊 content_type 绫诲瀷 ////////////////////////////////////////////////////////////
//    const char* type_id = "123415";//in.in_param().type_id();
//
//    if(IS_VALUED_STRING(type_id))
//    {
//
//        type_decor.add_field_filter_condition("content_type", type_id);
//        compound_decor.add_primary_table_decorator(&type_decor);
//    }
//
//    //////////////////////////////////////////////////////////// 澶勭悊 Class.id 绫诲瀷 //////////////////////////////////////////////////////////////
//    const char* class_id = "35253463";//in.in_param().class_id();
//    LOGD("[GWJ] %s: class.id[%s]", __FUNCTION__, class_id);
//
//    /*
//     *  銆愭敞鎰忋�
//     *  淇濆瓨 class_id 鐨勫疄浣�//     *  鍥犱负鍦�Decorator 涓彧淇濆瓨鎸囬拡
//     *  鐪熸鐨勫疄浣撳彧鏈夎繖涓�唤
//     *
//     *  鍦ㄩ檮琛� mcp.mcp_content_class_list 涓瓫閫� class_id 椤�//     */
//    vector < string > class_ids;
//
//    if(strcmp(class_id, "-1") == 0)
//    {
//        // 绛涢�鎵�湁娓告垙绫诲瀷
//        char i_str[8];
//
//        for(int i = 97; i < 103; i++)
//        {
//            memset(i_str, 0x00, sizeof(i_str));
//            sprintf(i_str, "%d", i);
//            class_ids.push_back(i_str);
//
//            class_or_decor.add_field_filter_condition("class.id",
//                                                      class_ids[i - 97].c_str());
//        }
//
//        class_or_decor.add_field_filter_condition("class.id", "104");
//        class_or_decor.add_field_filter_condition("class.id", "108");
//    }
//    else if(strcmp(class_id, "-2") == 0)
//    {
//        // 绛涢�鍑烘墍鏈夊崟鏈烘父鎴�//        char i_str[8];
//
//        for(int i = 97; i < 103; i++)
//        {
//            memset(i_str, 0x00, sizeof(i_str));
//            sprintf(i_str, "%d", i);
//            class_ids.push_back(i_str);
//
//            class_or_decor.add_field_filter_condition("class.id",
//                                                      class_ids[i - 97].c_str());
//        }
//
//        class_or_decor.add_field_filter_condition("class.id", "104");
//    }
//    else if(strcmp(class_id, "-3") == 0)
//    {
//        // 绛涢�鍑烘墍鏈夌綉缁滄父鎴�//        class_or_decor.add_field_filter_condition("class.id", "108");
//    }
//    else
//    {
//        // 鎸夌収鎸囧畾鐨�class.id 杩涜绛涢�
//        class_or_decor.add_field_filter_condition("class.id", class_id);
//    }
//
//    compound_decor.add_primary_table_decorator(&class_or_decor);
//
//    //////////////////////////////////////////////////////////// 澶勭悊 Lable 鏍囩 //////////////////////////////////////////////////////////////
//    const char* label_ids = "124846,18423428,123423849,1234234850";//in.in_param().label_id();
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
//    //////////////////////////////////////////////////////////// 澶勭悊 鏉冮噸//////////////////////////////////////////////////////////////
//    weight_decor.weight_type
//            = WeightDecorator::get_weight_type("3");//in.in_param().weight_order());
//
//    compound_decor.add_primary_table_decorator(&weight_decor);
//
//    //////////////////////////////////////////////////////////// 澶勭悊 img_type 鍥剧墖绫诲瀷 //////////////////////////////////////////////////////////////
//    const char* img_types = "7";//in.in_param().img_type();
//
//    if(IS_VALUED_STRING(img_types))
//    {
//        /*
//         *  img_types 瑕佸湪杈撳嚭鏃惰繘琛屽垽鏂繃婊わ紝鎵�互娌℃湁 Decorator
//         */
//        string img_types_str = img_types;
//        CommonInterface::splic_string(img_types_str, in_conditions.img_types, ",");
//    }
//
//    /*
//     *  璁剧疆 module
//     *  鏌ヨ mcp.mcp_content 琛�//     *  骞朵笌 mcp.mcp_content_data 琛ㄨ繘琛屾暟鎹粡鍚�//     */
//    MCPGameContent mcp_content;
//    MCPContentData mcp_content_data;
//
//    // 缁�module 瑁呭瑁呴グ鍣�//    mcp_content.load_decorator(&compound_decor);
//    mcp_content.set_unit_info(30, 0);
//
//    /*
//     *  鎵ц瑁呴厤濂界殑 module
//     *  浠�mongoDB 鑾峰彇鏁版嵁锛屽苟鎸夌収鎸囧畾椤甸潰瑙勬牸鑾峰彇
//     *  淇濆瓨鍒�mcp_content鍐呴儴鐨�result_set 涓�//     */
//    mcp_content.general_fetch_list_records(true);
//
//    /*
//     *  GAME_DOWNLOAD_INFO 瑕佷粠鏁版嵁搴撲腑鎵惧埌  c_id 涓� GAME_BASIC_INFO.id 鐨�Record
//     */
//    OrDecorator id_filter;
//    id_filter.set_same_field_filter_conditions(mcp_content.get_result_keys(),
//                                               "c_id", true);
//
//    // 瑁呴厤  module
//    mcp_content_data.load_decorator(&id_filter);
//
//    /*
//     *  鎵ц瑁呴厤濂界殑 GAME_DOWNLOAD_INFO
//     *  涓嶅繀鑰冭檻椤甸潰澶у皬锛岃幏鍙栨墍鏈夋暟鎹嵆鍙�//     */
//    mcp_content_data.general_fetch_list_records(false);
//
//    /*
//     *  鎵ц榛忓悎宸ヤ綔
//     *  娉ㄦ剰椤哄簭锛屽墠闈㈢殑鏄粡鍚堜富琛紝鍚庨潰鐨勬槸闄勮〃
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
//}

void mysql_test2()
{
    UB_LOG_TRACE( "get_grab_list start" );

    string type_id("25");
    string c_id;
    uint32_t id(0);
    uint32_t page(1);
    uint32_t page_size(10);

    MysqlTable mcp_content_grab("mcp_content_grab", "GRB");
	mcp_content_grab.set_unit_info(page_size,page);

    mcp_content_grab.add_select_column("id");
    mcp_content_grab.add_select_column("name");
    mcp_content_grab.add_select_column("c_id");
    mcp_content_grab.add_select_column("info");
    mcp_content_grab.add_select_column("start_time");
    mcp_content_grab.add_select_column("end_time");
    mcp_content_grab.add_select_column("amount");
    mcp_content_grab.add_select_column("send_amount");
	
	if( !type_id.empty() )
	{
    	mcp_content_grab["content_type"]->add_and_condition("=", type_id.c_str() );
     	mcp_content_grab["enable"]->add_and_condition("=", 1);
     	mcp_content_grab["start_time"]->add_orderby_decrease();
	}
	else if( !c_id.empty() )
	{
    	mcp_content_grab["c_id"]->add_and_condition("=",  c_id.c_str() );
     	mcp_content_grab["enable"]->add_and_condition("=", 1);
     	mcp_content_grab["start_time"]->add_orderby_decrease();
	}
	else if( id != 0 )
	{
    	mcp_content_grab["id"]->add_and_condition("=",  id );
     	mcp_content_grab["enable"]->add_and_condition("=", 1);
	}
	
	mcp_content_grab.process_query_sql("default_mysql", true);
	int result_size = mcp_content_grab.get_total_count_without_page();

    // 璁剧疆杈撳嚭鍙傛暟
   // grab_info_t::set_grab_info_out(out.m_result_params()->mutable_grab_list(), mcp_content_grab);
    //out.m_result_params()->set_count(result_size);
    
    LOGD("[GWJ] %s: item_count[%hu]", __FUNCTION__, result_size);
    UB_LOG_TRACE( "get_grab_list end" );
	return;
//	MysqlTable usr_order_rel("pt_user_consume_rel", "UOR");
//
//    usr_order_rel.add_select_column("id");
//    usr_order_rel.add_select_column("order_id");
//    usr_order_rel.add_select_column("goods_id");
//    usr_order_rel.add_select_column("consume_date");
//
//    /*
//     *  銆愭敞鎰忋�
//     *
//     *  琛ㄤ腑绗竴涓垽鏂鍙ラ�鐢ㄤ簬杩炴帴澶栧眰鏉′欢鐨勶紝鍗宠〃涓庤〃涔嬮棿閫昏緫鑱旂郴鐨�//     *  绗竴涓潯浠惰鐗瑰埆娉ㄦ剰淇濊瘉姝ｇ‘
//     *
//     *  銆愭敞鎰忋�
//     *  濡傛灉鏈�JOIN 杩炴帴琛ㄧ殑璇濅細鍏堥�褰掔殑鎵ц JOIN 琛ㄤ腑鐨�WHERE 閫昏緫
//     *  鐒跺悗鎵ц涓昏〃涓殑 WHERE 閫昏緫
//     *
//     *  鎵�互鍦ㄥ啓鍒ゆ柇璇彞鏃惰鐗瑰埆娉ㄦ剰椤哄簭
//     */
//    usr_order_rel["device_id"]->add_and_condition("Imei id 112");
//    usr_order_rel["id"]->add_or_condition("=", 6);
//
//    MysqlTable goods_prop("pt_local_game_prop_info", "GP");
//    goods_prop["is_consume_goods"]->add_and_condition(1);
//    goods_prop["game_id"]->add_and_condition(1);
//
//    goods_prop["game_id"]->add_or_condition("=", 4);
//    goods_prop["trigger_type"]->add_and_condition(1);
//    usr_order_rel["goods_id"]->add_left_join(&goods_prop, "id", NULL);
//
//    usr_order_rel.process_query_sql("dev_platform_mysql");
//
//    int key_size = usr_order_rel.get_total_key_count();
//    int col_size = usr_order_rel.get_total_columns_count();
//
//    for(int i = 0; i < key_size; i++)
//    {
//        for(int j = 0; j < col_size; j++)
//        {
//            const char* value = usr_order_rel.get_result_set_value(i, 0, j);
//            LOGD("[GWJ] %s: value[%s]", __FUNCTION__, value);
//        }
//    }
//
//    MysqlTable pt_tester_game_rel("pt_tester_game_rel", "TGR");
//    pt_tester_game_rel["game_id"]->set_value(1);
//    pt_tester_game_rel["tester_id"]->set_value(3);
//    pt_tester_game_rel["logdata"]->set_value("2013-05-13");
//    pt_tester_game_rel.add_new_insert_transaction();
//
//    pt_tester_game_rel.process_insert_transaction("dev_platform_mysql");
}
