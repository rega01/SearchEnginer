#ifndef __LRU_H__
#define __LRU_H__

#include "travial.h"

#include <pthread.h>
#include <boost/noncopyable.hpp>

class LRU
:public boost::noncopyable
{
public:
    LRU(int capacity);
    string get(const string & key);
    void put(string key,string value);
    void print()const;
    void synIn(LRU *plru);
    void synOut(LRU *plru);
    ~LRU();

    list<pair<string,string>> _pendings;
private:
    int _capacity;
    list<pair<string,string>> _nodes;
    unordered_map<string,list<pair<string,string>>::iterator> _cache;
    mutex _mtxLru;
};

#endif 

