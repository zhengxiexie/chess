/*
 * =====================================================================================
 *
 *       Filename:  UADicTree.cpp
 *
 *    Description:  在张高基础上，对字典树进行封装成一个类
 *
 *        Version:  1.0
 *        Created:  08/11/2011 03:16:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  RD. Hongming li (lhm), lihongming@baidu.com
 *        Company:  baidu, Beijing, China
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <cctype>
#include <algorithm>
#include "UADicTree.h"
#include "myclient_include.h"
#include "mcpsvr.h" 
using namespace std;


void UADicTree::string_ToLower(string &SrcString)  
{  
    for (string::iterator i = SrcString.begin(); i != SrcString.end(); i++)  
        if (*i >= 'A' && *i <= 'Z')  
            *i = (*i) + ('a' - 'A');  
}  


void UADicTree::AddAKey(string key, string id)
{
    int i = 0;
//    int j = 0;
//    int index = 0;
    TrieNode *Root = &trie;
    int len =0 ;
    len = key.size();
    string_ToLower(key);
    map<char, struct TrieNode*>::iterator iter;
    TrieNode *pos;

    //UB_LOG_DEBUG( "AddAKey [%s:%s]", id.c_str(), key.c_str() );
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
//    cout<< key << endl;
//    cout<< id << endl;
    Root->id_list.push_back( id );
}



int UADicTree::Search(string key,vector<string> *id)
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
    int count_len=0;
    int tmp_len=0;

    UB_LOG_NOTICE("search key is %s",key.c_str());

    for(int j =0; j <len; j++)
    {
        Root = pos;
        id_list.clear();
        tmp_len=0;
        for(i=j;i<len;i++)
        {
           // 匹配树中的字符
/*
           for(iter =Root->Next.begin(); iter!=Root->Next.end(); ++iter){
               cout<<iter->second->c<<" ";
           }
           cout<<endl;
*/
           iter = Root->Next.find(key[i]);
           //UB_LOG_DEBUG("current key[%c]", key[i] );
           if(iter != Root->Next.end())   
           // 在树下一层中找到匹配字符
           {
           //UB_LOG_DEBUG("[%c] not end", key[i] );
              // 节点下移
              Root=iter->second;
              // 匹配到的长度加1
              tmp_len++;
              if(  !Root->id_list.empty() )
              // 匹配到的字符正好为一个pid
              {
                  UB_LOG_NOTICE("uakey is find,id is %s",Root->id_list[0].c_str());
                  // 获取pid列表
                  id_list = Root->id_list;
              }
           }
           else if ( !id_list.empty() && (tmp_len > count_len))
           // 在树的下层中未匹配到字符，并且获取到过pid列表，并且匹配长度超过之前的长度
           {
           //UB_LOG_DEBUG("[%c] tmp_len greate than count_len", key[i] );
               id->clear();
               // 清空pid列表
               id->insert( id->end(), id_list.begin(), id_list.end());
               // 清空临时pid列表
               id_list.clear();
               // 匹配长度重新设置
               count_len = tmp_len;
               UB_LOG_NOTICE("count_len is %d",count_len);
               break;
           }
           else
           {
           //UB_LOG_DEBUG("[%c] else", key[i] );
               break; 
           }
        // 正好匹配整个ua
            if ( ( i ==  (len-1)) && (!id_list.empty()) && (tmp_len > count_len))
            {
                id->clear();
                id->insert( id->end(), id_list.begin(), id_list.end());
                id_list.clear();
                count_len = tmp_len;
                UB_LOG_NOTICE("uakey is full find, count_len is %d", count_len);
                continue;
            }
        }
    }
    if( 0 == id->size() )
    {
        UB_LOG_NOTICE("uakey is not find");
        return -1;
    }
    sort(id->begin(),id->end());
    id->erase(unique(id->begin(),id->end()),id->end());
    UB_LOG_NOTICE("id size is %d",(int)id->size());

    return 0;
} 

int UADicTree::create_trie_list()
{
    pthread_mutex_lock(&locker);   // 上线程锁
    int ret,err;
    MyclientRes data_res;
    char query_sql[1024];
    memset(query_sql, 0x00, sizeof(query_sql));
    MysqlConnection * database_connection = database_pool->fetchConnection("default_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        data_res.free();
        pthread_mutex_unlock(&locker);   // 解线程锁
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
        pthread_mutex_unlock(&locker);   // 解线程锁
        return MCP_NG;
    }
    for(int i = 0; i < ret; i++)
    {
        AddAKey(data_res[i][0],data_res[i][1]);
    }
    data_res.free();
    ret = database_pool->putBackConnection(database_connection, false);
    if (ret != 0)
    {
       ul_writelog(UL_LOG_FATAL, "putback connection is failed");
    }
    time(&this->createTime);
    pthread_mutex_unlock(&locker);   // 解线程锁
    return 0;
}


void UADicTree::delete_trie_list(TrieNode* deltrie)
{
    pthread_mutex_lock(&locker);   // 上线程锁
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
    pthread_mutex_unlock(&locker);   // 解线程锁
}

void UADicTree::delete_trie(){
    UADicTree::delete_trie_list(&trie);
}
void UADicTree::updateCreateTime(){
    time(&this->createTime);
}

int UADicTree::isTimeout(long expired){
    time_t now;
    time(&now);

    if(now - this->createTime > expired){
        return 1;
    }else{
        return 0;
    }
}



//added on 2013/09/03
void UAJsonDicTree::string_ToLower(string &SrcString)  
{  
    for (string::iterator i = SrcString.begin(); i != SrcString.end(); i++)  
        if (*i >= 'A' && *i <= 'Z')  
            *i = (*i) + ('a' - 'A');  
}  


void UAJsonDicTree::AddAKey(string ua, string uaJson)
{
    int i = 0;
//    int j = 0;
//    int index = 0;
    UaJsonNode *Root = &uaJsonNode;
    string_ToLower(ua);
    int len =0 ;
    len = ua.length();
    int nNlen = ua.rfind("\n"); //2013.09.16 数据库数据问题，抓取的数据中有\n(其长度是1),这必然是匹配不上的
    if(nNlen == len - 1)
    {
        ua = ua.substr(0,len - 1);
        len = ua.length();
    }
    map<string, struct UaJsonNode*>::iterator iter;
    UaJsonNode *pos;

    //UB_LOG_DEBUG( "AddAKey [%s:%s]", id.c_str(), key.c_str() );
    for(i=0;i<len;i+=nFixedLen)
    {
        string item = ua.substr(i,nFixedLen);
        iter = Root->Next.find(item);
//        index = Determine(key[i]);
        if(iter == Root->Next.end())
        {
            pos = new UaJsonNode;
//            memset(pos, 0x00, sizeof(TrieNode));
            pos->fixedSeg = item;
//            Root->Next.insert(map<char, struct TrieNode*>::value_type(key[i],pos));
            Root->Next[item] = pos;
            Root = pos;
        }
        else
        {
            Root = iter->second;
        }
    }
//    cout << Root->id_list.size() << endl;
//    cout << Root->id_list.max_size() << endl;
//    cout<< key << endl;
//    cout<< id << endl;
    //Root->id_list.push_back( id );
    Root->uaJson = uaJson;
}

int UAJsonDicTree::Search(string ua,string& uaJson)
{
//    int index = 0;
    int i = 0;
    int len = 0;
    UaJsonNode *Root = &uaJsonNode;
    uaJson = "";
    string_ToLower(ua);
    len = ua.length();
    int nNlen = ua.rfind("\n"); //2013.09.16 数据库数据问题，抓取的数据中有\n(其长度是1),这必然是匹配不上的
    if(nNlen == len - 1)
    {
        ua = ua.substr(0,len - 1);
        len = ua.length();
    }

    map<string, struct UaJsonNode*>::iterator iter;
    //UaJsonNode *pos;
    //pos = Root;

    UB_LOG_NOTICE("search key is %s",ua.c_str());
    //map<int,string> uaJsonMap;

    //for(int j =0; j <len; j++) // modified by laixiaokang 2013.09.04 去掉外层循环，完全匹配，不会出现前面或后面多出的字符串
    {
        //Root = pos;
        //map<int,string> uaJsonMap;
        //uaJsonMap.clear();
        int nIndex = 0;
        //bool bIsEnd = false;
        for(i=0;i<len;i+=nFixedLen)
        {
           string item = ua.substr(i, nFixedLen);
           iter = Root->Next.find(item);
           if(iter != Root->Next.end())   
           {
              // 节点下移
              Root=iter->second;
              if( !Root->uaJson.empty() )
              {
                  nIndex++;
                  //uaJsonMap.insert(make_pair(nIndex, Root->uaJson));
                  if((i+nFixedLen) >= len)
                  {
                      uaJson = Root->uaJson;
                      return 0;
                  }
                  //UB_LOG_NOTICE("uakey is find,uajson is %s",uaJson);
                  //return uaJson;
              }
           }
           else
           {
               break;
           }
           /*
           else
           {
               int nSize = uaJsonMap.size();
               if(nSize>=1)
               {
                   UB_LOG_NOTICE("uakey is find,uajson is %s",uaJsonMap[nSize].data());
                   uaJson = uaJsonMap[nSize];
                   return 0;
               }
               else
               {
                   //if( i >= (len - nFixedLen*2) && i < len)
                   {
                       for(int s = nFixedLen; s >= 1; s--)
                       {
                           string strItem = item.substr(0,s);
                           map<string, struct UaJsonNode*>::iterator itemIter = Root->Next.find(strItem);
                           if(itemIter != Root->Next.end())
                           {
                               if(!itemIter->second->uaJson.empty())
                               {
                                   uaJson = itemIter->second->uaJson;
                                   return 0;
                               }
                           }
                       }
                       //return 0;
                   }
                   break;
               }
           }
           */
        }
    }

    return 0;
} 

int UAJsonDicTree::create_uajson_list()
{
    pthread_mutex_lock(&locker);   // 上线程锁
    int ret,err;
    MyclientRes data_res;
    memset(&data_res, 0x00, sizeof(data_res));
    data_res.free();

    char query_sql[1024];
    memset(query_sql, 0x00, sizeof(query_sql));
    MysqlConnection * database_connection = database_pool->fetchConnection("uajson_mysql", &err, 0);
    if ( NULL == database_connection )
    {
        ul_writelog(UL_LOG_FATAL, "get database connection failed from database_pool");
        data_res.free();
        pthread_mutex_unlock(&locker);   // 解线程锁
        return MCP_NG;
    }
    for(int j = 0; j <= 29; j++) //线上库的数据，已经超过百万条，分开查询，初始化树  2013.09.17
    {
        data_res.free();
        memset(&data_res, 0x00, sizeof(data_res));
        memset(query_sql, 0x00, sizeof(query_sql));
        sprintf(query_sql,"SELECT uaLongStr,uajson FROM MCP.userAgentInfo where enable = 1 and id % 30 = %d", j);
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
            pthread_mutex_unlock(&locker);   // 解线程锁
            return MCP_NG;
        }
        for(int i = 0; i < ret; i++)
        {
            //UB_LOG_FATAL("key=%s,value=%s",data_res[i][0], data_res[i][1]);
            AddAKey(data_res[i][0],data_res[i][1]);
        }
    }
    data_res.free();
    ret = database_pool->putBackConnection(database_connection, false);
    if (ret != 0)
    {
       ul_writelog(UL_LOG_FATAL, "putback connection is failed");
    }
    pthread_mutex_unlock(&locker);   // 解线程锁
    return 0;
}

void UAJsonDicTree::destroyTree(struct UaJsonNode* pNode)
{
    pthread_mutex_lock(&locker);
    if(pNode != NULL)
    {
        map<string ,struct UaJsonNode*>& refMap = pNode->Next;
        map<string ,struct UaJsonNode*>::iterator iter = refMap.begin();
        for(; iter != refMap.end(); iter++)
        {
            struct UaJsonNode* pItem = iter->second;
            if(pItem != NULL)
            {
                destroyTree(pItem);
            }
        }
        
        refMap.clear();
        delete pNode;
        pNode = NULL;
    }
    pthread_mutex_unlock(&locker); 
}
