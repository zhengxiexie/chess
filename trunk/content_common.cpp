#include <sstream>
#include "content_common.h"
#include "mongo_connpool.h"
#include "ub_server.h"
#include "duokoo_mysql.h"
#include "content_class.h"
#include "duokoo_tools.h"
#include <iomanip>

// [s] add by gwj for mcp_refactory
#include "refactory/common_simple_buf.h"
// [e] add by gwj for mcp_refactory

using namespace content;
using namespace DuokooTools;

extern mongo_connpool* mongo_pool;

map<string,class_info_t> mcp_content_map::m_class_id_info;
map<string,column_info_t> mcp_content_map::m_column_id_info;
map<string,string> mcp_content_map::m_label_id_name;
map<string,string> mcp_content_map::m_charge_id_name;
map<string,string> mcp_content_map::m_cp_id_name;
map<string,grant_detail_t> mcp_content_map::m_grant_id_info;
map<string,topic_info_t> mcp_content_map::m_topic_id_info;
map<string, vector<string> > mcp_content_map::m_type_topic_list;
map<int,string> mcp_content_map::m_attr_id_name;;

template <typename T>
void content::getClassInfoFromBson(bson_iterator* it, T& info)
{
    bson_iterator sub[1];
    bson_iterator_subiterator( it, sub );
    bson_iterator subsub[1];
    string class_id;
    class_info_t class_info;
    int res;
    while( bson_iterator_next(sub) ){
        bson_iterator_subiterator( sub, subsub );
        class_info.clear();
        while( bson_iterator_next(subsub) ){
            if( strcmp(bson_iterator_key(subsub),"id")==0 ){
                class_id =bson_iterator_string(subsub);
                res = mcp_content_map::get_class_info( class_id, class_info );
                if( res!=0 ){
                    UB_LOG_FATAL( "get_content_class_name failed, class_id[%s], [%s:%d]",
                           class_id.c_str(), __FILE__, __LINE__ );
                }
                info.class_list.push_back(class_info);
            }
        }
    }
    return;
}

template <typename T>
void content::getImgInfoFromBson(bson_iterator* bson_it, T& info, const string& icon_type, const string preview_type="")
{
    bson_iterator sub[1];
    bson_iterator_subiterator( bson_it, sub );
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
        //img type
        if(img_info.type.compare(icon_type)==0){
            if(icon_type.compare("7")==0){
                info.thumb =img_info.url;
            }
            else{
                info.icon = img_info.url;
            }
        }else if( preview_type.empty() || img_info.type.compare(preview_type)==0 ){
            info.img_list.push_back(img_info.url);
        }
    }
    return;
}

template <typename T>
void content::getLabelFromBson(bson_iterator* bson_it, T& info)
{
    bson_iterator sub[1];
    bson_iterator_subiterator( bson_it, sub );
    while( bson_iterator_next(sub) ){
        info.label_list.push_back(bson_iterator_string(sub));
    }
    return;
}

template <typename T>
void content::getAttributeFromBson(bson_iterator* bson_it, T& info)
{
    bson_iterator sub[1];
    bson_iterator_subiterator( bson_it, sub );
    bson_iterator subsub[1];
    content_attribute_t attribute_info;
    while( bson_iterator_next(sub) ){
        bson_iterator_subiterator( sub, subsub );
        while( bson_iterator_next(subsub) ){
            if( strcmp(bson_iterator_key(subsub),"type")==0 ){
                attribute_info.type=bson_iterator_int(subsub);
            }
            else if( strcmp(bson_iterator_key(subsub),"content")==0 ){
                attribute_info.content=bson_iterator_string(subsub);
            }
        }
        info.attribute_list.push_back(attribute_info);
    }
    return;
}

int content::get_ios_content_from_bson(const bson* result, ios_content_t& ios_content )
{
    const char* key;
    bson_iterator it[1];
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        if(0==strcmp(key,"id")){
            ios_content.id=bson_iterator_string(it);
        }else if(0==strcmp(key,"name")){
            ios_content.name=bson_iterator_string(it);
        }else if(0==strcmp(key,"content_type")){
            ios_content.type=bson_iterator_string(it);
        }else if(0==strcmp(key,"itunes_id")){
            ios_content.itunes_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"info")){
            ios_content.info=bson_iterator_string(it);
        }else if(0==strcmp(key,"version")){
            ios_content.version=bson_iterator_string(it);
        }else if(0==strcmp(key,"cp_id")){
            ios_content.cp_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"star")){
            ios_content.star=bson_iterator_string(it);
        }else if(0==strcmp(key,"gdate")){
            ios_content.gdate=mcp_content_map::convert_mongo_ts_to_string(it);
        }else if(0==strcmp(key,"mdate")){
            ios_content.mdate=mcp_content_map::convert_mongo_ts_to_string(it);
        }else if(0==strcmp(key,"system_req")){
            ios_content.adapt=bson_iterator_string(it);
        }else if(0==strcmp(key,"path_url")){
            ios_content.url=bson_iterator_string(it);
        }else if(0==strcmp(key,"jump_url")){
            ios_content.jump_url=bson_iterator_string(it);
        }else if(0==strcmp(key,"size")){
            ios_content.size=bson_iterator_string(it);
        }else if(0==strcmp(key,"value")){
            ios_content.value=bson_iterator_double(it);
        }else if(0==strcmp(key,"original_value")){
            ios_content.original_value=bson_iterator_double(it);
        }else if(0==strcmp(key,"language")){
            ios_content.language=bson_iterator_string(it);
        }else if(0==strcmp(key,"rating")){
            ios_content.rating=bson_iterator_int(it);
        }else if(0==strcmp(key,"count")){
            ios_content.count=bson_iterator_int(it);
        }else if(0==strcmp(key,"crt_rating")){
            ios_content.crt_rating=bson_iterator_int(it);
        }else if(0==strcmp(key,"crt_count")){
            ios_content.crt_count=bson_iterator_int(it);
        }else if(0==strcmp(key,"class")){
            getClassInfoFromBson(it,ios_content);
        }else if(0==strcmp(key,"img_info")){
            getImgInfoFromBson(it,ios_content,"10");
        }else{
        }
    }
    return 0;
}

online_game_t content::getOnlineGameFromBson(const bson* result)
{
    online_game_t info;
    getOnlineGameFromBson(result, info);
    return info;
}

int content::getOnlineGameFromBson(const bson* result, online_game_t& info)
{
    const char* key;
    bson_iterator it[1];
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        UB_LOG_TRACE("key is %s", key);
        if(0==strcmp(key,"id")){
            info.id=bson_iterator_string(it);
        }
        // [s] add by gwj for mcp_content_pub_prop
        else if( 0 == strcmp(key,"pub_prop_ids"))
        {
            info.pub_prop_ids = bson_iterator_string(it);
            UB_LOG_DEBUG( "pub_prop_ids is %s", info.pub_prop_ids.c_str());
        }
        // [e] add by gwj for mcp_content_pub_prop
        else if(0==strcmp(key,"name")){
            info.name=bson_iterator_string(it);
        }else if(0==strcmp(key,"content_type")){
            info.type=bson_iterator_string(it);
        }else if(0==strcmp(key,"info")){
            info.info=bson_iterator_string(it);
        }else if(0==strcmp(key,"version")){
            info.version=bson_iterator_string(it);
        }else if(0==strcmp(key,"system_req")){
            info.system_req=bson_iterator_string(it);
        }else if(0==strcmp(key,"cp_id")){
            info.cp_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"star")){
            info.star=bson_iterator_double(it);
        }else if(0==strcmp(key,"popularity")){
            info.popularity=bson_iterator_int(it);
        }else if(0==strcmp(key,"language")){
            info.language=bson_iterator_string(it);
        }else if(0==strcmp(key,"gdate")){
            info.gdate=mcp_content_map::convert_mongo_ts_to_string(it);
        }else if(0==strcmp(key,"mdate")){
            info.mdate=mcp_content_map::convert_mongo_ts_to_string(it);
        }else if(0==strcmp(key,"ldate")){
            info.ldate=mcp_content_map::convert_mongo_ts_to_string(it);
        }else if(0==strcmp(key,"path_url")){
            info.url=bson_iterator_string(it);
        }else if(0==strcmp(key,"size")){
            info.size=bson_iterator_int(it);
        }else if(0==strcmp(key,"class")){
            getClassInfoFromBson(it,info);
        }else if(0==strcmp(key,"img_info")){
            getImgInfoFromBson(it,info,"5", "8");
            // "997" for not push_back icon url again
            getImgInfoFromBson(it,info,"7", "997");
        }else if(0==strcmp(key,"label_id")){
            getLabelFromBson(it,info);
        }else if(0==strcmp(key,"attribute")){
            getAttributeFromBson(it, info);
        }
        /* 20130328 zhengxie mongodb.mcp.mcp_online_game新增channel_info字段 beg*/
        else if(0==strcmp(key,"channel_info")) {
        	if(0==strcmp(bson_iterator_string(it),"TEMP") ){
        		info.apk_flag="1";
        	}
        	else{
        		info.apk_flag="0";
        	}
        	UB_LOG_DEBUG("apk_flag:[%s]",info.apk_flag.c_str());
        }
        /* 20130328 zhengxie mongodb.mcp.mcp_online_game新增channel_info字段 end*/

		/* 20130815 zhengxie mongodb.mcp.mcp_online_game新增editor_recom字段 beg*/
        else if(0==strcmp(key,"editor_recom")) {
            info.editor_recom=bson_iterator_string(it);
        }
		/* 20130815 zhengxie mongodb.mcp.mcp_online_game新增editor_recom字段 beg*/

        else
        {}
    }
    return 0;
}

int content::get_game_info_from_bson(const bson* result, game_t& game_info )
{   
    const char* key;
    bson_iterator it[1];
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){                  
        key=bson_iterator_key(it);                  
        if(0==strcmp(key,"id")){                    
            game_info.info_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"name")){            
            game_info.info_title=bson_iterator_string(it);
        }else if(0==strcmp(key,"content")){ 
            game_info.info_content=bson_iterator_string(it);
        }else if(0==strcmp(key,"type_id")){
            game_info.info_type=bson_iterator_string(it);
        }else if(0==strcmp(key,"logdate")){
            game_info.info_date=bson_iterator_string(it);
        }else if(0==strcmp(key,"c_id")){
            game_info.c_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"game_type")){
            game_info.game_type=bson_iterator_string(it);
       	} 
    }
    return 0;
}

int content::get_game_brief_info_from_bson(const bson* result, game_brief_t& game_brief_info)
{
	const char* key;
    bson_iterator it[1];
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        if(0==strcmp(key,"id")){
            game_brief_info.info_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"name")){
            game_brief_info.info_title=bson_iterator_string(it);
        }else if(0==strcmp(key,"type_id")){
            game_brief_info.info_type=bson_iterator_string(it);
        }else if(0==strcmp(key,"logdate")){
            game_brief_info.info_datetime=bson_iterator_string(it);
        }else if(0==strcmp(key,"game_type")){
            game_brief_info.game_type=bson_iterator_string(it);
        }
    }
    return 0;
}

int content::get_game_score_from_bson(const bson* result, game_score_t& game_info )
{
    const char* key;
    bson_iterator it[1];
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){                  
        key=bson_iterator_key(it);
        if(0==strcmp(key,"score")){
            game_info.score=bson_iterator_string(it);
        }
    }
    return 0;
}

int content::get_appinfo_from_bson(const bson* result, appinfo_t& app_info)
{
    const char* key;
    bson_iterator it[1];
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){                  
        key=bson_iterator_key(it);
        if(0==strcmp(key,"id")){
            app_info.id=bson_iterator_string(it);
        }
        if(0==strcmp(key,"c_id")){
            app_info.c_id=bson_iterator_string(it);
        }
        if(0==strcmp(key,"app_name")){
            app_info.app_name=bson_iterator_string(it);
        }
        if(0==strcmp(key,"package_name")){
            app_info.package_name=bson_iterator_string(it);
        }
        if(0==strcmp(key,"version_code")){
            app_info.version_code=bson_iterator_string(it);
        }
        if(0==strcmp(key,"version")){
            app_info.version=bson_iterator_string(it);
        }
        if(0==strcmp(key,"sdk_version")){
            app_info.sdk_version=bson_iterator_string(it);
        }
        if(0==strcmp(key,"permissions")){
            app_info.permissions=bson_iterator_string(it);
        }
        if(0==strcmp(key,"features")){
            app_info.features=bson_iterator_string(it);
        }
        if(0==strcmp(key,"apk_info")){
            app_info.apk_info=bson_iterator_string(it);
        }
    }
    return 0;
}

int content::get_cid_from_bson(const bson* result, string& cid )
{
    const char* key;
    bson_iterator it[1];
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){
	        key=bson_iterator_key(it);
	        if(0==strcmp(key,"c_id")){
				cid=bson_iterator_string(it);
				break;
			}   
	}   
	return 0;
}

int mcp_content_map::init()
{
	int res(0);
	if(init_class_id_info()!=0){
		res-=1;
	}
	if(init_column_id_info()!=0){
		res-=2;
	}
	if(init_label_id_name()!=0){
		res-=4;
	}
	if(init_charge_id_name()!=0){
		res-=8;
	}
	if(init_cp_id_name()!=0){
		res-=16;
	}
	if(init_grant_id_info()!=0){
		res-=32;
	}
	if(init_topic_id_info()!=0){
		res-=64;
	}
    if(init_attribute_name()!=0){
        res-=128;
    }
  
  // [s] add by gwj for mcp_refactory
  CommonSimpleBuffer::instance().init_buffer();
  // [e] add by gwj for mcp_refactory  
  
	return res;
}

int mcp_content_map::init_class_id_info()
{
	UB_LOG_TRACE( "init_class_id_info start" );
	mcp_mysql conn;
	if(conn.get_conn()==NULL){
		UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}   
	string query_sql("SELECT C.`id`, C.`name`, C.`icon_url`, C.`type_id` "
			"FROM `MCP`.`mcp_content_class` AS C "
			"WHERE C.`enable` = 1 ");
	int count; 
	count = conn.query(query_sql);
	if(count<0){
		UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5;
	} 

	class_info_t class_info;
	m_class_id_info.clear();
	for(uint32_t i=0; i<(uint32_t)count; ++i){
		class_info.clear();
		//UB_LOG_DEBUG("id[%s],name[%s]",conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str());
		class_info.id=conn.get_result(i,0);
		class_info.name=conn.get_result(i,1);
		class_info.icon=conn.get_result(i,2);
		class_info.type=conn.get_result(i,3);
		// ContentClass content_class(class_info.type,0,class_info.id);
		// class_info.count=content_class.get_content_count();
		// UB_LOG_DEBUG("+++++ class_id[%s],name[%s],count[%d] +++++", 
		//               class_info.id.c_str(), class_info.name.c_str(), class_info.count);
		class_info.count=0;
		m_class_id_info[class_info.id]=class_info;
	}
	UB_LOG_TRACE( "init_class_id_info end" );
	return 0;
}

int mcp_content_map::init_column_id_info()
{
	UB_LOG_TRACE( "init_column_id_info start" );

	mcp_mysql conn;
	if(conn.get_conn()==NULL){
		UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	string query_sql("SELECT C.`id`, C.`name` "
			"FROM `MCP`.`mcp_content_diy_column` AS C "
			"WHERE C.`enable` = 1 ");
	int count;
	count = conn.query(query_sql);
	if(count<0){
		UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5;
	}

	column_info_t column_info;
	m_column_id_info.clear();
	for(uint32_t i=0; i<(uint32_t)count; ++i){
		column_info.clear();
		//UB_LOG_DEBUG("id[%s],name[%s]",conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str());
		column_info.id=conn.get_result(i,0);
		column_info.name=conn.get_result(i,1);
		m_column_id_info[column_info.id]=column_info;
	}

	UB_LOG_TRACE( "init_column_id_info end" );
	return 0;
}

int mcp_content_map::init_label_id_name()
{
	UB_LOG_TRACE( "init_label_id_name start" );
	mcp_mysql conn;
	if(conn.get_conn()==NULL){
		UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	string query_sql("SELECT B.`id`, B.`name` "
			"FROM `MCP`.`mcp_content_bookmark` AS B "
			"WHERE B.`enable` = 1 ");
	int count;
	count = conn.query(query_sql);
	if(count<0){
		UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5;
	}

	string id, name;
	m_label_id_name.clear();
	for(uint32_t i=0; i<(uint32_t)count; ++i){
		id.clear();
		name.clear();
		UB_LOG_DEBUG("id[%s],name[%s]",conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str());
		id=conn.get_result(i,0);
		name=conn.get_result(i,1);
		m_label_id_name[id]=name;
	}

	UB_LOG_TRACE( "init_label_id_name end" );
	return 0;
}


int mcp_content_map::init_charge_id_name()
{
	UB_LOG_TRACE( "init_charge_id_name start" );

	mcp_mysql conn;
	if(conn.get_conn()==NULL){
		UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	string query_sql("SELECT C.`id`, C.`name` "
			"FROM `MCP`.`mcp_content_chargetype` AS C "
			"WHERE C.`enable` = 1 ");
	int count;
	count = conn.query(query_sql);
	if(count<0){
		UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5;
	}

	string id, name;
	m_charge_id_name.clear();
	for(uint32_t i=0; i<(uint32_t)count; ++i){
		id.clear();
		name.clear();
		UB_LOG_DEBUG("id[%s],name[%s]",conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str());
		id=conn.get_result(i,0);
		name=conn.get_result(i,1);
		m_charge_id_name[id]=name;
	}

	UB_LOG_TRACE( "init_charge_id_name end" );
	return 0;
}

int mcp_content_map::init_cp_id_name()
{
	UB_LOG_TRACE( "init_cp_id_name start" );
	mcp_mysql conn;
	if(conn.get_conn()==NULL){
		UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	string query_sql("SELECT C.`id`, C.`name` "
			"FROM `MCP`.`mcp_content_cp` AS C "
			"WHERE C.`enable` = 1 ");
	int count;
	count = conn.query(query_sql);
	if(count<0){
		UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5;
	}

	string id, name;
	m_cp_id_name.clear(); 
	for(uint32_t i=0; i<(uint32_t)count; ++i){
		id.clear();
		name.clear();
		UB_LOG_DEBUG("id[%s],name[%s]",conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str());
		id=conn.get_result(i,0);
		name=conn.get_result(i,1);
		m_cp_id_name[id]=name;
	}

	UB_LOG_TRACE( "init_cp_id_name end" );
	return 0;
}


int mcp_content_map::init_grant_id_info()
{
	UB_LOG_TRACE( "init_grant_id_info start" );
	mcp_mysql conn;
	if(conn.get_conn()==NULL){
		UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	string query_sql("SELECT G.`id`, G.`grant_date`, G.`sign_enddate`, G.`modify_date` "
			"FROM `MCP`.`mcp_content_grant` AS G "
			"WHERE G.`enable` = 1 ");
	int count;
	count = conn.query(query_sql);
	if(count<0){
		UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5;
	}

	grant_detail_t grant_info;
	m_grant_id_info.clear();
	for(uint32_t i=0; i<(uint32_t)count; ++i){
		grant_info.clear();
		UB_LOG_DEBUG("id[%s],grant[%s],sign[%s],modify[%s]",
				conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str(),conn.get_result(i,2).c_str(),conn.get_result(i,3).c_str());
		grant_info.id=conn.get_result(i,0);
		grant_info.grant_time=conn.get_result(i,1);
		grant_info.valid_time=conn.get_result(i,2);
		grant_info.modify_time=conn.get_result(i,3);
		m_grant_id_info[grant_info.id]=grant_info;
	}

	UB_LOG_TRACE( "init_grant_id_info end" );
	return 0;
}


int mcp_content_map::init_topic_id_info()
{
	UB_LOG_TRACE( "init_topic_id_info start" );
	mcp_mysql conn;
	if(conn.get_conn()==NULL){
		UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}
	string query_sql("SELECT T.`id`, T.`name`, T.`type_id`, T.`img_url`, T.`intro`, T.`log_date`, T.`topic_type` "
			"FROM `MCP`.`mcp_content_topic` AS T "
			"WHERE T.`enable` = 1 "
			"ORDER BY T.`priority` DESC ");
	int count;
	count = conn.query(query_sql);
	if(count<0){
		UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
		return -5;
	}
	topic_info_t topic_info;
	map<string,string> before;
	m_topic_id_info.clear();
	m_type_topic_list.clear();
	for(uint32_t i=0; i<(uint32_t)count; ++i){
		topic_info.clear();
		//UB_LOG_DEBUG("id[%s],name[%s]",conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str());
		topic_info.id=conn.get_result(i,0);
		topic_info.name=conn.get_result(i,1);
		topic_info.type_id=conn.get_result(i,2);
		topic_info.img=conn.get_result(i,3);
		topic_info.intro=conn.get_result(i,4);
		topic_info.log_date=conn.get_result(i,5);
		if(conn.get_result(i,6)=="1"){
			//设置前一个主题
			topic_info.before=before[topic_info.type_id];
			m_topic_id_info[topic_info.id]=topic_info;
			//将当前主题设置为前一个主题的下一个主题
			if(!before[topic_info.type_id].empty()&&before[topic_info.type_id].compare("")!=0){
				m_topic_id_info[before[topic_info.type_id]].next=topic_info.id;
			}
			//将当前主题设置为前主题
			before[topic_info.type_id]=topic_info.id;
			m_type_topic_list[topic_info.type_id].push_back(topic_info.id);
		}else{
			m_topic_id_info[topic_info.id]=topic_info;
		}
		//UB_LOG_DEBUG("type[%s],id[%s]", topic_info.type_id.c_str(), topic_info.id.c_str());
	}
	UB_LOG_TRACE( "init_topic_id_info end" );
	return 0;
}

int mcp_content_map::init_attribute_name()
{
    UB_LOG_TRACE( "init_attribute_name start" );

    mcp_mysql conn;
    if(conn.get_conn()==NULL){
        UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    string query_sql("SELECT AT.`id`, AT.`name` "
            "FROM `MCP`.`mcp_content_attribute_type` AS AT "
            "WHERE AT.`enable` = 1 ");
    int count;
    count = conn.query(query_sql);
    if(count<0){
        UB_LOG_FATAL("query failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
        return -5;
    }

    int id;
    string name;
    m_charge_id_name.clear();
    for(uint32_t i=0; i<(uint32_t)count; ++i){
        id=0;
        name.clear();
        UB_LOG_DEBUG("id[%s],name[%s]",conn.get_result(i,0).c_str(),conn.get_result(i,1).c_str());
        id=atoi(conn.get_result(i,0).c_str());
        name=conn.get_result(i,1);
        m_attr_id_name[id]=name;
    }

    UB_LOG_TRACE( "init_attribute_name end" );
    return 0;
}

int mcp_content_map::get_class_info(const string& id, class_info_t& class_info)
{
	map<string,class_info_t>::iterator it;
	it = m_class_id_info.find(id);
	if(it==m_class_id_info.end()){
		//UB_LOG_FATAL("class_id[%s] is unknown", id.c_str());
		return -1;
	}
	class_info=it->second;
	return 0;
}

int mcp_content_map::get_class_list_by_type(const string& type_id, vector<class_info_t>& class_list)
{
	class_list.clear();
	map<string,class_info_t>::iterator it=m_class_id_info.begin();
	for(;it!=m_class_id_info.end();++it){
		if(type_id==it->second.type){
			class_list.push_back(it->second);
		}
	}
	return 0;
}

int mcp_content_map::get_column_info(const string& id, column_info_t& column_info)
{
	map<string,column_info_t>::iterator it;
	it = m_column_id_info.find(id);
	if(it==m_column_id_info.end()){
		UB_LOG_FATAL("column_id[%s] is unknown", id.c_str());
		return -1;
	}
	column_info=it->second;
	return 0;
}

int mcp_content_map::get_label_name(const string& id, string& name)
{
	map<string,string>::iterator it;
	it = m_label_id_name.find(id);
	if(it==m_label_id_name.end()){
		UB_LOG_FATAL("label_id[%s] is unknown", id.c_str());
		return -1;
	}
	name=it->second;
	return 0;
}

string mcp_content_map::get_label_name(const string& id)
{
    map<string,string>::iterator it;
    it = m_label_id_name.find(id);
    if(it==m_label_id_name.end()){
        UB_LOG_FATAL("label_id[%s] is unknown", id.c_str());
        return "";
    }
    return it->second;
}

int mcp_content_map::get_charge_name(const string& id, string& name)
{
	map<string,string>::iterator it;
	it = m_charge_id_name.find(id);
	if(it==m_charge_id_name.end()){
		UB_LOG_FATAL("charge_id[%s] is unknown", id.c_str());
		return -1;
	}
	name=it->second;
	return 0;
}

int mcp_content_map::get_cp_name(const string& id, string& name)
{
	map<string,string>::iterator it;
	it = m_cp_id_name.find(id);
	if(it==m_cp_id_name.end()){
		UB_LOG_FATAL("cp_id[%s] is unknown", id.c_str());
		return -1;
	}
	name=it->second;
	return 0;
}

int mcp_content_map::get_grant_info(const string& id, grant_detail_t& grant_info) 
{
	map<string,grant_detail_t>::iterator it;
	it = m_grant_id_info.find(id);
	if(it==m_grant_id_info.end()){
		UB_LOG_FATAL("grant_id[%s] is unknown", id.c_str());
		return -1;
	}
	grant_info.grant_time=it->second.grant_time;
	grant_info.valid_time=it->second.valid_time;
	grant_info.modify_time=it->second.modify_time;
	UB_LOG_DEBUG("[%s]:grant[%s],valid[%s],modify[%s]", it->second.id.c_str(),
			it->second.grant_time.c_str(),it->second.valid_time.c_str(),it->second.modify_time.c_str());
	return 0;
}

int mcp_content_map::get_topic_info(const string& id, topic_info_t& topic_info)
{
	map<string,topic_info_t>::iterator it;
	it = m_topic_id_info.find(id);
	if(it==m_topic_id_info.end()){
		UB_LOG_FATAL("topic_id[%s] is unknown", id.c_str());
		return -1;
	}
	topic_info=it->second;
	return 0;
}

string mcp_content_map::get_attribute_name(const int id)
{
    return m_attr_id_name[id];
}

string mcp_content_map::convert_mongo_ts_to_string(bson_iterator* it)
{
    time_t ts= bson_iterator_time_t(it);
    struct tm *t;
    t = localtime(&ts);
    string time_string;
    stringstream ss;
    ss.unsetf(ios::skipws);
    UB_LOG_DEBUG("%d-%d-%d %d:%d:%d",t->tm_year+1990,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_mon,t->tm_sec);
    ss<<setw(4)<<setfill('0')<<t->tm_year+1900<<"-"
      <<setw(2)<<setfill('0')<<t->tm_mon+1<<"-"
      <<setw(2)<<setfill('0')<<t->tm_mday;        //<<" "<<t->tm_hour<<":"<<t->tm_mon<<":"<<t->tm_sec;
    ss>>time_string;
    return time_string;
}


vector<int> commonFunc::randomIntSequence(const int n)
{
	srand(time(NULL));
	vector<int> viRand;
	for(int i=0; i<n; i++)
		viRand.push_back(i);
	for(int i=0; i<n-1; i++)
	{
		int j=rand()%(n-i-1)+i+1;
		int tmp=viRand[i];
		viRand[i]=viRand[j];
		viRand[j]=tmp;
	}
	return viRand;
}


void content::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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

int content::getCalenderList(vector<calender_t>& list)
{
    DuokooMysql mysql;
    if(mysql.get_conn()==NULL){
        UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }
    time_t t = time(0);
    uint32_t wday = (uint32_t)atoi(date("%w", t).c_str());
    t -= wday*24*60*60;
    string this_sunday = date("%F 00:00:00", t );
    string query_sql = "SELECT `id`, `title`, `content_id`, `type_id`, `enable_date` "
                       "FROM `MCP`.`mcp_content_calender` "
                       "WHERE `enable_date` >= '" + this_sunday + "' AND `enable` = 1 "
                       "ORDER BY `enable_date` ASC ";
    UB_LOG_DEBUG( "query_sql[%s]", query_sql.c_str() );
    int count = mysql.query(query_sql);
    if(count<0){
        UB_LOG_FATAL( "query sql faield, des:[%s], [%s:%d]", mysql.getErrDes().c_str(), __FILE__, __LINE__ );
        return -5;
    }
    calender_t calender_info;
    for(int i=0; i<count; ++i){
        calender_info.clear();
        calender_info.id = atoi(mysql.getResult(i,0).c_str());
        calender_info.title = mysql.getResult(i,1);
        calender_info.c_id = atoi(mysql.getResult(i,2).c_str());
        calender_info.type = atoi(mysql.getResult(i,3).c_str());
        calender_info.enable_date = mysql.getResult(i,4);
        list.push_back(calender_info);
    }
    return count;
}

int commonFunc::get_typeid_by_colid(const string col_id, string& type_id)
{
        DuokooMysql mysql;
        if(mysql.get_conn()==NULL){
                UB_LOG_FATAL( "get mysql conn failed, [%s:%d]", __FILE__, __LINE__ );
                return -5; 
        } 

        string query_sql ="SELECT `type_id` FROM `MCP`.`mcp_content_diy_column` WHERE id = " + col_id;
        UB_LOG_DEBUG( "query_sql[%s]", query_sql.c_str() );
        int count = mysql.query(query_sql);
        if(count<0){
                UB_LOG_FATAL( "query sql faield, des:[%s], [%s:%d]", mysql.getErrDes().c_str(), __FILE__, __LINE__ );
                return -5; 
            }
    
        type_id = mysql.getResult(0,0);
        return 0;
}
