//RSS.hpp
#ifndef __RSS_H__
#define __RSS_H__
#include <string>
#include <vector>

struct RssItem
{
    std::string title;
    std::string link;
    std::string description;
    std::string content;
};

class RssReader
{
public:
    RssReader(const char * filename);
    void parseRss();//解析
    void dump(const std::string & filename);//输出
    void print();//打印
private:
    std::vector<RssItem> _rss;
 };   


#endif 

