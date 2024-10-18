//SockIo.h
#ifndef __SockIo_H__
#define __SockIo_H__

#define BUFFSIZE 2048

#include "travial.h"

class SockIo
{
public:
    explicit SockIo(int);
    int readn(char *,int);
    int readJsonStr(char *,int);
    int sendJsonStr(const char *,int);
    int sendn(const char *buff,int len);
    ~SockIo(){}

private:
    int _sockio;
};

#endif 
