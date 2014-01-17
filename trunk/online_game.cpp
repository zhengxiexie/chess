#include "online_game.h"

using namespace content;

int OnlineGame::getInfo(online_game_t& info)
{
    UB_LOG_TRACE( "OnlineGame::getInfo start" );
    if(_conn==NULL){
        UB_LOG_FATAL( "mongo connnection is NULL" );
        return -5;
    }
    info.id=_id;
    int res(0);
    bson query[1], fields[1];
    bson bson_out[1];
    this->getQueryBson(query);
    this->getFieldsBson(fields);
    string ns = MCP_MONGODB_NAME+"."+MCP_ONLINE_GAME_COL;
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );
    if( res!=MONGO_OK ){
        UB_LOG_FATAL("find content basic info failed, id[%s], [%s:%d]", _id.c_str(), __FILE__, __LINE__ );
        free_data(NULL, query, fields, NULL);
        return -1;
    }
    this->getDetailFromBson(bson_out,info);
    free_data(NULL, query, fields, bson_out);
    UB_LOG_TRACE( "OnlineGame::getInfo end" );
    return 0;
}

void OnlineGame::getDetailFromBson(bson* result, online_game_t& info)
{
    content::getOnlineGameFromBson(result, info);
    return;
}

void OnlineGame::getQueryBson(bson* query)
{
    bson_init(query);
    bson_append_string(query, "id", _id.c_str() );
    bson_finish(query);
    return ;
}

void OnlineGame::getFieldsBson(bson* fields)
{
    bson_init(fields);
    bson_finish(fields);
    return;
}
