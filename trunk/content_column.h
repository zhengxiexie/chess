#ifndef _CONTENT_COLUMN_H_
#define _CONTENT_COLUMN_H_

#include "content_list.h"

namespace content{

class ContentColumn: public ContentList
{
public:
    ContentColumn( const string& column_id ):ContentList()
    {
        _column_id = column_id;
    }
    ContentColumn( const string& content_type, uint32_t adapt, const string& column_id ):ContentList(content_type,adapt)
    {
        _column_id = column_id;
    }


    int get_content_list_all();
    int get_content_basic_info(const vector<string>& id_list, vector<basic_info_t>& basic_list);
    int get_content_soft_list(vector<soft_info_t>& soft_list);
    int get_android_theme_basic_list(vector<android_theme_t>& theme_list);
    int get_ios_content_list(int32_t& count, vector<ios_content_t>& list);
    int get_ios_content_list(vector<ios_content_t>& list);
    int getOnlineGameList(int32_t& count,vector<online_game_t>& list);
    int getOnlineGameList(vector<online_game_t>& list);
    int get_id_list(vector<string>& id_list);
    virtual void get_basic_query_bson(bson* query);
    void get_basic_query_bson(const vector<string>& id_list,bson* query);
    virtual void get_soft_query_bson(bson* query);
    void get_soft_query_command(bson* query);
    void get_soft_query_bson(const vector<string>& id_list, bson* query);
    virtual void get_id_list_query_bson(bson* query);
    virtual void get_id_list_fields_bson(bson* fields);
    virtual void get_android_theme_query_bson(bson* query);
    void get_android_theme_query_bson(const vector<string>& id_list, bson* query);
    virtual void get_basic_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_count_query_bson(bson* query);
    virtual void get_count_query_bson(const map<string,vector<download_info_t> >& m_id_download_info,bson* query);
    virtual void get_ios_query_bson(bson* query);
    void get_ios_query_bson(const vector<string>& id_list, bson* query);
    virtual void getOnlineGameQueryBson(bson *query);
    virtual void getOnlineGameCommand(bson* command);

private:
    string _column_id;
};

};

#endif
