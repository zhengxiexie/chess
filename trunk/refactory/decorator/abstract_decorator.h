/*
 * abstract_decorator.h
 *
 * 一套专门用于设置 bson_query 的接口
 * 根据具体业务条件实现子类，设置查询条件，修改 bson_query
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#ifndef ABSTRACTDECORATOR_H_
#define ABSTRACTDECORATOR_H_

#include "../common.h"

using namespace std;

enum WeightType
{
    // H5，游戏下载站中使用的一般权重
    CLASS_WEIGHT_TYPE = 0,
    LATEST_WEIGHT_TYPE,
    DOWNLOAD_WEIGHT_TYPE,
    POP_WEIGHT_TYPE,
    NO_WEIGH_TYPE,

    // 专门为游戏大厅预留的权重
    CLIENT_CLASS_WEIGHT_TYPE,
    CLIENT_LATEST_WEIGHT_TYPE,
    CLIENT_DOWNLOAD_WEIGHT_TYPE,
    CLIENT_POP_WEIGHT_TYPE,
    CLIENT_WEEKLY_DOWNLOAD_TYPE,

    // 按照 column.p 排序
    COLUMN_PRIORIY,

    // 按照 class.p 排序
    CLASS_PRIORIY,
	// 安装当天的下载量排序
	DOWNLOAD_DAY_WEIGHT_TYPE
};

class AbstractDecorator
{
protected:

    /*
     *  在 MongoDB 中一些语句必须前面用 $query
     *  例如 $orderby
     *  参见：http://docs.mongodb.org/manual/reference/operator/orderby/#op._S_orderby
     */
    bool is_special;

    /*
     *  判断是否是一个超级装饰器
     *  如果是超级装饰器，则所有的需求都要在超级装饰器里执行
     *  并且超级装饰器在执行时要使用 mongo_run_command 方法，而不是 mongo_find 方法
     */
    bool is_aggregate;


    /*
     *  如果是一个超级装饰器，要表示装饰的字段名称
     */
    const char* extra_name;

    /*
     *  Aggregate 装饰器需要知道分页信息
     */
    uint32_t start_pos;
    uint32_t page_size;

public:

    /*
     * 与  need_get_id_list 接口一起用, 保存多表查询时的附表名
     * 记住此项一定要加  mcp. 前缀, 构成 mcp.mcp_xxx_xxx 的形式
     */
    const char* secondary_table_name;

    /*
     *  当涉及到嵌套联表查询时，从附表中查询的 KEY 名有可能不叫 c_id
     *  根据需要设定一下，默认是 "c_id"
     */
    const char* secondary_query_key_name;

    /*
     *  当涉及到嵌套联表查询时，从主表中查询的 KEY 名有可能不叫 id
     *  根据需要设定一下，默认是 "id"
     */
    const char* primary_query_key_name;

    AbstractDecorator();
    virtual ~AbstractDecorator();

    /*
     *  由于在 mongoDB 中目标表格可能没有要查询筛选的 xxx_id，
     *  这时就需要从  secondary_table_name 表格中查询 xxx_id 对应的 c_id
     *  然后再根据   c_id 查询目标表格
     *  这时就需要一个判断接口，判断是否需要先查 id_list
     */
    bool need_get_id_list();

    /*
     * 根据设置筛选的 xxx_id 设置查询语句
     * 筛选的类型包括 filter_classes ， filter_columns ， filter_labels 等
     */
    void set_filter_query(vector<const char*>& filter_list,
                          bson* query,
                          const char* filter_log_ptr,
                          const char* bson_filter);


    /*
     *  一个通用接口：
     *  根据已有的 id_list 构建查询筛选 id 的条件
     *  这个接口专门用于通过从附表中找到的 id_list 查最终表
     */
    void set_basic_info_query_by_id_list(vector<string>& id_list, bson* query);

    /*
     *  一个通用接口：
     *  根据传入的 select_fields 设置筛选的域，适用任意表
     */
    void set_fields_bson(bson* query, vector<const char*>& select_fields);

    bool is_special_query();

    bool is_aggregate_query();
    const char* get_aggregate_extra_name();

    void set_page_info(const uint32_t start_pos,
                       const uint32_t page_size);

    /*
     *  一般抽象接口：
     *  默认执行子类的  custom_result_set_query 即可
     *
     *  其实很多情况与 custom_result_set_query 不同
     *  set_id_list_fetching_query 设置的是从  secondary_table_name 副表中查询的条件
     *  custom_result_set_query 设置的就是从最终目标表中查询的条件
     */
    virtual int set_id_list_fetching_query(bson* query);

    /*
     *  一般抽象接口：
     *
     *  对于 single decorator 来说直接调用核心方法即可
     *  在 compound decorator 中会重写此虚函数
     */
    virtual int get_count_query(bson* query);

    /*
     *  抽象接口：
     *  用于装饰指定业务查询的条件，查询最终目标表，是装饰器的核心接口
     */
    virtual int custom_result_set_query(bson* query) = 0;
};


#endif /* ABSTRACTDECORATOR_H_ */
