//TcpServer.h
#ifndef __TcpServer_H__
#define __TcpServer_H__

#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer
{
public:
    TcpServer(unsigned short port);
    TcpServer(const std::string &ip,unsigned short port);
    void start();
    void stop();
    void setAllCallback(std::function<void(std::shared_ptr<TcpConnection>)> && cb1,
                        std::function<void(std::shared_ptr<TcpConnection>)> && cb2,
                        std::function<void(std::shared_ptr<TcpConnection>)> && cb3);
    void setLRUCallback(std::function<void()> &&cbSyn);
    ~TcpServer(){}

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif 

