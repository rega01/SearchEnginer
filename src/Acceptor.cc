#include "Acceptor.h"

Acceptor::Acceptor(const std::string &ip,short port)
:_sok()
,_addr(ip,port)
{}

void Acceptor::setReuseIP(){
    int opt = 1;
    int ret = ::setsockopt(_sok.fd(),SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(ret == -1){
        perror("set reuse addr");
    }
}

void Acceptor::setReusePort(){
    int opt = 1;
    int ret = ::setsockopt(_sok.fd(),SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    if(ret == -1){
        perror("set reuse port");
    }
}
void Acceptor::bind(){
    int ret = ::bind(_sok.fd(),(struct sockaddr*)_addr.getAddr(),sizeof(struct sockaddr_in));
    if(ret == -1){
        perror("bind");
    }
}
void Acceptor::listen(){
    int ret = ::listen(_sok.fd(),1280);
    if(ret == -1){
        perror("listen");
    }
}

void Acceptor::startListen(){
    setReuseIP();   
    setReusePort();   
    this->bind();
    this->listen();
}

int Acceptor::accept(){
    int connFd = ::accept(_sok.fd(),nullptr,nullptr);
    if(connFd == -1){
        perror("accept");
    }
    return connFd;
}

Acceptor::~Acceptor(){

}

int Acceptor::fd(){
    return _sok.fd();
}

