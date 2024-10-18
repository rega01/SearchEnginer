#ifndef __RedisServer_H__
#define __RedisServer_H__

#include "travial.h"

#include <hiredis/hiredis.h>

#include <mutex>

using std::mutex;
using std::lock_guard;

class RedisServer
{
public:
    RedisServer();
    string getVal(const string& key);
    void setKey(const string& key,const string & val);
    ~RedisServer();

private:
    void selectDB(int dbNum);

    redisContext * _redis;   
    mutex _mtx;
};

#endif 

