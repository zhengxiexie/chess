#include "redis_connpool.h"
#include "ub_server.h"

using namespace std;
using namespace bsl;

const int redis_server_max_size = 50;
redisConnPool* redisConnPool::connPool = NULL;
bool redisConnPool::b_lock = false;

char REDIS_HOST[16] = "10.10.0.29";
char REDIS_PORT[6] = "6382";
uint32_t g_redis_db = 3;
redisConnPool::redisConnPool(char* host, int port, int db, struct timeval timeout)
{
    //设置最大连接数
    this->maxSize = redis_server_max_size;
    //当前连接数
    this->curSize = 0;

    memset(this->host, 0x00, sizeof(this->host));
    strcpy(this->host,host);
    this->port = port;
    this->db = db;
    this->timeout.tv_sec = timeout.tv_sec;
    this->timeout.tv_usec = timeout.tv_usec;
    //初始化线程互斥锁
    pthread_mutex_init(&locker, NULL);
    this->init(maxSize/2);
}

redisConnPool::~redisConnPool()
{
    //UB_LOG_DEBUG( "+++++ call_destroy +++++, [%s:%d]", __FILE__, __LINE__ );
    this->destroy();
    //销毁互斥锁
    pthread_mutex_destroy(&locker);
}

void redisConnPool::lockMutex(bool &b_lock = redisConnPool::b_lock)
{
    while(b_lock){
        ;
    }
    b_lock = true;
}

void redisConnPool::unlockMutex(bool &b_lock = redisConnPool::b_lock)
{
    b_lock = false;
}

redisConnPool* redisConnPool::getInstance()
{
    if(connPool == NULL){ //如果连接池未创建，创建连接池
        lockMutex();  // 上锁
        if(connPool == NULL)  //创建连接池
            connPool = new redisConnPool(REDIS_HOST, atoi(REDIS_PORT), (int)g_redis_db );
        unlockMutex(); //解锁
    }
    return connPool;
}

void redisConnPool::init(int size)
{
    redisContext* conn;
    for(int i = 0; i < size; ){
        conn = this->createConnection(); // 生成redis连接
        if(conn){
            i++;
            conns.push_back(conn); // 放进连接池队列
            curSize++; //当前连接数+1
        }else{
            UB_LOG_FATAL("init redis connpool fail one\n");
        }
    }
//    UB_LOG_DEBUG("init redis connpool complete total %d\n", curSize);
    return;
}

void redisConnPool::destroy()
{
    bsl::deque<redisContext*>::iterator pos;
    pthread_mutex_lock(&locker);   //上线程锁
    for(pos = conns.begin(); pos != conns.end(); ++pos){  //循环释放连接
        this->terminateConnection(*pos);
    }

    curSize = 0; // 重置当前连接数
    conns.clear(); // 重置连接池
    pthread_mutex_unlock(&locker);  //解除线程锁
}

redisContext* redisConnPool::createConnection()
{
    redisContext *conn;
    conn = redisConnectWithTimeout( host, port, timeout);
    if( conn!=NULL ){
        if(conn->err){
            UB_LOG_FATAL( "connection failed host is %s, code[%d], msg[%s]", host, conn->err, conn->errstr);
            return NULL;
        }
        redisReply* reply = (redisReply *)redisCommand( conn, "select %d", g_redis_db );
        if( reply != NULL ){
            //UB_LOG_DEBUG( "reply result type[%d]", reply->type );
            freeReplyObject(reply);
            reply = NULL;
        }
    }
    return conn;
}


void redisConnPool::terminateConnection(redisContext* conn)
{
    if(conn){
        try{
            redisFree(conn);
            conn=NULL;
        }
        catch(...){
            UB_LOG_FATAL("close redis error\n");
        }
    }
    return;
}


redisContext* redisConnPool::getConnection()
{
    redisContext* conn = NULL;
    pthread_mutex_lock(&locker);   // 上线程锁
   // UB_LOG_DEBUG( "+++++++++++++ size[%d] +++++++++++++++++", (int)conns.size() );
    if(conns.size() > 0){   //have free connection
        conn = conns.front();  // 取出连接
        conns.pop_front();   // 压出队列
        if(conn == NULL){
            UB_LOG_WARNING("get a redis conn is null");
            --curSize;
        }
        pthread_mutex_unlock(&locker);  // 线程解锁
        return conn;
    }else{
        if(curSize < maxSize){  //can still create new connection
            conn = this->createConnection();
            if(conn){
                ++curSize;
                pthread_mutex_unlock(&locker);
                return conn;
            }else{
                pthread_mutex_unlock(&locker);
                return NULL;
            }
        }else{
            pthread_mutex_unlock(&locker);
            return NULL;
        }
    }
}

void redisConnPool::releaseConnection(redisContext* conn)
{
    if(conn){
        pthread_mutex_lock(&locker);
        conns.push_back(conn);
        pthread_mutex_unlock(&locker);
    }
    return;
}

redisContext* redisConnPool::getConnectionTry(int maxNum)
{
    redisContext *conn;
    for(int i = 0; i < maxNum; ++i){
        conn = this->getConnection();
        if(conn){
            return conn;
        }else{
            sleep(2);
        }
    }
    return NULL;
}
