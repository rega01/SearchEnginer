#include "Dictionary.h"
#include "Configuration.h"


#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;

Dictionary* Dictionary::_pInstance= nullptr;
pthread_once_t  Dictionary::_once = PTHREAD_ONCE_INIT;

Dictionary* Dictionary::createInstance(){
    pthread_once(&_once,init_r);
    return _pInstance;
}

void Dictionary::init_r(){
    _pInstance = new Dictionary();
    atexit(destroy);
}

set<pair<string,int>> Dictionary::doQuery(string word){
   set<int> indexes = _indexTable[word];
   set<pair<string,int>> ret;
   //判断是否中文
   if(word[0] & 0X80){
       for(int i : indexes){
            ret.insert(_dictzh[i]);
       }
   }else{
       for(int i : indexes){
            ret.insert(_dict[i]);
       }
   }
   return ret;
}

void Dictionary::init(){
    ifstream ifsDict(Configuration::getInstance()->getConfigMap()["DICT_EN"]);
    ifstream ifsIndex(Configuration::getInstance()->getConfigMap()["INDEX_EN"]);
    if(!ifsDict || !ifsIndex){
        perror("Dictionary(open file)");
        exit(1);
    }
    string word,line;
    int freq,index;
    //存储词典
    while(ifsDict >> word >> freq){
        _dict.emplace_back(word,freq);
    }
    ifsDict.close();
    ifsDict.open(Configuration::getInstance()->getConfigMap()["DICT_ZH"]);
    while(ifsDict >> word >> freq){
        _dictzh.emplace_back(word,freq);
    }

    //存储索引
    while(std::getline(ifsIndex,line)){
        istringstream iss(line);
        iss >> word;
        while(iss >> index){
            _indexTable[word].insert(index);
        }
    }
    ifsIndex.close();
    ifsIndex.open(Configuration::getInstance()->getConfigMap()["INDEX_CN"]);
    while(std::getline(ifsIndex,line)){
        istringstream iss(line);
        iss >> word;
        while(iss >> index){
            _indexTable[word].insert(index);
        }
    }
    ifsDict.close();
    ifsIndex.close();
}


vector<pair<string,int>> & Dictionary::getDict(){
    return _dict;
}

unordered_map<string,set<int>> & Dictionary::getIndexTable(){
    return _indexTable;
}

Dictionary::Dictionary(){
    init();
}
void Dictionary::destroy(){
    if(_pInstance != nullptr){
        delete _pInstance;
        _pInstance=nullptr;
    }
}
Dictionary::~Dictionary(){
}
