#include "content_topic.h"

using namespace content;
using namespace std;

void ContentTopic::set_id(const string& topic_id)
{
    _topic_info.id = topic_id;
    mcp_content_map::get_topic_info(_topic_info.id, _topic_info);
}

int ContentTopic::get_topic_info(topic_info_t& topic_info)
{
    topic_info = _topic_info;
    return 0;
}

int ContentTopic::get_topic_info(const string& topic_id, topic_info_t& topic_info)
{
    mcp_content_map::get_topic_info(topic_id, topic_info);
    return 0;
}

int ContentTopic::get_column_list(int32_t& count, vector<column_info_t>& list)
{
    UB_LOG_TRACE( "get_column_list start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    get_count_query_bson(query);

    count = mongo_count(_conn,"mcp","mcp_topic_col_map",query);
    free_data( NULL, query, NULL, NULL );
    if(count<0){
        UB_LOG_FATAL( "count mcp_topic_col_map failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }
    else if( count==0){
        return 0;
    }

    get_column_query_bson(query);
    get_column_fields_bson(fields);

    cursor = mongo_find( _conn, "mcp.mcp_topic_col_map", query, fields, _page_size, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    list.clear();
    column_info_t column_info;
    const bson* result;
    const char* key;
    bson_iterator it[1];
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        column_info.clear();
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while( bson_iterator_next(it) ){
            key = bson_iterator_key(it);
            if( 0 == strcmp(key,"column")){
                column_info.id=bson_iterator_string(it);
                mcp_content_map::get_column_info(column_info.id, column_info);
            }
        }
        list.push_back(column_info);
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_column_list end" );
    return 0;
}

int ContentTopic::get_neighbour_topic(string& before, string& next)
{
    before.clear(),next.clear();
    vector<string>::iterator it = mcp_content_map::m_type_topic_list[_topic_info.type_id].begin();
    while(it!=mcp_content_map::m_type_topic_list[_topic_info.type_id].end()){
        if(*it==_topic_info.id){
            if(++it!=mcp_content_map::m_type_topic_list[_topic_info.type_id].end()){
                next=*it;
                break;
            }
        }
        else{
            before=*it;
            ++it;
        }
    }
    return 0;
}

void ContentTopic::get_column_query_bson(bson* query)
{
    bson_init(query);
    bson_append_start_object(query, "$query" );
    bson_append_string(query, "topic", _topic_info.id.c_str() );
    bson_append_finish_object(query );
       
    bson_append_start_object(query, "$orderby" );
    bson_append_int(query, "priority", -1 );
    bson_append_finish_object(query );
    bson_finish(query);
    return;
}

void ContentTopic::get_column_fields_bson(bson* fields)
{
    bson_init(fields);
    bson_append_int(fields,"_id",0);
    bson_append_int(fields,"column",1);
    bson_finish(fields);
    return;
}

void ContentTopic::get_count_query_bson(bson* query)
{
    bson_init(query);
    bson_append_string(query, "topic", _topic_info.id.c_str() );
    bson_finish(query);
    return;
}

void ContentTopic::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
{
    mongo_cursor_destroy( cursor );
    if( query != NULL && query->data != NULL ){
        bson_destroy( query );
    }
    if( fields != NULL && fields->data != NULL ){
        bson_destroy( fields );
    }
    if( bson_result != NULL && bson_result->data != NULL ){
       bson_destroy( bson_result );
    }
}
