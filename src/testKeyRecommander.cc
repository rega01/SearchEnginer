#include "Dictionary.h"
#include "KeyRecommander.h"


int main(int argc,char * argv[])
{
    //测试词典查询
    /* set<pair<string,int>> ret =  Dictionary::createInstance()->doQuery("z"); */
    /* for(pair<string,int> pir:ret){ */
    /*     cout << pir.first << " " <<pir.second << "\n"; */
    /* } */

    KeyRecommander keyRec("王i道v");
    keyRec.queryIndexTable();

    return 0;
}

