#include "WebPageSearcher.h"
#include "TcpConnection.h"

#include<nlohmann/json.hpp>


WebPageSearcher::WebPageSearcher(string word,shared_ptr<TcpConnection> ptcp,LRU* cacheLocal)
    :_sought(word)
    ,_conn(ptcp)
     ,_redisServer()
     ,_cacheLocal(cacheLocal)
{}

void WebPageSearcher::doQuery(){
    string resFromLRU =_cacheLocal->get(_sought);
    string resFromRedis;
    if(resFromLRU.size()){
        _conn->sendToLoop(resFromLRU);
        cout << "read from LRU\n";
    }
    else if((resFromRedis = _redisServer.getVal(_sought)).size()){
         cout << "through REDIS\n";
        _conn->sendToLoop(resFromRedis);
    }
    else {
        cout << "read from Xapian\n";
        /* _resultQue = WebPageQuery::createInstance()->doQuery(_sought); */
        _resultQue = WebPageQuery::createInstance()->doXapianQuery(_sought);
        response();
    }
}
void WebPageSearcher::response(){
    int topK = 10 ,pos=0;
    nlohmann::json json;
    json["type"] = "WebPageSeacher";
    json["length"] = 0;

    while(!_resultQue.empty() && pos < topK){
        WebPage web = _resultQue.top().first;
        json["value"][pos]["title"] = web._docTitle;
        json["value"][pos]["url"] = web._docUrl;

        _resultQue.pop();
        json["length"] = ++pos ;
    }
    string backMessage = json.dump(-1,' ',true);
    /* cout << backMessage <<"\n"; */
    _cacheLocal->put(_sought,backMessage);
    _redisServer.setKey(_sought,backMessage);
    _conn->sendToLoop(backMessage);
}

WebPageSearcher::~WebPageSearcher(){}

