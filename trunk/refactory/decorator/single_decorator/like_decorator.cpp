/*
 * like_decorator.cpp
 *
 *  Created on: 2013-7-4
 *      Author: guoweijiang
 */

#include "like_decorator.h"

LikeDecorator::LikeDecorator():AbstractSingleDecorator()
{
    this->is_special = false;
}

LikeDecorator::~LikeDecorator()
{
}

/*
 *  实现抽象接口：
 */
int LikeDecorator::custom_result_set_query(bson* query)
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
                bson_append_regex(query,
                                  this->fitler_conditions[i].table_column,
                                  this->fitler_conditions[i].value,
                                  "x");

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
