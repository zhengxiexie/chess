#ifndef _CONTENT_CALENDER_H_
#define _CONTENT_CALENDER_H_

#include "content_common.h"
#include "mongo_connpool.h"

namespace content{
	class ContentCalender{
		public:
			ContentCalender(string start_datetime, string end_datetime)
			{
				_start_datetime = start_datetime;
				_end_datetime = end_datetime;
				_conn = mongo_pool->getConnection();
			}

			~ContentCalender()
			{
				if(_conn != NULL)
				{   
					mongo_pool->releaseConnection(_conn);
				}   
			}

			int getCalenderList(vector<calender_t>& list);
			void get_query_bson(bson* query);
			void get_field_bson(bson* fields);
			int get_calender_from_cursor(mongo_cursor* cursor, calender_t& cal);


		private:
			string _start_datetime;
			string _end_datetime;
			mongo *_conn;
	};

};

#endif
