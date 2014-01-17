/*
 * column_info_t.h
 *
 *  Created on: 2013-6-6
 *      Author: guoweijiang
 */

#ifndef COLUMN_INFO_T_H_
#define COLUMN_INFO_T_H_

#include "common_header.h"
#include "../module/mcp_content_diy_column.h"

class diy_column_info_t
{
public:
    static void set_outparameter(idl::column_info_t* out,
                                 MCPDIYColumnRecord* diy_record);
};

#endif /* COLUMN_INFO_T_H_ */
