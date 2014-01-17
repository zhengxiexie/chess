/*
 * =====================================================================================
 *
 *       Filename:  IpGateway.h
 *
 *    Description:  IP网关查询数据结构的相关定义
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

#ifndef _IPGATEWAY_H_
#define _IPGATEWAY_H_

#include <string>
#include <vector>
#include <map>
using namespace std;

struct GatewayInfo
{
    int network;
    int province;
    GatewayInfo()
    {
        network=0;
        province=0;
    }
    GatewayInfo& operator = (const GatewayInfo& );
/*
    GatewayInfo(const string& network, const string& province)
    {
        this->network=atoi(network.c_str());
        this->province=atoi(province.c_str());
    }
*/
};

class IpGatewayNode
{
public:
    IpGatewayNode(int n=0);
    ~IpGatewayNode();
    void insert(IpGatewayNode*);
    IpGatewayNode* find(int);
    void setGateway(const string& network, const string& province);
    GatewayInfo getGateway();
private:
    int num;
    map<int, IpGatewayNode*> next;
    //IpGatewayNode* child;
    GatewayInfo gatewayInfo;
};

class IpGateway
{
public:
    ~IpGateway();
    int init();
    void destory();

    static IpGateway* getInstance();
    static void reload();
    int findIpGateway(const string& ipAddr, GatewayInfo& gateway);
    int addIpGateway(const string& ipAddr, const string& network, const string& province);

    static int ipStringToIntArray( const string& ipAddr, vector<int>& decimal );
    static int getIpGateway(const string& ipAddr, GatewayInfo& gateway);
protected:
    IpGateway();
private:
	IpGatewayNode* _root;
    static IpGateway* pIpGateway;
};

#endif
