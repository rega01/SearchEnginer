//SearchServer.h
#ifndef __EchoServer_H__
#define __EchoServer_H__

#include "TcpServer.h"
#include "ThreadPool.h"

class SearchServer
{
public:
    SearchServer(int poolSize,int queueSize,const string & ip,unsigned short port);
    void start();
    void stop();
    void setTcpAllCallback();
    void setLRUCallback();
    void onNewConnection(const shared_ptr<TcpConnection> & onNewConnection);
    void onMessage(const shared_ptr<TcpConnection> & onNewConnection);
    void onClose(const shared_ptr<TcpConnection> & onNewConnection);
    ~SearchServer();

private:
    ThreadPool _pool;
    TcpServer _tcp;
};

#endif 

