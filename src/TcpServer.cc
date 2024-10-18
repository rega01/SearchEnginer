//TcpServer.cc
#include "TcpServer.h"

using namespace std;

TcpServer::TcpServer(unsigned short port)
:_acceptor("127.0.0.1",port)
,_loop(_acceptor)
{}

TcpServer::TcpServer(const std::string &ip,unsigned short port)
:_acceptor(ip,port)
,_loop(_acceptor)
{}

void TcpServer::start(){
    _acceptor.startListen();
    _loop.loop();
}

void TcpServer::stop(){
    _loop.unloop();
}

void TcpServer::setAllCallback(std::function<void(std::shared_ptr<TcpConnection>)> && cb1,
                    std::function<void(std::shared_ptr<TcpConnection>)> && cb2,
                    std::function<void(std::shared_ptr<TcpConnection>)> && cb3){
    _loop.setAllCallback(std::move(cb1),std::move(cb2),std::move(cb3));
}

void TcpServer::setLRUCallback(std::function<void()> &&cbSyn){
    _loop.setTimeCb(move(cbSyn));
}
