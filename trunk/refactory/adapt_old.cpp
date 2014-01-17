/*
 * adapt_old.cpp
 *
 *  Created on: 2013-8-27
 *      Author: guoweijiang
 */
#include "adapt_old.h"

bool AdaptOldMcpInterface::is_enable_game(const char* c_id)
{
    LOGD("[GWJ] %s: check c_id[%s]", __FUNCTION__, c_id);

    IsDecorator is_dec;
    is_dec.add_field_filter_condition("id", c_id);

    MCPGameContent mcp_content("id");
    mcp_content.add_select_column("id");
    mcp_content.load_decorator(&is_dec);
    mcp_content.general_fetch_list_records(false);

    int k_size = mcp_content.get_key_size();

    if(k_size == 0)
    {
        LOGD("[GWJ] %s: k_size[%s]. c_id[%s] Not Exist!", __FUNCTION__, k_size, c_id);
        return false;
    }
    else
    {
        LOGD("[GWJ] %s: k_size[%s]. c_id[%s] Exist!", __FUNCTION__, k_size, c_id);
        return true;
    }
}
