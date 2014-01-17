#ifndef _CONTENT_TOPIC_H_
#define _CONTENT_TOPIC_H_

#include <vector>
#include "content_common.h"
#include "mongo_connpool.h"
using namespace std;

extern mongo_connpool* mongo_pool;

namespace content
{
    class ContentTopic
    {
        public:
            ContentTopic()
            {
                _conn = mongo_pool->getConnection();
                _page=_page_size=_start_pos=0;
            }
            ContentTopic(const string& topic_id)
            {
                 _conn = mongo_pool->getConnection();
                _page=_page_size=_start_pos=0;
                set_id(topic_id);
            }
            ~ContentTopic()
            {
                if( _conn != NULL ){
                    mongo_pool->releaseConnection(_conn);
                }
            }
            void set_page_info(uint32_t page, int32_t page_size)
            {
                if( page==0 ){
                    _page=1;
                }
                else{
                    _page=page;
                }
                if(page_size==0){
                    _page_size=10;
                }
                else{
                    _page_size=page_size;
                }
                _start_pos=(_page-1)*_page_size;
            }
            void set_id(const string& topic_id);
            int get_topic_info(topic_info_t& topic_info);
            int get_topic_info(const string& topic_id, topic_info_t& topic_info);
            int get_column_list(int32_t& count, vector<column_info_t>& list);
            int get_neighbour_topic(string& before, string& end);
            void free_data(mongo_cursor *cursor, bson* query, bson* fields, bson* bson_result);
        protected:
            void get_column_query_bson(bson* query);
            void get_column_fields_bson(bson* fields);
            void get_count_query_bson(bson* query);
        private:
            mongo *_conn;
            topic_info_t _topic_info;
            uint32_t _page,_page_size,_start_pos;
    };
};

#endif
