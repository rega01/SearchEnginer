#ifndef __WebPageQuery_H__
#define __WebPageQuery_H__

#include "travial.h"
#include "SplitTool.h"
#include "WebPage.h"
#include "LRU.h"

#include <pthread.h>

#include <queue>

using std::priority_queue;

struct MyCompareWebpage{
    bool operator()(const pair<WebPage,double> &lhs,
                    const pair<WebPage,double> &rhs)const {
        return lhs.second < rhs.second;
    }
};

class WebPageQuery
{
public:
    static WebPageQuery * createInstance();
    priority_queue<pair<WebPage,double>,vector<pair<WebPage,double>>,MyCompareWebpage> doQuery(string queryWords);
    priority_queue<pair<WebPage,double>,vector<pair<WebPage,double>>,MyCompareWebpage> &doXapianQuery(string queryWords);
private:
    static void init_r();
    static void destroy();
    WebPageQuery();
    void loadLibary();
    WebPage getWebPage(string & docItem);
    //存储交集文章
    void storeIntersection();
    //计算单词在文章中出现的次数
    int getTF(const string & content,const string & word);
    //计算单词在所有文章出现的次数
    int getDF(const string & word);
    double getCosin(const vector<double>& candidate);
    //计算所有单词的权重
    vector<double> calcuteWeights(const string & content,const vector<string> words);
    ~WebPageQuery();

public:
    //线程共享LRU，放入该单例模式中，确保全局初始化一次
    LRU _cacheLRU;
    
private:
    //存放分词后的句子
    vector<string> _splitedWords;
    //把句子当做文章后得到的权重
    vector<double> _queryWordsWeight;

    map<int,vector<double>> _pagesWeight;

    priority_queue<pair<WebPage,double>,vector<pair<WebPage,double>>,MyCompareWebpage> _resultQue;

    //存放包含所有分词后关键字的文章id及其内容
    unordered_map<int,WebPage> _pages;
    //存放所有偏移信息
    unordered_map<int,pair<int,int>> _offsetlib;
    //存放所有倒排索引
    unordered_map<string,set<pair<int,double>>> _invertIndexLib;
    SplitTool * _wordCutTool;


    static WebPageQuery * _pInstance;
    static pthread_once_t _once;
    static int docCnt;

};

#endif 

