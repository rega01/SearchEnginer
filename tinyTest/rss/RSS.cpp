//RSS.cpp
#include "RSS.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <tinyxml2.h>
using std::cout;
using std::endl;
using std::cin;
using namespace tinyxml2;

RssReader::RssReader(const char * filename){
    std::ifstream ifs(filename,std::ios_base::ate);
    if(!ifs){
        cout << "open file" <<endl;
        return ;
    }

    size_t fileLength = ifs.tellg();
    ifs.seekg(0);
    char * pfile = new char[fileLength+1]();
    ifs.read(pfile,fileLength);
    ifs.close();

    XMLDocument doc;
    doc.Parse(pfile);
    
    XMLElement * element = doc.FirstChildElement("rss")
        ->FirstChildElement("channel")
        ->FirstChildElement("item");
    while(element){
        RssItem rss;
        rss.title = element->FirstChildElement("title")?
            element->FirstChildElement("title")->GetText() : "null";
        rss.link =element->FirstChildElement("link")?
            element->FirstChildElement("link")->GetText() : "null";
        if(element->FirstChildElement("description")->GetText()){
            std::string s(element->FirstChildElement("description")->GetText());
            std::regex re("<p>(.*?)</p>");
            std::smatch match;
            std::regex_search(s,match,re);
            rss.description= match[1];
        }
        /* rss.description =element->FirstChildElement("description")? */
        /*     element->FirstChildElement("description")->GetText() : "null"; */
        if(element->FirstChildElement("content:encoded")->GetText()){
            std::string ori_s = element->FirstChildElement("content:encoded")->GetText();
            std::regex re("<p>((?:(?!<).)*?)</p>");
            std::smatch match;
            std::string res_s;
            auto search_start = ori_s.cbegin();
            while(std::regex_search(search_start,ori_s.cend(),match,re),match.size()){
                res_s += match[1].str();
                search_start = match.suffix().first;
            }
            rss.content = res_s;
        }
        /* rss.content =element->FirstChildElement("content:encoded")? */
        /*     element->FirstChildElement("content:encoded")->GetText() : "null"; */
        _rss.push_back(rss);

        element = element->NextSiblingElement();
    }

}
void RssReader::dump(const std::string & filename){
    std::ofstream ofs(filename);
    int idNum = 1;
    for(auto & rss:_rss){
        ofs << "<doc>" <<endl;
        ofs << "<docid>" << idNum++ << "</docid>" <<endl;
        ofs << "<title>" << rss.title << "</title>" <<endl;
        ofs << "<link>" << rss.link << "</link>" <<endl;
        ofs << "<description>" << rss.description << "</description>" <<endl;
        ofs << "<content>" << rss.content << "</content>" <<endl;
        ofs << "</doc>" <<endl;
    }
    ofs.close();
}

void RssReader::print(){
    for(auto & item : _rss){
        cout << "--------------------------------------------------------" <<endl;
        cout << item.title <<endl;
        cout << item.link <<endl;
        cout << item.description <<endl;
        cout << item.content <<endl;
        cout << "--------------------------------------------------------" <<endl;
    }
}

