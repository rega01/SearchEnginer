//EchoServer.cc
#include "Task.h"
#include "SearchServer.h"

//载入单例模式，防止用户查询卡顿
#include "Dictionary.h"
#include "WebPageQuery.h"

#include<functional>

//集合管理TCPServer和Threadpool
SearchServer::SearchServer(int poolSize,int queueSize,const string & ip,unsigned short port)
:_pool(poolSize,queueSize)
,_tcp(ip,port)
{}

void SearchServer::start(){
    Dictionary::createInstance();   
    WebPageQuery::createInstance();   
    _pool.start();
    _tcp.start();
}

void SearchServer::stop(){
    _pool.stop();
    _tcp.stop();
}

//该类定义回调函数，并将其设置给TCPserver,TCPServer再移交给loop
void SearchServer::setTcpAllCallback(){
    using namespace placeholders;
    _tcp.setAllCallback(bind(&SearchServer::onNewConnection,this,_1),
                        bind(&SearchServer::onMessage,this,_1),
                        bind(&SearchServer::onClose,this,_1));
}


void SearchServer::setLRUCallback(){
    _tcp.setLRUCallback(bind(&ThreadPool::exchangeLRU,&_pool));
}

void SearchServer::onNewConnection(const shared_ptr<TcpConnection> & onNewConnection){
    onNewConnection->printConnect();
    cout << "connected" << endl;
}

void SearchServer::onMessage(const shared_ptr<TcpConnection> & onMessage){
    string rec = onMessage->receive();
    /* cout << "recved{" << rec << "}" << endl; */
    Task task(rec,onMessage);

    _pool.addTask(bind(&Task::process,task,std::placeholders::_1));
}

void SearchServer::onClose(const shared_ptr<TcpConnection> & onClose){
    onClose->printConnect();
    cout << "closed" << endl;
}

SearchServer::~SearchServer(){}
