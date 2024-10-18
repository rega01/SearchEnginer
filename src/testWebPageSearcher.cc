/* #include "WebPageQuery.h" */
#include "WebPageSearcher.h"

#include <time.h>

int main(int argc,char * argv[])
{
    clock_t start,end;
    /* start=clock(); */
    /* WebPageQuery::createInstance()->doQuery("一一人民"); */
    /* end=clock(); */
    /* cout << "run time " <<(double)(end-start)/CLOCKS_PER_SEC << "\n"; */
    /* cout << "\n\n"; */
    /* /1* start=clock(); *1/ */
    /* /1* WebPageQuery::createInstance()->doQuery("国台湾中"); *1/ */
    /* /1* end=clock(); *1/ */
    /* cout << "run time " <<(double)(end-start)/CLOCKS_PER_SEC << "\n"; */

    /* WebPageSearcher web("国台湾中"); */
    WebPageSearcher web("一一人民");
    web.doQuery();
    return 0;
}

