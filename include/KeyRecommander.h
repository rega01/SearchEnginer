#ifndef __KeyRecommander_H__
#define __KeyRecommander_H__

#include "travial.h"
#include <queue>

using std::priority_queue;
class TcpConnection;

struct CandidateResult{
    string _word;
    int _freq;
    int _distance;
    CandidateResult(string word ,int freq,int dis);
};

struct MyCompareCandidate{
    bool operator()(const CandidateResult &lhs,const CandidateResult &rhs)const {
        if(lhs._distance != rhs._distance){
           return  lhs._distance > rhs._distance;
        }
        else if(lhs._freq != rhs._freq){
           return  lhs._freq > rhs._freq;
        }
        else if(lhs._word != rhs._word){
            return lhs._word > rhs._word;
        }
        else{
            return false;
        }
    }
};

class KeyRecommander
{
public:
    KeyRecommander(string word,shared_ptr<TcpConnection> ptcp=nullptr);
    //执行查询
    void execute();
    ~KeyRecommander();

private:
    //拆分单词查询词典类
    void queryIndexTable();
    //存放进优先队列
    void statistic(set<pair<string,int>> & res);
    //计算与_sought的最小编辑距离
    int distance(const string &rhs);
    void response();

private:
    string _sought;
    priority_queue<CandidateResult,vector<CandidateResult>,MyCompareCandidate> _resultQue;
    shared_ptr<TcpConnection>  _conn;
};

#endif 

