#ifndef _GAME_INFO_CONTENT_
#define _GAME_INFO_CONTENT_

#include "content_common.h"

namespace content
{
	class GameInfoContent //public ContentList
	{
		public:
			GameInfoContent(string strGameId)
			{
				_strGameId = strGameId;
				_conn = mongo_pool->getConnection();
			}
			~GameInfoContent()
			{
				if(NULL != _conn)
				{
					mongo_pool->releaseConnection(_conn);
				}
			}
			int get_info(game_t& info);
			void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
		protected:
			void get_query_bson(bson* query);
			void get_fields_bson(bson* fields);
			void get_info_from_bson(bson* result, game_t& info);

		private:
			mongo *_conn;
			string _strGameId;

	};
};

#endif
