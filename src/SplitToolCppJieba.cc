#include "SplitToolCppJieba.h"
#include "Configuration.h"

const char* const DICT_PATH = "/home/cmj/daily_use/searchProjectV1/dict/jieba.dict.utf8";
const char* const HMM_PATH = "/home/cmj/daily_use/searchProjectV1/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "/home/cmj/daily_use/searchProjectV1/dict/user.dict.utf8";
const char* const IDF_PATH = "/home/cmj/daily_use/searchProjectV1/dict/idf.utf8";
const char* const STOP_WORD_PATH = "/home/cmj/daily_use/searchProjectV1/dict/stop_words.utf8";

SplitToolCppJieba::SplitToolCppJieba()
    :_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
{}

size_t getPrefixOne(char ch){
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
vector<string> SplitToolCppJieba::cut(const string & strToCut) {
    vector<string> words;
    set<string> stop = Configuration::getInstance()->getStopWords();
    _jieba.CutAll(strToCut,words);
    
    words.erase(remove_if(words.begin(),words.end(),[&](string& word){
                          return stop.find(word) != stop.end() || getPrefixOne(word[0]) == 0 || word.size() <3 ||isspace(word[0]);}),words.end());
    return words;
}

SplitToolCppJieba::~SplitToolCppJieba(){

}

