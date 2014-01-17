/*
 * once_consume_goods.h
 *
 *  Created on: 2013-5-9
 *      Author: guoweijiang
 */

#ifndef ONCE_CONSUME_GOODS_H_
#define ONCE_CONSUME_GOODS_H_

#include "common_header.h"

/*
 *  一次性购买物品
orderid     string  订单号
itemid      string  物品ID
ordertime   string  订单生成时间
udid        string  用户设备标识
 */
class once_consume_goods_t
{
public:
    static void set_once_goods_out(idl::once_consume_goods_t* out_parameter,
                                   MysqlTable& result_set,
                                   int row_index);

    static unsigned int set_once_goods_out(idl::vector<idl::once_consume_goods_t>* out_parameter,
                                           MysqlTable& result_set);
};

#endif /* ONCE_CONSUME_GOODS_H_ */
