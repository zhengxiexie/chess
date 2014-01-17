#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"
#include "content_info.h"
#include "software_info.h"
#include "android_theme.h"
#include "content_list.h"
#include "content_class.h"
#include "content_rank.h"
#include "content_label.h"
#include "content_column.h"
#include "content_topic.h"
#include "content_type.h"
#include "content_cp.h"
#include "ios_content.h"
#include "game_info_content.h"
#include "game_list.h"
#include "game_score.h"
#include "appinfo_content.h"
#include "game_star.h"
#include "game_package_version.h"
#include "online_game.h"
#include "duokoo_tools.h"
#include "rating.h"
#include "mixed_topic_music.h"
#include "duoku_paradise.h"
#include "duoku_paradise_game_list.h"
#include "content_calender.h"
#include "get_star.h"
#include "refactory/client/community_client.h"
// [s] add by gwj for adaptive old_code to refactory_code
#include "./refactory/adapt_old.h"
// [e] add by gwj for adaptive old_code to refactory_code

#include <time.h>

using namespace content;
using namespace DuokooTools;

void set_get_content_list_by_date_result( idl::mcp_get_content_list_by_date_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_value.c_str() );
    out.m_result_params()->set_content_list( i, 10, iter->package_name.c_str() );
	if( iter->version_appinfo.empty() )
	{
    	out.m_result_params()->set_content_list( i, 11, iter->version.c_str() );
	}
	else
	{
    	out.m_result_params()->set_content_list( i, 11, iter->version_appinfo.c_str() );
	}

    out.m_result_params()->set_content_list( i, 12, iter->sdk_version.c_str() );

}

int McpServlet::get_content_list_by_date(const idl::mcp_get_content_list_by_date_params& in,
                                               idl::mcp_get_content_list_by_date_response& out)
{
    UB_LOG_TRACE( "get_content_list_by_date start" );

    int count = 0;
    int res = 0;
    string content_type( in.content_type() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    uint32_t for_adapt( in.for_adapt() );

    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;

    ContentList list( content_type, for_adapt );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    res = list.get_content_list(count,content_info_list);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_result( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i ){
        set_get_content_list_by_date_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_by_date end" );
    return 0;
}

template<typename T>
void set_get_content_list_from_classID_result( T& out,
                                               const uint32_t i, vector<content_info_t>::iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->logdate.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->package_name.c_str() );
	if( iter->version_appinfo.empty() )
	{
    	out.m_result_params()->set_content_list( i, 9, iter->version.c_str() );
	}
	else
	{
    	out.m_result_params()->set_content_list( i, 9, iter->version_appinfo.c_str() );
	}
    out.m_result_params()->set_content_list( i, 10, iter->sdk_version.c_str() );
    out.m_result_params()->set_content_list( i, 11, iter->version_code.c_str() );
    //out.m_result_params()->set_content_list( i, 11, iter->version_code.c_str() );
    
}

int McpServlet::get_content_list_from_classID(const idl::mcp_get_content_list_from_classID_params& in,
                                                    idl::mcp_get_content_list_from_classID_response& out)
{
    UB_LOG_TRACE( "get_content_list_from_classID start" );

    string class_id( in.class_id() );
    string content_type( in.content_type_id() );
    uint32_t priority( in.priority() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    string img_size( in.img_size() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );

    uint32_t for_adapt( in.for_adapt() );

    int res( 0 );
    int32_t count( 0 );
    uint32_t i( 0 );
    vector<content_info_t>::iterator iter;
    vector<content_info_t> content_info_list;

    ContentClass list( content_type, for_adapt, class_id );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    list.set_priority( priority );
    UB_LOG_TRACE("before ---------------");
    res = list.get_content_list(count,content_info_list);
    UB_LOG_TRACE("count is %d ------------", count);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    count= content_info_list.size();
    for( i = 0; iter != content_info_list.end(); ++iter, ++i ){
        UB_LOG_NOTICE( "id, [%s]", iter->id.c_str() );
        set_get_content_list_from_classID_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_from_classID end" );
    return 0;
}

void set_get_content_list_by_rank_result( idl::mcp_get_content_list_by_rank_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_value.c_str() );
}

int McpServlet::get_content_list_by_rank(const idl::mcp_get_content_list_by_rank_params& in,
                                       idl::mcp_get_content_list_by_rank_response& out)
{
    UB_LOG_TRACE( "get_content_list_by_rank start" );
    int count = 0;
    int res = 0;
    string content_type( in.content_type() );
    string class_id( in.class_id() );
    uint32_t p_type = in.p_type();
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    uint32_t for_adapt( in.for_adapt() );

    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;
    vector<string> info_id_list;

    ContentRank list( content_type, for_adapt, class_id, p_type );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    res = list.get_content_list(count,content_info_list);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i ){
        UB_LOG_DEBUG( "id, [%s]", iter->id.c_str() );
        set_get_content_list_by_rank_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_by_rank end" );
    return 0;
}

void set_get_content_list_by_label_result( idl::mcp_get_content_list_by_label_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_value.c_str() );
}

int McpServlet::get_content_list_by_label(const idl::mcp_get_content_list_by_label_params& in,
                                        idl::mcp_get_content_list_by_label_response& out)
{
    UB_LOG_TRACE( "get_content_list_by_label start" );
    int count = 0;
    int res = 0;
    string label_id( in.label_id() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    uint32_t for_adapt( in.for_adapt() );

    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;

    ContentLabel list( "1", for_adapt, label_id );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    res = list.get_content_list(count,content_info_list);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i ){
        UB_LOG_DEBUG( "id, [%s]", iter->id.c_str() );
        set_get_content_list_by_label_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_by_label end" );
    return 0;
}

void set_get_content_list_from_part_result( idl::mcp_get_content_list_from_part_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_value.c_str() );
    out.m_result_params()->set_content_list( i, 10, iter->logdate.c_str() );
    out.m_result_params()->set_content_list( i, 11, iter->package_name.c_str() );
	if( iter->version_appinfo.empty() )
	{
    	out.m_result_params()->set_content_list( i, 12, iter->version.c_str() );
	}
	else
	{
    	out.m_result_params()->set_content_list( i, 12, iter->version_appinfo.c_str() );
	}
    out.m_result_params()->set_content_list( i, 13, iter->sdk_version.c_str() );
    out.m_result_params()->set_content_list( i, 14, iter->version_code.c_str() );
    out.m_result_params()->set_content_list( i, 15, iter->show_highspeed.c_str() );
}

int McpServlet::get_content_list_from_part(const idl::mcp_get_content_list_from_part_params& in,
                                                 idl::mcp_get_content_list_from_part_response& out)
{
    UB_LOG_TRACE( "get_content_list_from_part start" );
    int count = 0;
    int res = 0;
    string column_id( in.part_id() );
    string content_type(in.content_type_id());
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    uint32_t for_adapt( 0 );

    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;

    ContentColumn list( content_type, for_adapt, column_id );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    res = list.get_content_list(count,content_info_list);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i ){
        set_get_content_list_from_part_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_from_part end" );
    return 0;
}


void set_get_content_list_from_phone_result( idl::mcp_get_content_list_from_phone_response& out,
                                               const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->charge_value.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->class_name.c_str() );
}

int McpServlet::get_content_list_from_phone(const idl::mcp_get_content_list_from_phone_params& in,
                                                  idl::mcp_get_content_list_from_phone_response& out)
{
    UB_LOG_TRACE( "get_content_list_from_phone start" );
    string p_id( in.p_id() );
    uint32_t priority( in.priority() );
    //uint32_t type( in.type() );   //just for theme now
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );

    int res(0), count(0);
    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;

    ContentList list( "5", 1 );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( 1, p_id );
    list.set_priority( priority );
    res = list.get_content_list(count,content_info_list);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i ){
        set_get_content_list_from_phone_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_from_phone end" );
    return 0;

}

void set_get_content_list_result( idl::mcp_get_content_list_response& out,
                                  const uint32_t i, vector<content_info_t>::const_iterator iter )
{
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
}

int McpServlet::get_content_list(const idl::mcp_get_content_list_params& in,
                                       idl::mcp_get_content_list_response& out)
{
    UB_LOG_TRACE( "get_content_list start" );
    uint32_t priority = in.priority();
    string info_id( in.info_id() );
    uint32_t info_type = in.info_type();
    uint32_t unit_num = in.unit_num();
    uint32_t unit_times = in.unit_times();

    int res(0), count(0);
    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;

    ContentList list( "1", 0 );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    list.set_priority( priority );
    res = list.get_content_list(count,content_info_list);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i ){
        set_get_content_list_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list end" );
    return 0;
}

void set_get_content_list_from_typeID_result( idl::mcp_get_content_list_from_typeID_response& out,
                                  const uint32_t i, vector<content_info_t>::const_iterator iter )
{   
    out.m_result_params()->set_content_list( i, 0, iter->id.c_str() );
    out.m_result_params()->set_content_list( i, 1, iter->name.c_str() );
    out.m_result_params()->set_content_list( i, 2, iter->class_id.c_str() );
    out.m_result_params()->set_content_list( i, 3, iter->class_name.c_str() );
    out.m_result_params()->set_content_list( i, 4, iter->code.c_str() );
    out.m_result_params()->set_content_list( i, 5, iter->img_url.c_str() );
    out.m_result_params()->set_content_list( i, 6, iter->down_id.c_str() );
    out.m_result_params()->set_content_list( i, 7, iter->down_url.c_str() );
    out.m_result_params()->set_content_list( i, 8, iter->charge_id.c_str() );
    out.m_result_params()->set_content_list( i, 9, iter->charge_name.c_str() );
}

int McpServlet::get_content_list_from_typeID(const idl::mcp_get_content_list_from_typeID_params& in,
                                                   idl::mcp_get_content_list_from_typeID_response& out)
{
    UB_LOG_TRACE( "get_content_list_from_typeID start" );
    const string content_type( in.content_type_id() );
    const uint32_t priority( in.priority() );
    const string info_id( in.info_id() );
    const uint32_t info_type( in.info_type() );
    const uint32_t unit_num( in.unit_num() );
    const uint32_t unit_times( in.unit_times() );

    int res(0), count(0);
    vector<content_info_t> content_info_list;
    vector<content_info_t>::const_iterator iter;

    ContentList list( content_type, 0 );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    list.set_priority( priority );
    res = list.get_content_list(count,content_info_list);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    for( int i = 0; iter != content_info_list.end(); ++iter, ++i ){
        set_get_content_list_from_typeID_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_from_typeID end" );
    return 0;
}

int McpServlet::get_content_class_name(const idl::mcp_get_content_class_name_params& in,
                                             idl::mcp_get_content_class_name_response& out)
{
    UB_LOG_TRACE( "get_content_class_name start" );

    const string class_id( in.class_id() );
    class_info_t class_info;
    int res(0);
    res = mcp_content_map::get_class_info( class_id, class_info );
    if( res!=0 ){
        UB_LOG_FATAL( "get_content_class_name failed, class_id[%s], [%s:%d]", class_id.c_str(), __FILE__, __LINE__ );
    }

    out.m_result_params()->set_class_name( class_info.name.c_str() );
    UB_LOG_TRACE( "get_content_class_name end" );
    return 0;
}

int McpServlet::get_content_diy_col_name(const idl::mcp_get_content_diy_col_name_params& in,
                                               idl::mcp_get_content_diy_col_name_response& out)
{   
    UB_LOG_TRACE( "get_content_diy_col_name start" );
    
    const string column_id( in.diycol_id() );
    column_info_t column_info;
    int res(0);
    res = mcp_content_map::get_column_info( column_id, column_info );
    if( res!=0 ){
        UB_LOG_FATAL( "get_content_column_name failed, column_id[%s], [%s:%d]", column_id.c_str(), __FILE__, __LINE__ );
    }
    
    out.m_result_params()->set_diycol_name( column_info.name.c_str() );
    UB_LOG_TRACE( "get_content_diy_col_name end" );
    return 0;
}

int McpServlet::get_label_name_by_id(const idl::mcp_get_label_name_by_id_params& in, 
                                           idl::mcp_get_label_name_by_id_response& out)
{
    UB_LOG_TRACE( "get_label_name_by_id start" );

    const string label_id( in.label_id() );
    string label_name;
    int res(0);
    res = mcp_content_map::get_label_name( label_id, label_name );
    if( res!=0 ){
        UB_LOG_FATAL( "get_label_name failed, label_id[%s], [%s:%d]", label_id.c_str(), __FILE__, __LINE__ );
    }

    out.m_result_params()->set_label_name( label_name.c_str() );
    UB_LOG_TRACE( "get_label_name_by_id end" );
    return 0;
}


void set_get_content_info_result( idl::mcp_get_content_info_response& out,
                                  const content_detail_t& content_info )
{
    uint32_t i=0;
    string class_id, class_name;
    out.m_result_params()->set_info_list( 0, content_info.id.c_str());
    out.m_result_params()->set_info_list( 1, content_info.name.c_str());
    for(i=0;i<content_info.class_info.size();++i){
        if(i!=0){
            class_id.append(",");
            class_name.append(",");
        }
        class_id.append(content_info.class_info[i].id);
        class_name.append(content_info.class_info[i].name);
    }
    out.m_result_params()->set_info_list( 2, class_id.c_str());
    out.m_result_params()->set_info_list( 3, class_name.c_str());
    for(i=0;i<content_info.img_info.size();++i){
        if(content_info.img_info[i].type=="3"){
            out.m_result_params()->set_info_list( 4, content_info.img_info[i].url.c_str()); 
            break;
        }
    }
    out.m_result_params()->set_info_list( 5, content_info.ext_id.c_str());
    out.m_result_params()->set_info_list( 6, content_info.abstract.c_str());
    out.m_result_params()->set_info_list( 7, content_info.version.c_str());
    out.m_result_params()->set_info_list( 8, content_info.method.c_str());
    out.m_result_params()->set_info_list( 9, content_info.star.c_str());
    out.m_result_params()->set_info_list( 10, content_info.network.c_str());
    out.m_result_params()->set_info_list( 11, content_info.code.c_str());
    out.m_result_params()->set_info_list( 12, content_info.cp_info.id.c_str());
    out.m_result_params()->set_info_list( 13, content_info.cp_info.name.c_str());
    out.m_result_params()->set_info_list( 14, content_info.grant_info.id.c_str());
    out.m_result_params()->set_info_list( 15, content_info.grant_info.grant_time.c_str());
    out.m_result_params()->set_info_list( 16, content_info.grant_info.valid_time.c_str());
    out.m_result_params()->set_info_list( 17, content_info.log_date.c_str());
    out.m_result_params()->set_info_list( 18, content_info.mod_date.c_str());
    out.m_result_params()->set_info_list( 19, content_info.dft_charge_type.c_str());
    out.m_result_params()->set_info_list( 20, content_info.dft_charge_val.c_str());
    string download_id, download_url, charge_type;
    for(i=0;i<content_info.download_info.size();++i){
        if(i!=0){
            download_id.append(",");
            download_url.append(",");
            charge_type.append(",");
        }
        download_id.append(content_info.download_info[i].id.c_str());
        download_url.append(content_info.download_info[i].url.c_str());
        charge_type.append(content_info.download_info[i].charge_id.c_str());
    }
    out.m_result_params()->set_info_list( 21, download_id.c_str());
    out.m_result_params()->set_info_list( 22, download_url.c_str());
    out.m_result_params()->set_info_list( 23, charge_type.c_str());
    out.m_result_params()->set_info_list( 24, content_info.enName.c_str());
    out.m_result_params()->set_info_list( 25, content_info.language.c_str());
    out.m_result_params()->set_info_list( 26, content_info.adapt.c_str());
    out.m_result_params()->set_info_list( 27, content_info.package_name.c_str());
	if( content_info.version_appinfo.empty() )
	{
    	out.m_result_params()->set_info_list(28, content_info.version.c_str() );
	}
	else
	{
    	out.m_result_params()->set_info_list(28, content_info.version_appinfo.c_str() );
	}
    out.m_result_params()->set_info_list( 29, content_info.sdk_version.c_str());
    out.m_result_params()->set_info_list( 30, content_info.version_code.c_str());
    out.m_result_params()->set_info_list( 31, content_info.ptype_0.c_str());
    out.m_result_params()->set_info_list( 32, content_info.ptype_1.c_str());
    out.m_result_params()->set_info_list( 33, content_info.ptype_2.c_str());
    out.m_result_params()->set_info_list( 34, content_info.ptype_3.c_str());
    out.m_result_params()->set_info_list( 35, content_info.ptype_4.c_str());

    // [s] add by gwj for mcp_content_pub_prop
    out.m_result_params()->set_info_list( 36, content_info.pub_prop_ids.c_str());
    // [e] add by gwj for mcp_content_pub_prop

}

template <typename T>
void set_get_content_info_result1( T& out,
                                  const content_detail_t& content_info )
{
    uint32_t i=0;
    string class_id, class_name;
    out.m_result_params()->set_info_list( 0, content_info.id.c_str());
    out.m_result_params()->set_info_list( 1, content_info.name.c_str());
    for(i=0;i<content_info.class_info.size();++i){
        if(i!=0){
            class_id.append(",");
            class_name.append(",");
        }
        class_id.append(content_info.class_info[i].id);
        class_name.append(content_info.class_info[i].name);
    }
    out.m_result_params()->set_info_list( 2, class_id.c_str());
    out.m_result_params()->set_info_list( 3, class_name.c_str());
    for(i=0;i<content_info.img_info.size();++i){
        if(content_info.img_info[i].type=="3"){
            out.m_result_params()->set_info_list( 4, content_info.img_info[i].url.c_str()); 
            break;
        }
    }
    out.m_result_params()->set_info_list( 5, content_info.ext_id.c_str());
    out.m_result_params()->set_info_list( 6, content_info.abstract.c_str());
    out.m_result_params()->set_info_list( 7, content_info.version.c_str());
    out.m_result_params()->set_info_list( 8, content_info.method.c_str());
    out.m_result_params()->set_info_list( 9, content_info.star.c_str());
    out.m_result_params()->set_info_list( 10, content_info.network.c_str());
    out.m_result_params()->set_info_list( 11, content_info.code.c_str());
    out.m_result_params()->set_info_list( 12, content_info.cp_info.id.c_str());
    out.m_result_params()->set_info_list( 13, content_info.cp_info.name.c_str());
    out.m_result_params()->set_info_list( 14, content_info.grant_info.id.c_str());
    out.m_result_params()->set_info_list( 15, content_info.grant_info.grant_time.c_str());
    out.m_result_params()->set_info_list( 16, content_info.grant_info.valid_time.c_str());
    out.m_result_params()->set_info_list( 17, content_info.log_date.c_str());
    out.m_result_params()->set_info_list( 18, content_info.mod_date.c_str());
    out.m_result_params()->set_info_list( 19, content_info.dft_charge_type.c_str());
    out.m_result_params()->set_info_list( 20, content_info.dft_charge_val.c_str());
    string download_id, download_url, charge_type;
    for(i=0;i<content_info.download_info.size();++i){
        if(i!=0){
            download_id.append(",");
            download_url.append(",");
            charge_type.append(",");
        }
        download_id.append(content_info.download_info[i].id.c_str());
        download_url.append(content_info.download_info[i].url.c_str());
        charge_type.append(content_info.download_info[i].charge_id.c_str());
    }
    out.m_result_params()->set_info_list( 21, download_id.c_str());
    out.m_result_params()->set_info_list( 22, download_url.c_str());
    out.m_result_params()->set_info_list( 23, charge_type.c_str());
    out.m_result_params()->set_info_list( 24, content_info.enName.c_str());
    out.m_result_params()->set_info_list( 25, content_info.language.c_str());
    out.m_result_params()->set_info_list( 26, content_info.adapt.c_str());
    out.m_result_params()->set_info_list( 27, content_info.package_name.c_str());
	if( content_info.version_appinfo.empty() )
	{
    	out.m_result_params()->set_info_list(28, content_info.version.c_str() );
	}
	else
	{
    	out.m_result_params()->set_info_list(28, content_info.version_appinfo.c_str() );
	}
    out.m_result_params()->set_info_list( 29, content_info.sdk_version.c_str());
    out.m_result_params()->set_info_list( 30, content_info.version_code.c_str());
    out.m_result_params()->set_info_list( 31, content_info.ptype_0.c_str());
    out.m_result_params()->set_info_list( 32, content_info.ptype_1.c_str());
    out.m_result_params()->set_info_list( 33, content_info.ptype_2.c_str());
    out.m_result_params()->set_info_list( 34, content_info.ptype_3.c_str());
    out.m_result_params()->set_info_list( 35, content_info.ptype_4.c_str());

    // [s] add by gwj for mcp_content_pub_prop
    out.m_result_params()->set_info_list( 36, content_info.pub_prop_ids.c_str());
    // [e] add by gwj for mcp_content_pub_prop
	return;
}



void get_cid_by_packagename_and_versioncode(string &packagename, string &versioncode, string& cid)
{
	GamePackageVersion gpv(packagename, versioncode);
	gpv.get_cid(cid,true);
	return;
}

void get_cid_by_packagename(string &packagename, string& cid)
{
	GamePackageVersion gpv(packagename);
	gpv.get_cid(cid,false);
	return;
}

int get_content_info_by_cid_infoid_infotype(string &cid, string &info_id, uint32_t info_type, content_detail_t& detail)
{
	int res(0);
	UB_LOG_FATAL("cid is %s, infoid is %s, info_type is %d ", cid.c_str(), info_id.c_str(), info_type);
	UB_LOG_TRACE("cid is %s, infoid is %s, info_type is %d ", cid.c_str(), info_id.c_str(), info_type);
	
	ContentInfo c_info(cid);
	c_info.set_adapt_info(info_type, info_id);
	res=c_info.get_content_detail(detail);
	if(res!=0){
		UB_LOG_FATAL("get_content_detail failed id[%s], [%s:%d]", cid.c_str(), __FILE__, __LINE__ );
		goto end;
	}

end:
	//set_get_content_info_result(out,detail);
	return res;
}

int McpServlet::get_content_info(const idl::mcp_get_content_info_params& in,
                                       idl::mcp_get_content_info_response& out)
{
    UB_LOG_TRACE( "get_content_info start" );
    string content_id = in.content_id();
    string info_id=in.info_id();
    uint32_t info_type = in.info_type();
    content_detail_t detail;
	int res = get_content_info_by_cid_infoid_infotype(content_id, info_id, info_type, detail);
	
    if(res!=0){
        UB_LOG_FATAL("get_content_detail failed id[%s], [%s:%d]", content_id.c_str(), __FILE__, __LINE__ );
        goto end;
    }
/*
    int res(0);
    content_detail_t detail;
    c_info.set_adapt_info(info_type, info_id);
    res=c_info.get_content_detail(detail);
    if(res!=0){
        UB_LOG_FATAL("get_content_detail failed id[%s], [%s:%d]", content_id.c_str(), __FILE__, __LINE__ );
        goto end;
    }
            
*/
end:
    set_get_content_info_result(out,detail);
    UB_LOG_TRACE( "get_content_info end" );
    return 0;
}

void set_get_soft_info_by_id_result( idl::mcp_get_soft_info_by_id_response& out, const soft_info_t& soft_info)
{
    string classid_name;
    vector<class_info_t>::iterator iter;
    vector<class_info_t> classid_list;
    classid_list = soft_info.class_info_list;

    out.m_result_params()->set_soft_info(0,soft_info.id.c_str());
    out.m_result_params()->set_soft_info(1,soft_info.name.c_str());
    out.m_result_params()->set_soft_info(2,soft_info.icon.c_str());
    out.m_result_params()->set_soft_info(3,soft_info.preview.c_str());
    out.m_result_params()->set_soft_info(4,soft_info.version.c_str());
    out.m_result_params()->set_soft_info(5,soft_info.size.c_str());
    out.m_result_params()->set_soft_info(6,soft_info.pop.c_str());
    out.m_result_params()->set_soft_info(7,soft_info.adapt.c_str());
    out.m_result_params()->set_soft_info(8,soft_info.date.c_str());
    out.m_result_params()->set_soft_info(9,soft_info.froms.c_str());
    out.m_result_params()->set_soft_info(10,soft_info.info.c_str());
    out.m_result_params()->set_soft_info(11,soft_info.down_url.c_str());
    for(iter = classid_list.begin(); iter != classid_list.end(); ++iter){
        classid_name.append(iter->id);
        classid_name.append(",");
        classid_name.append(iter->name);
        classid_name.append(";");
    }
    out.m_result_params()->set_soft_info(12,classid_name.c_str());
    out.m_result_params()->set_soft_info(13,soft_info.jump_url.c_str());
    out.m_result_params()->set_soft_info(14,soft_info.star.c_str());
    out.m_result_params()->set_soft_info(15,soft_info.show_highspeed.c_str());
}

int McpServlet::get_soft_info_by_id(const idl::mcp_get_soft_info_by_id_params& in, 
                idl::mcp_get_soft_info_by_id_response& out)
{
        UB_LOG_TRACE( "get_soft_info_by_id start" );
        string soft_id(in.soft_id());
        int res(0);

        soft_info_t soft_info;
        SoftInfo soft(soft_id);
        res=soft.get_soft_info(soft_info);
        if(res!=0){
                UB_LOG_FATAL("get_soft_info failed, id[%s], [%s:%d]", soft_id.c_str(), __FILE__, __LINE__ );
                goto end;
        }
end:
        out.m_result_params()->set_result(res);
        set_get_soft_info_by_id_result(out,soft_info);
        UB_LOG_TRACE( "get_soft_info_by_id end" );
        return 0;
}

int McpServlet::get_col_list_by_topic(const idl::mcp_get_col_list_by_topic_params& in, idl::mcp_get_col_list_by_topic_response& out)
{
        UB_LOG_TRACE( "get_col_list_by_topic start");
        string topic_id(in.topic_id());
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());

        int res(0),count(0);
        ContentTopic topic(topic_id);
        topic_info_t topic_info,before_topic, next_topic;
        topic.get_topic_info(topic_info);
        if(!topic_info.before.empty()&&topic_info.before.compare("")!=0){
                topic.get_topic_info(topic_info.before,before_topic);
        }
        if(!topic_info.next.empty()&&topic_info.next.compare("")!=0){
                topic.get_topic_info(topic_info.next,next_topic);
        }
        vector<column_info_t> column_list;
        topic.set_page_info(page,page_size);
        res = topic.get_column_list(count,column_list);
        if(res!=0){
                UB_LOG_FATAL("get_column_list failed, topic_id[%s], [%s:%d]", topic_id.c_str(), __FILE__, __LINE__ );
                goto end;
        }
end:
        out.m_result_params()->set_count(count);
        out.m_result_params()->set_topic_name(topic_info.name.c_str());
        out.m_result_params()->set_topic_img(topic_info.img.c_str());
        out.m_result_params()->set_topic_intro(topic_info.intro.c_str());
        out.m_result_params()->set_log_date(topic_info.log_date.c_str());
        out.m_result_params()->set_before_topic(before_topic.id.c_str());
        out.m_result_params()->set_before_name(before_topic.name.c_str());
        out.m_result_params()->set_next_topic(next_topic.id.c_str());
        out.m_result_params()->set_next_name(next_topic.name.c_str());
        vector<column_info_t>::iterator it = column_list.begin();
        for(int i=0; it!=column_list.end();++i,++it){
                out.m_result_params()->set_col_id_list(i,it->id.c_str());
        }
        UB_LOG_TRACE( "get_col_list_by_topic end");
        return 0;
}

void set_get_soft_list_by_col_id_result( idl::mcp_get_soft_list_by_col_id_response& out,
                const uint32_t i, vector<soft_info_t>::const_iterator iter )
{
        string classid_name;
        vector<class_info_t>::iterator class_info_iter;
        vector<class_info_t> classid_list;
        classid_list = iter->class_info_list;

        out.m_result_params()->set_soft_list(i,0,iter->id.c_str());
        out.m_result_params()->set_soft_list(i,1,iter->name.c_str());
        out.m_result_params()->set_soft_list(i,2,iter->icon.c_str());
        out.m_result_params()->set_soft_list(i,3,iter->preview.c_str());
        out.m_result_params()->set_soft_list(i,4,iter->version.c_str());
        out.m_result_params()->set_soft_list(i,5,iter->size.c_str());
        out.m_result_params()->set_soft_list(i,6,iter->pop.c_str());
        out.m_result_params()->set_soft_list(i,7,iter->adapt.c_str());
        out.m_result_params()->set_soft_list(i,8,iter->date.c_str());
        out.m_result_params()->set_soft_list(i,9,iter->froms.c_str());
        out.m_result_params()->set_soft_list(i,10,iter->info.c_str());
        out.m_result_params()->set_soft_list(i,11,iter->down_url.c_str());

        for(class_info_iter = classid_list.begin(); class_info_iter != classid_list.end(); ++class_info_iter){
                classid_name.append(class_info_iter->id);
                classid_name.append(",");
                classid_name.append(class_info_iter->name);
                classid_name.append(";");
        }
        out.m_result_params()->set_soft_list(i,12,classid_name.c_str());
        out.m_result_params()->set_soft_list(i,13,iter->jump_url.c_str());
        if(!iter->star.empty())
        {    
                out.m_result_params()->set_soft_list(i,14,iter->star.c_str());
        }  
        out.m_result_params()->set_soft_list(i,15,iter->show_highspeed.c_str());

}

int  McpServlet::get_soft_list_by_col_id(const idl::mcp_get_soft_list_by_col_id_params& in, 
                idl::mcp_get_soft_list_by_col_id_response& out)
{
        UB_LOG_TRACE("get_soft_list_by_col_id start");
        int count = 0;
        int res = 0;
        string col_id( in.col_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );

        vector<soft_info_t> soft_list;
        vector<soft_info_t>::const_iterator iter;

        ContentColumn list( content_type, 1, col_id );
        column_info_t column_info;
        res=mcp_content_map::get_column_info(col_id, column_info);
        if( res!=0 ){
                count = -5;
                goto end;
        }

        list.set_unit_info( unit_num, unit_times );
        res = list.get_soft_list(count,soft_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_soft_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count( count );
        out.m_result_params()->set_col_name( column_info.name.c_str() );
        iter = soft_list.begin();
        for( int i = 0; iter != soft_list.end(); ++iter, ++i ){
                set_get_soft_list_by_col_id_result( out, i, iter );
        }
        UB_LOG_TRACE("get_soft_list_by_col_id end");
        return 0;
}

        template <typename T>
void set_get_soft_list_by_class_id_result( T& out,
                const uint32_t i, vector<soft_info_t>::const_iterator iter )
{
        string classid_name;
        vector<class_info_t>::iterator iter2;
        vector<class_info_t> classid_list;
        classid_list = iter->class_info_list;

        out.m_result_params()->set_soft_list(i,0,iter->id.c_str());
        out.m_result_params()->set_soft_list(i,1,iter->name.c_str());
        out.m_result_params()->set_soft_list(i,2,iter->icon.c_str());
        out.m_result_params()->set_soft_list(i,3,iter->preview.c_str());
        out.m_result_params()->set_soft_list(i,4,iter->version.c_str());
        out.m_result_params()->set_soft_list(i,5,iter->size.c_str());
        out.m_result_params()->set_soft_list(i,6,iter->pop.c_str());
        out.m_result_params()->set_soft_list(i,7,iter->adapt.c_str());
        out.m_result_params()->set_soft_list(i,8,iter->date.c_str());
        out.m_result_params()->set_soft_list(i,9,iter->froms.c_str());
        out.m_result_params()->set_soft_list(i,10,iter->info.c_str());
        out.m_result_params()->set_soft_list(i,11,iter->down_url.c_str());
        for(iter2 = classid_list.begin(); iter2 != classid_list.end(); ++iter2){
            classid_name.append(iter2->id);
            classid_name.append(",");
            classid_name.append(iter2->name);
            classid_name.append(";");
        }    
        out.m_result_params()->set_soft_list(i,12,classid_name.c_str());
        out.m_result_params()->set_soft_list(i,13,iter->jump_url.c_str());
        if(!iter->star.empty())
        {
                out.m_result_params()->set_soft_list(i,14,iter->star.c_str());
        }
        out.m_result_params()->set_soft_list(i,15,iter->show_highspeed.c_str());
}

int McpServlet::get_soft_list_by_class_id(const idl::mcp_get_soft_list_by_class_id_params& in, 
                idl::mcp_get_soft_list_by_class_id_response& out)
{
        UB_LOG_TRACE("get_soft_list_by_class_id start");
        int count = 0;
        int res = 0;
        string class_id( in.class_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t orderby( in.orderby() );

        vector<soft_info_t> soft_list;
        vector<soft_info_t>::const_iterator iter;

        ContentClass list( content_type, 1, class_id );
        class_info_t class_info;
        res=mcp_content_map::get_class_info(class_id, class_info);
        if( res!=0 ){
                count = -5;
                goto end;
        }

        list.set_unit_info( unit_num, unit_times );
        list.set_priority( orderby );
        res = list.get_soft_list(count,soft_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_soft_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count( count );
        out.m_result_params()->set_class_name( class_info.name.c_str() );
        out.m_result_params()->set_class_icon( class_info.icon.c_str() );
        iter = soft_list.begin();
        for( int i = 0; iter != soft_list.end(); ++iter, ++i )
        {
                set_get_soft_list_by_class_id_result( out, i, iter );
        }
        UB_LOG_TRACE("get_soft_list_by_class_id end");
        return 0;
}

int McpServlet::mcp_content_common_init(const idl::mcp_mcp_content_common_init_params& in, 
                idl::mcp_mcp_content_common_init_response& out )
{
        UB_LOG_TRACE( "mcp_content_common_init start" );
        int res(0);
        (void)in;
        res = mcp_content_map::init();
        out.m_result_params()->set_result(res);
        UB_LOG_TRACE( "mcp_content_common_init end" );
        return 0;
}

void set_get_topic_list_by_type_result( idl::mcp_get_topic_list_by_type_response& out,
                const uint32_t i, vector<topic_info_t>::const_iterator iter )
{
        out.m_result_params()->set_topic_list(i,0,iter->id.c_str());
        out.m_result_params()->set_topic_list(i,1,iter->name.c_str());
        out.m_result_params()->set_topic_list(i,2,iter->img.c_str());
        out.m_result_params()->set_topic_list(i,3,iter->intro.c_str());
}

int McpServlet::get_topic_list_by_type(const idl::mcp_get_topic_list_by_type_params& in, 
                idl::mcp_get_topic_list_by_type_response& out)
{
        UB_LOG_TRACE( "get_topic_list_by_type start" );
        int count = 0;
        int res = 0;
        string type_id( in.type_id());
        uint32_t page_size( in.page_size());
        uint32_t page( in.page() );

        vector<topic_info_t> topic_list;
        vector<topic_info_t>::const_iterator iter;

        ContentType type( type_id );
        type.set_page_info( page, page_size );
        res = type.getTopicList(count,topic_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_soft_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count( count );
        iter = topic_list.begin();
        for( int i = 0; iter != topic_list.end(); ++iter, ++i ){
                set_get_topic_list_by_type_result( out, i, iter );
        }
        UB_LOG_TRACE( "get_topic_list_by_type end" );
        return 0;
}

void set_get_android_theme_by_col_id_result( idl::mcp_get_android_theme_by_col_id_response& out,
                const uint32_t i, vector<android_theme_t>::const_iterator iter )
{   
        string classid_name;
        vector<class_info_t>::iterator class_info_iter;
        vector<class_info_t> classid_list;
        classid_list = iter->class_info_list;

        out.m_result_params()->set_theme_list(i,0,iter->id.c_str());
        out.m_result_params()->set_theme_list(i,1,iter->name.c_str());
        out.m_result_params()->set_theme_list(i,2,iter->icon.c_str());
        out.m_result_params()->set_theme_list(i,3,iter->preview.c_str());
        out.m_result_params()->set_theme_list(i,4,iter->size.c_str());
        out.m_result_params()->set_theme_list(i,5,iter->pop.c_str());
        out.m_result_params()->set_theme_list(i,6,iter->adapt.c_str());
        out.m_result_params()->set_theme_list(i,7,iter->date.c_str()); 
        out.m_result_params()->set_theme_list(i,8,iter->froms.c_str());
        out.m_result_params()->set_theme_list(i,9,iter->info.c_str());
        out.m_result_params()->set_theme_list(i,10,iter->down_url.c_str());

        for(class_info_iter = classid_list.begin(); class_info_iter != classid_list.end(); ++class_info_iter){
                classid_name.append(class_info_iter->id);
                classid_name.append(",");
                classid_name.append(class_info_iter->name);
                classid_name.append(";");
        }
        out.m_result_params()->set_theme_list(i,11,classid_name.c_str());
} 

int McpServlet::get_android_theme_by_col_id(const idl::mcp_get_android_theme_by_col_id_params& in, 
                idl::mcp_get_android_theme_by_col_id_response& out)
{
        UB_LOG_TRACE("get_android_theme_by_col_id start");
        UB_LOG_DEBUG("*************************************");
        int count = 0;
        int res = 0;
        string col_id( in.col_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );

        vector<android_theme_t> theme_list;
        vector<android_theme_t>::const_iterator iter;

        ContentColumn list( content_type, 1, col_id );
        column_info_t column_info;
        res=mcp_content_map::get_column_info(col_id, column_info);
        if( res!=0 ){
                count = -5;
                goto end;
        }

        list.set_unit_info( unit_num, unit_times );
        res = list.get_android_theme_list(count,theme_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_android_theme_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count( count );
        out.m_result_params()->set_col_name( column_info.name.c_str() );
        iter = theme_list.begin();
        for( int i = 0; iter != theme_list.end(); ++iter, ++i ){
                set_get_android_theme_by_col_id_result( out, i, iter );
        }
        UB_LOG_TRACE("get_android_theme_by_col_id end");
        return 0;
}

void set_get_android_theme_by_class_id_result( idl::mcp_get_android_theme_by_class_id_response& out,
                const uint32_t i, vector<android_theme_t>::const_iterator iter )
{
        out.m_result_params()->set_theme_list(i,0,iter->id.c_str());
        out.m_result_params()->set_theme_list(i,1,iter->name.c_str());
        out.m_result_params()->set_theme_list(i,2,iter->icon.c_str());
        out.m_result_params()->set_theme_list(i,3,iter->preview.c_str());
        out.m_result_params()->set_theme_list(i,4,iter->size.c_str());
        out.m_result_params()->set_theme_list(i,5,iter->pop.c_str());
        out.m_result_params()->set_theme_list(i,6,iter->adapt.c_str());
        out.m_result_params()->set_theme_list(i,7,iter->date.c_str());
        out.m_result_params()->set_theme_list(i,8,iter->froms.c_str());
        out.m_result_params()->set_theme_list(i,9,iter->info.c_str());
        out.m_result_params()->set_theme_list(i,10,iter->down_url.c_str());
}

int McpServlet::get_android_theme_by_class_id(const idl::mcp_get_android_theme_by_class_id_params& in, 
                idl::mcp_get_android_theme_by_class_id_response& out)
{
        UB_LOG_TRACE("get_android_theme_by_class_id start");
        int count = 0;
        int res = 0;
        string class_id( in.class_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t orderby( in.orderby() );

        vector<android_theme_t> theme_list;
        vector<android_theme_t>::const_iterator iter;

        ContentClass list( content_type, 1, class_id );
        class_info_t class_info;
        res=mcp_content_map::get_class_info(class_id, class_info);
        if( res!=0 ){
                count = -5;
                goto end;
        }

        list.set_unit_info( unit_num, unit_times ); 
        list.set_priority( orderby );
        res = list.get_android_theme_list(count,theme_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_android_theme_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count( count );
        out.m_result_params()->set_class_name( class_info.name.c_str() );
        out.m_result_params()->set_class_icon( class_info.icon.c_str() );
        iter = theme_list.begin();
        for( int i = 0; iter != theme_list.end(); ++iter, ++i ){
                set_get_android_theme_by_class_id_result( out, i, iter );
        }
        UB_LOG_TRACE("get_android_theme_by_class_id end");
        return 0;
}


void set_get_android_theme_by_id_result( idl::mcp_get_android_theme_by_id_response& out, const android_theme_t& theme)
{
        string classid_name;
        vector<class_info_t>::iterator iter;
        vector<class_info_t> classid_list;
        out.m_result_params()->set_info(0,theme.id.c_str());
        out.m_result_params()->set_info(1,theme.name.c_str());
        out.m_result_params()->set_info(2,theme.icon.c_str());
        out.m_result_params()->set_info(3,theme.preview.c_str());
        out.m_result_params()->set_info(4,theme.size.c_str());
        out.m_result_params()->set_info(5,theme.pop.c_str());
        out.m_result_params()->set_info(6,theme.adapt.c_str());
        out.m_result_params()->set_info(7,theme.date.c_str());
        out.m_result_params()->set_info(8,theme.froms.c_str());
        out.m_result_params()->set_info(9,theme.info.c_str());
        out.m_result_params()->set_info(10,theme.down_url.c_str());
        classid_list = theme.class_info_list;
        for(iter = classid_list.begin(); iter != classid_list.end(); ++iter){
                classid_name.append(iter->id);
                classid_name.append(",");
                classid_name.append(iter->name);
                classid_name.append(";");
        }
        out.m_result_params()->set_info(11,classid_name.c_str());
        out.m_result_params()->set_info(12,theme.jump_url.c_str());
}

int McpServlet::get_android_theme_by_id(const idl::mcp_get_android_theme_by_id_params& in, 
                idl::mcp_get_android_theme_by_id_response& out)
{
        UB_LOG_TRACE( "get_android_theme_by_id start" );
        string id(in.id());
        int res(0);

        android_theme_t theme_info;
        AndroidTheme theme(id);
        res=theme.get_info(theme_info);
        if(res!=0){
                UB_LOG_FATAL("get_android_theme failed, id[%s], [%s:%d]", id.c_str(), __FILE__, __LINE__ );
                goto end;
        }
end:
        out.m_result_params()->set_result(res);
        set_get_android_theme_by_id_result(out,theme_info);
        UB_LOG_TRACE( "get_android_theme_by_id end" );
        return 0;
}

int McpServlet::get_sub_class_by_class(const idl::mcp_get_sub_class_by_class_params& in, 
                idl::mcp_get_sub_class_by_class_response& out)
{
        UB_LOG_TRACE( "get_sub_class_by_class start" );
        (void)in;
        out.m_result_params()->set_cooooooount(100);
        out.m_result_params()->mutable_sub_list(0)->set_id("101");
        out.m_result_params()->mutable_sub_list(0)->set_name("li-yong");
        UB_LOG_TRACE( "get_sub_class_by_class end" );
        return 0;
}

void set_get_download_info_result( idl::mcp_get_download_info_response& out, 
                const uint32_t i, vector<download_detail_t>::const_iterator it )
{
        out.m_result_params()->set_download_info(i,0,it->id.c_str());
        out.m_result_params()->set_download_info(i,1,it->url.c_str());
        out.m_result_params()->set_download_info(i,2,it->url_type.c_str());
        out.m_result_params()->set_download_info(i,3,it->media_format.c_str());
        out.m_result_params()->set_download_info(i,4,it->size.c_str());
        out.m_result_params()->set_download_info(i,5,it->mtype.c_str());
        out.m_result_params()->set_download_info(i,6,it->charge_id.c_str());
        out.m_result_params()->set_download_info(i,7,it->charge_value.c_str());
        out.m_result_params()->set_download_info(i,8,it->apk_flag.c_str());
}

int McpServlet::get_download_info(const idl::mcp_get_download_info_params& in,
                idl::mcp_get_download_info_response& out)
{
        UB_LOG_TRACE( "get_download_info start" );
        string cid(in.content_id());
        string info_id(in.info_id());
        uint32_t info_type(in.info_type());

        int res(0);
        vector<download_detail_t> download_list;
        ContentInfo c_info(cid);
        c_info.set_adapt_info(info_type, info_id);
        res=c_info.get_download_info(download_list);
        if(res!=0){
                UB_LOG_FATAL("get_download_info failed id[%s], [%s:%d]", cid.c_str(), __FILE__, __LINE__ );
                goto end;
        }
		UB_LOG_DEBUG("download_list size[%ld]", download_list.size());

end:
        vector<download_detail_t>::const_iterator iter = download_list.begin();
        for( uint32_t i = 0; iter != download_list.end(); ++iter, ++i ){
                set_get_download_info_result( out, i, iter );
        }
        UB_LOG_TRACE( "get_download_info end" );
        return 0;
}

int McpServlet::get_content_img(const idl::mcp_get_content_img_params& in, idl::mcp_get_content_img_response& out)
{
        UB_LOG_TRACE( "get_content_img start" );
        string cid(in.content_id());
        string img_type;
        stringstream ss;
        ss<<in.img_type();
        ss>>img_type;

        int res(0);
        content_detail_t content_detail;
        ContentInfo c_info(cid);
        res=c_info.get_basic_info(content_detail);
        if(res!=0){
                UB_LOG_FATAL("get_basic_info failed id[%s], [%s:%d]", cid.c_str(), __FILE__, __LINE__ );
                goto end;
        }
end:
        int i = 0;
        for(vector<img_info_t>::const_iterator it=content_detail.img_info.begin(); it!=content_detail.img_info.end();++it){
                if(it->type==img_type){
                        out.m_result_params()->set_img_url(i, it->url.c_str());
                        ++i;
                }
        }
        out.m_result_params()->set_count(i);
        UB_LOG_TRACE( "get_content_img end" );
        return 0;
}

void set_ios_content_result(idl::ios_content_info_t* out, const ios_content_t& info)
{
        out->set_id(info.id.c_str());
        out->set_name(info.name.c_str());
        out->set_icon(info.icon.c_str());
        out->set_type(info.type.c_str());
        out->set_itunes_id(info.itunes_id.c_str());
        out->set_info(info.info.c_str());
        out->set_version(info.version.c_str());
        out->set_cp_id(info.cp_id.c_str());
        string cp_name;
        mcp_content_map::get_cp_name(info.cp_id,cp_name);
        out->set_cp_name(cp_name.c_str());
        /* 2012-12-06 hongming yaoqiu xiugai */ 
        //out->set_star(toString(info.rating).c_str());
        out->set_star(toString(info.crt_rating).c_str());
        out->set_gdate(info.gdate.c_str());
        out->set_mdate(info.mdate.c_str());
        out->set_adapt(info.adapt.c_str());
        out->set_url(info.url.c_str());
        out->set_jump_url(info.jump_url.c_str());
        out->set_size(info.size.c_str());
        out->set_now_price(toString(info.value).c_str());
        out->set_original_price(toString(info.original_value).c_str());
        out->set_rating(toString(info.rating).c_str());
        out->set_count(toString(info.count).c_str());
        out->set_crt_rating(toString(info.crt_rating).c_str());
        out->set_crt_count(toString(info.crt_count).c_str());
        out->set_language(info.language.c_str());
        vector<string>::const_iterator img_it = info.img_list.begin();
        for( int j=0; img_it!=info.img_list.end(); ++img_it, ++j ){
                out->set_img_list(j,img_it->c_str());
        }
        vector<class_info_t>::const_iterator class_it = info.class_list.begin();
        for( int j=0; class_it!=info.class_list.end(); ++class_it, ++j ){
                out->mutable_class_info(j)->set_id(class_it->id.c_str());
                out->mutable_class_info(j)->set_name(class_it->name.c_str());
        }
}

void set_game_brief_result(idl::game_brief_info_t* out, const game_brief_t& game_brief)
{
        out->set_info_type(game_brief.info_type.c_str());
        out->set_info_id(game_brief.info_id.c_str());
        out->set_info_title(game_brief.info_title.c_str());
        out->set_info_datetime(game_brief.info_datetime.c_str());
        if(!strcmp(game_brief.info_type.c_str(),"1"))
        {    
                out->set_type_name("公告");
        }    
        else if(!strcmp(game_brief.info_type.c_str(),"2"))
        {    
                out->set_type_name("新闻");
        }    
        else if(!strcmp(game_brief.info_type.c_str(),"3"))
        {    
                out->set_type_name("活动");
        }    
        else if(!strcmp(game_brief.info_type.c_str(),"4"))
        {    
                out->set_type_name("攻略");
        } 

        // [s] add by gwj for news_type_name
        else if(!strcmp(game_brief.info_type.c_str(),"5"))
        {
                out->set_type_name("开服");
        }
        else if(!strcmp(game_brief.info_type.c_str(),"6"))
        {
                out->set_type_name("开测");
        }
        else if(!strcmp(game_brief.info_type.c_str(),"7"))
        {
                out->set_type_name("新服");
        }
        else if(!strcmp(game_brief.info_type.c_str(),"8"))
        {
                out->set_type_name("预告");
        }
        else if(!strcmp(game_brief.info_type.c_str(),"9"))
        {
                out->set_type_name("问答");
        }
        else if(!strcmp(game_brief.info_type.c_str(),"10"))
        {
                out->set_type_name("评测");
        }
        // [e] add by gwj for news_type_name

        out->set_game_type(game_brief.game_type.c_str());
}

void set_get_ios_content_by_id_result( idl::mcp_get_ios_content_by_id_response& out, const ios_content_t& info)
{
        set_ios_content_result(out.m_result_params()->mutable_info(), info);
        return;
}

/*
   void set_get_game_brief_result(idl::mcp_get_game_info_list_responsei& out, const game_brief_t& info)
   {
   set_game_brief_result(out.m_result_params()->mutable)
   }
   */
int McpServlet::get_ios_content_by_id(const idl::mcp_get_ios_content_by_id_params& in, idl::mcp_get_ios_content_by_id_response& out)
{
        UB_LOG_TRACE( "get_ios_content_by_id start" );
        string id(in.id());
        int res(0);

        ios_content_t info;
        IosContent content(id);
        res=content.get_info(info);
        if( res!=0 ){
                UB_LOG_FATAL( "IosContent::get_info failed, id[%s], [%s:%d]", id.c_str(), __FILE__, __LINE__ );
                goto end;
        }
end:
        out.m_result_params()->set_result( res );
        set_get_ios_content_by_id_result(out, info);
        UB_LOG_TRACE( "get_ios_content_by_id end");
        return 0;
}


        template <typename T>
void set_ios_content_result(T& out, const int i, vector<ios_content_t>::const_iterator iter)
{
        set_ios_content_result(out.m_result_params()->mutable_info_list(i),*iter);
        return;
}

void set_gett_game_brief_result(idl::mcp_get_game_info_list_response& out, const int i,vector<game_brief_t>::const_iterator iter)
{
        set_game_brief_result(out.m_result_params()->mutable_game_info_list(i), *iter);
        return;
}

void set_get_game_score_by_imei_or_uid(idl::mcp_get_game_score_by_imei_or_uid_response& out,game_score_t game_score)
{
        if(game_score.score.empty())
        {
                game_score.score = "-1";
        }
        out.m_result_params()->set_score(game_score.score.c_str());
}	

void set_get_appinfo_by_packagename(idl::mcp_get_appinfo_by_packagename_response& out, appinfo_t app_info)
{
        out.m_result_params()->set_package_info(0,app_info.id.c_str());
        out.m_result_params()->set_package_info(1,app_info.c_id.c_str());
        out.m_result_params()->set_package_info(2,app_info.app_name.c_str());
        out.m_result_params()->set_package_info(3,app_info.version_code.c_str());
        out.m_result_params()->set_package_info(4,app_info.version.c_str());
        out.m_result_params()->set_package_info(5,app_info.sdk_version.c_str());
        out.m_result_params()->set_package_info(6,app_info.permissions.c_str());
        out.m_result_params()->set_package_info(7,app_info.features.c_str());
        out.m_result_params()->set_package_info(8,app_info.apk_info.c_str());
}	


int McpServlet::get_ios_content_by_class_id(const idl::mcp_get_ios_content_by_class_id_params& in, 
                idl::mcp_get_ios_content_by_class_id_response& out)
{
        UB_LOG_TRACE( "get_ios_content_by_class_id start" );
        int count = 0;
        int res = 0;
        string class_id( in.class_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t orderby( in.orderby() );
        uint32_t for_free( in.for_free() );

        vector<ios_content_t> ios_list;
        vector<ios_content_t>::const_iterator iter;

        ContentClass list( content_type, 1, class_id );

        list.set_unit_info( unit_num, unit_times );
        list.set_priority( orderby );
        list.set_for_free( for_free );
        res = list.get_ios_content_list(count,ios_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_ios_content_by_clss_id failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count(count);
        int i=0;
        for(i=0,iter=ios_list.begin(); iter!=ios_list.end(); ++i, ++iter){
                //set_get_ios_content_by_class_id_result(out, i, iter);
                set_ios_content_result(out, i, iter);
        }
        UB_LOG_TRACE( "get_ios_content_by_class_id end" );
        return 0;
}

int McpServlet::get_ios_content_by_col_id(const idl::mcp_get_ios_content_by_col_id_params& in,
                idl::mcp_get_ios_content_by_col_id_response& out)
{
        UB_LOG_TRACE( "get_ios_content_by_col_id start" );
        int count = 0;
        int res = 0;
        string column_id( in.column_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t orderby( in.orderby() );

        vector<ios_content_t> ios_list;
        vector<ios_content_t>::const_iterator iter;

        ContentColumn list( content_type, 1, column_id );

        column_info_t column_info;
        res=mcp_content_map::get_column_info(column_id, column_info);
        if( res!=0 ){
                count = -5;
                goto end;
        }


        list.set_unit_info( unit_num, unit_times );
        list.set_priority( orderby );
        res = list.get_ios_content_list(count,ios_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_ios_content_by_clss_id failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count(count);
        out.m_result_params()->set_name(column_info.name.c_str());
        int i=0;
        for(i=0,iter=ios_list.begin(); iter!=ios_list.end(); ++i, ++iter){
                //set_get_ios_content_by_col_id_result(out, i, iter);
                set_ios_content_result(out, i, iter);
        }
        UB_LOG_TRACE( "get_ios_content_by_col_id end" );
        return 0;
}

int McpServlet::get_ios_content_by_cp_id(const idl::mcp_get_ios_content_by_cp_id_params& in, 
                idl::mcp_get_ios_content_by_cp_id_response& out)
{
        UB_LOG_TRACE( "get_ios_content_by_cp_id start" );
        int count = 0;
        int res = 0;
        string cp_id(in.cp_id());
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t orderby( in.orderby() );

        vector<ios_content_t> ios_list;
        vector<ios_content_t>::const_iterator iter;

        ContentCp list( content_type, 1, cp_id );

        list.set_unit_info( unit_num, unit_times );
        list.set_priority( orderby );

        res = list.get_ios_content_list(count,ios_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_ios_content_by_cp_id failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
        }

        string cp_name;
        mcp_content_map::get_cp_name(cp_id,cp_name);
        out.m_result_params()->set_count(count);
        out.m_result_params()->set_cp_name(cp_name.c_str());
        int i=0;
        for(i=0,iter=ios_list.begin(); iter!=ios_list.end(); ++i, ++iter){
                //set_get_ios_content_by_cp_id_result(out, i, iter);
                set_ios_content_result(out, i, iter);
        }
        UB_LOG_TRACE( "get_ios_content_by_cp_id end" );
        return 0;
}


int McpServlet::get_ios_content_by_rank(const idl::mcp_get_ios_content_by_rank_params& in, 
                idl::mcp_get_ios_content_by_rank_response& out)
{
        UB_LOG_TRACE( "get_ios_content_by_rank start" );
        int count = 0;
        int res = 0;
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t p_type( in.p_type() );
        uint32_t for_free( in.for_free() );
        UB_LOG_DEBUG( "for_free:[%d]", in.for_free() );

        vector<ios_content_t> ios_list;
        vector<ios_content_t>::const_iterator iter;

        ContentRank list( content_type, 1, "", p_type );
        list.set_unit_info( unit_num, unit_times );
        list.set_for_free( for_free );

        res = list.get_ios_content_list(count,ios_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count(count);
        int i=0;
        for(i=0,iter=ios_list.begin(); iter!=ios_list.end(); ++i, ++iter){
                //set_get_ios_content_by_rank_result(out, i, iter);
                set_ios_content_result(out, i, iter);
        }
        UB_LOG_TRACE( "get_ios_content_by_rank end" );
        return 0;
}

int McpServlet::get_class_list_by_type_id(const idl::mcp_get_class_list_by_type_id_params& in,
                idl::mcp_get_class_list_by_type_id_response &out)
{
        UB_LOG_TRACE( "get_class_list_by_type_id start" );
        string type_id(in.type_id());
        vector<class_info_t> class_list;
        int res=mcp_content_map::get_class_list_by_type(type_id,class_list);
        if(res!=0){
                UB_LOG_FATAL("mcp_content_map::get_class_list_by_type failed, [%s:%d]", __FILE__, __LINE__ );
                out.m_result_params()->set_count(res);
        }else{
                out.m_result_params()->set_count(class_list.size());
        }

        vector<class_info_t>::const_iterator it=class_list.begin();
        for(int i=0;it!=class_list.end();++i,++it){
                out.m_result_params()->mutable_class_list(i)->set_id(it->id.c_str());
                out.m_result_params()->mutable_class_list(i)->set_name(it->name.c_str());
                out.m_result_params()->mutable_class_list(i)->set_icon(it->icon.c_str());
                out.m_result_params()->mutable_class_list(i)->set_count(toString(it->count).c_str());
        }
        UB_LOG_TRACE( "get_class_list_by_type_id end");
        return 0;
}

int McpServlet::get_ios_probably_like_list_by_id(const idl::mcp_get_ios_probably_like_list_by_id_params& in, 
                idl::mcp_get_ios_probably_like_list_by_id_response& out)
{

        UB_LOG_TRACE( "get_ios_probably_like_list_by_id start" );
        int count0=0,count1=0;
        int res=0,i,j;
        string id(in.id());
        uint32_t list_cnt(in.list_cnt());
        uint32_t type(in.type());
        int cnt=list_cnt;
        int left=cnt;

        vector<ios_content_t> ios_list0,ios_list1,ios_list,ios_list_tmp;
        vector<ios_content_t>::const_iterator iter;

        IosContent content(id);
        ios_content_t info;
        res=content.get_info(info);
        string cp_id(info.cp_id);
        string class_id="";
        if(info.class_list.size()>0){
                class_id=info.class_list[0].id;
        }
        string content_type(info.type);

        //PROBABLY_LIKE_TYPE_CP    1
        //PROBABLY_LIKE_TYPE_CLASS 2
        //PROBABLY_LIKE_TYPE_BOTH  3

        if(0 != (type & PROBABLY_LIKE_TYPE_CP))
        {
                UB_LOG_TRACE( "get ios probably like list by same cp_id start" );
                ContentCp list_with_the_same_cp_id( content_type, 1, cp_id );
                list_with_the_same_cp_id.set_unit_info( 100, 1 );
                res = list_with_the_same_cp_id.get_ios_content_list(count0,ios_list_tmp);
                if( res != 0 ){
                        UB_LOG_FATAL( "get_ios_content_by_clss_id failed, [%s:%d]", __FILE__, __LINE__ );
                }else{
                        left-=(count0-1);
                }
                for(i=0;  i<(int)ios_list_tmp.size(); i++)
                {
                        if(ios_list_tmp[i].id != id)
                        {
                                ios_list0.push_back( ios_list_tmp[i]);
                        }
                }	
                vector<int> vi1=commonFunc::randomIntSequence(ios_list0.size());
                for(i=0; i<(int)(vi1.size()); i++)
                {
                        ios_list.push_back(ios_list0[vi1[i]]);
                }
                UB_LOG_TRACE( "get ios probably like list by same cp_id end" );
        }

        UB_LOG_TRACE( "left is [%d]", left);
        if(0 != (type & PROBABLY_LIKE_TYPE_CLASS) && left > 0)
        {
                UB_LOG_TRACE( "get ios probably like list by same class_id start" );
                for(j=0; j<(int)info.class_list.size(); j++){
                        class_id=info.class_list[j].id;
                        ContentClass list_with_the_same_class_id( content_type, 1, class_id );
                        list_with_the_same_class_id.set_unit_info( 100, 1 );
                        res = list_with_the_same_class_id.get_ios_content_list(count0,ios_list_tmp);
                        if( res != 0 ){
                                UB_LOG_FATAL( "get_ios_content_by_clss_id failed, [%s:%d]", __FILE__, __LINE__ );
                        }
                        for(i=0; i<(int)ios_list_tmp.size(); i++)
                        {
                                if(ios_list_tmp[i].id != id)
                                {
                                        ios_list1.push_back( ios_list_tmp[i]);
                                }
                        }
                        count1+=i;
                        if(count1 >= (int)left){
                                break;
                        }
                        left-=ios_list_tmp.size();
                }
                vector<int> vi1=commonFunc::randomIntSequence(ios_list1.size());
                //UB_LOG_TRACE( "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr %d %d %d %d %d",vi1[0],vi1[1],vi1[2],vi1[3],vi1[4]);
                if(vi1.size()!=0)
                {
                        for(i=0; i<left && i<(int)vi1.size(); i++)
                        {
                                ios_list.push_back(ios_list1[vi1[i]]);
                        }
                }
                UB_LOG_TRACE( "get ios probably like list by same class_id end" );
        }

        for(i=0,iter=ios_list.begin(); iter!=ios_list.end(); ++i, ++iter){
                set_ios_content_result(out, i, iter);
        }

        UB_LOG_TRACE( "get_ios_probably_like_list_by_id end" );

        return 0;
}


void set_game_info(idl::game_info_t* out, const game_t& info)
{
        out->set_info_id(info.info_id.c_str());
        out->set_info_title(info.info_title.c_str());
        out->set_info_date(info.info_date.c_str());
        out->set_info_content(info.info_content.c_str());
        out->set_info_type(info.info_type.c_str());
        out->set_c_id(info.c_id.c_str());
        UB_LOG_TRACE("info_id is %s", info.info_type.c_str());
        if(!strcmp(info.info_type.c_str(),"1"))
        {
                out->set_type_name("公告");
        }
        else if(!strcmp(info.info_type.c_str(),"2"))
        {    
                out->set_type_name("新闻");
        } 
        else if(!strcmp(info.info_type.c_str(),"3"))
        {    
                out->set_type_name("活动");
        } 
        else if(!strcmp(info.info_type.c_str(),"4"))
        {    
                out->set_type_name("攻略");
        } 

        // [s] add by gwj for news_type_name
        else if(!strcmp(info.info_type.c_str(),"5"))
        {
                out->set_type_name("开服");
        }
        else if(!strcmp(info.info_type.c_str(),"6"))
        {
                out->set_type_name("开测");
        }
        else if(!strcmp(info.info_type.c_str(),"7"))
        {
                out->set_type_name("新服");
        }
        else if(!strcmp(info.info_type.c_str(),"8"))
        {
                out->set_type_name("预告");
        }
        else if(!strcmp(info.info_type.c_str(),"9"))
        {
                out->set_type_name("问答");
        }
        else if(!strcmp(info.info_type.c_str(),"10"))
        {
                out->set_type_name("评测");
        }
        // [e] add by gwj for news_type_name

        out->set_game_type(info.game_type.c_str());
}

int McpServlet::get_game_info(const idl::mcp_get_game_info_params& in, 
                idl::mcp_get_game_info_response& out)
{
        UB_LOG_TRACE( "get_game_info start" );
        string info_id(in.info_id());
        UB_LOG_TRACE( "info_id is %s",info_id.c_str() );
        int res(0);
        game_t info;

        GameInfoContent gameInfoContent(info_id);
        res = gameInfoContent.get_info(info);

        if(res!=0){
                UB_LOG_FATAL("get_game_info failed, id[%s], [%s:%d]", info_id.c_str(), __FILE__, __LINE__ );
        }  	

        set_game_info(out.m_result_params()->mutable_game_info(),info);

        UB_LOG_TRACE( "get_game_info end" );
        return 0;
}

int McpServlet::get_game_info_list(const idl::mcp_get_game_info_list_params& in, 
                idl::mcp_get_game_info_list_response& out)
{
        UB_LOG_TRACE( "get_game_info_list start" );
        string info_type(in.info_type());
        string game_type(in.game_type());
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());
        int32_t count=0;
        vector<game_brief_t> game_brief_info_list;
        vector<game_brief_t>::const_iterator iter;

        GameList gameList(MCP_GAME_NEWS);
        gameList.set_unit_info(page_size,page);
        //gameList.set_info_type(info_type);
        gameList.set_info_type(info_type, game_type);

        int res=gameList.get_game_brief_list(count,game_brief_info_list);

        if( res != 0 ){ 
                UB_LOG_FATAL( "get_game_info_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res; 
        }

        int i=0;
        for(i=0,iter=game_brief_info_list.begin(); iter!=game_brief_info_list.end(); ++i, ++iter){
                set_gett_game_brief_result(out, i, iter);
        }
        out.m_result_params()->set_count(count);

        UB_LOG_TRACE( "get_game_info_list end" );
        return 0;	
}

int  McpServlet::get_game_info_list_by_c_id(const idl::mcp_get_game_info_list_by_c_id_params& in, 
                idl::mcp_get_game_info_list_by_c_id_response& out)
{
        UB_LOG_TRACE( "get_game_info_list_by_c_id start" );
        string c_id(in.c_id());
        string info_type(in.info_type());
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());
        int32_t count=0;
        vector<game_brief_t> game_brief_info_list;
        vector<game_brief_t>::const_iterator iter;

        GameList gameList(MCP_GAME_NEWS);
        gameList.set_unit_info(page_size,page);
        gameList.set_c_id(c_id);
        gameList.set_info_type(info_type);

        int res=gameList.get_game_brief_list(count,game_brief_info_list);

        if( res != 0 ){
                UB_LOG_FATAL( "get_game_info_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
        }

        out.m_result_params()->set_count(count);
        int i=0;
        for(i=0,iter=game_brief_info_list.begin(); iter!=game_brief_info_list.end(); ++i, ++iter){
                set_game_brief_result(out.m_result_params()->mutable_game_info_list(i), *iter);
        }
        UB_LOG_TRACE( "get_game_info_list_by_c_id end" );
        return 0;
}


int McpServlet::get_game_score_by_imei_or_uid(const idl::mcp_get_game_score_by_imei_or_uid_params& in,  
                idl::mcp_get_game_score_by_imei_or_uid_response& out) 
{
        UB_LOG_TRACE("get_game_score_by_imei_or_uid start");
        string id(in.id());
        string c_id(in.c_id());
        string type(in.type());

        GameScore gameScore(id,c_id,type);
        game_score_t game_score;

        int res=gameScore.get_info(game_score);

        if( res != 0 ){  
                UB_LOG_FATAL( "get_game_score_by_imei_or_uid failed, [%s:%d]", __FILE__, __LINE__ ); 
        }

        set_get_game_score_by_imei_or_uid(out,game_score);
        UB_LOG_TRACE("get_game_score_by_imei_or_uid end");
        return 0;
}

int McpServlet::get_appinfo_by_packagename(const idl::mcp_get_appinfo_by_packagename_params& in,  
                idl::mcp_get_appinfo_by_packagename_response& out) 
{
        UB_LOG_TRACE("get_appinfo_by_packagename start");
        string package_name(in.packagename());

        AppinfoContent appInfo(package_name);
        appinfo_t app_info;

        int res=appInfo.get_info(app_info);

        if( res != 0 ){  
                UB_LOG_FATAL( "get_appinfo_by_packagename failed, [%s:%d]", __FILE__, __LINE__ ); 
        }

        set_get_appinfo_by_packagename(out,app_info);
        UB_LOG_TRACE("get_appinfo_by_packagename end");
        return 0;
}

string removeEscape(const string &str)
{
        string newStr;
        for(size_t i=0; i<str.size(); i++)
        {
                if(str[i] != '\\')
                {
                        newStr.push_back(str[i]);
                }
        }
        return newStr;
}

int McpServlet::get_score_by_packagename_and_versioncode(const idl::mcp_get_score_by_packagename_and_versioncode_params& in,  
                idl::mcp_get_score_by_packagename_and_versioncode_response& out) 
{
        UB_LOG_TRACE("get_score_by_packagename_and_versioncode start");
        string the_json(removeEscape(in.packagename_versioncode_json()));

        Json::Reader json_reader;
        Json::Value json_value;
        Json::Value res_array;
        json_reader.parse(the_json,json_value);

        vector<string> scores;
        string star;
        char num[16];

        const Json::Value arrayObj = json_value["array"];
        UB_LOG_TRACE("count is %d", arrayObj.size());
        for(int i=0; i<(int)arrayObj.size(); i++)
        {
                memset(num,0,sizeof(num));
                sprintf(num,"%d",i);
                star.clear();
                UB_LOG_TRACE("packagename is %s",arrayObj[i]["packagename"].asString().c_str());
                UB_LOG_TRACE("versioncode is %s",arrayObj[i]["versioncode"].asString().c_str());
                UB_LOG_TRACE("imei is %s",arrayObj[i]["imei"].asString().c_str());

                GameStar gamestar(arrayObj[i]["packagename"].asString(), arrayObj[i]["versioncode"].asString(), arrayObj[i]["imei"].asString().c_str());
                gamestar.get_star(star);

                res_array[num]=star.c_str();

                UB_LOG_TRACE("star is %s", star.c_str());
        }

        UB_LOG_TRACE("score out json is %s", res_array.toStyledString().c_str());	

        out.m_result_params()->set_scores_json(res_array.toStyledString().c_str());	

        UB_LOG_TRACE("get_score_by_packagename_and_versioncode end");
        return 0;
}


int McpServlet::get_content_info_by_packagename_and_versioncode(const idl::mcp_get_content_info_by_packagename_and_versioncode_params& in,  
                idl::mcp_get_content_info_by_packagename_and_versioncode_response& out) 
{

        UB_LOG_TRACE("get_content_info_by_packagename_and_versioncode start");
        string package_name(in.packagename());
        string version_code(in.versioncode());

        string cid;
        get_cid_by_packagename_and_versioncode(package_name,version_code,cid);
        if(cid.size() ==0)
        {
                UB_LOG_TRACE("no cid by with this packagename and versioncode");
        }

        content_detail_t detail;
        string info_id = "20,21,22,23,24,45,46";
        get_content_info_by_cid_infoid_infotype(cid, info_id, 3, detail); 		

        set_get_content_info_result1(out, detail);
        UB_LOG_TRACE("get_content_info_by_packagename_and_versioncode end");
        return 0;
}

int McpServlet::get_content_info_by_packagename(const idl::mcp_get_content_info_by_packagename_params& in,  
                idl::mcp_get_content_info_by_packagename_response& out) 
{
        UB_LOG_TRACE("get_content_info_by_packagename start");
        string package_name(in.packagename());
        string cid;
        get_cid_by_packagename(package_name,cid);
        if(cid.size() ==0)
        {
                UB_LOG_TRACE("no cid by with this packagename %s", package_name.c_str());
        }

        content_detail_t detail;
        string info_id = "20,21,22,23,24,45,46";
        get_content_info_by_cid_infoid_infotype(cid, info_id, 3, detail); 		

        set_get_content_info_result1(out, detail);
        UB_LOG_TRACE("get_content_info_by_packagename end");
        return 0;
}

void setOnlineGameResult(idl::online_game_info_t* out, const online_game_t& info)
{
        out->set_id(info.id.c_str());
        out->set_name(info.name.c_str());
        out->set_icon(info.icon.c_str());
        out->set_thumb(info.thumb.c_str());
        out->set_type(info.type.c_str());
        out->set_info(info.info.c_str());
        out->set_version(info.version.c_str());
        out->set_system_req(info.system_req.c_str());
        out->set_cp_id(info.cp_id.c_str());
        string cp_name;
        mcp_content_map::get_cp_name(info.cp_id,cp_name);
        out->set_cp_name(cp_name.c_str());
        out->set_star(toString(info.star).c_str());
        out->set_popularity(toString(info.popularity).c_str());
        out->set_language(info.language.c_str());

        out->set_gdate(info.gdate.c_str());
        out->set_mdate(info.mdate.c_str());
        out->set_ldate(info.ldate.c_str());

        out->set_url(info.url.c_str());
        out->set_size(toString(info.size).c_str());
        /* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 beg*/
        out->set_apk_flag(info.apk_flag.c_str());
        /* 20130328 zhengxie mongodb.mcp.mcp_content_data新增channel_info字段 end*/
		//add by zhengxie
		out->set_editor_recom(info.editor_recom.c_str());
        vector<string>::const_iterator label_it = info.label_list.begin();
        for( int j=0; label_it!=info.label_list.end(); ++label_it, ++j){
                out->set_label_list(j,mcp_content_map::get_label_name(*label_it).c_str());
        }

        vector<string>::const_iterator img_it = info.img_list.begin();
        for( int j=0; img_it!=info.img_list.end(); ++img_it, ++j ){
                out->set_img_list(j,img_it->c_str());
        }

        vector<class_info_t>::const_iterator class_it = info.class_list.begin();
        for( int j=0; class_it!=info.class_list.end(); ++class_it, ++j ){
                out->mutable_class_info(j)->set_id(class_it->id.c_str());
                out->mutable_class_info(j)->set_name(class_it->name.c_str());
        }

        vector<content_attribute_t>::const_iterator attr_it = info.attribute_list.begin();
        for( int j=0; attr_it!=info.attribute_list.end(); ++attr_it, ++j ){
                out->mutable_attribute_list(j)->set_name(mcp_content_map::get_attribute_name(attr_it->type).c_str());
                out->mutable_attribute_list(j)->set_content(attr_it->content.c_str());
        }
        
        // [s] add by gwj for mcp_content_pub_prop
        out->set_pub_prop_ids(info.pub_prop_ids.c_str());
        // [e] add by gwj for mcp_content_pub_prop
        
        return;
}

int McpServlet::get_online_game_by_id(const idl::mcp_get_online_game_by_id_params& in, idl::mcp_get_online_game_by_id_response& out)
{
        UB_LOG_TRACE( "get_online_game_by_id start" );
        string id(in.id());
        int res(0);

        online_game_t info;
        OnlineGame content(id);
        res=content.getInfo(info);
        UB_LOG_TRACE( "info.apk_flag=%s",info.apk_flag.c_str() );
        if( res!=0 ){
                UB_LOG_FATAL( "OnlineGame::getInfo failed, id[%s], [%s:%d]", id.c_str(), __FILE__, __LINE__ );
                goto end;
        }
end:
        out.m_result_params()->set_result( res );
        setOnlineGameResult(out.m_result_params()->mutable_info(), info);
        UB_LOG_TRACE( "get_online_game_by_id end" );
        return 0;
}

        template <typename T>
void setOnlineGameResult(T& out, const int i, vector<online_game_t>::const_iterator iter)
{
        setOnlineGameResult(out.m_result_params()->mutable_info_list(i),*iter);
        return;
}

int McpServlet::get_online_game_by_class_id(const idl::mcp_get_online_game_by_class_id_params& in, 
                idl::mcp_get_online_game_by_class_id_response& out)
{
        UB_LOG_TRACE( "get_online_game_by_class_id start" );
        int count = 0;
        int res = 0;
        string class_id( in.class_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t orderby( in.orderby() );

        vector<online_game_t> og_list;
        vector<online_game_t>::const_iterator iter;

        ContentClass list( content_type, 1, class_id );
        class_info_t class_info;
        res=mcp_content_map::get_class_info(class_id, class_info);
        if( res!=0 ){
                count = -5;
                goto end;
        }

        list.set_unit_info( unit_num, unit_times );
        list.set_priority( orderby );
        res = list.getOnlineGameList(count,og_list);
        if( res != 0 ){
                UB_LOG_FATAL( "getOnlineGameList failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count(count);
        out.m_result_params()->set_class_name(class_info.name.c_str());
        out.m_result_params()->set_class_icon(class_info.icon.c_str());
        int i=0;
        for(i=0,iter=og_list.begin(); iter!=og_list.end(); ++i, ++iter){
                setOnlineGameResult(out, i, iter);
        }
        UB_LOG_TRACE( "get_online_game_by_class_id end" );
        return 0;
}

int McpServlet::get_online_game_by_cp_id(const idl::mcp_get_online_game_by_cp_id_params& in, 
                idl::mcp_get_online_game_by_cp_id_response& out)
{
        return 0;
}

int McpServlet::get_online_game_by_col_id(const idl::mcp_get_online_game_by_col_id_params& in, 
                idl::mcp_get_online_game_by_col_id_response& out)
{
        UB_LOG_TRACE( "get_online_game_by_col_id start" );
        int count = 0;
        int res = 0;
        string column_id( in.column_id() );
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t orderby( in.orderby() );

        vector<online_game_t> og_list;
        vector<online_game_t>::const_iterator iter;

        ContentColumn list( content_type, 1, column_id );

        column_info_t column_info;
        res=mcp_content_map::get_column_info(column_id, column_info);
        if( res!=0 ){
                count = -5;
                goto end;
        }

        list.set_unit_info( unit_num, unit_times );
        list.set_priority( orderby );
        res = list.getOnlineGameList(count,og_list);
        if( res != 0 ){
                UB_LOG_FATAL( "getOnlineGameList failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count(count);
        out.m_result_params()->set_name(column_info.name.c_str());
        int i=0;
        for(i=0,iter=og_list.begin(); iter!=og_list.end(); ++i, ++iter){
                setOnlineGameResult(out, i, iter);
        }
        UB_LOG_TRACE( "get_online_game_by_col_id end" );
        return 0;
}

int McpServlet::get_online_game_by_rank(const idl::mcp_get_online_game_by_rank_params& in, 
                idl::mcp_get_online_game_by_rank_response& out)
{
        UB_LOG_TRACE( "get_online_game_by_rank start" );
        int count = 0;
        int res = 0;
        string content_type(in.type_id());
        uint32_t unit_num( in.page_size() );
        uint32_t unit_times( in.page() );
        uint32_t p_type( in.p_type() );

        vector<online_game_t> og_list;
        vector<online_game_t>::const_iterator iter;

        ContentRank list( content_type, 1, "", p_type );
        list.set_unit_info( unit_num, unit_times );

        res = list.getOnlineGameList(count,og_list);
        if( res != 0 ){
                UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = res;
                goto end;
        }
end:
        out.m_result_params()->set_count(count);
        int i=0;
        for(i=0,iter=og_list.begin(); iter!=og_list.end(); ++i, ++iter){
                setOnlineGameResult(out, i, iter);
        }
        UB_LOG_TRACE( "get_online_game_by_rank end" );
        return 0;
}

int McpServlet::set_user_rating(const idl::mcp_set_user_rating_params& in, idl::mcp_set_user_rating_response& out)
{
        UB_LOG_TRACE( "set_user_rating start" );
        string user_id(in.user_id());
        string c_id(in.c_id());
        uint32_t rating_score(in.rating());
        int res(0);
        Rating rating(c_id);
        res=rating.setRating(user_id, rating_score);
        if(res!=0){
                UB_LOG_FATAL( "Rating::setRating failed, [%s:%d]", __FILE__, __LINE__ );
        }

        out.m_result_params()->set_result(res);
        UB_LOG_TRACE( "set_user_rating end" );
        return 0;
}

int McpServlet::get_user_rating(const idl::mcp_get_user_rating_params& in, idl::mcp_get_user_rating_response& out)
{
        UB_LOG_TRACE( "get_user_rating start" );
        string user_id(in.user_id());
        string c_id(in.c_id());
        Rating rating(c_id);
        int res=rating.getRating(user_id);
        if(res!=0){
                UB_LOG_FATAL( "Rating::setRating failed, [%s:%d]", __FILE__, __LINE__ );
        }

        out.m_result_params()->set_result(res);
        UB_LOG_TRACE( "get_user_rating end" );
        return 0;
}

void setCalenderInfoResult(idl::calender_info_t* out, const calender_t& info)
{
        out->set_id(toString(info.id).c_str());
        out->set_title(info.title.c_str());
        out->set_c_id(toString(info.c_id).c_str());
        out->set_type(info.type);
        out->set_date(info.enable_date.c_str());
}

int McpServlet::get_calender_list(const idl::mcp_get_calender_list_params& in, idl::mcp_get_calender_list_response& out)
{
        UB_LOG_TRACE( "get_calender_list start" );
        (void)in;
        vector<calender_t> vc;
        time_t t = time(0);
        //uint32_t wday = (uint32_t)atoi(date("%w", t).c_str());
        time_t t1 = t - 24*60*60*14;
        string strEndDatetime = date("%F %H:%M:%S", t );
        string strStartDatetime = date("%F %H:%M:%S", t1 );

        ContentCalender contentCalender(strStartDatetime, strEndDatetime);

        int res = contentCalender.getCalenderList(vc);
        if(res<0){
                UB_LOG_FATAL( "content::getCalenderList failed, [%s:%d]", __FILE__, __LINE__ );
        }

        out.m_result_params()->set_result(res);
        vector<calender_t>::const_iterator iter = vc.begin();
        for(int i=0; iter!=vc.end(); ++i, ++iter){
                setCalenderInfoResult(out.m_result_params()->mutable_calender_list(i), *iter);
        }
        UB_LOG_TRACE( "get_calender_list  end" );
        return 0;
}

void setSoftInfoResult(idl::soft_info_t *out, vector<soft_info_t>::const_iterator iter)
{
        UB_LOG_TRACE("set soft info result start");
        out->set_type_id("15");
        out->set_id(iter->id.c_str());
        out->set_name(iter->name.c_str());
        out->set_icon(iter->icon.c_str());
        out->set_preview(iter->preview.c_str());
        out->set_version(iter->version.c_str());
        out->set_size(iter->size.c_str());
        out->set_pop(iter->pop.c_str());
        out->set_adapt(iter->adapt.c_str());
        out->set_date(iter->date.c_str());
        out->set_froms(iter->froms.c_str());
        out->set_info(iter->info.c_str());
        out->set_down_url(iter->down_url.c_str());
        if(!iter->star.empty())
        {    
                out->set_star(iter->star.c_str());
        }  
        UB_LOG_TRACE("set soft info result end");
}

void setThemeInfoResult(idl::theme_info_t *out, vector<android_theme_t>::const_iterator iter)
{
        UB_LOG_TRACE("set theme info result start");
        out->set_type_id("16");
        out->set_id(iter->id.c_str());
        out->set_name(iter->name.c_str());
        out->set_icon(iter->icon.c_str());
        out->set_preview(iter->preview.c_str());
        out->set_size(iter->size.c_str());
        out->set_pop(iter->pop.c_str());
        out->set_adapt(iter->adapt.c_str());
        out->set_date(iter->date.c_str());
        out->set_froms(iter->froms.c_str());
        out->set_info(iter->info.c_str());
        out->set_down_url(iter->down_url.c_str());
        UB_LOG_TRACE("set theme info result end");
}

void setMusicInfoResult(idl::music_info_t *out, vector<music_t>::const_iterator iter)
{
        UB_LOG_TRACE("set music info result start");
        out->set_music_id(iter->music_id.c_str());
        out->set_songname(iter->songname.c_str());
        out->set_area_id(iter->area_id.c_str());
        out->set_area_name(iter->area_name.c_str());
        out->set_cp_id(iter->cp_name.c_str());
        out->set_language_id(iter->language_id.c_str());
        out->set_language(iter->language.c_str());
        out->set_publish_date(iter->publish_date.c_str());
        out->set_intro(iter->intro.c_str());
        out->set_lyric_writer(iter->lyric_writer.c_str());
        out->set_composer(iter->composer.c_str());
        out->set_price(iter->price.c_str());
        out->set_discount(iter->discount.c_str());
        out->set_download_base(iter->download_base.c_str());
        out->set_lyric_txt(iter->lyric_txt.c_str());
        UB_LOG_TRACE("set music info result end");
}


int McpServlet::get_content_list_by_mixed_topic_id(const idl::mcp_get_content_list_by_mixed_topic_id_params& in, 
                idl::mcp_get_content_list_by_mixed_topic_id_response& out)
{
        UB_LOG_TRACE("get content list by mixed topic id start");
        string topic_id(in.topic_id());
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());

        int res(0),count_soft(0),count_theme(0),count_music(0),count(0);
        int i=0,j=0;
        ContentTopic topic(topic_id);
        vector<column_info_t> column_list;

        topic.set_page_info(page,page_size);
        res = topic.get_column_list(count,column_list);
        if(res!=0){
                UB_LOG_FATAL("get_column_list failed, topic_id[%s], [%s:%d]", topic_id.c_str(), __FILE__, __LINE__ );
                //goto end; 
        }
        vector<column_info_t>::iterator it = column_list.begin();
        vector<soft_info_t> vSoft_info;
        vector<soft_info_t>::const_iterator it_soft;
        vector<android_theme_t> vTheme_info;
        vector<android_theme_t>::const_iterator it_theme;
        vector<music_t> vMusic_info;
        vector<music_t>::const_iterator it_music;
        string col_id;
        string content_type_soft("15");
        string content_type_theme("16");

        vector<soft_info_t> soft_list;
        vector<soft_info_t>::const_iterator iter_soft;
        vector<android_theme_t> theme_list;
        vector<android_theme_t>::const_iterator iter_theme;
        UB_LOG_TRACE("column list count is %d", (int)column_list.size());
        for(i=0; it!=column_list.end();++i,++it){
                UB_LOG_TRACE("column id is %s", it->id.c_str());
                do
                {
                        count=0;
                        ContentColumn list( content_type_soft, 1, it->id);
                        column_info_t column_info;
                        res=mcp_content_map::get_column_info(it->id, column_info);

                        list.set_unit_info( page_size, page );
                        res = list.get_soft_list(count,soft_list);
                        UB_LOG_TRACE("soft count of column %s is %d", it->id.c_str(), count);
                        if(count == 0)
                        {
                                UB_LOG_TRACE( "no soft in column %s", it->id.c_str() );
                        }
                        else
                        {
                                vector<soft_info_t>::iterator it_tmp = soft_list.begin();
                                for(j=0; it_tmp!=soft_list.end(); j++,++it_tmp)
                                {
                                        vSoft_info.push_back(*it_tmp);
                                }
                                soft_list.clear();
                                count_soft += count;
                                break;
                        }

                        count=0;
                        ContentColumn list1( content_type_theme, 1, it->id);
                        list1.set_unit_info( page_size, page );
                        res = list1.get_android_theme_list(count,theme_list);
                        UB_LOG_TRACE("theme count of column %s is %d", it->id.c_str(), count);
                        if(count == 0)
                        {    
                                UB_LOG_TRACE( "no theme in column %s", it->id.c_str() );
                        }    
                        else 
                        {
                                vector<android_theme_t>::iterator it_tmp = theme_list.begin();
                                for(j=0; it_tmp!=theme_list.end(); j++,++it_tmp)
                                {
                                        vTheme_info.push_back(*it_tmp);
                                }
                                theme_list.clear();
                                count_theme += count;
                                break;
                        }
                }while(0);	
        }
        out.m_result_params()->set_count_soft( count_soft );
        out.m_result_params()->set_count_theme( count_theme );
        it_soft = vSoft_info.begin();
        UB_LOG_TRACE("soft list count is %d", (int)vSoft_info.size());
        it_theme = vTheme_info.begin();
        UB_LOG_TRACE("theme list count is %d", (int)vTheme_info.size());
        for(i=0; it_soft != vSoft_info.end(); i++,it_soft++)
        {
                setSoftInfoResult(out.m_result_params()->mutable_soft_info_list(i), it_soft);
        }
        for(i=0; it_theme != vTheme_info.end(); i++, it_theme++)
        {
                setThemeInfoResult(out.m_result_params()->mutable_theme_info_list(i), it_theme);
        }

        MTMusicContentList music_content_list(topic_id,page,page_size);	
        music_content_list.get_MTMusic_content_list(count_music, vMusic_info);

        out.m_result_params()->set_count_song( vMusic_info.size() );
        it_music = vMusic_info.begin();
        for(i=0; it_music != vMusic_info.end(); i++, it_music++)
        {
                setMusicInfoResult(out.m_result_params()->mutable_music_info_list(i), it_music);
        }

        return 0;
}


int McpServlet::get_content_list_by_mixed_topic_id1(const idl::mcp_get_content_list_by_mixed_topic_id1_params& in, 
                idl::mcp_get_content_list_by_mixed_topic_id1_response& out)
{
        UB_LOG_TRACE("get content list by mixed topic id1 start");

        uint32_t col_num1=0,col_num2=0,col_num3=0;
        vector<string> vcol_id;
        vector<string> vcol_name;
        //vector<uint32_t> vcol_num;

        string topic_id(in.topic_id());
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());

        int res(0),count_soft(0),count_theme(0),count(0);
        int i=0,j=0;
        ContentTopic topic(topic_id);
        vector<column_info_t> column_list;

        topic.set_page_info(page,page_size);
        res = topic.get_column_list(count,column_list);
        if(res!=0){
                UB_LOG_FATAL("get_column_list failed, topic_id[%s], [%s:%d]", topic_id.c_str(), __FILE__, __LINE__ );
                //goto end; 
        }
        vector<column_info_t>::iterator it = column_list.begin();
        vector<soft_info_t> vSoft_info;
        vector<soft_info_t>::const_iterator it_soft;
        vector<android_theme_t> vTheme_info;
        vector<android_theme_t>::const_iterator it_theme;
        vector<string> vMusic_info;
        vector<string>::const_iterator it_music;
        string col_id;
        string content_type_soft("15");
        string content_type_theme("16");
        string content_type_H5_music("29");

        vector<soft_info_t> soft_list;
        vector<soft_info_t>::const_iterator iter_soft;
        vector<android_theme_t> theme_list;
        vector<android_theme_t>::const_iterator iter_theme;
        vector<string> id_list;
        vector<uint32_t>::const_iterator iter_musicid; 
        UB_LOG_TRACE("column list count is %d", (int)column_list.size());
        for(i=0; it!=column_list.end();++i,++it){
                UB_LOG_TRACE("column id is %s", it->id.c_str());
                do
                {
                        count=0;
                        ContentColumn list( content_type_soft, 1, it->id);
                        column_info_t column_info;
                        res=mcp_content_map::get_column_info(it->id, column_info);

                        list.set_unit_info( page_size, page );
                        res = list.get_soft_list(count,soft_list);
                        UB_LOG_TRACE("soft count of column %s is %d", it->id.c_str(), count);
                        if(count == 0)
                        {
                                UB_LOG_TRACE( "no soft in column %s", it->id.c_str() );
                        }
                        else
                        {
                                vector<soft_info_t>::iterator it_tmp = soft_list.begin();
                                for(j=0; it_tmp!=soft_list.end(); j++,++it_tmp)
                                {
                                        vcol_id.push_back(it->id);
                                        vcol_name.push_back(it->name);	
                                        vSoft_info.push_back(*it_tmp);
                                }
                                //vcol_num.push_back(j);
                                col_num1+=j;
                                soft_list.clear();
                                count_soft += count;
                                break;
                        }

                        count=0;
                        ContentColumn list1( content_type_theme, 1, it->id);
                        list1.set_unit_info( page_size, page );
                        res = list1.get_android_theme_list(count,theme_list);
                        UB_LOG_TRACE("theme count of column %s is %d", it->id.c_str(), count);
                        if(count == 0)
                        {    
                                UB_LOG_TRACE( "no theme in column %s", it->id.c_str() );
                        }    
                        else 
                        {
                                vector<android_theme_t>::iterator it_tmp = theme_list.begin();
                                for(j=0; it_tmp!=theme_list.end(); j++,++it_tmp)
                                {
                                        vcol_id.push_back(it->id);
                                        vcol_name.push_back(it->name);
                                        vTheme_info.push_back(*it_tmp);
                                }
                                //vcol_num.push_back(j);
                                col_num2+=j;
                                theme_list.clear();
                                count_theme += count;
                                break;
                        }

                        count=0;
                        ContentColumn list2( content_type_H5_music, 1, it->id);
                        list1.set_unit_info( page_size, page );
                        res = list1.get_H5_music_id_list(count,id_list, it->id);
                        UB_LOG_TRACE("music count of column %s is %d", it->id.c_str(), count);
                        //if(count == 0)
                        //{    
                        //        UB_LOG_TRACE( "no music in column %s", it->id.c_str() );
                        //}    
                        //else 
                        //{
                        vector<string>::iterator it_tmp = id_list.begin();
                        for(j=0; it_tmp!=id_list.end(); j++,++it_tmp)
                        {
                                vcol_id.push_back(it->id);
                                vcol_name.push_back(it->name);

                                UB_LOG_TRACE("push id is %s", id_list[j].c_str());
                                vMusic_info.push_back(id_list[j]);
                        }
                        id_list.clear();
                        //vcol_num.push_back(j);
                        UB_LOG_TRACE("jjjjjjjjjj is %d", j);
                        col_num3+=j;
                        //count_music += count;
                        break;
                        //}

                }while(0);	
        }
        it_soft = vSoft_info.begin();
        it_theme = vTheme_info.begin();
        UB_LOG_TRACE("soft list count is %d", (int)vSoft_info.size());
        UB_LOG_TRACE("theme list count is %d", (int)vTheme_info.size());
        UB_LOG_TRACE("music list count is %d", (int)vMusic_info.size());
        for(i=0; it_soft != vSoft_info.end(); i++,it_soft++)
        {
                setSoftInfoResult(out.m_result_params()->mutable_soft_info_list(i), it_soft);
        }
        for(i=0; it_theme != vTheme_info.end(); i++, it_theme++)
        {
                setThemeInfoResult(out.m_result_params()->mutable_theme_info_list(i), it_theme);
        }

        for(i=0; i<(int)vcol_id.size(); i++)
        {
                out.m_result_params()->set_col_name(i, vcol_name[i].c_str());
                out.m_result_params()->set_col_id(i, vcol_id[i].c_str());
        }
        for(i=0; i<(int)vMusic_info.size(); i++)
        {
                out.m_result_params()->set_music_id(i, vMusic_info[i].c_str());
        }

        //intentionally exchange 1 2
        out.m_result_params()->set_col_num1(col_num2);
        out.m_result_params()->set_col_num2(col_num1);
        out.m_result_params()->set_col_num3(col_num3);

        return 0;
}


int McpServlet::set_recently_played(const idl::mcp_set_recently_played_params& in, idl::mcp_set_recently_played_response& out){
        UB_LOG_TRACE("set_recently_played start");
        string u_id(in.u_id());
        string c_id(in.c_id());
        int res=0;

        DuokuParadise duokuParadise(u_id, c_id);
        res = duokuParadise.setRecentlyPlayed();

        out.m_result_params()->set_result(res);
        UB_LOG_TRACE("set_recently_played end");
        return 0;
}


        template <typename T>
void set_get_recently_played(T& out, int i, content_detail_t detail)
{
        UB_LOG_TRACE("set result start");
        out.m_result_params()->set_info_list(i,0,detail.id.c_str());	
        out.m_result_params()->set_info_list(i,1,detail.name.c_str());	

        int isset=0;
        for(int j=0;j<(int)detail.img_info.size();++j){
                if(!strcmp(detail.img_info[j].type.c_str(), "12")){
                        out.m_result_params()->set_info_list(i, 2, detail.img_info[j].url.c_str()); 
                        isset =1;
                        //break;
                }    
        }
        if(isset == 0)
        {
                out.m_result_params()->set_info_list(i, 2, "");
        }
        UB_LOG_TRACE("set result end");
}

int McpServlet::get_recently_played_by_uid(const idl::mcp_get_recently_played_by_uid_params& in, idl::mcp_get_recently_played_by_uid_response& out)
{
        UB_LOG_TRACE("get_recently_played start");
        string u_id(in.u_id());
        string c_id("");
        vector<string> vs;

        DuokuParadise duokuParadise(u_id, c_id);
        int count = duokuParadise.getRecentlyPlayed(vs);
        UB_LOG_TRACE("get recently played count is %d", count);
        out.m_result_params()->set_count(count);
        content_detail_t detail;
        int res=0;
        string info_id("");
        string info_type("1");
        for(int i=0; i<(int)vs.size(); i++)
        {
                res = get_content_info_by_cid_infoid_infotype(vs[i], info_id, 1, detail);	
                set_get_recently_played(out,i,detail);
        }
        out.m_result_params()->set_count(vs.size());

        UB_LOG_TRACE("get_recently_played end");

        return 0;
}

int McpServlet::add_game_to_favorite(const idl::mcp_add_game_to_favorite_params& in, idl::mcp_add_game_to_favorite_response& out)
{
        UB_LOG_TRACE("add_game_to_favorite start");
        string u_id(in.u_id());
        string c_id(in.c_id());

        DuokuParadise duokuParadise(u_id, c_id);
        int res = duokuParadise.setFavorite();
        out.m_result_params()->set_result(res);

        UB_LOG_TRACE("add_game_to_favorite end");
        return 0;
}

int McpServlet::get_favoraite_by_uid(const idl::mcp_get_favoraite_by_uid_params& in, idl::mcp_get_favoraite_by_uid_response& out)
{
        UB_LOG_TRACE("get_favoraite_by_uid start");
        string u_id(in.u_id());
        string c_id("");

        vector<string> vs;
        DuokuParadise duokuParadise(u_id, c_id);
        int count = duokuParadise.getFavorite(vs);
        out.m_result_params()->set_count(count);

        int res=0;
        string info_id("");
        string info_type("1");
        for(int i=0; i<(int)vs.size(); i++) 
        {    
                UB_LOG_TRACE("cid is %s", vs[i].c_str());
                content_detail_t detail;
                res = get_content_info_by_cid_infoid_infotype(vs[i], info_id, 1, detail);	
                set_get_recently_played(out,i,detail);
        }
        out.m_result_params()->set_count(vs.size());

        UB_LOG_TRACE("get_favoraite_by_uid end");
        return 0;
}

        template <typename T>
void set_H5_result(T& out, int i, duoku_paradise_game_t dp_game)
{
        UB_LOG_TRACE("set H5 result start");
        out.m_result_params()->set_info_list(i, 0, dp_game.id.c_str());
        out.m_result_params()->set_info_list(i, 1, dp_game.name.c_str());
        out.m_result_params()->set_info_list(i, 2, dp_game.class_id.c_str());
        out.m_result_params()->set_info_list(i, 3, dp_game.class_name.c_str());
        out.m_result_params()->set_info_list(i, 4, dp_game.icon_url.c_str());
        out.m_result_params()->set_info_list(i, 5, dp_game.img_small_url.c_str());
        out.m_result_params()->set_info_list(i, 6, dp_game.img_big_url.c_str());
        out.m_result_params()->set_info_list(i, 7, dp_game.intro.c_str());
        out.m_result_params()->set_info_list(i, 8, dp_game.num_playing.c_str());

        UB_LOG_TRACE("set H5 result end");
}

int McpServlet::get_H5_latest_on_shelf_by_date(const idl::mcp_get_H5_latest_on_shelf_by_date_params& in,
                idl::mcp_get_H5_latest_on_shelf_by_date_response& out)
{
        UB_LOG_TRACE("get_H5_latest_on_shelf start");
        int count = 0; 
        int res = 0;

        string content_type(MCP_H5_GAME);
        uint32_t unit_num( in.unit_num() );
        uint32_t unit_times( in.unit_times() );
        string info_id( "" );

        vector<duoku_paradise_game_t> vd;
        DuokuParadiseGameList dpGame(content_type, unit_num, unit_times);
        res = dpGame.get_h5_game_list(count, vd, 0);

        if( res != 0 ){ 
                UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        }

        out.m_result_params()->set_count( count );
        for(int i=0; i<(int)vd.size(); i++)
        {
                set_H5_result(out, i, vd[i]);				
        }

        UB_LOG_TRACE("get_H5_latest_on_shelf end");
        return 0;
}

int McpServlet::get_H5_latest_on_shelf_by_date_and_type(const idl::mcp_get_H5_latest_on_shelf_by_date_and_type_params& in,
                idl::mcp_get_H5_latest_on_shelf_by_date_and_type_response& out)
{
        UB_LOG_TRACE("get_H5_latest_on_shelf start");
        int count = 0; 
        int res = 0;

		//string content_type(MCP_H5_GAME);
		string content_type(in.content_type());
        uint32_t unit_num( in.unit_num() );
        uint32_t unit_times( in.unit_times() );
        string info_id( "" );

        vector<duoku_paradise_game_t> vd;
        DuokuParadiseGameList dpGame(content_type, unit_num, unit_times);
        //res = dpGame.get_h5_game_list(count, vd, 0);
        res = dpGame.get_h5_game_list_by_type(count, vd, atoi(content_type.c_str()));

        if( res != 0 ){ 
                UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        }

        out.m_result_params()->set_count( count );
        for(int i=0; i<(int)vd.size(); i++)
        {
                set_H5_result(out, i, vd[i]);				
        }

        UB_LOG_TRACE("get_H5_latest_on_shelf end");
        return 0;
}
int McpServlet::get_H5_info_list_by_col_id(const idl::mcp_get_H5_info_list_by_col_id_params& in,
                idl::mcp_get_H5_info_list_by_col_id_response& out)
{
        UB_LOG_TRACE("get_H5_info_list_by_col_id start");
        int count = 0; 
        int res = 0;

        string content_type(MCP_H5_GAME);
        uint32_t unit_num( in.unit_num() );
        uint32_t unit_times( in.unit_times() );
        string info_id( "" );
        string col_id(in.col_id());
        int type = atoi(col_id.c_str());

        vector<duoku_paradise_game_t> vd;
        DuokuParadiseGameList dpGame(content_type, unit_num, unit_times);
        res = dpGame.get_h5_game_list(count, vd, type);

        if( res != 0 ){ 
                UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        }

        out.m_result_params()->set_count( count );
        for(int i=0; i<(int)vd.size(); i++)
        {
                set_H5_result(out, i, vd[i]);				
        }

        UB_LOG_TRACE("get_H5_info_list_by_col_id end");
        return 0;
}

int McpServlet::get_imgurls_and_favorite_by_uid_cid(const idl::mcp_get_imgurls_and_favorite_by_uid_cid_params& in,
                idl::mcp_get_imgurls_and_favorite_by_uid_cid_response& out)
{
        UB_LOG_TRACE("get_imgurls_and_favorite_by_uid_cid start");

        string content_type(MCP_H5_GAME);
        string uid(in.uid());
        string cid(in.cid());

        vector<string> vs;
        DuokuParadiseGameList dpGame(content_type, 1,1);
        dpGame.get_h5_img_urls_by_cid(cid, vs);

        int isFavorite = dpGame.check_favorite(uid,cid);


        out.m_result_params()->set_is_favorite((uint32_t)isFavorite);

        if(vs.size()==0)
        {
                out.m_result_params()->set_count(0);
                out.m_result_params()->set_icon_url("");
                out.m_result_params()->set_xc_url1("");
                out.m_result_params()->set_xc_url2("");
        }
        else
        {
                out.m_result_params()->set_count((uint32_t)vs.size()-3);
                out.m_result_params()->set_icon_url(vs[0].c_str());
                out.m_result_params()->set_xc_url1(vs[1].c_str());
                out.m_result_params()->set_xc_url2(vs[2].c_str());
                for(int i=3; i<(int)vs.size(); i++)
                {
                        out.m_result_params()->set_xq_url(i-3,vs[i].c_str());
                }
        }
        return 0;
}

int McpServlet::get_random_h5_games(const idl::mcp_get_random_h5_games_params& in,
                idl::mcp_get_random_h5_games_response& out)
{
        UB_LOG_TRACE("get_random_h5_games start");
        uint32_t count(in.count());;
        vector<duoku_paradise_game_t> vd;	
        string content_type(MCP_H5_GAME);
        DuokuParadiseGameList dpGame(content_type, 1,1);
        int acnt = dpGame.get_rand_h5_games((int)count,vd);

        out.m_result_params()->set_actual_count(acnt);
        for(int i=0; i<(int)vd.size(); i++) 
        {    
                set_H5_result(out, i, vd[i]);     
        }   

        UB_LOG_TRACE("get_random_h5_games end");
        return 0;
}


        template <typename T>
void set_info(idl::mcp_get_info_by_col_id_and_type_id_response& out, int i, T info)
{
        out.m_result_params()->set_info_list(i, 0, info.id.c_str());
        out.m_result_params()->set_info_list(i, 1, info.name.c_str());
        out.m_result_params()->set_info_list(i, 2, info.icon.c_str());
        out.m_result_params()->set_info_list(i, 3, info.preview.c_str());
        out.m_result_params()->set_info_list(i, 4, info.version.c_str());
        out.m_result_params()->set_info_list(i, 5, info.size.c_str());
        out.m_result_params()->set_info_list(i, 6, info.pop.c_str());
        out.m_result_params()->set_info_list(i, 7, info.adapt.c_str());
        out.m_result_params()->set_info_list(i, 8, info.date.c_str());
        out.m_result_params()->set_info_list(i, 8, info.froms.c_str());
        out.m_result_params()->set_info_list(i, 9, info.info.c_str());
        out.m_result_params()->set_info_list(i, 10, info.down_url.c_str());
}

        template <typename T>
void set_info(idl::mcp_get_info_by_col_id_response& out, int i, T info, int type)
{
        string classid_name;
        vector<class_info_t>::iterator class_info_iter;
        vector<class_info_t> classid_list;
        classid_list = info.class_info_list;
        int j;
        if(type == 1)
        {
                j=4;
        }
        else if(type == 0)
        {
                j=5;
        }

        out.m_result_params()->set_info_list(i, 0, info.id.c_str());
        out.m_result_params()->set_info_list(i, 1, info.name.c_str());
        out.m_result_params()->set_info_list(i, 2, info.icon.c_str());
        out.m_result_params()->set_info_list(i, 3, info.preview.c_str());
        out.m_result_params()->set_info_list(i, 4, info.version.c_str());
        out.m_result_params()->set_info_list(i, j++, info.size.c_str());
        out.m_result_params()->set_info_list(i, j++, info.pop.c_str());
        out.m_result_params()->set_info_list(i, j++, info.adapt.c_str());
        out.m_result_params()->set_info_list(i, j++, info.date.c_str());
        out.m_result_params()->set_info_list(i, j++, info.froms.c_str());
        out.m_result_params()->set_info_list(i, j++, info.info.c_str());
        out.m_result_params()->set_info_list(i, j++, info.down_url.c_str());

        for(class_info_iter = classid_list.begin(); class_info_iter != classid_list.end(); ++class_info_iter){
                classid_name.append(class_info_iter->id);
                classid_name.append(",");
                classid_name.append(class_info_iter->name);
                classid_name.append(";");
        } 
        out.m_result_params()->set_info_list(i,j++,classid_name.c_str());
}


int McpServlet::get_info_by_col_id_and_type_id(const idl::mcp_get_info_by_col_id_and_type_id_params& in,
                idl::mcp_get_info_by_col_id_and_type_id_response& out)
{   
        UB_LOG_TRACE("get_info_by_col_id_and_type_id start");
        string col_id(in.col_id());
        string type_id(in.type_id());
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());
        ContentColumn list( type_id, 1, col_id);
        list.set_unit_info( page_size, page );
        int count;
        int i,j;

        vector<soft_info_t> soft_list;
        vector<android_theme_t> theme_list;
        vector<string> id_list;

        if(type_id.compare("15") == 0)
        {
                list.get_soft_list(count, soft_list);
        }
        else if(type_id.compare("16") == 0)
        {
                list.get_android_theme_list(count, theme_list);
        }
        else
        {
                list.get_H5_music_id_list(count, id_list, col_id);
                vector<string>::iterator it_tmp = id_list.begin();
                for(j=0; it_tmp!=id_list.end(); j++,++it_tmp)
                {
                        out.m_result_params()->set_info_list(j, 0, id_list[j].c_str());
                }
        }

        if(count>0)
        {
                out.m_result_params()->set_count(count);
                if(soft_list.size()>0)
                {
                        for(i=0; i<(int)soft_list.size(); i++)
                        {
                                set_info(out,i,soft_list[i]);
                        }
                }
                else
                {
                        for(i=0; i<(int)theme_list.size(); i++)
                        {
                                set_info(out,i,theme_list[i]);
                        }
                }
        }
        else
        {
                out.m_result_params()->set_count((int)id_list.size());
        }

        UB_LOG_TRACE("get_info_by_col_id_and_type_id end");
        return 0;
}

int McpServlet::get_info_by_col_id(const idl::mcp_get_info_by_col_id_params& in,
                idl::mcp_get_info_by_col_id_response& out)
{   
        UB_LOG_TRACE("get_info_by_col_id_and_type_id start");
        string col_id(in.col_id());
        //string type_id(in.type_id());
        string type_id;
        int res = commonFunc::get_typeid_by_colid(col_id, type_id);        
        if(res<0)
        {
                UB_LOG_TRACE("no col id %s", col_id.c_str());
                return 0;
        }
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());
        ContentColumn list( type_id, 1, col_id);
        list.set_unit_info( page_size, page );
        int count;
        int i,j;

        vector<soft_info_t> soft_list;
        vector<android_theme_t> theme_list;
        vector<string> id_list;

        if(type_id.compare("15") == 0)
        {
                list.get_soft_list(count, soft_list);
        }
        else if(type_id.compare("16") == 0)
        {
                list.get_android_theme_list(count, theme_list);
        }
        else
        {
                list.get_H5_music_id_list(count, id_list, col_id);
                vector<string>::iterator it_tmp = id_list.begin();
                for(j=0; it_tmp!=id_list.end(); j++,++it_tmp)
                {
                        out.m_result_params()->set_info_list(j, 0, id_list[j].c_str());
                }
        }

        column_info_t column_info;
        res = mcp_content_map::get_column_info( col_id, column_info );
        out.m_result_params()->set_col_name(column_info.name.c_str());
        UB_LOG_TRACE("col name is %s", column_info.name.c_str());
        out.m_result_params()->set_type_id(type_id.c_str());
        if(count>0)
        {
                out.m_result_params()->set_count(count);
                if(soft_list.size()>0)
                {
                        for(i=0; i<(int)soft_list.size(); i++)
                        {
                                set_info(out,i,soft_list[i],0);
                        }
                }
                else
                {
                        for(i=0; i<(int)theme_list.size(); i++)
                        {
                                set_info(out,i,theme_list[i],1);
                        }
                }
        }
        else
        {
                out.m_result_params()->set_count((int)id_list.size());
        }

        UB_LOG_TRACE("get_info_by_col_id_and_type_id end");
        return 0;
}

int McpServlet::get_soft_by_date_or_star(const idl::mcp_get_soft_by_date_or_star_params& in, idl::mcp_get_soft_by_date_or_star_response& out)
{
        UB_LOG_TRACE("get_soft_by_date_or_star start");
        uint32_t page(in.page());
        uint32_t page_size(in.page_size());
        uint32_t orderby(in.orderby());
        uint32_t for_adapt(in.for_adapt());
        uint32_t count = 0;
        string except_class_ids(in.except_class_ids());

        vector<soft_info_t> soft_list;
        vector<soft_info_t>::const_iterator iter;
        vector<string> except_class_list;

        string tmp;
        size_t pos=0;
        pos = except_class_ids.find(",");
        UB_LOG_TRACE("ids is %s", except_class_ids.c_str());
        while(pos != string::npos)
        {
                tmp = except_class_ids.substr(0,pos);
                except_class_ids.erase(0,pos+1);
                except_class_list.push_back(tmp);

                pos = except_class_ids.find(",");
        }
        if(!except_class_ids.empty())
        {
                except_class_list.push_back(except_class_ids);
        }


        ContentClass list( MCP_CONTENT_SOFT, for_adapt, "0", page, page_size);
        class_info_t class_info;

        list.set_unit_info( page, page_size);
        list.set_priority( orderby );
        int res = list.get_soft_list_with_except_class_ids(soft_list, except_class_list, orderby, count);
        UB_LOG_TRACE("counttttt is %d", count);
        if( res != 0 ){
                UB_LOG_FATAL( "get_soft_list failed, [%s:%d]", __FILE__, __LINE__ );
                count = 0;
                goto end;
        }
end:
        out.m_result_params()->set_count( count );
        iter = soft_list.begin();
        for( int i = 0; iter != soft_list.end(); ++iter, ++i )
        {
                set_get_soft_list_by_class_id_result( out, i, iter );
        }

        UB_LOG_TRACE("get_soft_by_date_or_star end");
        return 0;
}

int McpServlet::get_star_by_cid_and_type(const idl::mcp_get_star_by_cid_and_type_params& in, idl::mcp_get_star_by_cid_and_type_response& out)
{
        UB_LOG_TRACE("get_star_by_cid_and_type start");
        string c_id(in.c_id());
        string type_id(in.type_id());
        
        GetStar getStar(c_id, type_id);  
        uint32_t star = 0;
        int res = getStar.get_star(star);
        if(res != 0)
        {
            UB_LOG_FATAL("get star failed by cid %s", c_id.c_str());
        }
        out.m_result_params()->set_star(star);
        UB_LOG_TRACE("get_star_by_cid_and_type end");
        return 0;
}

int McpServlet::get_content_list_from_classID_with_weight(const idl::mcp_get_content_list_from_classID_with_weight_params& in,
                                                                    idl::mcp_get_content_list_from_classID_with_weight_response& out)
{
    UB_LOG_TRACE( "get_content_list_from_classID_with_weight start" );

    string class_id( in.class_id() );
    string content_type( in.content_type_id() );
    uint32_t priority( in.priority() );
    string info_id( in.info_id() );
    uint32_t info_type( in.info_type() );
    string img_size( in.img_size() );
    uint32_t unit_num( in.unit_num() );
    uint32_t unit_times( in.unit_times() );
    int32_t weight_type(in.weight_type());

    uint32_t for_adapt( in.for_adapt() );

    int res( 0 );
    int32_t count( 0 );
    uint32_t i( 0 );
    vector<content_info_t>::iterator iter;
    vector<content_info_t> content_info_list;

    ContentClass list( content_type, for_adapt, class_id );
    list.set_unit_info( unit_num, unit_times );
    list.set_adapt_info( info_type, info_id );
    list.set_priority( priority );
    res = list.get_content_list_with_weight(count,content_info_list,weight_type);
    if( res != 0 ){
        UB_LOG_FATAL( "get_content_list failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_count( count );
    iter = content_info_list.begin();
    count= content_info_list.size();
    for( i = 0; iter != content_info_list.end(); ++iter, ++i ){
        UB_LOG_NOTICE( "id, [%s]", iter->id.c_str() );
        set_get_content_list_from_classID_result( out, i, iter );
    }
    UB_LOG_TRACE( "get_content_list_from_classID_with_weight end" );
    return 0;
   
}

int McpServlet::use_exchange_code(const idl::mcp_use_exchange_code_params& in, idl::mcp_use_exchange_code_response& out) 
{
    UB_LOG_TRACE("use_exchange_code start");
    CommunityClient communityClient;
    int32_t userId(in.userId());
    string gameId(in.gameId());
    string exchangeCodeId(in.exchangeCodeId());
    bsl::mempool* mp;
    idl::result_cp resultCP(mp);

    int res = communityClient.use_exchange_code(userId, gameId, exchangeCodeId,  resultCP);
    if(res == 0)
    {    
                UB_LOG_TRACE("use_exchange_code success %d code is %s", res, exchangeCodeId.c_str());
            }    
    else 
    {    
                UB_LOG_TRACE("use_exchange_code success %d code is %s", res, exchangeCodeId.c_str());
            }    
    UB_LOG_TRACE("use_exchange_code end");

    return 0;
}
