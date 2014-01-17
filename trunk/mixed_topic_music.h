#ifndef _MIXED_TOPIC_MUSIC_H_
#define _MIXED_TOPIC_MUSIC_H_

#include "content_common.h"
#include "mongo_connpool.h"

namespace content{
	
class MTMusicContentList
{
	public:
		MTMusicContentList(const string& topic_id, uint32_t page, uint32_t page_size)
		{
			_conn = mongo_pool->getConnection();
			_topic_id = topic_id;
			_page = page;
			_page_size = page_size;
		}

		~MTMusicContentList()
		{
			if( _conn != NULL ){
				mongo_pool->releaseConnection(_conn);
			}   
		}

		int get_MTMusic_content_list(int &count, vector<music_t>& list);
		void get_query_bson(bson* query);
		void get_filed_bson(bson* query);
		int get_MTMusic_from_cursor(mongo_cursor *cursor, music_t& music_info);
		void free_data( mongo_cursor * cursor, bson* query, bson* fields, bson* bson_result );

	protected:
		mongo *_conn;
	private:
		string _topic_id;
		uint32_t _page;
		uint32_t _page_size;
};

};

#endif
