#ifndef CONTENT_CP_H
#define CONTENT_CP_H

#include "content_list.h"

namespace content{

	class ContentCp: public ContentList
		{
			public:
			ContentCp(const string& cp_id):ContentList()
			{
				_cp_id = cp_id;
			}
			ContentCp( const string& content_type, uint32_t adapt, const string& cp_id ):ContentList(content_type,adapt)
			{
				_cp_id = cp_id;
			}

				int get_ios_content_list(int32_t& count, vector<ios_content_t>& list);
				int get_ios_content_list(vector<ios_content_t>& list);
				virtual void get_basic_query_bson(bson* query);
				virtual void get_count_query_bson(bson* query);

				void get_ios_query_bson(bson* query);

			private:
				string _cp_id;
		};

}



#endif
