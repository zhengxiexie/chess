#include "game_package_version.h"

using namespace std;
using namespace content;

int GamePackageVersion::get_cid(string& cid, bool with_versioncode)
{
	UB_LOG_TRACE( "game get cid by package_name and version_code start" );
	if(_conn==NULL){
		UB_LOG_FATAL( "mongo connnection is NULL" );
		return -5; 
	} 

	int res(0);
	bson query[1], fields[1];
	bson bson_out[1];
	get_query_bson(query,with_versioncode);
	get_fields_bson(fields);

	//UB_LOG_TRACE( "1111" );
	string ns = MCP_MONGODB_NAME+"."+MCP_APPINFO_COL;    
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );

	if( res!=MONGO_OK ){
		//UB_LOG_FATAL("find appinfo failed, packagename[%s],versioncode[%s], [%s:%d]", _packagename.c_str(),_versioncode.c_str(), __FILE__, __LINE__ );
		free_data(NULL, query, fields, NULL);
		return -1;
	}

	get_cid_from_bson(bson_out,cid);
	free_data(NULL, query, fields, bson_out);
	UB_LOG_TRACE( "game cid by package_name and version_code end" );
	return 0;
}

void GamePackageVersion::get_query_bson(bson* query, bool with_version)
{
	bson_init(query);
	
	bson_append_start_object( query, "$query" );
	bson_append_string( query, "package_name", _packagename.c_str());

	if(with_version)
	{
		bson_append_int(query,"version_code", atoi(_versioncode.c_str()));
		bson_append_finish_object( query );
	}
	else
	{
		bson_append_finish_object( query );
		bson_append_start_object( query, "$orderby" );
		bson_append_int( query, "version_code", -1 );
		bson_append_finish_object(query );
	}

	bson_finish(query);
	return;
}

void GamePackageVersion::get_fields_bson(bson* fields)
{
	bson_init(fields);
	bson_finish(fields);
	return;
}

void GamePackageVersion::get_cid_from_bson(const bson* result, string& cid)
{
	content::get_cid_from_bson(result, cid);
	return;
}


void GamePackageVersion::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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
