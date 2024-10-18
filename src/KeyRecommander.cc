#include "KeyRecommander.h"
#include "Dictionary.h"
#include "TcpConnection.h"

#include <nlohmann/json.hpp>

#include <codecvt>
#include <locale>

CandidateResult::CandidateResult(string word,int freq,int dis)
:_word(word)
,_freq(freq)
,_distance(dis)
{}

int minDistance(const std::string& word1, const std::string& word2) {
    // 将 UTF-8 格式字符串转换为 UTF-32，能够正确处理英文字母和汉字混合的情况
    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> converter;
    std::u32string u32Word1 = converter.from_bytes(word1);
    std::u32string u32Word2 = converter.from_bytes(word2);

    int n = u32Word1.size(), m = u32Word2.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));

    // 初始化边界条件
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;

    // 动态规划计算编辑距离
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (u32Word1[i - 1] == u32Word2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + std::min(dp[i - 1][j - 1], std::min(dp[i][j - 1], dp[i - 1][j]));
        }
    }
    return dp[n][m];
}

KeyRecommander::KeyRecommander(string word,shared_ptr<TcpConnection> ptcp)
    :_sought(word)
     ,_conn(ptcp)
{}

void KeyRecommander::execute(){
    queryIndexTable();
}

void KeyRecommander::queryIndexTable(){
    size_t current = 0;
    set<pair<string,int>> res;
    string queryWord;
    while(current < _sought.size()){
        if(_sought[current] & 0X80){
            queryWord= _sought.substr(current,3);
            set<pair<string,int>> tempS = Dictionary::createInstance()->doQuery(queryWord);
            res.insert(tempS.begin(),tempS.end());
            current += 3;
        }else{
            queryWord = _sought.substr(current,1);
            set<pair<string,int>> tempS = Dictionary::createInstance()->doQuery(queryWord);
            res.insert(tempS.begin(),tempS.end());
            ++current;
        }
    }
    statistic(res);
}

void KeyRecommander::statistic(set<pair<string,int>>&iset){
    unordered_set<string> uniqueRes;
    for(auto couple : iset){
        if(uniqueRes.count(couple.first)==0){
            _resultQue.emplace(couple.first,couple.second,distance(couple.first));
            uniqueRes.insert(couple.first);
        }
    }
}

int KeyRecommander::distance(const string &rhs){
    return minDistance(_sought,rhs);
}

void KeyRecommander::response(){
    size_t topK = 10 ,pos=0;
    nlohmann::json json;
    json["type"] = "KeyRecommander";
    json["length"] = 0;
    
    while(!_resultQue.empty() && pos < topK){
        CandidateResult candi= _resultQue.top();
        json["value"][pos++]["word"] = candi._word;
        json["length"] = pos;
        _resultQue.pop();
    }
    string backMessage = json.dump();
    _conn->sendToLoop(backMessage);
}

KeyRecommander::~KeyRecommander(){}

