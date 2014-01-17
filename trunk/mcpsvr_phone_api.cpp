#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "mcp_session.h"
#include "mcp.idl.mcp.h"
#include "mcpsvr.h"

using namespace std;
//#define REAL_TIME_DATA

int McpServlet::get_phone_info(const idl::mcp_get_phone_info_params& in, idl::mcp_get_phone_info_response& out)
{
    UB_LOG_NOTICE("get_phone_info is start");
    const string user_info = in.user_info();
    const uint32_t user_type = in.user_type();
    const uint32_t phone_info_type = in.phone_info_type();

    out.m_result_params()->set_phone_info(i, j , value);

}

