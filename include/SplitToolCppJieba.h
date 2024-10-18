#ifndef __SplitToolCppJieba_H__
#define __SplitToolCppJieba_H__

#include "SplitTool.h"
#include <cppjieba/Jieba.hpp>

class SplitToolCppJieba
:public SplitTool
{
public:
    SplitToolCppJieba();
    virtual vector<string> cut(const string & strToCut) override;
    ~SplitToolCppJieba();
    SplitToolCppJieba(const SplitToolCppJieba &) =delete;
    SplitToolCppJieba(SplitToolCppJieba &&) =delete;
    SplitToolCppJieba & operator=(const SplitToolCppJieba&) =delete;
    SplitToolCppJieba & operator=(SplitToolCppJieba &&) =delete;

private:
    cppjieba::Jieba _jieba;
};

#endif 

