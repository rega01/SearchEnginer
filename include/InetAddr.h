//InetAddr.h
#ifndef __InetAddr_H__
#define __InetAddr_H__
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <string>

class InetAddr
{
public:
    InetAddr(const std::string &,short);
    InetAddr(const struct sockaddr_in &);
    struct sockaddr_in* getAddr();
    std::string getIP() const;
    unsigned short getPort() const;
    ~InetAddr();
private:
    struct sockaddr_in _addr;
};

#endif 

