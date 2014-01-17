#ifndef _GAME_SCORE_H_
#define _GAME_SCORE_H_

#include "content_common.h"

namespace content
{
	class GameScore //public ContentList
	{   
		public:
			GameScore(string id, string c_id, string type)
			{   
				_id = id;
				_c_id = c_id;
				_type = type;
				_conn = mongo_pool->getConnection();
			}   
			~GameScore()
			{   
				if(NULL != _conn)
				{   
					mongo_pool->releaseConnection(_conn);
				}   
			}   
			int get_info(game_score_t& info);
			void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
		protected:
			void get_query_bson(bson* query);
			void get_fields_bson(bson* fields);
			void get_game_score_from_bson(bson* result, game_score_t& info);

		private:
			mongo *_conn;
			string _id;
			string _c_id;
			string _type;
			int32_t _score;

	};  
};

#endif
