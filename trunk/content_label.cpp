#include "content_label.h"
#include "ub_server.h"

using namespace std;
using namespace content;

void ContentLabel::get_basic_query_bson(bson* query)
{
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "label_id", _label_id.c_str() );
    bson_append_finish_object( query );

    bson_finish( query );
    return;
}

void ContentLabel::get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query)
{
    char index[8];
    bson_init( query );
    bson_append_start_object( query, "$query" );
    bson_append_string( query, "label_id", _label_id.c_str() );

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
    return ;
}

void ContentLabel::get_count_query_bson(bson* query)
{
    bson_init( query );
    bson_append_string(query, "label_id", _label_id.c_str() );
    bson_finish( query );
    return;
}

void ContentLabel::get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info, bson* query)
{
    bson_init( query );
    bson_append_string(query, "label_id", _label_id.c_str() );

    char index[8];
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

