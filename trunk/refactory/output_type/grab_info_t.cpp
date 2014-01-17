/*
 * grab_info_t.cpp
 *
 *  Created on: 2013-5-14
 *      Author: zhengxie
 */
#include "grab_info_t.h"
#include "../../grab.h"

void grab_info_t::set_grab_info_out(idl::grab_info_t* out_parameter,
                                    MysqlTable& result_set,
                                    int row_index, uint32_t flag)
{
	int vec_count = result_set.get_record_count_by_key_index( row_index ); 
	for( int i=0; i<vec_count; i++)
	{
		const char* id = result_set.get_result_set_value(row_index, i, "GRB.id");
		const char* name = result_set.get_result_set_value(row_index, i, "GRB.name");
		const char* c_id = result_set.get_result_set_value(row_index, i, "GRB.c_id");
		const char* info = result_set.get_result_set_value(row_index, i, "GRB.info");
		const char* start_time = result_set.get_result_set_value(row_index, i, "GRB.start_time");
		const char* end_time = result_set.get_result_set_value(row_index, i, "GRB.end_time");
		const char* amount = result_set.get_result_set_value(row_index, i, "GRB.amount");
		const char* creat_time = result_set.get_result_set_value(row_index, i, "GRB.create_time");
		const char* num = NULL;
		if(1 == flag){
			num = result_set.get_result_set_value(row_index, i, "CUGRB.num");
			out_parameter->set_num( num );
		}
		LOGD("[GWJ] %s: id[%d] name[%s], c_id[%s], info[%s]",
			 __FUNCTION__, id, name, c_id, info);

		out_parameter->set_id( atoi(id) );
		out_parameter->set_name(name);
		out_parameter->set_c_id(c_id);
		out_parameter->set_info(info);
		out_parameter->set_create_time(creat_time);
		out_parameter->set_start_time(start_time);
		out_parameter->set_end_time(end_time);
		out_parameter->set_amount( atoi(amount) );
		Grab grab(atoi(id));
		int send_amount = grab.getSendAmount();
		out_parameter->set_send_amount( send_amount );
	}
}

unsigned int grab_info_t::set_grab_info_out(idl::vector<idl::grab_info_t>* out_parameter,
                                            MysqlTable& result_set, uint32_t flag)
{
    unsigned int key_size = result_set.get_total_key_count();

    for(unsigned int i = 0; i < key_size; i++)
    {
        grab_info_t::set_grab_info_out(out_parameter->mutable_get(i),
                                       result_set, i, flag);
    }

    return key_size;
}
