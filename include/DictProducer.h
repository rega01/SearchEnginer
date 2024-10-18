#ifndef __DictProducer_H__
#define __DictProducer_H__

#include "travial.h"
#include "SplitTool.h"

class DictProducer
{
public:
    DictProducer(const string & dir);
    void buildEnDict();
    void buildCnDict();
    void createEnIndex();
    void createCnIndex();
    void store();

    void getStopWords();
    ~DictProducer();

private:
    size_t getPrefixOne(char ch);
    set<string> _stopWords;

    vector<string> _cnFilePaths;
    //英文词典
    vector<pair<string,int>> _dict;
    //中文词典
    vector<pair<string,int>> _cnDict;
    //英文索引
    map<string,set<int>> _index;
    //中文索引
    map<string,set<int>> _indexCn;
    SplitTool * _cutter;
};

#endif 

