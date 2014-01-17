#include "game_score.h"

using namespace std;
using namespace content;

int GameScore::get_info(game_score_t& info)
{
    UB_LOG_TRACE( "get game score start" );
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
    string ns = MCP_MONGODB_NAME+"."+ MCP_MONGO_SCORE_COL;
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );

    if( res!=MONGO_OK ){
        UB_LOG_FATAL("find game score failed, c_id[%s], [%s:%d]", _c_id.c_str(), __FILE__, __LINE__ );
        free_data(NULL, query, fields, NULL);
        return -1;
    }

    get_game_score_from_bson(bson_out,info);
    free_data(NULL, query, fields, bson_out);
    UB_LOG_TRACE( "get game score end" );
    return 0;
}

void GameScore::get_query_bson(bson* query)
{
    bson_init(query);
	int32_t id_type;
	stringstream ss;
	ss<<this->_type;
	ss>>id_type;
	if(1 == id_type)
	{
		bson_append_string(query, "imei", _id.c_str());
	}
	else if(2 == id_type)
	{
		bson_append_string(query, "u_id", _id.c_str());
	}
	else
	{
		bson_finish(query);
		return;
	}

    bson_append_string(query,"c_id", _c_id.c_str());
	bson_finish(query);
	return;
}

void GameScore::get_fields_bson(bson* fields)
{
	bson_init(fields);
	bson_finish(fields);
	return;
}

void GameScore::get_game_score_from_bson(bson* result, game_score_t& info)
{
	content::get_game_score_from_bson(result, info);
	return;
}


void GameScore::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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
