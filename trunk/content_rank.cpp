#include "content_rank.h"
#include "ub_server.h"

using namespace std;
using namespace content;

void ContentRank::get_basic_query_bson(bson* query)
{
    bson_init(query);
    bson_append_start_object(query, "$query");
    bson_append_string(query, "content_type", _type.c_str());
    if( !_class_id.empty() && _class_id.compare("")!=0 ){
        bson_append_string(query, "class.id", _class_id.c_str());
    }
    bson_append_finish_object(query);

    char ptype[8];
    memset(ptype, 0x00, sizeof(ptype));
    sprintf( ptype, "ptype_%d", _p_type );
    bson_append_start_object(query, "$orderby");
    bson_append_int(query, ptype, -1);
    bson_append_finish_object(query);

    bson_finish(query);
    return;
}

void ContentRank::get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "content_type", _type.c_str() );
    if( !_class_id.empty() && _class_id.compare("")!=0 ){
        bson_append_string( query, "class.id", _class_id.c_str() );
    }

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

    char ptype[8];
    memset(ptype, 0x00, sizeof(ptype));
    sprintf( ptype, "ptype_%d", _p_type );
    bson_append_start_object(query, "$orderby");
    bson_append_int(query, ptype, -1);
    bson_append_finish_object(query);

    bson_finish(query);
    return;
}

void ContentRank::get_count_query_bson(bson* query)
{
    bson_init(query);
    bson_append_string(query, "content_type", _type.c_str());
    if( !_class_id.empty() && _class_id.compare("")!=0 ){
        bson_append_string(query, "class.id", _class_id.c_str() );
    }
    bson_finish(query);
    return;
}

void ContentRank::get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init(query);
    bson_append_string(query, "content_type", _type.c_str());
    if( !_class_id.empty() && _class_id.compare("")!=0 ){
        bson_append_string(query, "class.id", _class_id.c_str());
    }

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

void ContentRank::get_ios_query_bson(bson* query)
{
    bson_init(query);
    bson_append_start_object(query, "$query");
    bson_append_string(query, "content_type", _type.c_str());
    if( !_class_id.empty() && _class_id.compare("")!=0 ){
        bson_append_string(query, "class.id", _class_id.c_str());
    }
    if( _for_free ==1 ){
        bson_append_int(query, "value", 0 );
    }
    bson_append_finish_object(query);

    bson_append_start_object(query, "$orderby");
    switch(_p_type){
        case 0:
        case 1:
            bson_append_int(query, "gdate", -1);
            break;
        case 2:
            bson_append_int(query, "crt_rating", -1);
            if(_type=="23"){
                bson_append_int(query, "crt_count", -1);
            }else{
                bson_append_int(query, "gdate", -1);
            }
            break;
        case 3:
            bson_append_int(query, "ptype_1", -1);
            break;
        case 4:
            bson_append_int(query, "ptype_2", -1);
            break;
        case 5:
            bson_append_int(query, "ptype_3", -1);
            break;
        case 6:
            bson_append_int(query, "ptype_4", -1);
            break;
        case 7:
            bson_append_int(query, "ptype_5", -1);
            break;
        default:
            break;
    }
    bson_append_finish_object(query);

    bson_finish(query);
    return;
}

void ContentRank::getOnlineGameQueryBson(bson *query)
{
    bson_init(query);
    bson_append_start_object(query, "$query");
    bson_append_string(query, "content_type", _type.c_str());
    if( !_class_id.empty() && _class_id.compare("")!=0 ){
        bson_append_string(query, "class.id", _class_id.c_str());
    }
    bson_append_finish_object(query);

    bson_append_start_object(query, "$orderby");
    switch(_p_type){
        case 0:
        case 1:
            bson_append_int(query, "gdate", -1);
            break;
        case 2:
            bson_append_int(query, "star", -1);
            break;
        case 3:
            bson_append_int(query, "ptype_1", -1);
            break;
        case 4:
            bson_append_int(query, "ptype_2", -1);
            break;
        case 5:
            bson_append_int(query, "ptype_3", -1);
            break;
        case 6:
            bson_append_int(query, "ptype_4", -1);
            break;
        case 7:
            bson_append_int(query, "ptype_5", -1);
            break;
        default:
            break;
    }
    bson_append_finish_object(query);

    bson_finish(query);
    return;
}
