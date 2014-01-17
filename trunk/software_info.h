#ifndef _SOFTWARE_INFO_H_
#define _SOFTWARE_INFO_H_

#include <vector>
#include "content_common.h"
#include "mongo_connpool.h"
using namespace std;

namespace content
{
    class SoftInfo
    {
        public:
            SoftInfo(const string& id)
            {
                _id = id;
                _conn = mongo_pool->getConnection();
            }
            ~SoftInfo()
            {
                if(_conn!=NULL){
                    mongo_pool->releaseConnection(_conn);
                }
            }

            int get_soft_info(soft_info_t& soft_info);
            void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );

        protected:
            void get_soft_query_bson(bson* query);
            void get_soft_fields_bson(bson* fields);
            void get_soft_info_from_bson(bson* result, soft_info_t& soft_info);
            void get_img_info_from_bson(bson_iterator* it, soft_info_t& soft_info);
            void get_class_info_from_bson(bson_iterator* it, soft_info_t& soft_info);
        private:
            mongo *_conn;
            string _id;
    };
};

#endif
