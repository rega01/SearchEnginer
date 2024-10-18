//Socket.cc
#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
using namespace std;

Socket::Socket(){
    _fd = socket(AF_INET,SOCK_STREAM,0);
    if(_fd == -1){
        perror("socket");
    }
}

Socket::Socket(int fd)
:_fd(fd)
{}

int Socket::fd(){
    return _fd;
}
Socket::~Socket(){
    close(_fd);
}

