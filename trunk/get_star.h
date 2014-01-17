#ifndef _GET_STAR_H_
#define _GET_STAR_H_

#include "content_common.h"
#include "mongo_connpool.h"

class GetStar
{
        public:
                GetStar(const string& cid, const string& type_id)
                {
                    _cid = cid;
                    _typeid = type_id;
                    _conn = mongo_pool->getConnection();
                };

                ~GetStar()
                {
                    if(_conn!=NULL){
                        mongo_pool->releaseConnection(_conn);
                    }
                };
                
                void get_star_query_bson(bson* query);
                void get_star_field_bson(bson* fields);
                void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
                int get_star(uint32_t& star);

        private:
                mongo *_conn;
                string _cid;
                string _typeid;
};


#endif
