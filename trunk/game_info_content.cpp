#include "game_info_content.h"

using namespace std;
using namespace content;

int GameInfoContent::get_info(game_t& info)
{
	UB_LOG_TRACE( "game get_info start" );
	if(_conn==NULL){
		UB_LOG_FATAL( "mongo connnection is NULL" );
		return -5; 
	} 

	int res(0);
	bson query[1], fields[1];
	bson bson_out[1];
	get_query_bson(query);
	get_fields_bson(fields);

	//UB_LOG_TRACE( "1111" );
	string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_NEWS_COL;    
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );

	if( res!=MONGO_OK ){
		UB_LOG_FATAL("find content basic info failed, id[%s], [%s:%d]", _strGameId.c_str(), __FILE__, __LINE__ );
		free_data(NULL, query, fields, NULL);
		return -1;
	}

	get_info_from_bson(bson_out,info);
	free_data(NULL, query, fields, bson_out);
	UB_LOG_TRACE( "game get_info end" );
	return 0;
}

void GameInfoContent::get_query_bson(bson* query)
{
	bson_init(query);
	bson_append_string(query,"id", _strGameId.c_str());
	bson_finish(query);
	return;
}

void GameInfoContent::get_fields_bson(bson* fields)
{
	bson_init(fields);
	bson_finish(fields);
	return;
}

void GameInfoContent::get_info_from_bson(bson* result, game_t& info)
{
	content::get_game_info_from_bson(result, info);
	return;
}


void GameInfoContent::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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
