#include "appinfo_content.h"
#include "duokoo_tools.h"

using namespace std;
using namespace content;
using namespace DuokooTools;

int AppinfoContent::get_info(appinfo_t& info)
{
	UB_LOG_TRACE( "get app_info start" );
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
	string ns = MCP_MONGODB_NAME+"."+MCP_APPINFO_COL;    
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );

	if( res!=MONGO_OK ){
		UB_LOG_FATAL("find app info failed, id[%s], [%s:%d]", _package_name.c_str(), __FILE__, __LINE__ );
		free_data(NULL, query, fields, NULL);
		return -1;
	}

	get_info_from_bson(bson_out,info);
	free_data(NULL, query, fields, bson_out);
	UB_LOG_TRACE( "get app_info end" );
	return 0;
}

void AppinfoContent::get_query_bson(bson* query)
{
	bson_init(query);
    if(_id==0){
	    bson_append_string(query,"package_name", _package_name.c_str());
    }else{
	    bson_append_string(query,"c_id", toString(_id).c_str());
    }
	bson_finish(query);
	return;
}

void AppinfoContent::get_fields_bson(bson* fields)
{
	bson_init(fields);
	bson_finish(fields);
	return;
}

void AppinfoContent::get_info_from_bson(bson* result, appinfo_t& info)
{
	content::get_appinfo_from_bson(result, info);
	return;
}


void AppinfoContent::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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
