//TaskQueue.hpp
#ifndef __TaskQueue_H__
#define __TaskQueue_H__
#include<iostream>
#include<functional>
#include<mutex>
#include<condition_variable>
#include<queue>

using namespace std;
/* class Task; */
class LRU;

class TaskQueue
{
    using Task = function<void(LRU*)> ;
public:
    TaskQueue(size_t);
    TaskQueue(const TaskQueue&)=delete;
    TaskQueue(TaskQueue&&)=delete;
    TaskQueue& operator=(const TaskQueue&)=delete;
    ~TaskQueue();
    void push(Task&&);
    Task pop();
    bool full();
    bool empty();
    void stop();
private:
    size_t _queSize;
    queue<Task>_que;
    mutex _mutex;
    condition_variable _notFull;
    condition_variable _notEmpty;
    bool isExit;
};

#endif 

