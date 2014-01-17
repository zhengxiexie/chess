#ifndef _DUOKU_PARADISE_H_
#define _DUOKU_PARADISE_H_

#include "content_common.h"
#include "mongo_connpool.h"
#include "redis_connpool.h"

namespace content{
	class DuokuParadise{
		public:
			DuokuParadise(string u_id, string c_id)
			{
				_u_id = u_id;
				_c_id = c_id;
			}
			string getRecentlyPlayedKey();
			string getFavoriteKey();

			int setRecentlyPlayed();
			int getRecentlyPlayed(vector<string> &vs);
			//int getRecentlyPlayedMongo2Redis();
			int getRecentlyPlayedMysql2Redis();
			//void get_query_bson(bson* query);
			//void get_field_bson(bson* fields);
			//int getIdListFromBson(bson* result, vector<uint32_t> &vi);

			int setFavorite();
			int getFavorite(vector<string> &vs);
			//int getFavoriteMongo2Redis();
			int getFavoriteMysql2Redis();

		private:
			string _u_id;
			string _c_id;
	};

};

#endif
