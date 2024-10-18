#include <cctype>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "DictProducer.h"
#include "Configuration.h"
#include "DirScanner.h"
#include "SplitToolCppJieba.h"

using std::ifstream;
using std::ofstream;
using std::cout;

//传入中文语料目录即可，英文语料只有两个文件
DictProducer::DictProducer(const string & cnDir)
    :_cutter(new SplitToolCppJieba)
{
    getStopWords();
    DirScanner dirScanner;
    dirScanner.traverse(cnDir);
    vector<string> chFiles = dirScanner.getFiles();
    _cnFilePaths.insert(_cnFilePaths.end(),chFiles.begin(),chFiles.end());
}

void DictProducer::getStopWords(){
    //英文停用词
    ifstream ifs(Configuration::getInstance()->getConfigMap()["STOP_EN"]);
    
    string stopword;
    while(ifs >> stopword){
        _stopWords.insert(stopword);
    }
    ifs.close();

    //中文停用词
    ifstream ifszh(Configuration::getInstance()->getConfigMap()["STOP_ZH"]);
    while(ifszh >> stopword){
        _stopWords.insert(stopword);
    }
    ifszh.close();
}

void DictProducer::buildEnDict(){
    //存储内容，由于需要清洗，因此使用vector
    vector<string> enFiles = {Configuration::getInstance()->getConfigMap()["SOURCE_EN1"],
        Configuration::getInstance()->getConfigMap()["SOURCE_EN2"]};
    for(int i =0;i<2;++i){
        vector<char> file;
        ifstream ifs(enFiles[i]);
        ifs.seekg(0,std::ios::end);
        size_t totalSize = ifs.tellg();
        ifs.seekg(0,std::ios::beg);
        file.resize(totalSize);
        ifs.read(&*file.begin(),totalSize);

        //清洗内容 大写变小写，非字母数字换空格
        std::for_each(file.begin(),file.end(),[](char &ch){
                      if(!isalpha(ch)){
                            ch = ' ';
                      }else if(isupper(ch)){
                            ch = tolower(ch);
                      }
                      });

        //分隔、统计
        //临时map统计词频
        map<string,int> freq;
        std::istringstream iss(file.data());
        string word;
        while(iss >> word){
            if(_stopWords.find(word) == _stopWords.end() && getPrefixOne(word[0]) ==0){
                ++freq[word];
            }
        }
        for(auto pair : freq){
            _dict.push_back(make_pair(pair.first,pair.second));
        }
        ifs.close();
    }
    createEnIndex();
}

void DictProducer::buildCnDict(){
    for(size_t i =0;i<_cnFilePaths.size();++i){

        ifstream ifs(_cnFilePaths[i]);
        ifs.seekg(0,std::ios::end);
        size_t totalSize = ifs.tellg();
        ifs.seekg(0,std::ios::beg);
        vector<char> buff;
        buff.resize(totalSize);
        ifs.read(&*buff.begin(),totalSize);

        buff.erase(remove_if(buff.begin(),buff.end(),[](char & ch){
                             /* return (ch=='\r') || (ch =='\n') || (ch ==' '); */
                             return isspace(static_cast<unsigned char>(ch)) || isdigit(ch);
                             }
                            ),buff.end());
        buff.shrink_to_fit();
        vector<string> result = _cutter->cut(&*buff.begin());
        map<string,int> tempDict ;
        for(string word : result){
            if(_stopWords.find(word) == _stopWords.end() && getPrefixOne(word[0]) == 3 && word.size() >3){
                ++tempDict[word];
            }
        }
        for(auto pai : tempDict){
            _cnDict.emplace_back(pai.first,pai.second);
        }

        ifs.close();
    }
    createCnIndex();
}

void DictProducer::createEnIndex(){
    for(size_t i=0;i<_dict.size();++i){
        string word = _dict[i].first;
        for(char ch : word){
            if(isalpha(ch) && getPrefixOne(ch)==0){
                string key(1,ch);
                _index[key].insert(i);
            }
        }
    }
}

void DictProducer::createCnIndex(){
    for(size_t i =0;i<_cnDict.size();++i){
        string words = _cnDict[i].first;       
        size_t len_words = words.size();
        for(size_t j = 0;j<len_words;j+=3){
            _indexCn[string(words.begin()+j,words.begin()+j+3)].insert(i);
        }
    }
}

size_t DictProducer::getPrefixOne(char ch){
    if(ch & 0X80){
        size_t nBytes = 1;
        size_t leftMove = 6;
        while(ch & (1 << leftMove)){
            --leftMove;
            nBytes++;
        }
        return nBytes;
    }
    return 0;
}
void DictProducer::store(){
    vector<string> paths={Configuration::getInstance()->getConfigMap()["DICT_EN"],
        Configuration::getInstance()->getConfigMap()["INDEX_EN"],
        Configuration::getInstance()->getConfigMap()["DICT_ZH"],
        Configuration::getInstance()->getConfigMap()["INDEX_CN"]};
    //1.1、存储英文词典
    ofstream ofs(paths[0],std::ios_base::out|std::ios_base::trunc);
    for(pair<string,int> p:_dict){
        ofs << p.first << " " << p.second << "\n";
    }
    ofs.close();
    //1.2、存储英文索引
    ofstream indexOfs(paths[1],std::ios_base::out | std::ios_base::trunc);
    for(pair<string,set<int>> val : _index){
        indexOfs << val.first << "   " ;
        for(int i :val.second){
            indexOfs << i << " ";
        }
        indexOfs << "\n";
    }
    indexOfs.close();

    //2.1、存储中文词典
    ofs.open(paths[2],std::ios_base::out|std::ios_base::trunc);
    for(pair<string,int> p:_cnDict){
        ofs << p.first << " " << p.second << "\n";
    }
    ofs.close();
    //2.2、存储中文索引
    indexOfs.open(paths[3],std::ios_base::out | std::ios_base::trunc);
    for(pair<string,set<int>> val : _indexCn){
        indexOfs << val.first << "   " ;
        for(int i :val.second){
            indexOfs << i << " ";
        }
        indexOfs << "\n";
    }
    indexOfs.close();
}

DictProducer::~DictProducer(){
    delete _cutter;
}

