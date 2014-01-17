#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <cctype>
#include <algorithm>
using namespace std;

#include "mcpsvr.h"
#include "myclient_include.h"
#include "ub.h"


time_t McpServlet::createTime = 0;

void McpServlet::string_ToLower(string &SrcString)  
{  
    for (string::iterator i = SrcString.begin(); i != SrcString.end(); i++)  
        if (*i >= 'A' && *i <= 'Z')  
            *i = (*i) + ('a' - 'A');  
}  


void McpServlet::AddAKey(string key, string id)
{
    int i = 0;
//    int j = 0;
//    int index = 0;
    TrieNode *Root = &(McpServlet::trie);
    int len =0 ;
    len = key.size();
    string_ToLower(key);
    map<char, struct TrieNode*>::iterator iter;
    TrieNode *pos;

    for(i=0;i<len;i++)
    {
        iter = Root->Next.find(key[i]);
//        index = Determine(key[i]);
        if(iter == Root->Next.end())
        {
            pos = new TrieNode ;
//            memset(pos, 0x00, sizeof(TrieNode));
            pos->c = key[i];
//            Root->Next.insert(map<char, struct TrieNode*>::value_type(key[i],pos));
            Root->Next[key[i]] = pos;
            Root = pos;
        }
        else
        {
            Root = iter->second;
        }
        // id列表的追加
    }
//    cout << Root->id_list.size() << endl;
//    cout << Root->id_list.max_size() << endl;
  //  cout<< key << endl;
  //  cout<< id << endl;
    Root->id_list.push_back( id );
}



int McpServlet::Search(string key,vector<string> *id)
{
//    int index = 0;
    int i = 0;
    int len = 0;
    TrieNode *Root = &trie;
    vector<string> id_list;
    len = key.size();
    string_ToLower(key);
    map<char, struct TrieNode*>::iterator iter;
    TrieNode *pos;
    pos = Root;

    for(int j =0; j <len; j++)
    {
        Root = pos;
        id_list.clear();
        for(i=j;i<len;i++)
        {
           iter = Root->Next.find(key[i]);
           if(iter != Root->Next.end())
           {
              Root=iter->second;
              if(  !Root->id_list.empty() )
              {
                  id_list = Root->id_list;
              }
           }
           else if ( !id_list.empty() )
           {
               id->insert( id->end(), id_list.begin(), id_list.end());
               continue;
           }
           else
           {
               continue; 
           }
        }
        if ( ( i ==  len) && (!id_list.empty()) )
        {
            id->insert( id->end(), id_list.begin(), id_list.end());
            continue;
        }
    }
    if( 0 == id->size() )
    {
        return -1;
    }

    return 0;
} 

int McpServlet::create_trie_list()
{
    int ret,err;
    MyclientRes data_res;
    char query_sql[1024];
    memset(query_sql, 0x00, sizeof(query_sql));
    MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        data_res.free();
        return MCP_NG;
    }
    sprintf(query_sql,
       "select mcp_phone_ua.uakey,mcp_phone_ua.p_id from mcp_phone_ua "
       "INNER JOIN mcp_phone_info ON mcp_phone_info.id = mcp_phone_ua.p_id "
       "where mcp_phone_ua.enable = 1 AND mcp_phone_info.status = 1"
       );
    ret = database_connection->query(query_sql, &data_res, true);
    if ( ret < 0 )
    {
        UB_LOG_FATAL("query data from database failed");
        ret = database_pool->putBackConnection(database_connection, false);
        if (ret != 0)
        {
           ul_writelog(UL_LOG_FATAL, "putback connection is failed");
        }
        data_res.free();
        return MCP_NG;
    }
    for(int i = 0; i < ret; i++)
    {
        AddAKey(data_res[i][0],data_res[i][1]);
    }
    data_res.free();
    time(&this->createTime);

}


void McpServlet::delete_trie_list(TrieNode* deltrie)
{
    map<char, struct TrieNode*>::iterator iter;
    for(iter = deltrie->Next.begin(); iter != deltrie->Next.end(); iter++)
    {
        while(NULL != iter->second)
        {
            if(0 == iter->second->Next.size())
            {
                delete iter->second;
                iter->second= NULL;
                deltrie->Next.erase(iter);
            }
            else
            {
                delete_trie_list(iter->second);
            }
        }
    }
}

int McpServlet::isTimeout(unsigned long int expired){
    time_t now;
    time(&now);

    UB_LOG_NOTICE("%p %d %d",this,now,this->createTime);  

    if(now - this->createTime > expired){
        return 1;
    }else{
        return 0;
    }

}
