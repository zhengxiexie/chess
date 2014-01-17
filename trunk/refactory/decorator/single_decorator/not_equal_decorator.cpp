/*
 * not_equal_decorator.cpp
 *
 *  Created on: 2013-7-11
 *      Author: guoweijiang
 */

#include "not_equal_decorator.h"

NotEqualDecorator::NotEqualDecorator():AbstractSingleDecorator()
{
    this->is_special = false;
}

NotEqualDecorator::~NotEqualDecorator()
{
}

/*
 *  实现抽象接口：
 */
int NotEqualDecorator::custom_result_set_query(bson* query)
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

        bson_append_string(query, "$ne", this->fitler_conditions[0].value);

        LOGD("[GWJ] %s: [%s] not equal[%s]",
             __FUNCTION__, column_name, this->fitler_conditions[0].value);

        bson_append_finish_object(query);
    }
    else if(size == 0)
    {
        LOGD("[GWJ] %s: BiggerThanDecorator no filter Error!", __FUNCTION__);
        return -1;
    }

    LOGD("[GWJ] %s: BiggerThanDecorator end", __FUNCTION__);
    return 0;
}
