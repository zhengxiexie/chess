/*
 * common.h
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "../content_list.h"
#include <stdlib.h>
#include <sys/syscall.h>

#include <iterator>
#include <list>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define MYSQL_INT_VALUE_LEN 16
#define OBJECT_POOL_SIZE 40

// 输入参数的字串长度限制
#define KEY_NAME_MAX_LEN 64
#define VALUE_MAX_LEN 1024

// 对 char* 的 非空字符串判断
#define IS_VALUED_STRING(str_instence)\
        (str_instence != NULL && strcmp(str_instence, "") != 0)

// 判断两个 char* 字符串是否相同
#define IS_SAME_VALUED_STRING(str1, str2)\
        (str1 == str2 || strcmp(str1, str2) == 0)

// 专门处理MYSQL的插入条件
#define SET_MYSQL_INSERT_CONDITION(value, column_name, table)\
        if(IS_VALUED_STRING(value))\
        {\
            table[column_name]->set_value(value);\
        }

// 专门处理MYSQL的筛选过滤条件
#define SET_MYSQL_SELECT_CONDITION(value, column_name, table, oper)\
        if(IS_VALUED_STRING(value))\
        {\
            table[column_name]->add_and_condition(oper, value);\
        }

// 专门用于清理 OR 条件时使用的  vector<string> 数组
#define CLEAR_VECTOR_STRING_CONDITIONS(column_name)\
        CommonInterface::clear_vector_string(column_name##_id_vals);

// 专门处理MYSQL的筛选 OR 条件
#define SET_MYSQL_OR_CONDITION(value, column_name, table, oper)\
        vector<string> value##_id_vals;\
        if(IS_VALUED_STRING(value))\
        {\
            string value##_id_str = value;\
            int size = CommonInterface::splic_string(value##_id_str, value##_id_vals, ",");\
\
            if(size > 0)\
            {\
                table[column_name]->add_and_condition(oper, value##_id_vals[0].c_str());\
\
                for(int i = 1; i < size; i++)\
                {\
                    table[column_name]->add_or_condition(oper, value##_id_vals[i].c_str());\
                }\
            }\
        }

#define WEIGHT_ORDER_CONDITION(column_id, class_id, weight_order, compound_decor, ultimate)\
        WeightDecorator weight_decor;\
        OrderTypeStruct order_struct;\
        order_struct.order_type = weight_order;\
        order_struct.extra_tuple["column"] = column_id;\
        order_struct.extra_tuple["class"] = class_id;\
        order_struct.outer_decorator = &compound_decor;\
        weight_decor.weight_type = weight_decor.get_weight_type(order_struct);\
\
        if(!weight_decor.is_aggregate_query())\
        {\
            compound_decor.add_primary_table_decorator(&weight_decor);\
            ultimate = &compound_decor;\
        }\
        else\
        {\
            ultimate = &weight_decor;\
        }

// 专门用于处理输入参数 NOT-EQUAL 逻辑的宏模板，以 JSON 格式输入
#define NOT_EQUAL_CONDITION_JSON(field_name, mongodb_fname, compound_decor)\
    NotEqualDecorator field_name##_not_equal_dec;\
    vector<char*> field_name##_id_vals;\
\
    if(IS_VALUED_STRING(field_name))\
    {\
        int size = CommonInterface::splic_string((char*)field_name, field_name##_id_vals, ";");\
\
        if(size > 0)\
        {\
            for(int i = 0; i < size; i++)\
            {\
                field_name##_not_equal_dec.add_field_filter_condition(mongodb_fname,\
                                                                      field_name##_id_vals[i]);\
            }\
\
            compound_decor.add_primary_table_decorator(&field_name##_not_equal_dec);\
        }\
    }

// 专门用于处理输入参数 OR 逻辑的宏模板，以 JSON 格式输入
#define OR_CONDITION_JSON(field_name, mongodb_fname, compound_decor)\
    OrDecorator field_name##_or_dec;\
    vector<char*> field_name##_id_vals;\
\
    if(IS_VALUED_STRING(field_name))\
    {\
        int size = CommonInterface::splic_string((char*)field_name, field_name##_id_vals, ";");\
\
        if(size > 0)\
        {\
            for(int i = 0; i < size; i++)\
            {\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname,\
                                                               field_name##_id_vals[i]);\
            }\
\
            compound_decor.add_primary_table_decorator(&field_name##_or_dec);\
        }\
    }

// 专门用于处理输入参数 IS 逻辑的宏模板，最简单的 IS 逻辑，没有数组形式
#define IS_CONDITION_TEMPLATE(field_value, mongodb_fname, compound_decor)\
    IsDecorator field_value##_is_dec;\
\
    if(IS_VALUED_STRING(field_value))\
    {\
        field_value##_is_dec.add_field_filter_condition(mongodb_fname, field_value);\
        compound_decor.add_primary_table_decorator(&field_value##_is_dec);\
    }\

// add by zhengxie 专门用于删除输入参数 IS 逻辑的宏模板，最简单的 IS 逻辑，没有数组形式
#define REMOVE_IS_CONDITION_TEMPLATE(field_value, compound_decor)\
        compound_decor.remove_primary_table_decorator(&field_value##_is_dec);\

// 专门用于处理输入参数 OR 逻辑的宏模板，以自定义格式输入
#define OR_CONDITION_TEMPLATE(field_name, mongodb_fname, compound_decor)\
    OrDecorator field_name##_or_dec;\
    vector<char*> field_name##_id_vals;\
\
    if(IS_VALUED_STRING(field_name))\
    {\
        int size = CommonInterface::splic_string((char*)field_name, field_name##_id_vals, ",");\
\
        if(size > 0)\
        {\
            for(int i = 0; i < size; i++)\
            {\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname,\
                                                               field_name##_id_vals[i]);\
            }\
\
            compound_decor.add_primary_table_decorator(&field_name##_or_dec);\
        }\
    }

// add by zhengxie 专门用于删除输入参数 OR 逻辑的宏模板，最简单的 OR 逻辑，没有数组形式
#define REMOVE_OR_CONDITION_TEMPLATE(field_value, compound_decor)\
        compound_decor.remove_primary_table_decorator(&field_value##_or_dec);\


// 专门为设置 class 游戏分类写的宏模板，包含了设置全部分类，全部单机分类，网游分类以及具体只能某一分类
#define CLASS_ID_OR_CONDITION(field_name, mongodb_fname, compound_decor)\
        OrDecorator field_name##_or_dec;\
\
        if(IS_VALUED_STRING(field_name))\
        {\
            if(strcmp(field_name, "-1") == 0)\
            {\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "97");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "98");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "99");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "100");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "101");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "102");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "104");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "108");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "337");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "341");\
            }\
            else if(strcmp(field_name, "-2") == 0)\
            {\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "97");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "98");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "99");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "100");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "101");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "102");\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "104");\
            }\
            else if(strcmp(field_name, "-3") == 0)\
            {\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, "108");\
            }\
            else\
            {\
                field_name##_or_dec.add_field_filter_condition(mongodb_fname, field_name);\
            }\
\
            compound_decor.add_primary_table_decorator(&field_name##_or_dec);\
        }

// 专门用于处理输入参数 "大于等于" 逻辑的宏模板
#define BIGGER_EQUAL_CONDITION_TEMPLATE(field_name, mongodb_fname, compound_decor)\
        BiggerThanDecorator bigger_##field_name;\
        bigger_##field_name.is_bigger_and_equal = true;\
        if(IS_VALUED_STRING(field_name))\
        {\
            smaller_##field_name.add_field_filter_condition(mongodb_fname, field_name);\
            compound_decor.add_primary_table_decorator(&bigger_##field_name);\
        }

// 专门用于处理输入参数 "大于" 逻辑的宏模板
#define BIGGER_CONDITION_TEMPLATE(field_name, mongodb_fname, compound_decor)\
        BiggerThanDecorator bigger_##field_name;\
        if(IS_VALUED_STRING(field_name))\
        {\
            bigger_##field_name.add_field_filter_condition(mongodb_fname, field_name);\
            compound_decor.add_primary_table_decorator(&bigger_##field_name);\
        }

// 专门用于处理输入参数 "小于等于" 逻辑的宏模板
#define SMALLER_EQUAL_CONDITION_TEMPLATE(field_name, mongodb_fname, compound_decor)\
        SmallerThanDecorator smaller_##field_name;\
        smaller_##field_name.is_smaller_and_equal = true;\
        if(IS_VALUED_STRING(field_name))\
        {\
            smaller_##field_name.add_field_filter_condition(mongodb_fname, field_name);\
            compound_decor.add_primary_table_decorator(&smaller_##field_name);\
        }

// 专门用于处理输入参数 "小于" 逻辑的宏模板
#define SMALLER_CONDITION_TEMPLATE(field_name, mongodb_fname, compound_decor)\
        SmallerThanDecorator smaller_##field_name;\
        if(IS_VALUED_STRING(field_name))\
        {\
            smaller_##field_name.add_field_filter_condition(mongodb_fname, field_name);\
            compound_decor.add_primary_table_decorator(&smaller_##field_name);\
        }

// 专门用于升序排列的宏模板
#define ACREASE_ORDER_CONDITION_TEMPLATE(field_name, mongodb_fname, compound_decor)\
        OrderbyDecorator field_name##_order;\
        field_name##_order.order_type = 1;\
        field_name##_order.add_field_filter_condition(mongodb_fname, "");\
        compound_decor.add_primary_table_decorator(&field_name##_order);

// 专门用于降序排列的宏模板
#define DECREASE_ORDER_CONDITION_TEMPLATE(field_name, mongodb_fname, compound_decor)\
        OrderbyDecorator field_name##_order;\
        field_name##_order.order_type = -1;\
        field_name##_order.add_field_filter_condition(mongodb_fname, "");\
        compound_decor.add_primary_table_decorator(&field_name##_order);

// 专门设置多输入参数的宏模板，自定义输入格式
#define SET_INPUT_PARAMETERS_TEMPLATE(string_value, input_type_mem)\
        if(IS_VALUED_STRING(string_value))\
        {\
            string contents_str = string_value;\
            CommonInterface::splic_string(contents_str,\
                                          input_type_mem, ",");\
        }

// 专门设置多输入参数的宏模板，JSON格式
#define SET_INPUT_PARAMETERS_JSON(string_value, input_type_mem)\
        if(IS_VALUED_STRING(string_value))\
        {\
            string contents_str = string_value;\
            CommonInterface::splic_string(contents_str,\
                                          input_type_mem, ";");\
        }

// 专门处理 upmongoDB 中，把 mysql 值设置到 BSON 中的模板
#define SET_BSON_VALUE(mysql_row, mongodb_fname, value_type)\
        if(mysql_row == NULL)\
        {\
            bson_append_##value_type(b, mongodb_fname, "");\
        }\
        else\
        {\
            bson_append_##value_type(b, mongodb_fname, mysql_row);\
        }

// 专门为输出参数设置资讯类型名称的宏模板
#define SET_NEWS_TYPE_NAME(news_record, out, out_name)\
        LOGD("[GWJ] %s: type_id[%s]", __FUNCTION__, news_record->type_id.c_str());\
\
        string type_name_value;\
        if(strcmp(news_record->type_id.c_str(), "1") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("1", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "2") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("2", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "3") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("3", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "4") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("4", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "5") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("5", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "6") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("6", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "7") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("7", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "8") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("8", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "9") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("9", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }\
        else if(strcmp(news_record->type_id.c_str(), "10") == 0)\
        {\
            CommonSimpleBuffer::instance().get_game_info_type_name("10", type_name_value);\
            out->set_##out_name(type_name_value.c_str());\
        }

// 专门为输出参数设置图片及类型的宏模板
#define SET_OUTPUT_IMG_INFO(img_info, select_imgs, output_img)\
        vector<MCPTupleItem>::const_iterator itr = img_info.begin();\
        vector<MCPTupleItem>::const_iterator end_itr = img_info.end();\
        for(int i = 0; itr != end_itr; itr++)\
        {\
            const char* img_type = (*itr).key.c_str();\
\
            int index = CommonInterface::find_key_index(img_type, select_imgs);\
\
            if(index < 0)\
            {\
                LOGD("[GWJ] %s: No need img_type[%s]", __FUNCTION__, img_type);\
            }\
            else\
            {\
                LOGD("[GWJ] %s: need img_type[%s]", __FUNCTION__, img_type);\
                output_img(i)->set_type(img_type);\
                output_img(i)->set_url((*itr).value.c_str());\
                i++;\
            }\
        }

#define SET_OUTPUT_PUB_PROP_INFO_NEW(i, key, value)\
	if( !strcmp(key, "name") )\
	{\
			out->mutable_pub_prop_info(i)->set_name(value);\
	}\
	else if( !strcmp(key, "color") )\
	{\
			out->mutable_pub_prop_info(i)->set_color(value);\
	}\
	else if( !strcmp(key, "prop_url") )\
	{\
			out->mutable_pub_prop_info(i)->set_prop_url(value);\
	}\
	LOGA("[zx] %s: set struct [%s %s]",__FUNCTION__, key, value);


//add by zhengxie 
#define SET_OUTPUT_PUB_PROP_INFO(pub_prop_info,  output_pub_prop_info)\
		LOGD("[ZX] pub_prop_info size:%d ", pub_prop_info.size());\
        vector<MCPTupleItem>::const_iterator itr2 = pub_prop_info.begin();\
        vector<MCPTupleItem>::const_iterator end_itr2 = pub_prop_info.end();\
        for(int i=0; itr2 != end_itr2; itr2++)\
        {\
            const char* name = (*itr2).key.c_str();\
            const char* color = (*itr2).value.c_str();\
			output_pub_prop_info(i)->set_name(name);\
			output_pub_prop_info(i)->set_color(color);\
			i++;\
			LOGD("[ZX] set name:%s: name[%s]", __FUNCTION__, name);\
			LOGD("[ZX] set color:%s: color[%s]", __FUNCTION__, color);\
        }

// 定义一个类为单例模式的宏
#define NSP_SINGLETONIZE(class_name__)                      \
    protected:                                              \
        class_name__();                                     \
        class_name__(const class_name__&);                  \
        class_name__& operator = (const class_name__&);     \
    public:                                                 \
        static class_name__& instance()                     \
        {                                                   \
            static class_name__ singletonInstance;          \
            return  singletonInstance;                      \
        }                                                   \
    protected:


/*
 *  整个 Refactory 重构框架中全用 string 类型
 *  从数据库中查询的数据不保存为其它类型
 */
struct MCPTupleItem
{
    string key;
    string value;
    bool is_string_value;

    void clear()
    {
        key.clear();
        value.clear();
    }
};

struct MCPStructItem
{
	map<string, string> record;
};
/*
 *  专门用于设置查询条件的小部件
 */
struct MCPTupleDecoratorItem
{
    /*
     *  设置的 value 是否为 string 类型
     *  如果不是 string 类驮虬凑?integer 类型来处理
     */
    bool is_string_value;

    // 要操作的表格列名，即  key
    const char* table_column;

    /*
     *  列名满足的条件
     *  都先按照 char* 类型来保存
     *  具体处理时判断是 string 类型还是 integer 类型
     *  到时候再做相应的转换
     */
    const char* value;

    MCPTupleDecoratorItem()
    {
        // 默认为 string 类型
        is_string_value = true;
    }
};

/*
 *  没有成员变量的通用接口类
 *  可以理解为单例模式
 */
class CommonInterface
{
public:

    CommonInterface();
    ~CommonInterface();

    // 找到 value 在 list 中的索引，不存在时返回 -1
    static int find_key_index(const char* value, vector<const char*>& list);
    static int find_key_index(const char* value, vector<string>& list);
    static int find_key_index(const char* value, list<const char*>& list);
    static int find_key_index(const char* value, list<string>& list);

    // 数组赋值拷贝
    static void copy_vector_values(vector<string>& src, vector<string>& des);

    // 删除字符串数组数据
    static void clear_vector_string(vector<string>& list);

    // 找到 MYSQL 表中不含匿名的列名
    static const char* find_pure_column_name(const char* column_name);

    /*
     * 判断输入的字符串是否是标准的列名形式，即 `column_name`
     *
     * 最终结果放入 standard_column 保证其实一个标准格式
     *
     * return : 0 : 不是标准格式，需要转换
     *          1 : 已经是标准格式，不需要转换
     *          -1 : 错误的列名格式
     */
    static int is_standard_column_format(const char* column_name, string& standard_column);

    /*
     * 获取输入 column 中的 nick name, 保存到输出参数中
     *
     * return : 0 : column 中不含 nick_name
     *          1 : 包含 nick_name 并返回
     *          -1 : 错误的列名
     */
    static int get_nick_name(const char* column, string& nick_name);

    // 获取系统当期时间戳
    static void get_time_now(string& tm_now);

    /*
     *  按照指定符号分割字符串
     *  return : 返回分割后的字符串个数
     */
    static int splic_string(string& orig_string, vector<string>& split_result, const char* seperator);
    static int splic_string(char* orig_string, vector<char*>& split_result, const char* seperator);

    /*
     *  去掉首尾边界字符，获取中间字符串
     */
    static char* elimite_shell_flag(char* orig_string,
                                    const char start_flag,
                                    const char end_flag);

    static void set_struct_mongodb_value(bson* b,
                                         const char* field_name,
                                         vector<const char*>& struct_cols,
                                         string& mysql_value);

    static void set_array_mongodb_value(bson* b,
                                        const char* field_name,
                                        string& mysql_value);

	static double get_interface_process_time( struct timeval &s, struct timeval &e);
	static pid_t get_thread_id();
};

/*
 *  模块化 LOG 日志
 *
 *  LOGA 的级别最高，用于最终上线版 LOG
 *  所以一般情况下不要用 LOGA
 *  LOGA 只打印最外层接口错误信息即可
 */
#define LOGA(...) UB_LOG_WARNING(__VA_ARGS__)

/* add by zhengxie 精确到毫秒级别的时间调试 */
#define LOGT(START, END) UB_LOG_WARNING("Interface[%s] process time [%lf ms]\n", __FUNCTION__, CommonInterface::get_interface_process_time(START, END))

/*
 *  一下为调试用 LOG
 *  如有需要也可以自行增加
 *  但记住最终上线版时关掉这些调试 LOG
 */
#define MEM_HEAP_TEST 0
#if MEM_HEAP_TEST
#define LOGFC(...) //UB_LOG_NOTICE(__VA_ARGS__)
#else
#define LOGFC(...) //UB_LOG_NOTICE(__VA_ARGS__)
#endif

// 全局调试 log
#define LOGD(...)  UB_LOG_NOTICE(__VA_ARGS__)

// 从数据库中获取数据时的 log
#define LOGG(...)  //UB_LOG_NOTICE(__VA_ARGS__)

// test 目录中测试用的 log
#define LOGXX(...) //UB_LOG_NOTICE(__VA_ARGS__)
#define LOGX(...)  //UB_LOG_NOTICE(__VA_ARGS__)

// 关于 RPC 通信的 log
#define LOGN(...)  //UB_LOG_NOTICE(__VA_ARGS__)


#endif /* COMMON_H_ */
