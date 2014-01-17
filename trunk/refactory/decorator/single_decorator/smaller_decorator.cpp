/*
 * smaller_decorator.cpp
 *
 *  Created on: 2013-6-3
 *      Author: guoweijiang
 */

#include "smaller_decorator.h"

SmallerThanDecorator::SmallerThanDecorator():AbstractSingleDecorator()
{
    this->is_smaller_and_equal = false;
    this->is_special = false;
}

SmallerThanDecorator::~SmallerThanDecorator()
{
}

/*
 *  实现抽象接口：
 */
int SmallerThanDecorator::custom_result_set_query(bson* query)
{
    int size = this->fitler_conditions.size();
    LOGD("[GWJ] %s: filter_size[%d]", __FUNCTION__, size);

    if(size > 0)
    {
        const char* column_name = this->fitler_conditions[0].table_column;

        if(column_name == NULL)
        {
            LOGD("[GWJ] %s: end. Error filter column name!!", __FUNCTION__);
            return -1;
        }

        LOGD("[GWJ] %s:  start. column[%s], size[%d]", __FUNCTION__, column_name, size);
        bson_append_start_object(query, column_name);

        if(this->is_smaller_and_equal)
        {
            bson_append_string(query, "$lte", this->fitler_conditions[0].value);
            LOGD("[GWJ] %s: [%s] smaller and equal[%s]",
                 __FUNCTION__, column_name, this->fitler_conditions[0].value);
        }
        else
        {
            bson_append_string(query, "$lt", this->fitler_conditions[0].value);
            LOGD("[GWJ] %s: [%s] smaller than[%s]",
                 __FUNCTION__, column_name, this->fitler_conditions[0].value);
        }

        bson_append_finish_object(query);
    }
    else if(size == 0)
    {
        LOGD("[GWJ] %s: SmallerThanDecorator no filter Error!", __FUNCTION__);
        return -1;
    }

    LOGD("[GWJ] %s: SmallerThanDecorator end", __FUNCTION__);
    return 0;
}
