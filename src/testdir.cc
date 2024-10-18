#include <iostream>

#include "DirScanner.h"

using namespace std;

int main(int argc,char * argv[])
{
    DirScanner dirscanner;
    dirscanner.traverse("/home/cmj/daily_use/searchProject/01_moudle/art");
    auto & files = dirscanner.getFiles();
    for(auto val : files){
        cout << val << endl;
    }
    return 0;
}

