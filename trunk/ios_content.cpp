#include "ios_content.h"

using namespace content;
using namespace std;

int IosContent::get_info(ios_content_t& info)
{
    UB_LOG_TRACE( "IosContent get_info start" );
    if(_conn==NULL){
        UB_LOG_FATAL( "mongo connnection is NULL" );
        return -5;
    }
    info.id=_id;
    int res(0);
    bson query[1], fields[1];
    bson bson_out[1];
    get_query_bson(query);
    get_fields_bson(fields);
    string ns = MCP_MONGODB_NAME+"."+MCP_IOS_CONTENT_COL;
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );
    if( res!=MONGO_OK ){
        UB_LOG_FATAL("find content basic info failed, id[%s], [%s:%d]", _id.c_str(), __FILE__, __LINE__ );
        free_data(NULL, query, fields, NULL);
        return -1;
    }
    get_info_from_bson(bson_out,info);
    free_data(NULL, query, fields, bson_out);
    UB_LOG_TRACE( "IosContent get_info end" );
    return 0;
}
/*
int IosContent::get_info_by_itunes_id(ios_content_t& info, string ituned_id)
{
	UB_LOG_TRACE( "IosContent get_info_by_itunes_id start" );
    if(_conn==NULL){
        UB_LOG_FATAL( "mongo connnection is NULL" );
        return -5;
    }
	info.itunes_id=itunes_id;
	int res(0);
	bson query[1],fields[1];
	bson bson_out[1];
	get_itunes_id_query_bson(query);
	gres = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );
    if( res!=MONGO_OK ){
        UB_LOG_FATAL("find content basic info failed, id[%s], [%s:%d]", _id.c_str(), __FILE__, __LINE__ );
        free_data(NULL, query, fields, NULL);
        return -1;
    }
    get_info_from_bson(bson_out,info);
    free_data(NULL, query, fields, bson_out);

    UB_LOG_TRACE( "IosContent get_info_by_itunes_id end" );
	return 0;
}
*/
void IosContent::get_info_from_bson(bson* result, ios_content_t& info)
{
/*
    bson_iterator it[1];
    bson_iterator_init(it,result);
    const char* key;
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        if(0==strcmp(key,"name")){
            info.name=bson_iterator_string(it);
        }else if(0==strcmp(key,"content_type")){
            info.type=bson_iterator_string(it);
        }else if(0==strcmp(key,"itunes_id")){
            info.itunes_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"info")){
            info.info=bson_iterator_string(it);
        }else if(0==strcmp(key,"version")){
            info.version=bson_iterator_string(it);
        }else if(0==strcmp(key,"cp_id")){
            info.cp_id=bson_iterator_string(it);
        }else if(0==strcmp(key,"star")){
            info.star=bson_iterator_string(it);
        }else if(0==strcmp(key,"gdate")){
            info.gdate=mcp_content_map::convert_mongo_ts_to_string(it);;
        }else if(0==strcmp(key,"system_req")){
            info.adapt=bson_iterator_string(it);
        }else if(0==strcmp(key,"path_url")){
            info.url=bson_iterator_string(it);
        }else if(0==strcmp(key,"size")){
            info.size=bson_iterator_string(it);
        }else if(0==strcmp(key,"value")){
            info.value=bson_iterator_double(it);
        }else if(0==strcmp(key,"original_value")){
            info.original_value=bson_iterator_double(it);
        }else if(0==strcmp(key,"rating")){
            info.rating=bson_iterator_int(it);
        }else if(0==strcmp(key,"count")){
            info.count=bson_iterator_int(it);
        }else if(0==strcmp(key,"crt_rating")){
            info.crt_rating=bson_iterator_int(it);
        }else if(0==strcmp(key,"crt_count")){
            info.crt_count=bson_iterator_int(it);
        }else if(0==strcmp(key,"class")){
            get_class_info_from_bson(it,info);
        }else if(0==strcmp(key,"img_info")){
            get_img_info_from_bson(it,info);
        }else{
        }
    }
*/
    content::get_ios_content_from_bson(result, info);
    return;
}

void IosContent::get_img_info_from_bson(bson_iterator* it, ios_content_t& info)
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
        //ios icon type
        if(img_info.type.compare("9")==0){
            info.icon=img_info.url;
        }else{
            info.img_list.push_back(img_info.url);
        }
/*
        else{
            if(!info.preview.empty()&&info.preview.compare("")!=0){
                info.preview.append(",");
            }
            info.preview.append(img_info.url);
        }
*/
    }
    return;
} 


void IosContent::get_class_info_from_bson(bson_iterator* it, ios_content_t& info)
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

void IosContent::get_query_bson(bson* query)
{
    bson_init(query);
    bson_append_string(query,"id",_id.c_str());
    bson_finish(query);
    return;
}
/*
void IosContent::get_itunes_id_query_bson(bson* query)
{
    bson_init(query);
    bson_append_string(query,"itunes_id",_itunes_id.c_str());
    bson_finish(query);
    return;
}
*/
void IosContent::get_fields_bson(bson* fields)
{
    bson_init(fields);
    bson_finish(fields);
    return;
}

void IosContent::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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
