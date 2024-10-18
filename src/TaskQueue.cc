//TaskQueue.cpp
#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t size)
    :_queSize(size)
     ,isExit(false)
{}

void TaskQueue::push(Task&& task){
    unique_lock<mutex> ulk(_mutex);
    while(!isExit && full()){
        /* _notFull.wait(ulk,[&](){return !full();}); */
        _notFull.wait(ulk);
    }

    if(ulk.owns_lock()){
        _que.push(task);
        _notEmpty.notify_one();
        ulk.unlock();
    }
}

TaskQueue::Task TaskQueue::pop(){
    unique_lock<mutex> ulk(_mutex);
    while(!isExit && empty()){
        /* _notEmpty.wait(ulk,[&](){return !empty();});//第二个参数会导致无法正确通知到 */
        _notEmpty.wait(ulk);
    }
    Task task;
    /* if(ulk.owns_lock()){//退出唤醒时会导致段错误 */
    if(!isExit && ulk.owns_lock()){
        task=_que.front();
        _que.pop();
        _notFull.notify_one();
    }
    return task;
}

bool TaskQueue::full(){
    return _que.size() == _queSize;
}

bool TaskQueue::empty(){
    return _que.size() == 0;
}

void TaskQueue::stop(){
    isExit = true;
    _notFull.notify_all();
    _notEmpty.notify_all();
}

TaskQueue::~TaskQueue(){
    cout << "~TaskQueue" << endl;
}
