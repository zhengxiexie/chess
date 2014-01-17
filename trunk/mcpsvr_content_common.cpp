#include "mcpsvr_content_common.h"

int get_content_type_by_id( mongo *conn, const string id, string& type_id )
{
    UB_LOG_NOTICE( "get_content_type_by_id start" );

    int res = 0;

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_TYPE_COL) );

    bson query, fields, bson_out;

    bson_init( &query );
    bson_append_string( &query, "id", id.c_str() );
    bson_finish( &query );

    bson_init( &fields );
    bson_append_int( &fields, "type_id", 1 );
    bson_finish( &fields );

    res = mongo_find_one( conn, ns.c_str(), &query, &fields, &bson_out );

    bson_destroy( &query );
    bson_destroy( &fields );

    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "query type_id where id(%s) from MongoDB %s failed", id.c_str(), ns.c_str() );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query type_id where id(%s) from MongoDB %s success", id.c_str(), ns.c_str() );

    bson_iterator bson_it;
    if( bson_find( &bson_it, &bson_out, "type_id" ) == BSON_STRING )
    {
        type_id = bson_iterator_string( &bson_it );
        UB_LOG_NOTICE( "type_id: %s", type_id.c_str() );
        UB_LOG_NOTICE( "get_content_type_by_id finished" );

        bson_destroy( &bson_out );
        return MCP_OK;

    }
    UB_LOG_FATAL( "type_id has wrong datatype" );
    bson_destroy( &bson_out );
    return MCP_NG;
}

int get_charge_type_name_by_id( mongo *conn, const string id, string& name )
{
    UB_LOG_NOTICE( "get_charge_type_name_by_id start" );

    int res = 0;

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_CHARGETYPE_COL) );

    bson query, fields, bson_out;
                    
    bson_init( &query );
    bson_append_string( &query, "id", id.c_str() );
    bson_finish( &query );
                        
    bson_init( &fields );
    bson_append_int( &fields, "name", 1 );
    bson_finish( &fields );
                
    res = mongo_find_one( conn, ns.c_str(), &query, &fields, &bson_out );
                    
    bson_destroy( &query );
    bson_destroy( &fields );

    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "query charge_type_name where charge_type_id(%s) from MongoDB %s failed", id.c_str(), ns.c_str() );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query charge_type_name where charge_type_id(%s) from MongoDB %s success", id.c_str(), ns.c_str() );

    bson_iterator bson_it;
    if( bson_find( &bson_it, &bson_out, "name" ) == BSON_STRING )
    {
        name = bson_iterator_string( &bson_it );
        UB_LOG_NOTICE( "name: %s", name.c_str() );
        UB_LOG_NOTICE( "get_charge_type_name finished" );

        bson_destroy( &bson_out );
        return MCP_OK;

    }
    UB_LOG_FATAL( "charge_type_name has wrong datatype id(%s)", id.c_str() );
    bson_destroy( &bson_out );
    return MCP_NG;

}

int get_default_charge_type_by_cid( mongo *conn, const string content_type_id, const string c_id, string& default_charge_type )
{
    UB_LOG_NOTICE( "get_default_charge_type_by_cid start" );

    int res = 0;

    string ns;

    if( content_type_id == MCP_CONTENT_GAME )
    {
         ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL);
    }
    else if( content_type_id == MCP_ANDROID_GAME )
    {
         ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL);
    }
    else if( content_type_id == MCP_CONTENT_THEME )
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL);
    }

    bson query, fields, bson_out;
                    
    bson_init( &query );
    bson_append_string( &query, "id", c_id.c_str() );
    bson_append_string( &query, "content_type", content_type_id.c_str() );
    bson_finish( &query );
                        
    bson_init( &fields );
    bson_append_int( &fields, "default_charge_typeid", 1 );
    bson_finish( &fields );
                
    res = mongo_find_one( conn, ns.c_str(), &query, &fields, &bson_out );
                    
    bson_destroy( &query );
    bson_destroy( &fields );

    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "query default_charge_type_id where c_id(%s) from MongoDB %s failed", c_id.c_str(), ns.c_str() );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query default_charge_type_id where c_id(%s) from MongoDB %s success", c_id.c_str(), ns.c_str() );

    bson_iterator bson_it;
    if( bson_find( &bson_it, &bson_out, "default_charge_typeid" ) == BSON_STRING )
    {
        default_charge_type = bson_iterator_string( &bson_it );
        UB_LOG_NOTICE( "default_charge_type_id: %s", default_charge_type.c_str() );
        UB_LOG_NOTICE( "get_default_charge_type_by_cid finished" );

        bson_destroy( &bson_out );
        return MCP_OK;

    }
    UB_LOG_FATAL( "default_charge_type_id has wrong datatype c_id(%s)", c_id.c_str() );
    bson_destroy( &bson_out );
    return MCP_NG;

}

int get_theme_basic_info_by_id( mongo *conn, const string id, string& name, string logdate )
{
    UB_LOG_NOTICE( "get_theme_basic_info_by_id start" );

    int res = 0;

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL) );

    bson query, fields, bson_out;

    bson_init( &query );
    bson_append_string( &query, "id", id.c_str() );
    bson_finish( &query );

    bson_init( &fields );
    bson_append_int( &fields, "name", 1 );
    bson_append_int( &fields, "logdate", 1 );
    bson_finish( &fields );

    res = mongo_find_one( conn, ns.c_str(), &query, &fields, &bson_out );

    bson_destroy( &query );
    bson_destroy( &fields );

    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "query theme basic info where id(%s) from MongoDB %s failed", id.c_str(), ns.c_str() );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query theme basic info where id(%s) from MongoDB %s success", id.c_str(), ns.c_str() );

    bson_iterator bson_it;
    bson_iterator_init( &bson_it, &bson_out );
    const char *key;
    string tmp_result = "";
    while( bson_iterator_next(&bson_it) )
    {
        key = bson_iterator_key( &bson_it );
        if( strcmp( "name", key ) == 0 )
        {
            name = bson_iterator_string( &bson_it );
            UB_LOG_NOTICE( "name: %s", name.c_str() );
        }
        else if( strcmp( "logdate", key ) == 0 )
        {
            change_everything_to_string(&bson_it,tmp_result);
            logdate = tmp_result;
            UB_LOG_NOTICE( "logdate: [%s]", logdate.c_str() );
        }
        else
        {
            UB_LOG_FATAL( "Unknown key[%s],[%s:%d]", key, __FILE__, __LINE__ );
        }
    }


    UB_LOG_NOTICE( "get_theme_basic_info_by_id finished" );
    bson_destroy( &bson_out );
    return MCP_OK;

}

int get_theme_class_info( mongo *conn, const string c_id, map<string, string>& m_class_id_name,
                                             string& class_id, string& class_name )
{
    UB_LOG_NOTICE( "get_theme_class_info start" );

    //int res = 0;

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_CLASS_LIST_COL) );

    bson query[1], fields[1];
    const bson* bson_result;

    bson_init( query );
    bson_append_string( query, "c_id", c_id.c_str() );
    bson_append_int( query, "enable", 1 );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "class_id", 1 );
    bson_finish( fields );

    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );

    class_id = class_name = "";
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query class_info where c_id(%s) from MongoDB %s failed", c_id.c_str(), ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query class_info where c_id(%s) from MongoDB %s success", c_id.c_str(), ns.c_str() );

    string tmp_class_id, tmp_class_name;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];


        tmp_class_id = tmp_class_name = "";

        if( bson_find( bson_it, bson_result, "class_id" ) == BSON_STRING )
        {
            tmp_class_id = bson_iterator_string( bson_it );
            UB_LOG_NOTICE( "class_id: %s", class_id.c_str() );
/*
            res = get_class_name( conn, tmp_class_id, tmp_class_name );
            if( res != MCP_OK )
            {
                tmp_class_name = "";
                UB_LOG_FATAL( "get theme class info by c_id[%s] failed, [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
            }
*/
            tmp_class_name = m_class_id_name[tmp_class_id];
        }
        else
        {
            UB_LOG_FATAL( "class_id has a wrong data type" );
            continue;
        }

        if( !class_id.empty() )
        {
            class_id += ",";
            class_name += ",";
        }
        class_id += tmp_class_id;
        class_name += tmp_class_name;

    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_NOTICE( "get_theme_class_info end" );
    return MCP_OK;

}

int get_img_url_by_content_id( mongo *conn, const string c_id, const string image_type_id, string& image_url )
{
    UB_LOG_NOTICE( "get_img_url_by_content_id start" );

    bson query[1], fields[1];
    bson bson_result[1];

    bson_init( query );
    bson_append_string( query, "c_id", c_id.c_str() );
    bson_append_string( query, "image_type_id", image_type_id.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "image_url", 1 );
    bson_finish( fields );

    const string preview_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_PREVIEW_COL) );

    int res( 0 );
    image_url = "";

    res = mongo_find_one( conn, preview_ns.c_str(), query, fields, bson_result );
    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "get content image_url by c_id[%s] failed, [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "get content image_url by c_id[%s] success, [%s:%d]", c_id.c_str(), __FILE__, __LINE__ );
    bson_iterator bson_it[1];
    if( bson_find( bson_it, bson_result, "image_url" ) == BSON_STRING )
    {
        image_url = bson_iterator_string( bson_it );
    }
    else
    {
        free_data( NULL, query, fields, bson_result );
        UB_LOG_FATAL( "image_url has a wrong data type" );
        return MCP_NG;
    }

    free_data( NULL, query, fields, bson_result );
    UB_LOG_NOTICE( "get_img_url_by_content_id start" );
    return MCP_OK;
}

int get_content_id_list_by_typeID( mongo *conn, const string type_id, vector<string>& content_id_list )
{
    UB_LOG_NOTICE( "get_content_id_list_by_typeID start" );

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_TYPE_COL) );

    bson query[1], fields[1];
    const bson* bson_result;

    bson_init( query );
    bson_append_string( query, "type_id", type_id.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_finish( fields );

    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );

    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query content_id list where type_id(%s) from MongoDB %s failed", type_id.c_str(), ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query content_id list where type_id(%s) from MongoDB %s success", type_id.c_str(), ns.c_str() );

    string tmp_content_id;
    content_id_list.clear();
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        tmp_content_id = "";
        if( bson_find( bson_it, bson_result, "id" ) == BSON_STRING )
        {
            tmp_content_id = bson_iterator_string( bson_it ); 
            UB_LOG_NOTICE( "get content_id[%s] by type_id[%s]", tmp_content_id.c_str(), type_id.c_str() );
            content_id_list.push_back( tmp_content_id );
        }
        else
        {
            UB_LOG_FATAL( "content_id has a wrong data type" );
            continue;
        }
    }
    
    free_data( cursor, query, fields, NULL );
    UB_LOG_NOTICE( "get_content_id_list_by_typeID end" );
    return MCP_OK;
}

/*
int get_content_info_by_id_list( mongo *conn, const string content_type_id, const uint32_t priority, const uint32_t unit_num,
                                 const uint32_t unit_times, const string info_id, const uint32_t info_type, const string img_size,
                           vector<string>& content_id_list, uint32_t& count, vector<content_basic_info_t>& content_info_list )
{
    UB_LOG_NOTICE( "get_content_info_by_id_list start" );
    
    if( unit_num == 0 || unit_times == 0 )
    {
        UB_LOG_FATAL( "The unit_num or unit_times is invalid " );
        return MCP_NG;
    }

    uint32_t start_pos = unit_num * ((int)unit_times - 1 );

    int res = 0;

    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );
    map<string, string> m_class_id_name;
    get_content_class_id_name_map( conn, m_class_id_name );


    string ns;
    if( content_type_id == MCP_CONTENT_THEME )
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL) ;
    }
    else if( content_type_id == MCP_ANDROID_GAME )
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL) ;
    }
    else 
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL) ;
    }

    bson query[1], fields[1];
    const bson* bson_result;
    //char index[16];
    vector<string>::iterator vec_it;
    //int j( 0 );

    bson_init( query );
    bson_append_string( query, "content_type", content_type_id.c_str() );
    bson_finish(query);

    if( content_type_id == MCP_CONTENT_THEME )
    {
        count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_THEME_COL, query );
    }
    else if( content_type_id == MCP_CONTENT_GAME )
    {
        count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_GAME_COL, query );
    }
    else if( content_type_id == MCP_ANDROID_GAME )
    {
        count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_GAME_COL, query );
    }
    else
    {
        count = 0;
        UB_LOG_FATAL( "Unknown content_type_id[%s],[%s:%d]", content_type_id.c_str(), __FILE__, __LINE__ );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "count[%d], [%s:%d]", count, __FILE__, __LINE__ );

    bson_destroy( query );

    bson_init( query );
    //设置查询条件
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", content_type_id.c_str() );
    bson_append_finish_object( query );
    //设置排序条件
    //优先级， 暂时都按录入时间 排序
    if( priority )
    {
        bson_append_start_object( query, "$orderby" );
        bson_append_int( query, "logdate", -1 );
        bson_append_finish_object( query );
    }
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "name", 1 );
    bson_append_int( fields, "class_id", 1 );
    bson_append_int( fields, "game_code", 1 );
    bson_append_int( fields, "default_charge_typeid", 1 );
    bson_finish( fields );

    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query content info list from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query content info list from MongoDB %s success", ns.c_str() );

    string id, name, game_code, default_charge_typeid, default_charge_typename;
    string class_id, class_name, image_url;
    string download_id, download_path, charge_type_id, charge_type_name;
    content_basic_info_t content_info;

    const char* key;
    content_info_list.clear();
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        id = name = class_id = game_code = default_charge_typeid = "";
        download_id = download_path = charge_type_id = charge_type_name = "";

        bson_iterator_init( bson_it, bson_result );
        bson_iterator sub[1];
        while( bson_iterator_next( bson_it ) )
        {
            key = bson_iterator_key( bson_it );
            if( strcmp( key, "id" ) == 0 )
            {
                id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "name" ) == 0 )
            {
                name = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "class_id" ) == 0 )
            {
                //class_id = bson_iterator_string( bson_it );
                bson_iterator_subiterator( bson_it, sub );
                while( bson_iterator_next( sub ) ) 
                {
                    if( !class_id.empty() || class_id.compare("") != 0 )
                    {
                        class_id.append( "," );
                        class_name.append( "," );
                    }
                    class_id.append(bson_iterator_string(sub));
                    class_name.append(m_class_id_name[class_id]);
                }
            }
            else if( strcmp( key, "game_code" ) == 0 )
            {
                game_code = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "default_charge_typeid" ) == 0 )
            {
                default_charge_typeid = bson_iterator_string( bson_it );
                UB_LOG_NOTICE( "default_charge_typeid[%s]", default_charge_typeid.c_str());
            }
            else
            {
                //UB_LOG_FATAL( "Unknown key[%s], [%s:%d}", key, __FILE__, __LINE__ );
            }
        }
        if( content_type_id == MCP_CONTENT_THEME )
        {
            res = get_theme_class_info( conn, id, m_class_id_name, class_id, class_name );
            if( res != MCP_OK )
            {
                UB_LOG_FATAL( "get theme class info where theme_id[%s] failed, [%s:%d]", id.c_str(), __FILE__, __LINE__  );
            }
        }
        else
        {
            //class_name = m_class_id_name[class_id];
//            res = get_class_name( conn, class_id, class_name );
//            if( res != MCP_OK )
//            {
//                UB_LOG_FATAL( "get class name where class_id[%s] failed, [%s:%d]", class_id.c_str(), __FILE__, __LINE__  );
//            }
        }

        string image_type;
        if( !img_size.empty())
        {
            int width = atoi(img_size.c_str());
            width = (width * 8) / 10;
            if( width <= 0 )
            {
                image_type = "3";
            }
            else if ( width > 160 )
            {
                image_type = "2";
            }
            else if ( width > 120 )
            {
                image_type = "3";
            }
            else if ( width > 50 )
            {
                image_type = "4";
            }
            else 
            {
                image_type = "4";
            }
        }
        else
        {
            image_type = "3";
        }
 
        res = get_img_url_by_content_id( conn, id, image_type,  image_url );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get image url failed, c_id=[%s], image_type_id=[%s], [%s:%d]",
                           id.c_str(), image_type.c_str(), __FILE__, __LINE__ );
        }

        if( !info_id.empty() && info_id.compare( "" ) != 0 )
        {
            res = get_download_basic_info( conn, id, info_type, info_id, default_charge_typeid, m_charge_id_name,
                                                       download_id, download_path, charge_type_id, charge_type_name );
            if( res != MCP_OK )
            {
                UB_LOG_FATAL( "get download basic info failed, c_id=[%s], info_type=[%d], info_id=[%s], [%s:%d]",
                               id.c_str(), info_type, info_id.c_str(), __FILE__, __LINE__ );
            }
        }

        content_info.content_id = id;
        content_info.content_name = name;
        content_info.class_id = class_id;
        content_info.class_name = class_name;
        content_info.game_code = game_code;
        content_info.wap_url = image_url;
        content_info.data_id = download_id;
        content_info.data_path = download_path;
        content_info.charge_type_id = charge_type_id;
        content_info.charge_type_name = charge_type_name;       

        content_info_list.push_back( content_info );
    }

    UB_LOG_NOTICE( "get_content_info_by_id_list end" );
    return MCP_OK;
}
*/

int get_content_count_by_classID( mongo* conn, const string class_id, uint32_t& count )
{
    UB_LOG_NOTICE( "get_content_count_by_classID start" );

    uint32_t tmp_count( 0 );
    bson query[1];

    bson_init( query );
    bson_append_string( query, "class_id", class_id.c_str() );
    bson_finish( query );

    tmp_count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_GAME_COL, query );
    count = tmp_count;
    tmp_count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_CLASS_LIST_COL, query );   
    count+=tmp_count;

    bson_destroy( query );

    UB_LOG_NOTICE( "get_content_count_by_classID end" );
    return MCP_OK;
}

int get_content_count_by_typeID( mongo* conn, const string type_id, uint32_t& count )
{
    UB_LOG_NOTICE( "get_content_count_by_typeID start" );

    bson query[1];

    bson_init( query );
    bson_append_string( query, "type_id", type_id.c_str() );
    bson_finish( query );

    count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_TYPE_COL, query );

    bson_destroy( query );
    
    UB_LOG_NOTICE( "get_content_count_by_typeID end" );
    return MCP_OK;
}


/*
int get_column_name( mongo *conn, const string column_id, string& column_name )
{
    UB_LOG_NOTICE( "get_column_name start" );

    int res = 0;

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DIY_COL ) );

    bson query, fields, bson_out;

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
        UB_LOG_FATAL( "query column_name where column_id(%s) from MongoDB %s failed", column_id.c_str(), ns.c_str() );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query column_name where column_id(%s) from MongoDB %s success", column_id.c_str(), ns.c_str() );

    bson_iterator bson_it;
    if( bson_find( &bson_it, &bson_out, "name" ) == BSON_STRING )
    {
        column_name = bson_iterator_string( &bson_it );
        UB_LOG_NOTICE( "column_name: %s", column_name.c_str() );

    }
    else
    {
        UB_LOG_FATAL( "column_name has wrong datatype column_id(%s)", column_id.c_str() );
        bson_destroy( &bson_out );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "get_content_column_name finished" );

    bson_destroy( &bson_out );
    return MCP_OK;

}
*/

int get_content_count_by_columnID( mongo* conn, const string column_id, uint32_t& count )
{
    UB_LOG_NOTICE( "get_content_count_by_columnID start" );

    bson query[1];

    bson_init( query );
    bson_append_string( query, "column_id", column_id.c_str() );
    bson_append_int( query, "enable", 1 );
    bson_finish( query );

    count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_COLUMN_COL, query );

    bson_destroy( query );

    UB_LOG_NOTICE( "get_content_count_by_columnID end" );
    return MCP_OK;
}

int get_content_charge_id_name_map( mongo* conn, map<string, string>& m_charge_id_name )
{
    UB_LOG_NOTICE( "get_content_charge_id_name_map start" );

    //int res = 0;
    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_CHARGETYPE_COL ) );

    bson query[1], fields[1];
    const bson* bson_result;

    bson_init( query );
    bson_finish( query );
    bson_init( fields );
    bson_finish( fields );

    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query charge_type from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query charge_type from MongoDB %s success", ns.c_str() );

    const char* key;
    string id, name;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        id = name = "";
        bson_iterator_init( bson_it, bson_result );
        while( bson_iterator_next( bson_it ) )
        {
            key = bson_iterator_key( bson_it );
            if( strcmp( key, "id" ) == 0 )
            {
                id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "name" ) == 0 )
            {
                name = bson_iterator_string( bson_it );
            }
            else
            {
                //UB_LOG_FATAL( "Unknown key[%s], [%s:%d]", key, __FILE__, __LINE__ );
            }
        }
        m_charge_id_name[id] = name;
    }

    UB_LOG_NOTICE( "get_content_charge_id_name_map end" );
    free_data( cursor, query, fields, NULL );
    return MCP_OK;
}

int get_content_class_id_name_map( mongo* conn, map<string, string>& m_class_id_name )
{
    UB_LOG_NOTICE( "get_content_class_id_name_map start" );

    //int res = 0;
    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_CLASS_COL ) );

    bson query[1], fields[1];
    const bson* bson_result;

    bson_init( query );
    bson_finish( query );
    bson_init( fields );
    bson_finish( fields );

    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query class info from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query class info from MongoDB %s success", ns.c_str() );

    const char* key;
    string id, name;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        id = name = "";
        bson_iterator_init( bson_it, bson_result );
        while( bson_iterator_next( bson_it ) )
        {
            key = bson_iterator_key( bson_it );
            if( strcmp( key, "id" ) == 0 )
            {
                id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "name" ) == 0 )
            {
                name = bson_iterator_string( bson_it );
            }
            else
            {
                //UB_LOG_FATAL( "Unknown key[%s], [%s:%d]", key, __FILE__, __LINE__ );
            }
        }
        m_class_id_name[id] = name;
    }

    UB_LOG_NOTICE( "get_content_class_id_name_map end" );
    free_data( cursor, query, fields, NULL );
    return MCP_OK;
}

int get_label_id_name_map( map<string,string>& m_label_id_name )
{
    UB_LOG_TRACE( "get_label_id_name_map start" );

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_LABEL_COL ) );


    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }


    bson query[1], fields[1];
    const bson* bson_result;

    bson_init( query );
    bson_finish( query );
    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "name", 1 );
    bson_finish( fields );

    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query label info from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query label info from MongoDB %s success", ns.c_str() );

    const char* key;
    string id, name;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        id = name = "";
        bson_iterator_init( bson_it, bson_result );
        while( bson_iterator_next( bson_it ) )
        {
            key = bson_iterator_key( bson_it );
            if( strcmp( key, "id" ) == 0 )
            {
                id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "name" ) == 0 )
            {
                name = bson_iterator_string( bson_it );
            }
            else
            {
                //UB_LOG_FATAL( "Unknown key[%s], [%s:%d]", key, __FILE__, __LINE__ );
            }
        }
        m_label_id_name[id] = name;
    }

    free_data( cursor, query, fields, NULL );
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_label_id_name_map end" );
    return 0;
}

int mcp_get_url_info_by_id( mongo* conn, const string& id, url_info_t& url_info )
{
    UB_LOG_NOTICE( "get_url_info_by_id start" );

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_URL_COL ) );

    int res( 0 );
    bson query[1], fields[1], bson_out[1];

    bson_init( query );
    bson_append_string( query, "id", id.c_str() );
    bson_finish( query );
    bson_init( fields );
    bson_finish( fields );

    res = mongo_find_one( conn, ns.c_str(), query, fields, bson_out );
    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "mongo_find_one failed, id[%s], ns[%s], [%s:%d]", id.c_str(), ns.c_str(), __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }

    bson_iterator bson_it[1];
    const char* key;

    bson_iterator_init( bson_it, bson_out );
    while( bson_iterator_next( bson_it ) )
    {
        key = bson_iterator_key( bson_it );
        if( strcmp( key, "id" ) == 0 )
        {
            url_info.id = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "name" ) == 0 )
        {
            url_info.name = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "class_id" ) == 0 )
        {
            url_info.class_id = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "ext_id" ) == 0 )
        {
            url_info.ext_id = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "ad" ) == 0 )
        {
            url_info.ad = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "url" ) == 0 )
        {
            url_info.url = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "baiduurl" ) == 0 )
        {
            url_info.baiduurl = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "mark" ) == 0 )
        {
            url_info.mark = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "logdate" ) == 0 )
        {
            url_info.logdate = bson_iterator_string( bson_it );
        }
        else if( strcmp( key, "modify_date" ) == 0 )
        {
            url_info.modify_date = bson_iterator_string( bson_it );
        }
        else
        {
            //UB_LOG_FATAL( "Unknown key[%s], [%s:%d]", key, __FILE__, __LINE__ );
        }
    }

    if( !url_info.class_id.empty() )
    {
        res = get_class_name( conn, url_info.class_id, url_info.class_name );
        if( res != MCP_OK )
        {
            UB_LOG_FATAL( "get_class_name failed, [%s:%d]", __FILE__, __LINE__ );
        }
    }

    free_data( NULL, query, fields, bson_out );
    UB_LOG_NOTICE( "get_url_info_by_id end" );
    return MCP_OK;
}

int mcp_get_url_count_by_column_id( mongo* conn, const string column_id, uint32_t& count )
{
    UB_LOG_NOTICE( "mcp_get_url_count_by_column_id start" );

    bson query[1];
    
    bson_init( query );
    bson_append_string( query, "column_id", column_id.c_str() );
    bson_append_int( query, "enable", 1 );
    bson_finish( query );
    
    count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_COLUMN_COL, query );
    
    bson_destroy( query );

    UB_LOG_NOTICE( "mcp_get_url_count_by_column_id end" );
    return MCP_OK;
}


int mcp_get_url_count_by_class_id( mongo* conn, const string class_id, uint32_t& count )
{
    UB_LOG_NOTICE( "mcp_get_url_count_by_class_id start" );

    bson query[1];

    bson_init( query );
    bson_append_string( query, "class_id", class_id.c_str() );
    bson_finish( query );

    count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_URL_COL, query );

    bson_destroy( query );

    UB_LOG_NOTICE( "mcp_get_url_count_by_class_id end" );
    return MCP_OK;
}

int mcp_get_url_info_by_column_id(  mongo* conn, const string column_id, const uint32_t unit_num, const uint32_t unit_time, 
                                                     vector<url_info_t>& url_list )
{
    UB_LOG_NOTICE( "mcp_get_url_info_by_column_id start"  );

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_COLUMN_COL ) );
    int res( 0 );

/*
    map<string, string> m_class_id_name;
    res = get_content_class_id_name_map( conn, m_class_id_name );
    if( res != MCP_OK )
    {
        UB_LOG_FATAL( "get_content_class_id_name_map failed, [%s:%d]", __FILE__, __LINE__ );
    }
*/

    bson query[1], fields[1];
    const bson* bson_result;

    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "column_id", column_id.c_str() );
    bson_append_int( query, "enable", 1 );
    bson_append_finish_object( query );
    bson_append_start_object( query, "$orderby" );
    bson_append_int( query, "priority", -1 );
    bson_append_finish_object( query );
    bson_finish( query );
    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_finish( fields );
    
    uint32_t start_pos = unit_num * ( unit_time - 1 );
    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query class info from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query class info from MongoDB %s success", ns.c_str() );

    const char* key;
    url_info_t url_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        url_info.clear();
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        bson_iterator_init( bson_it, bson_result );
        while( bson_iterator_next( bson_it ) )
        {
            key = bson_iterator_key( bson_it );
            if( strcmp( key, "c_id" ) == 0 )
            {
                url_info.id = bson_iterator_string( bson_it );
                res = mcp_get_url_info_by_id( conn, url_info.id, url_info );
                if( res != MCP_OK )
                {
                    UB_LOG_FATAL( "mcp_get_url_info_by_id failed, [%s:%d]", __FILE__, __LINE__ );
                    break;
                }
            }
            else
            {
            }
        }
//        memset( &url_info, 0x00, sizeof( url_info ) );
/*
        url_info.clear();
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        bson_iterator_init( bson_it, bson_result );
        while( bson_iterator_next( bson_it ) )
        {
            key = bson_iterator_key( bson_it );
            if( strcmp( key, "id" ) == 0 )
            {
                url_info.id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "name" ) == 0 )
            {
                url_info.name = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "class_id" ) == 0 )
            {
                url_info.class_id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "ext_id" ) == 0 )
            {
                url_info.ext_id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "ad" ) == 0 )
            {
                url_info.ad = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "url" ) == 0 )
            {
                url_info.url = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "baiduurl" ) == 0 )
            {
                url_info.baiduurl = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "mark" ) == 0 )
            {
                url_info.mark = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "logdate" ) == 0 )
            {
                url_info.logdate = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "modify_date" ) == 0 )
            {
                url_info.modify_date = bson_iterator_string( bson_it );
            }
            else
            {
            //UB_LOG_FATAL( "Unknown key[%s], [%s:%d]", key, __FILE__, __LINE__ );
            }
        }
        url_info.class_name = m_class_id_name[url_info.class_id];
*/
        url_list.push_back( url_info );
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_NOTICE( "mcp_get_url_info_by_ext_id end" );
    return MCP_OK;
}
int mcp_get_url_info_by_class_id(  mongo* conn, const string class_id, const uint32_t unit_num, const uint32_t unit_time, 
                                                      vector<url_info_t>& url_list )
{
    UB_LOG_NOTICE( "mcp_get_url_info_by_class_id start" );

    const string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_URL_COL ) );
    int res( 0 );

    string class_name;
    res = get_class_name( conn, class_id, class_name );
    if( res != MCP_OK )
    {
        UB_LOG_FATAL( "get_class_name failed, [%s:%d]", __FILE__, __LINE__ );
    }

    bson query[1], fields[1];
    const bson* bson_result;

    bson_init( query );
    bson_append_string( query, "class_id", class_id.c_str() );
    bson_finish( query );
    bson_init( fields );
    bson_finish( fields );

    uint32_t start_pos = unit_num * ( unit_time - 1 );
    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query class info from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    UB_LOG_NOTICE( "query class info from MongoDB %s success", ns.c_str() );

    const char* key;
    url_info_t url_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        url_info.clear();
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        bson_iterator_init( bson_it, bson_result );
        while( bson_iterator_next( bson_it ) )
        {
            key = bson_iterator_key( bson_it );
            if( strcmp( key, "id" ) == 0 )
            {
                url_info.id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "name" ) == 0 )
            {
                url_info.name = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "class_id" ) == 0 )
            {
                url_info.class_id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "ext_id" ) == 0 )
            {
                url_info.ext_id = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "ad" ) == 0 )
            {
                url_info.ad = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "url" ) == 0 )
            {
                url_info.url = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "baiduurl" ) == 0 )
            {
                url_info.baiduurl = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "mark" ) == 0 )
            {
                url_info.mark = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "logdate" ) == 0 )
            {
                url_info.logdate = bson_iterator_string( bson_it );
            }
            else if( strcmp( key, "modify_date" ) == 0 )
            {
                url_info.modify_date = bson_iterator_string( bson_it );
            }
            else
            {
            //UB_LOG_FATAL( "Unknown key[%s], [%s:%d]", key, __FILE__, __LINE__ );
            }
        }
        url_info.class_name = class_name;
        url_list.push_back( url_info );
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_NOTICE( "mcp_get_url_info_by_class_id end" );
    return MCP_OK;
}

int get_download_info_by_c_id_list( mongo* conn, const string& info_id, const uint32_t info_type, 
          const vector<string>& c_id_list, map<string, string>& m_charge_id_name, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_download_info_by_c_id_list start" );

    string format_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_FORMAT_COL ) );
    string data_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DATA_COL ) );
    bson query[1], fields[1];
    string id;
    map<string, vector<string> > m_id_data_id;
    char index[8];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    string data_id, path_url, charge_id, charge_name;
    int res( 0 );
    char *tmp = NULL;
    char stype_id_list[1024];
    memset( stype_id_list, 0x00, sizeof( stype_id_list ) );
    char *p_stype_id = stype_id_list;

    vector<string>::const_iterator id_iter;

    char c_info_type[8];
    memset( c_info_type, 0x00, sizeof( c_info_type ) );
    sprintf( c_info_type, "%d", info_type );

    if( !info_id.empty() && info_id.compare( "" ) != 0 )
    {
        if( info_type != 1 )
        {
            bson_init( query );
            bson_append_start_object( query, "c_id" );
            bson_append_start_array( query, "$in" );
            vector<string>::const_iterator iter = c_id_list.begin();
            for( int j=0; iter != c_id_list.end(); ++iter,++j )
            {
                memset( index, 0x00, sizeof( index ) );
                sprintf( index, "%d", j );
                bson_append_string( query, index, iter->c_str() );
            }
            bson_append_finish_object( query );
            bson_append_finish_object( query );
            bson_append_string( query, "stype",  c_info_type );
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
            bson_finish( query );

            bson_init( fields );
            bson_append_int( fields, "c_id", 1 );
            bson_append_int( fields, "cdata_id", 1 );
            bson_finish( fields );

            cursor = mongo_find( conn, format_ns.c_str(), query, fields, 0, 0, 0 );
            if( cursor == NULL )
            {
                UB_LOG_FATAL( "query charge_type from MongoDB %s failed", format_ns.c_str() );
                free_data( NULL, query, fields, NULL );
                return MCP_NG;
            }
            while( mongo_cursor_next( cursor ) == MONGO_OK )
            {
                id.clear();
                data_id.clear();
                bson_iterator it[1];
                result = mongo_cursor_bson( cursor );
                bson_iterator_init( it, result);
                while( bson_iterator_next(it) )
                {
                    key = bson_iterator_key(it);
                    if( 0 == strcmp(key,"c_id"))
                    {
                        id = bson_iterator_string( it );
                    }
                    else if( 0 == strcmp(key,"cdata_id"))
                    {
                        data_id = bson_iterator_string( it );
                    }
                    else
                    {
                        continue;
                    }
                }
                m_id_data_id[id].push_back( data_id );
                UB_LOG_NOTICE( "m_id_data_id[%s] push %s", id.c_str(), data_id.c_str() );
            } 
            free_data( cursor, query, fields, NULL );

        }
        else
        {
            id_iter = c_id_list.begin();
            while( id_iter != c_id_list.end() )
            {
                res = get_cdata_id_by_type( conn, *id_iter, c_info_type, info_id, m_id_data_id[*id_iter] );
                if( res == -5 )
                {
                    UB_LOG_FATAL( "get_cdata_id_by_type failed , [%s:%d]", __FILE__, __LINE__ );
                    return -5;
                }
                ++id_iter;
            }
        }
    }

    vector<string> data_id_list;
    int j;
    vector<string>::iterator data_iter;
    vector<content_info_t>::iterator content_it( content_info_list.begin() );
    while( content_it != content_info_list.end() )
    {
        id = content_it->id;
        data_id_list.clear();
        data_id_list = m_id_data_id[id];
        bson_init( query );
        bson_append_start_object(query,"id");
        bson_append_start_array(query,"$in");
        for( data_iter = data_id_list.begin(), j = 0; data_iter != data_id_list.end(); ++data_iter, ++j)
       {
            memset( index, 0x00, sizeof( index ) );
            sprintf(index,"%d", j);
            bson_append_string( query, index, data_iter->c_str() );
            UB_LOG_NOTICE("cdata id %d is %s", j, data_iter->c_str() );
        }
        bson_append_finish_object( query );
        bson_append_finish_object( query );
        bson_finish(query);
        //设置返回域
        bson_init(fields);
        bson_append_int(fields, "id", 1);
        bson_append_int(fields, "path_url", 1);
        bson_append_int(fields, "charge_typeid", 1);
        bson_finish(fields);

        cursor = mongo_find( conn, data_ns.c_str(), query, fields, 0, 0, 0 );
        if( cursor == NULL )
        {
            UB_LOG_FATAL( "query charge_type from MongoDB %s failed", data_ns.c_str() );
            free_data( NULL, query, fields, NULL );
            content_it->charge_id = content_it->dft_charge_id ;
            content_it->charge_name = m_charge_id_name[content_it->charge_id];
            continue;
        }
        while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
            data_id.clear();
            bson_iterator it[1];
            result = mongo_cursor_bson( cursor );
            bson_iterator_init( it, result);
            while( bson_iterator_next(it) )
            {
                key = bson_iterator_key(it);
                if( 0 == strcmp(key,"id"))
                {
                    data_id = bson_iterator_string( it );
                }
                else if( 0 == strcmp(key,"path_url"))
                {
                    path_url = bson_iterator_string( it );
                }
                else if( 0 == strcmp(key,"charge_typeid"))
                {
                    charge_id = bson_iterator_string( it );
                    charge_name  = m_charge_id_name[charge_id];
                }
                else
                {
                    continue;
                }
            }
            if( !content_it->down_id.empty() || content_it->down_id.compare( "" ) != 0 )
            {
                content_it->down_id.append( "," );
                content_it->down_url.append( "," );
                content_it->charge_id.append( "," );
                content_it->charge_name.append( "," );
            }
            content_it->down_id.append( data_id );
            content_it->down_url.append( path_url );
            if( charge_id.empty() || charge_id.compare( "" ) == 0 )
            {
                charge_id = content_it->dft_charge_id;
                //charge_name = content_it->dft_charge_name;
                charge_name = m_charge_id_name[charge_id];
            }
            content_it->charge_id.append( charge_id );
            content_it->charge_name.append( charge_name );

            UB_LOG_NOTICE( "m_id_data_id[%s] push %s", id.c_str(), data_id.c_str() );
        }
        if( content_it->charge_id.empty() || content_it->charge_id.compare( "" )==0 )
        {
            content_it->charge_id = content_it->dft_charge_id;
            content_it->charge_name = m_charge_id_name[content_it->charge_id];
        }
        free_data( cursor, query, fields, NULL );
        ++content_it;
    }

    UB_LOG_TRACE( "get_download_info_by_c_id_list end" );
    return 0;
}

int get_image_info_by_c_id_list( mongo* conn, const string& img_type, 
                                 vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_image_info_by_c_id_list start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_PREVIEW_COL ) );
    bson query[1], fields[1];
    string id, image_url;
    map<string, string> m_id_img_url;
    char index[8];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;

    bson_init( query );
    bson_append_start_object( query, "c_id" );
    bson_append_start_array( query, "$in" );
    vector<content_info_t>::const_iterator iter = content_info_list.begin();
    for( int j=0; iter != content_info_list.end(); ++iter,++j )
    {
        memset( index, 0x00, sizeof( index ) );
        sprintf( index, "%d", j );
        bson_append_string( query, index, iter->id.c_str() );
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_append_string( query, "image_type_id", img_type.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_append_int( fields, "image_url", 1 );
    bson_finish( fields );
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query charge_type from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        id.clear();
        image_url.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"c_id"))
            {
                id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"image_url"))
            {
                image_url = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
	if( m_id_img_url[id].empty() || m_id_img_url[id].compare( "" ) == 0 )
        {
            m_id_img_url[id] = image_url;
        }
        UB_LOG_NOTICE( "m_id_img_url[%s] = %s", id.c_str(), m_id_img_url[id].c_str() );
    }

    vector<content_info_t>::iterator iter1 = content_info_list.begin();
    while( iter1 != content_info_list.end() )
    {
        iter1->wap_url = m_id_img_url[iter1->id];
        ++iter1;
    }

    UB_LOG_TRACE( "get_image_info_by_c_id_list end" );
    free_data( cursor, query, fields, NULL );
    return 0;
}

int get_theme_name_by_id_list( mongo* conn, const vector<string>& c_id_list, 
                         map<string, string>& m_charge_id_name, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_theme_name_by_id_list start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL ) );
    bson query[1], fields[1];
    string id, name, default_charge_id;
    map<string, string> m_id_name;
    map<string, string> m_id_charge;
    char index[8];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;

    bson_init( query );
    bson_append_start_object( query, "id" );
    bson_append_start_array( query, "$in" );
    vector<string>::const_iterator iter = c_id_list.begin();
    for( int j=0; iter != c_id_list.end(); ++iter,++j )
    {
        memset( index, 0x00, sizeof( index ) );
        sprintf( index, "%d", j );
        bson_append_string( query, index, iter->c_str() );
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "name", 1 );
    bson_append_int( fields, "default_charge_typeid", 1 );
    bson_finish( fields );
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query charge_type from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        return MCP_NG;
    }
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        id.clear();
        name.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id"))
            {
                id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"name"))
            {
                name = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"default_charge_typeid"))
            {
                default_charge_id = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
        m_id_name[id] = name;
        UB_LOG_NOTICE( "m_id_name[%s] = %s", id.c_str(), m_id_name[id].c_str() );
        m_id_charge[id] = default_charge_id;
        UB_LOG_NOTICE( "m_id_charge[%s] = %s", id.c_str(), m_id_charge[id].c_str() );
    }

    vector<content_info_t>::iterator iter1 = content_info_list.begin();
    while( iter1 != content_info_list.end() )
    {
        iter1->name = m_id_name[iter1->id];
        iter1->dft_charge_id = m_id_charge[iter1->id];
        iter1->dft_charge_name = m_charge_id_name[iter1->dft_charge_id];
        ++iter1;
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_theme_name_by_id_list end" );
    return 0;
}

int get_content_id_list_by_class_id( mongo* conn, const string& content_type_id, const string& class_id,
                                     const uint32_t priority, const uint32_t unit_num, const uint32_t unit_times,
                                     const string& info_id, const uint32_t info_type, const string& img_size,
                                     uint32_t& count, vector<content_info_t>& content_info_list)
{
    UB_LOG_TRACE( "get_content_id_list_by_class_id start" );
    string c_id;
    const char* key;
    bson query[1];
    bson fields[1];
    const bson *result;
    string coll;
    string ns;
    int res( 0 );
    string theme_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL ) );

    vector<string> c_id_list;
    content_info_t content_info;

    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );

    if( content_type_id == MCP_CONTENT_GAME )
    {
        //设置查询条件
        bson_init(query);
        //TODO query append content type
        bson_append_string( query, "content_type", content_type_id.c_str() );
        if( !class_id.empty() && class_id.compare( "" ) != 0 )
        {
            bson_append_string(query,"class_id",class_id.c_str());
        }
        bson_finish(query);
        coll = MCP_MONGO_GAME_COL;
    }
    else if( content_type_id == MCP_ANDROID_GAME )
    {
        bson_init(query); 
        bson_append_string( query, "content_type", content_type_id.c_str() );
        if( !class_id.empty() && class_id.compare( "" ) != 0 )
        {
            bson_append_string(query,"class_id",class_id.c_str());
        }
        bson_finish(query);
        coll = MCP_MONGO_GAME_COL;
    }
    else if( content_type_id == MCP_CONTENT_THEME )
    {
        //设置查询条件
        bson_init(query);
        bson_append_string(query,"class_id",class_id.c_str());
        bson_append_int( query, "enable", 1 );
        bson_finish(query);
        coll = MCP_MONGO_CLASS_LIST_COL;
    }
    else
    {
        UB_LOG_FATAL( "Unknown content_type_id[%s], [%s:%d]", content_type_id.c_str(), __FILE__, __LINE__ );
        return 0;
    }

    int64_t m_count;
    m_count = mongo_count( conn, MCP_MONGODB_NAME, coll.c_str(), query );
    bson_destroy( query );
    if( m_count == 0 )
    {
        return 0;
    }
    else if( m_count <  0 )
    {
        UB_LOG_FATAL( "mongo_count failed, [%s:%d|", __FILE__, __LINE__ );
        return -1;
    }

    count = (uint32_t)m_count;
    ns = get_mongo_namespace( MCP_MONGODB_NAME, coll );

    int32_t start_pos = unit_num * ( unit_times - 1 );
    if( start_pos < 0 || start_pos >=  (int)count )
    {
       UB_LOG_FATAL( "start_pos[%d], count[%d], [%s:%d}", start_pos, count, __FILE__, __LINE__ );
        return -1;
    }
    mongo_cursor* cursor = NULL;
    
    if( content_type_id == MCP_CONTENT_GAME || content_type_id == MCP_ANDROID_GAME )
    {
        //设置查询条件
        bson_init(query);
        bson_append_start_object( query, "$query" );
        bson_append_string( query, "content_type", content_type_id.c_str() );
        if( !class_id.empty() && class_id.compare( "" ) != 0 )
        {
            bson_append_string(query,"class_id",class_id.c_str());
        }
        bson_append_finish_object( query );
        if( priority == 1 )
        {
            bson_append_start_object( query, "$orderby" );
            bson_append_int( query, "priority", -1 );
            bson_append_finish_object( query );
        }
        else
        {
            bson_append_start_object( query, "$orderby" );
            bson_append_int( query, "logdate", -1 );
            bson_append_finish_object( query );
        }
        bson_finish(query);
        //设置返回域
        bson_init(fields);
        bson_append_int(fields,"id",1);
        bson_append_int(fields,"name",1);
        bson_append_int(fields,"game_code",1);
        bson_append_int(fields, "default_charge_typeid", 1 );
        bson_finish(fields);
        cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
        if( cursor == NULL )
        {
            UB_LOG_FATAL( "query charge_type from MongoDB %s failed", ns.c_str() );
            free_data( NULL, query, fields, NULL );
            return MCP_NG;
        }
        c_id_list.clear();
        while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
            content_info.clear();
            bson_iterator it[1];
            result = mongo_cursor_bson( cursor );
            bson_iterator_init( it, result);
            while( bson_iterator_next(it) )
            {
                key = bson_iterator_key(it);
                if( 0 == strcmp(key,"id"))
                {
                    content_info.id = bson_iterator_string( it );
                    c_id_list.push_back( content_info.id );
                }
                else if( 0 == strcmp(key,"name"))
                {
                    content_info.name = bson_iterator_string( it );
                }
                else if( 0 == strcmp(key,"game_code"))
                {
                    content_info.code = bson_iterator_string( it );
                }
                else if( 0 == strcmp(key,"default_charge_typeid"))
                {
                    content_info.dft_charge_id = bson_iterator_string( it );
                    content_info.dft_charge_name = m_charge_id_name[content_info.dft_charge_id];
                }
                else
                {
                    continue;
                }
            }
            content_info_list.push_back( content_info );
        }
        free_data( cursor, query, fields, NULL );
    }
    else
    {
        if( priority != 2 )
        {
            bson_init(query);

            bson_append_start_object( query, "$query" );
            bson_append_string(query,"class_id",class_id.c_str());
            bson_append_int( query, "enable", 1 );
            bson_append_finish_object( query );

            bson_append_start_object( query, "$orderby" );
            bson_append_int( query, "priority", -1 );
            bson_append_finish_object( query );

            bson_finish(query);

            //设置返回域
            bson_init(fields);
            bson_append_int( fields, "c_id", 1 );
            bson_finish(fields);

            cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
            if( cursor == NULL )
            {
                UB_LOG_FATAL( "query c_id list from MongoDB %s failed [%s:%d]", ns.c_str(), __FILE__, __LINE__ );
                free_data( NULL, query, fields, NULL );
                return MCP_NG;
            }
            while( mongo_cursor_next( cursor ) == MONGO_OK )
            {
                content_info.clear();
                result = mongo_cursor_bson( cursor );
                bson_iterator bson_it[1];
                if( bson_find( bson_it, result, "c_id" ) == BSON_STRING )
                {
                    c_id_list.push_back( bson_iterator_string( bson_it ) );
                    content_info.id = bson_iterator_string( bson_it );
                    content_info_list.push_back( content_info );
                }
                else
                {
                    continue;
                }
            }
            free_data( cursor, query, fields, NULL );
            res = get_theme_name_by_id_list( conn, c_id_list, m_charge_id_name, content_info_list );
            if( res == -5 )
            {
                UB_LOG_FATAL( "get_theme_name_by_id_list failed, [%s:%d]", __FILE__, __LINE__ );
                return -5;
            }
            else if( res != 0 )
            {
                UB_LOG_NOTICE( "get_theme_name_by_id_list failed, [%s:%d]", __FILE__, __LINE__ );
                return -1;
            }
        }
        else
        {
            bson_init(query);
            bson_append_string(query,"class_id",class_id.c_str());
            bson_append_int( query, "enable", 1 );
            bson_finish(query);

            //设置返回域
            bson_init(fields);
            bson_append_int( fields, "c_id", 1 );
            bson_finish(fields);

            cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
            if( cursor == NULL )
            {
                UB_LOG_FATAL( "query c_id list from MongoDB %s failed", ns.c_str() );
                free_data( NULL, query, fields, NULL );
                return MCP_NG;
            }
            while( mongo_cursor_next( cursor ) == MONGO_OK )
            {
                result = mongo_cursor_bson( cursor );
                bson_iterator bson_it[1];
                if( bson_find( bson_it, result, "c_id" ) == BSON_STRING )
                {
                    c_id_list.push_back( bson_iterator_string( bson_it ) );
                }
                else
                {
                    continue;
                }
            }
            free_data( cursor, query, fields, NULL );

            char index[8];

            bson_init(query);

            bson_append_start_object( query, "$query" );
            bson_append_start_object( query, "id" );
            bson_append_start_array( query, "$in" );
            vector<string>::iterator iter = c_id_list.begin();
            for( int j=0; iter != c_id_list.end(); ++iter,++j )
            {
                memset( index, 0x00, sizeof( index ) );
                sprintf( index, "%d", j );
                bson_append_string( query, index, iter->c_str() );
            }
            bson_append_finish_object( query );
            bson_append_finish_object( query );
            bson_append_finish_object( query );

            bson_append_start_object( query, "$orderby" );
            bson_append_int( query, "logdate", -1 );
            bson_append_finish_object( query );

            bson_finish(query);
            //设置返回域
            bson_init(fields);
            bson_append_int(fields,"id",1);
            bson_append_int(fields,"name",1);
            bson_append_int(fields,"default_charge_typeid",1);
            bson_finish(fields);
            cursor = mongo_find( conn, theme_ns.c_str(), query, fields, unit_num, start_pos, 0 );
            if( cursor == NULL )
            {
                UB_LOG_FATAL( "query theme_info from MongoDB %s failed", theme_ns.c_str() );
                free_data( NULL, query, fields, NULL );
                return MCP_NG;
            }
            c_id_list.clear();
            while( mongo_cursor_next( cursor ) == MONGO_OK )
            {
                content_info.clear();
                bson_iterator it[1];
                result = mongo_cursor_bson( cursor );
                bson_iterator_init( it, result);
                while( bson_iterator_next(it) )
                {
                    key = bson_iterator_key(it);
                    if( 0 == strcmp(key,"id"))
                    {
                        content_info.id = bson_iterator_string( it );
                        c_id_list.push_back( content_info.id );
                    }
                    else if( 0 == strcmp(key,"name"))
                    {
                        content_info.name = bson_iterator_string( it );
                    }
                    else if( 0 == strcmp(key,"default_charge_typeid"))
                    {
                        content_info.dft_charge_id = bson_iterator_string( it );
                        content_info.dft_charge_name = m_charge_id_name[content_info.dft_charge_id];
                    }
                    else
                    {
                        continue;
                    }
                }
                content_info_list.push_back( content_info );
            }
            free_data( cursor, query, fields, NULL );
        }
    }

    if( c_id_list.empty() )
    {
        bson_destroy(query);
        bson_destroy(fields);
        //mongo_cursor_destroy(cursor);
        return -1;
    }

    res = get_download_info_by_c_id_list( conn, info_id, info_type, c_id_list, m_charge_id_name, content_info_list );
    if( res == -5 )
    {
        UB_LOG_FATAL( "get_download_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        bson_destroy(query);
        bson_destroy(fields);
        mongo_cursor_destroy(cursor);
        return -5;
    }

    string image_type;
    if( !img_size.empty() )
    {
        int width = atoi( img_size.c_str() );
        width = (width*8)/10;
        if( width < 0 )
        {
            image_type = "3";
        }
        else if( width > 160 )
        {
            image_type = "2";
        }
        else if( width > 120 )
        {
            image_type = "3";
        }
        else
        {
            image_type = "4";
        }
    }
    else
    {
        image_type = "3";
    }

    res = get_image_info_by_c_id_list( conn, image_type, content_info_list );
    if( res == -5 )
    {
        UB_LOG_FATAL( "get_image_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    UB_LOG_TRACE( "get_content_id_list_by_class_id end" );
    return 0;
}


int get_content_list_by_phone_id(mongo* conn, const string p_id, const uint32_t priority, const uint32_t type, const uint32_t unit_num, const uint32_t unit_times, int64_t *count, vector<content_info_t>* content_info_list)
{

    if( unit_num == 0 || unit_times == 0 )
    {
        UB_LOG_FATAL( "The unit_num or unit_times is invalid " );
        return MCP_NG;
    }
    uint32_t start_pos( 0 );
    start_pos = unit_num * ((int)unit_times - 1 );
    map<string, string> c_id_data;
    vector<string> cid_tmp;

    content_info_t tmp;
    bson query0[1];
    bson_init(query0);
    bson_append_string(query0, "stype", "1");
    bson_append_string(query0, "stype_id", p_id.c_str() );
    bson_finish(query0);

    *count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_FORMAT_COL, query0 );
    
    bson fields0[1];
    bson_init( fields0 );
    bson_append_int( fields0, "c_id", 1 );
    bson_append_int( fields0, "cdata_id", 1 );
    bson_finish( fields0 );

    mongo_cursor cursor0[1];
    mongo_cursor_init(cursor0, conn, "mcp.mcp_content_format");
    
    mongo_cursor_set_query( cursor0, query0 );
    mongo_cursor_set_fields( cursor0, fields0 );
    while( mongo_cursor_next( cursor0 ) == MONGO_OK )
    {
        bson_iterator iterator0[1];
        if(bson_find(iterator0, mongo_cursor_bson(cursor0), "c_id"))
        {
            cid_tmp.push_back(bson_iterator_string(iterator0));
            tmp.id = bson_iterator_string(iterator0);
        }
        if(bson_find(iterator0, mongo_cursor_bson(cursor0), "cdata_id"))
        {
            tmp.down_id = bson_iterator_string(iterator0);
            c_id_data.insert(map<string,string>::value_type(tmp.id, tmp.down_id));
        }
    }
    bson_destroy(query0);
    bson_destroy(fields0);
    mongo_cursor_destroy(cursor0);

    char index[8];
    bson query1[1];
    bson_init(query1);
    bson_append_start_object( query1, "$query" );
    bson_append_start_object( query1, "id" );
    bson_append_start_array( query1, "$in" );
    vector<string>::iterator iter = cid_tmp.begin();
    for( int j=0; iter != cid_tmp.end(); ++iter,++j )
    {
        memset( index, 0x00, sizeof( index ) );
        sprintf( index, "%d", j );
        bson_append_string( query1, index, iter->c_str() );
    }
    bson_append_finish_object( query1 );
    bson_append_finish_object( query1 );
    bson_append_finish_object( query1 );
    bson_append_start_object( query1, "$orderby" );
    if(priority == 1)
        bson_append_int( query1, "star", -1 );
    else
        bson_append_int( query1, "logdate", -1 );
    bson_append_finish_object( query1 );
    bson_finish(query1);

    bson fields1[1];
    bson_init( fields1 );
    bson_append_int( fields1, "id", 1 );
    bson_append_int( fields1, "name", 1 );
    bson_append_int( fields1, "default_charge_typeid", 1 );
    bson_append_int( fields1, "default_charge_value", 1 );
    bson_finish( fields1 );

    mongo_cursor cursor1[1];
    if(type == 1)
        mongo_cursor_init(cursor1, conn, "mcp.mcp_theme_content");
    else
    {
        bson_destroy(query1);
        bson_destroy(fields1);
        return 1;
    }
    mongo_cursor_set_query( cursor1, query1 );
    mongo_cursor_set_fields( cursor1, fields1 );
    mongo_cursor_set_limit( cursor1, unit_num );
    mongo_cursor_set_skip( cursor1, start_pos );
    while( mongo_cursor_next( cursor1 ) == MONGO_OK )
    {
        tmp.clear();

        bson_iterator iterator1[1];
        if(bson_find(iterator1, mongo_cursor_bson(cursor1), "id"))
        {    
            tmp.id = bson_iterator_string(iterator1);

            bson query2[1];
            bson_init(query2);
            bson_append_string(query2, "c_id", tmp.id.c_str() );
            bson_finish(query2);

            bson fields2[1];
            bson_init( fields2 );
            bson_append_int( fields2, "image_url", 1 );
            bson_finish( fields2 );

            mongo_cursor cursor2[1];
            mongo_cursor_init(cursor2, conn, "mcp.mcp_content_preview");
            mongo_cursor_set_query( cursor2, query2 );
            mongo_cursor_set_fields( cursor2, fields2 );
            while( mongo_cursor_next( cursor2 ) == MONGO_OK )
            {
                bson_iterator iterator2[1];
                if(bson_find(iterator2, mongo_cursor_bson(cursor2), "image_url"))
                {
                    tmp.wap_url = bson_iterator_string(iterator2);
                }
            }
            bson_destroy(query2);
            bson_destroy(fields2);
            mongo_cursor_destroy(cursor2);

            bson query3[1];
            bson_init(query3);
            bson_append_string(query3, "c_id", tmp.id.c_str() );
            bson_append_int( query3, "enable", 1 );
            bson_finish(query3);

            bson fields3[1];
            bson_init( fields3 );
            bson_append_int( fields3, "class_id", 1 );
            bson_finish( fields3 );

            mongo_cursor cursor3[1];
            mongo_cursor_init(cursor3, conn, "mcp.mcp_content_class_list");
            mongo_cursor_set_query( cursor3, query3 );
            mongo_cursor_set_fields( cursor3, fields3 );
            int i = 0;
            while( mongo_cursor_next( cursor3 ) == MONGO_OK )
            {
                bson_iterator iterator3[1];
                if(bson_find(iterator3, mongo_cursor_bson(cursor3), "class_id"))
                {
                    char classid[8];
                    strlcpy(classid, bson_iterator_string(iterator3), sizeof(classid));
                    if( i==0 )
                        tmp.class_id = bson_iterator_string(iterator3);
                    else
                        tmp.class_id = tmp.class_id + "," + bson_iterator_string(iterator3);

                    bson query4[1];
                    bson_init(query4);
                    bson_append_string(query4, "id", classid );
                    bson_finish(query4);

                    bson fields4[1];
                    bson_init( fields4 );
                    bson_append_int( fields4, "name", 1 );
                    bson_finish( fields4 );

                    mongo_cursor cursor4[1];
                    mongo_cursor_init(cursor4, conn, "mcp.mcp_content_class");
                    mongo_cursor_set_query( cursor4, query4 );
                    mongo_cursor_set_fields( cursor4, fields4 );
                    while( mongo_cursor_next( cursor4 ) == MONGO_OK )
                    {
                        bson_iterator iterator4[1];
                        if(bson_find(iterator4, mongo_cursor_bson(cursor4), "name"))
                        {
                            if(i == 0)
                                tmp.class_name = bson_iterator_string(iterator4);
                            else
                                tmp.class_name = tmp.class_name + "," + bson_iterator_string(iterator4);
                        }
                    }
                    bson_destroy(query4);
                    bson_destroy(fields4);
                    mongo_cursor_destroy(cursor4);
                }
                i++;
            }
            bson_destroy(query3);
            bson_destroy(fields3);
            mongo_cursor_destroy(cursor3);

            tmp.down_id = c_id_data[tmp.id];

            bson query5[1];
            bson_init(query5);
            bson_append_string(query5, "id", tmp.down_id.c_str());
            bson_finish(query5);

            bson fields5[1];
            bson_init( fields5 );
            bson_append_int( fields5, "path_url", 1 );
            bson_append_int( fields5, "charge_typeid", 1 );
            bson_append_int( fields5, "value", 1 );
            bson_finish( fields5 );

            mongo_cursor cursor5[1];
            mongo_cursor_init(cursor5, conn, "mcp.mcp_content_data");
            mongo_cursor_set_query( cursor5, query5 );
            mongo_cursor_set_fields( cursor5, fields5 );
            while( mongo_cursor_next( cursor5 ) == MONGO_OK )
            {
                bson_iterator iterator5[1];
                if(bson_find(iterator5, mongo_cursor_bson(cursor5), "path_url"))
                {
                    tmp.down_url = bson_iterator_string(iterator5);
                }
                if(bson_find(iterator5, mongo_cursor_bson(cursor5), "charge_typeid"))
                {
                    tmp.charge_id = bson_iterator_string(iterator5);
                }
                if(bson_find(iterator5, mongo_cursor_bson(cursor5), "value"))
                {
                    char tmpstr[6];
                    sprintf(tmpstr,"%d",bson_iterator_int(iterator5));
                    tmp.charge_value = tmpstr;
                }
            }
            bson_destroy(query5);
            bson_destroy(fields5);
            mongo_cursor_destroy(cursor5);
        }
        if(bson_find(iterator1, mongo_cursor_bson(cursor1), "name"))
        {
            tmp.name = bson_iterator_string(iterator1);
        }
        if(bson_find(iterator1, mongo_cursor_bson(cursor1), "default_charge_typeid"))
        {
            tmp.charge_id = bson_iterator_string(iterator1);
        }
        if(bson_find(iterator1, mongo_cursor_bson(cursor1), "default_charge_value"))
        {
            tmp.charge_value = bson_iterator_string(iterator1);
        }
        content_info_list->push_back(tmp);
    }
    bson_destroy(query1);
    bson_destroy(fields1);
    mongo_cursor_destroy(cursor1);
    cid_tmp.clear();
    c_id_data.clear();

    return 0;
}

int get_adapt_count_by_class_id( const vector<string>& class_id, const string& info_id, const uint32_t info_type, 
                                       vector<int>& count )
{
    UB_LOG_TRACE( "get_adapt_count_by_class_id start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_ADAPT_COL ) );
    bson query[1], fields[1];
    string tmp_class_id;
    map<string, int> m_class_count;
    char index[8];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    vector<string>::const_iterator iter = class_id.begin();
    int tmp_count = 0;
    char c_info_type[8];
    memset( c_info_type, 0x00, sizeof( c_info_type ) );
    sprintf( c_info_type, "%d", info_type );

    bson_init(query);
    bson_append_start_object( query, "class_id" );
    bson_append_start_array( query, "$in" );
    iter = class_id.begin();
    for( int j=0; iter != class_id.end(); ++iter,++j )
    {
        memset( index, 0x00, sizeof( index ) );
        sprintf( index, "%d", j );
        bson_append_string( query, index, iter->c_str() );
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    if( !info_id.empty() && info_id.compare( "" ) != 0 )
    {
        bson_append_int(query,"stype", info_type );
        bson_append_string(query,"stype_id", info_id.c_str());
    }
    else
    {
        bson_append_int(query,"stype", 0 );
        bson_append_string(query,"stype_id", "0" ); 
    }
    bson_finish(query);

    //设置返回域
    bson_init(fields);
    bson_append_int( fields, "class_id", 1 );
    bson_append_int( fields, "count", 1 );
    bson_finish(fields);

    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query c_id list from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return MCP_NG;
    }
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_class_id.clear();
        tmp_count = 0;
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"class_id"))
            {
                tmp_class_id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"count"))
            {
                tmp_count = bson_iterator_int( it );
            }
            else
            {
                continue;
            }
        }
        m_class_count[tmp_class_id] = tmp_count;
        UB_LOG_NOTICE( "m_class_count[%s] = %d", tmp_class_id.c_str(), tmp_count );
    }
    for( iter = class_id.begin(); iter != class_id.end(); ++iter )
    {
        count.push_back( m_class_count[*iter] );
    }

    free_data( cursor, query, fields, NULL );
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_adapt_count_by_class_id end" );
    return 0;
}

/*
int get_c_id_list_by_class( const string& content_type, const string& class_id, vector<string>& c_id_list )
{
    UB_LOG_TRACE( "get_c_id_list_by_class start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_CLASS_LIST_COL ) );
    bson query[1], fields[1];
    const bson* bson_result;
    mongo *conn;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson_init( query );
    bson_append_string( query, "class_id", class_id.c_str() ); 
    bson_append_int( query, "enable", 1 );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_finish( fields );
    
    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );

    if( cursor == NULL )
    {   
        UB_LOG_FATAL( "query c_id where class_id(%s) from MongoDB %s failed", class_id.c_str(), ns.c_str() );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return MCP_NG;
    }

    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        if( bson_find( bson_it, bson_result, "c_id" ) == BSON_STRING )
        {
            c_id_list.push_back(bson_iterator_string(bson_it));
        }
    }

    free_data( cursor, query, fields, NULL );
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_c_id_list_by_class end" );
    return 0;
}
*/

int get_adapt_content_by_c_id_list( const vector<string>& info_id_list, const uint32_t info_type, const vector<string>& c_id_list, 
                                  set<string>& c_id_set )
{
    UB_LOG_TRACE( "get_adapt_content_by_c_id_list start" );

    string format_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_FORMAT_COL ) );
    bson query[1], fields[1];
    string id;
    char index[8];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    vector<string>::const_iterator id_iter;

    char c_info_type[8];
    memset( c_info_type, 0x00, sizeof( c_info_type ) );
    sprintf( c_info_type, "%d", info_type );

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    if( !info_id_list.empty() && info_id_list[0].compare( "" ) != 0 )
    {
        if( info_type != 1 )
        {
            bson_init( query );
            bson_append_start_object( query, "c_id" );
            bson_append_start_array( query, "$in" );
            vector<string>::const_iterator iter = c_id_list.begin();
            for( int j=0; iter != c_id_list.end(); ++iter,++j )
            {
                memset( index, 0x00, sizeof( index ) );
                sprintf( index, "%d", j );
                bson_append_string( query, index, iter->c_str() );
            }
            bson_append_finish_object( query );
            bson_append_finish_object( query );
            bson_append_string( query, "stype",  c_info_type );
            bson_append_start_object( query, "stype_id" );
            bson_append_start_array( query, "$in" );
            iter = info_id_list.begin();
            for( int j=0; iter != info_id_list.end(); ++iter,++j )
            {
                memset( index, 0x00, sizeof( index ) );
                sprintf( index, "%d", j );
                bson_append_string( query, index, iter->c_str() );
                UB_LOG_DEBUG( "+++++ stype_id:[%s], [%s:%d] ++++++", iter->c_str(), __FILE__, __LINE__ );
            }
            bson_append_finish_object( query );
            bson_append_finish_object( query );
            bson_finish( query );

            bson_init( fields );
            bson_append_int( fields, "c_id", 1 );
            bson_finish( fields );

            cursor = mongo_find( conn, format_ns.c_str(), query, fields, 0, 0, 0 );
            if( cursor == NULL )
            {
                UB_LOG_FATAL( "query charge_type from MongoDB %s failed", format_ns.c_str() );
                free_data( NULL, query, fields, NULL );
                mongo_pool->releaseConnection(conn);
                return MCP_NG;
            }
            while( mongo_cursor_next( cursor ) == MONGO_OK )
            {
                id.clear();
                bson_iterator it[1];
                result = mongo_cursor_bson( cursor );
                bson_iterator_init( it, result);
                while( bson_iterator_next(it) )
                {
                    key = bson_iterator_key(it);
                    if( 0 == strcmp(key,"c_id"))
                    {
                        c_id_set.insert( bson_iterator_string(it) );
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            free_data( cursor, query, fields, NULL );
        }
        else
        {
            //do nothing
            //现在的需求不涉及同时传多个机型信息取适配数目
        }
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_adapt_content_by_c_id_list end" );
    return 0;
}

/*
int get_count_by_rank( const string& content_type, const string& class_id, const vector<string>& info_id_list,
                       const uint32_t info_type, const uint32_t for_adapt )
{
    UB_LOG_TRACE( "get_count_by_rank start" );

    //string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_ADAPT_COL ) );
    //bson query[1], fields[1], bson_out[1];
    //string tmp_class_id;
    int count = 0;
    //mongo *conn;
    int res = 0;


    if( info_id_list.size() == 1 || for_adapt == 0 )
    {
        count = get_count_by_rank( content_type, class_id, info_id_list[0], info_type, for_adapt );
    }
    else if( info_id_list.size() > 1 )
    {
        vector<string> c_id_list;
        set<string> c_id_set;
        res = get_c_id_list_by_class( content_type, class_id, c_id_list );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_c_id_list_by_class failed, [%s:%d]", __FILE__, __LINE__ );
            //mongo_pool->releaseConnection(conn);
            return -5;
        }
        res = get_adapt_content_by_c_id_list( info_id_list, info_type, c_id_list, c_id_set );
        if( res < 0 )
        {
            UB_LOG_FATAL( "get_adapt_count_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
            //mongo_pool->releaseConnection(conn);
            return -5;
        }
        count = c_id_set.size();
    }
    else
    {
        count = 0;
    }

//    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_count_by_rank end" );
    return count;
}
*/

/*
int get_count_by_rank( const string& content_type, const string& class_id, const string& info_id,
                       const uint32_t info_type, const uint32_t for_adapt )
{
    UB_LOG_TRACE( "get_count_by_rank start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_ADAPT_COL ) );
    bson query[1], fields[1], bson_out[1];
    string tmp_class_id;
    int count = 0;
    mongo *conn;
    int res = 0;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    char c_info_type[8];
    memset( c_info_type, 0x00, sizeof( c_info_type ) );
    sprintf( c_info_type, "%d", info_type );

    bson_init(query);
    if( class_id.empty() || class_id.compare( "" ) == 0 )
    {
        bson_append_string( query, "class_id", "0" );
        UB_LOG_DEBUG( "+++++ class_id:[0] +++++" );
    }
    else
    {
        bson_append_string( query, "class_id", class_id.c_str() );
        UB_LOG_DEBUG( "+++++ class_id:[%s] +++++", class_id.c_str() );
    }
    if( info_id.empty() || info_id.compare( "" ) == 0 || for_adapt == 0 )
    {
        bson_append_int( query, "stype", 0 );
        bson_append_string( query, "stype_id", "0" );
        UB_LOG_DEBUG( "+++++ stype:[0], stype_id:[0] +++++" );
    }
    else
    {
        bson_append_int( query, "stype", info_type );
        bson_append_string( query, "stype_id", info_id.c_str() );
        UB_LOG_DEBUG( "+++++ stype:[%s], stype_id:[%s] +++++", c_info_type, info_id.c_str() );
    }
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "count", 1 );
    bson_finish( fields );

    res = mongo_find_one( conn, ns.c_str(), query, fields, bson_out );

    if( res != MONGO_OK )
    {
        UB_LOG_FATAL( "query adapt count where class_id[%s] stype_id[%s] stype[%d] from MongoDB %s failed, [%s:%d]",
                                               class_id.c_str(), info_id.c_str(), info_type, ns.c_str(), __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    bson_iterator bson_it[1];
    if( bson_find( bson_it, bson_out, "count" ) == BSON_INT )
    {
        count = bson_iterator_int( bson_it );
        UB_LOG_DEBUG( "count: %d", count );
    }

    free_data( NULL, query, fields, bson_out );
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_count_by_rank end" );
    return count;
}
*/

int get_c_id_list_by_rank( const string& content_type, const string& class_id, const uint32_t p_type, const uint32_t unit_num,
                                 const uint32_t unit_times, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_c_id_list_by_rank start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_RANK_COL ) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;

    int start_pos = (unit_times-1)*unit_num;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson_init( query );
    bson_append_start_object( query, "$query" );
    if( content_type.compare( "5" ) == 0 )
    {
        bson_append_int( query, "type_base_id", 4 );
    }
    else
    {
        bson_append_int( query, "type_base_id", 1 );
    }
    if( !class_id.empty() && class_id.compare( "" ) != 0 )
    {
        bson_append_string( query, "class_id", class_id.c_str() );
    }
    bson_append_int( query, "ptype_id", p_type );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_int( query, "priority", -1 );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_finish( fields );

    content_info_t tmp_info;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_info.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"c_id"))
            {
                tmp_info.id = bson_iterator_string( it );
                break;
            }
            else
            {
                continue;
            }
        }
        content_info_list.push_back( tmp_info );
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_info.id.c_str(), __FILE__, __LINE__ );
    }
    
    UB_LOG_TRACE( "get_c_id_list_by_rank end" );
    mongo_pool->releaseConnection(conn);
    return 0;
}

int get_c_id_list_by_rank( const string& content_type, const string& class_id, const uint32_t p_type, const uint32_t unit_num,
                                 const uint32_t unit_times, set<string>& c_id_list, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_c_id_list_by_rank start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_RANK_COL ) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;
    char index[8];

    int start_pos = (unit_times-1)*unit_num;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    set<string>::const_iterator c_it;
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_start_object( query, "c_id" );
    bson_append_start_array( query, "$in" );
    int j = 0;
    for(c_it = c_id_list.begin(); c_it != c_id_list.end(); ++c_it)
    {   
        UB_LOG_DEBUG( "+++++ c_id:[%s] +++++ ", c_it->c_str() );
        memset( index, 0x00, sizeof( index ) );
        sprintf(index,"%d", j);
        bson_append_string( query, index, c_it->c_str() );
        j++;
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    if( content_type.compare( "5" ) == 0 )
    {
        bson_append_int( query, "type_base_id", 4 );
    }
    else
    {
        bson_append_int( query, "type_base_id", 1 );
    }
    if( !class_id.empty() && class_id.compare( "" ) != 0 )
    {
        bson_append_string( query, "class_id", class_id.c_str() );
    }
    bson_append_int( query, "ptype_id", p_type );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_int( query, "priority", -1 );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_finish( fields );

    content_info_t tmp_info;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_info.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"c_id"))
            {
                tmp_info.id = bson_iterator_string( it );
                break;
            }
            else
            {
                continue;
            }
        }
        content_info_list.push_back( tmp_info );
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_info.id.c_str(), __FILE__, __LINE__ );
    }

    UB_LOG_TRACE( "get_c_id_list_by_rank end" );
    mongo_pool->releaseConnection(conn);
    return 0;
}

int get_basic_info_by_c_id_list( const string& content_type, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_basic_info_by_c_id_list start" );
     
    string ns;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;
    char index[8];
    vector<content_info_t>::iterator c_it;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    //theme
    if( content_type.compare( "5" ) == 0 )
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL );
    }
    //default: game
    else
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL );
    }

    map<string, string> m_class_id_name;
    get_content_class_id_name_map( conn, m_class_id_name );

    bson_init( query );
    bson_append_string( query, "content_type", content_type.c_str() );
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in"); 
    int j = 0;
    for(c_it = content_info_list.begin(); c_it < content_info_list.end(); ++c_it)
    {   
        UB_LOG_DEBUG( "+++++++++ c_id:[%s] +++++++++", c_it->id.c_str() );
        memset( index, 0x00, sizeof( index ) );
        sprintf(index,"%d", j);
        bson_append_string( query, index, c_it->id.c_str() );
        j++;
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "name", 1 );
    bson_append_int( fields, "class_id", 1 );
    bson_append_int( fields, "game_code", 1 );
    bson_append_int( fields, "default_charge_typeid", 1 );
    bson_finish( fields );

    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    string tmp_id, tmp_name, tmp_code, tmp_charge, tmp_class_id, tmp_class_name;
    bson_iterator sub[1];
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_id.clear();
        tmp_name.clear();
        tmp_charge.clear();
        tmp_class_id.clear();
        tmp_class_name.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            UB_LOG_DEBUG( "++++++++++ key[%s] ++++++++++++", key );
            if( 0 == strcmp(key,"id"))
            {
                tmp_id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"name"))
            {
                tmp_name = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"class_id"))
            {
                bson_iterator_subiterator( it, sub );
                while( bson_iterator_next( sub ) )
                {
                    if( !tmp_class_id.empty() || tmp_class_id.compare("") != 0 )
                    {
                        tmp_class_id.append( "," );
                        tmp_class_name.append( "," );
                    }
                    tmp_class_id.append(bson_iterator_string(sub));
                    tmp_class_name.append( m_class_id_name[bson_iterator_string(sub)] );
                }
            }
            else if( 0 == strcmp(key,"game_code"))
            {
                tmp_code = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"default_charge_typeid"))
            {
                tmp_charge = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
        for( c_it=content_info_list.begin(); c_it!=content_info_list.end(); ++c_it )
        {
            if( c_it->id.compare(tmp_id) == 0 )
            {
                c_it->name = tmp_name;
                c_it->class_id = tmp_class_id;
                c_it->class_name = tmp_class_name;
                c_it->code = tmp_code;
                c_it->dft_charge_id = tmp_charge;
                UB_LOG_DEBUG( "+++++ [%s]:{name:%s,code:%s,charge_id:%s} ++++", 
                              tmp_id.c_str(), tmp_name.c_str(), tmp_code.c_str(), tmp_charge.c_str() );
            }
        }
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_id.c_str(), __FILE__, __LINE__ );
    }


    //TODO
    UB_LOG_TRACE( "get_basic_info_by_c_id_list end" );
    mongo_pool->releaseConnection(conn);
    return 0;
}


int get_basic_info_by_c_id_list( const string& content_type, const uint32_t unit_num,  
                                 const uint32_t unit_times, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_basic_info_by_c_id_list start" );

    string ns;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;
    set<string>::const_iterator c_it;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    //theme
    if( content_type.compare( "5" ) == 0 )
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL );
    }
    //default: game
    else
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL );
    }

    map<string, string> m_class_id_name;
    get_content_class_id_name_map( conn, m_class_id_name );

    bson_init( query );
    bson_append_string( query, "content_type", content_type.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "name", 1 );
    bson_append_int( fields, "class_id", 1 );
    bson_append_int( fields, "game_code", 1 );
    bson_append_int( fields, "default_charge_typeid", 1 );
    bson_finish( fields );

    uint32_t start_pos = (unit_times-1)*unit_num;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    content_info_list.clear();
    content_info_t tmp_info;
    bson_iterator sub[1];
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_info.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            UB_LOG_DEBUG( "++++++++++ key[%s] ++++++++++++", key );
            if( 0 == strcmp(key,"id"))
            {
                tmp_info.id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"name"))
            {
                tmp_info.name = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"class_id"))
            {
                //tmp_info.class_id = bson_iterator_string( it );
                bson_iterator_subiterator( it, sub );
                while( bson_iterator_next( sub ) )
                {
                    if( !tmp_info.class_id.empty() || tmp_info.class_id.compare("") != 0 )
                    {
                        tmp_info.class_id.append( "," );
                        tmp_info.class_name.append( "," );
                    }
                    tmp_info.class_id.append(bson_iterator_string(sub));
                    tmp_info.class_name.append( m_class_id_name[bson_iterator_string(sub)] );
                }
            }
            else if( 0 == strcmp(key,"game_code"))
            {
                tmp_info.code = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"default_charge_typeid"))
            {
                 tmp_info.dft_charge_id = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
        content_info_list.push_back( tmp_info );
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_info.id.c_str(), __FILE__, __LINE__ );
    }


    free_data( cursor, query, fields, NULL );
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_basic_info_by_c_id_list end" );
    return 0;
}

int get_basic_info_by_c_id_list( const string& content_type, const set<string>& c_id_list, const uint32_t unit_num, 
                                 const uint32_t unit_times, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_basic_info_by_c_id_list start" );

    string ns;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;
    char index[8];
    set<string>::const_iterator c_it;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    //theme
    if( content_type.compare( "5" ) == 0 )
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_THEME_COL );
    }
    //default: game
    else
    {
        ns = get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_GAME_COL );
    }

    map<string, string> m_class_id_name;
    get_content_class_id_name_map( conn, m_class_id_name );

    bson_init( query );
    bson_append_string( query, "content_type", content_type.c_str() );
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    int j = 0;
    for(c_it = c_id_list.begin(); c_it != c_id_list.end(); ++c_it)
    {
        UB_LOG_DEBUG( "+++++++++ c_id:[%s] +++++++++", c_it->c_str() );
        memset( index, 0x00, sizeof( index ) );
        sprintf(index,"%d", j);
        bson_append_string( query, index, c_it->c_str() );
        j++;
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "name", 1 );
    bson_append_int( fields, "class_id", 1 );
    bson_append_int( fields, "game_code", 1 );
    bson_append_int( fields, "default_charge_typeid", 1 );
    bson_finish( fields );

    uint32_t start_pos = (unit_times-1) * unit_num;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    content_info_list.clear();
    content_info_t tmp_info;
    bson_iterator sub[1];
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_info.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            UB_LOG_DEBUG( "++++++++++ key[%s] ++++++++++++", key );
            if( 0 == strcmp(key,"id"))
            {
                tmp_info.id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"name"))
            {
                tmp_info.name = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"class_id"))
            {
                //tmp_info.class_id = bson_iterator_string( it );
                bson_iterator_subiterator( it, sub );
                while( bson_iterator_next( sub ) )
                {
                    if( !tmp_info.class_id.empty() || tmp_info.class_id.compare("") != 0 )
                    {
                        tmp_info.class_id.append( "," );
                        tmp_info.class_name.append( "," );
                    }
                    tmp_info.class_id.append(bson_iterator_string(sub));
                    tmp_info.class_name.append(m_class_id_name[bson_iterator_string(sub)]);
                }
            }
            else if( 0 == strcmp(key,"game_code"))
            {
                tmp_info.code = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"default_charge_typeid"))
            {
                 tmp_info.dft_charge_id = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
        content_info_list.push_back( tmp_info );
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_info.id.c_str(), __FILE__, __LINE__ );
    }


    UB_LOG_TRACE( "get_basic_info_by_c_id_list end" );
    mongo_pool->releaseConnection(conn);
    return 0;
}

int get_content_info_by_rank( const string& content_type, const string& class_id, const uint32_t p_type, const uint32_t unit_num,
                              const uint32_t unit_times, const string& info_id, const uint32_t info_type, 
                              vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_content_info_by_rank start" );

    int res  = 0;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;            
    }   

    res = get_c_id_list_by_rank( content_type, class_id, p_type, unit_num, unit_times, content_info_list );   
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_c_id_list_by_rank faield, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_basic_info_by_c_id_list( content_type, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_basic_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    vector<string> c_id_list;
    vector<content_info_t>::iterator it;
    for( it = content_info_list.begin(); it != content_info_list.end(); ++it )
    {
        c_id_list.push_back( it->id );
    }
    
    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );
    res = get_download_info_by_c_id_list( conn, info_id, info_type, c_id_list, m_charge_id_name, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_download_info_by_c_id_list faield, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_image_info_by_c_id_list( conn, "3", content_info_list );
    if( res == -5 )
    {
        UB_LOG_FATAL( "get_image_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_content_info_by_rank end" );
    return 0;
}

int get_content_info_by_date( const string& content_type, const uint32_t unit_num,
                              const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_content_info_by_date start" );

    int res  = 0;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    res = get_basic_info_by_c_id_list( content_type, unit_num, unit_times, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_basic_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    vector<string> c_id_list;
    vector<content_info_t>::iterator it;
    for( it = content_info_list.begin(); it != content_info_list.end(); ++it )
    {
        c_id_list.push_back( it->id );
    }

    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );
    res = get_download_info_by_c_id_list( conn, info_id, info_type, c_id_list, m_charge_id_name, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_download_info_by_c_id_list faield, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_image_info_by_c_id_list( conn, "3", content_info_list );
    if( res == -5 )
    {
        UB_LOG_FATAL( "get_image_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_content_info_by_date end" );
    return 0;
}


int get_c_id_list_by_adapt_info( const uint32_t& info_type, const string& info_id, 
                                 set<string>& c_id_list, map<string, vector<string> >& m_id_data_id )
{
    UB_LOG_TRACE( "get_c_id_list_by_adapt_info start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_FORMAT_COL ) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    char index[8];
    char stype_id_list[1024];
    char *p_stype_id = stype_id_list;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    char c_info_type[8];
    char* tmp=NULL;
    memset( c_info_type, 0x00, sizeof(c_info_type) );
    sprintf( c_info_type, "%d", info_type );
    bson_init( query );
    bson_append_string( query, "stype",  c_info_type );
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
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_append_int( fields, "cdata_id", 1 );
    bson_finish( fields );

    string id, data_id;
    const char* key;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query charge_type from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return MCP_NG;
    }
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        id.clear();
        data_id.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"c_id"))
            {
                id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"cdata_id"))
            {
                data_id = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
        c_id_list.insert( id );
        m_id_data_id[id].push_back( data_id );
        UB_LOG_NOTICE( "m_id_data_id[%s] push %s", id.c_str(), data_id.c_str() );
    }

    free_data( cursor, query, fields, NULL );
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_c_id_list_by_adapt_info end" );
    return 0;
}

int get_download_info_by_data_id_list( const vector<string>& data_id_list, map<string, string>& m_charge_id_name, 
                                      content_info_t& content_info )
{
    UB_LOG_TRACE( "get_download_info_by_data_id_list start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DATA_COL ) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    char index[8];
    const char* key;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    vector<string>::const_iterator data_iter;
    bson_init( query );
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    int j = 0;
    for( data_iter = data_id_list.begin(); data_iter != data_id_list.end(); ++data_iter, ++j)
    {
        memset( index, 0x00, sizeof( index ) );
        sprintf(index,"%d", j);
        bson_append_string( query, index, data_iter->c_str() );
        UB_LOG_NOTICE("cdata id %d is %s", j, data_iter->c_str() );
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_finish(query);
    //设置返回域
    bson_init(fields);
    bson_append_int(fields, "id", 1);
    bson_append_int(fields, "path_url", 1);
    bson_append_int(fields, "charge_typeid", 1);
    bson_finish(fields);

    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query charge_type from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        content_info.charge_id = content_info.dft_charge_id ;
        content_info.charge_name = m_charge_id_name[content_info.charge_id];
        mongo_pool->releaseConnection(conn);
    }

    string data_id, charge_id, charge_name, path_url;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id"))
            {
                data_id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"path_url"))
            {
                path_url = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"charge_typeid"))
            {
                charge_id = bson_iterator_string( it );
                charge_name  = m_charge_id_name[charge_id];
            }
            else
            {
                continue;
            }
        }
        if( !content_info.down_id.empty() || content_info.down_id.compare( "" ) != 0 )
        {
            content_info.down_id.append( "," );
            content_info.down_url.append( "," );
            content_info.charge_id.append( "," );
            content_info.charge_name.append( "," );
        }
        content_info.down_id.append( data_id );
        content_info.down_url.append( path_url );
        if( charge_id.empty() || charge_id.compare( "" ) == 0 )
        {
            charge_id = content_info.dft_charge_id;
            //charge_name = content_it->dft_charge_name;
            charge_name = m_charge_id_name[charge_id];
        }
        content_info.charge_id.append( charge_id );
        content_info.charge_name.append( charge_name );

        UB_LOG_NOTICE( "m_id_data_id[%s] push %s", content_info.id.c_str(), data_id.c_str() );
    }
    if( content_info.charge_id.empty() || content_info.charge_id.compare( "" )==0 )
    {
        content_info.charge_id = content_info.dft_charge_id;
        content_info.charge_name = m_charge_id_name[content_info.charge_id];
    }
    free_data( cursor, query, fields, NULL );

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_download_info_by_data_id_list end" );
    return 0;
}

int get_adapt_content_info_by_rank( const string& content_type, const string& class_id, const uint32_t p_type, 
                              const uint32_t unit_num, const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_adapt_content_info_by_rank start" );

    int res  = 0;
    map<string, vector<string> > m_id_data_id;
    set<string> c_id_list;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    res = get_c_id_list_by_adapt_info( info_type, info_id, c_id_list, m_id_data_id );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_c_id_list_by_adapt_info failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_c_id_list_by_rank( content_type, class_id, p_type, unit_num, unit_times, c_id_list, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_c_id_list_by_rank faield, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_basic_info_by_c_id_list( content_type, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_basic_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    vector<content_info_t>::iterator it;

    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );
    for( it = content_info_list.begin(); it != content_info_list.end(); ++it )
    {
        res = get_download_info_by_data_id_list( m_id_data_id[it->id], m_charge_id_name, *it );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_download_info_by_data_id_list faield, [%s:%d]", __FILE__, __LINE__ );
            continue;
        }
    }

    res = get_image_info_by_c_id_list( conn, "3", content_info_list );
    if( res == -5 )
    {
        UB_LOG_FATAL( "get_image_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_adapt_content_info_by_rank end" );
    return 0;
}


int get_adapt_content_info_by_date( const string& content_type,
                              const uint32_t unit_num, const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_adapt_content_info_by_date start" );

    int res  = 0;
    map<string, vector<string> > m_id_data_id;
    set<string> c_id_list;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    res = get_c_id_list_by_adapt_info( info_type, info_id, c_id_list, m_id_data_id );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_c_id_list_by_adapt_info failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_basic_info_by_c_id_list( content_type, c_id_list, unit_num, unit_times, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_basic_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    vector<content_info_t>::iterator it;

    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );
    for( it = content_info_list.begin(); it != content_info_list.end(); ++it )
    {
        res = get_download_info_by_data_id_list( m_id_data_id[it->id], m_charge_id_name, *it );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_download_info_by_data_id_list faield, [%s:%d]", __FILE__, __LINE__ );
            continue;
        }
    }

    res = get_image_info_by_c_id_list( conn, "3", content_info_list );
    if( res == -5 )
    {
        UB_LOG_FATAL( "get_image_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_adapt_content_info_by_date end" );
    return 0;
}


/*
int get_c_id_list_by_class( const string& content_type, const string& class_id, const uint32_t priority, const uint32_t unit_num, 
                            const uint32_t unit_times, const set<string>& c_id_list, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_c_id_list_by_class start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_CLASS_LIST_COL ) );
    bson query[1], fields[1];
    const bson* bson_result;
    mongo *conn;
    
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {        
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    uint32_t start_pos = ( unit_times - 1 ) * unit_num;
        
    char index[8];
    set<string>::const_iterator c_it;
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_start_object( query, "c_id" );
    bson_append_start_array( query, "$in" );
    int j = 0;
    for(c_it = c_id_list.begin(); c_it != c_id_list.end(); ++c_it)
    {   
        memset( index, 0x00, sizeof( index ) );
        sprintf(index,"%d", j);
        bson_append_string( query, index, c_it->c_str() );
        j++;
    }
    bson_append_finish_object( query );
    bson_append_finish_object( query );
    bson_append_string( query, "class_id", class_id.c_str() );
    bson_append_int( query, "enable", 1 );
    bson_append_finish_object( query );
    if( priority == 1 )
    {
        bson_append_start_object( query, "$orderby" );
        bson_append_int( query, "priority", -1 );
        bson_append_finish_object( query );
    }
    bson_finish( query );
    
    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_finish( fields );
        
    mongo_cursor* cursor = NULL;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );

    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query c_id where class_id(%s) from MongoDB %s failed", class_id.c_str(), ns.c_str() );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return MCP_NG;
    }

    content_info_t tmp_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_info.clear();
        bson_result = mongo_cursor_bson( cursor );
        bson_iterator bson_it[1];

        if( bson_find( bson_it, bson_result, "c_id" ) == BSON_STRING )
        {
            tmp_info.id = bson_iterator_string(bson_it);
        }
        content_info_list.push_back( tmp_info );
    }

    free_data( cursor, query, fields, NULL );
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_c_id_list_by_class end" );
    return 0;
}
*/

/*
int get_adapt_content_info_by_class( const string& content_type, const string& class_id, const uint32_t priority,
                              const uint32_t unit_num, const uint32_t unit_times, const string& info_id, const uint32_t info_type,
                              vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_adapt_content_info_by_class start" );

    int res  = 0;
    map<string, vector<string> > m_id_data_id;
    set<string> c_id_list;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    res = get_c_id_list_by_adapt_info( info_type, info_id, c_id_list, m_id_data_id );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_c_id_list_by_adapt_info failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_c_id_list_by_class( content_type, class_id, priority, unit_num, unit_times, c_id_list, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_c_id_list_by_class faield, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    res = get_basic_info_by_c_id_list( content_type, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_basic_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    vector<content_info_t>::iterator it;
    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );
    for( it = content_info_list.begin(); it != content_info_list.end(); ++it )
    {
        res = get_download_info_by_data_id_list( m_id_data_id[it->id], m_charge_id_name, *it );
        if( res != 0 )
        {
            UB_LOG_FATAL( "get_download_info_by_data_id_list faield, [%s:%d]", __FILE__, __LINE__ );
            continue;
        }
    }

    res = get_image_info_by_c_id_list( conn, "3", content_info_list );
    if( res == -5 )
    {
        UB_LOG_FATAL( "get_image_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_adapt_content_info_by_class end" );
    return 0;
}
*/

int get_label_by_c_id( const string& c_id, vector<string>& label_id_list )
{
    UB_LOG_TRACE( "get_label_by_c_id start" );

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_LABEL_LIST_COL ) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;

    bson_init( query );
    bson_append_string( query, "c_id", c_id.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "cb_id", 1 );
    bson_finish( fields );

    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_FATAL( "query charge_type from MongoDB %s failed", ns.c_str() );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -5;
    }

    string label_id;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"cb_id"))
            {
                label_id = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
        label_id_list.push_back( label_id );
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_label_by_c_id end" );
    return 0;
}


int get_count_by_label( const string& label_id )
{
    UB_LOG_TRACE( "get_count_by_label start" );

    bson query[1];
    bson_init( query );
    bson_append_string( query, "cb_id", label_id.c_str() );
    bson_finish( query );

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int count;
    count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_LABEL_LIST_COL, query );
    if( count < 0 )
    {
        UB_LOG_FATAL( "mongo_count failed, [%s:%d]", __FILE__, __LINE__ );
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_count_by_label end" );
    return count;
}

int get_c_id_list_by_label( const string& label_id, const uint32_t unit_num, const uint32_t unit_times, 
                                   vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_c_id_list_by_label start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_LABEL_LIST_COL ) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;

    int start_pos = (unit_times-1)*unit_num;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "cb_id", label_id.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_int( query, "priority", -1 );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_finish( fields );

    content_info_t tmp_info;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_info.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"c_id"))
            {
                tmp_info.id = bson_iterator_string( it );
                break;
            }
            else
            {
                continue;
            }
        }
        content_info_list.push_back( tmp_info );
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_info.id.c_str(), __FILE__, __LINE__ );
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_c_id_list_by_label end" );
    return 0;
}

int get_content_info_by_label( const string& label_id, const uint32_t unit_num, const uint32_t unit_times, const string& info_id,
                               const uint32_t info_type, vector<content_info_t>& content_info_list )
{
    UB_LOG_TRACE( "get_content_info_by_label start" );

    int res  = 0;

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    res = get_c_id_list_by_label( label_id, unit_num, unit_times, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_c_id_list_by_label faield, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    //youxi
    res = get_basic_info_by_c_id_list( "1", content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_basic_info_by_c_id_list failed, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    vector<string> c_id_list;
    vector<content_info_t>::iterator it;
    for( it = content_info_list.begin(); it != content_info_list.end(); ++it )
    {
        c_id_list.push_back( it->id );
    }

    map<string, string> m_charge_id_name;
    get_content_charge_id_name_map( conn, m_charge_id_name );
    res = get_download_info_by_c_id_list( conn, info_id, info_type, c_id_list, m_charge_id_name, content_info_list );
    if( res != 0 )
    {
        UB_LOG_FATAL( "get_download_info_by_c_id_list faield, [%s:%d]", __FILE__, __LINE__ );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_content_info_by_label end" );
    return 0;
}

int get_content_image( const string& content_id, const uint32_t img_type, vector<string>& img_url_list )
{
    UB_LOG_TRACE( "get_content_image start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_PREVIEW_COL ) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    char c_img_type[8];
    memset(c_img_type, 0x00, sizeof(c_img_type));
    sprintf( c_img_type, "%d", img_type );

    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "c_id", content_id.c_str() );
    bson_append_string( query, "image_type_id", c_img_type );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_int( query, "priority", -1 );
    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "image_url", 1 );
    bson_finish( fields );

    string tmp_url;
    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_url.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"image_url"))
            {
                tmp_url = bson_iterator_string( it );
                break;
            }
            else
            {
                continue;
            }
        }
        img_url_list.push_back( tmp_url );
        UB_LOG_DEBUG( "+++++++++ push url[%s] ++++++++++, [%s:%d]", tmp_url.c_str(), __FILE__, __LINE__ );
    }

    free_data(cursor, query,fields,NULL);
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_content_image end" );
    return 0;
}

int get_topic_count_by_type( const string& content_type )
{
    UB_LOG_TRACE( "get_topic_count_by_type start" );

    bson query[1];
    bson_init( query );
    bson_append_string( query, "type_id", content_type.c_str() );
    bson_finish( query );
    int count = 0;;
        
    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    
    count = mongo_count( conn, MCP_MONGODB_NAME, MCP_MONGO_DIY_COL, query );
    if( count < 0 )
    {
        UB_LOG_FATAL( "mongo_count failed, [%s:%d]", __FILE__, __LINE__ );
    }
    
    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_topic_count_by_type end" );
    return count;
}

int get_topic_info_by_type( const string& content_type, const uint32_t unit_num, const uint32_t unit_times,
                            vector<topic_info_t>& topic_info_list )
{
    UB_LOG_TRACE( "get_topic_info_by_type start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DIY_COL) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;

    uint32_t start_pos = (unit_times-1)*unit_num;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "type_id", content_type.c_str() );
    bson_append_finish_object( query );

//    bson_append_start_object( query, "$orderby" );
//    bson_append_int( query, "priority", -1 );
//    bson_append_finish_object( query );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "name", 1 );
    bson_finish( fields );

    topic_info_t tmp_topic;
    cursor = mongo_find( conn, ns.c_str(), query, fields, unit_num, start_pos, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_topic.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id"))
            {
                tmp_topic.id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"name"))
            {
                tmp_topic.name = bson_iterator_string( it );
            }
            else
            {
                continue;
            }
        }
        topic_info_list.push_back( tmp_topic );
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_topic.id.c_str(), __FILE__, __LINE__ );
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_topic_info_by_type end" );
    return 0;
}

int get_label_name_by_label_id( const string& label_id, string& label_name )
{
    UB_LOG_TRACE( "get_label_name_by_label_id start" );

    string ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_LABEL_COL) );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;
    const char* key;
    mongo *conn;

    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson_init( query );
    bson_append_string( query, "id", label_id.c_str() );
    bson_finish( query );

    bson_init( fields );
    bson_append_int( fields, "name", 1 );
    bson_finish( fields );

    cursor = mongo_find( conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL )
    {
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        mongo_pool->releaseConnection(conn);
        return -1;
    }

    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"name"))
            {
                label_name = bson_iterator_string( it );
                break;
            }
        }
    }

    mongo_pool->releaseConnection(conn);
    UB_LOG_TRACE( "get_label_name_by_label_id end" );
    return 0;
}

int get_download_info_by_cid( const string& c_id, const uint32_t info_type, const vector<string>& info_id_list, 
                              vector<download_info_t>& download_info_list )
{
    UB_LOG_TRACE( "get_download_info_by_cid start" );

    mongo *conn;
    if( mcp_connect_mongo_hosts( conn )!= MCP_OK )
    {
        //mongo_destroy( conn );
        //mongo_pool->releaseConnection(conn);
        UB_LOG_FATAL("Can not connect to MongoDB [%s:%d]", __FILE__, __LINE__ );
        return MCP_OK;
    }

    string data_ns( get_mongo_namespace( MCP_MONGODB_NAME, MCP_MONGO_DATA_COL ) );
    bson query[1], fields[1];
    const bson* result;
    const char* key;
    mongo_cursor* cursor = NULL;

    char c_info_type[4];
    sprintf( c_info_type, "%d", info_type );

    string cdata_id;
    bson_iterator bson_it[1];
    char index[8];

    bson_init( query );
    bson_append_string( query, "c_id", c_id.c_str() );

    bson_append_start_array( query, "adapt_info" );

    vector<string>::const_iterator iter = info_id_list.begin();
    for( int j = 0; iter != info_id_list.end(); ++j,++iter )
    {
        memset( index, 0x00, sizeof(index) );
        sprintf( index, "%d", j );
        bson_append_start_object( query, index );
        bson_append_string( query, "stype", c_info_type );
        bson_append_string( query, "stype_id", iter->c_str() );
        bson_append_finish_object( query );
    }

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

    int tmp_int;
    char tmp_char[32];
    download_info_t tmp_download;
    while( mongo_cursor_next( cursor ) == MONGO_OK )
    {
        tmp_download.clear();
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) )
        {
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"id"))
            {
                tmp_download.id = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"path_url"))
            {
                tmp_download.path_url = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"path_type"))
            {
                tmp_download.path_type = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"media_format_id"))
            {
                tmp_download.media_format = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"size"))
            {
                tmp_int = bson_iterator_int( bson_it );
                memset( tmp_char, 0x00, sizeof(tmp_char) );
                sprintf( tmp_char, "%d", tmp_int );
                tmp_download.size = tmp_char;
            }
            else if( 0 == strcmp(key,"mtype"))
            {
                tmp_download.mtype = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"charge_typeid"))
            {
                tmp_download.charge_type = bson_iterator_string( it );
            }
            else if( 0 == strcmp(key,"value"))
            {
                tmp_int = bson_iterator_int( bson_it );
                memset( tmp_char, 0x00, sizeof(tmp_char) );
                sprintf( tmp_char, "%d", tmp_int );
                tmp_download.value = tmp_char;
            }
            else
            {
                continue;
            }
        }
        UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", tmp_download.id.c_str(), __FILE__, __LINE__ );
        download_info_list.push_back( tmp_download );
    }

    UB_LOG_TRACE( "get_download_info_by_cid end" );
    return 0;
}

int create_phone_invalid_list()
{
    MyclientRes data_res;
    int err = 0;
    int count_num = 0;
    int ret = 0;
    char query_sql[1024];
    memset(query_sql, 0x00, sizeof(query_sql));
    UB_LOG_NOTICE("create_phone_invalid_list is start");
    MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        UB_LOG_FATAL("get database connection failed from database_pool");
        data_res.free();
        return MCP_NG;
    }
    sprintf(query_sql,
               "SELECT mcp_province.id, mcp_province.enable FROM mcp_province" );
    count_num = database_connection->query(query_sql, &data_res, true);
    if ( count_num < 0 )
    {
        UB_LOG_FATAL("query data from database failed, query_sql is %s", query_sql);
        ret = database_pool->putBackConnection(database_connection, false);
        if (ret != 0)
        {
           UB_LOG_FATAL("putback connection is failed");
        }
        data_res.free();
        return MCP_NG;
    }
    invalid_flag.clear();
    for ( int i = 0; i < count_num ; i++)
    {
        invalid_flag.insert(map<int,int>::value_type(atoi(data_res[i][0]), atoi(data_res[i][1]) ));
    }
    ret = database_pool->putBackConnection(database_connection, false);
    if (ret != 0)
    {
       UB_LOG_FATAL("putback connection is failed");
    }
    data_res.free();
    return MCP_OK;
}


