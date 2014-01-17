#include "game_list.h"
#include "ub_server.h"

using namespace std;
using namespace content;

int GameList::get_game_brief_list(int32_t& count,  vector<game_brief_t>& list)
{
    return ContentList::get_game_brief_list(count, list);
}

int GameList::get_game_brief_list(vector<game_brief_t>& list)
{
    UB_LOG_TRACE( "get_game_list start" );
    return ContentList::get_game_brief_list(list);
    UB_LOG_TRACE( "get_game_list end" );
    return 0;
}

void GameList::get_count_query_bson(bson* query)
{
    UB_LOG_TRACE( "game query count start" );
    vector<string> type;
    vector<string> type2;
    char index[2];
    size_t pos=0;
	string info_type=_info_type;
    string game_type=_game_type;
    while( !info_type.empty() )
    {
        pos = info_type.find(",");
        if(pos != string::npos)
        {
            type.push_back(info_type.substr(0,pos));
            info_type.erase(0,pos+1);
        }
		else
		{
			type.push_back(info_type);
			info_type="";
		}
    }
    
    while( !game_type.empty() )
    {
        pos = game_type.find(",");
        if(pos != string::npos)
        {
            type2.push_back(game_type.substr(0,pos));
            UB_LOG_TRACE("game type is %s", game_type.substr(0,pos).c_str());

            game_type.erase(0,pos+1);
        }
		else
		{
			type2.push_back(game_type);
            UB_LOG_TRACE("game type is %s", game_type.c_str());
			game_type="";
		}
    }
	
    bson_init( query );
    //bson_append_start_object(query,"$query");
	if(type.size()>0)
    {
		bson_append_start_object(query,"type_id");
        bson_append_start_array(query,"$in");
        for(int j=0; j<(int)type.size(); j++)
        {
            if(!type[j].empty())
            {
                memset(index,0,sizeof(index));
                sprintf(index,"%d",j);
				UB_LOG_TRACE("bson append is %s",type[j].c_str());
                bson_append_string(query,index,type[j].c_str());
            }
        }
        bson_append_finish_array(query);
		bson_append_finish_object(query);
    }
    
    if(type2.size()>0)
    {
		bson_append_start_object(query,"game_type");
        bson_append_start_array(query,"$in");
        for(int j=0; j<(int)type2.size(); j++)
        {
            if(!type2[j].empty())
            {
                memset(index,0,sizeof(index));
                sprintf(index,"%d",j);
				UB_LOG_TRACE("bson append is %s",type2[j].c_str());
                bson_append_string(query,index,type2[j].c_str());
            }
        }
        bson_append_finish_array(query);
		bson_append_finish_object(query);
    }

	if( !_c_id.empty() && _c_id.compare("0")!=0 ){
        bson_append_string( query, "c_id", _c_id.c_str() );
        UB_LOG_TRACE("cid is %s", _c_id.c_str());
    }
    else{
        UB_LOG_TRACE("cid is null");
    }
    //bson_append_finish_object(query);

    bson_finish( query );
    UB_LOG_TRACE( "game query count end" );
    return;
}

void GameList::get_game_brief_query_bson(bson* query)
{
	UB_LOG_TRACE( "game brief query start" );
    char index[2];
	vector<string> type;
    vector<string> type2;
    int i=0;
	size_t pos=0;
    string info_type=_info_type;
    string game_type=_game_type;
    while( !info_type.empty() )
    {   
        pos = info_type.find(",");
        if(pos != string::npos)
        {   
            type.push_back(info_type.substr(0,pos));
            UB_LOG_TRACE("type %d is %s", i-1, info_type.substr(0,pos).c_str());
            info_type.erase(0,pos+1);
        }   
        else
        {   
            type.push_back(info_type);
            info_type="";
        }   
    }   

    while( !game_type.empty() )
    {
        pos = game_type.find(",");
        if(pos != string::npos)
        {
            type2.push_back(game_type.substr(0,pos));
            game_type.erase(0,pos+1);
        }
		else
		{
			type2.push_back(game_type);
			game_type="";
		}
    }

    bson_init(query);
    //bson_append_start_object(query,"$query");
    UB_LOG_TRACE("i is %d",i);
    if(type.size()>0)
    {
		//bson_append_start_object(query,"type_id");
        bson_append_start_object( query, "$query" );
        bson_append_start_object( query, "type_id" );
        bson_append_start_array( query, "$in" );
        for(int j=0; j<(int)type.size(); j++)
        {
            if(!type[j].empty())
            {
                memset(index,0,sizeof(index));
                sprintf(index,"%d",j);
                UB_LOG_TRACE("bson append is %s",type[j].c_str());
                bson_append_string(query,index,type[j].c_str());
            }
        }

        bson_append_finish_array(query);
		bson_append_finish_object(query);
        bson_append_finish_object(query);
    }
 
    if(type2.size()>0)
    {
        bson_append_start_object( query, "$query" );
		bson_append_start_object(query,"game_type");
        bson_append_start_array(query,"$in");
        for(int j=0; j<(int)type2.size(); j++)
        {
            if(!type2[j].empty())
            {
                memset(index,0,sizeof(index));
                sprintf(index,"%d",j);
				UB_LOG_TRACE("bson append is %s",type2[j].c_str());
                bson_append_string(query,index,type2[j].c_str());
            }
        }
        bson_append_finish_array(query);
		bson_append_finish_object(query);
		bson_append_finish_object(query);
    }

    if( !_c_id.empty() && _c_id.compare("0")!=0 ){
        bson_append_string( query, "c_id", _c_id.c_str() );
    }
    bson_append_start_object( query, "$orderby" );
    bson_append_int( query, "logdate", -1 );
    bson_append_finish_object( query );
    //bson_append_finish_object(query);
    bson_finish(query);
	UB_LOG_TRACE( "game brief query end" );
    return;
}
/*
void GameList::get_game_brief_fields_bson(bson* fields)
{
    bson_init(fields);
    bson_finish(fields);
    return;
}

*/
