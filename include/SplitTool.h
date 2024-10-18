#ifndef __SplitTool_H__
#define __SplitTool_H__

#include "travial.h"

class SplitTool
{
public:
    virtual vector<string> cut(const string &)=0;
    virtual ~SplitTool(){}
};

#endif 

