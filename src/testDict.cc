#include "DictProducer.h"


int main(int argc,char * argv[])
{
    DictProducer dictp("/home/cmj/daily_use/searchProject/01_moudle/art");
    dictp.buildCnDict();
    dictp.buildEnDict();
    dictp.store();

    return 0;
}

