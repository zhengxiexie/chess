#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"

#define CACHE_EXPIRED_TIME 60*1000*24*60

struct mcp_carriers_st
{
    char ip_addres[16];
    char carriers_name[8];
    char owner_place[8];
    mcp_carriers_st()
    {
        memset(ip_addres, 0x00, sizeof(ip_addres));
        memset( carriers_name, 0x00, sizeof(carriers_name) );
        memset( owner_place, 0x00, sizeof(owner_place) );
    }
};

struct carriers_name_st
{
    uint32_t carriers_name;
    uint32_t owner_place;
    carriers_name_st()
    {
        carriers_name = 0;
        owner_place = 0;
    }
};

int get_full_ip_address_info( const string ip_addres, string& carriers_name, string& owner_place )
{
    UB_LOG_NOTICE( "get_full_ip_address_info start" );

    MyclientRes data_res;
    char query_sql[1024];
    int err = 0;
    int count_num =0;
    int res = 0;

    memset(&data_res, 0x00, sizeof(data_res));
    memset(query_sql, 0x00, sizeof(query_sql));

    UB_LOG_NOTICE("fetchConnection() start");
    MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);

    //从连接池获取连接失败
    if ( NULL == database_connection )
    {
        UB_LOG_FATAL( "get database connection from database_pool  failed" );
        data_res.free();
        return MCP_NG;
    }

    sprintf(query_sql,
            "SELECT network,province_id FROM mcp_gateway_ip WHERE ip = \"%s\" ", ip_addres.c_str() );
    UB_LOG_NOTICE( "query_sql: %s", query_sql );
    count_num = database_connection->query(query_sql, &data_res, true);
    //从数据库查询数据失败
    if ( count_num <= 0 )
    {
        UB_LOG_NOTICE("select full ip address info from database failed");
        data_res.free();
        res = database_pool->putBackConnection(database_connection, false);
        if (res != 0)
        {
            UB_LOG_FATAL( "putback connection is failed" );
        }
        return MCP_NG;
    }
    UB_LOG_NOTICE("select full ip address info from database success");

    carriers_name = data_res[0][0];
    owner_place = data_res[0][1];

    data_res.free();
    res = database_pool->putBackConnection(database_connection, false);
    if (res != 0)
    {
        UB_LOG_FATAL( "putback connection is failed" );
    }
    UB_LOG_NOTICE( "get_full_ip_address_info end" );
    return MCP_OK;
}

int get_ip_sub_network_info( const string ip_addres, const int network_bits, string& carriers_name, string& owner_place )
{
    UB_LOG_NOTICE( "get_ip_sub_network_info start" );

    MyclientRes data_res;
    char query_sql[1024];
    int err = 0;
    int count_num = 0;
    int res = 0;

    memset(&data_res, 0x00, sizeof(data_res));
    memset(query_sql, 0x00, sizeof(query_sql));

    UB_LOG_NOTICE("fetchConnection() start");
    MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);

    //从连接池获取连接失败
    if ( NULL == database_connection )
    {
        UB_LOG_FATAL( "get database connection from database_pool  failed" );
        data_res.free();
        return MCP_NG;
    }

    string network;
    string network1;
    string::size_type pos;
    switch( network_bits )
    {
        case 24:
            pos = ip_addres.find_last_of( "." );
            break;
        case 16:
            pos = ip_addres.find_first_of( "." );
            //跳过第一个".",查找第二个"."
            pos = ip_addres.find( ".", pos+1 );
            break;
        case 8:
            pos = ip_addres.find_first_of( "." );
            break;
        default:
            UB_LOG_NOTICE( "Error network_bits,it must in(8, 16, 24)" );
            return MCP_NG;
            break;
    }
    //不加"."
    network = ip_addres.substr( 0, pos );
    //加"."
    network1 = ip_addres.substr( 0, pos+1 );
    UB_LOG_NOTICE( "search network:[%s]", network.c_str() );

    sprintf(query_sql,
            "SELECT network,province_id FROM mcp_gateway_ip WHERE ip = \"%s\" or ip = \"%s\" ",
                    network.c_str(), network1.c_str() );
    UB_LOG_NOTICE( "query_sql: %s", query_sql );

    count_num = database_connection->query(query_sql, &data_res, true);
    //从数据库查询数据失败
    if ( count_num <= 0 )
    {
        UB_LOG_NOTICE("select full ip address info from database failed");
        data_res.free();
        res = database_pool->putBackConnection(database_connection, false);
        if (res != 0)
        {
            UB_LOG_FATAL( "putback connection is failed" );
        }
        return MCP_NG;
    }
    UB_LOG_NOTICE("select full ip address info from database success");

    carriers_name = data_res[0][0];
    owner_place = data_res[0][1];

    data_res.free();
    res = database_pool->putBackConnection(database_connection, false);
    if (res != 0)
    {
        UB_LOG_FATAL( "putback connection is failed" );
    }
    UB_LOG_NOTICE( "get_ip_sub_network_info end" );
    return MCP_OK;
}

int get_carriers_info_from_database( const string ip_addres, uint32_t& carriers_name, uint32_t& owner_place )
{
    UB_LOG_NOTICE( "get_carriers_info_from_database start" );

    string carriers_name_str;
    string owner_place_str;

    int res = 0;

    res = get_full_ip_address_info( ip_addres, carriers_name_str, owner_place_str );
    if( res == MCP_OK )
    {
        carriers_name = atoi( carriers_name_str.c_str() );
        owner_place = atoi( owner_place_str.c_str() );
        return MCP_OK;
    }

    res = get_ip_sub_network_info( ip_addres, 24, carriers_name_str, owner_place_str );
    if( res == MCP_OK )
    {
        carriers_name = atoi( carriers_name_str.c_str() );
        owner_place = atoi( owner_place_str.c_str() );
        return MCP_OK;
    }

    res = get_ip_sub_network_info( ip_addres, 16, carriers_name_str, owner_place_str );
    if( res == MCP_OK )
    {
        carriers_name = atoi( carriers_name_str.c_str() );
        owner_place = atoi( owner_place_str.c_str() );
        return MCP_OK;
    }

    res = get_ip_sub_network_info( ip_addres, 8, carriers_name_str, owner_place_str );
    if( res == MCP_OK )
    {
        carriers_name = atoi( carriers_name_str.c_str() );
        owner_place = atoi( owner_place_str.c_str() );
        return MCP_OK;
    }
    

    //UB_LOG_FATAL( "get carriers_info failed" );
    UB_LOG_NOTICE( "get_carriers_info_from_database end" );
    return MCP_NG;
}


/*
int McpServlet::get_carriers_info_from_ip(const idl::mcp_get_carriers_info_from_ip_params& in, idl::mcp_get_carriers_info_from_ip_response& out)
{
    UB_LOG_NOTICE( "get_carriers_info_from_ip start" );
    string ip_addres( in.ip_addres() );

    mcp_session conn;

    int ret = 0;
    uint32_t carriers_name = 0;
    uint32_t owner_place = 0;

    string mainkey_name( "phone_ip_info" );
    string subkey_name = ip_addres;

    string carriers_name_place;

    ret = conn.session_search( mainkey_name, subkey_name, carriers_name_place );

    //命中缓存
    if( 0 == ret )
    {
        UB_LOG_NOTICE("query cache data success");
        int pos = carriers_name_place.find( '+' );
        carriers_name = atoi( carriers_name_place.substr(0, pos).c_str() );
        pos++;
        owner_place = atoi( carriers_name_place.substr(pos).c_str() );

        out.m_result_params()->set_carriers_name( carriers_name );
        out.m_result_params()->set_owner_place( owner_place );

        return MCP_OK;
    }
    //未命中缓存
    else
    {
        UB_LOG_NOTICE("query cache data failed");

        ret = get_carriers_info_from_database( ip_addres, carriers_name, owner_place );

        out.m_result_params()->set_carriers_name( carriers_name );
        out.m_result_params()->set_owner_place( owner_place );

        if( ret == MCP_OK )
        {
            char name[16];
            sprintf(name, "%d", carriers_name);
            string tmpname = name;
            char place[16];
            sprintf(place, "%d", owner_place);
            string tmpplace = place;
            carriers_name_place = tmpname + "+" + tmpplace;
            UB_LOG_WARNING("%s\n\n\n", carriers_name_place.c_str() );
            //将数据库中取得的数据插入缓存
            ret = conn.session_insert( mainkey_name, subkey_name, carriers_name_place, CACHE_EXPIRED_TIME );
            if (0 != ret)
                UB_LOG_NOTICE("insert ip carriers info into session failed!");
            else
                UB_LOG_NOTICE("insert ip carriers info into session success");
        }
    }
    UB_LOG_NOTICE( "get_carriers_info_from_ip finished" );
    return MCP_OK;
}
*/
