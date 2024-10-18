#ifndef __Acceptor_H__
#define __Acceptor_H__
#include "Socket.h"
#include "InetAddr.h"

class Acceptor
{
public:
    Acceptor(const std::string &,short);
    void setReuseIP();
    void setReusePort();
    void bind();
    void listen();
    void startListen();
    int accept();
    int fd();
    ~Acceptor();

private:
    Socket _sok;
    InetAddr _addr;
};

#endif 

