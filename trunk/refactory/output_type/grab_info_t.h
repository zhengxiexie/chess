/*
 * grab_info_t.h
 *
 *  Created on: 2013-5-14
 *      Author: zhengxie
 */

#ifndef GRAB_INFO_T_H_
#define GRAB_INFO_T_H_

#include "common_header.h"

class grab_info_t
{
public:
    static void set_grab_info_out(idl::grab_info_t* out_parameter,
                                   MysqlTable& result_set,
                                   int row_index, uint32_t flag);

    static unsigned int set_grab_info_out(idl::vector<idl::grab_info_t>* out_parameter,
                                           MysqlTable& result_set, uint32_t flag);
};
#endif
