/*
 * or_decorator.cpp
 *
 *  专门处理 mongoDB 中对某一个字段执行 or 逻辑的装饰器
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#include "or_decorator.h"

OrDecorator::OrDecorator():AbstractSingleDecorator()
{
    this->is_special = false;
}

OrDecorator::~OrDecorator()
{
}

/*
 *  实现抽象接口：
 */
int OrDecorator::custom_result_set_query(bson* query)
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

        char index[8];
        bson_append_start_object(query, column_name);
        bson_append_start_array(query, "$in");

        for(int i = 0; i < size; i++)
        {
            memset(index, 0x00, sizeof(index));
            sprintf(index, "%d", i);

            if(this->fitler_conditions[i].value != NULL)
            {
                if(this->fitler_conditions[i].is_string_value)
                {
                    bson_append_string(query, index,
                                       this->fitler_conditions[i].value);

                    LOGD("[GWJ] %s: set %s in [%s]", __FUNCTION__, column_name, this->fitler_conditions[i].value);
                }
                else
                {
                    int value = atoi(this->fitler_conditions[i].value);
                    bson_append_int(query, index, value);

                    LOGD("[GWJ] %s: set %s in [%d]", __FUNCTION__, column_name, value);
                }
            }
        }

        bson_append_finish_array(query);
        bson_append_finish_object(query);
    }
    else if(size == 0)
    {
        LOGD("[GWJ] %s: OrDecorator no filter Error!", __FUNCTION__);
        return -1;
    }

    LOGD("[GWJ] %s: OrDecorator end", __FUNCTION__);
    return 0;
}
