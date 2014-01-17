/*
 * pt_game_basic_info_t.h
 *
 *  Created on: 2013-07-04
 *      Author: zhengxie
 */

#ifndef PT_GAME_BASIC_INFO_T_H_
#define PT_GAME_BASIC_INFO_T_H_

#include "common_header.h"

class pt_game_basic_info_t 
{
public:
    static void set_pt_game_basic_info_out(idl::pt_game_basic_info_t* out_parameter,
                                   MysqlTable& result_set,
                                   int row_index);

    static unsigned int set_pt_game_basic_info_out(idl::vector<idl::pt_game_basic_info_t>* out_parameter,
                                           MysqlTable& result_set);
    static unsigned int set_pt_game_basic_info_out(idl::pt_game_basic_info_t* out_parameter,
												   map<string, string>& record);

	static void set_pt_game_basic_info_record(MysqlTable& result_set,
											  map<string, string>& record);
};
#endif
