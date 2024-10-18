#include "WebPageQuery.h"
#include "Configuration.h"
#include "SplitToolCppJieba.h"

#include<iterator>

#include<xapian.h>

WebPageQuery* WebPageQuery::_pInstance = nullptr;
pthread_once_t WebPageQuery::_once = PTHREAD_ONCE_INIT;
int WebPageQuery::docCnt = 0;

WebPageQuery * WebPageQuery::createInstance(){
    pthread_once(&_once,init_r);
    return _pInstance;
}

void WebPageQuery::init_r(){
    if(_pInstance == nullptr){
        _pInstance = new WebPageQuery();   
        atexit(destroy);
    }
}

priority_queue<pair<shared_ptr<WebPage>,double>,vector<pair<shared_ptr<WebPage>,double>>,MyCompareWebpage> 
WebPageQuery::doXapianQuery(string queryWords){
    lock_guard<mutex> lock(_mtxForXapian);
    _pages.clear();
    while(!_resultQue.empty()){
        _resultQue.pop();
    }
    const char * const K_DB_PATH = "index_data";
    vector<string> words = _wordCutTool->cut(queryWords);
    Xapian::Query query = Xapian::Query("");
    for(auto word:words){
        query = Xapian::Query(Xapian::Query::OP_AND,query,word);
    }

    Xapian::Database db(K_DB_PATH);
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    Xapian::MSet result = enquire.get_mset(0,10);
    for (auto it = result.begin(); it != result.end(); ++it) {
        Xapian::Document doc = it.get_document();
        string docstr = doc.get_data();
        size_t start,end;
        start = docstr.find("<docid>")+7;
        end = docstr.find("</docid>");
        string docId =string(docstr.begin()+start,docstr.begin()+end);
        start = docstr.find("<title>")+7;
        end = docstr.find("</title>");
        string title = string(docstr.begin()+start,docstr.begin()+end);
        start = docstr.find("<link>")+6;
        end = docstr.find("</link>");
        string link = string(docstr.begin()+start,docstr.begin()+end);
        start = docstr.find("<content>")+9;
        end = docstr.find("</content>");
        string content = string(docstr.begin()+start,docstr.begin()+end);
        double docScoreWeight = it.get_weight();
        shared_ptr<WebPage> web = std::make_shared<WebPage>(docstr,stoi(docId),title,link,content);
        /* web._doc = docstr; */
        /* web._docId = stoi(docId); */
        /* web._docTitle = title; */
        /* web._docUrl = link; */
        /* web._docContent = content; */
        /* _resultQue.emplace(std::make_shared<WebPage>(web),docScoreWeight); */
        _resultQue.emplace(web,docScoreWeight);
    }
    /* auto tempQue = std::move(_resultQue); */
    return _resultQue;
    /* return tempQue; */
}

int WebPageQuery::getTF(const string & content,const string & word){
    int count = 0;
    size_t pos = content.find(word);

    while (pos != std::string::npos) {
        ++count;
        // 从下一个字符继续搜索
        pos = content.find(word, pos + word.size());
    }
    return count;
}

int WebPageQuery::getDF(const string & word){
    return _invertIndexLib[word].size();
}

vector<double> WebPageQuery::calcuteWeights(const string & content,const vector<string> words){
    vector<double> res;
    for(auto word:words){
        int tf = getTF(content,word);
        int df = getDF(word);
        double idf = log((double)WebPageQuery::docCnt /(df+1) + 1);
        res.push_back(idf * tf);
    }
    return res;
}

double WebPageQuery::getCosin(const vector<double>& candidate){
    double res=0 , square_query = 0,square_candidate=0 , total=0;   
    for(size_t i=0;i<candidate.size();++i){
        total = _queryWordsWeight[i] * candidate[i];
        square_query += _queryWordsWeight[i] * _queryWordsWeight[i];
        square_candidate += candidate[i] * candidate[i];
    }
    res = total / (sqrt(square_query) * sqrt(square_candidate));
    return res;
}

priority_queue<pair<shared_ptr<WebPage>,double>,vector<pair<shared_ptr<WebPage>,double>>,MyCompareWebpage> WebPageQuery::doQuery(string queryWords){
    _splitedWords.clear();
    _queryWordsWeight.clear();
    _pagesWeight.clear();
    while(!_resultQue.empty()){
        _resultQue.pop();
    }

    _splitedWords =_wordCutTool->cut(queryWords);
    set<string> uniqueWords(_splitedWords.begin(),_splitedWords.end());
    _splitedWords = vector<string>(uniqueWords.begin(),uniqueWords.end());
    if(_splitedWords.size()){
        storeIntersection();
        _queryWordsWeight = calcuteWeights(queryWords,_splitedWords);
        for(size_t i =0 ; i < _queryWordsWeight.size();++i){
            /* cout << "word " << _splitedWords[i] << " weight " << _queryWordsWeight[i] <<"\n"; */
        }
        //计算候选文章的权重
        for(pair<int,WebPage> couple:_pages){
            _pagesWeight[couple.first] = calcuteWeights(couple.second._docContent,_splitedWords);
        }
        for(auto couple:_pagesWeight){
            /* cout << "docId " << couple.first << "\n"; */
            /* for(size_t i =0 ; i < couple.second.size();++i){ */
            /*     cout << "word " << _splitedWords[i] << " weight " << couple.second[i] <<"\n"; */
            /* } */
            double cosin = getCosin(couple.second);
            /* cout <<  " consi " << cosin << "page" << _pages[couple.first]._docTitle  << "\n"; */
            _resultQue.emplace(std::make_shared<WebPage>(_pages[couple.first]),cosin);
        }

        /* while(_resultQue.size()){ */
        /*     WebPage web = _resultQue.top().first; */
        /*     cout << _resultQue.top().second <<  web._docUrl  <<"\n"; */
        /*     _resultQue.pop(); */
        /* } */
    }
    return _resultQue;
}

void WebPageQuery::storeIntersection(){
    _pages.clear();
    //存放每个关键词对应的偏移信息
    vector<set<pair<int,pair<int,int>>>> vectorOffset;
    for(auto word : _splitedWords){
        /* cout << word << "\n"; */
        //在倒排索引找到文档id
        set<pair<int,double>> tempInvert = _invertIndexLib[word];
        //存放倒排id对应的偏移信息
        set<pair<int,pair<int,int>>> tempOffset;
        for_each(tempInvert.begin(),tempInvert.end(),[&](pair<int,double>couple){
                 tempOffset.insert(std::make_pair(couple.first,_offsetlib[couple.first]));
                 });
        vectorOffset.push_back(tempOffset);
    }

    //存放偏移信息并集
    set<pair<int,pair<int,int>>> resOffset = vectorOffset[0];

    for(size_t i=1;i<vectorOffset.size();++i){
        set<pair<int,pair<int,int>>> temp = vectorOffset[i];
        set<pair<int,pair<int,int>>> tempRes;
        set_intersection(resOffset.begin(),resOffset.end(),
                         temp.begin(),temp.end(),
                         std::insert_iterator<set<pair<int,pair<int,int>>>>(tempRes,tempRes.begin()));
        resOffset = tempRes;
    }

    ifstream ifsPages(Configuration::getInstance()->getConfigMap()["NEWPAGE"]);
    if(!ifsPages){
        perror("open NEWpage");
        exit(1);
    }

    //存储查询到的文档
    for(auto couple:resOffset){
        size_t docId=couple.first , start_pos=couple.second.first,len=couple.second.second;
        ifsPages.seekg(start_pos,std::ios_base::beg);
        /* cout << docId << " " << start_pos << " " << len << "\n"; */
        vector<char> docItem;
        docItem.resize(len);
        ifsPages.read(&docItem[0],len);
        string doc(docItem.begin(),docItem.end());
        /* cout << doc  ; */
        _pages[docId] = getWebPage(doc);

        ifsPages.seekg(0);
    }
    ifsPages.close();
}

WebPage WebPageQuery::getWebPage(string & doc){
    WebPage web;
    web._doc = doc;
    size_t start,end;
    start = doc.find("<docid>") +7;
    end = doc.find("</docid>");
    web._docId = std::stoi(string(doc.begin()+start,doc.begin()+end));

    start = doc.find("<title>") +7;
    end = doc.find("</title>");
    web._docTitle = string(doc.begin()+start,doc.begin()+end);

    start = doc.find("<link>") +6;
    end = doc.find("</link>");
    web._docUrl = string(doc.begin()+start,doc.begin()+end);

    start = doc.find("<content>") + 9;
    end = doc.find("</content>");
    web._docContent = string(doc.begin()+start,doc.begin()+end);
    return web;
}

void WebPageQuery::loadLibary(){
    ifstream ifsOffset(Configuration::getInstance()->getConfigMap()["NEWOFFSET"]);
    ifstream ifsInvert(Configuration::getInstance()->getConfigMap()["INVERTINDEX"]); 
    int docId,start_pos,len;
    while(ifsOffset >> docId >> start_pos >> len){
        _offsetlib[docId] = std::make_pair(start_pos,len);
    }
    WebPageQuery::docCnt = docId;

    string word,line;
    double tf_idf; 
    while(getline(ifsInvert,line)){
        istringstream iss(line);
        iss >> word;
        while(iss >> docId>>tf_idf){
            _invertIndexLib[word].insert(std::make_pair(docId,tf_idf));
        }
    }

    ifsOffset.close();
    ifsInvert.close();
}

void WebPageQuery::destroy(){
    if(_pInstance != nullptr){
        delete _pInstance;
        _pInstance = nullptr;
    }
}


WebPageQuery::WebPageQuery()
     :_cacheLRU(10)
    ,_wordCutTool(new SplitToolCppJieba())
{
    loadLibary();
}
WebPageQuery::~WebPageQuery(){
    delete _wordCutTool;
}

