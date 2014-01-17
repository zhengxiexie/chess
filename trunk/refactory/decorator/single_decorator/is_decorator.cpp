/*
 * is_decorator.cpp
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#include "is_decorator.h"

IsDecorator::IsDecorator():AbstractSingleDecorator()
{
    this->is_special = false;
}

IsDecorator::~IsDecorator()
{
}

/*
 *  实现抽象接口：
 */
int IsDecorator::custom_result_set_query(bson* query)
{
    int size = this->fitler_conditions.size();
    LOGD("[GWJ] %s: filter_size[%d]", __FUNCTION__, size);

    if(size > 0)
    {
        for(int i = 0; i < size; i++)
        {
            if(this->fitler_conditions[i].table_column != NULL &&
               this->fitler_conditions[i].value != NULL)
            {
                if(this->fitler_conditions[i].is_string_value)
                {
                    bson_append_string(query,
                                       this->fitler_conditions[i].table_column,
                                       this->fitler_conditions[i].value);
                }
                else
                {
                    int value = atoi(this->fitler_conditions[i].value);
                    bson_append_int(query,
                                    this->fitler_conditions[i].table_column,
                                    value);
                }

                LOGD("[GWJ] %s: %s is [%s]",
                        __FUNCTION__, this->fitler_conditions[i].table_column, this->fitler_conditions[i].value);
            }
        }
    }
    else if(size == 0)
    {
        LOGD("[GWJ] %s: IsDecorator no filter Error!", __FUNCTION__);
        return -1;
    }

    LOGD("[GWJ] %s: IsDecorator end", __FUNCTION__);
    return 0;
}
