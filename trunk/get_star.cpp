#include "get_star.h"

using namespace std;

void GetStar::get_star_query_bson(bson* query)
{
    bson_init(query);
    bson_append_string(query,"id",_cid.c_str());
    bson_finish(query);
}

void GetStar::get_star_field_bson(bson* fields)
{
    bson_init(fields);
    bson_append_int(fields, "star" ,1);
    bson_finish(fields);
    return;
}

void GetStar::free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result )
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

int GetStar::get_star(uint32_t& star)
{
    if( _conn == NULL ){
        UB_LOG_FATAL( "mongo connection is NULL, [%s:%d]", __FILE__, __LINE__ );
        return -5;
    }

    bson query, fields, bson_out;
    bson_iterator bson_it[1]; 
    
    this->get_star_query_bson(&query);
    this->get_star_field_bson(&fields);
    int res=0;
    if(0==strcmp(_typeid.c_str(),"15"))
    {
        res = mongo_find_one( _conn, "mcp.mcp_software_content", &query, &fields, &bson_out ); 
    }
    else if(0 ==strcmp(_typeid.c_str(),"11"))
    {
        res = mongo_find_one( _conn, "mcp.mcp_content", &query, &fields, &bson_out );
    }

    bson_destroy( &query );
    bson_destroy( &fields );

    if( res != MONGO_OK )
    {    
        UB_LOG_FATAL( "query star where cid(%s) failed", _cid.c_str() );
        return -1;
    }
    
    bson_iterator_init( bson_it, &bson_out );
    
    const char* key;
    while( bson_iterator_next( bson_it ) )
    {
        key = bson_iterator_key( bson_it );
        if( strcmp( key, "star" ) == 0 )
        {
            star = bson_iterator_int( bson_it );
        }
    }
    bson_destroy( &bson_out);
    return 0;
}
