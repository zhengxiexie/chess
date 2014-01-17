#include "content_class.h"
#include "ub_server.h"

using namespace std;
using namespace content;

int ContentClass::get_content_list_with_weight(int32_t &count, vector<content_info_t>& list, int32_t weight_type)
{
    UB_LOG_TRACE( "get_content_list start" );
    int res(0);
    list.clear();

    res = this->get_content_list_all(weight_type, list);
    if(res < 0)
    {
        return res;
    }
    count = res;

    UB_LOG_TRACE( "get_content_list end" );
    return res;
}

int ContentClass::get_content_list_all(int32_t weight_type, vector<content_info_t>& list)
{
    UB_LOG_TRACE( "get_content_list_all start" );
//    if(_priority==0){
        //return ContentList::get_content_list_all();
//    }

    int res(0), count(0);
    count = this->get_content_count(weight_type);
    UB_LOG_TRACE("content class get list all count is %d", count);
    if( count==0 ){
        return 0;
    }
    else if( count<0 ){
        UB_LOG_FATAL( "get_content_count failed, [%s:%d]", __FILE__, __LINE__ );
        return count;
    }

    vector<string> id_list;
    res=this->get_id_list(id_list, weight_type);
    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _class_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    vector<basic_info_t> basic_list;
    res = this->get_content_basic_info(id_list,basic_list);
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

    combine_content_list(basic_list, download_list, list);
    UB_LOG_TRACE( "get_content_list_all end" );
    return count;
}

int ContentClass::get_content_basic_info(const vector<string>& id_list, vector<basic_info_t>& basic_list)
{
    UB_LOG_TRACE( "get_content_basic_info start" );

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;

    this->get_basic_query_bson(id_list, query);
    this->get_basic_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    basic_info_t basic_info;
    map<string,basic_info_t> m_id_basic_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        basic_info.clear();
        res=this->get_basic_info_from_cursor(cursor, basic_info);
        if(res==0){
            //basic_list.push_back( basic_info );
            m_id_basic_info[basic_info.id] = basic_info;
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", basic_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    basic_list.clear();
    map<string,basic_info_t>::iterator map_iter;
    for(vector<string>::const_iterator it = id_list.begin(); it != id_list.end(); ++it){
        map_iter = m_id_basic_info.find(*it);
        if( map_iter != m_id_basic_info.end() ){
            basic_list.push_back(map_iter->second);
        }
    }
    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_basic_info end" );
    return 0;
}

int ContentClass::get_content_soft_list(vector<soft_info_t>& soft_list)
{
    UB_LOG_TRACE( "get_content_soft_list start" );

    if(_priority!=1){
        return ContentList::get_content_soft_list(soft_list);
    }

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    vector<string> id_list;
    res=this->get_id_list(id_list);
    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _class_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    this->get_soft_query_bson(id_list,query);
    this->get_soft_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    soft_list.clear();
    soft_info_t soft_info;
    map<string,soft_info_t> m_id_soft_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        soft_info.clear();
        res=this->get_soft_info_from_cursor(cursor, soft_info);
        if(res==0){
            m_id_soft_info[soft_info.id]=soft_info;
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", soft_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    vector<string>::const_iterator it = id_list.begin();
    for(; it!= id_list.end(); ++it ){
        soft_list.push_back(m_id_soft_info[*it]);
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_TRACE( "get_content_soft_list end" );
    return 0;
}

int ContentClass::get_ios_content_list(int32_t& count, vector<ios_content_t>& list)
{
    return ContentList::get_ios_content_list(count,list);
}

int ContentClass::get_ios_content_list(vector<ios_content_t>& list)
{
    UB_LOG_TRACE( "get_ios_content_list start" );
    return ContentList::get_ios_content_list(list);
    UB_LOG_TRACE( "get_ios_content_list end" );
    return 0;
}

int ContentClass::getOnlineGameList(int32_t& count, vector<online_game_t>& list)
{
    return ContentList::getOnlineGameList(count,list);
}

int ContentClass::getOnlineGameList(vector<online_game_t>& list)
{
    UB_LOG_TRACE( "ContentClass::getOnlineGameList start" );
    if( _priority != 0 ){
        return ContentList::getOnlineGameList(list);
    }
    int res(0);
    bson command[1], out[1];
    this->getOnlineGameCommand(command);
    res = mongo_run_command(_conn, "mcp", command, out);
    if(res!=0){
        UB_LOG_FATAL( "run mongo_command failed, [%s:%d]", __FILE__, __LINE__ );
        free_data(NULL, command, out, NULL );
        return -1;
    }

    list.clear();
    bson_iterator it[1], sub[1];
    bson subsub[1];
    bson_find(it, out, "result");
    bson_iterator_subiterator( it, sub );
    while(bson_iterator_next(sub)){
        bson_iterator_subobject( sub, subsub );
        list.push_back(getOnlineGameFromBson(subsub));
    }
    free_data( NULL, command, out, NULL );
    UB_LOG_TRACE( "ContentClass::getOnlineGameList end" );
    return 0;
}

int ContentClass::get_android_theme_basic_list(vector<android_theme_t>& theme_list)
{
    UB_LOG_DEBUG( "get_android_theme_basic_list start" );

    if(_priority!=1){
        return ContentList::get_android_theme_basic_list(theme_list);
    }

    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    int res = 0;
    vector<string> id_list;
    res=this->get_id_list(id_list);
    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _class_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    this->get_android_theme_query_bson(id_list,query);
    this->get_android_theme_fields_bson(fields);

    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    theme_list.clear();
    android_theme_t theme_info;
    map<string,android_theme_t> m_id_theme_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        theme_info.clear();
        res=this->get_android_theme_from_cursor(cursor, theme_info);
        if(res==0){
            m_id_theme_info[theme_info.id]=theme_info;
            UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", theme_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    vector<string>::const_iterator it = id_list.begin();
    for(; it!= id_list.end(); ++it ){
        theme_list.push_back(m_id_theme_info[*it]);
    }

    free_data( cursor, query, fields, NULL );
    UB_LOG_DEBUG( "get_android_theme_basic_list end" );
    return 0;
}


int ContentClass::get_id_list(vector<string>& id_list, int32_t weight_type)
{
    UB_LOG_TRACE( "get_id_list start" );
    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    const bson* result;

    this->get_id_list_query_bson(query, weight_type);
    this->get_id_list_fields_bson(fields);

    string ns, id_name;
    if(weight_type == 0)
    {
        ns = "mcp.mcp_content_class_list";
        id_name = "c_id";
    }
    else
    {
        ns = "mcp.mcp_content";
        id_name = "id";
    }
    string id="";

    UB_LOG_TRACE("weight_type is %d", weight_type);
    UB_LOG_TRACE("_unit_num is %d, _start_pos is %d", _unit_num, _start_pos);
    cursor = mongo_find( _conn, ns.c_str(), query, fields, _unit_num, _start_pos, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }
    id_list.clear();
    int cnt =0;
    char  ch[50];// = const_cast<char *>(bson_iterator_string(it));

    bson_iterator it[1];
    while(mongo_cursor_next(cursor)==MONGO_OK){
        cnt++;
        result=mongo_cursor_bson(cursor);
        bson_find(it, result, id_name.c_str());
        memset(ch,0,sizeof(ch));
        strcpy(ch, bson_iterator_string(it));
        UB_LOG_TRACE("id is %s", ch);
        id_list.push_back(ch);
        UB_LOG_DEBUG("push id[%s]",ch);
    }
    free_data(cursor, query, fields, NULL);
    UB_LOG_TRACE("cnt----------------- is %d", cnt);
    UB_LOG_TRACE( "get_id_list end" );
    return 0;
}


void ContentClass::get_basic_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "class.id", _class_id.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "priority", -1 );
    }
    else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    else if( _priority == 3 ){
        bson_append_int( query, "logdate", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentClass::get_basic_query_bson(const vector<string>& id_list, bson* query)
{
    char index[8];
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator it = id_list.begin();
    for(int i=0; it!=id_list.end(); ++it,++i){
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        UB_LOG_DEBUG("push id[%s]",it->c_str());
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_finish(query);
    return;
}

void ContentClass::get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "class.id", _class_id.c_str() );

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

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "priority", -1 );
    }
    else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentClass::get_soft_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "class.id", _class_id.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "priority", -1 );
    }
    else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentClass::get_soft_query_bson(const vector<string>& id_list, bson* query)
{       
    char index[8];
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator it = id_list.begin();
    for(int i=0; it!=id_list.end(); ++it,++i){
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        //UB_LOG_DEBUG("push id[%s]",it->c_str());
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_finish(query);
    return;
}

void ContentClass::get_android_theme_query_bson(const vector<string>& id_list, bson* query)
{
    char index[8];
    bson_init(query);
    bson_append_start_object(query,"id");
    bson_append_start_array(query,"$in");
    vector<string>::const_iterator it = id_list.begin();
    for(int i=0; it!=id_list.end(); ++it,++i){
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        UB_LOG_DEBUG("push id[%s]",it->c_str());
    }
    bson_append_finish_object(query);
    bson_append_finish_object(query);
    bson_finish(query);
    return;
}

void ContentClass::get_android_theme_query_bson(bson* query)
{   
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "class.id", _class_id.c_str() );
    bson_append_finish_object( query );
    
    bson_append_start_object( query, "$orderby" );
    if( _priority == 3 ){
        bson_append_int( query, "releasedate", -1 );
    }
    else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentClass::get_count_query_bson(bson* query, int32_t weight_type)
{
    int cnt = 0;
    char index[8],classid[8];
    bson_init( query );
    
    UB_LOG_TRACE("weight_type is %d", weight_type);
    UB_LOG_TRACE("class is %s", _class_id.c_str());
    UB_LOG_TRACE("type is %s", _type.c_str());


    //bson_append_start_object(query, "$query");
/*
    bson_append_start_object(query, "class.id");
    bson_append_start_array(query,"$in");

    memset(index,0,sizeof(index));
    sprintf(index,"%d",cnt);
    bson_append_string(query, index, _class_id.c_str());

    bson_append_finish_array(query);
    bson_append_finish_object(query);
*/
    //bson_append_finish_object(query);
        

    //bson_append_string(query, "class.id", _class_id.c_str() );
    if(weight_type == 0)
    {
        bson_append_string(query, "content_type", _type.c_str() );
        bson_append_string(query, "class.id", _class_id.c_str() );
    }    
    else 
    {
        bson_append_start_object(query, "class.id");
        bson_append_start_array(query,"$in");

        if(weight_type == 1)
        {
            memset(index,0,sizeof(index));
            sprintf(index,"%d",cnt);
            cnt++;
            bson_append_string(query, index, _class_id.c_str());
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
    }
    

    bson_finish( query );
    return;
}

void ContentClass::get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info, bson* query, int32_t weight_type)
{
    char index[8], classid[8];
    int cnt=0;
    bson_init( query );
    bson_append_start_object( query, "$query" );
    if(weight_type == 0)
    {
        bson_append_string(query, "content_type", _type.c_str() );
        bson_append_string(query, "class.id", _class_id.c_str() );
    }    
    else 
    {
        bson_append_start_object(query, "class.id");
        bson_append_start_array(query,"$in");

        if(weight_type == 1)
        {
            memset(index,0,sizeof(index));
            sprintf(index,"%d",cnt);
            cnt++;
            bson_append_string(query, index, _class_id.c_str());
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
    }

    //bson_append_start_object( query, "class.id");
    //bson_append_start_array(query, "$in");


    //bson_append_string( query, "content_type", _type.c_str() );
    //bson_append_string( query, "class.id", _class_id.c_str() );

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

void ContentClass::get_id_list_query_bson(bson* query, int32_t weight_type)
{
    int cnt = 0;
    char index[8], classid[8];
    bson_init(query);

    UB_LOG_TRACE("get id list query bson weight_type is %d", weight_type);
    //bson_append_start_object(query, "$query");
    //bson_append_string(query, "class.id", _class_id.c_str());
    //bson_append_finish_object(query);

    if(0 == weight_type)
    {

        bson_append_start_object(query, "$query");
        bson_append_string(query, "class_id", _class_id.c_str());
        bson_append_finish_object(query);

        bson_append_start_object(query, "$orderby");
        bson_append_int(query,"priority",-1);
        bson_append_finish_object( query );
    }
    else
    {
        bson_append_start_object( query, "$query" );
            bson_append_start_object(query, "class.id");
            bson_append_start_array(query,"$in");

            if(weight_type == 1)
            {
                memset(index,0,sizeof(index));
                sprintf(index,"%d",cnt);
                cnt++;
                bson_append_string(query, index, _class_id.c_str());
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


    }
    
    bson_finish( query );
    return;
}
/*
   void ContentClass::get_soft_count_query_bson2(bson* query, )
   {
   bson_init(query);
   bson_append_string(query,"id");
   bson_finish(query);
   return;
   }
   */
void ContentClass::get_id_list_fields_bson(bson* query)
{
    bson_init(query);
    bson_append_int(query,"c_id",1);
    bson_append_int(query,"id",1);
    bson_finish( query );
    return;
}

void ContentClass::get_id_list_fields_bson2(bson* query)
{
    bson_init(query);
    bson_append_int(query,"id",1);
    bson_finish( query );
    return;
}

void ContentClass::get_ios_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "class.id", _class_id.c_str() );
    if( _for_free == 1 ){
        bson_append_int( query, "value", 0 );
    }
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "gdate", -1 );
    }else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }else if( _priority == 3 ){
        bson_append_int( query, "crt_rating", -1 );
        bson_append_int( query, "crt_count", -1 );
    }
    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

void ContentClass::getOnlineGameQueryBson(bson *query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    bson_append_string( query, "class.id", _class_id.c_str() );
    bson_append_finish_object( query );

    bson_append_start_object( query, "$orderby" );
    if( _priority == 1 ){
        bson_append_int( query, "gdate", -1 );
    }else if( _priority == 2 ){
        bson_append_int( query, "ptype_5", -1 );
    }else if(  _priority == 3 ){
        bson_append_int( query, "mdate", -1 );
    }
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}


void ContentClass::getOnlineGameCommand(bson* command)
{
    bson_init(command);
    bson_append_string(command, "aggregate", MCP_ONLINE_GAME_COL.c_str() );
    bson_append_start_array(command,"pipeline");
    bson_append_start_object(command,"0");
    bson_append_start_object(command,"$match");
    bson_append_string(command, "content_type", _type.c_str() );
    bson_append_string(command,"class.id", _class_id.c_str() );
    bson_append_finish_object(command);
    bson_append_finish_object(command);

    bson_append_start_object(command,"1");
    bson_append_string(command,"$unwind", "$class");
    bson_append_finish_object(command);

    bson_append_start_object(command,"2");
    bson_append_start_object(command,"$match");
    bson_append_string(command,"class.id", _class_id.c_str() );
    bson_append_finish_object(command);
    bson_append_finish_object(command);

    bson_append_start_object(command,"3");
    bson_append_start_object(command,"$sort");
    bson_append_int(command,"class.p",-1);
    bson_append_finish_object(command);
    bson_append_finish_object(command);

    bson_append_start_object(command, "4");
    bson_append_int(command, "$skip", _start_pos);
    bson_append_finish_object(command);

    bson_append_start_object(command, "5");
    bson_append_int(command, "$limit", _unit_num);
    bson_append_finish_object(command);
    bson_append_finish_object(command);
    bson_finish(command);
    return;
}

int ContentClass::get_soft_list_with_except_class_ids(vector<soft_info_t>& soft_list, vector<string>& except_class_ids, uint32_t priority, uint32_t& count)
{
    UB_LOG_TRACE("get_soft_list_with_except_class_ids start");
    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    } 

    int res = 0;
    vector<string> id_list;
    res=this->get_id_list(id_list, except_class_ids, priority, count);
    UB_LOG_TRACE("-----------id list count is %d", id_list.size());
    if(id_list.size() > 0)
    {
        UB_LOG_TRACE("id is %s", id_list[0].c_str());
    }

    if(res!=0){
        UB_LOG_FATAL("get_id_list failed, column_id[%s], [%s:%d]", _class_id.c_str(), __FILE__, __LINE__ );
        return res;
    }

    bson query[1], fields[1];
    mongo_cursor *cursor = NULL;
    this->get_soft_query_bson(id_list,query);
    this->get_soft_fields_bson(fields);

    UB_LOG_TRACE("ns is %s", _basic_ns.c_str());
    cursor = mongo_find( _conn, _basic_ns.c_str(), query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }

    soft_list.clear();
    soft_info_t soft_info;
    map<string,soft_info_t> m_id_soft_info;
    while( mongo_cursor_next( cursor ) == MONGO_OK ){
        soft_info.clear();
        res=this->get_soft_info_from_cursor(cursor, soft_info);
        if(res==0){
            m_id_soft_info[soft_info.id]=soft_info;
            //UB_LOG_DEBUG( "+++++++++ push id[%s] ++++++++++, [%s:%d]", soft_info.id.c_str(), __FILE__, __LINE__ );
        }
    }

    vector<string>::const_iterator it = id_list.begin();
    for(; it!= id_list.end(); ++it ){
        soft_list.push_back(m_id_soft_info[*it]);
    }

    free_data( cursor, query, fields, NULL );


    UB_LOG_TRACE("get_soft_list_with_except_class_ids end");
    return 0;
}


void ContentClass::get_id_list_query_bson2(bson* query, vector<string>& except_class_ids, uint32_t priority)
{
    char index[8];
    bson_init(query);

    bson_append_start_object(query, "class.id");
    bson_append_start_array(query, "$nin");

    for(int i=0; i<except_class_ids.size(); i++)
    {   
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", i); 
        bson_append_string(query, index, except_class_ids[i].c_str());
    } 
    bson_append_finish_array(query);
    bson_append_finish_object(query);

    bson_finish( query );
    return;
}

void ContentClass::get_id_list_query_bson(bson* query, vector<string>& except_class_ids, uint32_t priority)
{
    char index[8];
    bson_init(query);

    /*
       bson_append_start_object(query, "class.id");
       bson_append_start_array(query, "$nin");

       for(int i=0; i<except_class_ids.size(); i++)
       {
       memset( index, 0x00, sizeof(index) );
       sprintf(index, "%d", i);
       bson_append_string(query, index, except_class_ids[i].c_str());
       }
       bson_append_finish_array(query);
       bson_append_finish_object(query);
       */

    bson_append_start_object(query, "$query");
    bson_append_start_object(query, "class.id");
    bson_append_start_array(query, "$nin");
    for(int i=0; i<except_class_ids.size(); i++)
    {
        memset( index, 0x00, sizeof(index) );
        sprintf(index, "%d", i);
        bson_append_string(query, index, except_class_ids[i].c_str());
    }
    bson_append_finish_array(query);
    bson_append_finish_object(query);
    bson_append_finish_object(query);
    /*
       bson_append_start_object(query, "$orderby");
       bson_append_int( query, "star", 1 );
       bson_append_finish_object(query);
       */   

    bson_append_start_object( query, "$orderby" );
    if( priority == 1 ){ 
        bson_append_int( query, "star", -1 );
        UB_LOG_TRACE("order by star");
    }    
    else if( priority == 2 ){ 
        bson_append_int( query, "releasedate", -1 ); 
    }
    else if( priority == 3 ){ 
        bson_append_int( query, "ptype_1", -1 );
    }
    else if( priority == 4 ){ 
        bson_append_int( query, "ptype_2", -1 );
    }
    else if( priority == 5 ){ 
        bson_append_int( query, "ptype_3", -1 );
    }
    else if( priority == 6 ){ 
        bson_append_int( query, "ptype_4", -1 );
    }
    else if( priority == 7 ){ 
        bson_append_int( query, "ptype_5", -1 );
        UB_LOG_TRACE("order by total download");
    }

    bson_append_finish_object( query );
    bson_finish( query );
    return;
}

int ContentClass::get_id_list(vector<string>& id_list, vector<string>& except_class_ids, uint32_t priority, uint32_t& count)
{
    UB_LOG_TRACE( "get_id_list start-------------------" );
    bson query[1], fields[1],query2[1];
    mongo_cursor *cursor = NULL;
    const bson* result;

    this->get_id_list_query_bson(query, except_class_ids, priority);
    this->get_id_list_fields_bson2(fields);
    this->get_id_list_query_bson2(query2, except_class_ids, priority);

    count = mongo_count( _conn, "mcp", "mcp_software_content", query2 );
    UB_LOG_TRACE("xxxxxxxxxxxxxxxx count is %d xxxxxxxxxxxxxxx", count);
    cursor = mongo_find( _conn, "mcp.mcp_software_content", query, fields, _page_size, _page_size * (_page-1), 0 );
    //cursor = mongo_find( _conn, "mcp.mcp_software_content", query, fields, 0, 0, 0 );
    UB_LOG_TRACE("page is %d", _page);
    UB_LOG_TRACE("page size is %d", _page_size);
    //cursor = mongo_find( _conn, "mcp.mcp_software_content", query, fields, 0, 0, 0 );
    if( cursor == NULL ){
        UB_LOG_WARNING( "mongo_find has no record, [%s:%d]", __FILE__, __LINE__ );
        free_data( NULL, query, fields, NULL );
        return -1;
    }
    id_list.clear();
    bson_iterator it[1];
    while(mongo_cursor_next(cursor)==MONGO_OK){
        result=mongo_cursor_bson(cursor);
        bson_find(it, result, "id");
        id_list.push_back(bson_iterator_string(it));
        UB_LOG_DEBUG("push id[%s]",bson_iterator_string(it));
    }
    UB_LOG_TRACE("innnnnn count is %d", count);
    free_data(cursor, query, fields, NULL);
    if( query2 != NULL && query2->data != NULL ){
        bson_destroy( query2 );
    }
    UB_LOG_TRACE( "get_id_list end" );
    return 0;
}

int ContentClass::combine_content_list(const vector<basic_info_t>& basic_list,
                                                const vector<download_info_t>& download_list,
                                                 vector<content_info_t>& list)
{
    list.clear();
    content_info_t content_info;
    vector<basic_info_t>::const_iterator basic_it = basic_list.begin();
    for( ; basic_it!=basic_list.end(); ++basic_it ){
        content_info.clear();
        content_info.set_basic_info(*basic_it);
        list.push_back( content_info );
    }

    vector<download_info_t>::const_iterator download_it = download_list.begin();
    vector<content_info_t>::iterator content_it;
    for( ; download_it!=download_list.end(); ++download_it ){
        for( content_it=list.begin(); content_it!=list.end(); ++content_it ){
            if(content_it->id==download_it->id){
                content_it->set_download_info(*download_it);
                break;
            }
        }
    }

    return 0;
}
