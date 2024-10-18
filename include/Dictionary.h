#ifndef __Dictionary_H__
#define __Dictionary_H__

#include "travial.h"
#include <pthread.h>

class Dictionary
{
public:
    static Dictionary* createInstance();
    //查询单字，返回索引中对应的词语和频率
    set<pair<string,int>> doQuery(string word);
    vector<pair<string,int>> & getDict();
    unordered_map<string,set<int>> & getIndexTable();

private:
    Dictionary();
    void init();
    static void init_r();
    static void destroy();
    ~Dictionary();

private:
    vector<pair<string,int>> _dict;
    vector<pair<string,int>> _dictzh;
    unordered_map<string,set<int>> _indexTable;

    static Dictionary* _pInstance;
    static pthread_once_t  _once;
};

#endif 

