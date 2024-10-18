//TcpConnection.h
#ifndef __TcpConnection_H__
#define __TcpConnection_H__

#include "SockIo.h"
#include "Socket.h"
#include "InetAddr.h"


#include <string>
#include <memory>
#include <functional>

class EventLoop;

class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{
public:
    /* TcpConnection(int); */
    TcpConnection(int,EventLoop*);
    std::string receive();
    void send(string & res);
    bool isClosed();
    void printConnect() const;

    void setAllCallback(std::function<void(const std::shared_ptr<TcpConnection>)> & cb1,
                        std::function<void(const std::shared_ptr<TcpConnection>)> & cb2,
                        std::function<void(const std::shared_ptr<TcpConnection>)> & cb3);
    void handleNewConnection();
    void handleMessage();
    void handleClose();
    ~TcpConnection();

public:
    void sendToLoop(string  res);
private:
    InetAddr getlocalAddr();
    InetAddr getpeerAddr();

    SockIo _sockio;
    Socket _sock;
    InetAddr _localAddr;
    InetAddr _peerAddr;

    std::function<void(std::shared_ptr<TcpConnection>)> _onNewConnection;
    std::function<void(std::shared_ptr<TcpConnection>)> _onMessage;
    std::function<void(std::shared_ptr<TcpConnection>)> _onClose;

    EventLoop * _loop;
};

#endif 

