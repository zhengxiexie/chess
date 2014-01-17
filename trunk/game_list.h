#ifndef _GAME_LIST_H_
#define _GAME_LIST_H_

#include "content_list.h"

namespace content{

    class GameList: public ContentList
        {   
            public:
            GameList(const string& content_type="1"):ContentList(content_type)
            {   
				//_num=num;
            }

            void set_c_id(const string& c_id)
            {
                _c_id = c_id;
            }
            void set_info_type(const string& info_type, const string& game_type = "")
            {
                _info_type = info_type;
                _game_type = game_type;
            }
			int get_game_brief_list(int32_t& count, vector<game_brief_t>& list);
			int get_game_brief_list(vector<game_brief_t>& list);
			//virtual void get_basic_query_bson(int32_t day, bson* query);
			virtual void get_count_query_bson(bson* query);


			//int  get_game_brief_list(uint32_t&count,vector<game_brief_t> list);
			//int  get_game_brief_from_cursor(mongo_cursor * cursor, game_brief_t& ios_content );
			void get_game_brief_query_bson(bson* query);
			//void get_game_brief_fields_bson(bson* fields);
			private:
				uint32_t _num;
                string _c_id;
                string _info_type;
                string _game_type;
		};  

};


#endif
