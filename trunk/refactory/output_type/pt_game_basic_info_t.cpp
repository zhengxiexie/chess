/*
 * pt_game_basic_info_t.cpp
 *
 *  Created on: 2013-07-04
 *      Author: zhengxie
 */
#include "pt_game_basic_info_t.h"

void pt_game_basic_info_t::set_pt_game_basic_info_out(idl::pt_game_basic_info_t* out_parameter,
														MysqlTable& result_set,
														int row_index)
{
	int vec_count = result_set.get_record_count_by_key_index( row_index ); 
	for( int i=0; i<vec_count; i++)
	{
		const char* game_id = result_set.get_result_set_value(row_index, i, "PGBI.app_id");
		const char* name = result_set.get_result_set_value(row_index, i, "PGBI.name");
		const char* app_key = result_set.get_result_set_value(row_index, i, "PGBI.app_keys");
		const char* app_secret = result_set.get_result_set_value(row_index, i, "PGBI.app_secret");
		const char* notify_url = result_set.get_result_set_value(row_index, i, "PGBI.pay_report_addr");

		out_parameter->set_game_id( game_id );
		out_parameter->set_name(name);
		out_parameter->set_app_key(app_key);
		out_parameter->set_app_secret(app_secret);
		out_parameter->set_notify_url(notify_url);
	}
}

void pt_game_basic_info_t::set_pt_game_basic_info_record(MysqlTable& result_set,
														map<string, string>& record)
{
		const char* game_id = result_set.get_result_set_value(0, 0, "PGBI.app_id");
		const char* name = result_set.get_result_set_value(0, 0, "PGBI.name");
		const char* app_key = result_set.get_result_set_value(0, 0, "PGBI.app_keys");
		const char* app_secret = result_set.get_result_set_value(0, 0, "PGBI.app_secret");
		const char* notify_url = result_set.get_result_set_value(0, 0, "PGBI.pay_report_addr");

		//record.insert( pair<string, string>("game_id", string(game_id) ) );
		//record.insert( pair<string, string>("name", string(name) ));
		//record.insert( pair<string, string>("app_key", string(app_key) ));
		//record.insert( pair<string, string>("app_secret", string(app_secret) ));
		//record.insert( pair<string, string>("notify_url", string(notify_url) ));
		record["game_id"] = string(game_id);
	// printf("[GWJ] %s: game_id[%s]\n", __FUNCTION__, record["game_id"].c_str());
		record["name"] = string(name);
		record["app_key"] = string(app_key);
		record["app_secret"] = string(app_secret);
		record["notify_url"] = string(notify_url);
}

unsigned int pt_game_basic_info_t::set_pt_game_basic_info_out(idl::pt_game_basic_info_t* out_parameter,
											   map<string, string>& record)
{
		map<string, string>::iterator it;
		for ( it=record.begin(); it != record.end(); it++ )
		{
			if( !strcmp("game_id", (it->first).c_str() )) {
				out_parameter->set_game_id( (it->second).c_str() );
			} else if( !strcmp("name", (it->first).c_str() )) {
				out_parameter->set_name( (it->second).c_str() );
			} else if( !strcmp("app_key", (it->first).c_str()) ) {
				out_parameter->set_app_key( (it->second).c_str() );
			} else if( !strcmp("app_secret", (it->first).c_str()) ) {
				out_parameter->set_app_secret( (it->second).c_str() );
			} else if( !strcmp("notify_url", (it->first).c_str() )) {
				out_parameter->set_notify_url( (it->second).c_str() );
			}
		}
}

unsigned int pt_game_basic_info_t::set_pt_game_basic_info_out(idl::vector<idl::pt_game_basic_info_t>* out_parameter,
																   MysqlTable& result_set)
{
    unsigned int key_size = result_set.get_total_key_count();

    for(unsigned int i = 0; i < key_size; i++)
    {
		pt_game_basic_info_t::set_pt_game_basic_info_out(out_parameter->mutable_get(i),
                                       result_set, i);
    }

    return key_size;
}
