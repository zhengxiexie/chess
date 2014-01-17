#include "content_cp.h"
#include "ub_server.h"

using namespace std;
using namespace content;

int ContentCp::get_ios_content_list(int32_t& count, vector<ios_content_t>& list)
{
    return ContentList::get_ios_content_list(count,list);
}

int ContentCp::get_ios_content_list(vector<ios_content_t>& list)
{
    UB_LOG_TRACE( "get_ios_content_list start" );
    return ContentList::get_ios_content_list(list);
    UB_LOG_TRACE( "get_ios_content_list end" );
    return 0;
}
void ContentCp::get_count_query_bson(bson* query)
{
    bson_init( query );
    bson_append_string(query, "content_type", _type.c_str() );
    bson_append_string(query, "cp_id", _cp_id.c_str() );
    bson_finish( query );
    return;
}



void ContentCp::get_basic_query_bson(bson* query)
{
	bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "cp_id", _cp_id.c_str() );
    bson_append_finish_object( query );

	bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "priority", -1 );
    }
    else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    else if( _priority == 3 ){
        bson_append_int( query, "logdate", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}
/*
int ContentCp::get_id_list(vector<string>$ id_list)
{
	UB_LOG_TRACE( "get_id_list start" );
	bson query[1], fields[1];
	mongo_cursor *cursor = NULL;
	const bson* result;

	this->get_id_list_query_bson(query);
	this->get_id_list_fields_bson(fields);

	cursor = mongo_find( _conn, "mcp.mcp_content_class_list", query, fields, _unit_num, _start_pos, 0 );
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
*/

void ContentCp::get_ios_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "cp_id", _cp_id.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "gdate", -1 );
    }
    else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}
