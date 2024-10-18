#ifndef __EventLoop_H__
#define __EventLoop_H__

#include "Acceptor.h"
#include "TcpConnection.h"

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <mutex>
#include <iostream>


class EventLoop
{
public:
    EventLoop(Acceptor &);
    void loop();
    void unloop();
    void setAllCallback(std::function<void(std::shared_ptr<TcpConnection>)> && cb1,
                        std::function<void(std::shared_ptr<TcpConnection>)> && cb2,
                        std::function<void(std::shared_ptr<TcpConnection>)> && cb3);
    void runInLoop(std::function<void()>&&);
    ~EventLoop();

    void setTimeCb(std::function<void()>&&);
private:
    int createEpollfd();
    void addEpollEvent(int);
    void delEpollEvent(int);
    void epollWait();
    void handleNewConnection();
    void handleMessage(int);

    int createEventfd();
    void handleRead(int);
    void wakeup();
    void doThings();

    int createtimerFd();
    void setTime(int init,int oot);
private:
    int _epfd;
    std::vector<struct epoll_event> _epList;
    //包装好的socket服务端
    Acceptor & _acceptor;
    std::map<int,std::shared_ptr<TcpConnection>> _listenMap;
    bool _inLoop;
    int _evtfd;
    std::vector<std::function<void()>> _todo;
    std::mutex _mtx;

    int _timerFd;
    std::function<void()> _timeCb;
private:
    std::function<void(std::shared_ptr<TcpConnection>)> _onNewConnection;
    std::function<void(std::shared_ptr<TcpConnection>)> _onMessage;
    std::function<void(std::shared_ptr<TcpConnection>)> _onClose;
};

#endif 

