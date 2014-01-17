#ifndef _ANDROID_THEME_H_
#define _ANDROID_THEME_H_

#include "content_common.h"

namespace content
{

class AndroidTheme
{
    public:
        AndroidTheme(const string& id)
        {
            _id = id;
            _conn = mongo_pool->getConnection();
        }
        ~AndroidTheme()
        {
            if(_conn!=NULL){
                mongo_pool->releaseConnection(_conn);
            }
        }
        int get_info(android_theme_t& theme);
        void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
    protected:
        void get_query_bson(bson* query);
        void get_fields_bson(bson* fields);
        void get_theme_info_from_bson(bson* result, android_theme_t& theme);
        void get_img_info_from_bson(bson_iterator* it, android_theme_t& theme);
        void get_class_info_from_bson(bson_iterator* it, android_theme_t& theme);
    private:
        mongo *_conn;
        string _id;
};

};

#endif
