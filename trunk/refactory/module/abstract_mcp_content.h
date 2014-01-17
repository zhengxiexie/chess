/*
 * abstract_mcp_content.h
 *
 *  整个框架的抽象 module
 *  真正执行从 mongoDB 中获取查询数据的模块
 *  将取得的数据保存在内部的结果集中
 *
 *  Created on: 2013-4-25
 *      Author: guoweijiang
 */

#ifndef WEIGHT_CONTENT_LIST_H_
#define WEIGHT_CONTENT_LIST_H_

#include "../common.h"
#include "../decorator/abstract_decorator.h"
#include "../factory/object_pool_factory.h"

using namespace std;
using namespace content;

#define MONGO_DB_NAME "mcp"

/*
 *  专门用于把从数据库中得到的一条记录填入到  result_set 中
 *
 *  1. 如果个 result_set.key 对应多个 Record
 *     则只用第一条 Record 的 key_name.c_str() 指针作为索引
 *
 *  2. 将索引指针填入  id_s 中
 */
#define FILL_RESULT_BUCKET(detail, key_name)\
{\
    int index = CommonInterface::find_key_index(detail->key_name.c_str(), this->id_s);\
    LOGD("[GWJ] %s: current key[%s], index[%d]", __FUNCTION__, detail->key_name.c_str(), index);\
\
    if(index < 0)\
    {\
        LOGD("[GWJ] %s: 1st Record of key[%s]", __FUNCTION__, detail->key_name.c_str());\
        this->id_s.push_back(detail->key_name.c_str());\
\
        KeyBucket* vec_records = ObjectPoolFactory<KeyBucket>::instance().fetch_object();\
\
        vec_records->push_back(detail);\
\
        int size = this->id_s.size();\
        this->result_set[this->id_s[size-1]] = vec_records;\
    }\
    else\
    {\
        LOGD("[GWJ] %s: Exist Record of key[%d:%s]. list_id[size:%hu]",\
             __FUNCTION__, index, this->id_s[index], this->id_s.size());\
\
        this->result_set[this->id_s[index]]->push_back(detail);\
    }\
}

/*
 *  专门负责回收结果集中从对象池获取的对象
 */
#define RECYCLE_RESULT_SET(type_name)\
{\
    int size = this->id_s.size();\
\
    for(int i = 0; i < size; i++)\
    {\
        KeyBucket* vec_records = this->result_set[this->id_s[i]];\
\
        if(vec_records != NULL)\
        {\
            int vec_size = vec_records->size();\
\
            for(int vec_index = 0; vec_index < vec_size; vec_index++)\
            {\
                LOGFC("[GWJ] %s: Recycle Object", __FUNCTION__);\
                ObjectPoolFactory<type_name>::instance().recycle_object((type_name*)(*vec_records)[vec_index]);\
            }\
        }\
\
        LOGFC("[GWJ] %s: Recycle Vec Records", __FUNCTION__);\
        ObjectPoolFactory<KeyBucket>::instance().recycle_object(vec_records);\
\
    }\
\
    this->result_set.clear();\
}

/*
 *  一个 mongoDB 各个表结构的抽象
 *  具体结构根据表结构的不同在子类中体现
 */
class AbstractMCPRecord
{
public:

    AbstractMCPRecord()
    {
    }

    virtual void clear()
    {
    }

    virtual ~AbstractMCPRecord()
    {
    }
};

/*
 *  专门盛放拥有相同 KEY 值的记录
 *
 *  要通过对象池管理
 *  所以要实现 ClassName 和 clear 方法
 */
class KeyBucket
{
public:

    static const char* ClassName;
    vector<AbstractMCPRecord*> records;

    virtual ~KeyBucket()
    {
        this->records.clear();
    }

    void clear()
    {
        this->records.clear();
    }

    inline int size()
    {
        return this->records.size();
    }

    inline AbstractMCPRecord* operator[] (int index)
    {
        return this->records[index];
    }

    inline void push_back(AbstractMCPRecord* record)
    {
        this->records.push_back(record);
    }
};

class AbstractMCPContent
{
protected:

    mongo *_conn;

    uint32_t _unit_times;
    uint32_t _unit_num;
    uint32_t _start_pos;

    string _basic_coll;
    string _basic_ns;

    /*
     *   只保存 id 的指针，真正的值在结果集  result_set 的 AbstractMCPRecord 中
     *
     *   建立  id_s 的目的：
     *
     *   1. 保证结果集中记录的排序不被 map 破坏
     *      id_s 中的排序即是最终输出的排序
     *
     *   2. 如果业务比较复杂需要多次嵌套查询时
     *      id_s 可以作为再查询的输入参数
     *      在下一层查询中直接使用 or_decorator 装饰器修饰 id_s 即可
     *
     *   id_s 更正确的名字应该是数据索引
     *   一般来说大部分时候数据都是通过 id 来索引的
     */
    vector<const char*> id_s;

    /*
     *   map 的 KEY 也是 id 的指针，且一个 id 可能会有多条记录的情况
     *   真正 id 的值是 AbstractMCPRecord 结构体第一条记录的 id.c_str() 指针
     *
     *   【注意】:
     *   用 result_set[x] 索引一条记录时
     *   传入的 KEY 必须是  id_s 里的值
     *   不能是别的值，否则找不到对应的记录
     *   因为使用的是指针地址索引，用的地址值作为 KEY
     */
    map<const char*, KeyBucket* > result_set;

    /*
     *  设置索引的名字
     *  this->id_s 中的索引值会以这个指定的 key_name 为准
     */
    const char* key_name;

    /*
     *  抽象装饰器，根据不同的业务查询条件实现不同的子类
     *  可以让  module 变化多端的神器
     */
    AbstractDecorator* decorator;

    /*
     *  满足查询条件的记录总数，不是分页后的个数
     */
    int total_counts;

    /*
     *  用于设定从最终表中要取得的字段名
     *  不设置时默认取字段全集
     */
    vector<const char*> select_fields;

    // 转换日期类型
    string convert_mongo_ts_to_string(bson_iterator* it);

    void free_data(mongo_cursor * cursor,
                   bson* query,
                   bson* fields, bson*
                   bson_result);

    // 专门处理从 mongoDB 中取回来的数组类型数据，适用任意表
    void set_vector_data_from_bson(bson_iterator* bson_it, vector<string>& list);
    void set_vector_data_from_bson(bson_iterator* bson_it, vector<int>& list);
    void set_vector_data_from_bson(bson_iterator* bson_it, vector<double>& list);

    /*
     *  保存从 mongoDB 取得的针对 [key : value] 结构的数组，适用任何表格
     *  KEY / VALUE 都是 string 类型
     */
    void set_tuple_data_from_bson(bson_iterator* it,
                                  void* detail,
                                  bool  is_vector,
                                  char* bson_key,
                                  char* bson_value);

	void AbstractMCPContent::set_struct_data_from_bson(bson_iterator* it,
                                                  vector<MCPStructItem>* target,
                                                  vector<const char*> key_vector);
    /*
     *  保存从 mongoDB 取得的针对 [key : value] 结构的数组，适用任何表格
     *  VALUE 是 int 类型
     */
    void set_tuple_data_from_bson_integer_value(bson_iterator* it,
                                                void* tuple,
                                                bool  is_vector,
                                                char* bson_key,
                                                char* bson_value);

    /*
     *  保存从 mongoDB 取得的针对 [key : value] 结构的数组，适用任何表格
     *  VALUE 是 double 类型
     */
    void set_tuple_data_from_bson_double_value(bson_iterator* it,
                                               void* tuple,
                                               bool  is_vector,
                                               char* bson_key,
                                               char* bson_value);

    // 根据总数量调整  [_start_pos, _unit_num]
    int modify_query_boundary(int total_count);

    /*
     * 适用任意表
     * 根据装饰器设定的查询条件，得到查询列表的总数量
     * 并根据总数量调整 [_start_pos, _unit_num]
     */
    int get_list_count();

    /*
     *  一个通用接口：
     *  尽量满足任何表格
     *  根据指定的筛选条件，从装饰器指定的表中得到对应的 id_list
     */
    int get_id_list(vector<string>& id_list);

    /*
     *  一个通用接口：
     *  尽量满足任何表格
     *  根据传入的 id_list 从指定的目标表格中找到对应的全数据
     *  并将数据保存到  result_set 和 id_s 中
     *  其中  id_s 都是指向  result_set 实际数据的指针，保证最小占用内存，只有一份数据拷贝
     */
    int get_full_set_info_by_id_list(vector<string>& id_list);

    /*
     *  一个通用接口：
     *  尽量满足任何表格
     *  直接根据制定的装饰器中的条件从 mongoDB 中获取数据
     *  并将数据保存到  result_set 和 id_s 中
     *
     *  need_modify_page : 是否需要按照指定页面大小取得数据
     *                     置为 false 时则不考虑页面大小，尽量取得全部数据即可
     */
    int get_full_set_info(bool need_modify_page);

    /*
     *  抽象接口：
     *  子类来实现，根据  mongoDB 返回的表格数据全集，设置对应的 id_s 和 result_set
     *  这个接口用于保存查询到的一条 Record
     */
    virtual int set_basic_info_from_bson_result(bson* bson_out,
                                                const char* aggregate_name) = 0;

public:

    AbstractMCPContent(const char* key_name);
    virtual ~AbstractMCPContent();

    // 装载装饰器
    void load_decorator(AbstractDecorator* dec);

    /*
     *  分页设置
     */
    void set_unit_info(const uint32_t unit_num,
                       const uint32_t unit_times);

    /*
     *  返回查询条件的总数，不是分页后的个数
     *  是分页前的总数
     */
    int get_total_result_count();

    /*
     *  返回结果集的索引
     *  不是万不得已，尽量不要使用这个方法
     */
    vector<const char*>* get_result_keys();

    /*
     *  返回结果集中的索引个数
     */
    int get_key_size();

    /*
     *  获取结果集中的索引值
     */
    const char* get_key_value(int key_index);

    /*
     *  获取结果集中的索引值
     */
    const char* get_key_value(const char* key);

    /*
     *  获取同一索引下的所有记录
     */
    KeyBucket* get_key_bucket(int key_index);

    /*
     *  获取同一索引下的所有记录
     */
    KeyBucket* get_key_bucket(const char* key);

    /*
     *  获取同一索引下的所有记录个数
     */
    int get_key_bucket_size(int key_index);

    /*
     *  获取同一索引下的所有记录个数
     */
    int get_key_bucket_size(const char* key);

    /*
     *  一个通用接口：
     *  返回结果集中相同 id 的所有记录
     *  key       : 根据 key 找到属于所以此 key 的 records
     *  vec_index : 找到属于此 KEY 索引的第几条记录
     */
    AbstractMCPRecord* get_record_by_key_vec(const char* key,
                                             int vec_index);

    /*
     *  一个通用接口：
     *  返回结果集中相同 id 的所有记录
     *  key_index : 根据  key_index 找到属于所以此 key_index 的 records
     *  vec_index : 找到属于此 KEY 索引的第几条记录
     */
    AbstractMCPRecord* get_record_by_key_vec(int key_index,
                                             int vec_index);

    /*
     *  一个通用接口：
     *  尽量满足任意表查询需求
     *  根据设置的装饰器(decorator 目录)得到查询结果
     *  并把结果保存到 id_s 和 result_set 中
     *
     *  need_modify_page : 是否需要按照指定页面大小取得数据
     *                     置为 false 时则不考虑页面大小，尽量取得全部数据即可
     */
    int general_fetch_list_records(bool need_modify_page);

    /*
     *  获取装饰器设置条件的记录总数
     */
    int general_get_list_records_count();

    /*
     *  向 mongoDB 中插入数据
     */
    int general_insert_records();

    /*
     *  将 mongoDB 中的一个 collection 删除掉
     */
    int general_drop_collection();

    /*
     *  设定要筛选出的字段
     */
    void add_select_column(const char* column_name);
};

#endif /* WEIGHT_CONTENT_LIST_H_ */
