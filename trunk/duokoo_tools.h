#ifndef _DUOKOOTOOLS_H_
#define _DUOKOOTOOLS_H_

#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

namespace DuokooTools{
    string MD5(const string& src);
    string DES(const string& src);
    int YEAR();
    int MONTH();
    string NOW();
    string timestampToString(const time_t t);
    time_t stringToTimestamp(const string& s, const string& format="%F %T");
    string date(const string& format, time_t t = 0);
    long getRandomNum(const long range);
    string getRandomStr(const uint32_t length);

    template <typename T>
    string toString(const T& t,const uint32_t w=0)
    {
        stringstream ss;
        string str;
        ss<<setw(w)<<setfill('0')<<t;
        ss>>str;
        return str;
    }
};

#endif
