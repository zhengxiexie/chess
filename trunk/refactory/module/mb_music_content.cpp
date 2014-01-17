/*
 * mb_music_content.cpp
 *
 *  Created on: 2013-6-27
 *      Author: zhengxie
 */

#include "mb_music_content.h"

using namespace std;
using namespace content;

const char* MCPMBMusicInfo::ClassName = "MCPMBMusicInfo";

MCPMBMusicContent::MCPMBMusicContent(const char* key_name):AbstractMCPContent(key_name)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    // 该子类专门处理 mongoDB 中的 mcp.mcp_content 表
    this->_basic_coll = "mb_music_content";
    this->_basic_ns = MCP_MONGODB_NAME + "." + this->_basic_coll;

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

MCPMBMusicContent::~MCPMBMusicContent()
{
    LOGD("[GWJ] %s: start. recycle result_set", __FUNCTION__);
    RECYCLE_RESULT_SET(MCPMBMusicInfo)
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

int MCPMBMusicContent::set_basic_info_from_bson_result(bson* bson_out,
                                                       const char* aggregate_name)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    const char* key;
    bson_iterator it[1];
    stringstream ss;

    MCPMBMusicInfo* detail = ObjectPoolFactory<MCPMBMusicInfo>::instance().fetch_object();
    bson_iterator_init(it, bson_out);

    while(bson_iterator_next(it))
    {
        ss.clear();
        key = bson_iterator_key(it);

        if(0 == strcmp(key, "ring_id"))
        {
            detail->ring_id = bson_iterator_string(it);
            LOGG("[GWJ] %s: set ring_id[%s]",
                 __FUNCTION__, detail->ring_id.c_str());
        }
        else if(0 == strcmp(key, "name"))
        {
            detail->name = string(bson_iterator_string(it));
            LOGG("[GWJ] %s: set name[%s]",
                 __FUNCTION__, detail->name.c_str());
        }
        else if(0 == strcmp(key, "singer"))
        {
            detail->singer = bson_iterator_string(it);
            LOGG("[GWJ] %s: set singer[%s]",
                 __FUNCTION__, detail->singer.c_str());
        }
        else if(0 == strcmp(key, "bell_id"))
        {
            detail->bell_id = bson_iterator_string(it);
        }
        else if(0 == strcmp(key, "song_id"))
        {
            detail->song_id = bson_iterator_string(it);
        }
        else if(0 == strcmp(key, "cp_id"))
        {
            detail->cp_id = bson_iterator_string(it);
        }
        else if(0 == strcmp(key, "date"))
        {
            detail->date = bson_iterator_string(it);
        }
        else if(0 == strcmp(key, "dolby_flag"))
        {
            detail->dolby_flag = bson_iterator_string(it);
        }
        else if(0 == strcmp(key, "status"))
        {
            detail->status = bson_iterator_string(it);
        }
    }// while

    if(IS_SAME_VALUED_STRING(this->key_name, "ring_id"))
    {
        // 以 detail->id 作为索引将查询到的数据存储到 result_set 和 id_s 中
        FILL_RESULT_BUCKET(detail, ring_id)
    }
    else
    {
        FILL_RESULT_BUCKET(detail, ring_id)
    }

    LOGD("[GWJ] %s: end", __FUNCTION__);
    return 0;
}

