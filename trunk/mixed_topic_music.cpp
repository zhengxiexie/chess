#include "mixed_topic_music.h"
#include "ub_server.h"

using namespace std;
using namespace content;

int MTMusicContentList::get_MTMusic_content_list(int &count, vector<music_t>& list)
{
	UB_LOG_TRACE( "get_MTMusic_content_list start" );
	int res(0);
	
	bson query[1], fields[1];
	this->get_query_bson(query);
	this->get_filed_bson(fields);

	UB_LOG_TRACE("page is %d, page size is %d", _page, _page_size);
	UB_LOG_TRACE("ns is %s", MCP_MIXED_TOPIC_MUSIC_COL.c_str());
	string ns = MCP_MONGODB_NAME+"."+MCP_MIXED_TOPIC_MUSIC_COL;
	mongo_cursor *cursor = mongo_find( _conn, ns.c_str(), query, fields, 0, 0, 0 );
	if( cursor == NULL ){
		UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
		free_data( NULL, query, fields, NULL );
		return -1;
	}

	list.clear();
	music_t music_info;

	while( mongo_cursor_next( cursor ) == MONGO_OK ){
		music_info.clear();
		res = this->get_MTMusic_from_cursor(cursor, music_info);
		if(res==0){
			list.push_back( music_info);
			UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", music_info.music_id.c_str(), __FILE__, __LINE__ );
		}

	}

	UB_LOG_TRACE("music list size is %d", (int)list.size());
	free_data( cursor, query, fields, NULL );
	UB_LOG_TRACE( "get_MTMusic_content_list end" );
	return 0;
}
/*
void MTMusicContentList::get_count_query_bson(bson* query)
{
	UB_LOG_TRACE( "get_count_query start" );
	UB_LOG_TRACE( "topic id is %s", _topic_id.c_str() );

	bson_init( query );
	bson_append_start_object( query, "$query" );
	bson_append_string( query, "topic_id", _topic_id.c_str() );
	bson_append_finish_object( query );

	UB_LOG_TRACE( "get_count_query end" );
	return;
}


int MTMusicContentList::get_MTMusic_count()
{
	UB_LOG_TRACE( "get_content_count start" );
	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return -5; 
	}   

	int res(0);
	bson query[1];
	this->get_count_query_bson(query);
	UB_LOG_TRACE( "_conn is %d", &_conn);

	res = mongo_count( _conn, "mcp", "mcp_mixed_topic_music", query );
	UB_LOG_TRACE( "res is %d", res );
	if( res < 0 ){  
		UB_LOG_FATAL( "mongo_count failed, [%s:%d]", __FILE__, __LINE__ );
	}

	bson_destroy(query);
	UB_LOG_TRACE( "get_content_count end" );
	return res;
}
*/
void MTMusicContentList::get_query_bson(bson* query)
{
	bson_init( query );
	bson_append_string( query, "topic_id", _topic_id.c_str() );
	bson_finish(query);

	return;
}

void MTMusicContentList::get_filed_bson(bson* fields)
{
	bson_init( fields );
	bson_finish( fields );
	return;
}

int MTMusicContentList::get_MTMusic_from_cursor(mongo_cursor *cursor, music_t& music_info)
{
	const bson* result;
	const char* key;

	bson_iterator it[1];
	result = mongo_cursor_bson( cursor );
	bson_iterator_init( it, result);

	UB_LOG_TRACE("in from cursor");
	stringstream ss;
	while( bson_iterator_next(it) ){
		ss.clear();
		key = bson_iterator_key(it);
		if(0==strcmp(key,"music_id"))
		{
			music_info.music_id=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"music_name"))
		{
			music_info.songname=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"area_id"))
		{
			music_info.area_id=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"area_name"))
		{
			music_info.area_name=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"cp_id"))
		{
			music_info.cp_id=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"cp_name"))
		{
			music_info.cp_name=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"language_id"))
		{
			music_info.language_id=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"language"))
		{
			music_info.language=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"publish_date"))
		{
			music_info.publish_date=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"intro"))
		{
			music_info.intro=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"lyric_writer"))
		{
			music_info.lyric_writer=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"composer"))
		{
			music_info.composer=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"price"))
		{
			music_info.price=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"discount"))
		{
			music_info.discount=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"download_base"))
		{
			music_info.download_base=bson_iterator_string(it);
		}
		else if(0 == strcmp(key,"lyric_txt"))
		{
			music_info.lyric_txt=bson_iterator_string(it);
		}
	}
	return 0;
}

void MTMusicContentList::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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
