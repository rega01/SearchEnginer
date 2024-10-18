//InetAddr.cc
#include "InetAddr.h"

#include <arpa/inet.h>

#include <iostream>

using namespace std;

InetAddr::InetAddr(const string & ip,short port){
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddr::InetAddr(const struct sockaddr_in & addr){
    _addr = addr;
}

std::string InetAddr::getIP() const{
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddr::getPort() const{
    return ntohs(_addr.sin_port);
}

struct sockaddr_in* InetAddr::getAddr(){
    return &_addr;
}

InetAddr::~InetAddr(){

}
