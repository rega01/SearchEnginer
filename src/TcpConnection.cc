//TcpConnection.cc
#include "TcpConnection.h"
#include "EventLoop.h"
#include <string.h>
#include <iostream>

using namespace std;

TcpConnection::TcpConnection(int connFd,EventLoop * loop)
:_sockio(connFd)
,_sock(connFd)
,_localAddr(getlocalAddr())
,_peerAddr(getpeerAddr())
,_loop(loop)
{}

std::string TcpConnection::receive(){
    char buff[2048] = {0};
    _sockio.readJsonStr(buff,sizeof(buff));
    /* printf("size={%lu},{%s}\n",strlen(buff),buff); */
    return string(buff);
}

void TcpConnection::send(string & res){
    _sockio.sendJsonStr(res.c_str(),res.size());
}

void TcpConnection::printConnect() const{
    cout << _localAddr.getIP() <<":" << _localAddr.getPort() <<"----->"
        << _peerAddr.getIP()<<":" << _peerAddr.getPort() << endl;
}

InetAddr TcpConnection::getlocalAddr(){
    struct sockaddr_in localAddr;
    socklen_t len = sizeof(localAddr);
    ::memset(&localAddr,0,sizeof(localAddr));
    int ret = getsockname(_sock.fd(),(struct sockaddr *)&localAddr,&len);
    if(ret ==-1){
        perror("getsockname");
    }
    return InetAddr(localAddr);
}

InetAddr TcpConnection::getpeerAddr(){
    struct sockaddr_in localAddr;
    socklen_t len = sizeof(localAddr);
    ::memset(&localAddr,0,sizeof(localAddr));
    int ret = getpeername(_sock.fd(),(struct sockaddr *)&localAddr,&len);
    if(ret ==-1){
        perror("getpeername");
    }
    return InetAddr(localAddr);
}

bool TcpConnection::isClosed(){
    char buff[1] ={0};
    int ret = recv(_sock.fd(),buff,sizeof(buff),MSG_PEEK);
    return ret == 0;
}

void TcpConnection::setAllCallback(std::function<void(const std::shared_ptr<TcpConnection>)> & cb1,
                    std::function<void(const std::shared_ptr<TcpConnection>)> & cb2,
                    std::function<void(const std::shared_ptr<TcpConnection>)> & cb3){
    _onNewConnection = cb1;
    _onMessage = cb2;
    _onClose = cb3;
}

//三个回调函数都是在EchoServer中定义
void TcpConnection::handleNewConnection(){
    if(_onNewConnection){
        _onNewConnection(shared_from_this());
    }else{
        cout << "_onNewConnection == nullptr" << endl;
    }
}
void TcpConnection::handleMessage(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }else{
        cout << "_onMessage == nullptr" << endl;
    }

}
void TcpConnection::handleClose(){
    if(_onClose){
        _onClose(shared_from_this());
    }else{
        cout << "_onClose == nullptr" << endl;
    }
}

//当task中process处理完数据后，会调用函数，放入loop待执行任务中
void TcpConnection::sendToLoop(string res){
    _loop->runInLoop(bind(&TcpConnection::send,this,res));
}

TcpConnection::~TcpConnection(){

}
