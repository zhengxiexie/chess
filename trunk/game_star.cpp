#include "game_star.h"

using namespace std;
using namespace content;

int GameStar::get_star(string& star)
{
    UB_LOG_TRACE( "get game star start" );
    if(_conn==NULL){
        UB_LOG_FATAL( "redis connnection is NULL" );
        return -5;
    }
	
	string key=_packagename;
	key += "_";
	key += _versioncode;
	key += "_";
	key += _imei;

	star = "";
	redisReply * _reply = (redisReply *)redisCommand( _conn, "GET %s ",  key.c_str());
    UB_LOG_TRACE( "key is %s", key.c_str());
	if(_reply ==NULL || _reply->elements == 0)
	{
    	UB_LOG_TRACE( "have no value");
    	UB_LOG_TRACE( "no score with key %s", key.c_str());
	}
	else
	{
		if(_reply->str == NULL)
		{
    		UB_LOG_TRACE( "NULL");
		}
		else
		{
    		UB_LOG_TRACE( "NOT NULL");
			if(_reply->type == REDIS_REPLY_STRING)
			{
    			UB_LOG_TRACE( "value is %s", _reply->str);
				star = _reply->str;
			}
		}
	}
    UB_LOG_TRACE( "get game star end" );
    return 0;
}
/*
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
*/
