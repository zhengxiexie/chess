/*
 * user_grab_number_t.cpp
 *
 *  Created on: 2013-5-23
 *      Author: zhengxie
 */
#include "user_grab_number_t.h"

void user_grab_number_t::set_user_grab_number_out(idl::user_grab_number_t* out_parameter,
                                   MysqlTable& result_set,
                                   int row_index)
{
	int vec_count = result_set.get_record_count_by_key_index( row_index ); 
	for( int i=0; i<vec_count; i++)
	{
		const char* grab_id = result_set.get_result_set_value(row_index, i, "CUGRB.grab_id");
		const char* num = result_set.get_result_set_value(row_index, i, "CUGRB.num");
		const char* c_id = result_set.get_result_set_value(row_index, i, "GRB.c_id");
		const char* name = result_set.get_result_set_value(row_index, i, "GRB.name");
		const char* start_time = result_set.get_result_set_value(row_index, i, "GRB.start_time");
		const char* end_time = result_set.get_result_set_value(row_index, i, "GRB.end_time");
		const char* info = result_set.get_result_set_value(row_index, i, "GRB.info");
		const char* creat_time = result_set.get_result_set_value(row_index, i, "GRB.create_time");

		LOGD("[GWJ] %s: grab_id[%d] num[%s]",
			 __FUNCTION__, grab_id, num);

		out_parameter->set_grab_id( atoi(grab_id) );
		out_parameter->set_num(num);
		out_parameter->set_c_id(c_id);
		out_parameter->set_name(name);
		out_parameter->set_create_time(creat_time);
		out_parameter->set_start_time(start_time);
		out_parameter->set_end_time(end_time);
		out_parameter->set_info(info);
	}
}

unsigned int user_grab_number_t::set_user_grab_number_out(idl::vector<idl::user_grab_number_t>* out_parameter,
                                           MysqlTable& result_set)
{
    unsigned int key_size = result_set.get_total_key_count();

    for(unsigned int i = 0; i < key_size; i++)
    {
        user_grab_number_t::set_user_grab_number_out(out_parameter->mutable_get(i),
                                       result_set, i);
    }

    return key_size;
}
