/*
 * vir_currency_info.cpp
 *
 *  Created on: 2013-6-20
 *      Author: guoweijiang
 */

#include "vir_currency_info.h"

void vir_currency_info_t::set_outparameter(idl::game_dev_vir_currency_info* out,
                                           MysqlTable& records)
{
    const char* id = records.get_result_set_value(0, 0, "id");
    if(IS_VALUED_STRING(id))
    {
        out->set_id(id);
    }
    else
    {
        out->set_id("");
    }

    const char* game_id = records.get_result_set_value(0, 0, "game_id");
    if(IS_VALUED_STRING(game_id))
    {
        out->set_game_id(game_id);
    }
    else
    {
        out->set_game_id("");
    }

    const char* name = records.get_result_set_value(0, 0, "name");
    if(IS_VALUED_STRING(name))
    {
        out->set_name(name);
    }
    else
    {
        out->set_name("");
    }

    const char* exchange_rate = records.get_result_set_value(0, 0, "exchange_rate");
    if(IS_VALUED_STRING(exchange_rate))
    {
        out->set_exchage_rate(exchange_rate);
    }
    else
    {
        out->set_exchage_rate("");
    }

    const char* icon = records.get_result_set_value(0, 0, "icon");
    if(IS_VALUED_STRING(icon))
    {
        out->set_icon(icon);
    }
    else
    {
        out->set_icon("");
    }

    const char* desc = records.get_result_set_value(0, 0, "desc");
    if(IS_VALUED_STRING(desc))
    {
        out->set_desc(desc);
    }
    else
    {
        out->set_desc("");
    }
}
