#include "PageLibPreprocessor.h"

using namespace std;

int main(int argc,char * argv[])
{
    clock_t start ,end;
    start = clock();
    PageLibPreprocessor pre;
    /* pre.doProcess(); */
    /* pre.cutRedundantPages(); */
    /* pre.buildInvertIndexTable(); */
    pre.xapianBuildInvertIndexTable();
    end = clock();
    cout << "run time " <<(double)(end-start)/CLOCKS_PER_SEC << "\n";

    return 0;
}

