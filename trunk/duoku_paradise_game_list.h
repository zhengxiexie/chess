#ifndef _DUOKU_PARADISE_GAME_LIST_H_
#define _DUOKU_PARADISE_GAME_LIST_H_
//#include "content_list.h"
#include "content_common.h"
#include "mongo_connpool.h"
#include "redis_connpool.h"
#include "duokoo_tools.h"

namespace content{
	class DuokuParadiseGameList
	{
		public:
			
			DuokuParadiseGameList(const string& content_type, const uint32_t unit_num, const uint32_t unit_times)
			{
				_unit_num = unit_num;
				_unit_times = unit_times;
				_conn = mongo_pool->getConnection();
				_content_type = content_type;
			}

			~DuokuParadiseGameList()
			{
				if(_conn != NULL)
				{
					mongo_pool->releaseConnection(_conn);
				}
			}

			int get_h5_game_list(int &count, vector<duoku_paradise_game_t>& vd, const int type);
			int get_h5_game_list_by_type(int &count, vector<duoku_paradise_game_t>& vd, const int type);
			int get_h5_game_count(int type);
			int get_h5_game_count_by_type(int type);
			void get_h5_game_query_bson(bson *query, int type);
			void get_h5_game_query_bson_by_type(bson *query, int type);
			void get_h5_game_fields_bson(bson *fields);
			int get_h5_game_from_cursor(mongo_cursor *cursor, duoku_paradise_game_t& dp_game);
			void get_one_class_info(bson_iterator* it, string &class_id, string &class_name);
			void get_h5_img_urls(bson_iterator* it, duoku_paradise_game_t& dp_game);
			void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
			void get_h5_game_count_query_bson(bson *query, int type);
			void get_h5_game_count_query_bson_by_type(bson *query, int type);
			void get_h5_img_urls_by_cid(const string cid, vector<string>& vs);
			void get_h5_one_game_query_bson(bson *query, const string cid);
			void get_urls_from_bson(bson* result, vector<string>& vs);
			void get_urls(bson_iterator* it, img_h5_url_t& url_info);
			int check_favorite(const string u_id, const string c_id);
			int get_rand_h5_games(const int count, vector<duoku_paradise_game_t>& vd);	
			int get_cids(vector<int> &vi);
			void get_rand_query_bson(bson* query, const vector<int> vi);
			void get_h5_game_cid_fields_bson(bson* fields);
			int get_h5_game_cid_from_cursor(mongo_cursor*, int&);
			void get_cids_query_bson(bson*);

		private:
			mongo *_conn;
			string _content_type;
			uint32_t _unit_num;
			uint32_t _unit_times;
	};
};

#endif 
