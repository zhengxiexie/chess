/*
 * mysql_general_module.h
 *
 *  Created on: 2013-5-3
 *      Author: guoweijiang
 */

#ifndef MYSQL_GENERAL_MODULE_H_
#define MYSQL_GENERAL_MODULE_H_

#include "../common.h"
#include "extend_column.h"
#include "../../duokoo_mysql.h"

class MysqlTable;

/*
 *  从 MySQL 返回的数据都是一维数据，不存在嵌套关系
 *  是利用接口  DuokooMysql::getResult 只返回 string 类型而设计的
 */
class MySQLRecord
{
friend class MysqlTable;

    public:/* TODO Only for TEST */
    /*
     *  KEY 指向 select_fields 中的项
     */
    map<const char*, string> record;
};

class MysqlTable
{

// 定义 ExMysqlColumn 为 MysqlTable 的友元类
friend class ExMysqlColumn;

protected:

    /*
     *  【注意】
     *
     *  key_name 必须指向  select_fields 中的某个成员变量
     *  而不是其它的地方，即便是相同的字符串
     */
    const char* key_name;

    /*
     *  专门用于 SQL 语句中参与 where / orderby / groupby 等判断和  join 等"大"操作的列
     */
    vector<ExMysqlColumn> extend_columns;

    /*
     *  与此 Table 表 JOIN 到一起的表队列
     */
    vector<MysqlTable*> joint_tables;

    /*
     *  执行 INSERT / UPDATE 操作时的列和值
     */
    vector<MCPTupleItem> insert_update_conditions;
    vector<string> insert_transaction_sqls;
    vector<string> update_transaction_sqls;

    // 真实表名
    const char* table_name;

    // 表的匿名，可以为空
    const char* nick_name;

    /*
     *  专门用于 SQL 语句中 select 部分
     *  与 extent_columns 作用不同
     *
     *  由于 select column 有可能有拼接操作
     *  所以此处保存 string 实体，而不是 const char*
     */
    vector<string> select_fields;

    /*
     *  保存的是 result_set 结果集中  MySQLRecord 实体的指针
     *  是索引结果集的  KEY
     */
    vector<const char*> id_s;

    /*
     *  结果集的 [KEY] 与 id_s 中的指针都指向同一个位置，即  MySQLRecord 中 key_name.c_str()
     */
    map<const char*, vector<MySQLRecord> > result_set;

    /*
     *  支持分页显示
     */
    uint32_t _unit_times;
    uint32_t _unit_num;
    uint32_t _start_pos;

    /*
     *  元素型 SQL 语句
     */
    string select_query;
    bool has_select_column;

    string from_query;

    string join_query;
    bool has_join;

    string groupby_query;
    bool has_groupby;

    string orderby_query;
    bool has_orderby;

    string where_query;
    bool has_where;

    string limited_query;
    bool has_limited;

    /*
     *  按照条件的结果总数，分页前总数
     */
    int total_results;

    /*
     *  检查一个列名是否已经是  nick.column 的形式
     */
    bool is_had_nick_name(const char* column);

    /*
     *  只是获取 SQL 语句中的  SELECT 段
     *  即要选出的所有列名
     */
    bool get_select_query(string& query);

    /*
     *  获取表中的 WHERE 语句
     *  如果有 JOIN 联合表格，则 JOIN 表的 WHERE 语句也在外层
     *  不在 JOIN 语句中，即先进行 JOIN 然后对 JOIN 后的结果进行 WHERE
     */
    bool get_where_query(string& query);

    /*
     *  获取当前主表的 WHERE 语句
     *
     *  【注意】
     *  在处理  AND / OR 逻辑时，如果有 OR 逻辑的话，两端的判断语句都会增加一个 ( )
     *  例： (xxx) OR (xxx) 无论 xxx 的具体内容是什么
     */
    bool get_self_where_query(string& where_query);

    bool get_join_query(string& join_query);
    bool get_self_join_query(string& join_query);

    /*
     *  获取表中的 ORDER BY 语句
     *  如果有 JOIN 联合表格，则 JOIN 表的 ORDER BY 语句在外层
     *  不在 JOIN 语句中，即先进行 JOIN 然后对 JOIN 后的结果进行 ORDER BY
     */
    bool get_orderby_query(string& query);

    /*
     *  获取表中的 GROUP BY 语句
     *  如果有 JOIN 联合表格，则 JOIN 表的 GROUP BY 语句在外层
     *  不在 JOIN 语句中，即先进行 JOIN 然后对 JOIN 后的结果进行 GROUP BY
     */
    bool get_groupby_query(string& query);

    /*
     *  获取表中的 LIMITED 语句
     *  设置分页数据
     */
    bool get_limited_query(string& query);

    /*
     *  拼接  SQL 查询语句：
     *
     *  此处只将各个 SQL 语句元素组合好即可
     */
    void get_every_sql_elements(bool need_seperate_page);

    /*
     *  拼接  SQL 获取查询个数语句：
     */
    bool get_whole_count_query(string& query);

    /*
     *  拼接  SQL 查询语句：
     */
    bool get_whole_query_string(string& query);

    /*
     *  拼接  SQL 插入语句：
     */
    bool get_whole_insert_string(string& query);

    /*
     *  拼接  SQL 修改语句：
     */
    bool get_whole_update_string(string& query);

    /*
     *  从 this->insert_update_conditions 中取出数据
     *  并拼接这些数据形成 SQL 插入语句
     */
    bool append_column_values_list(string& columns, string& values);

    /*
     *  从 this->insert_update_conditions 中取出数据
     *  并拼接这些数据形成 SQL 修改语句
     */
    bool append_column_set_list(string& set_query);

    void add_join_table(MysqlTable* join_table);

    void fill_mysql_result_bucket(MySQLRecord& record);
    void fill_result_set(DuokooMysql& mysql, int record_count);

    /*
     *  判断是否需要将传入的列名转换为  nick_name.column_name 的形式
     *  按照此对象内部的形式，统一格式
     *
     *  return: -1 错误 ; 0 不需要转换 ; 1 需要转换
     */
    int change_to_table_nick_column(const char* value, string& result_name);

    // 获取不分页时的记录总数
    int get_result_total_count(DuokooMysql& mysql);

	// 根据总数量调整  [_start_pos, _unit_num]
    int modify_query_boundary(int total_count);
	

public:

    MysqlTable(const char* table_name);

    MysqlTable(const char* table_name,
               const char* nick_name);

    ~MysqlTable();

    int get_select_column_size();

    /*
     *  设置分页数
     */
    void set_unit_info(const uint32_t unit_num,
                       const uint32_t unit_times);

    /*
     *  设置最终 SQL 语句中 select 段的列名
     *  并设置此列为 KEY 索引列，对应 this.id_s
     *  之后通过  this.id_s 遍历时，id 就是此列的数据
     */
    void add_select_column(const char* col_name, const char* nick_name);
    void add_select_column(const char* col_name);

    /*
     *  [] 运算符重载
     *  返回 ExMysqlColumn 类型的列，应该永远不能返回  NULL 指针
     *  如果传入的 column_name 不存在，则新建列，并保存在  extend_columns 中
     */
    ExMysqlColumn* operator[] (const char* column_name);

    /*
     *  [] 运算符重载
     *  返回 string 类型的列, 没有时返回 NULL
     *  即设置的参与 SQL 语句中 select 段的列名
     */
    const char* operator[] (int i);

    /*
     *  执行查询语句，从数据库中获取数据
     */
    bool process_query_sql(const char* db_name, bool need_seperate_page);

    /*
     *  插入操作
     */
    bool add_new_insert_transaction();
    bool process_insert_transaction(const char* db_name);

    /*
     *  修改操作
     */
    bool add_new_update_transaction();
    bool process_update_transaction(const char* db_name);

    unsigned int get_total_key_count();

    int get_total_columns_count();
	
	int get_record_count_by_key_index(int key_index);

    // 获取不分页时的记录总数
	int get_total_count_without_page();

    /*
     *  返回结果集中数据的接口：
     *  key_index    第几个索引值
     *  vec_index    该索引值下的第几条数据
     *  column_index 该条数据的第几列
     */
    const char* get_result_set_value(int key_index,
                                     int vec_index,
                                     int column_index);

    /*
     *  返回结果集中数据的接口：
     *  key       索引值
     *  vec_index 该索引值下的第几条数据
     *  column    该条数据的哪列
     *
     *  【注意】
     *  如果有 JOIN 表格并且匿名，在获取数据时
     *  column 必须以 nick_name.column_name 的形式传入才行
     */
    const char* get_result_set_value(const char* key,
                                     int vec_index,
                                     const char* column);

    /*
     *  返回结果集中数据的接口：
     *  key_index 索引值
     *  vec_index 该索引值下的第几条数据
     *  column    该条数据的哪列
     *
     *  【注意】
     *  如果有 JOIN 表格并且匿名，在获取数据时
     *  column 必须以 nick_name.column_name 的形式传入才行
     */
    const char* get_result_set_value(int key_index,
                                     int vec_index,
                                     const char* column);
};


#endif /* MYSQL_GENERAL_MODULE_H_ */
