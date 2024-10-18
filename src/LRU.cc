#include "LRU.h"

LRU::LRU(int capacity)
    : _capacity(capacity)
{}

void LRU::synIn(LRU *plru){
    lock_guard<mutex> lock(plru->_mtxLru);
    for(auto couple:plru->_pendings){
        this->put(couple.first,couple.second);
    }
    plru->_pendings.clear();
}

void LRU::synOut(LRU *plru){
    lock_guard<mutex> lock1(_mtxLru);
    lock_guard<mutex> lock(plru->_mtxLru);
    plru->_cache = _cache;
    plru->_nodes = _nodes;
}

void LRU::put(string key,string value)
{
    lock_guard<mutex> lock(_mtxLru);
    auto it = _cache.find(key);
    //key值存在list中
    if(it != _cache.end())
    {
        it->second->second  = value;//更新key对应的value值
                                    //将key与value移动到最前面
        _nodes.splice(_nodes.begin(), _nodes, it->second);
        _pendings.push_front(*(it->second));
        _pendings.sort();
        _pendings.unique();
    }
    //key值不存在list中
    else
    {
        //容量是满的（元素的个数在list达到上限）
        if(_capacity == (int)_nodes.size())
        {
            //获取list中要删除的最后一个元素的值
            auto deleteNode = _nodes.back();
            //删除最后一个元素
            _nodes.pop_back();
            //删除在list中的最后一个元素在um中的信息
            _cache.erase(_cache.find(deleteNode.first));
            /* _cache.erase(deleteNode.first); */
        }
        //key与value放在list的最前面
        _nodes.push_front({key, value});
        //将key与key在list中的位置放在um中
        _cache[key] = _nodes.begin();
        _pendings.push_front(_nodes.front());
    }
}

string LRU::get(const string & key)
{
    lock_guard<mutex> lock(_mtxLru);
    auto it = _cache.find(key);
    if(it == _cache.end())
    {
        return string("");
    }
    else
    {
        //将key与value移动到最前面
        _nodes.splice(_nodes.begin(), _nodes, it->second);
        return it->second->second;
    }
}
void LRU::print() const{
    for(auto &elem : _nodes)
    {
        cout << "(" << elem.first
            /* << ", " << elem.second */
            << ")  \n" ;
    }
    /* for(auto couple : _cache){ */
    /*     cout <<"cache key: " <<  couple.first */
    /*         << " node key : " << (couple.second)->first <<"\n"; */
    /*         /1* << " val : " << (couple.second)->second<<"\n"; *1/ */
    /* } */
}

LRU::~LRU(){}
