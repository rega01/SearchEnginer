#include "RedisServer.h"

RedisServer::RedisServer()
    :_redis(redisConnect("127.0.0.1",6379))
{
    selectDB(1);
}

void RedisServer::selectDB(int dbNum){
    redisReply* reply =static_cast<redisReply*>(redisCommand(_redis,"select %d",dbNum));
    if(reply->type != REDIS_REPLY_STATUS){
        perror("change DB");
        exit(1);
    }
    freeReplyObject(reply);
}

string RedisServer::getVal(const string& key){
    redisReply* reply =nullptr;
    {
        lock_guard<mutex> lock(_mtx);
        reply = static_cast<redisReply*>(redisCommand(_redis,"GET %s",key.c_str()));
    }
    string res;
    if(reply->type == REDIS_REPLY_STRING){
        res = reply->str;
    }else if(reply->type == REDIS_REPLY_NIL){
    }
    freeReplyObject(reply);
    return res;
}

void RedisServer::setKey(const string& key,const string & val){
    /* redisReply* reply =static_cast<redisReply*>( */
    /*                                             redisCommand(_redis,"SET %s %s",key.c_str(),val.c_str())); */
    redisReply* reply =nullptr;
    {
        lock_guard<mutex> lock(_mtx);
        reply = static_cast<redisReply*>(redisCommand(_redis,"SETEX %s %d %s",key.c_str(),10,val.c_str()));
    }
    if(reply->type != REDIS_REPLY_STATUS){
        perror("set kv");
        exit(1);
    }
    freeReplyObject(reply);

}

RedisServer::~RedisServer(){
    redisFree(_redis);
}

