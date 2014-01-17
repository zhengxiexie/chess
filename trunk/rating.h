#ifndef _RATING_H_
#define _RATING_H_

#include "content_common.h"

class Rating{
public:
    Rating()
    {
    }
    Rating(const string& c_id)
    {
        setCid(c_id);
    }
    ~Rating()
    {
    }
    void setCid(const string& c_id)
    {
        _c_id = c_id;
        _package_name = getPackageNameByCid(c_id);
    }
    int setRating(const string& user_id, const uint32_t rating);
    int getRating(const string& user_id);
    string getPackageNameByCid(const string& c_id);
    string getRedisKey(const string& user_id);
protected:
private:
    string _c_id;
    string _package_name;
};

#endif
