/*
 * bigger_decorator.cpp
 *
 *  Created on: 2013-5-31
 *      Author: guoweijiang
 */

#include "bigger_decorator_int.h"

BiggerThanIntDecorator::BiggerThanIntDecorator():AbstractSingleDecorator()
{
    this->is_bigger_and_equal = false;
    this->is_special = false;
}

BiggerThanIntDecorator::~BiggerThanIntDecorator()
{
}

/*
 *  实现抽象接口：
 */
int BiggerThanIntDecorator::custom_result_set_query(bson* query)
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

        if(this->is_bigger_and_equal)
        {
			int v = atoi(this->fitler_conditions[0].value);
            //bson_append_int(query, "$gte", this->fitler_conditions[0].value);
			bson_append_int(query, "$gte", v);
            LOGD("[GWJ] %s: [%s] bigger and equal[%s]",
                 __FUNCTION__, column_name, this->fitler_conditions[0].value);
        }
        else
        {
			int v = atoi(this->fitler_conditions[0].value);
            //bson_append_int(query, "$gt", this->fitler_conditions[0].value);
			bson_append_int(query, "$gt", v);
            LOGD("[GWJ] %s: [%s] bigger than[%s]",
                 __FUNCTION__, column_name, this->fitler_conditions[0].value);
        }

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
