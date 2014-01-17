#include "content_info.h"
#include "content_common.h"
#include "duokoo_mysql.h"
#include "grab.h"
#include <sstream>

using namespace content;
using namespace std;

int ContentInfo::get_content_detail(content_detail_t& detail)
{
    UB_LOG_TRACE( "get_content_detail start" );
    int res(0);
    res = this->get_basic_info(detail);
    if(res!=0){
        UB_LOG_FATAL("get_basic_info failed, id[%s], [%s:%d]", _id.c_str(), __FILE__, __LINE__ );
        return -1;
    }
    res = this->get_download_info(detail.download_info);
    if(res!=0){
        UB_LOG_NOTICE("get_download_info failed, id[%s], [%d], [%s:%d]",
                                                _id.c_str(), _adapt_type, __FILE__, __LINE__ );
    }
    UB_LOG_TRACE( "get_content_detail end" );
    return 0;
}

int ContentInfo::get_basic_info(content_detail_t& detail)
{
    UB_LOG_TRACE( "get_basic_info start" );
    if(_conn==NULL){
		UB_LOG_TRACE( "mongo conn is NULL");
        UB_LOG_FATAL( "mongo connnection is NULL" );
        return -5;
    }
    detail.id = _id;
    int res(0);
    bson *query, *fields;
    bson bson_out[1];
	UB_LOG_TRACE( "basic 111111111111");
    query = get_basic_query_bson();
    fields = get_basic_fields_bson();
	UB_LOG_TRACE( "basic 2222222222222");
	UB_LOG_TRACE( "basic 2222222222222");
	UB_LOG_TRACE( "basic 2222222222222");
	UB_LOG_TRACE( "basic 2222222222222");
	UB_LOG_TRACE( "basic 2222222222222");
	UB_LOG_TRACE( "basic 2222222222222");
	UB_LOG_TRACE( "basic 2222222222222");
    string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_CONTENT_COL;  
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );
	UB_LOG_TRACE( "basic 3333333333333");
    if( res!=MONGO_OK ){
	UB_LOG_TRACE( "basic 44444444444444");
        UB_LOG_FATAL("find content basic info failed, id[%s], [%s:%d]", _id.c_str(), __FILE__, __LINE__ );
        free_data(NULL, query, fields, NULL);
        return -1;
    }
	UB_LOG_TRACE( "basic 55555555555");
    get_basic_info_from_bson(bson_out,detail);
	UB_LOG_TRACE( "basic 666666666666");
    free_data(NULL, query, fields, bson_out);
    UB_LOG_TRACE( "get_basic_info end" );
    return 0;
}

int ContentInfo::get_download_info(vector<download_detail_t>& download_list)
{
    UB_LOG_TRACE( "get_download_info start" );
    if(_conn==NULL){
        UB_LOG_FATAL( "mongo connnection is NULL" );
        return -5;
    }
    bson *query, *fields;
    mongo_cursor *cursor=NULL;
    query = get_download_query_bson();
    fields = get_download_fields_bson();
    string ns = MCP_MONGODB_NAME+"."+MCP_MONGO_DATA_COL;
    cursor = mongo_find( _conn, ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_FATAL("find content download info failed, id[%s], [%s:%d]", _id.c_str(), __FILE__, __LINE__ );
        free_data(NULL, query, fields, NULL);
        return -1;
    }
    UB_LOG_TRACE( "++++++++111111111");
    get_download_info_from_cursor(cursor,download_list);
    UB_LOG_TRACE( "++++++++222222");
    free_data(cursor, query, fields, NULL);
    UB_LOG_TRACE( "get_download_info end" );
    return 0;
}

int ContentInfo::get_grant_info()
{
    UB_LOG_TRACE( "get_grant_info start" );
    UB_LOG_TRACE( "get_grant_info end" );
    return 0;
}

int ContentInfo::getGrabList(int32_t& count, vector<grab_t>& grab_list)
{
    UB_LOG_TRACE( "ContentInfo::getGrabList start" );
    string query_sql("SELECT `id`, `name`, `c_id`, `info`, `start_time`, `end_time`, `amount`, `probability` "
                     "FROM `MCP`.`mcp_content_grab` "
                     "WHERE `c_id` = " + _id + " AND `enable` = 1 "
                     "ORDER BY `start_time` DESC " );
    UB_LOG_DEBUG("%s",query_sql.c_str());
    //DuokooMysql mysql;
    DuokooMysql mysql("grab_mysql");
    count=mysql.query(query_sql);
    if(count<0){
        UB_LOG_FATAL( "sql[%s] [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
        return -5;
    }
    grab_list.clear();
    grab_t grab_info;
    for(int i=0; i<count; ++i){
        grab_info.clear();
        grab_info.id=atoi(mysql.getResult(i,0).c_str());
        grab_info.name=mysql.getResult(i,1);
        grab_info.c_id=mysql.getResult(i,2);
        grab_info.info=mysql.getResult(i,3);
        grab_info.start_time=mysql.getResult(i,4);
        grab_info.end_time=mysql.getResult(i,5);
        grab_info.amount=atoi(mysql.getResult(i,6).c_str());
        grab_info.probability=atof(mysql.getResult(i,7).c_str());
        Grab grab(grab_info.id);
        grab_info.send_amount=grab.getSendAmount();
        grab_list.push_back(grab_info);
    }
    UB_LOG_TRACE( "ContentInfo::getGrabList end" );
    return 0;
}

void ContentInfo::get_basic_info_from_bson(bson* bson_out, content_detail_t& detail)
{
    bson_iterator it[1];
    bson_iterator_init(it,bson_out);
    const char* key;
    stringstream ss;
    while(bson_iterator_next(it)){
		ss.clear();
        key=bson_iterator_key(it);
        if(0==strcmp(key,"name")){
            detail.name=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"content_type")){
            detail.type=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"class")){
             this->get_class_info_from_bson(it,detail);
        }
        else if(0==strcmp(key,"ext_id")){
            detail.ext_id=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"info")){
            detail.abstract=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"version")){
            detail.version=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"method")){
            detail.method=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"star")){
            ss.clear();
            ss<<bson_iterator_double(it);
            ss>>detail.star;
        }
        else if(0==strcmp(key,"network_id")){
            ss.clear();
            ss<<bson_iterator_int(it);
            UB_LOG_DEBUG( "network:[%d]", bson_iterator_int(it));
            ss>>detail.network;
            UB_LOG_DEBUG( "network:[%s]", detail.network.c_str());
        }
        else if(0==strcmp(key,"game_code")){
            detail.code=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"grant_id")){
            detail.grant_info.id=bson_iterator_string(it);
            mcp_content_map::get_grant_info(detail.grant_info.id, detail.grant_info);
        }
        else if(0==strcmp(key,"cp_id")){
            detail.cp_info.id=bson_iterator_string(it);
            UB_LOG_DEBUG( "cp_info.id is:[%s]", detail.cp_info.id.c_str());
            mcp_content_map::get_cp_name(detail.cp_info.id, detail.cp_info.name);
        }
        else if(0==strcmp(key,"img_info")){
            this->get_img_info_from_bson(it,detail);
        }
        else if(0==strcmp(key,"modify_date")){
            detail.mod_date = bson_iterator_string(it);
        }
        else if(0==strcmp(key,"logdate")){
            detail.log_date = this->convert_mongo_ts_to_string(it);
        }
        else if(0==strcmp(key,"default_charge_typeid")){
            detail.dft_charge_type = bson_iterator_string(it);
        }
        else if(0==strcmp(key,"default_charge_value")){
            detail.dft_charge_val = bson_iterator_string(it);
        }
        else if(0==strcmp(key,"enName")){
            detail.enName = bson_iterator_string(it);
        }
        else if(0==strcmp(key,"language")){
            detail.language = bson_iterator_string(it);
        }
        else if(0==strcmp(key,"adapt")){
            detail.adapt = bson_iterator_string(it);
        }
		else if( 0 == strcmp(key,"package_name")){
			detail.package_name = bson_iterator_string(it);
		}    
		else if( 0 == strcmp(key,"version")){
			detail.version = bson_iterator_string(it);
		}    
		else if( 0 == strcmp(key,"version_appinfo")){
			detail.version_appinfo = bson_iterator_string(it);
		}    
		else if( 0 == strcmp(key,"version_code")){
			detail.version_code = bson_iterator_string(it);
		}    
		else if( 0 == strcmp(key,"sdk_version")){
			int sdk_version = bson_iterator_int(it);
			stringstream s1;
			s1 << sdk_version;
			detail.sdk_version = s1.str();
		}    
		else if( 0 == strcmp(key,"ptype_1")){
			int iptype_0 =bson_iterator_int(it);
			stringstream s1;
			s1 << iptype_0;
			detail.ptype_0 = s1.str();
            UB_LOG_DEBUG( "ptype_0 is %s", detail.ptype_0.c_str());
		}    
		else if( 0 == strcmp(key,"ptype_2")){
			int iptype_1 =bson_iterator_int(it);
			stringstream s2;
			s2 << iptype_1;
			detail.ptype_1 = s2.str();
            UB_LOG_DEBUG( "ptype_0 is %s", detail.ptype_1.c_str());
		}    
		else if( 0 == strcmp(key,"ptype_3")){
			int iptype_2 =bson_iterator_int(it);
			stringstream s3;
			s3 << iptype_2;
			detail.ptype_2 = s3.str();
            UB_LOG_DEBUG( "ptype_0 is %s", detail.ptype_2.c_str());
		}    
		else if( 0 == strcmp(key,"ptype_4")){
			int iptype_3 =bson_iterator_int(it);
			stringstream s4;
			s4 << iptype_3;
			detail.ptype_3 = s4.str();
            UB_LOG_DEBUG( "ptype_0 is %s", detail.ptype_3.c_str());
		}    
		else if( 0 == strcmp(key,"ptype_5")){
			int iptype_4 =bson_iterator_int(it);
			stringstream s5;
			s5 << iptype_4;
			detail.ptype_4 = s5.str();
            UB_LOG_DEBUG( "ptype_0 is %s", detail.ptype_4.c_str());
		}    
        // [s] add by gwj for mcp_content_pub_prop
		else if( 0 == strcmp(key,"pub_prop_ids"))
		{
		    detail.pub_prop_ids = bson_iterator_string(it);
		    UB_LOG_DEBUG( "pub_prop_ids is %s", detail.pub_prop_ids.c_str());
		}
        // [e] add by gwj for mcp_content_pub_prop
		else{  //do nothing, continue;
		}
	}
}

void ContentInfo::get_download_info_from_cursor(mongo_cursor* cursor, vector<download_detail_t>& download_list)
{
	const bson* result;
	const char* key;
	bson_iterator it[1];
	download_detail_t download_info;
	stringstream ss;
	while(mongo_cursor_next(cursor)==MONGO_OK){
		download_info.clear();
		result = mongo_cursor_bson( cursor );
		bson_iterator_init( it, result);
		while(bson_iterator_next(it)){
			key = bson_iterator_key(it);
			UB_LOG_TRACE("key is %s", key);
			if(0==strcmp(key,"id")){
				download_info.id=bson_iterator_string(it);
			}
			else if(0==strcmp(key,"path_url")){
				download_info.url=bson_iterator_string(it);
				UB_LOG_TRACE("url is %s", download_info.url.c_str());
			}
			else if(0==strcmp(key,"path_type")){
				download_info.url_type=bson_iterator_string(it);
			}
			else if(0==strcmp(key,"media_format_id")){
				download_info.media_format=bson_iterator_string(it);
			}
			else if(0==strcmp(key,"size")){
				ss.clear();
				ss<<bson_iterator_int(it);
				UB_LOG_DEBUG("size:[%d]",bson_iterator_int(it));
				ss>>download_info.size;
				UB_LOG_DEBUG("size:[%s]",download_info.size.c_str());
			}
			else if(0==strcmp(key,"mtype")){
				download_info.mtype=bson_iterator_string(it);
			}
			else if(0==strcmp(key,"charge_typeid")){
				download_info.charge_id=bson_iterator_string(it);
			}
			else if(0==strcmp(key,"value")){
				ss.clear();
				ss<<bson_iterator_int(it);
				UB_LOG_DEBUG("value:[%d]",bson_iterator_int(it));
				ss>>download_info.charge_value;
				UB_LOG_DEBUG("value:[%s]",download_info.charge_value.c_str());
			}
			/* 20130328  mongodb.mcp.mcp_content_data新增channel_info字段 beg*/
			else if(0==strcmp(key,"channel_info"))
			{
			  UB_LOG_DEBUG("channel_info:[%s]",bson_iterator_string(it));  
				if( strcmp(bson_iterator_string(it),"TEMP") == 0 )
				{
					download_info.apk_flag = "1";
					UB_LOG_DEBUG("apk_flag:[%s],LINE=%d",download_info.apk_flag.c_str(),__LINE__);
				}
				else
				{
					download_info.apk_flag = "0";
					UB_LOG_DEBUG("apk_flag:[%s]",download_info.apk_flag.c_str());
				}
			}
			/* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 end*/
			else{
			}
		}
		download_list.push_back(download_info);
		UB_LOG_DEBUG("downloadlist ++");
	}
}

void ContentInfo::get_class_info_from_bson(bson_iterator* it, content_detail_t& detail)
{
	bson_iterator sub[1];
	bson_iterator_subiterator( it, sub );
	bson_iterator subsub[1];
	class_info_t class_info;
	while( bson_iterator_next(sub) ){
		bson_iterator_subiterator( sub, subsub );
		class_info.clear();
		while( bson_iterator_next(subsub) ){
			if( strcmp(bson_iterator_key(subsub),"id")==0 ){
				class_info.id=bson_iterator_string(subsub);
			}
		}
		mcp_content_map::get_class_info(class_info.id, class_info);
		detail.class_info.push_back(class_info);
	}
}

void ContentInfo::get_img_info_from_bson(bson_iterator* it, content_detail_t& detail)
{
	bson_iterator sub[1];
	bson_iterator_subiterator( it, sub );
	bson_iterator subsub[1];
	img_info_t img_info;
	while( bson_iterator_next(sub) ){
		bson_iterator_subiterator( sub, subsub );
		img_info.clear();
		while( bson_iterator_next(subsub) ){
			if( strcmp(bson_iterator_key(subsub),"img_type")==0 ){
				img_info.type=bson_iterator_string(subsub);
			}
			else if( strcmp(bson_iterator_key(subsub),"img_url")==0 ){
				img_info.url=bson_iterator_string(subsub);
			}
		}
		detail.img_info.push_back(img_info);
	}
}

bson* ContentInfo::get_basic_query_bson()
{
	UB_LOG_TRACE( "aaaaaaaaaaaa");
	bson_init(_query);
	bson_append_string(_query,"id",_id.c_str());
	bson_finish(_query);
	UB_LOG_TRACE( "bbbbbbbbbbb");
	return _query;
}

bson* ContentInfo::get_basic_fields_bson()
{
	UB_LOG_TRACE( "cccccccccc");
	bson_init(_fields);
	bson_finish(_fields);
	UB_LOG_TRACE( "dddddddddd");
	return _fields;
}

bson* ContentInfo::get_download_query_bson()
{
	char index[8];
	bson_init(_query);
	bson_append_string(_query,"c_id",_id.c_str());

	// [s] add by gwj for only use DuoKu channel
    bson_append_start_object(_query,"channel_info");
    bson_append_start_array(_query, "$in");
    bson_append_string(_query, "0", "DuoKu");
    bson_append_string(_query, "1", "TEMP");
    bson_append_finish_array(_query);
    bson_append_finish_object(_query);
	// [e] add by gwj for only use DuoKu channel

	//bson_append_start_object(_query,"stype_id");
	bson_append_start_object(_query,"adapt_info");
	bson_append_start_array(_query, "$in");
	vector<string>::const_iterator iter = _adapt_id.begin();
	for( int j=0; iter !=_adapt_id.end(); ++iter,++j ){
		memset( index, 0x00, sizeof(index) );
		UB_LOG_DEBUG("stype_id[%s]", iter->c_str());

		sprintf(index, "%d", 2*j);
		bson_append_start_object(_query, index);
		bson_append_int(_query,"stype", _adapt_type);
		bson_append_string(_query, "stype_id", iter->c_str());
		bson_append_finish_object(_query);

		sprintf(index, "%d", 2*j+1);
		bson_append_start_object(_query, index);
		bson_append_string(_query, "stype_id", iter->c_str());
		bson_append_int(_query,"stype", _adapt_type);
		bson_append_finish_object(_query);
	}
	bson_append_finish_object(_query);
	bson_append_finish_object(_query);
	//bson_append_string(_query,"stype_id",_adapt_id.c_str());
	bson_finish(_query);
	bson_print( _query );
	return _query;
}

bson* ContentInfo::get_download_fields_bson()
{
	bson_init(_fields);
	bson_finish(_fields);
	return _fields;
}

void ContentInfo::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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

string ContentInfo::convert_mongo_ts_to_string(bson_iterator* it)
{
	time_t ts= bson_iterator_time_t(it);
	struct tm *t;
	t = gmtime(&ts);
	string time_string;
	char c_time[32];
	memset(c_time, 0x00, sizeof(c_time));
	sprintf( c_time, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec );
	time_string = c_time;
	return time_string;
}
