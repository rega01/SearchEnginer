//ThreadPool.cc
#include "ThreadPool.h"
#include "WebPageQuery.h"

#include <stdlib.h>
#include <time.h>

#include <functional>

using Task = function<void()>;
using namespace std;

ThreadPool::ThreadPool(size_t threadNum,size_t queSize)
    :_threadNum(threadNum)
    ,_queSize(queSize)
    ,_taskQue(_queSize)
    ,_isExit(false)
     ,exchanged(false)
{
    _threads.reserve(_threadNum);
}

void ThreadPool::start(){
    ::srand(::clock());
    for(size_t i=0;i <_threadNum;++i){
        _threads.emplace_back(bind(&ThreadPool::doTask,this));
    }
    synThread = thread(bind(&ThreadPool::doSyn,this));
}

void ThreadPool::stop(){
    while(!_taskQue.empty()){
        std::this_thread::sleep_for(200ms);
    }
    _taskQue.stop();
    _isExit = true;
    for(auto & thrd : _threads){
        thrd.join();
    }
    synThread.join();
}

void ThreadPool::doSyn(){
    while(!_isExit){
        unique_lock<mutex> lock(_condMtx);
        _cond.wait(lock,[this]{return exchanged;});
        if(exchanged){
            synAllLRU();
            exchanged =false;
        }
    }
}

//用于线程内部LRU与LRU’进行交换，供timerfd调用
void ThreadPool::exchangeLRU(){
    for(auto couple:_clientLRU){
        {
            lock_guard<mutex> lock1(*(couple.second.second));
            lock_guard<mutex> lock2(*(_synLRU[couple.first].second));
            swap(couple.second.first,_synLRU[couple.first].first);
        }
    }
    exchanged = true;
    _cond.notify_one();
}

//该函数用于公共LRU与LRU‘进行交换,缓存管理线程使用
void ThreadPool::synAllLRU(){
    /* cout << "LRU syning\n"; */
    //1、将所有synLRU放入公共LRU
    for(auto couple:_synLRU){
        /* cout << "---------------------\n"; */
        /* cout << "Single SYNLRU' " << couple.first << "\n"; */
        /* couple.second.first->print(); */
        /* cout << "---------------------\n"; */
        WebPageQuery::createInstance()->_cacheLRU.synIn(couple.second.first);
    }
    /* cout << "\n"; */
    /* cout << "----- ---------------------------------\n"; */
    /* cout << "Current shared LRU:\n"; */
    /* WebPageQuery::createInstance()->_cacheLRU.print(); */
    /* cout << "---------------------\n"; */
    //2、将公共LRU同步回线程synLRU
    for(auto couple :_synLRU){
        WebPageQuery::createInstance()->_cacheLRU.synOut(couple.second.first);

        //同步到clientLRU
        lock_guard<mutex> lock1(*(_clientLRU[couple.first].second));
        lock_guard<mutex> lock2(*(couple.second.second));
        swap(_clientLRU[couple.first].first,couple.second.first);
    }
}

void ThreadPool::doTask(){
    thread_local LRU cacheLruClient(10);
    thread_local LRU cacheLruSyn(10);
    mutex mtx1;
    mutex mtx2;
    _clientLRU[std::this_thread::get_id()] = std::make_pair(&cacheLruClient,&mtx1);
    _synLRU[std::this_thread::get_id()] = std::make_pair(&cacheLruSyn,&mtx2);

    while(!_isExit){
        Task task =_taskQue.pop();
        if(task!= nullptr){
            /* std::this_thread::sleep_for(3000ms); */
            task(&cacheLruClient);
        }
    }
}

void ThreadPool::addTask(Task&& task){
    _taskQue.push(std::move(task));
}

