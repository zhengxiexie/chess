#ifndef _APPINFO_CONTENT_
#define _APPINFO_CONTENT_

#include "content_common.h"

namespace content
{
	class AppinfoContent //public ContentList
	{
		public:
			AppinfoContent(const uint32_t id)
			{
				_id = id;
				_conn = mongo_pool->getConnection();
			}
			AppinfoContent(string package_name)
			{
                _id = 0;
				_package_name = package_name;
				_conn = mongo_pool->getConnection();
			}
			~AppinfoContent()
			{
				if(NULL != _conn)
				{
					mongo_pool->releaseConnection(_conn);
				}
			}
			int get_info(appinfo_t& info);
			void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );
		protected:
			void get_query_bson(bson* query);
			void get_fields_bson(bson* fields);
			void get_info_from_bson(bson* result, appinfo_t& app_info);

		private:
            uint32_t _id;
			string _package_name;
			mongo *_conn;

	};
};

#endif
