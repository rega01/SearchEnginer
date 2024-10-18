//SockIo.cc
#include "SockIo.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <iostream>

using namespace std;

SockIo::SockIo(int connFd)
:_sockio(connFd)
{}

int SockIo::readn(char *buff,int len){
    int left = len;
    char * pCur = buff;
    while(left > 0){
        int ret = read(_sockio,pCur,BUFFSIZE);
        if(ret ==-1 && errno == EINTR){
            continue;
        }else if(ret == -1){
            perror("read(-1)");
            exit(1);
        }else if(ret == 0){
            break;
        }else {
            pCur += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SockIo::readJsonStr(char *buff,int len){
    int msgLen = 0;
    read(_sockio,reinterpret_cast<char*>(&msgLen),sizeof(int));
    /* cout << "recvLen" <<msgLen << "\n"; */
    char * pCur = buff;
    readn(pCur,msgLen);
    
    return msgLen;
}

int SockIo::sendJsonStr(const char *buff,int len){
    int msgLen = len;
    write(_sockio,reinterpret_cast<char*>(&msgLen),sizeof(int));
    /* write(_sockio,&msgLen,sizeof(size_t)); */
    /* cout << "sendLen " << msgLen <<"\n"; */
    const char * pCur = buff;
    sendn(pCur,msgLen);
    
    /* cout << "send " << buff <<"\n"; */
    return msgLen;
}

int SockIo::sendn(const char *buff,int len){
    int left = len;
    const char * pCur = buff;
    while(left > 0){
        int ret = write(_sockio,pCur,left);
        if(ret == -1 && errno == EINTR){
            continue;
        }else if(ret == -1){
            perror("read(-1)");
            exit(1);
        }else if(ret == 0){
            break;
        }else {
            pCur += ret;
            left -= ret;
        }
    }
    return len - left;
}
