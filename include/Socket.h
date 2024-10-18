//Socket.h
#ifndef __Socket_H__
#define __Socket_H__

class Socket
{
public:
    Socket();
    Socket(int);
    int fd();
    ~Socket();

private:
    int _fd;
};

#endif 

