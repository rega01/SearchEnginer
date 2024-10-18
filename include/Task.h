#ifndef __Task_H__
#define __Task_H__

#include "TcpConnection.h"

#include <string>

class LRU;
class Task
{
public:
    Task(const std::string &,std::shared_ptr<TcpConnection>);
    void process(LRU*);
    ~Task();
private:
    std::string _msg;
    //用于处理完数据后调用sendToLoop
    std::shared_ptr<TcpConnection> _con;
};

#endif 

