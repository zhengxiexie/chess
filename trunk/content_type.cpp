#include "content_type.h"
#include "ub_server.h"
#include "duokoo_mysql.h"
#include "grab.h"

using namespace content;
using namespace std;

int ContentType::getTopicList(int32_t& count, vector<topic_info_t>& topic_list)
{
    UB_LOG_TRACE("getTopicList start");
    count = (int)mcp_content_map::m_type_topic_list[_id].size();
    UB_LOG_DEBUG("count[%d],id[%s]", count, _id.c_str() );
    if((int)_start_pos>=count){
        return 0;
    }
    topic_info_t topic_info;
    UB_LOG_DEBUG("start_pos[%d],count[%d],size[%d]",_start_pos,count,_page_size);
    for(uint32_t i=_start_pos; (int)i<count&&i<_start_pos+_page_size; ++i ){
        topic_info.clear();
        mcp_content_map::get_topic_info(mcp_content_map::m_type_topic_list[_id][i], topic_info);
        topic_list.push_back(topic_info);
        UB_LOG_DEBUG("i[%d],id[%s]", i,topic_info.id.c_str());
    }
    UB_LOG_TRACE("getTopicList end");
    return 0;
}

int ContentType::getGrabList(int32_t& count, vector<grab_t>& grab_list)
{
    UB_LOG_TRACE( "ContentType::getGrabList start" );
    string query_sql("SELECT `id`, `name`, `c_id`, `info`, `start_time`, `end_time`, `amount`, `probability` "
                     "FROM `MCP`.`mcp_content_grab` "
                     "WHERE `content_type` = " + _id + " AND `enable` = 1 "
                     "ORDER BY `start_time` DESC " );
    UB_LOG_DEBUG("%s",query_sql.c_str());
    DuokooMysql mysql("grab_mysql");
    count=mysql.query(query_sql);
    if(count<0){
        UB_LOG_FATAL( "sql[%s] [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
        return -5;
    }
    grab_list.clear();
    grab_t grab_info;
    for(int i=_start_pos; i<count&&i-_start_pos<_page_size; ++i){
        grab_info.clear();
        grab_info.id=atoi(mysql.getResult(i,0).c_str());
        grab_info.name=mysql.getResult(i,1);
        grab_info.c_id=mysql.getResult(i,2);
        grab_info.info=mysql.getResult(i,3);
        grab_info.start_time=mysql.getResult(i,4);
        grab_info.end_time=mysql.getResult(i,5);
        grab_info.amount=atoi(mysql.getResult(i,6).c_str());
        grab_info.probability=atof(mysql.getResult(i,7).c_str());
        Grab grab(grab_info.id);
        grab_info.send_amount=grab.getSendAmount();
        grab_list.push_back(grab_info);
    }
    UB_LOG_TRACE( "ContentType::getGrabList end" );
    return 0;
}
