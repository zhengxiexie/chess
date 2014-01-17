#ifndef _GAME_PACKAGE_VERSION_H_
#define _GAME_PACKAGE_VERSION_H_

#include "content_common.h"

namespace content
{
	class GamePackageVersion //public ContentList
	{   
		public:
			GamePackageVersion(string packagename,string versioncode)
			{   
				_packagename = packagename;
				_versioncode = versioncode;
				_conn = mongo_pool->getConnection();
			}   
			GamePackageVersion(string packagename)
			{
				_packagename = packagename;
				_conn = mongo_pool->getConnection();
			}
			~GamePackageVersion()
			{   
				if(NULL != _conn)
				{   
					mongo_pool->releaseConnection(_conn);
				}   
			}   
			int get_cid(string& cid, bool with_versioncode);
			void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
		protected:
			void get_query_bson(bson* query, bool with_versioncode);
			void get_fields_bson(bson* fields);
			void get_cid_from_bson(const bson* result, string& cid);

		private:
			mongo *_conn;
			string _packagename;
			string _versioncode;

	};  
};

#endif
