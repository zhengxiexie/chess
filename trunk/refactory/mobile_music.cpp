 /*
 * mobile_music.cpp
 *
 *  Created on: 2013-6-26
 *      Author: zhengxie
 */
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <time.h>

#include "../redis_connpool.h"
#include "decorator/single_decorator/is_decorator.h"
#include "decorator/single_decorator/like_decorator.h"
#include "decorator/compound_decorator/abstract_compound_decorator.h"
#include "module/mb_music_content.h"
#include "music_output_type/mobile_music_info_t.h"
#include "redis_module/redis_module.h"

#include "../mcp_session.h"
#include "../mcp.idl.mcp.h"
#include "../mcpsvr.h"


using namespace std;
using namespace content;

int McpServlet::get_mobile_music_info(const idl::mcp_get_mobile_music_info_params& in, idl::mcp_get_mobile_music_info_response& out)
{
    LikeDecorator name_decor;
    IsDecorator singer_decor;
    AbstractCompoundDecorator compound_decor;

    const char* name = in.name();
    const char* singer = in.singer();
	const char* ring_id = in.ring_id();

	LOGA("[zx] %s: start. name[%s], singer[%s], ring_id[%s] ",
         __FUNCTION__, name, singer, ring_id );
    
    if(IS_VALUED_STRING(name))
    {
        name_decor.add_field_filter_condition("name", name);
        compound_decor.add_primary_table_decorator(&name_decor);
    }

    if(IS_VALUED_STRING(singer))
    {
        singer_decor.add_field_filter_condition("singer", singer);
        compound_decor.add_primary_table_decorator(&singer_decor);
    }

    if(IS_VALUED_STRING(ring_id))
    {
        singer_decor.add_field_filter_condition("ring_id", ring_id);
        compound_decor.add_primary_table_decorator(&singer_decor);
    }


    //////////////////////////////////////////////////////////// 处理 module 获取数据  ////////////////////////////////////////////////////////////
	
    MCPMBMusicContent mb_music_content("ring_id");

    // 给 module 装备装饰器
    mb_music_content.load_decorator(&compound_decor);

    int ret = mb_music_content.general_fetch_list_records(false);

    if(ret < 0)
    {
        LOGA("[GWJ] %s: [mb_music_content] fetch list Error! ", __FUNCTION__);
    }

    int record_count = mb_music_content.get_key_size();

    LOGD("[GWJ] %s: record_count[%d] after fetch_list_records",
         __FUNCTION__, record_count);

	out.m_result_params()->set_count(record_count);
    if(record_count == 0)
    {
        return 0;
    }

    for(int i = 0; i < record_count; i++)
    {
		MCPMBMusicInfo* music_info = (MCPMBMusicInfo*)(mb_music_content.get_record_by_key_vec(i,0));
		mobile_music_info_t::set_outparameter(out.m_result_params()->mutable_music_list(i),
                                              music_info);
    }

    return 0;
}

int McpServlet::get_mobile_music_ring_id_by_ptag(const idl::mcp_get_mobile_music_ring_id_by_ptag_params& in, 
													   idl::mcp_get_mobile_music_ring_id_by_ptag_response& out)
{
	const char* ptag = in.ptag();

	LOGA("[zx] %s: start. ptag[%s]", __FUNCTION__, ptag);

	string ptag_key = "MBMUSIC:"+string(ptag);
	string ring_id;

	RedisModule redis_proc;
	int result = redis_proc.get_value_by_key(ptag_key.c_str(), ring_id);
	if(0!=result)
	{
		LOGA("[GWJ] %s: get redis fail,ptag_key:%s", __FUNCTION__, ptag_key.c_str());
		out.m_result_params()->set_result(0);
		return 0;
	}

	out.m_result_params()->set_ring_id(ring_id.c_str());
	out.m_result_params()->set_result(0);
	return 0;
}

int McpServlet::set_mobile_music_ptag_by_ring_id(const idl::mcp_set_mobile_music_ptag_by_ring_id_params& in, 
												       idl::mcp_set_mobile_music_ptag_by_ring_id_response& out)
{
	const char* ring_id = in.ring_id();
	const char* ptag = in.ptag();

	LOGA("[zx] %s: start.ring_id[%s], ptag[%s]", __FUNCTION__, ring_id, ptag);

	string ptag_key = "MBMUSIC:"+string(ptag);

	RedisModule redis_proc;
	int result = redis_proc.del_key_record(ptag_key.c_str());
	if(0!=result)
	{
		LOGA("[GWJ] %s: remove redis fail,ptag_key:%s", __FUNCTION__, ptag_key.c_str());
		return 0;
	}

	RedisModule redis_proc_2;
    result = redis_proc_2.add_new_tuple(ptag_key.c_str(), ring_id);
	if(0!=result)
	{
		LOGA("[GWJ] %s: add redis fail,ptag_key:%s", __FUNCTION__, ptag_key.c_str());
		return 0;
	}

	out.m_result_params()->set_result(0);

	return 0;
}

