#include "PageLibPreprocessor.h"
#include "DirScanner.h"
#include "Configuration.h"
#include "SplitToolCppJieba.h"

#include <regex>
#include <fstream>
#include <sstream>

#include <tinyxml2.h>
#include <xapian.h>
#include <simhash/Simhasher.hpp>
#define PAGEMAXSIZE 5

using std::ifstream;
using std::ofstream;
using std::ostringstream;
using namespace tinyxml2;
using simhash::Simhasher;
int PageLibPreprocessor::docId = 1;
int PageLibPreprocessor::_offSet=0;

PageLibPreprocessor::PageLibPreprocessor()
    :_pageList()
     ,_wordCutter(new SplitToolCppJieba)
{}

void PageLibPreprocessor::doProcess(){
    DirScanner scaner;
    scaner.traverse(Configuration::getInstance()->getConfigMap()["PAGES_DIR"]);
    vector<string> allXml = std::move(scaner.getFiles());

    for(size_t i=0;i<allXml.size();++i){
        string curPath = allXml[i];
        ifstream ifs(curPath,std::ios_base::in|std::ios_base::ate);
        size_t totalSize = ifs.tellg();
        ifs.seekg(0,std::ios_base::beg);
        vector<char> file;
        file.resize(totalSize);
        ifs.read(file.data(),totalSize);
        ifs.close();
        file.push_back('\0');
        XMLDocument doc;
        doc.Parse(file.data(),file.size());
        XMLElement * upitem= doc.FirstChildElement("rss");
        XMLElement * item = upitem ? upitem->FirstChildElement("channel")->
            FirstChildElement("item") : nullptr;
        while(item){
            WebPage page;
            page._doc = file.data();
            page._docTitle = item->FirstChildElement("title")->GetText();
            page._docUrl = item->FirstChildElement("link")->GetText();
            XMLElement * contentEle = item->FirstChildElement("description");
            string dirtyContent ;
            if(contentEle){
                dirtyContent = contentEle->GetText();   
            }else {
                contentEle=item->FirstChildElement("content:encoded");
                if(contentEle){
                    dirtyContent = contentEle->GetText();
                    cout << curPath << "\n";
                }
            }

            if(dirtyContent.size() > 0){
                page._docId = docId++;
                std::regex re("<[^>]+>");
                page._docContent = regex_replace(dirtyContent,re,"");
                std::regex jsRe("showPlayer\\(\\{[^\\}]*\\}\\);");
                page._docContent = std::regex_replace(page._docContent, jsRe, "");
                _pageList.push_back(page);

                if(_pageList.size() == (PAGEMAXSIZE)){
                    storePages();
                    _pageList.clear();
                }else
                {}
            }else{
                /* cout << "No content or descrip\n" << curPath <<"\n"; */
            }
            item = item->NextSiblingElement();
        }
    }
    storePages();
    storeOffset();
}

void PageLibPreprocessor::storePages(){
    ofstream ofs;
    if(docId==6){
        ofs.open(Configuration::getInstance()->getConfigMap()["OLDPAGE"],std::ios_base::out|std::ios_base::trunc);
    }else{
        ofs.open(Configuration::getInstance()->getConfigMap()["OLDPAGE"],std::ios_base::app);
    }
    
    for(WebPage & page:_pageList){
        ostringstream singleDoc;
        singleDoc << "<doc>\n\t" 
            << "<docid>" <<page._docId <<"</docid>\n"
            << "\t<title>" <<page._docTitle << "</title>\n"
            << "\t<link>" << page._docUrl << "</link>\n"
            << "\t<content>" << page._docContent << "</content>\n"
            << "</doc>\n";
        string strDoc(singleDoc.str());
        _offsetLib[page._docId] = std::make_pair(_offSet,strDoc.size());
        ofs << singleDoc.str();
        /* cout << page._docId << " " << _offSet << " " << strDoc.size()<<"\n"; */
        _offSet += strDoc.size();
    }
    ofs.close();
}

void PageLibPreprocessor::storeOffset(){
    ofstream ofs(Configuration::getInstance()->getConfigMap()["OLDOFFSET"],
                 std::ios_base::out | std::ios_base::trunc);

    for(auto off : _offsetLib){
        ofs << off.first << " " << off.second.first << " " << off.second.second <<"\n";
    }
    ofs.close();
}

void PageLibPreprocessor::cutRedundantPages(){
    //旧偏移流
    ifstream oldOffsetStream(Configuration::getInstance()->getConfigMap()["OLDOFFSET"]);
    //旧文档流
    ifstream docStream(Configuration::getInstance()->getConfigMap()["OLDPAGE"]);
    //新文档流
    ofstream newRipepage(Configuration::getInstance()->getConfigMap()["NEWPAGE"],std::ios_base::trunc|std::ios_base::out);
    //新偏移文件
    ofstream newOffset(Configuration::getInstance()->getConfigMap()["NEWOFFSET"],std::ios_base::trunc|std::ios_base::out);
    size_t newDocId=1,newOffSet = 0;

    Simhasher simhasher(Configuration::getInstance()->getConfigMap()["JIEBA_DICT_PATH"],
                        Configuration::getInstance()->getConfigMap()["JIEBA_HMM_MODEL"],
                        Configuration::getInstance()->getConfigMap()["JIEBA_IDF_PATH"],
                        Configuration::getInstance()->getConfigMap()["JIEBA_STOP_DICT"]);
    size_t docId ,docOffset,docSize;
    while(oldOffsetStream >> docId >> docOffset >> docSize){
        //读出字符
        vector<char> doc;
        doc.resize(docSize);
        docStream.read(doc.data(),docSize);
        string docstr(doc.begin(),doc.end());
        /* cout << docstr <<"\n"; */
        size_t start =docstr.find("<content>")+9 ,end=docstr.find("</content>");
        string content(doc.begin()+start,doc.begin()+end);
        uint64_t u64 = 0;
        simhasher.make(content,5,u64);
        
        //判断是否有重复
        bool isSame=false;
        for(auto finger:_fingerPrints){
            if(Simhasher::isEqual(finger,u64,5)){
                isSame=true;
                break;
            }
        }
        //无重复的指纹
        if(!isSame){
            _fingerPrints.push_back(u64);
            //替换docid,写入新page文件
            start = docstr.find("<docid>")+7,end=docstr.find("</docid>");
            docstr.replace(start,end-start,std::to_string(newDocId));
            newRipepage << docstr;
            newOffset << newDocId << " " << newOffSet << " " << docstr.size()<<"\n";
            newOffSet += docstr.size();
            ++newDocId;
        }
    }
    oldOffsetStream.close();
    docStream.close();
    newRipepage.close();
    newOffset.close();
}

void PageLibPreprocessor::xapianBuildInvertIndexTable(){
    ifstream offsetStream(Configuration::getInstance()->getConfigMap()["NEWOFFSET"]);
    ifstream pageStream(Configuration::getInstance()->getConfigMap()["NEWPAGE"]);
    const char* const K_DB_PATH = "index_data";
    Xapian::WritableDatabase db(K_DB_PATH,Xapian::DB_CREATE_OR_OVERWRITE);
    size_t docId ,docOffset,docSize;
    
    while(offsetStream >> docId >> docOffset >> docSize){
        vector<char> vdoc;
        vdoc.resize(docSize);
        pageStream.read(vdoc.data(),docSize);
        string docstr(vdoc.begin(),vdoc.end());
        size_t start =docstr.find("<content>")+9 ,end=docstr.find("</content>");
        string content(vdoc.begin()+start,vdoc.begin()+end);
        Xapian::Document doc;
        doc.set_data(docstr);
        
        vector<string> words = _wordCutter->cut(content);;
        for(auto word:words){
            doc.add_posting(word,1);
            cout << "word =" << word <<"\n";
        }
        string docUniqueId = "Q" + std::to_string(docId);
        doc.add_boolean_term(docUniqueId);
        db.replace_document(docUniqueId,doc);
    }
    db.commit();
    offsetStream.close();
    pageStream.close();
}

void PageLibPreprocessor::buildInvertIndexTable(){
    ifstream offsetStream(Configuration::getInstance()->getConfigMap()["NEWOFFSET"]);
    ifstream pageStream(Configuration::getInstance()->getConfigMap()["NEWPAGE"]);
    ofstream invertStream(Configuration::getInstance()->getConfigMap()["INVERTINDEX"]);
    size_t docId ,docOffset,docSize;
    
    while(offsetStream >> docId >> docOffset >> docSize){
        vector<char> doc;
        doc.resize(docSize);
        pageStream.read(doc.data(),docSize);
        string docstr(doc.begin(),doc.end());
        size_t start =docstr.find("<content>")+9 ,end=docstr.find("</content>");
        string content(doc.begin()+start,doc.begin()+end);
        vector<string> words = _wordCutter->cut(content);;
        for(auto & word :words){
            ++_wordSingleCnt[word][docId];
            _wordAllDocCnt[word].insert(docId);
        }
    }
    size_t N = docId;
    //每个词的IDF
    map<string,double> idfs;
    for(pair<string,set<int>> allCnt:_wordAllDocCnt){
        idfs[allCnt.first] = log((double)N /(allCnt.second.size()+1) +1);
    }
    //每个词的w ,单词、文章id、权重
    map<string,map<int,double>> tf;
    for(pair<string,map<int,int>> words : _wordSingleCnt){
        for(pair<int,int> docAndFreq:words.second){
            tf[words.first][docAndFreq.first] = idfs[words.first] * docAndFreq.second;
        }
    }
    //归一化，每篇文章的权重平方
    map<int,double> docWeights;
    for(auto wordTf : tf){
        for(auto weight : wordTf.second){
            docWeights[weight.first] += weight.second * weight.second;
        }
    }
    //计算逆文档率
    for(auto wordTf : tf){
        for(auto idAndWei : wordTf.second){
            _invertIndexLib[wordTf.first]
                .insert(std::make_pair(
                   idAndWei.first,
                   (double)idAndWei.second/sqrt(docWeights[idAndWei.first])));
        }
    }
    for(auto pir:_invertIndexLib){
        invertStream << pir.first ;
        for(auto sets:pir.second){
            invertStream << " " << sets.first << " " << sets.second  ;
        }
        invertStream << "\n";
    }

    offsetStream.close();
    pageStream.close();
    invertStream.close();
}


PageLibPreprocessor::~PageLibPreprocessor(){
    delete _wordCutter;
}

