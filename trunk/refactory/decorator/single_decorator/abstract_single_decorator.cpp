/*
 * abstract_single_decorator.cpp
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#include "abstract_single_decorator.h"

using namespace std;

AbstractSingleDecorator::AbstractSingleDecorator():AbstractDecorator()
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);
    this->weight_type = NO_WEIGH_TYPE;
    this->fitler_conditions.clear();
    LOGD("[GWJ] %s:  end", __FUNCTION__);
}

AbstractSingleDecorator::~AbstractSingleDecorator()
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);
    this->fitler_conditions.clear();
    LOGD("[GWJ] %s:  end", __FUNCTION__);
}

void AbstractSingleDecorator::push_back(MCPTupleDecoratorItem& decorator)
{
    this->fitler_conditions.push_back(decorator);
}

void AbstractSingleDecorator::add_field_filter_condition(const char* key_name,
                                                         const char* key_value)
{
    MCPTupleDecoratorItem tuple_item;
    tuple_item.is_string_value = true;
    tuple_item.table_column = key_name;
    tuple_item.value = key_value;

    this->fitler_conditions.push_back(tuple_item);

    LOGD("[GWJ] %s: add key[%s],value[%s]",
         __FUNCTION__, tuple_item.table_column, tuple_item.value);
}

/*
 *  由于   this->fitler_conditions 中只保存指针
 *  所以传入的  value_entity 保存  key_value 的 string 类型实体
 *  this->fitler_conditions 指向这个实体
 */
void AbstractSingleDecorator::add_field_filter_condition(const char* key_name,
                                                         const int key_value,  // 参与判断的值
                                                         string& value_entity) // 判断值转换成 string 类型的实体
{
    char value[8];
    memset(value, 0x00, sizeof(value));
    sprintf(value, "%d", key_value);
    value_entity = value;

    MCPTupleDecoratorItem tuple_item;
    tuple_item.is_string_value = false;
    tuple_item.table_column = key_name;
    tuple_item.value = value_entity.c_str();

    this->fitler_conditions.push_back(tuple_item);

    LOGD("[GWJ] %s: add key[%s],value[%s]",
         __FUNCTION__, tuple_item.table_column, tuple_item.value);
}

/*
 *  从一个已知的vector列表中获取查询条件
 */
int AbstractSingleDecorator::set_same_field_filter_conditions(vector<const char*>* condition_list, // 已知条件数值
                                                              const char* table_field_name,        // 参与数组判断的列名
                                                              bool is_string_value)                // 参与判别的值是否为 string 类型
{
    if(condition_list == NULL)
    {
        LOGD("[GWJ] %s: NULL condition list Error!", __FUNCTION__);
        return 0;
    }

    int size = condition_list->size();
    LOGD("[GWJ] %s:  start. con_size[%d], column_name[%s]",
         __FUNCTION__, size, table_field_name);

    if(size == 0)
    {
        return 0;
    }

    for(int i = 0; i < size; i++)
    {
        MCPTupleDecoratorItem tuple;
        tuple.table_column = table_field_name;
        tuple.is_string_value = is_string_value;
        tuple.value = (*condition_list)[i];
        this->fitler_conditions.push_back(tuple);

        LOGD("[GWJ] %s: set %s[%s]",
                 __FUNCTION__, table_field_name, (*condition_list)[i]);
    }

    return 1;
}
