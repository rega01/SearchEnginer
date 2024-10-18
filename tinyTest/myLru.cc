#include <map>
#include <list>
#include <unordered_map>
#include <iostream>

using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) {
        _capacity = capacity;
    }

    int get(int key) {
        auto it = lru.find(key);
        if(it == lru.end()) {return -1;}
        int ret = it->second.first;
        numfreq.splice(numfreq.begin(),numfreq,it->second.second);
        return ret;
    }

    void put(int key, int value) {
        auto it = lru.find(key);
        if(it != lru.end()){
            lru[key].first = value;
            numfreq.splice(numfreq.begin(),numfreq,it->second.second);
        }else{
            if(static_cast<int>(lru.size()) == _capacity){
                int keyToPop = numfreq.back();
                numfreq.pop_back();
                lru.erase(keyToPop);
            }
            numfreq.push_front(key);
            lru[key] = pair<int,list<int>::iterator>(value,numfreq.begin());
        }
    }
private:
    map<int,pair<int,list<int>::iterator>> lru;
    list<int> numfreq;
    int _capacity; //2
};


int main(int argc,char * argv[])
{
    string strArry[] ={"LRUCache","get","put","get","put","put","get","get"};
    int  num[] ={2,2,2,6,1,1,5,1,2,1,2};
    LRUCache * plru =nullptr;
    int pos =0;
    for(int i =0 ; i < 10;++i){
        if(strArry[i] == "LRUCache"){
            plru = new LRUCache(num[pos++]);
        }
        if(strArry[i] == "put"){
            int key = num[pos++],val = num[pos++];
            plru->put(key,val);
        }
        else if(strArry[i] == "get"){
            int ret = plru->get(num[pos++]);
            if(ret==-1){
                cout << "no such element" << endl;
            }else{
                cout << ret << endl;
            }
        }
    }
    return 0;
}

