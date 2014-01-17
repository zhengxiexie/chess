#ifndef _GRAB_H_
#define _GRAB_H_

#include "content_common.h"
#include "duokoo_tools.h"
#include "refactory/common.h"

using namespace content;
using namespace DuokooTools;

class Grab
{
public:
    Grab(const uint32_t id)
    {
        _id = id;
    }
    int getGrabInfo(grab_t* grab_info=NULL);
    int getSendAmount();
    int setSendAmount(const uint32_t amount);
    int incrSendAmount();
    int userGrab(const string& user_id, string& num);
    int userGrabPre(const string& user_id, string& num);
    int getGrabNum(const string& user_id, string& num, int32_t is_occupied);
    int getUserGrab(const string& user_id, string& num);
    int getUserGrabEx(const string& user_id, string& num, int32_t& occupy);
private:
    uint32_t _id;
    grab_t _info;
};

#endif
