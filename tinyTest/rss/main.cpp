//main.cpp
#include "RSS.hpp"
#include <tinyxml2.h>
#include<cerrno>
#include<cstdlib>
#include<cstring>
#include<ctime>
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using namespace tinyxml2;

void test0()

{
    /* XMLDocument * doc = new XMLDocument(); */
    /* doc->LoadFile("./rss-cainiao.xml"); */
    /* int errorID = doc->ErrorID(); */
    /* cout << "Errornum " << errorID<<endl; */

    /* if(errorID){ */
    /*     cout << "read file "<<endl; */
    /*     exit(1); */
    /* } */

    /* doc->Print(); */
    std::ifstream ifs("./rss-cainiao.xml",std::ios_base::ate);
    size_t length = ifs.tellg();
    char * pdata = new char[length+1]();

    ifs.seekg(0);
    ifs.read(pdata,length);
    cout << pdata <<endl;
    
    XMLDocument doc;
    doc.Parse(pdata);

    XMLElement* titleElement = doc.FirstChildElement( "rss" )
        ->FirstChildElement( "channel" )
        ->FirstChildElement( "title" )
        ;
    if(titleElement == nullptr){
        cout << "child" <<endl;
        doc.Print();
    }
    const char* title = titleElement->GetText();
    printf(  "Name of play (1): %s\n", title );

    XMLElement* linkElement = doc.FirstChildElement( "rss" )
        ->FirstChildElement( "channel" )
        ->FirstChildElement( "link" )
        ;
    XMLElement * desnode = titleElement->NextSiblingElement("description");
    cout << desnode->GetText() <<endl;
    /* cout << desnode->FirstChild()->ToText() <<endl; */

    cout << endl;
    XMLElement * next = titleElement->NextSiblingElement();
    cout << next->GetText() <<endl;
    
    cout << endl;
    next = next->NextSiblingElement();
    cout << next->GetText() <<endl;
    cout << endl;

    cout << endl;
    next = next->NextSiblingElement();
    cout << strcmp(next->Name(),("iem")) <<endl;
    cout << endl;

    cout << endl;
    next = next->NextSiblingElement();
    cout << (next->Name()) <<endl;
    cout << endl;

    /* XMLText* textNode = linkElement->FirstChild()->ToText(); */
    /* title = textNode->Value(); */
    const char* link = linkElement->GetText();
    printf( "link: %s\n", link );

}

void test1(){
    RssReader rss("./feed.xml");
    rss.print();
    rss.dump("./pagelib.txt");
}


int main(){
    test1();
    return 0;
} 
