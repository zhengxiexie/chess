#ifndef _CONTENT_TYPE_H_
#define _CONTENT_TYPE_H_

#include "content_common.h"

using namespace std;

namespace content
{

    class ContentType
    {
        public:
            ContentType()
            {
                _page=_page_size=_start_pos=0;
            }
            ContentType(const string& type)
            {
                _id = type;
                _page=_page_size=_start_pos=0;
            }
            void set_page_info(uint32_t page, uint32_t page_size )
            {
                if(page==0){
                    _page=1;
                }
                else{
                    _page=page;
                }
                if(page_size==0){
                    _page_size = 10;
                }
                else{
                    _page_size = page_size;
                }
                _start_pos = (_page-1)*page_size;
            }

            int getTopicList(int32_t& count,vector<topic_info_t>& topic_list);
            int getGrabList(int32_t& count, vector<grab_t>& grab_list);
        private:
            string _id;
            uint32_t _page, _page_size, _start_pos;
    };
};

#endif
