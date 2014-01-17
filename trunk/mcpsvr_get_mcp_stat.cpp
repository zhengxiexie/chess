/*
 * =====================================================================================
 *
 *       Filename:  mcpsvr_get_map_stat.cpp
 *
 *    Description:  get mcp MongoDB, MySQL and session connection status
 *
 *        Version:  1.0
 *        Created:  09/09/2011 15:38 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  RD. Yong li (ly), liyong04@baidu.com
 *        Company:  baidu, Beijing, China
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"
#include "mongo.h"
#include "mcpsvr_content_common.h"

enum conn_type
{
    MONGODB = 0,
    MYSQLDB   = 1,
    ZCACHE  = 2
};

/**
 * Check the MongoDB server connection status.
 *
 * @param NULL.
 *
 * @return MCP_OK or MCP_NG on failure.
 */
int32_t get_mongo_stat()
{
    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -1;
    }
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    return 0;
}

typedef struct
{
    char charset[128];
    char tag[128];
    char dbname[128];
    char username[128];
    char password[128];
    int32_t port;
    char ip[128];
    int32_t read_timeout;
    int32_t write_timeout;
    int32_t connect_timeout;
    int32_t min_connection;
    int32_t max_connection;
}mysql_conf;

/**
 * get MySQL server info from config.
 *
 * @param config_path the config file path.
 * @param config_file the config file name.
 * @param out conf  the structure of the MySQL server info that get from the config.
 *
 * @return MCP_OK or MCP_NG on failure.
 */
int get_mysql_conf( const char * config_path, const char * config_file, mysql_conf* conf )
{
    int i = 0;
    int res = 0;
    int err = 0;
    comcfg::Configure conf_loader;
    res = conf_loader.load(config_path, config_file);
    if (res != 0)
    {
        return -1;
    }
    char temp_name [MAX_CONF_NAME];
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_charset");
    snprintf(conf [i].charset, sizeof(conf [i].charset), "%s",
                    conf_loader [temp_name].to_cstr(&err));
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_tag");
    snprintf(conf->tag, sizeof(conf->tag), "%s",
                    conf_loader [temp_name].to_cstr(&err));
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_dbname");
    snprintf(conf->dbname, sizeof(conf->dbname), "%s",
                    conf_loader [temp_name].to_cstr(&err));
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_username");
    snprintf(conf->username, sizeof(conf->username), "%s",
                    conf_loader [temp_name].to_cstr(&err));
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_password");
    snprintf(conf->password, sizeof(conf->password), "%s",
                    conf_loader [temp_name].to_cstr(&err));
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_ip");
    snprintf(conf->ip, sizeof(conf->ip), "%s",
                    conf_loader [temp_name].to_cstr(&err));
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_port");
    conf->port = conf_loader [temp_name].to_int32(&err);
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_read_timeout");
    conf->read_timeout = conf_loader [temp_name].to_int32(&err);
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_write_timeout");
    conf->write_timeout = conf_loader [temp_name].to_int32(&err);
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_connect_timeout");
    conf->connect_timeout = conf_loader [temp_name].to_int32(&err);
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_min_connection");
    conf->min_connection = conf_loader [temp_name].to_int32(&err);
    if (err != 0)
    {
        return -1;
    }
    snprintf(temp_name, sizeof(temp_name), "%s%u%s", "server", i, "_max_connection");
    conf->max_connection = conf_loader [temp_name].to_int32(&err);
    if (err != 0)
    {
        return -1;
    }
    if (conf->max_connection <= 0 || conf->min_connection <= 0
                    || conf->connect_timeout <= 0 || conf->write_timeout <= 0
                    || conf->read_timeout <= 0 || conf->connect_timeout <= 0
                    || conf->max_connection < conf->min_connection)
    {
        return -1;
    }
    return 0;
}

/**
 * Check the MySQL server connection status.
 *
 * @param NULL.
 *
 * @return MCP_OK or MCP_NG on failure.
 */
int32_t get_mysql_stat()
{
    int res = 0;
    mysql_conf conf;
    res = get_mysql_conf( "./conf", "queryf.conf", &conf );

    char* ip = conf.ip;
    char* username = conf.username;
    char* password = conf.password;
    char* dbname = conf.dbname;
    int32_t port = conf.port;
    

    MYSQL mysql_conn;
    mysql_init(&mysql_conn);
//    if ( mysql_real_connect( &mysql_conn, "10.23.245.118", "root", "1q2w3e4r", "MCP", 3306, NULL, 0 ) == NULL )
    if ( mysql_real_connect( &mysql_conn, ip, username, password, dbname, port, NULL, 0 ) == NULL )
    {
        UB_LOG_FATAL( "Connect to MySQL server failed, [%s,%d]", __FILE__, __LINE__ );
        mysql_close(&mysql_conn);
        return -1;
    }
    mysql_close(&mysql_conn);
    return 0;
}

/**
 * Check the ZCache server connection status.
 *
 * @param NULL.
 *
 * @return MCP_OK or MCP_NG on failure.
 */
/*int32_t get_zcache_stat()
{
    int res = 0;

    ub_t * fw = ub_init("mcp", 0, NULL); //初始化框架
    if (fw == NULL) {       //返回0表示要退出
            return 0;
    }

    ub_conf_data_t *conf = ub_open_conf(fw);        //打开配置文件
    UBFW_ASSERT(conf != NULL, "open configure %s/%s error", fw->path, fw->file);
    char zcache_server_addrs[256];
    memset(zcache_server_addrs, 0x00, sizeof(zcache_server_addrs));

    res = ub_conf_getnstr( conf, "zcache_server_addr", zcache_server_addrs, sizeof(zcache_server_addrs), NULL,NULL);
    if ( 0 != res )
    {
        UB_LOG_FATAL("get zcache server addr config failed, ret is %d", res);
        return -1;
    }

    char zcache_product_name[65];
    memset(zcache_product_name, 0x00, sizeof(zcache_product_name));
    res = ub_conf_getnstr( conf, "zcache_product_name", zcache_product_name, sizeof(zcache_product_name), NULL,NULL);
    if ( 0 != res )
    {
        UB_LOG_FATAL("get zcache product name config failed, ret is %d", res);
        return -1;
    }

    char zcache_token[65];
    memset(zcache_token, 0x00, sizeof(zcache_token));
    res = ub_conf_getnstr( conf, "zcache_token", zcache_token, sizeof(zcache_token), NULL,NULL);
    if ( 0 != res )
    {
        UB_LOG_FATAL("get zcache token config failed, ret is %d", res);
        return -1;
    }

    zcache_handel = zcache_create(zcache_server_addrs, zcache_product_name, ZCACHEAPI_DEFAULT_FD_NUM,
                        ZCACHEAPI_DEFAULT_FD_TIMEOUT, 0);
    if( NULL == zcache_handel)
    {
            UB_LOG_FATAL("create api handle fail!");
            return -1;
    }
    UB_LOG_DEBUG("create zcache api handle ok!");

    zcache_destroy(zcache_handel);
    return 0;

}
*/
/**
 * get mcp database and cache server connection status.
 *
 * @param out out the array of sever connection status.
 *
 * @return MCP_OK.
 */
int McpServlet::get_mcp_stat(const idl::mcp_get_mcp_stat_params& in, idl::mcp_get_mcp_stat_response& out)
{
    (void)in;
    int32_t res = 0;
    res = get_mongo_stat();
    out.m_result_params()->set_stat( MONGODB, res );
    res = get_mysql_stat();
    out.m_result_params()->set_stat( MYSQLDB, res );
    //res = get_zcache_stat();
    out.m_result_params()->set_stat( ZCACHE, 0 );
    return MCP_OK;
}
