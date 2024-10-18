#ifndef __DirScanner_H__
#define __DirScanner_H__

#include "travial.h"

class DirScanner
{
public:
    DirScanner();
    void traverse(string dir);
    vector<string> & getFiles();
    ~DirScanner();

private:
    vector<string> _files;
};

#endif 

