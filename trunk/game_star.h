#ifndef _GAME_STAR_H_
#define _GAME_STAR_H_

#include "content_common.h"
#include "redis_connpool.h"

extern redisConnPool *redis_pool;

namespace content
{
	class GameStar //public ContentList
	{   
		public:
			GameStar(string packagename, string versioncode, string imei)
			{   
				_packagename = packagename;
				_versioncode = versioncode;
				_imei = imei;
				_conn = redis_pool->getConnection();
			}   
			~GameStar()
			{   
				if(NULL != _conn)
				{   
					redis_pool->releaseConnection(_conn);
				}   
			}   
			int get_star(string& star);
			//void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
		protected:
			//void get_query_bson(bson* query);
			//void get_fields_bson(bson* fields);
			//void get_game_start_from_bson(bson* result, int& star);

		private:
			redisContext *_conn;
			string _packagename;
			string _versioncode;
			string _imei;

	};  
};

#endif
