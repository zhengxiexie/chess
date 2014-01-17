#ifndef _REDIS_CONNPOOL_H_
#define _REDIS_CONNPOOL_H_

#include "hiredis.h"
#include "bsl/deque.h"

static struct timeval redis_dft_tmout={1,500000};
extern char REDIS_HOST[16];
extern char REDIS_PORT[6];
extern uint32_t g_redis_db;

class redisConnPool
{
public:
    ~redisConnPool();
    static redisConnPool* getInstance();  //get ConnPool handle
    //返回指针为NULL时 连接失败
    redisContext* getConnection();   //get one connection
    void releaseConnection(redisContext* conn);   //release one connection
    redisContext* getConnectionTry(int maxNum);   //try connect to redis server maxNum times

protected:
    redisConnPool(char* host, int port, int db, struct timeval timeout=redis_dft_tmout);

private:
    char host[128];
    int port;
    struct timeval timeout;
    int db;
    bsl::deque<redisContext*> conns;
    int curSize;
    int maxSize;
    static bool b_lock;
    pthread_mutex_t locker;
    static redisConnPool* connPool;
    redisContext* createConnection();  //create one connection
    void terminateConnection(redisContext* conn);   //end one connection
    void init(int initialSize); //initial connect Pool
    void destroy();    //destroy connect Pool
    static void lockMutex(bool& b_lock);
    static void unlockMutex(bool& b_lock);
};

#endif
