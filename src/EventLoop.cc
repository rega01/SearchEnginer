#include "EventLoop.h"

#include <muduo/base/LogStream.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>

using namespace std;

EventLoop::EventLoop(Acceptor &acceptor)
:_epfd(createEpollfd())
,_epList(1024)
,_acceptor(acceptor)
,_listenMap()
,_inLoop(true)
,_evtfd(createEventfd())
,_todo(0)
,_mtx()
,_timerFd(createtimerFd())
{
    int listenFd = acceptor.fd();
    addEpollEvent(listenFd);
    addEpollEvent(_evtfd);

    addEpollEvent(_timerFd);
    /* setTimeCb(timeCallback); */
    setTime(5,5);
}

void EventLoop::loop(){
    cout << "Server start success\n";
    while(_inLoop){
        epollWait();
    }
}

void EventLoop::unloop(){
    _inLoop = false;
}

int EventLoop::createEpollfd(){
    int epfd = epoll_create1(0);
    if(epfd == -1){
        perror("epoll create");
    }
    return epfd;
}

void EventLoop::addEpollEvent(int fd){
    struct epoll_event ev;
    ::bzero(&ev,sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
    if(ret ==-1){
        perror("addEpoll");
    }
}

void EventLoop::delEpollEvent(int fd){
    struct epoll_event ev;
    ::bzero(&ev,sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    if(ret ==-1){
        perror("addEpoll");
    }
}

void EventLoop::epollWait(){
    int nready = epoll_wait(_epfd,&*_epList.begin(),_epList.size(),-1);
    if(nready == -1){
        perror("epoll_wait");
        exit(1);
    }
    else{
        if(nready == (int)_epList.capacity()){
            _epList.resize(2 * nready);
        }
        for(int i=0;i<nready ;++i){
            int curFd = _epList[i].data.fd;
            if(curFd == _acceptor.fd()){
                handleNewConnection();
            }
            else if(curFd == _evtfd){
                handleRead(_evtfd);
                doThings();
            }
            else if(curFd == _timerFd){
                if(_timeCb){
                    handleRead(_timerFd);
                    _timeCb();
                }
            }
            else{
                map<int,shared_ptr<TcpConnection>>::iterator it = _listenMap.find(curFd);
                if(it != _listenMap.end()){
                    if(it->second->isClosed()){
                        delEpollEvent(curFd);
                        it->second->handleClose();
                        _listenMap.erase(curFd);
                        /* _listenMap.erase(it); */
                    }else{
                        it->second->handleMessage();
                        /* handleMessage(it->first); */
                    }
                }
            }
        }
    }
}

void EventLoop::handleNewConnection(){
    int newFd = _acceptor.accept();
    addEpollEvent(newFd);
    //使用make_shared可以更好的控制内存
    _listenMap[newFd]=make_shared<TcpConnection>(newFd,this);
    //此时不传入右值，因为三个回调函数要多次使用给新连接
    _listenMap[newFd]->setAllCallback(_onNewConnection,_onMessage,_onClose);
    _listenMap[newFd]->handleNewConnection();
}


void EventLoop::setAllCallback(std::function<void(std::shared_ptr<TcpConnection>)> && cb1,
                               std::function<void(std::shared_ptr<TcpConnection>)> && cb2,
                               std::function<void(std::shared_ptr<TcpConnection>)> && cb3){
    _onNewConnection = std::move(cb1);
    _onMessage = std::move(cb2);
    _onClose = std::move(cb3);
}

int EventLoop::createEventfd(){
    _evtfd = eventfd(10,0);
    if(_evtfd == -1){
        perror("eventfd");
    }
    return _evtfd;
}

//用于在eventfd和timerfd响应时读出数据，防止一直调用
void EventLoop::handleRead(int fd){
    uint64_t feedback = 0;
    int ret = read(fd,&feedback,sizeof(uint64_t));
    if(ret != sizeof(uint64_t)){
        perror("read feedback");
    }
}

//当放入新的需要发送给客户端的数据时，使用该函数唤醒eventfd（即服务端）
void EventLoop::wakeup(){
    uint64_t feedback = 1;
    int ret = write(_evtfd,&feedback,sizeof(uint64_t));
    if(ret != sizeof(uint64_t)){
        perror("write feedback");
    }
}

//唤醒后执行todo里的函数
void EventLoop::doThings(){
    //防止调用时又有新任务，直接交换出todo里所有任务
    vector<function<void()>> temp;
    {
        lock_guard<mutex> lgx(_mtx);
        temp.swap(_todo);
    }

    for(auto & cb : temp){
        cb();
    }
}

//该函数由TCPConnection调用
void EventLoop::runInLoop(std::function<void()>&& func){
    {
        lock_guard<mutex> lgx(_mtx);
        _todo.push_back(std::move(func));
    }
    wakeup();
}

int EventLoop::createtimerFd(){
    int fd = timerfd_create(CLOCK_REALTIME,0);
    if(fd == -1){
        perror("timerfd_crete");
    }
    return fd;
}

void EventLoop::setTimeCb(std::function<void()>&& cb){
    _timeCb = std::move(cb);
}

void EventLoop::setTime(int init,int oot){
    struct itimerspec tim;
    //周期时间
    tim.it_interval.tv_sec = oot;
    tim.it_interval.tv_nsec =0;

    //起始时间
    tim.it_value.tv_sec = init;
    tim.it_value.tv_nsec = 0;
    timerfd_settime(_timerFd,0,&tim,NULL);
}

EventLoop::~EventLoop(){
    close(_epfd);
    close(_evtfd);
    setTime(0,0);
    close(_timerFd);
}
