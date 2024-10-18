#ifndef __Configuration_H__
#define __Configuration_H__

#include "travial.h"
#include <pthread.h>

class Configuration
{
public:
    static Configuration * getInstance();
    map<string,string> & getConfigMap();
    set<string> & getStopWords();
private:
    Configuration();
    static void init_r();
    static void destroy();
    void initStopWords();
    ~Configuration();

private:
    string _filepath;
    map<string,string> _configMap;
    set<string> _stopWords;
    static Configuration * _instance;
    static pthread_once_t _once;
};

#endif 

