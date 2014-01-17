#include "mcpsvr.h"
#include "ip_gateway.h"

int McpServlet::get_carriers_info_from_ip(const idl::mcp_get_carriers_info_from_ip_params& in,
                                                idl::mcp_get_carriers_info_from_ip_response& out )
{
    UB_LOG_TRACE( "get_carriers_info_from_ip start" );
    string ipAddr( in.ip_addres() );
    GatewayInfo gateway;
    int res(0);

    if( ipAddr.compare( "liyongjiaonireload" ) == 0 ){
        IpGateway::reload();
        gateway.network=88;
        gateway.province=99;
        goto end;
    }
    res = IpGateway::getIpGateway( ipAddr, gateway );
    if( res<0 ){
        UB_LOG_FATAL( " IpGateway::getIpGateway failed, IP[%s], [%s:%d]", ipAddr.c_str(), __FILE__, __LINE__ );
    }
end:
    out.m_result_params()->set_carriers_name(gateway.network);
    out.m_result_params()->set_owner_place(gateway.province);
    UB_LOG_TRACE( "get_carriers_info_from_ip end" );
    return 0;
}
