#include "DictProducer.h"
#include <time.h>

int main(int argc,char * argv[])
{
    clock_t start,end;
    start = clock();
    DictProducer dictp("/home/cmj/daily_use/searchProjectV3.0/01_moudle/art");
    dictp.buildCnDict();
    dictp.buildEnDict();
    dictp.store();
    end = clock();
    cout << "run time " <<(double)(end-start)/CLOCKS_PER_SEC << "\n";

    return 0;
}

