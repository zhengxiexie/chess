#ifndef _CONTENT_CLASS_H_
#define _CONTENT_CLASS_H_

#include "content_list.h"

namespace content{

class ContentClass: public ContentList
{
public:
    ContentClass( const string& class_id ):ContentList()
    {
        _class_id = class_id;
    }
    ContentClass(const string& content_type, uint32_t adapt, const string& class_id):ContentList(content_type,adapt)
    {
        _class_id = class_id;
    }
    ContentClass( const string& content_type, uint32_t adapt, const string& class_id, const uint32_t page, const uint32_t page_size):ContentList(content_type,adapt)
    {
        _class_id = class_id;
        _page = page;
        if(_page <= 1)
        {
            _page = 1;
        }
        _page_size = page_size;
    }

    int get_content_list_with_weight(int32_t &count, vector<content_info_t>& content_info_list, int32_t weight_type);
    int get_content_list_all(int32_t weight_type = 0, vector<content_info_t>& content_info_list=NULL);
    int get_content_basic_info(const vector<string>& id_list, vector<basic_info_t>& basic_list);
    int get_content_soft_list(vector<soft_info_t>& soft_list);
    int get_id_list(vector<string>& id_list, int32_t weight_type = 0);
    int get_id_list(vector<string>& id_list, vector<string>& except_class_ids, uint32_t priority, uint32_t& count);
    int get_android_theme_basic_list(vector<android_theme_t>& theme_list);
    int get_ios_content_list(int32_t& count, vector<ios_content_t>& list);
    int get_ios_content_list(vector<ios_content_t>& list);
    int getOnlineGameList(int32_t& count, vector<online_game_t>& list);
    int getOnlineGameList(vector<online_game_t>& list);
    int combine_content_list(const vector<basic_info_t>& basic_list, const vector<download_info_t>& download_list, vector<content_info_t>& list);
    void get_count_query_bson(bson* query, int32_t weight_type = 0);
    virtual void get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query, int32_t weight_type);
    virtual void get_basic_query_bson(bson* query);
    virtual void get_basic_query_bson(const vector<string>& id_list, bson* query);
    virtual void get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_soft_query_bson(bson* query);
    virtual void get_android_theme_query_bson(bson* query);
    void get_soft_query_bson(const vector<string>& id_list, bson* query);
    void get_android_theme_query_bson(const vector<string>& id_list, bson* query);
    virtual void get_id_list_query_bson(bson* query, int32_t weight_type = 0);
    virtual void get_id_list_query_bson(bson* query, vector<string>& except_class_ids, uint32_t priority);
    virtual void get_id_list_query_bson2(bson* query, vector<string>& except_class_ids, uint32_t priority);
    virtual void get_id_list_fields_bson(bson* fields);
    virtual void get_id_list_fields_bson2(bson* fields);
    virtual void get_ios_query_bson(bson* query);

    virtual void getOnlineGameQueryBson(bson *query);
    virtual void getOnlineGameCommand(bson *command);
    int get_soft_list_with_except_class_ids(vector<soft_info_t>& soft_list, vector<string>& except_class_ids, uint32_t priority, uint32_t& count);

private:
    string _class_id;
    uint32_t _page;
    uint32_t _page_size;
};

};

#endif
