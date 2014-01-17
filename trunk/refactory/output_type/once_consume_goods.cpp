/*
 * once_consume_goods.cpp
 *
 *  Created on: 2013-5-10
 *      Author: guoweijiang
 */
#include "once_consume_goods.h"

void once_consume_goods_t::set_once_goods_out(idl::once_consume_goods_t* out_parameter,
                                              MysqlTable& result_set,
                                              int row_index)
{
    const char* goods_id = result_set.get_result_set_value(row_index, 0, "goods_id");
    const char* order_id = result_set.get_result_set_value(row_index, 0, "order_id");
    const char* consume_date = result_set.get_result_set_value(row_index, 0, "consume_date");
    const char* udid = result_set.get_result_set_value(row_index, 0, "udid");

    LOGD("[GWJ] %s: key_index[%d] goods_id[%s], order_id[%s], consume_data[%s], udid[%s]",
         __FUNCTION__, row_index, goods_id, order_id, consume_date, udid);

    out_parameter->set_itemid(goods_id);
    out_parameter->set_orderid(order_id);
    out_parameter->set_ordertime(consume_date);
    out_parameter->set_udid(udid);
}

unsigned int once_consume_goods_t::set_once_goods_out(idl::vector<idl::once_consume_goods_t>* out_parameter,
                                                      MysqlTable& result_set)
{
    unsigned int key_size = result_set.get_total_key_count();

    for(unsigned int i = 0; i < key_size; i++)
    {
        once_consume_goods_t::set_once_goods_out(out_parameter->mutable_get(i),
                                                 result_set, i);
    }

    return key_size;
}
