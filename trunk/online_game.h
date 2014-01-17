#ifndef _ONLINE_GAME_H_
#define _ONLINE_GAME_H_

#include "content_common.h"

namespace content{

class OnlineGame{
    public:
        OnlineGame(const string& id)
        {
            _id = id;
            _conn = mongo_pool->getConnection();
        }
        ~OnlineGame()
        {
            if(_conn!=NULL){
                mongo_pool->releaseConnection(_conn);
            }
        }
        int getInfo(online_game_t& info);
    protected:
        void getDetailFromBson(bson* result, online_game_t& info);
        void getQueryBson(bson* query);
        void getFieldsBson(bson* query);
    private:
        mongo *_conn;
        string _id;
};
   
};
#endif
