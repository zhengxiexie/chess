/*
 * user_grab_number_t.h
 *
 *  Created on: 2013-5-23
 *      Author: zhengxie
 */

#ifndef USER_GRAB_NUMBER_T
#define USER_GRAB_NUMBER_T


#include "common_header.h"

class user_grab_number_t
{
public:
    static void set_user_grab_number_out(idl::user_grab_number_t* out_parameter,
                                   MysqlTable& result_set,
                                   int row_index);

    static unsigned int set_user_grab_number_out(idl::vector<idl::user_grab_number_t>* out_parameter,
                                           MysqlTable& result_set);
};
#endif
