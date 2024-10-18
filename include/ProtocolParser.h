#ifndef __ProtocolParser_H__
#define __ProtocolParser_H__

#include "travial.h"

class ProtocolParser
{
public:
    ProtocolParser(string & msg);
    void doParse();
    ~ProtocolParser();

private:
    string _msg;
};

#endif 

