#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>

using std::cout;
using std::endl;
using std::list;
using std::unordered_map;
using std::pair;

class LRU
{
public:
    LRU(int capacity)
    : _capacity(capacity)
    {

    }

    void put(int key, int value)
    {
        auto it = _cache.find(key);
        //key值存在list中
        if(it != _cache.end())
        {
            it->second->second  = value;//更新key对应的value值
            //将key与value移动到最前面
            _nodes.splice(_nodes.begin(), _nodes, it->second);
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

        }
    }

    int get(int key)
    {
        auto it = _cache.find(key);
        if(it == _cache.end())
        {
            return -1;
        }
        else
        {
            //将key与value移动到最前面
            _nodes.splice(_nodes.begin(), _nodes, it->second);
            return it->second->second;
        }
    }

    void print() const
    {
        for(auto &elem : _nodes)
        {
            cout << "(" << elem.first
                 << ", " << elem.second
                 << ")  ";
        }
        cout <<endl;
    }
private:
    int _capacity;//元素的个数
    list<pair<int, int>> _nodes;//存放key-value的容器
    unordered_map<int, list<pair<int, int>>::iterator> _cache;
};

void test()
{
    LRU lru(3);
    lru.put(1, 10);
    lru.print();

    cout << endl;
    lru.put(2, 20);
    lru.print();

    cout << endl;
    cout << "lru.get(1) = " << lru.get(1) << endl;
    lru.print();

    cout << endl;
    lru.put(3, 30);
    lru.print();

    cout << endl;
    lru.put(4, 40);
    lru.print();

    cout << endl;
    lru.put(1, 11);
    lru.print();

    cout << endl;
    cout << "lru.get(2) = " << lru.get(2) << endl;
    lru.print();

    cout << endl;
    lru.put(4, 44);
    lru.print();
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}

