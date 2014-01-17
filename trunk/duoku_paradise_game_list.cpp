#include "duoku_paradise_game_list.h"
#include "ub_server.h"
#include <exception>

using namespace content;
using namespace std;

int DuokuParadiseGameList::get_h5_game_list(int &count, vector<duoku_paradise_game_t>& vd, const int type)
{
	UB_LOG_TRACE("get paradise game list start");
	int res(0);

	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}

	bson query[1], fields[1];
	mongo_cursor *cursor = NULL;

	count = this->get_h5_game_count(type);

	this->get_h5_game_query_bson(query,type);
	this->get_h5_game_fields_bson(fields);

	string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_CONTENT_COL;
	cursor = mongo_find( _conn, ns.c_str(), query, fields, _unit_num, _unit_num * (_unit_times-1), 0 );
	if( cursor == NULL ){
		UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
		free_data( NULL, query, fields, NULL );
		return -1;
	}

	duoku_paradise_game_t dp_game;
	while( mongo_cursor_next( cursor ) == MONGO_OK ){
		UB_LOG_TRACE("---------------in");
		dp_game.clear();
		res=this->get_h5_game_from_cursor(cursor,dp_game);
		if(res==0){
			vd.push_back(dp_game);
		}
	}
	free_data( cursor, query, fields, NULL );

	UB_LOG_TRACE("get paradise game list end");
	return 0;
}

int DuokuParadiseGameList::get_h5_game_list_by_type(int &count, vector<duoku_paradise_game_t>& vd, const int type)
{
	UB_LOG_TRACE("get paradise game list start");
	int res(0);

	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}

	bson query[1], fields[1];
	mongo_cursor *cursor = NULL;

	//count = this->get_h5_game_count(type);
	count = this->get_h5_game_count_by_type(type);

	//this->get_h5_game_query_bson(query,type);
	this->get_h5_game_query_bson_by_type(query,type);
	this->get_h5_game_fields_bson(fields);

	string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_CONTENT_COL;
	cursor = mongo_find( _conn, ns.c_str(), query, fields, _unit_num, _unit_num * (_unit_times-1), 0 );
	if( cursor == NULL ){
		UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
		free_data( NULL, query, fields, NULL );
		return -1;
	}

	duoku_paradise_game_t dp_game;
	while( mongo_cursor_next( cursor ) == MONGO_OK ){
		UB_LOG_TRACE("---------------in");
		dp_game.clear();
		res=this->get_h5_game_from_cursor(cursor,dp_game);
		if(res==0){
			vd.push_back(dp_game);
		}
	}
	free_data( cursor, query, fields, NULL );

	UB_LOG_TRACE("get paradise game list end");
	return 0;
}
void DuokuParadiseGameList::get_h5_game_query_bson(bson *query, int type)
{
	bson_init( query );
	bson_append_start_object( query, "$query" );
	bson_append_string( query, "content_type", MCP_H5_GAME.c_str() );
	if(type != 0)
	{
		char col[16];
        sprintf(col,"%d", type);
		bson_append_string( query, "column.id", col );
	}
	bson_append_finish_object( query );

	bson_append_start_object( query, "$orderby" );
	bson_append_int( query, "modify_date", -1 );
	bson_append_finish_object( query );

	bson_finish( query );
	UB_LOG_TRACE("query end");
	return;
}

void DuokuParadiseGameList::get_h5_game_query_bson_by_type(bson *query, int type)
{
	string type_s = DuokooTools::toString(type);
	bson_init( query );
	bson_append_start_object( query, "$query" );
	bson_append_string( query, "content_type", type_s.c_str() );
	/*if(type != 0)*/
	//{
		//char col[16];
        //sprintf(col,"%d", type);
		//bson_append_string( query, "column.id", col );
	/*}*/
	bson_append_finish_object( query );

	bson_append_start_object( query, "$orderby" );
	bson_append_int( query, "modify_date", -1 );
	bson_append_finish_object( query );

	bson_finish( query );
	UB_LOG_TRACE("query end");
	return;
}
void DuokuParadiseGameList::get_h5_game_count_query_bson(bson *query, int type)
{
	bson_init( query );
	bson_append_string( query, "content_type", MCP_H5_GAME.c_str() );
	if(type != 0)
	{   
		char col[16];
		sprintf(col,"%d", type);
		bson_append_string( query, "column.id", col );
	}   

	bson_finish( query );
	UB_LOG_TRACE("query end");
	return;
}

void DuokuParadiseGameList::get_h5_game_count_query_bson_by_type(bson *query, int type)
{
	bson_init( query );
	string type_s = DuokooTools::toString(type);
	bson_append_string( query, "content_type", type_s.c_str() );
	/*if(type != 0)*/
	//{   
		//char col[16];
		//sprintf(col,"%d", type);
		//bson_append_string( query, "column.id", col );
	/*}  */ 

	bson_finish( query );
	UB_LOG_TRACE("query end");
	return;
}
void DuokuParadiseGameList::get_h5_game_fields_bson(bson* fields)
{
	bson_init( fields );
	bson_finish( fields );
	return;
}

void DuokuParadiseGameList::get_h5_game_cid_fields_bson(bson* fields)
{
	bson_init( fields );
	bson_append_int( fields, "id", 1);
	bson_finish( fields );
	return;
}

void DuokuParadiseGameList::get_one_class_info(bson_iterator* it, string &class_id, string &class_name)
{
	bson_iterator sub[1];
	bson_iterator_subiterator( it, sub );
	bson_iterator subsub[1];
	class_info_t class_info;
	int res;

	while( bson_iterator_next(sub) ){
		bson_iterator_subiterator( sub, subsub );
		while( bson_iterator_next(subsub) ){
			if( strcmp(bson_iterator_key(subsub),"id")==0 ){
				class_id = bson_iterator_string(subsub);
				res = mcp_content_map::get_class_info( class_id, class_info );
				class_name = class_info.name;
				if(res == 0){
					return;
				}
			}
		}
	}

	return;
}

void DuokuParadiseGameList::get_h5_img_urls(bson_iterator* it, duoku_paradise_game_t& dp_game)
{
	bson_iterator sub[1];
	bson_iterator_subiterator( it, sub );
	bson_iterator subsub[1];

	int isset = 0;
	string strTmp;
	while( bson_iterator_next(sub) ){
		bson_iterator_subiterator( sub, subsub );
		isset = 0;
		strTmp = "";
		while( bson_iterator_next(subsub) ){
			if( strcmp(bson_iterator_key(subsub),"img_type")==0 ){
				strTmp=bson_iterator_string(subsub);
			}
			else if( strcmp(bson_iterator_key(subsub),"img_url")==0 ){
				string tmp = bson_iterator_string(subsub);
				if(strcmp(strTmp.c_str(),"12")==0){
					dp_game.icon_url = tmp;
				}else if(strcmp(strTmp.c_str(),"13")==0){
					dp_game.img_small_url = tmp;
				}else if(strcmp(strTmp.c_str(),"15")==0){
					dp_game.img_big_url = tmp;
				}
			}

		}
	}
}

int DuokuParadiseGameList::get_h5_game_from_cursor(mongo_cursor * cursor, duoku_paradise_game_t& dp_game)
{
	const bson* result;
	const char* key;
	bson_iterator it[1];
	result = mongo_cursor_bson( cursor );
	bson_iterator_init( it, result);
	while(bson_iterator_next(it)){
		key=bson_iterator_key(it);
		if(0==strcmp(key,"id")){
			dp_game.id=bson_iterator_string(it);
		}else if(0==strcmp(key,"name")){
			dp_game.name=bson_iterator_string(it);
			UB_LOG_TRACE("game name is %s", dp_game.name.c_str());
		}else if(0==strcmp(key,"info")){
			dp_game.intro=bson_iterator_string(it);
		}else if(0==strcmp(key,"class")){
			get_one_class_info(it, dp_game.class_id, dp_game.class_name);
		}else if(0==strcmp(key,"img_info")){
			get_h5_img_urls(it, dp_game);
		}
	}   
	return 0;
}

int DuokuParadiseGameList::get_h5_game_cid_from_cursor(mongo_cursor * cursor, int& cid)
{
	const bson* result;
	const char* key;
	bson_iterator it[1];
	string scid;
	result = mongo_cursor_bson( cursor );
	bson_iterator_init( it, result);

	while(bson_iterator_next(it)){
		key=bson_iterator_key(it);
		if(0 == strcmp(key,"id")){
			scid = bson_iterator_string(it);
			cid = atoi(scid.c_str());
			return 0;
		}
	}
	return 0;
}

void DuokuParadiseGameList::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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

int DuokuParadiseGameList::get_h5_game_count(int type)
{
	UB_LOG_TRACE( "get_H5_game_count start" );
	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	} 
	int res(0);
	bson query[1];
	this->get_h5_game_count_query_bson(query,type);

	res = mongo_count( _conn, MCP_MONGODB_NAME.c_str(), MCP_MONGO_CONTENT_COL.c_str(), query );
	UB_LOG_TRACE("count is %d", res);

	if( res < 0 ){ 
		UB_LOG_FATAL( "mongo_count failed, [%s:%d]", __FILE__, __LINE__ );
	}    

	UB_LOG_TRACE("after mongo count");
	bson_destroy(query);
	UB_LOG_TRACE( "get_H5_game_count end" );
	return res; 
}

int DuokuParadiseGameList::get_h5_game_count_by_type(int type)
{
	UB_LOG_TRACE( "get_H5_game_count start" );
	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	} 
	int res(0);
	bson query[1];
	//this->get_h5_game_count_query_bson(query,type);
	this->get_h5_game_count_query_bson_by_type(query,type);

	res = mongo_count( _conn, MCP_MONGODB_NAME.c_str(), MCP_MONGO_CONTENT_COL.c_str(), query );
	UB_LOG_TRACE("count is %d", res);

	if( res < 0 ){ 
		UB_LOG_FATAL( "mongo_count failed, [%s:%d]", __FILE__, __LINE__ );
	}    

	UB_LOG_TRACE("after mongo count");
	bson_destroy(query);
	UB_LOG_TRACE( "get_H5_game_count end" );
	return res; 
}
void DuokuParadiseGameList::get_h5_one_game_query_bson(bson *query, const string cid)
{
	bson_init(query);
	bson_append_string( query, "id", cid.c_str() );
	bson_finish(query);
}

void DuokuParadiseGameList::get_urls(bson_iterator* it, img_h5_url_t& url_info)
{
	bson_iterator sub[1];
	bson_iterator_subiterator( it, sub );
	bson_iterator subsub[1];

	int isset = 0;
	string strTmp;
	while( bson_iterator_next(sub) ){
		bson_iterator_subiterator( sub, subsub );
		isset = 0;
		strTmp = "";
		while( bson_iterator_next(subsub) ){
			if( strcmp(bson_iterator_key(subsub),"img_type")==0 ){
				strTmp=bson_iterator_string(subsub);
			}
			else if( strcmp(bson_iterator_key(subsub),"img_url")==0 ){
				string tmp = bson_iterator_string(subsub);
				if(strcmp(strTmp.c_str(),"12")==0){
					url_info.icon_url = tmp;
				}else if(strcmp(strTmp.c_str(),"13")==0){
					url_info.xc1_url = tmp;
				}else if(strcmp(strTmp.c_str(),"15")==0){
					url_info.xc2_url = tmp;
				}else if(strcmp(strTmp.c_str(),"14")==0){
					url_info.xq_url_list.push_back(tmp);	
				}

			}
		}	
	}
}

void DuokuParadiseGameList::get_urls_from_bson(bson* result, vector<string>& vs)
{
	const char* key;
	bson_iterator it[1];
	img_h5_url_t url_info;
	bson_iterator_init( it, result);
	while(bson_iterator_next(it))
	{
		key=bson_iterator_key(it);
		if(0==strcmp(key,"img_info"))
		{
			get_urls(it, url_info);	
			break;
		}
	}
	vs.push_back(url_info.icon_url);
	vs.push_back(url_info.xc1_url);
	vs.push_back(url_info.xc2_url);
	for(int i=0; i<(int)url_info.xq_url_list.size(); i++)
	{
		vs.push_back(url_info.xq_url_list[i]);
	}
}

void DuokuParadiseGameList::get_h5_img_urls_by_cid(const string cid, vector<string>& vs)
{
	UB_LOG_TRACE( "get_h5_img_urls_by_cid start" );
	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return ; 
	}
	bson query[1],fields[1],bson_out[1];

	this->get_h5_one_game_query_bson(query, cid);
	this->get_h5_game_fields_bson(fields);

	string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_CONTENT_COL;
	int res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );
	if( res != MONGO_OK )
	{
		UB_LOG_FATAL("mongo error");
		return;
	}

	this->get_urls_from_bson(bson_out, vs);

	UB_LOG_TRACE( "get_h5_img_urls_by_cid end" );
	return;
}

int DuokuParadiseGameList::check_favorite(const string u_id, const string c_id)
{
	int res = 0;
	string fv_key = "favorite_" + u_id;
	redisContext *c;
	c=redisConnPool::getInstance()->getConnection();
	if(c==NULL){
		UB_LOG_FATAL( "get redis connection failed, [%s:%d]", __FILE__, __LINE__ );
		return -5; 
	}

	redisReply *reply;
	reply=(redisReply *)redisCommand( c, "SISMEMBER %s %s", fv_key.c_str(), c_id.c_str());

	if(reply!=NULL){
		if(reply->type==REDIS_REPLY_ERROR){
			UB_LOG_FATAL( "REDIS ERROR[%s]", reply->str );
			redisConnPool::getInstance()->releaseConnection(c);
			return -5; 
		}   
		else{
			res = reply->integer;
		}   
		freeReplyObject(reply);
	}else{
		UB_LOG_FATAL( "get a NULL redis connection, [%s:%d]", __FILE__, __LINE__ );
		redisConnPool::getInstance()->releaseConnection(c);
		return -5;
	}

	redisConnPool::getInstance()->releaseConnection(c);
	return res;
}

void DuokuParadiseGameList::get_rand_query_bson(bson* query, const vector<int> vi)
{
	char index[8];
	char cid[32];
	bson_init( query );
	bson_append_start_object( query, "id" );
	bson_append_start_array( query, "$in" );

	for(int i=0; i<(int)vi.size(); i++)
	{
		sprintf(index,"%d",i);
		sprintf(cid, "%d", vi[i]);
		UB_LOG_TRACE("query cid is %d", vi[i]);
		bson_append_string(query,index,cid);
	}

	bson_append_finish_array( query );
	bson_append_finish_object( query );
	bson_finish(query);
}

void DuokuParadiseGameList::get_cids_query_bson(bson *query)
{
	bson_init( query );
	bson_append_string( query, "content_type", MCP_H5_GAME.c_str() );
	bson_finish(query);
}

int DuokuParadiseGameList::get_cids(vector<int> &vi)
{
	int res;
	bson query[1], fields[1];
	mongo_cursor *cursor = NULL;
	this->get_cids_query_bson(query);
	this->get_h5_game_cid_fields_bson(fields);

	string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_CONTENT_COL;

	// [s] add by gwj
	try
	{
	    cursor = mongo_find( _conn, ns.c_str(), query, fields, 0, 0, 0 );
	}
	catch(...)
	{
	    cursor = NULL;
	    UB_LOG_TRACE("mongo_find throw exception");
	}
	// [e] add by gwj

	if( cursor == NULL ){
		UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
		free_data( NULL, query, fields, NULL );
		return -1;
	}
	int cid;
	while( mongo_cursor_next( cursor ) == MONGO_OK ){
		UB_LOG_TRACE("---------------in");
		res=this->get_h5_game_cid_from_cursor(cursor,cid);
		if(res==0){
			vi.push_back(cid);
		}
	}	

	return (int)vi.size();
}

int DuokuParadiseGameList::get_rand_h5_games(const int count, vector<duoku_paradise_game_t>& vd)
{
	UB_LOG_TRACE("get random paradise game list start");
	mongo_cursor *cursor = NULL;
	int res,i;
	vector<int> vi;
	this->get_cids(vi);
	for(i=0; i<(int)vi.size(); i++)
	{
		UB_LOG_TRACE("cid is %d", vi[i]);
	}

	vector<int> vi_r=commonFunc::randomIntSequence(vi.size());
	int cnt= ((int)vi.size() > count)?count:(int)vi.size();

	for(i=0; i<(int)vi_r.size(); i++)
	{
		UB_LOG_TRACE("rand is %d", vi_r[i]);
	}

	vector<int> vi2;
	for(i=0; i<cnt; i++)
	{
		vi2.push_back(vi[vi_r[i]]);
	}

	for(i=0; i<(int)vi2.size(); i++)
	{   
		UB_LOG_TRACE("rand cid is %d", vi2[i]);
	} 

	bson query[1], fields[1];
	this->get_rand_query_bson(query,vi2);
	this->get_h5_game_fields_bson(fields);

	string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_CONTENT_COL;
	cursor = mongo_find( _conn, ns.c_str(), query, fields, 100, 0, 0 );
	UB_LOG_TRACE("---------------check1");
	if( cursor == NULL ){
		UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
		free_data( NULL, query, fields, NULL );
		return -1; 
	}   

	duoku_paradise_game_t dp_game;
	UB_LOG_TRACE("---------------check2");
	while( mongo_cursor_next( cursor ) == MONGO_OK ){
		UB_LOG_TRACE("---------------in2");
		dp_game.clear();
		res=this->get_h5_game_from_cursor(cursor,dp_game);
		if(res==0){
			vd.push_back(dp_game);
		}   
	}   
	free_data( cursor, query, fields, NULL );
	UB_LOG_TRACE("get random paradise game list end");
	return (int)vd.size();
}
