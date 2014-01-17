#include "content_calender.h"
#include "ub_server.h"

using namespace content;
using namespace std;

int ContentCalender::getCalenderList(vector<calender_t>& vc)
{
	int res = 0;
	UB_LOG_TRACE("get calender list start");
	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return -5; 
	}   

	bson query[1], fields[1];
	mongo_cursor *cursor = NULL;

	this->get_query_bson(query);
	this->get_field_bson(fields);

	string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_CALENDER;
	cursor = mongo_find( _conn, ns.c_str(), query, fields, 0, 0, 0 );
	if( cursor == NULL ){
		UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
		free_data( NULL, query, fields, NULL );
		return -1; 
	}

	calender_t cal;
	while( mongo_cursor_next( cursor ) == MONGO_OK ){
		UB_LOG_TRACE("---------------in");
		cal.clear();
		res=this->get_calender_from_cursor(cursor, cal);
		if(res==0){
			vc.push_back(cal);
		}   
	}

	UB_LOG_TRACE("get paradise game list end");
	return 0;
}

void ContentCalender::get_query_bson(bson* query)
{
	bson_init(query);
	bson_append_start_object(query,"$query");

	//bson_append_start_array(query, "$and");
	
	UB_LOG_TRACE("start time is %s", _start_datetime.c_str());
	bson_append_start_object(query, "enable_date");
	bson_append_string(query,"$gt",_start_datetime.c_str());
	bson_append_finish_object(query);

	UB_LOG_TRACE("end time is %s", _end_datetime.c_str());
	bson_append_start_object(query,"enable_date");
	bson_append_string(query,"$lte",_end_datetime.c_str());
	bson_append_finish_object(query);

	//bson_append_finish_array(query);

	bson_append_finish_object(query);
	bson_finish(query);
	return;
}

void ContentCalender::get_field_bson(bson* fields)
{
	bson_init(fields);
	bson_finish(fields);
	return;
}

int ContentCalender::get_calender_from_cursor(mongo_cursor* cursor, calender_t& cal)
{
	const bson* result;
	const char* key;
	bson_iterator it[1];
	stringstream ss;
	string strtmp;
	result = mongo_cursor_bson( cursor );
	bson_iterator_init( it, result);
	while(bson_iterator_next(it)){
		ss.clear();
		strtmp.clear();
		key=bson_iterator_key(it);
		if(0==strcmp(key,"id")){
			strtmp = bson_iterator_string(it);	
			cal.id =atoi(strtmp.c_str());
		}
		if(0==strcmp(key,"enable_date")){
			cal.enable_date = bson_iterator_string(it);  
		} 	
		if(0==strcmp(key,"content_id")){
			strtmp = bson_iterator_string(it);	
			cal.c_id =atoi(strtmp.c_str());
		}
		if(0==strcmp(key,"type_id")){
			strtmp = bson_iterator_string(it);	
			cal.type =atoi(strtmp.c_str());
		}
		if(0==strcmp(key,"title")){
			cal.title = bson_iterator_string(it);	
		}

	}
	return 0;
}
