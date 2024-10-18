#ifndef __WebPage_H__
#define __WebPage_H__

#include "travial.h"
struct WebPage
{
public:
    WebPage(string doc,int docId,string docTitle,string docUrl,string docContent)
    :_doc(doc)
    ,_docId(docId)
    ,_docTitle(docTitle)
    ,_docUrl(docUrl)
    ,_docContent(docContent)
    {}
    WebPage(){}
    string _doc;
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
};

#endif 

