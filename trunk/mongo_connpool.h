#ifndef MONGO_CONNPOOL_H
#define MONGO_CONNPOOL_H

#include "mongo.h"
#include <bsl/deque.h>
#include <bsl/exception.h>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <bsl/string.h>
#include <time.h>
#include <pthread.h>
#include "myclient_include.h"

const int mongo_server_num = 2;
const int mongo_server_max_size = 200;

extern std::string mongo_host[mongo_server_num];
extern int mongo_port[mongo_server_num];

class mongo_connpool
{
	public:
                //mongo_connpool();
		~mongo_connpool();
                //返回指针为NULL时 连接失败
		mongo* getConnection();   //get one connection
		void releaseConnection(mongo* conn);   //release one connection
		mongo* getConnectionTry(int maxNum);   //try connect to mongo server maxNum times
		static mongo_connpool* getInstance();  //get ConnPool handle
	protected:
		mongo_connpool(std::string host[], int port[], int maxSize );
	private:
		char host[mongo_server_num][128]; 
		int port[mongo_server_num];
		struct timeval timeout;
		int db;
		static bool b_lock;
		bsl::deque<mongo*> conns; //connect queue
		int curSize;         //current connection nums
		int maxSize;         //max connection nums
		pthread_mutex_t locker;
		static mongo_connpool* connPool;
		mongo* createConnection();  //create one connection
		void terminateConnection(mongo* conn);   //end one connection
		void init(int initialSize); //initial connect Pool
		void destroy();    //destroy connect Pool
		static void lockMutex(bool& b_lock);
		static void unlockMutex(bool& b_lock);
};

#endif
