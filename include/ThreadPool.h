//ThreadPool.hpp
#ifndef __threadPool_H__
#define __threadPool_H__

#include "travial.h"
#include "TaskQueue.h"
#include "LRU.h"

#include <boost/noncopyable.hpp>
#include <iostream>
#include <thread>

/* class Task; */

class ThreadPool
:public boost::noncopyable
{
    using Task = function<void(LRU *)> ;
public:
    ThreadPool(size_t threadNum,size_t queSize);
    ~ThreadPool(){}
    void start();
    void stop();
    void doTask();
    void doSyn();
    void synAllLRU();
    void exchangeLRU();

    void addTask(Task&&);

private:
    size_t _threadNum;
    size_t _queSize;
    TaskQueue _taskQue;
    vector<thread> _threads;
    bool _isExit;

    map<std::thread::id,pair<LRU*,mutex*>> _clientLRU;

    thread synThread;
    map<std::thread::id,pair<LRU*,mutex*>> _synLRU;
    bool exchanged;
    condition_variable _cond;
    mutex _condMtx;
};

#endif 
