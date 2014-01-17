#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr_content_common.h"
#include "mcpsvr.h"
#include <time.h>
//#define REAL_TIME_DATA
#define BRAND_PLAT_ST_TIMEOUT 60*60*24
#define ZCACHE_TIMEOUT 100

#include "UADicTree.h"
extern UAJsonDicTree uaJsonTree;
void* uaJson_create_thread(void *);

int McpServlet::get_phone_area_by_phone_num(const idl::mcp_get_phone_area_by_phone_num_params &in,
                                                  idl::mcp_get_phone_area_by_phone_num_response &out)
{
    clock_t start, finish;       
    map<int ,Data *>::iterator iter;
//    map<int,int> invalid_flag;
    char ph_area[8]="";
    int province;
    char invalid[2]="1";
    char valid[2]="0";
    int ret = 0;
    memset(ph_area, 0, sizeof(ph_area));
    memcpy(ph_area,in.phone_num()+1,6);
    UB_LOG_NOTICE("get_phone_area_by_phone_num is start"); 
    if (!(phonevalid_update_flag&mcpupdate_flag))
    {
        ret = create_phone_invalid_list();
        if ( MCP_NG == ret )
        {
            UB_LOG_FATAL("create_phone_invalid_list is NG");
        }
        mcpupdate_flag |= (1<<0);
        UB_LOG_NOTICE("mcpupdate_flag is %d",mcpupdate_flag); 
//        UB_LOG_NOTICE("mcpupdate_flag is %x",mcpupdate_flag); 
    }
    start = clock(); 		
    iter = database_map.find(atoi(ph_area));
    finish = clock();
//    UB_LOG_NOTICE("Time to do search start is %ld  finish is %ld seconds",start,finish); 
    if(iter != database_map.end()){
        out.m_result_params()->set_result("0");	
        out.m_result_params()->set_numseg((iter->second->numseg).c_str());
//        out.m_result_params()->set_type((iter->second->type).c_str());
        province = atoi((iter->second->province).c_str());
        if ( 0 == invalid_flag[province] )
        {
            out.m_result_params()->set_type(invalid);
        }
        else
        {
            out.m_result_params()->set_type(valid);
        }
        out.m_result_params()->set_oid((iter->second->oid).c_str());
        out.m_result_params()->set_provice((iter->second->province).c_str());
        out.m_result_params()->set_city((iter->second->city).c_str());
        out.m_result_params()->set_code((iter->second->code).c_str());
        out.m_result_params()->set_enabled((iter->second->enabled).c_str());
    }else{
        out.m_result_params()->set_result("-1");      
        out.m_result_params()->set_numseg("");
        out.m_result_params()->set_type("");
        out.m_result_params()->set_oid("");
        out.m_result_params()->set_provice("");
        out.m_result_params()->set_city("");
        out.m_result_params()->set_code("");
        out.m_result_params()->set_enabled("");
        UB_LOG_NOTICE("search_phone_area is failed");
    }
    return 0;
}


int McpServlet::reload_config_by_id(const idl::mcp_reload_config_by_id_params &in,
                                                  idl::mcp_reload_config_by_id_response &out)
{
    int config_id =0;
    
    config_id = in.reload_id();

    if ( config_id > 7 )
    {
        out.m_result_params()->set_result(MCP_NG);
        return MCP_OK;
    }
    mcpupdate_flag &= (~(1<<config_id)); 
   
    out.m_result_params()->set_result(MCP_OK);
    return MCP_OK;

}


int McpServlet::get_test(const idl::mcp_get_test_params &in, idl::mcp_get_test_response &out) 
{
    (void)in;
    (void)out;
    return 0;
}

int McpServlet::get_phone_uaJson(const idl::mcp_get_phone_uaJson_params& in, idl::mcp_get_phone_uaJson_response& out) 
{
    UB_LOG_NOTICE("%s start", __func__);
    string strua = in.ua();
    string strUaJson = "";
    //uaJsonTree.string_ToLower(strua);
    uaJsonTree.Search(strua, strUaJson);
    out.m_result_params()->set_ua_Json(0,strUaJson.data());
    return MCP_OK;
    //int nLen = strUaJson.length();
    //char* strResult = (char *)malloc((nLen+1)*sizeof(char));
    //memset(strResult, 0x00, nLen+1);
    //strResult[nLen]='\0';
    //memcpy(strResult, strUaJson.data(), nLen+1);
    //out.m_result_params()->set_ua_Json(0,innstr.data());
    /*   
     *       string strUa = in.strUa();
     *       string strUaJson = "";
     *       uaJsonTree.string_ToLower(strUa);
     *       uaJsonTree.Search(strUa, strUaJson);
     *       out.m_result_params()->set_uaJson(strUaJson.data());
     *       return MCP_OK;*/
}

int McpServlet::init_phone_uaJson_map(const idl::mcp_init_phone_uaJson_map_params& in, idl::mcp_init_phone_uaJson_map_response& out)
{
    UB_LOG_NOTICE("%s start", __func__);
    //uaJsonTree.destroyTree(&uaJsonTree.uaJsonNode); //对于新添加的元素，构建新的元素就好了，原来的内存不用析构

    pthread_t uajson_thread;
    pthread_create(&uajson_thread, NULL, uaJson_create_thread, NULL);

    out.m_result_params()->set_output(0,"init_phone_uaJson_map");
    return MCP_OK;
}

int McpServlet::get_phone_maker_plat(const idl::mcp_get_phone_maker_plat_params& in, idl::mcp_get_phone_maker_plat_response& out)
{
    ul_writelog(UL_LOG_NOTICE,"get_phone_maker_plat is start");
    const uint32_t info_type = in.info_type();
    const uint32_t unit_num = in.unit_num();
    const uint32_t unit_times = in.unit_times();
    std::vector<std::string> id_list;
    std::vector<std::string> name_list;
    uint32_t min_num,max_num;
    int count_num = 0;
    min_num = unit_times * unit_num - unit_num;
    max_num =  unit_times * unit_num - 1;
    int ret = 0;
    int err = 0;
    int cache_flag = 0;
    MyclientRes data_res;

#ifndef REAL_TIME_DATA

    string valueout;
    //int logid= rand();
    
    string mainkey;
    string subkey_count;
    string subkey_list;
    
    if ( PHONE_MAKER == info_type )
    {
        //  获取厂商总数
        mainkey = "mcp_phone_brand";
        subkey_list = "brand_list";
    }
    else if (PHONE_PLAT == info_type )
    {
        mainkey = "mcp_phone_plat";
        subkey_list = "plat_list";
    }
    else
    {
        return 0;
    }
    subkey_count = "count";

    mcp_session conn;  
    ret = conn.session_search( mainkey, subkey_count, valueout);
    count_num = atoi( valueout.c_str() );
    if ( 0 == ret )
    {
        UB_LOG_NOTICE("count_num is %d", count_num );
        out.m_result_params()->set_info_count( count_num );
    }
    else
    {
        UB_LOG_FATAL("query cache data failed!!");
        cache_flag = MCP_NG;
    }
    // 厂商总数未命中 或者 厂商列表未命中
    if( 0 == cache_flag)
    {
        valueout.clear();
        mcp_brand_st maker_plat_list[count_num];
        // 厂商总数未命中 或者 厂商列表未命中
        ret = conn.session_search( mainkey, subkey_list, valueout);
        if (0 != ret )
        {
            UB_LOG_FATAL("query cache data failed!!");
            ret = MCP_NG;
        }
        UB_LOG_NOTICE("get maker list from session is ok");
        Json::Reader json_reader;
        Json::Value json_value;
        string tmpmsg;
        size_t pos= 0;
        int k = 0;
        while( (pos = valueout.find_first_of( '{', pos )) < valueout.size() && k < count_num )
        {
            tmpmsg.assign( valueout, pos, valueout.find_first_of( '}', pos ) - pos +1 );
            ++pos;
            if(json_reader.parse(tmpmsg,json_value))
            {
                strlcpy( maker_plat_list[k].id, json_value["id"].asString().c_str(), sizeof(maker_plat_list[k].id) );
                strlcpy( maker_plat_list[k].name, json_value["name"].asString().c_str(), sizeof(maker_plat_list[k].name) );
                strlcpy( maker_plat_list[k].phone_count, json_value["phone_count"].asString().c_str(), sizeof(maker_plat_list[k].phone_count) );
            }
            k++;
        }

        int j = min_num;

        UB_LOG_NOTICE("set out params is start, min_num is %d ,unit_num is %d",min_num, unit_num);
        for ( int i = 0; i< (int)unit_num; i++)
        {
            if( j == count_num)
            {
                UB_LOG_NOTICE("set out params is ok,unit_num is more than count_num");
                return MCP_OK;
            }
            out.m_result_params()->set_info_id(i, maker_plat_list[j].id);
            out.m_result_params()->set_info_name(i,maker_plat_list[j].name);

            out.m_result_params()->set_phone_count(i,atoi(maker_plat_list[j].phone_count));

            j++;
        }
    }

    if( 0  != cache_flag )
    {
        MyclientRes data_res;
        char query_sql[1024];
        memset(query_sql, 0x00, sizeof(query_sql));
        UB_LOG_NOTICE("fetchConnection is start");
        MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
        if ( NULL == database_connection )
        {
            UB_LOG_FATAL("get database connection failed from database_pool");
            data_res.free();
            return 0;
        }
        if ( PHONE_MAKER == info_type )
        {
                //  获取厂商总数
            sprintf(query_sql,
                    "SELECT mcp_phone_brand.id, mcp_phone_brand.name, COUNT( mcp_phone_info.id ) "
			"FROM mcp_phone_brand, mcp_phone_info "
			"WHERE mcp_phone_brand.name !=  '' "
			"AND mcp_phone_brand.id = mcp_phone_info.brand_id "
			"GROUP BY mcp_phone_brand.id "
                        "ORDER BY COUNT( mcp_phone_info.id ) DESC");
        }
        else
        {
            sprintf(query_sql,
                    "SELECT mcp_phone_plat.id, mcp_phone_plat.name, COUNT( mcp_phone_plat.id ) "
                        "FROM mcp_phone_plat, mcp_phone_info "
                        "WHERE mcp_phone_plat.name !=  '' "
                        "AND mcp_phone_plat.id = mcp_phone_info.plat_id "
                        "GROUP BY mcp_phone_plat.id "
                        "ORDER BY COUNT( mcp_phone_plat.id ) DESC");
        }
        UB_LOG_NOTICE(" query database is start");
        UB_LOG_NOTICE("%s",query_sql);
        count_num = database_connection->query(query_sql, &data_res, true);
        if ( count_num < 0 )
        {
            UB_LOG_FATAL("query data from database failed");
            ret = database_pool->putBackConnection(database_connection, false);
            if (ret != 0)
            {
               ul_writelog(UL_LOG_FATAL, "putback connection is failed");
            }
            data_res.free();
            return 0;
        }
        UB_LOG_NOTICE("get database is ok");
        int out_count = 0;
        for ( int i = 0; i < count_num ; i++)
        {
            if ( 0 == strcmp(data_res[i][3], "0") )
            {
                UB_LOG_NOTICE("data_res[%d][3] is 0", i );
                continue;
            }
            out_count++;
        }
        mcp_brand_st maker_plat_list[out_count];
        int out_i = 0;
        for ( int i = 0; i < count_num ; i++)
        {
            if ( 0 == strcmp(data_res[i][3], "0") )
            {
                continue;
            }
            strcpy(maker_plat_list[out_i].id, data_res[i][0]);
            strcpy(maker_plat_list[out_i].name,data_res[i][1]);
            strcpy(maker_plat_list[out_i].phone_count, data_res[i][2]);
            ++out_i;
        }
        out.m_result_params()->set_info_count(out_count);
        count_num = out_count;



        UB_LOG_NOTICE("putBackConnection is start");
        int j = min_num;
        UB_LOG_NOTICE("set out params is start, min_num is %d ,unit_num is %d",min_num, unit_num);
        for ( int i = 0; i< (int)unit_num && j < out_count; i++)
        {
            UB_LOG_NOTICE("maker_plat_list[%d].id is %s", j , maker_plat_list[j].id);
            UB_LOG_NOTICE("maker_plat_list[%d].name is %s", j , maker_plat_list[j].name);
            out.m_result_params()->set_info_id(i, maker_plat_list[j].id);
            out.m_result_params()->set_info_name(i,maker_plat_list[j].name);
            out.m_result_params()->set_phone_count(i,atoi(maker_plat_list[j].phone_count));

            j++;
        }
        UB_LOG_NOTICE("set out paramas is ok");
        data_res.free();
        ret = database_pool->putBackConnection(database_connection, false);
        if (ret != 0)
        {
           ul_writelog(UL_LOG_FATAL, "putback connection is failed");
        }

        //  插入数据库获得的数据
        char num[12];
	sprintf(num, "%d", count_num);
	string value = num;
        ret = conn.session_search(mainkey, subkey_count, value);
        if (0 != ret)
        {
            UB_LOG_FATAL("insert fail!");
            return 0;
        }
        UB_LOG_NOTICE("session insert maker count %d is ok",count_num);

        value.clear();
	string id;
	string name;
	string phone_count;
        for( int i=0; i< count_num; i++)
        {
	   id.clear();
	   name.clear();
	   phone_count.clear();
	   id = maker_plat_list[i].id;
	   name = maker_plat_list[i].name;
	   phone_count = maker_plat_list[i].phone_count;
	   value += "{id:" + id + ",name:" + name + ",phone_count:" + phone_count + "}" ;
        }

        ret = conn.session_insert(mainkey, subkey_list, value, BRAND_PLAT_ST_TIMEOUT);
        if (0 != ret)
        {
            UB_LOG_FATAL("insert fail!");
            return 0;;
        }
    }


    UB_LOG_NOTICE("get_phone_maker_plat is  end");
    return MCP_OK;

#else

    char query_sql[1024];
    MysqlConnection *database_connection;

    memset(query_sql, 0x00, sizeof(query_sql));
    // 从连接池中获取一个连接
    database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        data_res.free();
        return 0;
    }
    // 根据输入的信息类型处理
    switch ( info_type )
    {
        // 厂商的场合
        case PHONE_MAKER:
            //  获取厂商总数
            count_num = database_connection->selectCount("select count(*) from mcp_phone_brand"); 
            out.m_result_params()->set_info_count(count_num);
            if ( count_num < 0 ) // 获取厂商总数失败的场合
            {
                ret = database_pool->putBackConnection(database_connection, false);
                if (ret != 0)
                {
                    ul_writelog(UL_LOG_FATAL, "putback connection is failed");
                }
                data_res.free();
                ul_writelog(UL_LOG_FATAL, "get maker count is failed ");
                return 0;
            } 
            sprintf(query_sql, 
               "SELECT id,name FROM mcp_phone_brand WHERE name != '' ORDER BY  `order` DESC LIMIT %d , %d ", min_num, unit_num);
            ret = database_connection->query(query_sql, &data_res, true);
            for ( int i = 0; i < ret ; i++)
            {
                out.m_result_params()->set_info_id(i, data_res[i][0]);
                out.m_result_params()->set_info_name(i,data_res[i][1]);
                sprintf(query_sql, "SELECT  count(*) FROM mcp_phone_info WHERE brand_id = %s", 
                                     data_res[i][0]);
                count_num = database_connection->selectCount(query_sql);
                out.m_result_params()->set_phone_count(i,count_num);
            }
            ret = database_pool->putBackConnection(database_connection, false);
            if (ret != 0)
            {
               ul_writelog(UL_LOG_FATAL, "putback connection is failed");
            }
 
            break;

        // 的场合
        case PHONE_PLAT:
            //  平台
            count_num = database_connection->selectCount("select count(*) from mcp_phone_plat");
            out.m_result_params()->set_info_count(count_num);
            if ( count_num < 0 ) // 获取总数失败的场合
            {
                ret = database_pool->putBackConnection(database_connection, false);
                if (ret != 0)
                {
                   ul_writelog(UL_LOG_FATAL, "putback connection is failed");
                }
                data_res.free();
                ul_writelog(UL_LOG_FATAL, "get maker count is failed ");
                return 0;
            }
            sprintf(query_sql, 
                "SELECT id,name FROM mcp_phone_plat WHERE name != '' ORDER BY  `order` DESC LIMIT %d , %d ", min_num, unit_num);
            ret = database_connection->query(query_sql, &data_res, true);
            for ( int i = 0; i < ret ; i++)
            {
                out.m_result_params()->set_info_id(i, data_res[i][0]);
                out.m_result_params()->set_info_name(i,data_res[i][1]);
                sprintf(query_sql, "SELECT  count(*) FROM mcp_phone_info WHERE plat_id = %s",
                                     data_res[i][0]);
                count_num = database_connection->selectCount(query_sql);
                out.m_result_params()->set_phone_count(i,count_num);
            }
            ret = database_pool->putBackConnection(database_connection, false);
            if (ret != 0)
            {
               ul_writelog(UL_LOG_FATAL, "putback connection is failed");
            }

            break;

        default:
            ret = database_pool->putBackConnection(database_connection, false);
            if (ret != 0)
            {
               ul_writelog(UL_LOG_FATAL, "putback connection is failed");
            }
            ul_writelog(UL_LOG_FATAL, "input info type is %d invalid", info_type);
            data_res.free();
            return 0;
            break;
    }
    data_res.free();
#endif
    return MCP_OK;
    
};

ubrpc::Servlet * McpServer::createServlet()
        {
            return new McpServlet ;
        };


