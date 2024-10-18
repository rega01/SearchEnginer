#include <string>
#include <iostream>

#include <unordered_map>
#include <set>

#include <cppjieba/Jieba.hpp>

using std::string;

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

int main(int argc,char * argv[])
{
    string s = "hello倪哥im大";
    for(size_t i = 0;i<s.size();){
        char ch = s[i];
        if(ch & 0X80){
            std::cout << s.substr(i,3) << "\n";
            std::cout << getPrefixOne(ch) << "\n";
            i += 3;
        }else{
            std::cout << s.substr(i,1) << "\n";
            std::cout << getPrefixOne(ch) << "\n";
            ++i;
        }
    }
    char ch = '0X11111110';
    std::cout << getPrefixOne(ch) << "\n";
    std::unordered_map<string,std::set<int,double>> _invertIndexLib;
    _invertIndexLib["fuck"].insert(std::pair(5,0.0));

    return 0;
}


