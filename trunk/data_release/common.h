#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "mongo.h"
#include "mysql.h"
#include "redis_connpool.h"
#include <map>
#include <set>
#include <vector>

using namespace std;

//#define MONGODB_HOST "10.40.22.14"
#define MONGODB_PORT 27020
#define NG -1
#define OK 0

#define TIMESTAMP_FILE_PATH "/home/work/upmongodb/last_update_timestamp.txt"

extern MYSQL mysql_conn;
extern mongo mongodb_conn[1];
extern set<string> s_cids;
time_t date_to_int(char *date);
extern redisContext *c;
extern redisReply *reply;

//add by zhengxie
typedef struct pub_prop_record
{
	string id;
	string name;
	string color;
	string prop_url;
}pub_prop_record;

int set_user_question_id();
int set_user_question();
int get_pub_prop_info();
int bson_append_pub_prop_info(bson* b, string pub_prop_info);
int get_channel_info();
int get_channel_info2();
/* 2013.05.28 add by zhengxie */
double get_star_by_c_id(const char *c_id);
double getStarFromRedis(redisContext* &c, char* c_id);

int set_mcp_game_content();
int set_mcp_game_content_new();
int set_mcp_content_format();
int set_mcp_content_class();

int set_mcp_content_data();
int set_mcp_content_data_kjava();

int set_mcp_content_diy_column();
int set_mcp_content_column_list();
int set_mcp_content_cp();
int set_mcp_content_grant();
int set_mcp_content_preview();
int set_mcp_content_rank();
int set_mcp_phone_info();
int set_mcp_theme_content();
int set_mcp_type_list();
int set_mcp_content_chargetype();
int set_mcp_content_class_list();
int set_mcp_url_content();
int set_mcp_content_bookmark();
int set_mcp_content_bookmark_list();
int set_mcp_class_adapt_count();
int set_mcp_software_content();
int set_mcp_android_theme();
int set_mcp_content_topic();
int set_mcp_topic_col_map();
int set_mcp_class_class_map();
int bson_append_class_info(bson* b, string class_list);
int bson_append_column_info(bson* b, string column_list);
int bson_append_img_info(bson* b, string img_list);
int check_cid(char c_id[20]);
int set_mcp_ios_content();
int set_mcp_content_news();

// [s] add by gwj for empty_game_news
int set_mcp_content_empty_game_news();
// [e] add by gwj for empty_game_news

int set_mcp_appscore();
int set_mcp_appinfo();
int set_mcp_online_game();
int set_mcp_mixed_topic_music();
int set_mcp_recently_played();
int set_mcp_favorite_list();
int set_mcp_content_calender();
int set_mcp_H5_music_id();

// [s] add by gwj for common_adv_info
int set_mcp_content_common_info();
int set_mcp_netgame_activity_info();
int set_mcp_activity_award_info();
int set_mcp_activity_info();
int set_mcp_appInfo_data_info();

void bson_append_img_info2(bson* b, string& img_info_list);
void bson_append_lable_info(bson* b, string& lable_list1);
void bson_append_ptype_info(bson* b, string& ptype1);

// [e] add by gwj for common_adv_info

// [s] add by zhengxie for mobile_music_info
int set_mb_music_content();
// [e] add by zhengxie for mobile_music_info

// add by zhengxie for 增量更新
int set_mcp_diff_update_info();

// add by zhengxie for 争霸赛
int set_mcp_content_championship();

int set_question_and_answer();
int set_user_question_id();
int set_qp_activity();
int set_qp_activity_award();
int set_qp_activity_person();
int set_person_award();

//add by zhengxie 
#define IS_REDIS_REPLY_ERROR(type)\
    (type == REDIS_REPLY_NIL || type == REDIS_REPLY_ERROR)

#define FREE_REDIS_REPLY(reply)\
    if(reply != NULL)\
    {\
        freeReplyObject(reply);\
        reply = NULL;\
    }

// 对 char* 的 非空字符串判断
#define IS_VALUED_STRING(str_instence)\
        (str_instence != NULL && strcmp(str_instence, "") != 0)

// 判断两个 char* 字符串是否相同
#define IS_SAME_VALUED_STRING(str1, str2)\
        (str1 == str2 || strcmp(str1, str2) == 0)

#define SET_BSON_VALUE_STRING_B(b, mysql_row, mongodb_fname)\
        if(mysql_row == NULL)\
        {\
            bson_append_string(b, mongodb_fname, "");\
        }\
        else\
        {\
            bson_append_string(b, mongodb_fname, mysql_row);\
        }

#define SET_BSON_VALUE_INT_B(b, mysql_row, mongodb_fname)\
        if(mysql_row == NULL)\
        {\
            bson_append_int(b, mongodb_fname, 0);\
        }\
        else\
        {\
            bson_append_int(b, mongodb_fname, atoi(mysql_row));\
        }

#define SET_BSON_VALUE_STRING(mysql_row, mongodb_fname)\
        if(mysql_row == NULL)\
        {\
            bson_append_string(b, mongodb_fname, "");\
        }\
        else\
        {\
            bson_append_string(b, mongodb_fname, mysql_row);\
        }

#define SET_BSON_VALUE_INT(mysql_row, mongodb_fname)\
        if(mysql_row == NULL)\
        {\
            bson_append_int(b, mongodb_fname, 0);\
        }\
        else\
        {\
            bson_append_int(b, mongodb_fname, atoi(mysql_row));\
        }

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

class CommonInterface
{
public:

    CommonInterface();
    ~CommonInterface();
	

    // 找到 value 在 list 中的索引，不存在时返回 -1
    static int find_key_index(const char* value, vector<const char*>& list);
    static int find_key_index(const char* value, vector<string>& list);

    // 数组赋值拷贝
    static void copy_vector_values(vector<string>& src, vector<string>& des);

    // 删除字符串数组数据
    static void clear_vector_string(vector<string>& list);

    /*
     *  按照指定符号分割字符串
     *  return : 返回分割后的字符串个数
     */
    static int splic_string(string& orig_string, vector<string>& split_result, const char* seperator);

    static void set_struct_mongodb_value(bson* b,
                                         const char* field_name,
                                         vector<const char*>& struct_cols,
                                         string& mysql_value);

    static void set_array_mongodb_value(bson* b,
                                        const char* field_name,
                                        string& mysql_value);
};
// [e] add by gwj for common_interface
class FILEShell
{
private:
    FILE* ts_fd;

public:
    FILEShell(FILE* ts_fd)
    {
        this->ts_fd = ts_fd;
    }

    ~FILEShell()
    {
        if(this->ts_fd != NULL)
        {
            fclose(this->ts_fd);
            this->ts_fd = NULL;
        }
    }
};

class TimeStampProcessor
{
    NSP_SINGLETONIZE(TimeStampProcessor)

    FILE* ts_fd;
    string last_update_ts;

    ~TimeStampProcessor();
    void get_time_now(string& time_now);

public:
    const char* get_last_update_ts();
};



void nowtime();

#endif
