/*
 * abstract_compound_decorator.cpp
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#include "abstract_compound_decorator.h"

using namespace std;

AbstractCompoundDecorator::AbstractCompoundDecorator()
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    this->primary_table_query.clear();
    this->secondery_table_query.clear();

    // 组合型装饰器统一为非特殊逻辑
    this->is_special = false;

    LOGD("[GWJ] %s:  end", __FUNCTION__);
}

AbstractCompoundDecorator::~AbstractCompoundDecorator()
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    this->primary_table_query.clear();
    this->secondery_table_query.clear();

    LOGD("[GWJ] %s:  end", __FUNCTION__);
}

void AbstractCompoundDecorator::add_primary_table_decorator(AbstractDecorator* decorator)
{
    this->primary_table_query.push_back(decorator);
}

void AbstractCompoundDecorator::add_secondery_table_decorator(AbstractSingleDecorator* decorator)
{
    this->secondery_table_query.push_back(decorator);
}

/* add by zhengxie */
void AbstractCompoundDecorator::remove_primary_table_decorator(AbstractSingleDecorator* decorator)
{
	vector<AbstractDecorator*>::iterator it;
	vector<AbstractDecorator*>& vec = this->primary_table_query;
	for(it=vec.begin(); it!=vec.end(); it++)
	{
		if(*it == decorator) {
			vec.erase(it);
			break;
		}
	}
    LOGD("[GWJ] %s: after remove primary_table_query size[%d]", __FUNCTION__, (int)vec.size());
}

/* add by zhengxie */
void AbstractCompoundDecorator::remove_secondery_table_decorator(AbstractSingleDecorator* decorator)
{
	vector<AbstractDecorator*>::iterator it;
	vector<AbstractDecorator*>& vec = this->secondery_table_query;
	for(it=vec.begin(); it!=vec.end(); it++)
	{
		if(*it == decorator) {
			vec.erase(it);
			break;
		}
	}
    LOGD("[GWJ] %s: after remove secondery_table_query size[%d]", __FUNCTION__, (int)vec.size());
}

/* add by zhengxie */
void AbstractCompoundDecorator::remove_all_primary_table_decorator()
{
	this->primary_table_query.clear();
    LOGD("[GWJ] %s: after remove all primary_table_query size[%d]", __FUNCTION__, (int)(this->primary_table_query.size()));
}

/* add by zhengxie */
void AbstractCompoundDecorator::remove_all_secondery_table_decorator()
{
	this->secondery_table_query.clear();
    LOGD("[GWJ] %s: after remove all secondery_table_query size[%d]", __FUNCTION__, (int)(this->secondery_table_query.size()));
}
int AbstractCompoundDecorator::set_id_list_fetching_query(bson* query)
{
    int size = this->secondery_table_query.size();

    vector<int> basic_logic;

    // 一次查询中只允许有一个 special 逻辑
    int special_logic = -1;

    LOGD("[GWJ] %s: secondery_table_query size[%d]", __FUNCTION__, size);

    if(size == 0)
    {
        LOGD("[GWJ] %s: secondery_table_query NULL error!!", __FUNCTION__);
        return -1;
    }

    // 区分基本逻辑和 special 逻辑
    for(int i = 0; i < size; i++)
    {
        if(this->secondery_table_query[i]->is_special_query())
        {
            special_logic = i;
        }
        else
        {
            basic_logic.push_back(i);
        }
    }

    size = basic_logic.size();

    if(special_logic != -1 && size > 0)
    {
        LOGD("[GWJ] %s: Have special Logic[%d]. Begin $query", __FUNCTION__, special_logic);
        bson_append_start_object( query, "$query" );
    }

    for(int i = 0; i < size; i++)
    {
        int ret = this->secondery_table_query[i]->custom_result_set_query(query);

        if(ret < 0)
        {
            LOGD("[GWJ] %s: Logic[%d]. Empty", __FUNCTION__, i);
            return -1;
        }
    }

    if(special_logic != -1)
    {
        if(size > 0)
        {
            LOGD("[GWJ] %s: Have special Logic. End $query", __FUNCTION__);
            bson_append_finish_object(query);
        }

        this->secondery_table_query[special_logic]->custom_result_set_query(query);
    }

    return 1;
}

/*
 *  一般抽象接口：
 *
 *  由于在计算 count 时不支持 is_special_query(例如：orderby) 逻辑
 *  所以在 count 时要刨除这些逻辑，保留基本逻辑即可
 *  子类中需要根据具体业务需要，按照不同的要求进行相应的条件嵌套
 */
int AbstractCompoundDecorator::get_count_query(bson* query)
{
    int size = this->primary_table_query.size();

    vector<int> basic_logic;

    // 一次查询中只允许有一个 special 逻辑
    int special_logic = -1;

    LOGD("[GWJ] %s: primary_table_query size[%d]", __FUNCTION__, size);

    if(size == 0)
    {
        LOGD("[GWJ] %s: primary_table_query NULL error!!", __FUNCTION__);
        return -1;
    }

    // 区分基本逻辑和 special 逻辑
    for(int i = 0; i < size; i++)
    {
        if(this->primary_table_query[i]->is_special_query())
        {
            LOGD("[GWJ] %s: %d is special logic", __FUNCTION__, i);
            special_logic = i;
        }
        else
        {
            LOGD("[GWJ] %s: %d is basic logic", __FUNCTION__, i);
            basic_logic.push_back(i);
        }
    }

    size = basic_logic.size();
    LOGD("[GWJ] %s: basic logic size[%d]", __FUNCTION__, size);

    // 计算 count 时只执行基本逻辑
    for(int i = 0; i < size; i++)
    {
        int b_logic = basic_logic[i];
        LOGD("[GWJ] %s: basic logic index[%d]", __FUNCTION__, b_logic);

        int res = this->primary_table_query[b_logic]->custom_result_set_query(query);

        if(res < 0)
        {
            LOGD("[GWJ] %s: decorator [%d] error!!", __FUNCTION__, i);
            return -1;
        }
    }

    return 1;
}

/*
 *  一般抽象接口：
 *
 *  一般简单的业务中，简单的嵌套所有查询条件即可
 *  一般抽象接口的实现中，会将所有查询条件简单的嵌套在一起
 *
 *  【注意】：
 *  如果有 is_special_query 逻辑时，需要将基本逻辑用 $query 嵌套起来
 *
 *  子类中需要根据具体业务需要，按照不同的要求进行相应的条件嵌套
 */
int AbstractCompoundDecorator::custom_result_set_query(bson* query)
{
    int size = this->primary_table_query.size();

    vector<int> basic_logic;

    // 一次查询中只允许有一个 special 逻辑
    int special_logic = -1;

    LOGD("[GWJ] %s: primary_table_query size[%d]", __FUNCTION__, size);

    if(size == 0)
    {
        LOGD("[GWJ] %s: primary_table_query NULL error!!", __FUNCTION__);
        return -1;
    }

    // 区分基本逻辑和 special 逻辑
    for(int i = 0; i < size; i++)
    {
        if(this->primary_table_query[i]->is_special_query())
        {
            special_logic = i;
        }
        else
        {
            basic_logic.push_back(i);
        }
    }

    size = basic_logic.size();

    if(special_logic != -1 && size > 0)
    {
        LOGD("[GWJ] %s: Have special Logic[%d]. Begin $query", __FUNCTION__, special_logic);
        bson_append_start_object( query, "$query" );
    }

    for(int i = 0; i < size; i++)
    {
        int b_logic = basic_logic[i];
        LOGD("[GWJ] %s: basic logic index[%d]", __FUNCTION__, b_logic);

        int ret = this->primary_table_query[b_logic]->custom_result_set_query(query);

        if(ret < 0)
        {
            LOGD("[GWJ] %s: Logic[%d]. Empty", __FUNCTION__, i);
            return -1;
        }
    }

    if(special_logic != -1)
    {
        if(size > 0)
        {
            LOGD("[GWJ] %s: Have special Logic. End $query", __FUNCTION__);
            bson_append_finish_object(query);
        }

        this->primary_table_query[special_logic]->custom_result_set_query(query);
    }

//	bson_print(query);
    return 1;
}
