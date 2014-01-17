/*
 * =====================================================================================
 *
 *       Filename:  
 *
 *    Description:  some interface get game and theme info from MongoDB
 *
 *        Version:  1.0
 *        Created:  08/18/2011 19:50:42 
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
#include "tools.h"
#include "content_list.h"
#include "mcpsvr_content_common.h"
#include <time.h>

using namespace std;

/**
 * Connect to MongoDB server which is defined in the config.
 *
 * @param conn a mongo object.
 *
 * @return MCP_OK or MCP_NG on failure.
 */
int mcp_connect_mongo_hosts( mongo* &conn )
{
/*
    for( int i = 0; i < 2; ++i )
    {
        UB_LOG_NOTICE( "try to connect [%s:%s], [%s:%d}", MCP_MONGODB_HOST[i], MCP_MONGODB_PORT[i], __FILE__, __LINE__ );
        if( mongo_connect( conn, MCP_MONGODB_HOST[i], atoi(MCP_MONGODB_PORT[i]) ) == MONGO_OK )
        {
            UB_LOG_NOTICE( "connect [%s:%s] success, [%s:%d}", MCP_MONGODB_HOST[i], MCP_MONGODB_PORT[i], __FILE__, __LINE__ );
            return MCP_OK;
        }
    }
    UB_LOG_FATAL( "connect all mongo hosts failed, [%s:%d]", __FILE__, __LINE__ );
*/
    conn = mongo_pool->getConnection();
    if( conn == NULL )
    {
        UB_LOG_FATAL( "mongodb connnection pool get connect failed, [%s:%d]", __FILE__, __LINE__ );
        return -1;
    }
    return 0;
}

/**
 * Check the mongo object connection status, when failed, try to reconnection
 *
 * @param conn a mongo object.
 *
 * @return MCP_OK or MCP_NG on failure.
 */
int check_mongo_conn( mongo* conn )
{
    const int retry_times(3);

    if( mongo_check_connection(conn) == MONGO_OK )
    {
        return MCP_OK;
    }

    UB_LOG_NOTICE( "check mongo connnection failed, need reconnection" );
    
    for( int i = 0; i < retry_times; ++i )
    {
        UB_LOG_NOTICE( "reconnect MongoDB, retey_times=[%d] [%s:%d]", i+1, __FILE__, __LINE__ );
        if( mcp_connect_mongo_hosts( conn ) == MCP_OK )
        {
            return MCP_OK;
        }
    }
    
    UB_LOG_FATAL( "reconnect MongoDB failed, [%s:%d]", __FILE__, __LINE__ );
    return MCP_NG;
}

/**
 * Free the mongo cursor and bson object
 *
 * @param cursor a mongo cursor.
 * @param query a bson object.
 * @param fields a bson object.
 * @param bson_result a bson object.
 *
 * @return MCP_OK or MCP_NG on failure.
 */
void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
{
    mongo_cursor_destroy( cursor );
    if( query != NULL && query->data != NULL )
    {
        bson_destroy( query );
    }
    if( fields != NULL && fields->data != NULL )
    {
        bson_destroy( fields );
    }
    if( bson_result != NULL && bson_result->data != NULL )
    {
        //bson_destroy( const_cast<bson*>(bson_result) );
       bson_destroy( bson_result );
    }
}

/**
 * ¸ù¾ÝMongoDBµÄDBÃûºÍCollectionÃû»ñÈ¡namespace
 *
 * @param mongo_db_name DBÃû³Æ.
 * @param mongo_collection_name CollectionÃû³Æ.
 *
 * @return namespace.
 */
string get_mongo_namespace( string mongo_db_name, string mongo_collection_name )
{
    if( mongo_db_name.empty() || mongo_collection_name.empty() )
    {
        return string();
    }
    else
    {
        string ns( mongo_db_name + "." + mongo_collection_name );
        return ns;
    }
}

int get_class_name( mongo *conn, const string class_id, string& class_name )
{
    UB_LOG_NOTICE( "get_content_class_name start" );

    int res = 0;

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_CLASS_COL) );

    bson query, fields, bson_out;

    bson_init( &query );
    bson_append_string( &query, "id", class_id.c_str() );
    bson_finish( &query );

    bson_init( &fields );
    bson_append_int( &fields, "name", 1 );
    bson_finish( &fields );

    res = mongo_find_one( conn, ns.c_str(), &query, &fields, &bson_out );

    bson_destroy( &query );
    bson_destroy( &fields );

    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "query class_name where class_id(%s) from MongoDB %s failed", class_id.c_str(), ns.c_str() );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query class_name where class_id(%s) from MongoDB %s success", class_id.c_str(), ns.c_str() );

    bson_iterator bson_it;
    if( bson_find( &bson_it, &bson_out, "name" ) == BSON_STRING )
    {
        class_name = bson_iterator_string( &bson_it );
        UB_LOG_NOTICE( "class_name: %s", class_name.c_str() );
        UB_LOG_NOTICE( "get_content_class_name finished" );

        bson_destroy( &bson_out );
        return MCP_OK;

    }
    UB_LOG_FATAL( "class_name has wrong datatype class_id(%s)", class_id.c_str() );
    bson_destroy( &bson_out );
    return MCP_NG;

}

/*
int McpServlet::get_content_class_name(const idl::mcp_get_content_class_name_params& in,
                                             idl::mcp_get_content_class_name_response& out)
{
    UB_LOG_NOTICE( "get_content_class_name start" );
    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    const string class_id( in.class_id() );
    string class_name;

    int res(0);
    res = get_class_name( conn, class_id, class_name );
    if( res != MCP_OK )
    {
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        UB_LOG_FATAL( "get_content_class_name failed, class_id(%s)", class_id.c_str() );
        return MCP_OK;
    }
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    out.m_result_params()->set_class_name( class_name.c_str() );
    return MCP_OK;
}
*/

int get_phone_info_by_phone_id( mongo* conn, const string phone_id,
                                vector<string>& series_list, string& plat_id, string& res_id )
{
    UB_LOG_NOTICE( "get_phone_info_by_phone_id start" );

    string phone_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_PHONE_COL ) );

    bson query[1], fields[1], result[1];
    int res = 0;

    bson_init( query );
    bson_append_string( query, "id", phone_id.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_finish( fields );

    res = mongo_find_one( conn, phone_ns.c_str(), query, fields, result );

    if( res != MONGO_OK )
    {
        free_data( NULL, query, fields, NULL );
        UB_LOG_FATAL( "query phone info from %s failed, id=[%s], [%s:%d]", phone_ns.c_str(),
                       phone_id.c_str(), __FILE__, __LINE__ );
        return MCP_NG;
    }

    bson_iterator it[1];
    if( bson_find( it, result, "sid" ) == BSON_ARRAY )
    {
        bson_iterator sub[1];
        bson_iterator_subiterator( it, sub );
        string series_id;
        while( bson_iterator_next( sub ) )
        {
            series_id = bson_iterator_string( sub );
            series_list.push_back( series_id );
            UB_LOG_NOTICE( "get series_id:[%s] by phone_id:[%s], [%s:%d]", series_id.c_str(),
                            phone_id.c_str(), __FILE__, __LINE__ );
        }
    }

    if( bson_find( it, result, "plat_id" ) == BSON_STRING )
    {
        plat_id = bson_iterator_string( it );
        UB_LOG_NOTICE( "get plat_id:[%s] by phone_id:[%s], [%s:%d]", plat_id.c_str(),
                            phone_id.c_str(), __FILE__, __LINE__ );
    }

    if( bson_find( it, result, "res_id" ) == BSON_STRING )
    {
        res_id = bson_iterator_string( it );
        UB_LOG_NOTICE( "get res_id:[%s] by phone_id:[%s], [%s:%d]", res_id.c_str(),
                            phone_id.c_str(), __FILE__, __LINE__ );
    }

    free_data( NULL, query, fields, result );
    UB_LOG_NOTICE( "get_phone_info_by_phone_id end" );
    return MCP_OK;
}

int get_cdata_id_by_type( mongo* conn, const string content_id, const string info_type, 
                                       const string info_id, vector<string>& cdata_id_list )
{
    UB_LOG_NOTICE( "get_cdata_id_by_type start" );

    string format_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_FORMAT_COL ) );
    string cdata_id;
    int res = 0;
    mongo_cursor* cursor = NULL;

    bson query[1], fields[1];
    const bson* bson_result;

    char stype_id_list[1024];
    memset( stype_id_list, 0x00, sizeof( stype_id_list ) );
    char *tmp = NULL;
    char index[8];
    char *p_stype_id = stype_id_list;

    bson_init( query );
    bson_append_string( query, "stype", info_type.c_str() );
    bson_append_start_object( query, "stype_id" );
    bson_append_start_array( query, "$in" );
    strcpy( stype_id_list, info_id.c_str() );
    for( int j=0; (tmp=strsep(&p_stype_id,","))!=NULL; ++j )
    {
        memset( index, 0x00, sizeof( index ) );
        sprintf( index, "%d", j );
        bson_append_string( query, index, tmp );
        UB_LOG_DEBUG( "+++++ stype_id:[%s], [%s:%d] ++++++", tmp, __FILE__, __LINE__ );
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_append_string( query, "c_id", content_id.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "cdata_id", 1 );
    bson_finish( fields );

    UB_LOG_NOTICE( "c_id[%s], stype[%s], stype_id[%s]", content_id.c_str(), info_type.c_str(), info_id.c_str() );
 //   struct timeval s_tv;
//    gettimeofday( &s_tv, NULL );

    cursor = mongo_find( conn, format_ns.c_str(), query, fields, 0, 0, 0 );

    if( cursor != NULL )
    {
         while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
            bson_result = mongo_cursor_bson( cursor );
            bson_iterator bson_it[1];
            if( bson_find( bson_it, bson_result, "cdata_id" ) != BSON_STRING )
            {
                free_data( cursor, query, fields, NULL );
                UB_LOG_FATAL( "The cdata_id type is not string, [%s:%d]", __FILE__, __LINE__ );
                return MCP_NG;
            }
            cdata_id = bson_iterator_string( bson_it );
            UB_LOG_NOTICE( "get cdata_id:[%s] by stype(%s),stype_id(%s),c_id(%s), [%s:%d]", cdata_id.c_str(),
                                 info_type.c_str(), info_id.c_str(), content_id.c_str(), __FILE__, __LINE__ );
            cdata_id_list.push_back( cdata_id );
        }
    }

//    struct timeval e_tv;
//    gettimeofday( &e_tv, NULL );

    //UB_LOG_NOTICE( "+++++++start[%u:%u], end[%u:%u]+++++++", s_tv.tv_sec, s_tv.tv_usec, e_tv.tv_sec, e_tv.tv_usec );

    if( cursor == NULL || cdata_id_list.begin() == cdata_id_list.end() )
    {
        UB_LOG_FATAL( "get cdata_id failed by stype(%s),stype_id(%s),c_id(%s), [%s:%d]", 
                            info_type.c_str(), info_id.c_str(), content_id.c_str(), __FILE__, __LINE__ );
        if( 0 == strcmp( info_type.c_str(), "1" ) )
        {
            vector<string> series_list;
            string plat_id;
            string res_id;
            res = get_phone_info_by_phone_id( conn, info_id, series_list, plat_id, res_id );
            if( res != MCP_OK )
            {
                free_data( cursor, query, fields, NULL );
                UB_LOG_FATAL( "get_phone_info_by_phone_id failed, [%s:%d]", __FILE__, __LINE__ );
                return MCP_NG;
            }
            //¸ù¾Ý¿ÁÐÁ¿í²éÕÒ¿ÅäµÄÏÂÔØÐÅ¿ID
            vector<string>::iterator iter = series_list.begin();
            for( ; iter != series_list.end(); ++iter )
            {
                res = get_cdata_id_by_type( conn, content_id, "2", *iter, cdata_id_list );
                if( res == MCP_OK )
                {
                    free_data( cursor, query, fields, NULL );
                    UB_LOG_NOTICE( "get cdata_id success, c_id=[%s],stype=[2], stype_id=[%s],[%s:%d]",
                                    content_id.c_str(), (*iter).c_str(), __FILE__, __LINE__ );
                    return MCP_OK;
                }
            }
            //´Ó¿ÁÐ¿Åä¿°¿¬ ¸ù¾Ý¿¿¿ÅäÏÂÔØÐÅ¿ID
            res = get_cdata_id_by_type( conn, content_id, "3", plat_id, cdata_id_list );
            if( res == MCP_OK )
            {
                free_data( cursor, query, fields, NULL );
                UB_LOG_NOTICE( "get cdata_id success, c_id=[%s],stype=[3], stype_id=[%s],[%s:%d]",
                                    content_id.c_str(), plat_id.c_str(), __FILE__, __LINE__ );
                return MCP_OK;
            }
            //´Ó¿¿¿Åä¿°¿¬ ¸ù¾¿¿æÂÊ¿ÅäÏÂÔØÐÅ¿ID
            res = get_cdata_id_by_type( conn, content_id, "4", res_id, cdata_id_list );
            if( res == MCP_OK )
            {
                free_data( cursor, query, fields, NULL );
                UB_LOG_NOTICE( "get cdata_id success, c_id=[%s],stype=[4], stype_id=[%s],[%s:%d]",
                                    content_id.c_str(), res_id.c_str(), __FILE__, __LINE__ );
                return MCP_OK;
            }
            free_data( cursor, query, fields, NULL );
            UB_LOG_FATAL( "Can not match cdata_id by all phone_info, [%s:%d]", __FILE__, __LINE__ );
            return MCP_NG;
        }
        else
        {
            free_data( cursor, query, fields, NULL );
            UB_LOG_FATAL( "query cdata_id from %s failed", format_ns.c_str() );
            return MCP_NG;
        }
    }

    UB_LOG_NOTICE( "query cdata_id from %s success", format_ns.c_str() );

    free_data( cursor, query, fields, NULL );
    UB_LOG_NOTICE( "get_cdata_id_by_type end" );
    return MCP_OK;
}

/*
int McpServlet::get_download_info(const idl::mcp_get_download_info_params& in,
                                        idl::mcp_get_download_info_response& out)
{
    UB_LOG_NOTICE( "get_download_info start" );

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        //mongo_pool->releaseConnection(conn);
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    string content_id( in.content_id() );
    string info_id( in.info_id() );
    uint32_t info_type = in.info_type();

    int res(0);

    if( info_id.empty() )
    {
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        UB_LOG_FATAL( "info_id must not be NULL" );
        out.m_result_params()->set_download_info( 0, 0, "" );
        out.m_result_params()->set_download_info( 0, 1, "" );
        out.m_result_params()->set_download_info( 0, 2, "" );
        out.m_result_params()->set_download_info( 0, 3, "" );
        out.m_result_params()->set_download_info( 0, 4, "" );
        out.m_result_params()->set_download_info( 0, 5, "" );
        out.m_result_params()->set_download_info( 0, 6, "" );
        out.m_result_params()->set_download_info( 0, 7, "" );
        return MCP_OK;
    }

    string data_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DATA_COL) );

    bson query[1], fields[1];
    const bson* result;
    const char* key;
    mongo_cursor* cursor = NULL;

    char c_info_type[4];
    sprintf( c_info_type, "%d", info_type );

    int i( 0 );
    vector<string> cdata_id_list;
    res = get_cdata_id_by_type( conn, content_id, c_info_type, info_id, cdata_id_list );
    if( res != MCP_OK )
    {
        UB_LOG_NOTICE( "get cdata_id_list failed, c_id=[%s], info_type=[%s], info_id=[%s], [%s:%d]",
                        content_id.c_str(), c_info_type, info_id.c_str(), __FILE__, __LINE__ );
        out.m_result_params()->set_download_info( 0, 0, "" );
        out.m_result_params()->set_download_info( 0, 1, "" );
        out.m_result_params()->set_download_info( 0, 2, "" );
        out.m_result_params()->set_download_info( 0, 3, "" );
        out.m_result_params()->set_download_info( 0, 4, "" );
        out.m_result_params()->set_download_info( 0, 5, "" );
        out.m_result_params()->set_download_info( 0, 6, "" );
        out.m_result_params()->set_download_info( 0, 7, "" );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }
    string cdata_id;
    char index[8];
    vector<string>::iterator iter = cdata_id_list.begin();
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_start_object( query, "id" );
    bson_append_start_array( query, "$in" );
    for( int j = 0; iter != cdata_id_list.end(); ++iter, ++j )
    {
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j );
        bson_append_string( query, index, iter->c_str() );
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "path_url", 1);
    bson_append_int( fields, "path_type", 1 );
    bson_append_int( fields, "media_format_id", 1);
    bson_append_int( fields, "size", 1 );
    bson_append_int( fields, "mtype", 1 );
    bson_append_int( fields, "charge_typeid", 1 );
    bson_append_int( fields, "value", 1 );
    bson_finish( fields );

    cursor = mongo_find( conn, data_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    i = 0;
    int tmp_int;
    char tmp_char[32];
    string tmp_id, tmp_path_url, tmp_path_type, tmp_media, tmp_size, tmp_mtype, tmp_charge_type, tmp_value;
    UB_LOG_NOTICE( "mongo_cursor_next is start " );
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        UB_LOG_NOTICE( "mongo_cursor_next is OK " );
        tmp_id.clear();
        tmp_path_url.clear();
        tmp_path_type.clear();
        tmp_media.clear();
        tmp_size.clear();
        tmp_mtype.clear();
        tmp_charge_type.clear();
        tmp_value.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            UB_LOG_NOTICE( "query down_load info key is %s ", key );
            if( 0 == strcmp(key,"id"))
            {
                tmp_id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"path_url"))
            {
                tmp_path_url = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"path_type"))
            {
                tmp_path_type = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"media_format_id"))
            {
                tmp_media = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"size"))
            {
//                tmp_int = bson_iterator_int( bson_it );
                memset( tmp_char, 0x00, sizeof(tmp_char) );
                sprintf( tmp_char, "%d", bson_iterator_int( it) );
                tmp_size = tmp_char;
                UB_LOG_NOTICE( "query down_load info size is %d ", bson_iterator_int( it) );

            }
            else if( 0 == strcmp(key,"mtype"))
            {
                tmp_mtype = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"charge_typeid"))
            {
                tmp_charge_type = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"value"))
            {
                tmp_int = bson_iterator_int( it );
                memset( tmp_char, 0x00, sizeof(tmp_char) );
                sprintf( tmp_char, "%d", tmp_int );
                tmp_value = tmp_char;
            }
            else
            {
                continue;
            }
        }
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_id.c_str(), __FILE__, __LINE__ );
        out.m_result_params()->set_download_info( i, 0, tmp_id.c_str() );
        out.m_result_params()->set_download_info( i, 1, tmp_path_url.c_str() );
        out.m_result_params()->set_download_info( i, 2, tmp_path_type.c_str() );
        out.m_result_params()->set_download_info( i, 3, tmp_media.c_str() );
        out.m_result_params()->set_download_info( i, 4, tmp_size.c_str() );
        out.m_result_params()->set_download_info( i, 5, tmp_mtype.c_str() );
        out.m_result_params()->set_download_info( i, 6, tmp_charge_type.c_str() );
        out.m_result_params()->set_download_info( i, 7, tmp_value.c_str() );
        ++i;
    }

//    while( iter != cdata_id_list.end() )
//    {
//        cdata_id = *iter;
//
//        bson_init( query );
//        bson_append_string( query, "id", cdata_id.c_str() );
//        bson_finish( query );
//
//        bson_init( fields );
//        bson_append_int( fields, "path_url", 1);
//        bson_append_int( fields, "path_type", 1 );
//        bson_append_int( fields, "media_format_id", 1);
//        bson_append_int( fields, "size", 1 );
//        bson_append_int( fields, "mtype", 1 );
//      bson_append_int( fields, "charge_typeid", 1 );
//        bson_append_int( fields, "value", 1 );
//        bson_finish( fields );
//
//        res = mongo_find_one( conn, data_ns.c_str(), query, fields, result );
//        if( res != MONGO_OK )
//        {
//            //mongo_destroy( conn );
//            mongo_pool->releaseConnection(conn);
//            free_data( NULL, query, fields, NULL );
//            UB_LOG_FATAL( "query download info from %s by cdata_id[%s] failed", data_ns.c_str(), cdata_id.c_str() );
//            out.m_result_params()->set_download_info( 0, 0, "" );
//            out.m_result_params()->set_download_info( 0, 1, "" );
//            out.m_result_params()->set_download_info( 0, 2, "" );
//            out.m_result_params()->set_download_info( 0, 3, "" );
//            out.m_result_params()->set_download_info( 0, 4, "" );
//            out.m_result_params()->set_download_info( 0, 5, "" );
//            out.m_result_params()->set_download_info( 0, 6, "" );
//            out.m_result_params()->set_download_info( 0, 7, "" );
//            return MCP_OK;
//        }
//        UB_LOG_NOTICE( "query down_load info from %s by cdata_id[%s] success", data_ns.c_str(), cdata_id.c_str() );
//
//        out.m_result_params()->set_download_info( i, 0, cdata_id.c_str() );
//
//        bson_iterator_init( bson_it, result );
//        int cnt(1);
//        bson_iterator_next( bson_it );
//        int int_tmp(0);
//        string str_tmp;
//        char char_tmp[32];
//        while( bson_iterator_next( bson_it ) && cnt < 8 )
//        {
//            switch( bson_iterator_type( bson_it ) )
//            {
//                case BSON_INT:
//                    int_tmp = bson_iterator_int( bson_it );
//                    sprintf( char_tmp, "%d", int_tmp );
//                    out.m_result_params()->set_download_info( i, cnt, char_tmp );
//                    break;
//                case BSON_STRING:
//                    str_tmp = bson_iterator_string( bson_it );
//                    out.m_result_params()->set_download_info( i, cnt, str_tmp.c_str() );
//                    break;
//               default:
//                    UB_LOG_FATAL( "Error datatype" );
//                    break;
//            }
//            ++cnt;
//        }
//        free_data( NULL, query, fields, result );
//        ++i;
//        ++iter;
//    }

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    free_data( cursor, query, fields, NULL );
    UB_LOG_NOTICE( "get_download_info finished" );
    return MCP_OK;

}
*/

/*s.10/18/2011 liyong */
int get_content_basic_info( mongo *conn, const string content_type_id, const string id, string& name, string& class_id,
                                         string& class_name, string& game_code,  string& image_url )
/*e.10/18/2011 liyong */
{
    UB_LOG_NOTICE( "get_content_basic_info start" );
    int res(0);

    string game_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL ) );
/*s.10/18/2011 liyong */
    string theme_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL ) );
/*e.10/18/2011 liyong */
    string preview_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_PREVIEW_COL ) );

    string image_type;
    map<string, string> m_class_id_name;
    get_content_class_id_name_map( conn, m_class_id_name );
   
    bson query[1], fields[1], bson_result[1];
    
    bson_init( query );
    bson_append_string( query, "id", id.c_str() );
    bson_append_string( query, "content_type", content_type_id.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "name", 1 );
    bson_append_int( fields, "class_id", 1 );
    bson_append_int( fields, "game_code", 1 );
    bson_append_int( fields, "img_info", 1 );
    bson_finish( fields );

/*s.10/18/2011 liyong */
    if( content_type_id == MCP_CONTENT_GAME || content_type_id == MCP_ANDROID_GAME )
    {
        res = mongo_find_one( conn, game_ns.c_str(), query, fields, bson_result );
        if( res != MONGO_OK )
        {
            free_data( NULL, query, fields, NULL );
            UB_LOG_FATAL( "get content basic info from %s failed, c_id=[%s]", game_ns.c_str(), id.c_str() );
            return MCP_NG;
        }
    }
    else if( content_type_id == MCP_CONTENT_THEME )
    {
        res = mongo_find_one( conn, theme_ns.c_str(), query, fields, bson_result );
        if( res != MONGO_OK )
        {
            free_data( NULL, query, fields, NULL );
            UB_LOG_FATAL( "get content basic info from %s failed, c_id=[%s]", theme_ns.c_str(), id.c_str() );
            return MCP_NG;
        }
    }
    else
    {
        free_data( NULL, query, fields, NULL );
        UB_LOG_FATAL( "Unknown conent_type_id" );
        return MCP_NG;
    }
/*e.10/18/2011 liyong */

    bson_iterator bson_it[1];
    bson_iterator_init( bson_it, bson_result );
    
/*s.10/19/2011 liyong */

    const char* key;
    bson_iterator sub[1];
    string tmp_class_id, tmp_class_name;
    while( bson_iterator_next( bson_it ) )
    {
        key = bson_iterator_key( bson_it );
        if( strcmp( key, "name" ) == 0 )
        {
            name = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "class_id" ) == 0 )
        {
            //µ¥Ò»class_idµÄÇé¿ö
            if( bson_iterator_type( bson_it ) == BSON_STRING )
            {
                class_id = bson_iterator_string( bson_it );

                res = get_class_name( conn, class_id, class_name );
                if( res != MCP_OK )
                {
                    class_name = "";
                    UB_LOG_FATAL( "get content class name by class_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
                }
            }
            //¶ÔÓ¦¶à¸öclass_idµÄÇé¿ö
            else if( bson_iterator_type( bson_it ) == BSON_ARRAY )
            {
                bson_iterator_subiterator( bson_it, sub );
                while( bson_iterator_next( sub ) )
                {
                    tmp_class_id = bson_iterator_string( bson_it );
                    res = get_class_name( conn, tmp_class_id, tmp_class_name );
                    if( res != MCP_OK )
                    {
                        tmp_class_name = "";
                        UB_LOG_FATAL( "get content class name by class_id[%s] failed, [%s:%d]",
                                                             tmp_class_id.c_str(), __FILE__, __LINE__ );
                    }
                    if( !class_id.empty() )
                    {
                        class_id += ",";
                        class_name += ",";
                    }
                    class_id += tmp_class_id;
                    class_name += tmp_class_name;
                }
            }
        }
        else if( strcmp( key, "game_code" ) == 0 )
        {
            game_code = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "img_info" ) == 0 )
        {
            if( bson_iterator_type( bson_it ) == BSON_ARRAY )
            {
                bson_iterator_subiterator( bson_it, sub );
                bson_iterator subsub[1];
                while( bson_iterator_next( sub ) )
                {
                    bson_iterator_subiterator( sub, subsub );
                    image_type.clear();
                    image_url.clear();
                    while( bson_iterator_next(subsub) )
                    {
                        if( strcmp( bson_iterator_key(subsub), "img_type" ) == 0 )
                        {
                            image_type = bson_iterator_string( subsub );
                        }
                        else if( strcmp( bson_iterator_key(subsub), "img_url" ) == 0 )
                        {
                            image_url = bson_iterator_string( subsub );
                        }
                    }
                    if( image_type.compare("3") == 0 )
                    {
                        break;
                    }
                    else
                    {
                        image_type.clear();
                        image_url.clear();
                    }
                }
            }
            
        }
        else
        {
            UB_LOG_FATAL( "Unknown Mongo key[%s]", key );
        }

    }
/*
    if( content_type_id == MCP_CONTENT_THEME )
    {
        res = get_theme_class_info( conn, id, m_class_id_name, class_id, class_name );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get_theme_class_info failed, [%s:%d]", __FILE__, __LINE__ );
        }
    }
    bson_iterator_next( bson_it );
    bson_iterator_next( bson_it );
    name = bson_iterator_string( bson_it );
    bson_iterator_next( bson_it );
    class_id = bson_iterator_string( bson_it );
    if( content_type_id == MCP_CONTENT_GAME )
    {
        bson_iterator_next( bson_it );
        game_code = bson_iterator_string( bson_it );
    }
    else
    {
        game_code = "";
    }
*/
/*e.10/19/2011 liyong */

/*
    res = get_class_name( conn, class_id, class_name );
    if( res != MCP_OK )
    {
        class_name = "";
        UB_LOG_FATAL( "get content class name by class_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
        free_data( NULL, query, fields, bson_result );
        return MCP_OK;
    }
*/

    free_data( NULL, query, fields, bson_result );
    UB_LOG_NOTICE( "get_content_basic_info finish" );
    return MCP_OK;
    
}

int get_download_basic_info( mongo *conn, const string c_id, const int stype, const string stype_id,
                             const string default_charge_typeid, map<string, string>& m_charge_id_name,                             
                             string& cdata_id, string& path_url, string& charge_typeid, string& charge_typename )
{
    string format_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_FORMAT_COL ) );
    string data_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DATA_COL ) );

    bson query[1], fields[1], result[1];
    int res( 0 );

    cdata_id = path_url = charge_typeid = charge_typename =  "";

    char c_stype[4];
    sprintf( c_stype, "%d", stype );


    int i( 0 );
    vector<string> cdata_id_list;
    res = get_cdata_id_by_type( conn, c_id, c_stype, stype_id, cdata_id_list );
    if( res != MCP_OK )
    {
        UB_LOG_NOTICE( "get cdata_id_list failed, c_id=[%s], info_type=[%s], info_id=[%s], [%s:%d]",
                        c_id.c_str(), c_stype, stype_id.c_str(), __FILE__, __LINE__ );
       charge_typeid =  default_charge_typeid;
       charge_typename = m_charge_id_name[charge_typeid];
       return MCP_NG;
    }
    bson_iterator bson_it[1];
    vector<string>::iterator iter = cdata_id_list.begin();
    string temp_cdata_id, temp_path_url, temp_charge_typeid, temp_charge_typename;
    while( iter != cdata_id_list.end() )
    {

        if( i != 0 )
        {
            cdata_id += ",";
        }
        temp_cdata_id = *iter;
        cdata_id += temp_cdata_id;
        UB_LOG_NOTICE( "cdata_id:[%s], temp_cdata_id:[%s]", cdata_id.c_str(), temp_cdata_id.c_str() );

        bson_init( query );
        bson_append_string( query, "id", temp_cdata_id.c_str() );
        bson_finish( query );

        bson_init( fields );
        bson_append_int( fields, "path_url", 1 );
        bson_append_int( fields, "charge_typeid", 1 );
        bson_finish( fields );

        res = mongo_find_one( conn, data_ns.c_str(), query, fields, result );
        if( res != MONGO_OK )
        {
            UB_LOG_FATAL( "query download info from %s by cdata_id[%s], failed", data_ns.c_str(), temp_cdata_id.c_str() );
            free_data( NULL, query, fields, NULL );
            return MCP_NG;
        }
        UB_LOG_NOTICE( "query down_load info from %s by cdata_id[%s] success", data_ns.c_str(), temp_cdata_id.c_str() );

        bson_iterator_init( bson_it, result );
        bson_iterator_next( bson_it );
        bson_iterator_next( bson_it );
        if( i != 0 )
        {
            path_url += ",";
            charge_typeid += ",";
            charge_typename += ",";
        }
        temp_path_url = bson_iterator_string( bson_it );
        path_url += temp_path_url;
        bson_iterator_next( bson_it );
        temp_charge_typeid = bson_iterator_string( bson_it );
        if( temp_charge_typeid == "" )
        {
            temp_charge_typeid = default_charge_typeid;
            //temp_charge_typename = default_charge_typename;
        }
/*
        else
        {
            
            res = get_charge_type_name_by_id( conn, temp_charge_typeid, temp_charge_typename );
            if( res != MCP_OK )
            {
                UB_LOG_FATAL( "getr charge_type_name by id[%s] failed, [%s:%d]", temp_charge_typeid.c_str(), __FILE__, __LINE__ );
                temp_charge_typename = "";
            }
        }
*/
        UB_LOG_NOTICE( "tmp_charge_id[%s], default_charge_id[%s]", temp_charge_typeid.c_str(), default_charge_typeid.c_str() );
        temp_charge_typename = m_charge_id_name[temp_charge_typeid];
        charge_typeid += temp_charge_typeid;
        charge_typename += temp_charge_typename;
        
        free_data( NULL, query, fields, result );
        ++i;
        ++iter;
    }
    free_data( NULL, NULL, NULL, NULL );
    UB_LOG_NOTICE( "get_download_info finished" );
    return MCP_OK;

}



void set_content_list_basic_result( idl::mcp_get_content_list_response& out, const int cnt, const string content_id, 
         const string content_name, const string class_id, const string class_name, const string game_code, const string image_url )
{
    out.m_result_params()->set_content_list( cnt, 0, content_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 1, content_name.c_str() );
    out.m_result_params()->set_content_list( cnt, 2, class_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 3, class_name.c_str() );
    out.m_result_params()->set_content_list( cnt, 4, game_code.c_str() );
    out.m_result_params()->set_content_list( cnt, 5, image_url.c_str() );
}

void set_content_list_download_result( idl::mcp_get_content_list_response& out, const int cnt, const string download_id,
                                          const string download_path, const string charge_id )
{
    out.m_result_params()->set_content_list( cnt, 6, download_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 7, download_path.c_str() );
    out.m_result_params()->set_content_list( cnt, 8, charge_id.c_str() );
}

enum priority_type
{
    PRIORITY_DAY = 1,
    PRIORITY_WEEK,
    PRIORITY_MONTH,
    PRIORITY_YEAR,
    PRIORITY_STARS,
    PRIORITY_LOG_DATE,
    PRIORITY_SIZE_DESC,
    PRIORITY_SIZE_UPON = 8
};

/*
int McpServlet::get_content_list(const idl::mcp_get_content_list_params& in,
                                       idl::mcp_get_content_list_response& out)
{
    UB_LOG_NOTICE( "get_content_list start" );

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );

    uint32_t priority = in.priority();
    string info_id( in.info_id() );
    uint32_t info_type = in.info_type();
    uint32_t unit_num = in.unit_num();
    uint32_t unit_times = in.unit_times();

    if( unit_num == 0 || unit_times == 0 )
    {
        UB_LOG_FATAL( "The unit_num or unit_times is invalid " );
        //mongo_destroy( conn ); 
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }

    uint32_t start_pos = unit_num * ((int)unit_times - 1 );

    int res(0);

    string rank_col( "mcp_content_rank" );
    string game_col( "mcp_game_content" );
    string data_col( "mcp_content_data" );

    string rank_ns( get_mongo_namespace( MCP_MONGODB_NAME, rank_col ) );
    string game_ns( get_mongo_namespace( MCP_MONGODB_NAME, game_col ) );
    string data_ns( get_mongo_namespace( MCP_MONGODB_NAME, data_col ) );

    string c_id, content_name, class_id, class_name, game_code, image_url;
    string download_id, download_path, charge_id, charge_name;
    string default_charge_typeid, default_charge_typename;

    bson query[1], fields[1];
    const bson *bson_result;
    mongo_cursor *cursor = NULL;

    bson_iterator bson_it[1];

    if( PRIORITY_DAY <= priority && priority <= PRIORITY_YEAR )
    {
        char c_priority[8];
        sprintf( c_priority, "%d", priority );
        bson_init( query );
        bson_append_string( query, "ptype_id", c_priority );
        bson_finish( query );

        uint32_t count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_RANK_COL, query );
        out.m_result_params()->set_count( uint32_t( count) );
        if( count == 0 || start_pos >= count )
        {
            UB_LOG_FATAL( "count content info from [%s] faied", rank_ns.c_str() );
            UB_LOG_FATAL( "count is 0 or start_pos is greater than count, [count=%d], [ start_pos=%d]", count, start_pos );
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            free_data( cursor, query, fields, NULL );
            return MCP_OK;
        }
        UB_LOG_NOTICE( "count rank from content rank success, count=[%d], priority=[%d]", count, priority );

        bson_destroy( query );

        bson_init( query );
        bson_append_start_object( query, "$query" );
        bson_append_string( query, "ptype_id", c_priority );
        bson_append_finish_object( query );
        bson_append_start_object( query, "$orderby" );
        bson_append_int( query, "priority", 1 );
        bson_append_finish_object( query );
        bson_finish( query );

        bson_init( fields );
        bson_append_int( fields, "c_id", 1 );
        bson_finish( fields );

        cursor = mongo_find( conn, rank_ns.c_str(), query, fields, unit_num, start_pos, 0 );

        if( cursor == NULL )
        {
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            UB_LOG_FATAL( "query content info from [%s] by ptype_id[%s] faied", rank_ns.c_str(), c_priority );
            free_data( cursor, query, fields, NULL );
            return MCP_OK;
        }

        int i( 0 );
        while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
            bson_result = mongo_cursor_bson( cursor );

            if( bson_find( bson_it, bson_result, "c_id" ) == BSON_STRING )
            {
                c_id = bson_iterator_string( bson_it );

//s.10/18/2011 liyong 
                //res = get_content_basic_info( conn, c_id, content_name, class_id, class_name, game_code, image_url );
                res = get_content_basic_info( conn, MCP_CONTENT_GAME, c_id, content_name, class_id, class_name, game_code, image_url );
//e.10/18/2011 liyong 
                if( res != MCP_OK )
                {
                    //mongo_destroy( conn );
                    mongo_pool->releaseConnection(conn);
                    UB_LOG_FATAL( "get content basic info failed, c_id=[%s], [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
                    free_data( cursor, query, fields, NULL );
                    return MCP_OK;
                }
                set_content_list_basic_result( out, i, c_id, content_name, class_id, class_name, game_code, image_url );
                if( !info_id.empty() )
                {
                    res = get_download_basic_info( conn, c_id, info_type, info_id, default_charge_typeid, m_charge_id_name,
                                                   download_id, download_path, charge_id, charge_name );
                    if( res != MCP_OK )
                    {
                        UB_LOG_FATAL( "get download basic info failed, c_id=[%s], info_type=[%d], info_id=[%s], [%s:%d]",
                                       c_id.c_str(), info_type, info_id.c_str(), __FILE__, __LINE__ );
                        //download_id = download_path = charge_id = "";
                    }
                    set_content_list_download_result( out, i, download_id, download_path, charge_id );
                }
            }
            else
            {
                //mongo_destroy( conn );
                mongo_pool->releaseConnection(conn);
                free_data( cursor, query, fields, NULL );
                return MCP_OK;
            }
            ++i;
        }
    }
    else if( priority == PRIORITY_STARS || priority == PRIORITY_LOG_DATE )
    {
        bson_init( query );
        bson_finish( query );

        bson_init( fields );
        bson_append_int( fields, "id", 1 );
        bson_finish( fields );

        uint32_t count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_GAME_COL, query );
        out.m_result_params()->set_count( uint32_t( count) );
        if( count == 0 || start_pos >= count )
        {
            UB_LOG_FATAL( "count content info from [%s] faied", game_ns.c_str() );
            UB_LOG_FATAL( "count is 0 or start_pos is greater than count, [count=%d], [ start_pos=%d]", count, start_pos );
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            free_data( cursor, query, fields, NULL );
            return MCP_OK;
        }

        bson_destroy( query );

        bson_init( query );
        bson_append_start_object( query, "$query" );
        bson_append_finish_object( query );

        bson_append_start_object( query, "$orderby" );
        if( priority == PRIORITY_STARS )
        {
            bson_append_int( query, "star", -1 );
        }
        else
        {
            bson_append_int( query, "logdate", -1 );
        }
        bson_append_finish_object( query );
        bson_finish( query );

        cursor = mongo_find( conn, game_ns.c_str(), query, fields, unit_num, start_pos, 0 );

        if( cursor == NULL )
        {
            UB_LOG_FATAL( "query content info from [%s] faied", game_ns.c_str() );
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            free_data( cursor, query, fields, NULL );
            return MCP_OK;
        }

        int i( 0 );
        while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
            bson_result = mongo_cursor_bson( cursor );

            if( bson_find( bson_it, bson_result, "id" ) == BSON_STRING )
            {
                c_id = bson_iterator_string( bson_it );
//s.10/18/2011 liyong 
                //res = get_content_basic_info( conn, c_id, content_name, class_id, class_name, game_code, image_url );
                res = get_content_basic_info( conn, MCP_CONTENT_GAME, c_id, content_name, class_id, class_name, game_code, image_url );
//e.10/18/2011 liyong 
                if( res != MCP_OK )
                {
                    UB_LOG_FATAL( "get content basic info failed, c_id=[%s], [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
                    //mongo_destroy( conn );
                    mongo_pool->releaseConnection(conn);
                    free_data( cursor, query, fields, NULL );
                    return MCP_OK;
                }
                set_content_list_basic_result( out, i, c_id, content_name, class_id, class_name, game_code, image_url );
                if( !info_id.empty() )
                {
                    res = get_download_basic_info( conn, c_id, info_type, info_id, default_charge_typeid, m_charge_id_name,
                                                               download_id, download_path, charge_id, charge_name );
                    if( res != MCP_OK )
                    {
                        UB_LOG_NOTICE( "get download basic info failed, c_id=[%s], info_type=[%d], info_id=[%s], [%s:%d]",
                                       c_id.c_str(), info_type, info_id.c_str(), __FILE__, __LINE__ );
                        download_id = download_path = charge_id = "";
                    }
                    set_content_list_download_result( out, i, download_id, download_path, charge_id );
                }
            }
            else
            {
                //mongo_destroy( conn );
                mongo_pool->releaseConnection(conn);
                free_data( cursor, query, fields, NULL );
                return MCP_OK;
            }
            ++i;
        }
    }
    else if( priority == PRIORITY_SIZE_DESC || priority == PRIORITY_SIZE_UPON )
    {

        if( info_id.empty() )
        {
            UB_LOG_FATAL( "Have not phone type, can not match download info, [%s:%d]", __FILE__, __LINE__ );
        }

        bson_init( query );
        bson_finish( query );

        uint32_t count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_DATA_COL, query );
        out.m_result_params()->set_count( uint32_t( count) );
        if( count == 0 || start_pos >= count )
        {
            UB_LOG_FATAL( "count content info from [%s] faied", data_ns.c_str() );
            UB_LOG_FATAL( "count is 0 or start_pos is greater than count, [count=%d], [ start_pos=%d]", count, start_pos );
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            free_data( cursor, query, fields, NULL );
            return MCP_OK;
        }

        bson_destroy( query );

        bson_init( query );
        bson_append_start_object( query, "$query" );
        bson_append_finish_object( query );
        bson_append_start_object( query, "$orderby" );
        if( priority == PRIORITY_SIZE_DESC )
        {
            bson_append_int( query, "size", -1 );
        }
        else
        {
            bson_append_int( query, "size", 1 );
        }
        bson_append_finish_object( query );
        bson_finish( query );

        bson_init( fields );
        bson_append_int( fields, "id", 1 );
        bson_append_int( fields, "path_url", 1 );
        bson_append_int( fields, "charge_typeid", 1 );
        bson_append_int( fields, "c_id", 1);
        bson_finish( fields );

        cursor = mongo_find( conn, data_ns.c_str(), query, fields, unit_num, start_pos, 0 );

        if( cursor == NULL )
        {
            UB_LOG_FATAL( "query content info from [%s] faied", data_ns.c_str() );
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            free_data( cursor, query, fields, NULL );
            return MCP_OK;
        }

        int i( 0 );
        while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
            bson_iterator_init( bson_it, mongo_cursor_bson( cursor ) );

            bson_iterator_next( bson_it );
            bson_iterator_next( bson_it );
            download_id = bson_iterator_string( bson_it );
            bson_iterator_next( bson_it );
            download_path = bson_iterator_string( bson_it );
            bson_iterator_next( bson_it );
            charge_id = bson_iterator_string( bson_it );
            bson_iterator_next( bson_it );
            c_id = bson_iterator_string( bson_it );
//s.10/18/2011 liyong 
            //res = get_content_basic_info( conn, c_id, content_name, class_id, class_name, game_code, image_url );
            res = get_content_basic_info( conn, MCP_CONTENT_GAME, c_id, content_name, class_id, class_name, game_code, image_url );
//e.10/18/2011 liyong
            if( res != MCP_OK )
            {
                UB_LOG_FATAL( "get content basic info failed, c_id=[%s], [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
                //mongo_destroy( conn );
                mongo_pool->releaseConnection(conn);
                free_data( cursor, query, fields, NULL );
                return MCP_OK;
            }
            set_content_list_basic_result( out, i, c_id, content_name, class_id, class_name, game_code, image_url );
            set_content_list_download_result( out, i, download_id, download_path, charge_id );
            ++i;
        }
    }
    else
    {
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        free_data( NULL, query, fields, NULL );
        return MCP_OK;
    }

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    free_data( cursor, query, fields, NULL );
    UB_LOG_NOTICE( "get_content_list finished, [%s:%d]", __FILE__, __LINE__ );
    return MCP_OK;
}
*/

/*
void set_part_list_basic_result( idl::mcp_get_content_list_from_part_response& out, const int cnt, const string content_id,
         const string content_name, const string class_id, const string class_name, const string game_code, const string image_url )
{
    out.m_result_params()->set_content_list( cnt, 0, content_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 1, content_name.c_str() );
    out.m_result_params()->set_content_list( cnt, 2, class_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 3, class_name.c_str() );
    out.m_result_params()->set_content_list( cnt, 4, game_code.c_str() );
    out.m_result_params()->set_content_list( cnt, 5, image_url.c_str() );
}

void set_part_list_download_result( idl::mcp_get_content_list_from_part_response& out, const int cnt, const string download_id,
                                          const string download_path, const string charge_id, const string charge_name )
{
    out.m_result_params()->set_content_list( cnt, 6, download_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 7, download_path.c_str() );
    out.m_result_params()->set_content_list( cnt, 8, charge_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 9, charge_name.c_str() );
}

int McpServlet::get_content_list_from_part(const idl::mcp_get_content_list_from_part_params& in,
                                                 idl::mcp_get_content_list_from_part_response& out)
{
    UB_LOG_NOTICE( "get_content_list_from_part start" );

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }
  
    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );

    string part_id( in.part_id() );
//s.10/18/2011 liyong 
    string content_type_id( in.content_type_id() );

//e.10/18/2011 liyong 
    string info_id( in.info_id() );
    uint32_t info_type = in.info_type();
    uint32_t unit_num = in.unit_num();
    uint32_t unit_times = in.unit_times();

    UB_LOG_DEBUG( "++++++ part_id[%s], content_type_id[%s] +++++", part_id.c_str(), content_type_id.c_str() );

    if( unit_num == 0 || unit_times == 0 )
    {
        UB_LOG_FATAL( "The unit_num or unit_times is invalid " );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }

    uint32_t start_pos = unit_num * ((int)unit_times - 1 );

    int res(0);

    string column_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_COLUMN_COL ) );
    string game_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL ) );
    string data_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DATA_COL ) );

    bson query[1], fields[1];
    const bson *bson_result;
    mongo_cursor *cursor = NULL;

    bson_iterator bson_it[1];

    bson_init( query );
    bson_append_string( query, "column_id", part_id.c_str() );
    bson_append_int( query, "enable", 1 );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_finish( fields );
   
    uint32_t count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_COLUMN_COL, query );
//    out.m_result_params()->set_count( uint32_t(count) );

    if( count == 0 || start_pos >= count )
    {
        UB_LOG_FATAL( "count content info from [%s] by column_id[%s] faied", column_ns.c_str(), part_id.c_str() );
        UB_LOG_FATAL( "count is 0 or start_pos is greater than count, [count=%d], [ start_pos=%d]", count, start_pos );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        free_data( cursor, query, fields, NULL );
        return MCP_OK;
    }

    free_data( cursor, query, fields, NULL );

    bson_init( query );

    bson_append_start_object( query, "$query" );
    bson_append_string( query, "column_id", part_id.c_str() );
    bson_append_int( query, "enable", 1 );
    bson_append_finish_object( query );

//s.11/31/2011 liyong  youxi paixu   zhuti keneng huiyou yingxiang 
//s.10/20/2011 liyong 
    bson_append_start_object( query, "$orderby" );
    bson_append_int( query, "priority", -1 );
    bson_append_finish_object( query );
//e.10/20/2011 liyong 

    bson_finish( query );
            
    bson_init( fields );
    bson_append_int( fields, "c_id", 1 ); 
    bson_finish( fields );

    string c_id, content_name, class_id, class_name, game_code, image_url;
    string download_id, download_path, charge_id, charge_name;

    cursor = mongo_find( conn, column_ns.c_str(), query, fields, unit_num, start_pos, 0 );

    if( cursor == NULL )
    {
        free_data( NULL, query, fields, NULL );
        UB_LOG_FATAL( "query c_id from %s by column_id[%s] failed", column_ns.c_str(), part_id.c_str() );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }

    int i(0);
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );

        if( bson_find( bson_it, bson_result, "c_id" ) == BSON_STRING )
        {
            c_id = bson_iterator_string( bson_it );

//s.10/18/2011 liyong 
            //res = get_content_basic_info( conn, c_id, content_name, class_id, class_name, game_code, image_url );
            res = get_content_basic_info( conn, content_type_id, c_id, content_name, class_id, class_name, game_code, image_url );
//e.10/18/2011 liyong 
            if( res != MCP_OK )
            {
                //free_data( cursor, query, fields, NULL );
                //mongo_destroy( conn );
                UB_LOG_FATAL( "get content basic info failed, c_id=[%s], [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
                continue;
            }
            out.m_result_params()->set_count( uint32_t(count) );
            set_part_list_basic_result( out, i, c_id, content_name, class_id, class_name, game_code, image_url );
            if( !info_id.empty() )
            {
//s.10/18/2011 liyong
                string default_charge_typeid, default_charge_typename;
                res = get_default_charge_type_by_cid( conn, content_type_id, c_id, default_charge_typeid );
                if( res != MCP_OK )
                {
                    UB_LOG_FATAL( "get default_charge_type_id failed by c_id[%s], [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
                    default_charge_typeid = "";
                }
//                else
//                {
//                    res = get_charge_type_name_by_id( conn, default_charge_typeid, default_charge_typename );
//                    if( res != MCP_OK )
//                    {
//                        UB_LOG_FATAL("get charge_type_name failed by id[%s], [%s:%d]",
//                                              default_charge_typeid.c_str(),__FILE__,__LINE__);
//                        default_charge_typename = "";
//                    }
//                }
                res = get_download_basic_info( conn, c_id, info_type, info_id, default_charge_typeid, m_charge_id_name,
                                                           download_id, download_path, charge_id, charge_name );
//e.10/18/2011 liyong 
                if( res != MCP_OK )
                {
                    UB_LOG_FATAL( "get download basic info failed, c_id=[%s], info_type=[%d], info_id=[%s], [%s:%d]",
                                   c_id.c_str(), info_type, info_id.c_str(), __FILE__, __LINE__ );
                    //download_id = download_path = charge_id = charge_name =  "";
                }
                set_part_list_download_result( out, i, download_id, download_path, charge_id, charge_name );
            }
        }
        else
        {
            UB_LOG_FATAL( "wrong c_id data type, [%s:%d]", __FILE__, __LINE__ );
            //mongo_destroy( conn );
            mongo_pool->releaseConnection(conn);
            free_data( cursor, query, fields, NULL );
            return MCP_OK;
        }
        ++i;
    }

    UB_LOG_NOTICE( "get_content_list_from_part finished, [%s:%d]", __FILE__, __LINE__ );
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    free_data( cursor, query, fields, NULL );
    return MCP_OK;
}
*/

int get_column_name( mongo *conn, const string column_id, string& column_name )
{
    UB_LOG_NOTICE( "get_content_column_name start" );

    int res = 0;

    bson query, fields, bson_out;

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DIY_COL ) );

    bson_init( &query );
    bson_append_string( &query, "id", column_id.c_str() );
    bson_finish( &query );

    bson_init( &fields );
    bson_append_int( &fields, "name", 1 );
    bson_finish( &fields );

    res = mongo_find_one( conn, ns.c_str(), &query, &fields, &bson_out );

    bson_destroy( &query );
    bson_destroy( &fields );

    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "query _name where column_id(%s) from MongoDB %s failed", column_id.c_str(), ns.c_str() );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query column_name where column_id(%s) from MongoDB %s success", column_id.c_str(), ns.c_str() );

    bson_iterator bson_it;
    if( bson_find( &bson_it, &bson_out, "name" ) == BSON_STRING )
    {
        column_name = bson_iterator_string( &bson_it );
        UB_LOG_NOTICE( "column_name: %s", column_name.c_str() );
        UB_LOG_NOTICE( "get_content_column_name finished" );

        bson_destroy( &bson_out );
        return MCP_OK;

    }
    UB_LOG_FATAL( "get column_name failed" );
    bson_destroy( &bson_out );
    return MCP_NG;

}

/*
int McpServlet::get_content_diy_col_name(const idl::mcp_get_content_diy_col_name_params& in, 
                                               idl::mcp_get_content_diy_col_name_response& out)
{
    UB_LOG_NOTICE( "get_content_diy_column_name start" );
    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    const string column_id( in.diycol_id() );
    string column_name;

    int res(0);
    res = get_column_name( conn, column_id, column_name );
    if( res != MCP_OK )
    {
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        UB_LOG_FATAL( "get_content_diy_column_name failed" );
        return MCP_OK;
    }
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    out.m_result_params()->set_diycol_name( column_name.c_str() );
    return MCP_OK;
}
*/

/*
void set_typeID_content_list_result( idl::mcp_get_content_list_from_typeID_response& out,
                                       const uint32_t cnt, const content_basic_info_t content_info )
{
    out.m_result_params()->set_content_list( cnt, 0, content_info.content_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 1, content_info.content_name.c_str() );
    out.m_result_params()->set_content_list( cnt, 2, content_info.class_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 3, content_info.class_name.c_str() );
    out.m_result_params()->set_content_list( cnt, 4, content_info.game_code.c_str() );
    out.m_result_params()->set_content_list( cnt, 5, content_info.wap_url.c_str() );
    out.m_result_params()->set_content_list( cnt, 6, content_info.data_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 7, content_info.data_path.c_str() );
    out.m_result_params()->set_content_list( cnt, 8, content_info.charge_type_id.c_str() );
    out.m_result_params()->set_content_list( cnt, 9, content_info.charge_type_name.c_str() );
}

int McpServlet::get_content_list_from_typeID(const idl::mcp_get_content_list_from_typeID_params& in,
                                       idl::mcp_get_content_list_from_typeID_response& out)
{
    UB_LOG_NOTICE( "get_content_list_from_type_id start" );
    mongo *conn;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    const string type_id( in.content_type_id() );
    const uint32_t priority( in.priority() );
    const string info_id( in.info_id() );
    const uint32_t info_type( in.info_type() );
    const string img_size( in.img_size() );
    const uint32_t unit_num( in.unit_num() );
    const uint32_t unit_times( in.unit_times() );

    vector<string> content_id_list;
    vector<content_basic_info_t> content_info_list;
    uint32_t count;
    int res( 0 );

//    res = get_content_id_list_by_typeID( conn, type_id, content_id_list );
//    if( res != MCP_OK )
//    {
//        out.m_result_params()->set_count( 0 );
//        //mongo_destroy( conn );
//        UB_LOG_FATAL( "get content_id list by content_type_id[%s] failed, [%s:%d]", type_id.c_str(), __FILE__, __LINE__ );
//        return MCP_OK;
//    }
//    UB_LOG_NOTICE( "get content_id list by content_type_id[%s] success, [%s:%d]", type_id.c_str(), __FILE__, __LINE__ );

    res = get_content_info_by_id_list( conn, type_id, priority, unit_num, unit_times, info_id, info_type, img_size, content_id_list,
                                       count, content_info_list );
    if( res != MCP_OK )
    {
        out.m_result_params()->set_count( 0 );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        UB_LOG_FATAL( "get content info list failed, [%s:%d]",  __FILE__, __LINE__ );
        return MCP_OK;
    }
    UB_LOG_NOTICE( "get content info list success, [%s:%d]",  __FILE__, __LINE__ );

    out.m_result_params()->set_count( count );

    int size = content_info_list.size();
    for( int i = 0; i < size; ++i )
    {
        set_typeID_content_list_result( out, i, content_info_list[i]);
    }

    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    UB_LOG_NOTICE( "get_content_list_from_typeID finish" );
    return MCP_OK;
}
*/

int McpServlet::get_content_count_from_class(const idl::mcp_get_content_count_from_class_params& in, 
                                                   idl::mcp_get_content_count_from_class_response& out)
{
    UB_LOG_NOTICE( "get_content_count_from_class start" );

    mongo *conn;
    int res( 0 );

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    string class_id( in.class_id() );
    uint32_t class_type( in.class_type() );

    string class_name;
    uint32_t count(0);
    if( class_type == 1 )
    {
        res = get_class_name( conn, class_id, class_name );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get class_name by class_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
        }
        res = get_content_count_by_classID( conn, class_id, count );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get count by class_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
        }
    }
    else if( class_type == 2 )
    {
        res = get_column_name( conn, class_id, class_name );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get column_name by column_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
        }
        res = get_content_count_by_columnID( conn, class_id, count );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get count by column_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
        }
    }
    else if( class_type == 3 )
    {
/*
        int type_id = atoi( class_id.c_str() );
        switch( type_id )
        {
            case 1:
                class_name = "ÓÎÏ·";
                break;
            case 5:
                class_name = "Ö÷Ìâ";
                break;
            default:
                class_name = "";
                break;
        }
*/
        res = get_content_count_by_typeID( conn, class_id, count );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get count by type_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
        }
    }

    out.m_result_params()->set_class_name( class_name.c_str() );
    out.m_result_params()->set_count( count );

    UB_LOG_NOTICE( "get_content_count_from_class end" );
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    return MCP_OK;
}

void set_get_url_info_by_id_result( idl::mcp_get_url_info_by_id_response& out, const url_info_t& url_info )
{
    out.m_result_params()->set_url_info( 0, url_info.name.c_str() );
    out.m_result_params()->set_url_info( 1, url_info.class_id.c_str() );
    out.m_result_params()->set_url_info( 2, url_info.class_name.c_str() );
    out.m_result_params()->set_url_info( 3, url_info.ext_id.c_str() );
    out.m_result_params()->set_url_info( 4, url_info.ad.c_str() );
    out.m_result_params()->set_url_info( 5, url_info.url.c_str() );
    out.m_result_params()->set_url_info( 6, url_info.baiduurl.c_str() );
    out.m_result_params()->set_url_info( 7, url_info.mark.c_str() );
    out.m_result_params()->set_url_info( 8, url_info.logdate.c_str() );
    out.m_result_params()->set_url_info( 9, url_info.modify_date.c_str() );
}

int McpServlet::get_url_info_by_id(const idl::mcp_get_url_info_by_id_params& in, idl::mcp_get_url_info_by_id_response& out)
{
    UB_LOG_NOTICE( "get_url_info_by_id start" );

    mongo *conn;
    int res( 0 );

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    string id( in.id() );
    struct url_info_t url_info;

    res = mcp_get_url_info_by_id( conn, id, url_info );
    if( res != MCP_OK )
    {
        UB_LOG_FATAL( "get_url_info_by_id failed, [%s:%d]", __FILE__, __LINE__ );
    }

    set_get_url_info_by_id_result( out, url_info );
    UB_LOG_NOTICE( "get_url_info_by_id end" );
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    return MCP_OK;
}

void set_get_url_info_by_class_id_result( idl::mcp_get_url_info_by_class_id_response& out,
                                    const uint32_t cnt, const url_info_t& url_info )
{
    out.m_result_params()->set_url_info( cnt, 0, url_info.id.c_str() );
    out.m_result_params()->set_url_info( cnt, 1, url_info.name.c_str() );
    out.m_result_params()->set_url_info( cnt, 2, url_info.class_id.c_str() );
    out.m_result_params()->set_url_info( cnt, 3, url_info.class_name.c_str() );
    out.m_result_params()->set_url_info( cnt, 4, url_info.ext_id.c_str() );
    out.m_result_params()->set_url_info( cnt, 5, url_info.ad.c_str() );
    out.m_result_params()->set_url_info( cnt, 6, url_info.url.c_str() );
    out.m_result_params()->set_url_info( cnt, 7, url_info.baiduurl.c_str() );
    out.m_result_params()->set_url_info( cnt, 8, url_info.mark.c_str() );
    out.m_result_params()->set_url_info( cnt, 9, url_info.logdate.c_str() );
    out.m_result_params()->set_url_info( cnt, 10, url_info.modify_date.c_str() );
}

int McpServlet::get_url_info_by_class_id(const idl::mcp_get_url_info_by_class_id_params& in,
                                               idl::mcp_get_url_info_by_class_id_response& out)
{
    UB_LOG_NOTICE( "get_url_info_by_class_id start" );


    mongo *conn;
    int res( 0 );

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    string class_id( in.class_id() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_time( in.unit_times() );
    vector<url_info_t> url_info_list;

    uint32_t count( 0 );
    res = mcp_get_url_count_by_class_id( conn, class_id, count );
    if( res != MCP_OK )
    {
        UB_LOG_FATAL( "mcp_get_url_count_by_class_id failed, [%s:%d]", __FILE__, __LINE__ );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }

    uint32_t start_pos = unit_num * ( unit_time-1 );
    if( start_pos > count )
    {
        UB_LOG_FATAL( "start_pos[%d] > count[%d], [%s:%d]", start_pos, count, __FILE__, __LINE__ );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }

    res = mcp_get_url_info_by_class_id( conn, class_id, unit_num, unit_time, url_info_list );
    if( res != MCP_OK )
    {
        UB_LOG_FATAL( "mcp_get_url_info_by_class_id faield, [%s:%d]", __FILE__, __LINE__ );
    }

    out.m_result_params()->set_count( count );
    for( uint32_t i = 0; i < url_info_list.size(); ++i )
    {
        set_get_url_info_by_class_id_result( out, i, url_info_list[i] );
    }

    UB_LOG_NOTICE( "get_url_info_by_class_id end" );
    return MCP_OK;
}

void set_get_url_info_by_column_id_result( idl::mcp_get_url_info_by_column_id_response& out,
                                    const uint32_t cnt, const url_info_t& url_info )
{
    out.m_result_params()->set_url_info( cnt, 0, url_info.id.c_str() );
    out.m_result_params()->set_url_info( cnt, 1, url_info.name.c_str() );
    out.m_result_params()->set_url_info( cnt, 2, url_info.class_id.c_str() );
    out.m_result_params()->set_url_info( cnt, 3, url_info.class_name.c_str() );
    out.m_result_params()->set_url_info( cnt, 4, url_info.ext_id.c_str() );
    out.m_result_params()->set_url_info( cnt, 5, url_info.ad.c_str() );
    out.m_result_params()->set_url_info( cnt, 6, url_info.url.c_str() );
    out.m_result_params()->set_url_info( cnt, 7, url_info.baiduurl.c_str() );
    out.m_result_params()->set_url_info( cnt, 8, url_info.mark.c_str() );
    out.m_result_params()->set_url_info( cnt, 9, url_info.logdate.c_str() );
    out.m_result_params()->set_url_info( cnt, 10, url_info.modify_date.c_str() );
}

int McpServlet::get_url_info_by_column_id(const idl::mcp_get_url_info_by_column_id_params& in,
                                             idl::mcp_get_url_info_by_column_id_response& out)
{
    UB_LOG_NOTICE( "get_url_info_by_column_id start" );

    mongo *conn;
    int res( 0 );

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    string column_id( in.column_id() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_time( in.unit_times() );
    vector<url_info_t> url_info_list;

    uint32_t count( 0 );
    res = mcp_get_url_count_by_column_id( conn, column_id, count );
    if( res != MCP_OK )
    {
        UB_LOG_FATAL( "mcp_get_url_count_by_column_id failed, [%s:%d]", __FILE__, __LINE__ );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }

    uint32_t start_pos = unit_num * ( unit_time-1 );
    if( start_pos > count )
    {
        UB_LOG_FATAL( "start_pos[%d] > count[%d], [%s:%d]", start_pos, count, __FILE__, __LINE__ );
        //mongo_destroy( conn );
        mongo_pool->releaseConnection(conn);
        return MCP_OK;
    }

    res = mcp_get_url_info_by_column_id( conn, column_id, unit_num, unit_time, url_info_list );
    if( res != MCP_OK )
    {
        UB_LOG_FATAL( "mcp_get_url_info_by_column_id faield, [%s:%d]", __FILE__, __LINE__ );
    }

    out.m_result_params()->set_count( count );
    for( uint32_t i = 0; i < url_info_list.size(); ++i )
    {
        set_get_url_info_by_column_id_result( out, i, url_info_list[i] );
    }

    UB_LOG_NOTICE( "get_url_info_by_column_id end" );
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    return MCP_OK;
}

/*
void set_get_content_list_from_classID_result( idl::mcp_get_content_list_from_classID_response& out,
                                               const uint32_t i, vector<content_info_t>::iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->wap_url.c_str() );
}

int McpServlet::get_content_list_from_classID(const idl::mcp_get_content_list_from_classID_params& in,
                                                    idl::mcp_get_content_list_from_classID_response& out)
{
    UB_LOG_TRACE( "get_content_list_from_classID start" );

    string class_id( in.class_id() );
    string content_type_id( in.content_type_id() );
    uint32_t priority( in.priority() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    string img_size( in.img_size() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );

    uint32_t for_adapt( in.for_adapt() );

    int res( 0 );
    mongo *conn;
    uint32_t count( 0 );
    uint32_t i( 0 );
    vector<content_info_t>::iterator iter; 
    vector<content_info_t> content_info_list;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        goto end;
    }

    if( for_adapt == 0 || info_id.empty() || info_id.compare( "" ) == 0 )
    {
        res = get_content_id_list_by_class_id( conn, content_type_id, class_id, priority, unit_num, unit_times,
                                               info_id, info_type, img_size, count, content_info_list );

        if( res == -5 )
        {
            UB_LOG_FATAL( "get_content_id_list_by_class_id failed, [%s:%d]", __FILE__, __LINE__ );
            goto end;
        }
    }
    else
    {
        res = get_count_by_rank( content_type_id, class_id, info_id, info_type, for_adapt );
        if( res < 0 )
        {
           UB_LOG_FATAL( "get_count_by_rank failed, [%s:%d]", __FILE__, __LINE__ );
           count = 0;
           goto end;
        }
        count = res;
        res = get_adapt_content_info_by_class( content_type_id, class_id, priority, unit_num, unit_times, info_id, info_type,
                                               content_info_list );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_adapt_content_info_by_class failed, [%s:%d]", __FILE__, __LINE__ );
            count = 0;
            goto end;
        }
    }

end:
    //mongo_destroy( conn );
    mongo_pool->releaseConnection(conn);
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    count= content_info_list.size();
    for( i = 0; iter != content_info_list.end(); ++iter, ++i )
    {
        UB_LOG_NOTICE( "id, [%s]", iter->id.c_str() );
        set_get_content_list_from_classID_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_from_classID end" );
    return 0;
}
*/

/*
void set_get_content_list_from_phone_result( idl::mcp_get_content_list_from_phone_response& out,
                                               const uint32_t i, vector<content_info_t>::iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->charge_value.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->wap_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->class_name.c_str() );
}

int McpServlet::get_content_list_from_phone(const idl::mcp_get_content_list_from_phone_params& in, 
                                                  idl::mcp_get_content_list_from_phone_response& out)
{
    UB_LOG_TRACE( "get_content_list_from_phone start" );

    string p_id( in.p_id() );
    uint32_t priority( in.priority() );
    uint32_t type( in.type() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    
    int64_t count = 0;

    vector<content_info_t> content_info_list;
    vector<content_info_t>::iterator iter;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

//    mongo *conn;
//    conn = mongo_pool->getConnection();
//    if( conn == NULL )
//    {
//        mongo_pool->releaseConnection(conn);
//        UB_LOG_FATAL( "mongodb connnection pool get connect failed, [%s:%d]", __FILE__, __LINE__ );
//        return 0;
//    }
    int res = 0;
    res = get_content_list_by_phone_id( conn, p_id, priority, type, unit_num, unit_times, &count, &content_info_list);
    //mongo_destroy(conn);
    mongo_pool->releaseConnection(conn);

    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i )
    {
        UB_LOG_NOTICE( "id, [%s]", iter->id.c_str() );
        set_get_content_list_from_phone_result( out, i, iter );
    }
    content_info_list.clear();
    UB_LOG_TRACE( "get_content_list_from_phone end" );
    return 0;
}
*/

/*
void set_get_content_list_by_rank_result( idl::mcp_get_content_list_by_rank_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->wap_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_value.c_str() );
}

int McpServlet::get_content_list_by_rank(const idl::mcp_get_content_list_by_rank_params& in, 
                                       idl::mcp_get_content_list_by_rank_response& out)
{
    UB_LOG_TRACE( "get_content_list_by_rank start" );
    int count = 0;
    int res = 0;
    string content_type( in.content_type() );
    string class_id( in.class_id() );
    uint32_t p_type = in.p_type();
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    uint32_t for_adapt( in.for_adapt() );

    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;
    vector<string> info_id_list;

//    char stype_id_list[1024];
//    memset( stype_id_list, 0x00, sizeof(stype_id_list) );
//    strcpy( stype_id_list, info_id.c_str() );
//    char *p_stype_id = stype_id_list;
//    for( int j=0; (tmp=strsep(&p_stype_id,","))!=NULL; ++j )
//    {
//        info_id_list.push_back( tmp );
//    }

    res = get_count_by_rank( content_type, class_id, info_id, info_type, for_adapt );
    if( res < 0 )
    {
        UB_LOG_FATAL( "get_count_by_rank failed, [%s:%d]", __FILE__, __LINE__ );
        if( res == -1 )
        {
            count = 0;
        }
        else
        {
            count = res;
        }
        goto end;
    }
    count = res;

    if( for_adapt==0 || info_id.empty() || info_id.compare("")==0 )
    {
        res = get_content_info_by_rank( content_type, class_id, p_type, unit_num, unit_times, info_id, info_type, 
                                        content_info_list );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_content_info_by_rank failed, [%s:%d]", __FILE__, __LINE__ );
            count = res;
            goto end;
        }
    }
    else
    {
        res = get_adapt_content_info_by_rank( content_type, class_id, p_type, unit_num, unit_times, info_id, info_type,
                                              content_info_list );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_adapt_content_info_by_rank failed, [%s:%d]", __FILE__, __LINE__ );
            count = res;
            goto end;
        }
    }

end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i )
    {
        UB_LOG_DEBUG( "id, [%s]", iter->id.c_str() );
        set_get_content_list_by_rank_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_by_rank end" );
    return 0;
}
*/

int McpServlet::get_label_by_content_id(const idl::mcp_get_label_by_content_id_params& in, 
                                      idl::mcp_get_label_by_content_id_response& out)
{
    UB_LOG_TRACE( "get_label_by_content_id start" );
    int result = 0;
    string content_id( in.content_id() );

    vector<string> label_id_list;
    map<string,string> m_label_id_name;
    int i;

    get_label_id_name_map( m_label_id_name );

    result = get_label_by_c_id( content_id, label_id_list );
    if( result <  0 )
    {
        UB_LOG_FATAL( "get_label_by_c_id failed, [%s:%d]", __FILE__, __LINE__ );
        goto end;
    }

    goto end;
end:
    out.m_result_params()->set_result( result );
    for( i = 0; i < (int)label_id_list.size(); ++i )
    {
        out.m_result_params()->set_label_list( i, 0, label_id_list[i].c_str() );
        out.m_result_params()->set_label_list( i, 1, m_label_id_name[label_id_list[i]].c_str() );
    }
    UB_LOG_TRACE( "get_label_by_content_id end" );
    return 0;
}

/*
void set_get_content_list_by_label_result( idl::mcp_get_content_list_by_label_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->wap_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_value.c_str() );
}

int McpServlet::get_content_list_by_label(const idl::mcp_get_content_list_by_label_params& in, 
                                        idl::mcp_get_content_list_by_label_response& out)
{
    UB_LOG_TRACE( "get_content_list_by_label start" );
    int count = 0;
    int res = 0;
    string label_id( in.label_id() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    //TODO for_adapt
    
    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;

    res = get_count_by_label( label_id );
    if( res < 0 )
    {
        UB_LOG_FATAL( "get_count_by_rank failed, [%s:%d]", __FILE__, __LINE__ );
        count =  res;
        goto end;
    }
    count = res;

    res = get_content_info_by_label( label_id, unit_num, unit_times, info_id, info_type, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_content_info_by_label failed, [%s:%d]", __FILE__, __LINE__ );
        count =  res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i )
    {
        UB_LOG_DEBUG( "id, [%s]", iter->id.c_str() );
        set_get_content_list_by_label_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_by_label end" );
    return 0;
}
*/
    
int McpServlet::get_topic_info(const idl::mcp_get_topic_info_params& in, idl::mcp_get_topic_info_response& out)
{
    UB_LOG_TRACE( "get_topic_info start" );
    int res = 0;
    int count = 0;
    int i = 0;
    string content_type( in.content_type() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    vector<topic_info_t> topic_info_list;

    res = get_topic_count_by_type( content_type );
    if( res < 0 )
    {
        UB_LOG_FATAL( "get_count_by_rank failed, [%s:%d]", __FILE__, __LINE__ );
        count =  res;
        goto end;
    }
    count = res;

    res = get_topic_info_by_type( content_type, unit_num, unit_times, topic_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_topic_info_by_type failed, [%s:%d]", __FILE__, __LINE__ );
        count =  res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    for( i = 0; i < (int)topic_info_list.size(); ++i )
    {
        out.m_result_params()->set_topic_list( i, 0, topic_info_list[i].id.c_str() );
        out.m_result_params()->set_topic_list( i, 1, topic_info_list[i].name.c_str() );
    }
    UB_LOG_TRACE( "get_topic_info end" );
    return 0;
}

int McpServlet::get_content_count_by_class(const idl::mcp_get_content_count_by_class_params& in, 
                                                idl::mcp_get_content_count_by_class_response& out)
{
    UB_LOG_TRACE( "get_content_count_by_class start" );
    vector<int> count;
    int res;
    int i;
    string info_id = in.info_id();
    uint32_t info_type = in.info_type();
    vector<string> class_id;
    char *tmp = NULL;

    UB_LOG_DEBUG( "class_id:[%s], strlen:[%d]", in.class_id(), (int)strlen(in.class_id()) );
    char *class_id_list = new char[strlen(in.class_id())+1];
    strcpy( class_id_list, in.class_id() );
    UB_LOG_DEBUG( "class_id_list:[%s], class_id:[%s]", class_id_list, in.class_id() );
    char *p_id_list = class_id_list;
    while((tmp=strsep(&p_id_list,","))!=NULL)
    {
        UB_LOG_DEBUG( "++++++ class_id:[%s]++++++, [%s:%d]", tmp, __FILE__, __LINE__ );
        class_id.push_back( tmp );
    }
    delete[] class_id_list;

    res = get_adapt_count_by_class_id( class_id, info_id, info_type, count );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_count_by_class_id failed,m [%s:%d]", __FILE__, __LINE__ );
        goto end;
    }

end:
    for( i = 0; i < (int)count.size(); ++i )
    {
        out.m_result_params()->set_count( i, count[i] );
    }
    UB_LOG_TRACE( "get_content_count_by_class end" );
    return 0;
}

/*
int McpServlet::get_content_img(const idl::mcp_get_content_img_params& in, idl::mcp_get_content_img_response& out)
{
    UB_LOG_TRACE( "get_content_img start" );
    int res = 0;
    string content_id( in.content_id() );
    uint32_t img_type( in.img_type() );
    vector<string> img_url_list;
    int i = 0;

    res = get_content_image( content_id, img_type, img_url_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_content_image failed,m [%s:%d]", __FILE__, __LINE__ );
        goto end;
    }
    else
    {
        res = img_url_list.size();
    }

end:
    out.m_result_params()->set_count( res );
    for( i = 0; i < res; ++i )
    { 
        out.m_result_params()->set_img_url( i, img_url_list[i].c_str() );
    }
    UB_LOG_TRACE( "get_content_img end" );
    return 0;
}
*/

/*
int McpServlet::get_label_name_by_id(const idl::mcp_get_label_name_by_id_params& in, idl::mcp_get_label_name_by_id_response& out)
{
    UB_LOG_TRACE( "get_label_name_by_id start" );
    int res = 0;
    string label_id( in.label_id() );
    string label_name;

    res = get_label_name_by_label_id( label_id, label_name );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_label_name_by_label_id failed, [%s:%d]", __FILE__, __LINE__ );
        goto end;
    }

end:
    out.m_result_params()->set_label_name( label_name.c_str() );
    UB_LOG_TRACE( "get_label_name_by_id end" );
    return 0;
}
*/


/*
void set_get_content_list_by_date_result( idl::mcp_get_content_list_by_date_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->wap_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_value.c_str() );
}

int McpServlet::get_content_list_by_date(const idl::mcp_get_content_list_by_date_params& in, 
                                               idl::mcp_get_content_list_by_date_response& out)
{
    UB_LOG_TRACE( "get_content_list_by_date start" );

    int count = 0;
    int res = 0;
    string content_type( in.content_type() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    uint32_t for_adapt( in.for_adapt() );

    vector<content::content_info_t> content_info_list;
    vector<content::content_info_t>::const_iterator iter;
    vector<string> info_id_list;

    res = get_count_by_rank( content_type, "0", info_id, info_type, for_adapt );
    if( res < 0 )
    {
        UB_LOG_FATAL( "get_count_by_rank failed, [%s:%d]", __FILE__, __LINE__ );
        if( res == -1 )
        {
            count = 0;
        }
        else
        {
            count = res;
        }
        goto end;
    }
    count = res;

    if( for_adapt==0 || info_id.empty() || info_id.compare("")==0 )
    {
        res = get_content_info_by_date( content_type, unit_num, unit_times, info_id, info_type,
                                        content_info_list );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_content_info_by_date failed, [%s:%d]", __FILE__, __LINE__ );
            count = res;
            goto end;
        }
    }
    else
    {
        res = get_adapt_content_info_by_date( content_type, unit_num, unit_times, info_id, info_type,
                                              content_info_list );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_adapt_content_info_by_date failed, [%s:%d]", __FILE__, __LINE__ );
            count = res;
            goto end;
        }
    }

end:
    out.m_result_params()->set_result( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i )
    {
        UB_LOG_DEBUG( "id, [%s]", iter->id.c_str() );
        set_get_content_list_by_date_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_by_date end" );
    return 0;
}
*/

