/*
 * =====================================================================================
 *
 *       Filename:  IpGateway.cpp
 *
 *    Description:  IP网关查询的相关实现
 *
 *        Version:  1.0
 *        Created:  09/03/2012 09:38:42 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  RD. Li,Yong 
 *          Email:  liyong@duoku.com & linuxyong@gmail.com
 *        Company:  Duoku, Baidu, Beijing, China
 *
 * =====================================================================================
 */

#include "ip_gateway.h"
#include "duokoo_mysql.h"

IpGateway* IpGateway::pIpGateway=NULL;

GatewayInfo& GatewayInfo::operator = (const GatewayInfo& gateway)
{
    this->network = gateway.network;
    this->province = gateway.province;
    return *this;
}

IpGatewayNode::IpGatewayNode(const int n)
{
    //child=NULL;
    num=n;
}

IpGatewayNode::~IpGatewayNode()
{
    map<int, IpGatewayNode*>::iterator iter=next.begin();
    for( ; iter!=next.end(); ++iter ){
        delete iter->second;
    }
    next.clear();
}

void IpGatewayNode::insert(IpGatewayNode* node)
{
    pair<map<int,IpGatewayNode*>::iterator, bool> ret;
    ret=next.insert( pair<int,IpGatewayNode*>(node->num, node));
    if(ret.second==false){
        UB_LOG_FATAL( "insert failed, current[%d], child[%d], [%s:%d]",
                       this->num, ret.first->first, __FILE__, __LINE__ );
        delete node;
    }
    return;
}

IpGatewayNode* IpGatewayNode::find(int decimal)
{
    map<int, IpGatewayNode*>::iterator iter;
    iter=next.find(decimal);
    if(iter==next.end()){
        return NULL;
    }
    return iter->second;
}

void IpGatewayNode::setGateway(const string& network, const string& province)
{
    gatewayInfo.network = atoi( network.c_str() );
    gatewayInfo.province = atoi( province.c_str() );
}

GatewayInfo IpGatewayNode::getGateway()
{
    return this->gatewayInfo;
}

IpGateway::IpGateway()
{
    _root= new IpGatewayNode;
    this->init();
}

IpGateway::~IpGateway()
{
    this->destory();
}

int IpGateway::init()
{
    mcp_mysql conn;
    string query_sql="SELECT `ip`, `network`, `province_id` FROM `MCP`.`mcp_gateway_ip` WHERE `enable`=1 ";
    int count_num(0);
    count_num=conn.query(query_sql);
    if(count_num<0){
        UB_LOG_FATAL( "query gateway info failed, sql[%s], [%s:%d]", query_sql.c_str(), __FILE__, __LINE__ );
        return -5;
    }else if( count_num == 0 ){
        UB_LOG_FATAL( "query gateway info has no record, sql[%s], [%s:%d]",
                                                         query_sql.c_str(), __FILE__, __LINE__ );
        return -1;
    }
    int res(0);
    for(int i=0; i<count_num; ++i){
       UB_LOG_DEBUG( "ip[%s], network[%s], province[%s]", 
               conn.get_result(i, 0).c_str(), conn.get_result(i, 1).c_str(), conn.get_result(i, 2).c_str() );
       res = this->addIpGateway( conn.get_result(i, 0), conn.get_result(i, 1), conn.get_result(i, 2) );
       if(res<0){
           UB_LOG_FATAL( "addIpGateway failed, ip[%s], network[%s], province[%s], [%s;%d]",
               conn.get_result(i, 0).c_str(), conn.get_result(i, 1).c_str(), conn.get_result(i, 2).c_str(),
               __FILE__, __LINE__ );
       }
    }
    return 0;
}

void IpGateway::destory()
{
     delete _root;
}

IpGateway* IpGateway::getInstance()
{
    if(pIpGateway==NULL){
        pIpGateway = new IpGateway();
    }
    return pIpGateway;
}

void IpGateway::reload()
{
    if(pIpGateway!=NULL){
        delete pIpGateway;
    }
    pIpGateway = new IpGateway();
    return;
}

int IpGateway::findIpGateway( const string& ipAddr, GatewayInfo& gateway )
{
    vector<int> decimal;
    ipStringToIntArray(ipAddr, decimal);
    IpGatewayNode *r=_root, *c=_root;
    vector<int>::iterator iter = decimal.begin();
    for( ; iter!=decimal.end(); ++iter ){
        c=r->find(*iter);
        if(c==NULL){
            gateway = r->getGateway();
            return 1;
        }
        r=c;
    }
    gateway = c->getGateway();
    return 0;
}

int IpGateway::addIpGateway( const string& ipAddr, const string& network, const string& province )
{
    vector<int> decimal;
    ipStringToIntArray(ipAddr, decimal);
    IpGatewayNode* r=_root, *c=NULL;
    vector<int>::iterator iter = decimal.begin();
    for( ; iter!=decimal.end(); ++iter ){
        c=r->find(*iter);
        if(c==NULL){
            c=new IpGatewayNode(*iter);
            r->insert(c);
        }
        r=c;
    }
    c->setGateway( network, province );
    return 0;
}

int IpGateway::ipStringToIntArray(const string& ipAddr, vector<int>& decimal )
{
    //decimal.resize( 4, 0 );
    decimal.clear();
    int i(0);
    size_t start(0), end(0);
    string tmp;
    while(start<ipAddr.size() && i<4){
        end=ipAddr.find( '.', start );
        //decimal[i]=atoi(idAddr.substr(start,end-start).c_str());
        tmp=ipAddr.substr(start,end-start);
        if(tmp=="*"){
            break;
        }
        decimal.push_back(atoi(tmp.c_str()));
        start=end+1;
        ++i;
    }
    return 0;
}

int IpGateway::getIpGateway(const string& ipAddr, GatewayInfo& gateway)
{
    IpGateway *p = IpGateway::getInstance();
    p->findIpGateway(ipAddr, gateway);
    return 0;
}
