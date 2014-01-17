#include "android_theme.h"

using namespace content;
using namespace std;

int AndroidTheme::get_info(android_theme_t& theme)
{
    UB_LOG_TRACE( "AndroidTheme get_info start" );
    if(_conn==NULL){
        UB_LOG_FATAL( "mongo connnection is NULL" );
        return -5;
    }
    theme.id=_id;
    int res(0);
    bson query[1], fields[1];
    bson bson_out[1];
    get_query_bson(query);
    get_fields_bson(fields);
    string ns = MCP_MONGODB_NAME+"."+MCP_ANDROID_THEME_COL;
    res = mongo_find_one( _conn, ns.c_str(), query, fields, bson_out );
    if( res!=MONGO_OK ){
        UB_LOG_FATAL("find content basic info failed, id[%s], [%s:%d]", _id.c_str(), __FILE__, __LINE__ );
        free_data(NULL, query, fields, NULL);
        return -1;
    }
    get_theme_info_from_bson(bson_out,theme);
    free_data(NULL, query, fields, bson_out);
    UB_LOG_TRACE( "AndroidTheme get_info end" );
    return 0;
}

void AndroidTheme::get_theme_info_from_bson(bson* result, android_theme_t& theme)
{
    bson_iterator it[1];
    bson_iterator_init(it,result);
    const char* key;
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        if(0==strcmp(key,"name")){
            theme.name=bson_iterator_string(it);
        }else if(0==strcmp(key,"info")){
            theme.info=bson_iterator_string(it);
        }else if(0==strcmp(key,"releasedate")){
            theme.date=bson_iterator_string(it);
        }else if(0==strcmp(key,"froms")){
            theme.froms=bson_iterator_string(it);
        }else if(0==strcmp(key,"adapt")){
            theme.adapt=bson_iterator_string(it);
        }else if(0==strcmp(key,"pop")){
            theme.pop=bson_iterator_string(it);
        }else if(0==strcmp(key,"path_url")){
            theme.down_url=bson_iterator_string(it);
        }else if(0==strcmp(key,"jump_url")){
            theme.jump_url=bson_iterator_string(it);
        }else if(0==strcmp(key,"size")){
            theme.size=bson_iterator_string(it);
        }else if(0==strcmp(key,"class")){
            get_class_info_from_bson(it,theme);
        }else if(0==strcmp(key,"img_info")){
            get_img_info_from_bson(it,theme);
        }else{
        }
    }
    return;
}

void AndroidTheme::get_img_info_from_bson(bson_iterator* it, android_theme_t& theme)
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
        theme.img_info.push_back(img_info);
        if(img_info.type.compare("4")==0){
            theme.icon=img_info.url;
        }
        else{
            if(!theme.preview.empty()&&theme.preview.compare("")!=0){
                theme.preview.append(",");
            }
            theme.preview.append(img_info.url);
        }
    }
    return;
} 


void AndroidTheme::get_class_info_from_bson(bson_iterator* it, android_theme_t& theme)
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
                theme.class_info_list.push_back(class_info);
            }
        }
    }
    return;
} 

void AndroidTheme::get_query_bson(bson* query)
{
    bson_init(query);
    bson_append_string(query,"id",_id.c_str());
    bson_finish(query);
    return;
}

void AndroidTheme::get_fields_bson(bson* fields)
{
    bson_init(fields);
    bson_finish(fields);
    return;
}

void AndroidTheme::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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
