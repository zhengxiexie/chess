#include <iostream>
#include <sstream>
#include "content_list.h"
#include "ub_server.h"

using namespace std;
using namespace content;

int ContentList::get_content_list(int32_t& count,vector<content_info_t>& list)
{
    UB_LOG_WARNING("get_content_list start warning");
    UB_LOG_DEBUG( "get_content_list start 0");
    UB_LOG_TRACE( "get_content_list start" );

    int res(0);
    list.clear();

	UB_LOG_TRACE("get_content_list _adapt =%d", _adapt);	
    if( _adapt==1 ){
		UB_LOG_TRACE("get_content_list adapt");	
        res = this->get_content_list_only_adapt();
    }
    else{
		UB_LOG_TRACE("get_content_list all");	
        res = this->get_content_list_all();
    }

    if( res < 0 ){
        UB_LOG_FATAL( "get_content_list failed, adapt[%d], [%s:%d]", _adapt, __FILE__, __LINE__ );
        return res;
    }

    count = res;
    list = _list;

    UB_LOG_TRACE( "get_content_list end" );
    return 0;
}

int ContentList::get_soft_list(int32_t& count, vector<soft_info_t>& soft_list)
{
    UB_LOG_TRACE( "get_soft_list start" );
    int res(0);

    count = this->get_content_count();
    if( count==0 ){
        return 0;
    }
    else if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }

    res = this->get_content_soft_list(soft_list);
    if( res < 0 ){
        UB_LOG_FATAL( "get_content_soft_list failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }
    UB_LOG_TRACE( "get_soft_list start" );
    return 0;
}

int ContentList::get_ios_content_list(int32_t& count, vector<ios_content_t>& list)
{
    UB_LOG_TRACE( "ContentList::get_ios_content_list start" );
    int res(0);

    count = this->get_content_count();
    if( count==0 ){
        return 0;
    }
    else if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }

    res = this->get_ios_content_list(list);
    if( res < 0 ){
        UB_LOG_FATAL( "get_ios_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }
    UB_LOG_TRACE( "ContentList::get_ios_content_list end" );
    return 0;
}

int ContentList::getOnlineGameList(int32_t& count, vector<online_game_t>& list)
{
    UB_LOG_TRACE( "ContentList::getOnlineGameList start" );
    int res(0);
    count = this->get_content_count();
    if(count==0){
        return 0;
    }else if(count<0){
        UB_LOG_FATAL("get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }
    res = this->getOnlineGameList(list);
    if(res<0){
        UB_LOG_FATAL( "getOnlineGameList failed. [%s:%d]", __FILE__, __LINE__ );
        return res;
    }
    UB_LOG_TRACE( "ContentList::getOnlineGameList end" );
    return 0;
}

int ContentList::get_android_theme_list(int32_t& count, vector<android_theme_t>& theme_list)
{
    UB_LOG_DEBUG( "get_android_theme_list start" );
    UB_LOG_DEBUG( "++++++++++++++++++++++++++++");
    int res(0);

    count = this->get_content_count();
    if( count==0 ){
        return 0;
    }
    else if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }

    res = this->get_android_theme_basic_list(theme_list);
    if( res < 0 ){
        UB_LOG_FATAL( "get_android_theme_basic_list failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }
    UB_LOG_DEBUG( "get_android_theme_list end" );
    return 0;
}

int ContentList::get_content_count(int32_t weight_type)
{
    UB_LOG_TRACE( "get_content_count start111" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res(0);
    bson query[1];

    this->get_count_query_bson(query);
    res = mongo_count( _conn, MCP_MONGODB_NAME.c_str(), _basic_coll.c_str(), query );
    UB_LOG_TRACE("content list count is %d", res);
    UB_LOG_TRACE("basic coll is %s", _basic_coll.c_str());
    if( res < 0 ){
        UB_LOG_FATAL( "mongo_count failed, [%s:%d]", __FILE__, __LINE__ );
    }

	UB_LOG_TRACE("after mongo count");
    if( res>0 && (uint32_t)res<=_start_pos ){
        _start_pos = ((res-1)/_unit_num)*_unit_num;
    }

    bson_destroy(query);
    UB_LOG_TRACE( "get_content_count end" );
    return res;
}

int ContentList::get_content_count(const map<string,vector<download_info_t> >& m_id_download_info)
{
    UB_LOG_TRACE( "get_content_count start222" );
    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res(0);
    bson query[1];

    this->get_count_query_bson(m_id_download_info,query);
    res = mongo_count( _conn, MCP_MONGODB_NAME.c_str(), _basic_coll.c_str(), query );
    if( res < 0 ){
        UB_LOG_FATAL( "mongo_count failed, [%s:%d]", __FILE__, __LINE__ );
    }

    if( res>0 && (uint32_t)res<=_start_pos ){
        _start_pos = ((res-1)/_unit_num)*_unit_num;
    }

    bson_destroy(query);
    UB_LOG_TRACE( "get_content_count end" );
    return res;
}

int ContentList::get_content_list_all()
{
    int res(0), count(0);

    UB_LOG_TRACE( "get_content_list_all start" );
    count = this->get_content_count();
    UB_LOG_TRACE( "get_content_all count is %d",count );
    //if( count==0 ){
    //    return 0;
    //}
    if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }

    vector<basic_info_t> basic_list;
	
    res = this->get_content_basic_info(basic_list);
    if( res < 0 ){
        UB_LOG_FATAL( "get_content_basic_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }

    vector<download_info_t> download_list;
    res = this->get_content_download_info(basic_list,download_list);
    if( res < 0 ){
        UB_LOG_FATAL( "get_content_download_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }

    combine_content_list(basic_list, download_list);
/*	
	vector<package_info_t> package_list;
    res = this->get_package_info(package_list);
    if( res!=0 ){
        UB_LOG_FATAL( "get_package_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
	}

    combine_content_list(package_list, download_list);
*/
    UB_LOG_TRACE( "get_content_list_all end" );
    return count;
}

int ContentList::get_content_list_only_adapt()
{
    int res(0);
    int count(0);

    map<string,vector<download_info_t> > m_id_download_info;
    res = this->get_content_download_info(m_id_download_info);
    if( res!=0 ){
        UB_LOG_FATAL( "get_content_download_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }

    count = this->get_content_count(m_id_download_info);
    //if( count==0 ){
    //    return 0;
    //}
    if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }

    vector<basic_info_t> basic_list;
    res = this->get_content_basic_info(m_id_download_info, basic_list);

    if( res!=0 ){
        UB_LOG_FATAL( "get_content_basic_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }
    combine_content_list(basic_list, m_id_download_info);
	/*
    vector<package_info_t> package_list;
    res = this->get_package_info(m_id_download_info, package_list);
    if( res!=0 ){
        UB_LOG_FATAL( "get_package_info failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
	}
    combine_content_list(package_list, m_id_download_info);
*/
    return count;
}

int ContentList::get_content_basic_info(vector<basic_info_t>& basic_list)
{
    UB_LOG_TRACE( "get_content_basic_info start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_basic_query_bson(query);
    this->get_basic_fields_bson(fields);

	UB_LOG_TRACE("basic_coll is %s",_basic_coll.c_str());
    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    basic_list.clear();
    basic_info_t basic_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        basic_info.clear();
        res=this->get_basic_info_from_cursor(cursor, basic_info);
        if(res==0){
            basic_list.push_back( basic_info );
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", basic_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_basic_info end" );
    return 0;
}

/*
int ContentList::get_package_info(vector<package_info_t>& package_list)
{
    UB_LOG_TRACE( "get_package_info start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_basic_query_bson(query);
    this->get_basic_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    package_list.clear();
    package_info_t package_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        package_info.clear();
        res=this->get_package_info_from_cursor(cursor, package_info);
        if(res==0){
            package_list.push_back( package_info );
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", package_info.c_id.c_str(), __FILE__, __LINE__ );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_package_info end" );
    return 0;
}
*/
/*
int ContentList::get_content_basic_info(vector<basic_info_t>& basic_list)
{

}
*/
int ContentList::get_content_soft_list (vector<soft_info_t>& soft_list)
{
    UB_LOG_TRACE( "get_content_soft_list start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_soft_query_bson(query);
    this->get_soft_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    soft_list.clear();
    soft_info_t soft_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        soft_info.clear();
        res=this->get_soft_info_from_cursor(cursor, soft_info);
        if(res==0){
            soft_list.push_back( soft_info );
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", soft_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_soft_list end" );
    return 0;
}

int ContentList::get_ios_content_list(vector<ios_content_t>& list)
{
    UB_LOG_TRACE( "ContentList::get_ios_content_list start" );
    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_ios_query_bson(query);
    this->get_ios_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    list.clear();
    ios_content_t ios_content;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        ios_content.clear();
        res=get_ios_content_from_cursor(cursor, ios_content);
        if(res==0){
            list.push_back( ios_content );
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", ios_content.id.c_str(), __FILE__, __LINE__ );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "ContentList::get_ios_content_list end" );
    return 0;
}

int ContentList::getOnlineGameList(vector<online_game_t>& list)
{
    UB_LOG_TRACE( "ContentList::getOnlineGameList start" );
    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->getOnlineGameQueryBson(query);
    this->getOnlineGameFieldsBson(fields);
    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    list.clear();
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        list.push_back(getOnlineGameFromCursor(cursor));
    }
    free_data(cursor, query, fields, NULL);
    UB_LOG_TRACE( "ContentList::getOnlineGameList start" );
    return 0;
}

int ContentList::get_android_theme_basic_list(vector<android_theme_t>& theme_list)
{
    UB_LOG_DEBUG( "get_android_theme_basic_list start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_android_theme_query_bson(query);
    this->get_android_theme_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    theme_list.clear();
    android_theme_t theme_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        theme_info.clear();
        res=this->get_android_theme_from_cursor(cursor, theme_info);
        if(res==0){
            theme_list.push_back( theme_info );
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", theme_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_DEBUG( "get_android_theme_basic_list end" );
    return 0;
}

int ContentList::get_content_basic_info(const map<string,vector<download_info_t> >& m_id_download_info,
                                               vector<basic_info_t>& basic_list)
{
    UB_LOG_TRACE( "get_content_basic_info start" );
    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_basic_query_bson(m_id_download_info,query);
    this->get_basic_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    basic_list.clear();
    basic_info_t basic_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        basic_info.clear();
        res=this->get_basic_info_from_cursor(cursor, basic_info);
        if(res==0){
            basic_list.push_back( basic_info );
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", basic_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_basic_info end" );
    return 0;
}
/*
int ContentList::get_package_info(const map<string,vector<download_info_t> >& m_id_download_info,
                                               vector<package_info_t>& package_list)
{
    UB_LOG_TRACE( "get_content_package_info start" );
    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_package_query_bson(m_id_download_info,query);
    this->get_package_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    package_list.clear();
    package_info_t package_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        package_info.clear();
        res=this->get_package_info_from_cursor(cursor, package_info);
        if(res==0){
            package_list.push_back( package_info );
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", package_info.c_id.c_str(), __FILE__, __LINE__ );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_package_info end" );
    return 0;
}
*/
int ContentList::get_content_download_info(const vector<basic_info_t>& basic_list, vector<download_info_t>& download_list)
{
    UB_LOG_TRACE( "get_content_download_info start" );

    if( _adapt_id.empty() || _adapt_id.compare("")==0 || basic_list.empty() ){
        return 0;
    }

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_download_query_bson(basic_list,query);
    this->get_download_fields_bson(fields);

    cursor = mongo_find( _conn, _download_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    download_list.clear();
    download_info_t download_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        download_info.clear();
        res=this->get_download_info_from_cursor(cursor, download_info);
        if(res==0){
            download_list.push_back(download_info);
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_download_info end" );
    return 0;
}

int ContentList::get_content_download_info(map<string,vector<download_info_t> >& m_id_download_info)
{
    UB_LOG_TRACE( "get_content_download_info start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_download_query_bson(query);
    this->get_download_fields_bson(fields);

    cursor = mongo_find( _conn, _download_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    m_id_download_info.clear();
    download_info_t download_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        download_info.clear();
        res=this->get_download_info_from_cursor(cursor, download_info);
        if(res==0){
            m_id_download_info[download_info.id].push_back(download_info);
            //UB_LOG_DEBUG( "c_id[%s]", download_info.id.c_str() );
        }
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_download_info end" );
    return 0;
}

int ContentList::combine_content_list(const vector<basic_info_t>& basic_list, const vector<download_info_t>& download_list)
{
    _list.clear();
    content_info_t content_info;
    vector<basic_info_t>::const_iterator basic_it = basic_list.begin();
    for( ; basic_it!=basic_list.end(); ++basic_it ){
        content_info.clear();
        content_info.set_basic_info(*basic_it);
        _list.push_back( content_info );
    }

    vector<download_info_t>::const_iterator download_it = download_list.begin();
    vector<content_info_t>::iterator content_it;
    for( ; download_it!=download_list.end(); ++download_it ){
        for( content_it=_list.begin(); content_it!=_list.end(); ++content_it ){
            if(content_it->id==download_it->id){
                content_it->set_download_info(*download_it);
                break;
            }
        }
    }
    
    return 0;
}

int ContentList::combine_content_list(const vector<basic_info_t>& basic_list, 
                                        map<string,vector<download_info_t> >& m_id_download_info)
{
    _list.clear();
    content_info_t content_info;
    vector<basic_info_t>::const_iterator basic_it = basic_list.begin();
    for( ; basic_it!=basic_list.end(); ++basic_it ){
        content_info.clear();
        content_info.set_basic_info(*basic_it);
        vector<download_info_t>::const_iterator download_it = m_id_download_info[content_info.id].begin();
        for( ; download_it!=m_id_download_info[content_info.id].end(); ++ download_it ){
            content_info.set_download_info(*download_it);
        }
        _list.push_back( content_info );
    }
    return 0;
}
/*
int ContentList::combine_content_list(const vector<package_info_t>& package_list, const vector<download_info_t>& download_list)
{
    _list.clear();
    content_info_t content_info;
    vector<package_info_t>::const_iterator package_it = package_list.begin();
    for( ; package_it!=package_list.end(); ++package_it ){
        content_info.clear();
        content_info.set_package_info(*package_it);
        _list.push_back( content_info );
    }

    vector<download_info_t>::const_iterator download_it = download_list.begin();
    vector<content_info_t>::iterator content_it;
    for( ; download_it!=download_list.end(); ++download_it ){
        for( content_it=_list.begin(); content_it!=_list.end(); ++content_it ){
            if(content_it->id==download_it->id){
                content_it->set_download_info(*download_it);
                break;
            }
        }
    }
    
    return 0;
}

int ContentList::combine_content_list(const vector<package_info_t>& package_list, 
                                        map<string,vector<download_info_t> >& m_id_download_info)
{
    _list.clear();
    content_info_t content_info;
    vector<package_info_t>::const_iterator package_it = package_list.begin();
    for( ; package_it!=package_list.end(); ++package_it ){
        content_info.clear();
        content_info.set_package_info(*package_it);
        vector<download_info_t>::const_iterator download_it = m_id_download_info[content_info.id].begin();
        for( ; download_it!=m_id_download_info[content_info.id].end(); ++ download_it ){
            content_info.set_download_info(*download_it);
        }
        _list.push_back( content_info );
    }
    return 0;
}
*/


int ContentList::get_basic_info_from_cursor(mongo_cursor * cursor, basic_info_t& basic_info)
{
    const bson* result;
    const char* key;
    bson_iterator it[1];
    result = mongo_cursor_bson( cursor );
    bson_iterator_init( it, result);
	stringstream ss;
    while( bson_iterator_next(it) ){
        key = bson_iterator_key(it);
        UB_LOG_TRACE("++++key[%s]+++++",key);
		string s=bson_iterator_string(it);
        UB_LOG_TRACE("key is %s",s.c_str());
        if( 0 == strcmp(key,"id")){
            basic_info.id = bson_iterator_string( it );
        }
        else if( 0 == strcmp(key,"name")){
            basic_info.name = bson_iterator_string( it );
        }
        else if( 0 == strcmp(key,"class")){
            this->get_class_info_from_bson(it,basic_info);
        }
        else if( 0 == strcmp(key,"game_code")){
            basic_info.code = bson_iterator_string( it );
        }
        else if( 0 == strcmp(key,"default_charge_typeid")){
            basic_info.charge_type = bson_iterator_string( it );
            mcp_content_map::get_charge_name(bson_iterator_string(it), basic_info.charge_name);
        }
        else if( 0 == strcmp(key,"default_charge_value")){
            basic_info.charge_value = bson_iterator_string( it );
        }
        else if( 0 == strcmp(key,"img_info")){
            this->get_img_info_from_bson( it, basic_info.img_url );
        }
        else if( 0 == strcmp(key,"logdate")){
            basic_info.logdate = this->convert_mongo_ts_to_string(it);
        }
        else if( 0 == strcmp(key,"package_name")){
            basic_info.package_name = bson_iterator_string(it);
        }
        else if( 0 == strcmp(key,"version")){
            basic_info.version = bson_iterator_string(it);
        }
        else if( 0 == strcmp(key,"version_appinfo")){
            basic_info.version_appinfo = bson_iterator_string(it);
        }
        else if( 0 == strcmp(key,"sdk_version")){
			int sdk_version = bson_iterator_int(it);
			ss.clear();
			ss << sdk_version;
            basic_info.sdk_version = ss.str();
        }
        else if( 0 == strcmp(key,"version_code")){
            basic_info.version_code = bson_iterator_string(it);
        }
        else if( 0 == strcmp(key,"star")){
			/* 2013.05.20 by zhengxie ÐÞ¸ÄÐÇ¼¶Âß¼­ */
            /* int star = bson_iterator_int(it); */
            double star = bson_iterator_double(it);
            ss.clear();
            ss << star;
            basic_info.star = ss.str();
        }
        // [s] add by gwj for mcp_content_pub_prop
        else if( 0 == strcmp(key,"pub_prop_ids"))
        {
            basic_info.pub_prop_ids = bson_iterator_string(it);
            UB_LOG_DEBUG( "pub_prop_ids is %s", basic_info.pub_prop_ids.c_str());
        }
		else if( 0 == strcmp(key,"show_highspeed") )
		{
			basic_info.show_highspeed = bson_iterator_string(it);
		}
        // [e] add by gwj for mcp_content_pub_prop
        else{
            continue;
        }
    }
    return 0;
}
/*
int ContentList::get_package_info_from_cursor(mongo_cursor * cursor, package_info_t& package_info)
{
    const bson* result;
    const char* key;
    bson_iterator it[1];
    result = mongo_cursor_bson( cursor );
    bson_iterator_init( it, result);
    while( bson_iterator_next(it) ){
        key = bson_iterator_key(it);
        UB_LOG_DEBUG("++++key[%s]+++++",key);
        if( 0 == strcmp(key,"c_id")){
            package_info.c_id = bson_iterator_string( it );
        }
        else if( 0 == strcmp(key,"package_name")){
            package_info.package_name = bson_iterator_string( it );
        }
        else if( 0 == strcmp(key,"version")){
            package_info.version = bson_iterator_string( it );
        }
        else if( 0 == strcmp(key,"sdk_version")){
            package_info.sdk_version= bson_iterator_string( it );
        }
        else{
            continue;
        }
    }
    return 0;
}
*/
int ContentList::get_download_info_from_cursor(mongo_cursor * cursor, download_info_t& download_info)
{
    const bson* result;
    const char* key;
    bson_iterator it[1];
    result = mongo_cursor_bson( cursor );
    bson_iterator_init( it, result);
    //string img_url;
    stringstream ss;
    while( bson_iterator_next(it) ){
        ss.clear();
        key = bson_iterator_key(it);
        if(0==strcmp(key,"id")){
            download_info.down_id = bson_iterator_string(it);
        }
        else if(0==strcmp(key,"path_url")){
            download_info.down_url = bson_iterator_string(it);
        }
        else if(0==strcmp(key,"charge_typeid")){
            download_info.charge_id = bson_iterator_string(it);
            mcp_content_map::get_charge_name(bson_iterator_string(it), download_info.charge_name);
        }
        else if(0==strcmp(key,"value")){
            ss<<bson_iterator_int(it);
            ss>>download_info.charge_value;
        }
        else if(0==strcmp(key,"c_id")){
            download_info.id=bson_iterator_string(it);
        }
        else{
            //continue
        }
    }
    return 0;
}

int ContentList::get_soft_info_from_cursor(mongo_cursor * cursor, soft_info_t& soft_info)
{
    const bson* result;
    //const char* key;
    bson_iterator it[1];
    result = mongo_cursor_bson( cursor );
    bson_iterator_init( it, result);
    get_soft_info_from_bson_iterator(it, soft_info);
/*
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        if(0==strcmp(key,"id")){
            soft_info.id=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"name")){
            soft_info.name=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"info")){
            soft_info.info=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"version")){
            soft_info.version=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"releasedate")){
            soft_info.date=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"froms")){
            soft_info.froms=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"adapt")){
            soft_info.adapt=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"pop")){
            soft_info.pop=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"path_url")){
            soft_info.down_url=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"size")){
            soft_info.size=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"class")){
            get_soft_class_info_from_bson(it,soft_info);
        }
        else if(0==strcmp(key,"img_info")){
            get_img_info_from_bson(it,soft_info);
        }
        else{
        }
    }
*/
    return 0;
}

int ContentList::get_soft_info_from_bson_iterator(bson_iterator* it, soft_info_t& soft_info)
{
    const char* key;
    stringstream ss;
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        if(0==strcmp(key,"id")){
            soft_info.id=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"name")){
            soft_info.name=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"info")){
            soft_info.info=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"version")){
            soft_info.version=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"releasedate")){
            soft_info.date=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"froms")){
            soft_info.froms=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"adapt")){
            soft_info.adapt=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"pop")){
            soft_info.pop=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"path_url")){
            soft_info.down_url=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"size")){
            soft_info.size=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"class")){
            get_soft_class_info_from_bson(it,soft_info);
        }
        else if(0==strcmp(key,"img_info")){
            get_img_info_from_bson(it,soft_info);
        }
        else if(0==strcmp(key,"star")){
            ss.clear();
            ss<<bson_iterator_double(it);
            soft_info.star=ss.str();
        }
		else if(0==strcmp(key,"show_highspeed")){
            soft_info.show_highspeed=bson_iterator_string(it);
        }
    }
    return 0;
}

int ContentList::get_ios_content_from_cursor(mongo_cursor * cursor, ios_content_t& ios_content )
{
    const bson* result;
    result = mongo_cursor_bson( cursor );
    return content::get_ios_content_from_bson(result, ios_content);
}

online_game_t ContentList::getOnlineGameFromCursor(mongo_cursor *cursor)
{
    online_game_t online_game;
    const bson* result;
    result = mongo_cursor_bson( cursor );
    content::getOnlineGameFromBson(result, online_game);
    return online_game;
}


int ContentList::get_android_theme_from_cursor(mongo_cursor * cursor, android_theme_t& theme_info)
{
    const bson* result;
    const char* key;
    bson_iterator it[1];
    result = mongo_cursor_bson( cursor );
    bson_iterator_init( it, result);
    while(bson_iterator_next(it)){
        key=bson_iterator_key(it);
        if(0==strcmp(key,"id")){
            theme_info.id=bson_iterator_string(it);
            UB_LOG_DEBUG("id[%s]", bson_iterator_string(it));
        }
        else if(0==strcmp(key,"name")){
            theme_info.name=bson_iterator_string(it);
            UB_LOG_DEBUG("name[%s]", bson_iterator_string(it));
        }
        else if(0==strcmp(key,"info")){
            theme_info.info=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"releasedate")){
            theme_info.date=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"froms")){
            theme_info.froms=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"adapt")){
            theme_info.adapt=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"pop")){
            theme_info.pop=bson_iterator_string(it);
            UB_LOG_DEBUG("pop[%s]", bson_iterator_string(it));
        }
        else if(0==strcmp(key,"path_url")){
            theme_info.down_url=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"size")){
            theme_info.size=bson_iterator_string(it);
        }
        else if(0==strcmp(key,"class")){
            get_theme_class_info_from_bson(it,theme_info);
        }
        else if(0==strcmp(key,"img_info")){
            get_img_info_from_bson(it,theme_info);
        }
        else{
        }
    }
    return 0;
}

void ContentList::get_class_info_from_bson(bson_iterator* bson_it, basic_info_t& basic_info)
{
    bson_iterator sub[1];
    bson_iterator_subiterator( bson_it, sub );
    bson_iterator subsub[1];
    class_info_t class_info;
    UB_LOG_DEBUG("get_class_info_from_bson");
    while( bson_iterator_next(sub) ){
        if( !basic_info.class_id.empty() || basic_info.class_id.compare("") != 0 ){
            basic_info.class_id.append( "," );
            basic_info.class_name.append( "," );
        }
        bson_iterator_subiterator( sub, subsub );
        class_info.clear();
        while( bson_iterator_next(subsub) ){
            if( strcmp(bson_iterator_key(subsub),"id")==0 ){
                class_info.id=bson_iterator_string(subsub);
                UB_LOG_DEBUG( "++++class_id[%s]++++",bson_iterator_string(subsub));
            }
        }
        mcp_content_map::get_class_info(class_info.id, class_info);
        basic_info.class_id.append(class_info.id);
        basic_info.class_name.append(class_info.name);
    }
}

void ContentList::get_img_info_from_bson(bson_iterator* bson_it, string& img_url)
{
    img_url.clear();
    string img_type;
    bson_iterator sub[1];
    if( bson_iterator_type(bson_it)==BSON_ARRAY ){
        bson_iterator_subiterator( bson_it, sub );
        bson_iterator subsub[1];
        while( bson_iterator_next(sub) ){
            bson_iterator_subiterator( sub, subsub );
            img_type.clear();
            img_url.clear();
            while( bson_iterator_next(subsub) ){
                if( strcmp(bson_iterator_key(subsub),"img_type")==0 ){
                    img_type = bson_iterator_string(subsub);
                }
                else if( strcmp(bson_iterator_key(subsub),"img_url")==0 ){
                    img_url = bson_iterator_string(subsub);
                }
            }
            if( img_type.compare("3") == 0 ){
                break;
            }
            else{
                img_type.clear();
                img_url.clear();
            }
        }
    }
}

void ContentList::get_soft_class_info_from_bson(bson_iterator* it, soft_info_t& soft_info)
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
                soft_info.class_info_list.push_back(class_info);
            }
        }
    }   
    return;
}  

/*
void ContentList::get_ios_class_info_from_bson(bson_iterator* it, ios_content_t& ios_content)
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
                ios_content.class_list.push_back(class_info);
            }
        }
    }
    content::get_ios_class_info_from_bson(it, ios_content);
    return;
}
*/

void ContentList::get_theme_class_info_from_bson(bson_iterator* it, android_theme_t& theme_info)
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
                theme_info.class_info_list.push_back(class_info);
            }
        }
    }   
    return;
}  

void ContentList::get_img_info_from_bson(bson_iterator* bson_it, soft_info_t& soft_info)
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
        soft_info.img_info.push_back(img_info);
        if(img_info.type.compare("4")==0){
            soft_info.icon=img_info.url;
        }
        else{
            if(!soft_info.preview.empty()&&soft_info.preview.compare("")!=0){
                soft_info.preview.append(",");
            }
            soft_info.preview.append(img_info.url);
        }
    }
    return;
}

/*
void ContentList::get_img_info_from_bson(bson_iterator* bson_it, ios_content_t& ios_content)
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
        //ios icon type
        if(img_info.type.compare("9")==0){
            ios_content.icon=img_info.url;
        }else{
            ios_content.img_list.push_back(img_info.url);
        }
    }
    content::get_img_info_from_bson(bson_it,ios_content);
    return;
}
*/

void ContentList::get_img_info_from_bson(bson_iterator* bson_it, android_theme_t& theme_info)
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
        theme_info.img_info.push_back(img_info);
        if(img_info.type.compare("4")==0){
            theme_info.icon=img_info.url;
        }
        else{
            if(!theme_info.preview.empty()&&theme_info.preview.compare("")!=0){
                theme_info.preview.append(",");
            }
            theme_info.preview.append(img_info.url);
        }
    }
    return;
}

void ContentList::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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

void ContentList::get_basic_query_bson(bson* query)
{
    bson_init( query );

    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "star", -1 );
    }
    else if( _priority == 3 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentList::get_basic_query_bson2(bson* query, const int32_t weight_type, const string class_id)
{
    char index[8],classid[8];
    int cnt=0;
    UB_LOG_TRACE("query2 start xxxxxxxxxxxxxx");
    bson_init( query );

    bson_append_start_object(query, "$query");
    bson_append_start_object(query, "class.id");
    bson_append_start_array(query,"$in");
    if(weight_type == 1)
    {
        memset(index,0,sizeof(index));
        sprintf(index,"%d",cnt);
        cnt++;
        bson_append_string(query, index, class_id.c_str());
    }
    else
    {
        for(int i=97; i<103; ++i)
        {
            memset(index,0,sizeof(index));
            memset(classid,0,sizeof(classid));
            sprintf(index,"%d",i-97);
            sprintf(classid,"%d",i);
            bson_append_string(query, index, classid);
            ++cnt;
        }
        memset(index,0,sizeof(index));
        sprintf(index,"%d",cnt);
        bson_append_string(query, index, "104");
        ++cnt;
        if(weight_type == 2)
        {
            memset(index,0,sizeof(index));
            sprintf(index,"%d",cnt);
            bson_append_string(query, index, "108");
            ++cnt;
        }
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_append_start_object(query, "$orderby");
    switch(weight_type)
    {
        case 1:
            bson_append_int(query,"class_weight",-1);
            bson_append_int(query,"utime",-1);
            break;
        case 2:
            //fall through
        case 3:
            bson_append_int(query,"latest_weight",-1);
            bson_append_int(query,"utime",-1);
            break;
        case 4:
            bson_append_int(query,"download_weight",-1);
            bson_append_int(query,"ptype_5",-1);
            break;
        case 5:
            bson_append_int(query,"pop_weight",-1);
            bson_append_int(query,"star",-1);
            break;
        default:
            //never come here
            break;
    }
    bson_append_finish_object(query);

    bson_finish( query );
    return;
}

void ContentList::get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );

    bson_append_start_object(query, "id");
    bson_append_start_array(query, "$in");
    map<string,vector<download_info_t> >::const_iterator iter = m_id_download_info.begin();
    for( int j=0; iter !=m_id_download_info.end(); ++iter,++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_string(query, index, (*iter).first.c_str());
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "star", -1 );
    }
    else if( _priority == 3 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    bson_append_finish_object(query);
    bson_finish( query );
    return;
}

void ContentList::get_basic_query_bson2(const map<string,vector<download_info_t> >& m_id_download_info,bson* query, const int32_t weight_type, const string class_id)
{
    UB_LOG_TRACE("query 2 start with download xxxxxxxxxxxxxxxxxxxxxxx");
    char index[8],classid[8];
    int cnt;

    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_start_object( query, "class.id");
    bson_append_start_array(query, "$in");
    if(weight_type == 1)
    {
        memset(index,0,sizeof(index));
        sprintf(index,"%d",cnt);
        cnt++;
        bson_append_string(query, index, class_id.c_str());
    }
    else
    {
        for(int i=97; i<103; ++i)
        {
            memset(index,0,sizeof(index));
            memset(classid,0,sizeof(classid));
            sprintf(index,"%d",i-97);
            sprintf(classid,"%d",i);
            bson_append_string(query, index, classid);
            ++cnt;
        }
        memset(index,0,sizeof(index));
        sprintf(index,"%d",cnt);
        bson_append_string(query, index, "104");
        ++cnt;
        if(weight_type == 2)
        {
            memset(index,0,sizeof(index));
            sprintf(index,"%d",cnt);
            bson_append_string(query, index, "108");
            ++cnt;
        }
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_append_start_object(query, "id");
    bson_append_start_array(query, "$in");
    map<string,vector<download_info_t> >::const_iterator iter = m_id_download_info.begin();
    for( int j=0; iter !=m_id_download_info.end(); ++iter,++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_string(query, index, (*iter).first.c_str());
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_append_start_object(query, "$orderby");
    switch(weight_type)
    {
        case 1:
            bson_append_int(query,"class_weight",-1);
            bson_append_int(query,"utime",-1);
            break;
        case 2:
            //fall through
        case 3:
            bson_append_int(query,"_weight",-1);
            bson_append_int(query,"utime",-1);
            break;
        case 4:
            bson_append_int(query,"down_weight",-1);
            bson_append_int(query,"ptype_5",-1);
            break;
        case 5:
            bson_append_int(query,"pop_weight",-1);
            bson_append_int(query,"star",-1);
            break;
        default:
            //never come here
            break;
    }
    bson_append_finish_object(query);

    bson_finish( query );
    return;
}


void ContentList::get_basic_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_append_int( fields, "id", 1 );
    bson_append_int( fields, "cp_id", 1 );

    bson_append_int( fields, "name", 1 );
    bson_append_int( fields, "class", 1 );
    bson_append_int( fields, "game_code", 1 );
    bson_append_int( fields, "default_charge_typeid", 1 );
    bson_append_int( fields, "default_charge_value", 1 );
    bson_append_int( fields, "img_info", 1 );
    bson_append_int( fields, "logdate", 1 );
    bson_append_int( fields, "package_name", 1 );
    bson_append_int( fields, "version", 1 );
    bson_append_int( fields, "version_appinfo", 1 );
    bson_append_int( fields, "sdk_version", 1 );
    bson_append_int( fields, "version_code", 1 );
    bson_append_int( fields, "show_highspeed", 1 );

    bson_finish( fields );
    return;
}
/*
void ContentList::get_package_query_bson(bson* query)
{
    bson_init( query );

    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "star", -1 );
    }
    else if( _priority == 3 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}  
   
void ContentList::get_package_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );

    bson_append_start_object(query, "id");
    bson_append_start_array(query, "$in");
    map<string,vector<download_info_t> >::const_iterator iter = m_id_download_info.begin();
    for( int j=0; iter !=m_id_download_info.end(); ++iter,++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_string(query, index, (*iter).first.c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_finish( query );
    return;
}

void ContentList::get_package_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_append_int( fields, "c_id", 1 );
    bson_append_int( fields, "package_name", 1 );
    bson_append_int( fields, "version", 1 );
    bson_append_int( fields, "sdk_version", 1 );
    bson_finish( fields );
    return;
}
*/
void ContentList::get_download_query_bson(const vector<basic_info_t>& basic_list, bson* query)
{
    bson_init( query );
    char index[8];

    // [s] add by gwj for only use DuoKu channel
    bson_append_start_object(query,"channel_info");
    bson_append_start_array(query, "$in");
    bson_append_string(query, "0", "DuoKu");
    bson_append_string(query, "1", "TEMP");
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    // [e] add by gwj for only use DuoKu channel

    bson_append_start_object(query, "c_id");
    bson_append_start_array(query, "$in");
    vector<basic_info_t>::const_iterator iter = basic_list.begin();
    for( int j=0; iter !=basic_list.end(); ++iter,++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_string(query, index, iter->id.c_str());
    }   
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_append_start_object(query, "adapt_info");
    bson_append_start_array(query, "$in");
    char* tmp=NULL;
    char *stype_id_list = new char[_adapt_id.size()+1];
    memset( stype_id_list, 0x00, _adapt_id.size()+1);
    strcpy( stype_id_list, _adapt_id.c_str() );
    char *p_id_list = stype_id_list;
    for( int j=0; (tmp=strsep(&p_id_list,","))!=NULL; ++j ){
        memset( index, 0x00, sizeof(index) ); 
        sprintf(index, "%d", j);
        bson_append_start_object(query, index);
        bson_append_int(query, "stype", _adapt_type);
        bson_append_string(query, "stype_id", tmp);
        bson_append_finish_object(query);
    }           
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_finish(query);
    delete[] stype_id_list;
    return;
}

void ContentList::get_download_query_bson(bson* query)
{
    bson_init( query );
    char index[8];

    // [s] add by gwj for only use DuoKu channel
    bson_append_start_object(query,"channel_info");
    bson_append_start_array(query, "$in");
    bson_append_string(query, "0", "DuoKu");
    bson_append_string(query, "1", "TEMP");
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    // [e] add by gwj for only use DuoKu channel

    bson_append_start_object(query, "adapt_info");
    bson_append_start_array(query, "$in");
    char* tmp=NULL;
    char *stype_id_list = new char[_adapt_id.size()+1];
    memset( stype_id_list, 0x00, _adapt_id.size()+1);
    strcpy( stype_id_list, _adapt_id.c_str() );
    for( int j=0; (tmp=strsep(&stype_id_list,","))!=NULL; ++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_start_object(query, index);
        bson_append_int(query, "stype", _adapt_type);
        bson_append_string(query, "stype_id", tmp);
        bson_append_finish_object(query);
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_finish(query);
    delete[] stype_id_list;
    return;
}

void ContentList::get_download_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_append_int(fields, "c_id", 1 );
    bson_append_int(fields, "id", 1 );
    bson_append_int(fields, "path_url", 1);
    bson_append_int(fields, "charge_typeid", 1);
    bson_append_int(fields, "value", 1);
    bson_finish( fields );
    return;
}

void ContentList::get_count_query_bson(bson* query)
{
    UB_LOG_TRACE("ContentList get_count_query_bson 1");
    bson_init( query );
	UB_LOG_TRACE("this does not happen!");
    bson_append_string(query, "content_type", _type.c_str() );
    bson_finish( query );
    return;
}

void ContentList::get_count_query_bson(bson* query, int weight)
{
    UB_LOG_TRACE("ContentList get_count_query_bson 1");
    bson_init( query );
	UB_LOG_TRACE("this does not happen!");
    bson_append_string(query, "content_type", _type.c_str() );
    bson_finish( query );
    return;
}


void ContentList::get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info, bson* query)
{
    char index[8];
    UB_LOG_TRACE("ContentList get_count_query_bson 2");
    bson_init( query );
    bson_append_string( query, "content_type", _type.c_str() );

    bson_append_start_object(query, "id");
    bson_append_start_array(query, "$in");
    map<string,vector<download_info_t> >::const_iterator iter = m_id_download_info.begin();
    for( int j=0; iter !=m_id_download_info.end(); ++iter,++j ){
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", j);
        bson_append_string(query, index, (*iter).first.c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);

    bson_finish( query );
    return;
}

void ContentList::get_count_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_append_int(fields, "count", 1 );
    bson_finish( fields );
    return;
}

void ContentList::get_soft_query_bson(bson* query)
{
    bson_init( query );

    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentList::get_soft_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_finish( fields );
    return;
}

void ContentList::get_android_theme_query_bson(bson* query)
{
    bson_init( query );

    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentList::get_android_theme_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_finish( fields );
    return;
}

void ContentList::get_ios_query_bson(bson* query)
{
    bson_init( query );

    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentList::get_ios_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_finish( fields );
    return;
}

void ContentList::getOnlineGameQueryBson(bson *query)
{
    bson_init( query );

    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentList::getOnlineGameFieldsBson(bson *fields)
{
    bson_init(fields);
    bson_finish(fields);
    return;
}

void ContentList::get_game_brief_query_bson(bson* query)
{
	bson_append_start_object( query, "$orderby" );
	bson_append_finish_object( query );
	bson_finish( query );
	return;
}

void ContentList::get_game_brief_fields_bson(bson* fields)
{
	bson_init( fields );
	bson_finish( fields );
	return;
}

string ContentList::get_mongo_namespace( string mongo_db_name, string mongo_collection_name )
{
	if( mongo_db_name.empty()||mongo_collection_name.empty() ){
		return string();
	}
	else{
		string ns( mongo_db_name + "." + mongo_collection_name );
		return ns;
	}
}

string ContentList::convert_mongo_ts_to_string(bson_iterator* it)
{
	time_t ts= bson_iterator_time_t(it);
	struct tm *t;
	//t = localtime(&ts);
	t = gmtime(&ts);
	string time_string;
	char c_time[32];
	memset(c_time, 0x00, sizeof(c_time));
	sprintf( c_time, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec );
	time_string = c_time;
	return time_string;
}

int ContentList::get_game_brief_list(int32_t& count, vector<game_brief_t>& list)
{
	UB_LOG_TRACE( "ContentList::get_game_brief_list start1" );
	int res(0);

	count = this->get_content_count();
	UB_LOG_TRACE("count is %d", count);
	if( count==0 ){
		return 0;
	}    
	else if( count<0 ){
		UB_LOG_FATAL( "get_game_brief failed, [%s:%d]", __FILE__, __LINE__ );
		return count;
	}    

	res = this->get_game_brief_list(list);
	if( res < 0 ){ 
		UB_LOG_FATAL( "get_game_brief_list failed, [%s:%d]", __FILE__, __LINE__ );
		return res; 
	}    
	UB_LOG_TRACE( "ContentList::get_game_brief_list end1" );
	return 0;
}

int ContentList::get_game_brief_list(vector<game_brief_t>& list)
{
	UB_LOG_TRACE( "ContentList::get_game_brief_list start2" );
	if( _conn == NULL ){
		UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
		return -5;
	}

	int res = 0;
	bson query[1], fields[1];
	mongo_cursor *cursor = NULL;

	this->get_game_brief_query_bson(query);
	this->get_game_brief_fields_bson(fields);

	cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
	if( cursor == NULL ){
		UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
		free_data( NULL, query, fields, NULL );
		return -1;
	}

	list.clear();
	game_brief_t game_brief;
	UB_LOG_TRACE("before");
	while( mongo_cursor_next( cursor ) == MONGO_OK ){
		game_brief.clear();
		res=this->get_game_brief_from_cursor(cursor, game_brief);
		if(res==0){
			list.push_back( game_brief );
			UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", game_brief.info_id.c_str(), __FILE__, __LINE__ );
		}
	}
	UB_LOG_TRACE("after");

	free_data( cursor, query, fields, NULL );
	UB_LOG_TRACE( "ContentList::get_game_brief_list end2" );
	return 0;

}

int ContentList::get_game_brief_from_cursor(mongo_cursor * cursor, game_brief_t& game_brief )
{
	const bson* result;
	result = mongo_cursor_bson( cursor );
	return content::get_game_brief_info_from_bson(result, game_brief);
}

int ContentList::get_H5_music_id_list(int32_t& count, vector<string>& id_list, string col_id)
{
    UB_LOG_TRACE( "get_H5_music_id_list start" );
    UB_LOG_TRACE( "++++++++++++++++++++++++++++");
    int res(0);
/*
    count = this->get_content_count();
    if( count==0 ){
        return 0;
    }
    else if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }
*/
    res = this->get_H5_music_id_list(id_list, col_id);
    if( res < 0 ){
        UB_LOG_FATAL( "get_android_theme_basic_list failed, [%s:%d]", __FILE__, __LINE__ );
        return res;
    }
    UB_LOG_TRACE( "get_H5_music_id_list end" );
    return 0;
}

int ContentList::get_H5_music_id_list(vector<string>& id_list, string col_id)
{
    UB_LOG_TRACE( "get_H5_music_id_list start1" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_H5_music_id_query_bson(query, col_id);
    this->get_H5_music_id_fields_bson(fields);

    
    UB_LOG_TRACE( "in++++++++++");    
    cursor = mongo_find( _conn, "mcp.mcp_content_H5_music_id", query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_TRACE( "in1111++++++++++");    
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    uint32_t id;
    UB_LOG_TRACE( "in2222++++++++++");    
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        UB_LOG_TRACE( "in-------");    
        res=this->get_H5_music_id_from_cursor(cursor, id_list);
        if(res==0){
            //UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", theme_info.id.c_str(), __FILE__, __LINE__ );
            break;
        }

    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_DEBUG( "get_H5_music_id_list end" );
    return 0;
}

void ContentList::get_H5_music_id_query_bson(bson* query, string col_id)
{
    bson_init( query );

    bson_append_string( query, "column_id", col_id.c_str() );

    bson_finish( query );
    return;
}

void ContentList::get_H5_music_id_fields_bson(bson* fields)
{
    bson_init( fields );
    bson_finish( fields );
    return;
}

int ContentList::get_H5_music_id_from_cursor(mongo_cursor * cursor, vector<string>& id_list)
{
        const bson* result;
        const char* key;
        bson_iterator it[1];
        result = mongo_cursor_bson( cursor );
        bson_iterator_init( it, result);
        while(bson_iterator_next(it)){
                UB_LOG_TRACE( "in2222--------");
                key=bson_iterator_key(it);
                if(0==strcmp(key,"cid_list")){
                    this->get_H5_music_ids_from_bson(it, id_list);        
                }
        }
        return 0;
}

void ContentList::get_H5_music_ids_from_bson(bson_iterator* bson_it, vector<string>& id_list)
{
    bson_iterator sub[1];
    bson_iterator_subiterator( bson_it, sub );
    bson_iterator subsub[1];
    class_info_t class_info;
    UB_LOG_DEBUG("get_class_info_from_bson");
    string strId;
    while( bson_iterator_next(sub) ){
            /*
        if( !basic_info.class_id.empty() || basic_info.class_id.compare("") != 0 ){
            basic_info.class_id.append( "," );
            basic_info.class_name.append( "," );
        }
        bson_iterator_subiterator( sub, subsub );
        class_info.clear();
        while( bson_iterator_next(subsub) ){
            if( strcmp(bson_iterator_key(subsub),"id")==0 ){
                class_info.id=bson_iterator_string(subsub);
                UB_LOG_DEBUG( "++++class_id[%s]++++",bson_iterator_string(subsub));
            }
        }
        mcp_content_map::get_class_info(class_info.id, class_info);
        basic_info.class_id.append(class_info.id);
        basic_info.class_name.append(class_info.name);
        */
            UB_LOG_TRACE( "in333-----");
       strId = bson_iterator_string(sub);
       UB_LOG_TRACE( "id is %s", strId.c_str());
       id_list.push_back(strId);
    }
}


