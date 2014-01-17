#include "content_column.h"
#include "ub_server.h"

using namespace std;
using namespace content;

int ContentColumn::get_content_list_all()
{
    UB_LOG_TRACE( "get_content_list_all start" );

    int res(0), count(0);
    count = this->get_content_count();
    if( count==0 ){
        return 0;
    }
    else if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }

    vector<string> id_list;
    res=this->get_id_list(id_list);
    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _column_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    vector<basic_info_t> basic_list;
    res = this->get_content_basic_info(id_list,basic_list);
    if( res < 0 ){
        UB_LOG_FATAL( "get_content_basic_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }

    vector<download_info_t> download_list;
    res = this->get_content_download_info(basic_list,download_list);
    if( res < 0 ){
        UB_LOG_FATAL( "get_content_download_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }

    combine_content_list(basic_list, download_list);
    UB_LOG_TRACE( "get_content_list_all end" );
    return count;
}

int ContentColumn::get_content_soft_list(vector<soft_info_t>& soft_list)
{
    UB_LOG_TRACE( "get_content_soft_list start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    vector<string> id_list;
    res=this->get_id_list(id_list);
    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _column_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    this->get_soft_query_bson(id_list,query);
    this->get_soft_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    soft_list.clear();
    soft_info_t soft_info;
    map<string,soft_info_t> m_id_soft_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        soft_info.clear();
        res=this->get_soft_info_from_cursor(cursor, soft_info);
        if(res==0){
            //soft_list.push_back( soft_info );
            m_id_soft_info[soft_info.id]=soft_info;
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", soft_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    vector<string>::const_iterator it = id_list.begin();
    for(; it!= id_list.end(); ++it ){
        soft_list.push_back(m_id_soft_info[*it]);
    }

    free_data( cursor, query, fields, NULL );

/*
    bson command[1], out[1];
    this->get_soft_query_command(command);
    res = mongo_run_command(_conn, "mcp", command, out);
    if(res!=0){
        UB_LOG_FATAL( "run mongo_command failed, [%s:%d]", __FILE__, __LINE__ );
        free_data(NULL, command, out, NULL );
        return -1;
    }

    soft_list.clear();
    soft_info_t soft_info;
    bson_iterator it[1], sub[1], subsub[1];
    bson_find(it, out, "result");
    bson_iterator_subiterator( it, sub );
    while(bson_iterator_next(sub)){
        bson_iterator_subiterator( sub, subsub );
        soft_info.clear();
        this->get_soft_info_from_bson_iterator(subsub,soft_info);
        soft_list.push_back(soft_info);
    }
    free_data( NULL, command, out, NULL );
*/
    
    UB_LOG_TRACE( "get_content_soft_list end" );
    return 0;
}

int ContentColumn::get_ios_content_list(int32_t& count, vector<ios_content_t>& list)
{
    return ContentList::get_ios_content_list(count,list);
}

int ContentColumn::get_ios_content_list(vector<ios_content_t>& list)
{
    UB_LOG_TRACE( "ContentColumn::get_ios_content_list start" );

    if( _priority != 0 ){
        return ContentList::get_ios_content_list(list);
    }

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    vector<string> id_list;
    res=this->get_id_list(id_list);
    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _column_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    this->get_ios_query_bson(id_list,query);
    this->get_ios_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    list.clear();
    ios_content_t ios_content;
    map<string,ios_content_t> m_id_ios_content;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        ios_content.clear();
        res=this->get_ios_content_from_cursor(cursor, ios_content);
        if(res==0){
            m_id_ios_content[ios_content.id]=ios_content;
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", ios_content.id.c_str(), __FILE__, __LINE__ );
        }
    }

    vector<string>::const_iterator it = id_list.begin();
    for(; it!= id_list.end(); ++it ){
        list.push_back(m_id_ios_content[*it]);
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "ContentColumn::get_ios_content_list end" );
    return 0;
}

int ContentColumn::getOnlineGameList(int32_t& count, vector<online_game_t>& list)
{
    return ContentList::getOnlineGameList(count, list);
}

int ContentColumn::getOnlineGameList(vector<online_game_t>& list)
{
    UB_LOG_TRACE( "ContentColumn::getOnlineGameList start" );
    if( _priority != 0 ){
        return ContentList::getOnlineGameList(list);
    }
    int res(0);
    bson command[1], out[1];
    this->getOnlineGameCommand(command);
    res = mongo_run_command(_conn, "mcp", command, out);
    if(res!=0){
        UB_LOG_FATAL( "run mongo_command failed, [%s:%d]", __FILE__, __LINE__ );
        free_data(NULL, command, out, NULL );
        return -1;
    }

    list.clear();
    bson_iterator it[1], sub[1];
    bson subsub[1];
    bson_find(it, out, "result");
    bson_iterator_subiterator( it, sub );
    while(bson_iterator_next(sub)){
        bson_iterator_subobject( sub, subsub );
        list.push_back(getOnlineGameFromBson(subsub));
    }
    free_data( NULL, command, out, NULL );
    UB_LOG_TRACE( "ContentColumn::getOnlineGameList end" );
    return 0;
}

int ContentColumn::get_android_theme_basic_list(vector<android_theme_t>& theme_list)
{
    UB_LOG_DEBUG( "get_android_theme_basic_list start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    vector<string> id_list;
    res=this->get_id_list(id_list);
    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _column_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    this->get_android_theme_query_bson(id_list,query);
    this->get_android_theme_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    theme_list.clear();
    android_theme_t theme_info;
    map<string,android_theme_t> m_id_theme_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        theme_info.clear();
        res=this->get_android_theme_from_cursor(cursor, theme_info);
        if(res==0){
            m_id_theme_info[theme_info.id]=theme_info;
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", theme_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    vector<string>::const_iterator it = id_list.begin();
    for(; it!= id_list.end(); ++it ){
        theme_list.push_back(m_id_theme_info[*it]);
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_DEBUG( "get_android_theme_basic_list end" );
    return 0;
}


int ContentColumn::get_id_list(vector<string>& id_list)
{
    UB_LOG_TRACE( "get_id_list start" );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;

    this->get_id_list_query_bson(query);
    this->get_id_list_fields_bson(fields);

    cursor = mongo_find( _conn, "mcp.mcp_content_column_list", query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }
    id_list.clear();
    bson_iterator it[1];
    while(mongo_cursor_next(cursor)==MONGO_OK){
        result=mongo_cursor_bson(cursor);
        bson_find(it, result, "c_id");
        id_list.push_back(bson_iterator_string(it));
        UB_LOG_DEBUG("push id[%s]",bson_iterator_string(it));
    }
    free_data(cursor, query, fields, NULL);
    UB_LOG_TRACE( "get_id_list end" );
    return 0;
}

int ContentColumn::get_content_basic_info(const vector<string>& id_list, vector<basic_info_t>& basic_list)
{
    UB_LOG_TRACE( "get_content_basic_info start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_basic_query_bson(id_list, query);
    this->get_basic_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    basic_info_t basic_info;
    map<string,basic_info_t> m_id_basic_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        basic_info.clear();
        res=this->get_basic_info_from_cursor(cursor, basic_info);
        if(res==0){
            //basic_list.push_back( basic_info );
            m_id_basic_info[basic_info.id] = basic_info;
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", basic_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    basic_list.clear();
    map<string,basic_info_t>::iterator map_iter;
    for(vector<string>::const_iterator it = id_list.begin(); it != id_list.end(); ++it){
        map_iter = m_id_basic_info.find(*it);
        if( map_iter != m_id_basic_info.end() ){
            basic_list.push_back(map_iter->second);
        }
    }
    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_basic_info end" );
    return 0;
}

void ContentColumn::get_basic_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "column.id", _column_id.c_str() );
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}

void ContentColumn::get_basic_query_bson(const vector<string>& id_list, bson* query)
{
    char index[8];
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator it = id_list.begin();
    for(int i=0; it!=id_list.end(); ++it,++i){
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        UB_LOG_DEBUG("push id[%s]",it->c_str());
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_finish(query);
    return;
}


void ContentColumn::get_id_list_query_bson(bson* query)
{
    bson_init(query);
    bson_append_start_object(query, "$query");
    bson_append_string(query, "column_id", _column_id.c_str());
    bson_append_finish_object(query);

    bson_append_start_object(query, "$orderby");
    bson_append_int(query,"priority",-1);
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentColumn::get_id_list_fields_bson(bson* query)
{
    bson_init(query);
    bson_append_int(query,"c_id",1);
    bson_append_int(query,"_id",0);
    bson_finish( query );
    return;
}

void ContentColumn::get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "column.id", _column_id.c_str() );

    bson_append_start_object(query, "id");
    bson_append_start_array(query, "$in");
    map<string,vector<download_info_t> >::const_iterator iter = m_id_download_info.begin();
    for( int j=0; iter !=m_id_download_info.end(); ++iter,++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_string(query, index, (*iter).first.c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_finish( query );
    return;
}

void ContentColumn::get_count_query_bson(bson* query)
{
    bson_init( query );
    bson_append_string(query, "content_type", _type.c_str() );
    bson_append_string(query, "column.id", _column_id.c_str() );
    bson_finish( query );
    return;
}

void ContentColumn::get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init( query );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "column.id", _column_id.c_str() );

    bson_append_start_object(query, "id");
    bson_append_start_array(query, "$in");
    map<string,vector<download_info_t> >::const_iterator iter = m_id_download_info.begin();
    for( int j=0; iter !=m_id_download_info.end(); ++iter,++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_string(query, index, (*iter).first.c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_finish(query);
    return;
}

void ContentColumn::get_soft_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "column.id", _column_id.c_str() );
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}

void ContentColumn::get_soft_query_command(bson* command)
{
    bson_init(command);
    bson_append_string(command, "aggregate", MCP_MONGO_SOFT_COL.c_str() );
    bson_append_start_array(command,"pipeline");
        bson_append_start_object(command,"0");
        bson_append_start_object(command,"$match");
        bson_append_string(command, "content_type", _type.c_str() );
        bson_append_string(command,"column.id", _column_id.c_str() );
        bson_append_finish_object(command);
        bson_append_finish_object(command);

        bson_append_start_object(command,"1");
        bson_append_string(command,"$unwind", "$column");
        bson_append_finish_object(command);

        bson_append_start_object(command,"2");
        bson_append_start_object(command,"$match");
        //bson_append_string(command, "content_type", _type.c_str() );
        bson_append_string(command,"column.id", _column_id.c_str() );
        bson_append_finish_object(command);
        bson_append_finish_object(command);

        bson_append_start_object(command,"3");
        bson_append_start_object(command,"$sort");
        bson_append_int(command,"column.p",-1);
        bson_append_finish_object(command);
        bson_append_finish_object(command);
    bson_append_finish_object(command);
    bson_finish(command);
    return;
}

void ContentColumn::get_soft_query_bson(const vector<string>& id_list, bson* query)
{   
    char index[8];
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator it = id_list.begin();
    for(int i=0; it!=id_list.end(); ++it,++i){
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        UB_LOG_DEBUG("push id[%s]",it->c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);
    bson_finish(query);
    return;
}

void ContentColumn::get_android_theme_query_bson(const vector<string>& id_list, bson* query)
{
    char index[8];
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator it = id_list.begin();
    for(int i=0; it!=id_list.end(); ++it,++i){
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        UB_LOG_DEBUG("push id[%s]",it->c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);
    bson_finish(query);
    return;
}

void ContentColumn::get_android_theme_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "column.id", _column_id.c_str() );
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}

void ContentColumn::get_ios_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "column.id", _column_id.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "gdate", -1 );
    }else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }else if(  _priority == 3 ){
        bson_append_int( query, "mdate", -1 );
	}
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}

void ContentColumn::get_ios_query_bson(const vector<string>& id_list, bson* query)
{
    char index[8];
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator it = id_list.begin();
    for(int i=0; it!=id_list.end(); ++it,++i){
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        UB_LOG_DEBUG("push id[%s]",it->c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);
    bson_finish(query);
    return;
}

void ContentColumn::getOnlineGameQueryBson(bson *query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "column.id", _column_id.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "gdate", -1 );
    }else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }else if(  _priority == 3 ){
        bson_append_int( query, "mdate", -1 );
    }
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}

void ContentColumn::getOnlineGameCommand(bson* command)
{
    bson_init(command);
    bson_append_string(command, "aggregate", MCP_ONLINE_GAME_COL.c_str() );
    bson_append_start_array(command,"pipeline");
        bson_append_start_object(command,"0");
        bson_append_start_object(command,"$match");
        bson_append_string(command, "content_type", _type.c_str() );
        bson_append_string(command,"column.id", _column_id.c_str() );
        bson_append_finish_object(command);
        bson_append_finish_object(command);

        bson_append_start_object(command,"1");
        bson_append_string(command,"$unwind", "$column");
        bson_append_finish_object(command);

        bson_append_start_object(command,"2");
        bson_append_start_object(command,"$match");
        bson_append_string(command,"column.id", _column_id.c_str() );
        bson_append_finish_object(command);
        bson_append_finish_object(command);

        bson_append_start_object(command,"3");
        bson_append_start_object(command,"$sort");
        bson_append_int(command,"column.p",-1);
        bson_append_finish_object(command);
        bson_append_finish_object(command);

        bson_append_start_object(command, "4");
        bson_append_int(command, "$skip", _start_pos);
        bson_append_finish_object(command);

        bson_append_start_object(command, "5");
        bson_append_int(command, "$limit", _unit_num);
        bson_append_finish_object(command);
    bson_append_finish_object(command);
    bson_finish(command);
    return;
}
