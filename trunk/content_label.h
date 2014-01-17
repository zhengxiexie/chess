#ifndef _CONTENT_LABEL_H_
#define _CONTENT_LABEL_H_

#include "content_list.h"

namespace content{

class ContentLabel: public ContentList
{
public:
    ContentLabel( const string& label_id ):ContentList()
    {
        _label_id = label_id;
    }
    ContentLabel( const string& content_type, uint32_t adapt, const string& label_id ):ContentList(content_type,adapt)
    {
        _label_id = label_id;
    }

    virtual void get_basic_query_bson(bson* query);
    virtual void get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_count_query_bson(bson* query);
    virtual void get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);

private:
    string _label_id;
};

};

#endif
