#ifndef __PageLibPreprocessor_H__
#define __PageLibPreprocessor_H__

#include "travial.h"
#include "SplitTool.h"
#include "WebPage.h"

class PageLibPreprocessor
{
public:
    PageLibPreprocessor();
    void doProcess();
    void cutRedundantPages();
    void buildInvertIndexTable();
    void xapianBuildInvertIndexTable();
    void storePages();
    void storeOffset();
    ~PageLibPreprocessor();

private:
    vector<WebPage> _pageList;
    map<int,pair<int,int>> _offsetLib;
    vector<uint64_t> _fingerPrints;
    SplitTool * _wordCutter;
    
    //统计某个词在某个文档id中出现的数量
    map<string,map<int,int>> _wordSingleCnt;
    //统计单词在所有文章中出现的数量
    unordered_map<string,set<int>> _wordAllDocCnt;
    unordered_map<string,set<pair<int,double>>> _invertIndexLib;

    static int docId;
    static int _offSet;
};

#endif 

