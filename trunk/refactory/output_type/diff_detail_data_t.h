/**
 * @file diff_detail_data_t.h
 * @brief 将更新信息记录输出
 * @author zhengxie
 * @version 1.0
 * @date 2013-08-03
 */

#ifndef DIFF_DETAIL_DATA_T_H 
#define DIFF_DETAIL_DATA_T_H

#include "common_header.h"
#include "../module/mcp_diff_update_info.h"

class diff_detail_data_t
{
public:
    static void set_diff_data_out(idl::game_ext_update_info_t* out,
                                      MCPDiffUpdateInfoRecord* record);
};

#endif
