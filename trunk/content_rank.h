#ifndef _CONTENT_RANK_H_
#define _CONTENT_RANK_H_

#include "content_list.h"

namespace content{

class ContentRank: public ContentList
{
public:
    ContentRank( const string& class_id, const uint32_t p_type ):ContentList()
    {
        _class_id = class_id;
        _p_type = p_type;
    }
    ContentRank( const string& content_type, uint32_t adapt, const string& class_id, const uint32_t p_type ):
                           ContentList(content_type,adapt)
    {
        _class_id = class_id;
        _p_type = p_type;
    }

    virtual void get_basic_query_bson(bson* query);
    virtual void get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_count_query_bson(bson* query);
    virtual void get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_ios_query_bson(bson* query);
    virtual void getOnlineGameQueryBson(bson* query);

private:
    string _class_id;
    uint32_t _p_type;
};

};

#endif
