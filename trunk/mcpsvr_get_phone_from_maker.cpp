#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"

#define CACHE_EXPIRED_TIME 60*60*24

struct phone_name_st
{
    char id[12];
    char name[201];
    char brand_name[201];

    phone_name_st()
    {
        memset( id, 0x00, sizeof(id) );
        memset( name, 0x00, sizeof(name) );
        memset( brand_name, 0x00, sizeof(brand_name) );
    }
};

int McpServlet::get_phone_info_from_maker_plat(const idl::mcp_get_phone_info_from_maker_plat_params& in, idl::mcp_get_phone_info_from_maker_plat_response& out)
{
    UB_LOG_NOTICE( "get_phone_info_from_maker_plat start" );

    char maker_id[256];
    strcpy( maker_id, in.maker_id() );
    char plat_id[256];
    strcpy( plat_id, in.plat_id() );
    uint32_t unit_num = in.unit_num();
    uint32_t unit_times = in.unit_times();

    //根据希望取得数据单位和位次计算获取数据的开始位置
    int start_pos = unit_num * ( (int)unit_times - 1 );

    int ret = 0;
    int err = 0;
    
    string  value;
    string  valueout;

    string mainkey_name = "brand_plat_phone_info";
    char subkey1_name[512];
    sprintf( subkey1_name, "%s_%s_count", maker_id, plat_id );
    char subkey2_name[512] ;
    sprintf( subkey2_name, "%s_%s", maker_id, plat_id );

    string mainkey = "brand_plat_phone_info";
    string subkey1 = subkey1_name;
    string subkey2 = subkey2_name;
    int count_num = 0;
    int count_index = 0;

    mcp_session conn;
    ret = conn.session_search( mainkey, subkey1, valueout);
    
    //从cache中获取对应厂商和平台的手机信息总数成功
    if( 0 == ret )
    {
        UB_LOG_NOTICE( "get phone count from cache success" );
        count_num = atoi( valueout.c_str() );
        //开始位置大于数据总数的情况
        if( start_pos < 0 || start_pos > count_num - 1 )
        {
            UB_LOG_NOTICE( "start_pos is bigger than count_num" );
            out.m_result_params()->set_phone_count( count_num );
            //返回值？？
            return MCP_OK;
        }

        phone_name_st phone_list[count_num];

        valueout.clear();

        string valueout;
	
        ret = conn.session_search( mainkey, subkey2, valueout);
        //从cache中获取对应厂商和平台的手机信息列表成功
        if( 0 == ret )
        {
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
		  strlcpy( phone_list[k].id, json_value["id"].asString().c_str(), sizeof(phone_list[k].id) );
		  strlcpy( phone_list[k].name, json_value["name"].asString().c_str(), sizeof(phone_list[k].name) );
		  strlcpy( phone_list[k].brand_name, json_value["brand_name"].asString().c_str(), sizeof(phone_list[k].brand_name) );
	       }
	       k++;
	    }
            UB_LOG_NOTICE( "get phone list from cache success" );
            //设置输出信息
            out.m_result_params()->set_phone_count( count_num );
            int j = 0;
            for( int i = start_pos; i < start_pos + (int)unit_num && i < count_num; ++i, ++j )
            {
                out.m_result_params()->set_phone_id( j, phone_list[i].id );
                out.m_result_params()->set_phone_name( j, phone_list[i].name );
            }

            return MCP_OK;
        }
        else
        {
            UB_LOG_NOTICE( "get phone count from cache success, but get phone list from cache failed" );
            //do nothing
        }

    }
    UB_LOG_NOTICE( "get phone info from cache failed" );
    MyclientRes data_res;
    char query_sql[1024];

    MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    //从连接池获取连接失败
    if ( NULL == database_connection )
    {
        UB_LOG_FATAL( "get database connection from database_pool  failed" );
        data_res.free();
        return MCP_NG;
    }

    //生成SQL语句
    sprintf( query_sql,
            "select mcp_phone_info.id, mcp_phone_info.name, mcp_phone_brand.name from mcp_phone_info, mcp_phone_brand where mcp_phone_info.brand_id = %s and mcp_phone_info.plat_id = %s and mcp_phone_info.brand_id = mcp_phone_brand.id ORDER BY  `mcp_phone_info`.`name` ASC ", 
            maker_id, plat_id );
    UB_LOG_NOTICE( "query_sql: %s", query_sql );

    count_num = database_connection->query(query_sql, &data_res, true);
    //从数据库查询数据失败
    if( count_num < 0 )
    {
        UB_LOG_FATAL("select phone name info from database failed");
        ret = database_pool->putBackConnection(database_connection, false);
        if( ret != 0)
        {
            UB_LOG_FATAL( "putback connection is failed" );
        }
        data_res.free();
        return MCP_NG;
    }

    phone_name_st phone_list[count_num];
    for( int i = 0; i < count_num; ++i )
    {
        if( count_index > 0 )
        {
            if( 0 == strcmp( phone_list[count_index-1].name, data_res[i][1] ) )
            {
                UB_LOG_NOTICE("phone name %s is same", phone_list[count_index-1].name);
                continue;
            }
        }
        strcpy( phone_list[count_index].id, data_res[i][0] );
        strcpy( phone_list[count_index].name, data_res[i][1] );
        strcpy( phone_list[count_index].brand_name, data_res[i][2] );
        count_index++;
       
    }
    count_num = count_index;

    //释放数据库资源
    data_res.free();
    ret = database_pool->putBackConnection(database_connection, false);
    if (ret != 0)
    {
        UB_LOG_FATAL( "putback connection failed" );
    }

    if( start_pos < 0 || start_pos > count_num - 1 )
    {
        UB_LOG_NOTICE( "start_pos is bigger than count_num" );
        out.m_result_params()->set_phone_count( count_num );
    }
    else
    {
        out.m_result_params()->set_phone_count( count_num );
        int j = 0;
        for( int i = start_pos; i < start_pos + (int)unit_num && i < count_num; ++i, ++j )
        {
            out.m_result_params()->set_phone_id( j, phone_list[i].id );
            out.m_result_params()->set_phone_name( j, phone_list[i].name );
        }
    }

    char countnum[16];
    sprintf(countnum, "%d", count_num);
    value = countnum;

    //将从数据库得到的对应厂商和平台的手机信息总数插入缓存
    ret = conn.session_insert(mainkey, subkey1, value, CACHE_EXPIRED_TIME); 
    if (0 != ret) 
    {   
        UB_LOG_NOTICE("insert brand_plat_phone_info count into session failed!");
    }   
    else    
    {   
        UB_LOG_NOTICE("insert brand_plat_phone_info count into session success");
    }

    value.clear();
    string id;
    string name;
    string brand_name;
    for( int i=0; i< count_num; i++)
    {
        id.clear(); id = phone_list[i].id;
	name.clear(); name = phone_list[i].name;
	brand_name.clear(); brand_name = phone_list[i].brand_name;
        value += "{\"id\":\"" + id + "\",\"name\":\"" + name + "\",\"brand_name\":\"" + brand_name + "\"}" ;
    }

     //将数据库中取得的对应厂商和平台的手机信息列表插入缓存
    ret = conn.session_insert(mainkey, subkey2, value, CACHE_EXPIRED_TIME);
    if (0 != ret)
    {
        UB_LOG_NOTICE("insert brand_plat_phone_info phone_name list into session failed!");
    }
    else
    {
        UB_LOG_NOTICE("insert brand_plat_phone_info phone_name list into session success");
    }

    UB_LOG_NOTICE( "get_phone_info_from_maker_plat finished" );
    return MCP_OK;

}
