#ifndef __WebPageSearcher_H__
#define __WebPageSearcher_H__

#include "travial.h"
#include "WebPageQuery.h"
#include "RedisServer.h"

class TcpConnection;
class LRU;

class WebPageSearcher
{
public:
    WebPageSearcher(string word,shared_ptr<TcpConnection> ptcp=nullptr,LRU * cacheLocal=nullptr);
    void doQuery();
    void response();
    ~WebPageSearcher();

private:
    string _sought;
    priority_queue<pair<WebPage,double>,vector<pair<WebPage,double>>,MyCompareWebpage> _resultQue;
    shared_ptr<TcpConnection> _conn;
    RedisServer _redisServer;
    LRU * _cacheLocal;
};

#endif 

