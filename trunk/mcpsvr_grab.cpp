#include "mcpsvr.h"
#include "content_type.h"
#include "content_info.h"
#include "grab.h"
#include "./refactory/rpc/notice_rpc.h"
#include "./refactory/common.h"

void setGrabInfoResult(idl::grab_info_t* out, const grab_t& info)
{
    out->set_id(info.id);
    out->set_c_id(info.c_id.c_str());
    out->set_name(info.name.c_str());
    out->set_info(info.info.c_str());
    out->set_amount(info.amount);
    out->set_send_amount(info.send_amount);
    out->set_start_time(info.start_time.c_str());
    out->set_end_time(info.end_time.c_str());
}

int McpServlet::get_grab_list_by_type(const idl::mcp_get_grab_list_by_type_params& in, idl::mcp_get_grab_list_by_type_response& out)
{
    UB_LOG_TRACE( "get_grab_list_by_type start" );
    string type_id(in.type_id());
    uint32_t page(in.page());
    uint32_t page_size(in.page_size());

    vector<grab_t> grab_list;
    int count=0;
    ContentType type(type_id);
    type.set_page_info(page, page_size);
    int res=type.getGrabList(count, grab_list);
    if(res!=0){
        UB_LOG_FATAL( "getGrabList failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_result(count);
    vector<grab_t>::const_iterator iter=grab_list.begin();
    for(int i=0; iter!=grab_list.end(); ++i, ++iter){
        setGrabInfoResult(out.m_result_params()->mutable_grab_list(i), *iter);
    }
    UB_LOG_TRACE( "get_grab_list_by_type end" );
    return 0;
}

int McpServlet::get_grab_info_by_c_id(const idl::mcp_get_grab_info_by_c_id_params& in, idl::mcp_get_grab_info_by_c_id_response& out)
{
    UB_LOG_TRACE( "get_grab_info_by_c_id start" );
    string c_id(in.c_id());
    vector<grab_t> grab_list;
    int count=0;
    ContentInfo c_info(c_id);
    int res=c_info.getGrabList(count, grab_list);
    if(res!=0){
        UB_LOG_FATAL( "getGrabList failed, [%s:%d]", __FILE__, __LINE__ );
        count = res;
        goto end;
    }
end:
    out.m_result_params()->set_result(count);
    vector<grab_t>::const_iterator iter=grab_list.begin();
    for(int i=0; iter!=grab_list.end(); ++i, ++iter){
        setGrabInfoResult(out.m_result_params()->mutable_grab_list(i), *iter);
    }
    UB_LOG_TRACE( "get_grab_info_by_c_id start" );
    return 0;
}

int McpServlet::get_grab_info_by_id(const idl::mcp_get_grab_info_by_id_params& in, idl::mcp_get_grab_info_by_id_response& out)
{
    UB_LOG_TRACE( "get_grab_info_by_id start" );
    uint32_t id(in.id());
    grab_t grab_info;
    int res(0);
    Grab grab(id);
    res = grab.getGrabInfo(&grab_info);
    if(res!=0){
        UB_LOG_FATAL( "getGrabInfo faield, [%s:%d]", __FILE__, __LINE__ );
        goto end;
    }
end:
    out.m_result_params()->set_result(res);
    setGrabInfoResult(out.m_result_params()->mutable_info(), grab_info);
    UB_LOG_TRACE( "get_grab_info_by_id end" );
    return 0;
}

int McpServlet::user_grab_num(const idl::mcp_user_grab_num_params& in, idl::mcp_user_grab_num_response& out)
{
    UB_LOG_TRACE( "user_grab_num start" );
    string user_id(in.user_id());
	string push_channelid(in.push_channelid());
	string push_userid(in.push_userid());
    uint32_t grab_id(in.grab_id());
    Grab grab(grab_id);
    string num;
    int res=grab.userGrab(user_id, num);
    if(res!=0){
        UB_LOG_FATAL( "userGrab failed, [%s:%d]", __FILE__, __LINE__ );
    }

	/*2013.05.23 zhengxie 发送成功结果给消息系统 beg */
	grab_t grab_info;
	grab.getGrabInfo(&grab_info);
	string grab_name = grab_info.name;
	grab_info.clear();
	if( (res == 0) && IS_VALUED_STRING(grab_name.c_str()) && IS_VALUED_STRING(num.c_str()) )
	{
	    NoticeRPC::instance().notice_grab( user_id.c_str(), grab_name.c_str(), num.c_str(), push_channelid.c_str(), push_userid.c_str() );
	}
	/*2013.05.23 zhengxie 发送成功结果给消息系统 end */

    out.m_result_params()->set_result(res);
    out.m_result_params()->set_num(num.c_str());
    UB_LOG_TRACE( "user_grab_num end" );
    return 0;
}

int McpServlet::user_grab_num_pre(const idl::mcp_user_grab_num_pre_params& in, idl::mcp_user_grab_num_pre_response& out)
{
    UB_LOG_TRACE( "user_grab_num start" );
    string user_id(in.user_id());
	string push_channelid(in.push_channelid());
	string push_userid(in.push_userid());
    uint32_t grab_id(in.grab_id());
    Grab grab(grab_id);
    string num;
    int res=grab.userGrabPre(user_id, num);
    if(res!=0){
        UB_LOG_FATAL( "userGrab failed, [%s:%d]", __FILE__, __LINE__ );
    }

	/*2013.05.23 zhengxie 发送成功结果给消息系统 beg */
	grab_t grab_info;
	grab.getGrabInfo(&grab_info);
	string grab_name = grab_info.name;
	grab_info.clear();
	if( (res == 0) && IS_VALUED_STRING(grab_name.c_str()) && IS_VALUED_STRING(num.c_str()) )
	{
	    NoticeRPC::instance().notice_grab( user_id.c_str(), grab_name.c_str(), num.c_str(), push_channelid.c_str(), push_userid.c_str() );
	}
	/*2013.05.23 zhengxie 发送成功结果给消息系统 end */

    out.m_result_params()->set_result(res);
    out.m_result_params()->set_num(num.c_str());
    UB_LOG_TRACE( "user_grab_num end" );
    return 0;
}

/*
int McpServlet::get_user_grab_num(const idl::mcp_get_user_grab_num_params& in, idl::mcp_get_user_grab_num_response& out)
{
    UB_LOG_TRACE( "get_user_grab_num start" );
    string user_id(in.user_id());
    uint32_t grab_id(in.grab_id());
    Grab grab(grab_id);
    string num;
    int res=grab.getUserGrab(user_id, num);
    if(res<0){
        UB_LOG_FATAL( "getUserGrab failed, [%s:%d]", __FILE__, __LINE__ );
    }else if(res>1){
        res = 1;
    }
    out.m_result_params()->set_result(res);
    out.m_result_params()->set_num(num.c_str());
    UB_LOG_TRACE( "get_user_grab_num end" );
    return 0;
}
*/

int McpServlet::get_user_grab_num(const idl::mcp_get_user_grab_num_params & in, idl::mcp_get_user_grab_num_response & out)
{
    UB_LOG_TRACE( "get_user_grab_num start" );
    string user_id(in.user_id());
    uint32_t grab_id(in.grab_id());
    Grab grab(grab_id);
    string num;
    int32_t occupy;
    int res=grab.getUserGrabEx(user_id, num, occupy);
    if(res<0){
            UB_LOG_FATAL( "getUserGrab failed, [%s:%d]", __FILE__, __LINE__ );
        }else if(res>1){
                res = 0;
            }   
    out.m_result_params()->set_result(res);
    out.m_result_params()->set_num(num.c_str());
    out.m_result_params()->set_occupy(occupy);
    UB_LOG_TRACE( "get_user_grab_num end" );
    return 0;
}
