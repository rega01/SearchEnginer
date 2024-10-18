#include "Configuration.h"

#include <fstream>

using std::ifstream;

Configuration * Configuration::_instance = nullptr;
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;

Configuration * Configuration::getInstance(){
    pthread_once(&_once,init_r);
    return _instance;
}

void Configuration::init_r(){
    if(_instance == nullptr){
        _instance = new Configuration();
        atexit(destroy);
    }
}

Configuration::Configuration()
{
    _filepath = "/home/cmj/daily_use/searchProjectV3.0/conf/search_engine.conf";

    std::ifstream ifs(_filepath);
    if(!ifs){
        perror("conf file");
        exit(1);
    }
    string key ,val;
    while(ifs >> key,ifs >> val){
        _configMap[key] =val;
    }
    initStopWords();
}

void Configuration::initStopWords(){
    ifstream ifs(_configMap["STOP_EN"]);
    string word;
    while(ifs >> word){
        _stopWords.insert(word);
    }
    ifs.close();
    ifs.open(_configMap["STOP_ZH"]);
    while(ifs >> word){
        _stopWords.insert(word);
    }
    ifs.close();
}

map<string,string> & Configuration::getConfigMap(){
    return _configMap;
}

set<string> & Configuration::getStopWords(){
    return _stopWords;
}

void Configuration::destroy(){
    if(_instance != nullptr){
        delete _instance;
        _instance = nullptr;
    }
}

Configuration::~Configuration(){
}
