#ifndef _IOSCONTENT_H_
#define _IOSCONTENT_H_

#include "content_common.h"

namespace content{

class IosContent{
    public:
        IosContent(const string& id)
        {
            _id = id;
            _conn = mongo_pool->getConnection();
        }
        ~IosContent()
        {
            if(_conn!=NULL){
                mongo_pool->releaseConnection(_conn);
            }
        }
        int get_info(ios_content_t& info);
        void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
    protected:
        void get_query_bson(bson* query);
        void get_fields_bson(bson* fields);
        void get_info_from_bson(bson* result, ios_content_t& info);
        void get_img_info_from_bson(bson_iterator* it, ios_content_t& info);
        void get_class_info_from_bson(bson_iterator* it, ios_content_t& info);
    private:
        mongo *_conn;
        string _id;
};

};

#endif
