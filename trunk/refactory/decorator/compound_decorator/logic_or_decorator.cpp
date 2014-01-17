/*
 * logic_or_decorator.cpp
 *
 *  Created on: 2014-1-16
 *      Author: guoweijiang
 */

#include "logic_or_decorator.h"

LogicOrDecorator::LogicOrDecorator()
{
    LOGD("[TAG] %s:  start", __FUNCTION__);

    this->primary_table_query.clear();

    // 组合型装饰器统一为非特殊逻辑
    this->is_special = false;
    //this->decorator_type = LOGIC_OR_CONDITION;

    LOGD("[TAG] %s:  end", __FUNCTION__);
}

LogicOrDecorator::~LogicOrDecorator()
{
    LOGD("[TAG] %s:  start", __FUNCTION__);
	this->primary_table_query.clear();
	this->secondery_table_query.clear();
    //this->clear();
    LOGD("[TAG] %s:  end", __FUNCTION__);
}

int LogicOrDecorator::custom_result_set_query(bson* query)
{
    int size = this->primary_table_query.size();

    std::vector<int> basic_logic;

    // 一次查询中只允许有一个 special 逻辑
    int special_logic = -1;

    LOGD("[TAG] %s: primary_table_query size[%d]", __FUNCTION__, size);

    if(size == 0)
    {
        LOGD("[TAG] %s: primary_table_query NULL error!!", __FUNCTION__);
        return -1;
    }

    // 区分基本逻辑和 special 逻辑
    for(int i = 0; i < size; i++)
    {
        if(this->primary_table_query[i]->is_special_query())
        {
            LOGD("[TAG] %s: %d is special logic", __FUNCTION__, i);
            special_logic = i;
        }
        else
        {
            LOGD("[TAG] %s: %d is basic logic", __FUNCTION__, i);
            basic_logic.push_back(i);
        }
    }

    size = basic_logic.size();
    LOGD("[TAG] %s: basic logic size[%d]", __FUNCTION__, size);

/*    bson_append_start_object(query, "$or");*/

    //// 计算 count 时只执行基本逻辑
    //for(int i = 0; i < size; i++)
    //{
        //int b_logic = basic_logic[i];
        //LOGD("[TAG] %s: basic logic index[%d]", __FUNCTION__, b_logic);

        //int res = this->primary_table_query[b_logic]->custom_result_set_query(query);

        //if(res < 0)
        //{
            //LOGD("[TAG] %s: decorator [%d] error!!", __FUNCTION__, i);
            //return -1;
        //}
    //}

    //bson_append_finish_object(query);

	bson_append_start_array(query, "$or");

	//计算 count 时只执行基本逻辑
	for(int i = 0; i < size; i++)
	{
		int b_logic = basic_logic[i];
		LOGD("[TAG] %s: basic logic index[%d]", __FUNCTION__, b_logic);

		char i_str[8];
		memset(i_str, 0x00, sizeof(i_str));
		snprintf(i_str, sizeof(i_str), "%d", i);
		bson_append_start_object(query, i_str);
		int res = this->primary_table_query[b_logic]->custom_result_set_query(query);
		bson_append_finish_object(query);

		if(res < 0)
		{
			LOGD("[TAG] %s: decorator [%d] error!!", __FUNCTION__, i);
			return -1;
		}
	}

	bson_append_finish_array(query);
	//bson_print(query);

    return 1;
}

int LogicOrDecorator::get_count_query(bson* query)
{
    return this->custom_result_set_query(query);
}
