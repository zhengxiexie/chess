/*
 * orderby_decorator.cpp
 *
 *  专门处理 mongoDB 中对某几个字段执行 orderby 逻辑的装饰器
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#include "orderby_decorator.h"

OrderbyDecorator::OrderbyDecorator():AbstractSingleDecorator()
{
    this->is_special = true;
}

OrderbyDecorator::~OrderbyDecorator()
{
}

/*
 *  实现抽象接口：
 */
int OrderbyDecorator::custom_result_set_query(bson* query)
{
    int size = this->fitler_conditions.size();
    LOGD("[GWJ] %s: filter_size[%d]", __FUNCTION__, size);

    if(size > 0)
    {
        bson_append_start_object(query, "$orderby");

        for(int i = 0; i < size; i++)
        {
            if(this->fitler_conditions[i].table_column != NULL)
            {
                bson_append_int(query, this->fitler_conditions[i].table_column, this->order_type);

                LOGD("[GWJ] %s: let %s in [%d] order",
                     __FUNCTION__, this->fitler_conditions[i].table_column, this->order_type);
            }
        }

        bson_append_finish_object(query);
    }
    else if(size == 0)
    {
        LOGD("[GWJ] %s: OrderbyDecorator no filter Error!", __FUNCTION__);
        return -1;
    }

    LOGD("[GWJ] %s: OrderbyDecorator end", __FUNCTION__);
    return 0;
}
