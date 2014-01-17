/*
 * abstract_decorator.cpp
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#include "abstract_decorator.h"

using namespace std;
using namespace content;

AbstractDecorator::AbstractDecorator()
{
    this->secondary_table_name = NULL;

    // 从附表中查询结果一般需要 "c_id" 即可
    this->secondary_query_key_name = "c_id";

    // 从主表中查询结果一般关键字名叫 "id"
    this->primary_query_key_name = "id";

    // 默认不是 aggregate 类型装饰器
    this->is_aggregate = false;

    // 默认没有 aggregate 处理的特殊字段名
    this->extra_name = NULL;
}

AbstractDecorator::~AbstractDecorator()
{
}

/*
 *  由于在 mongoDB 中目标表格可能没有要查询筛选的 xxx_id，
 *  这时就需要从  secondary_table_name 表格中查询 xxx_id 对应的 c_id
 *  然后再根据   c_id 查询目标表格
 *  这时就需要一个判断接口，判断是否需要先查 id_list
 */
bool AbstractDecorator::need_get_id_list()
{
    // 判断是否设置了附表名，来判断是否需要嵌套查询
    if(this->secondary_table_name == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
 * 根据设置筛选的 xxx_id 设置查询语句
 * 筛选的类型包括 filter_classes ， filter_columns ， filter_labels 等
 */
void AbstractDecorator::set_filter_query(vector<const char*>& filter_list,
                                         bson* query,
                                         const char* filter_log_ptr,
                                         const char* bson_filter)
{
    int size = filter_list.size();
    LOGD("[GWJ] %s:  start. %s_size[%d]", __FUNCTION__, filter_log_ptr, size);

    if(size > 0)
    {
        char index[8];
        bson_append_start_object(query, bson_filter);
        bson_append_start_array(query, "$in");

        for(int i = 0; i < size; i++)
        {
            memset(index, 0x00, sizeof(index));
            sprintf(index, "%d", i);

            bson_append_string(query, index, filter_list[i]);
            LOGD("[GWJ] %s: set %s[%s]", __FUNCTION__, bson_filter, filter_list[i]);
        }

        bson_append_finish_array(query);
        bson_append_finish_object(query);
    }
    else if(size == 0)
    {
        LOGD("[GWJ] %s: %s no filter", __FUNCTION__, filter_log_ptr);
    }
}

/*
 *  一个通用接口：
 *  根据已有的 id_list 构建查询筛选 id 的条件
 *  这个接口专门用于通过从附表中找到的 id_list 查最终表
 */
void AbstractDecorator::set_basic_info_query_by_id_list(vector<string>& id_list,
                                                        bson* query)
{
    LOGD("[GWJ] %s:  start. id_list size[%d]",
         __FUNCTION__, id_list.size());

    char index[8];
    bson_append_start_object(query, this->primary_query_key_name);
    bson_append_start_array(query, "$in");

    vector<string>::const_iterator it = id_list.begin();

    for(int i = 0; it != id_list.end(); ++it, ++i)
    {
        memset(index, 0x00, sizeof(index));
        sprintf(index, "%d", i);
        bson_append_string(query, index, it->c_str());
        LOGD("[GWJ] %s:  push id[%s]", __FUNCTION__, it->c_str());
    }

    bson_append_finish_array(query);
    bson_append_finish_object(query);
    LOGD("[GWJ] %s:  end", __FUNCTION__);
    return;
}

/*
 *  一个通用接口：
 *  根据传入的 select_fields 设置筛选的域，适用任意表
 */
void AbstractDecorator::set_fields_bson(bson* query, vector<const char*>& select_fields)
{
    LOGD("[GWJ] %s:  start. select_fields size[%d]",
             __FUNCTION__, select_fields.size());

    bson_init(query);

    int size = select_fields.size();

    for(int i = 0; i < size; i++)
    {
        if(select_fields[i] != NULL)
        {
            bson_append_int(query, select_fields[i], 1);
            LOGD("[GWJ] %s:  select[%s]", __FUNCTION__, select_fields[i]);
        }
    }

    bson_finish(query);

    LOGD("[GWJ] %s:  end", __FUNCTION__);
    return;
}

/*
 *  实现一般抽象接口：
 *  默认执行子类的  custom_result_set_query 即可
 *
 *  很多情况与 custom_result_set_query 不同
 *  set_id_list_fetching_query 设置的是从  secondary_table_name 副表中查询的条件
 *  custom_result_set_query 设置的就是从最终目标表中查询的条件
 */
int AbstractDecorator::set_id_list_fetching_query(bson* query)
{
    // 暂时默认为  custom_result_set_query , 具体差异在子类中再体现
    return this->custom_result_set_query(query);
}

/*
 *  一般抽象接口：
 *
 *  对于 single decorator 来说直接调用核心方法即可
 *  在 compound decorator 中会重写此虚函数
 */
int AbstractDecorator::get_count_query(bson* query)
{
    return this->custom_result_set_query(query);
}

bool AbstractDecorator::is_special_query()
{
    return this->is_special;
}

bool AbstractDecorator::is_aggregate_query()
{
    return this->is_aggregate;
}

const char* AbstractDecorator::get_aggregate_extra_name()
{
    LOGD("[GWJ] %s: AggregateColumn[%s]",
         __FUNCTION__, this->extra_name);

    return this->extra_name;
}

void AbstractDecorator::set_page_info(const uint32_t start_pos,
                                      const uint32_t page_size)
{
    LOGD("[GWJ] %s: Aggregate Page[start_pos:%d, size:%d]",
         __FUNCTION__, start_pos, page_size);

    this->start_pos = start_pos;
    this->page_size = page_size;
}
