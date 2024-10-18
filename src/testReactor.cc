#include "Task.h"
#include "SearchServer.h"
#include <iostream>

using namespace std;

int main(int argc,char * argv[])
{
    SearchServer echo(2,10,"0.0.0.0",8080);

    echo.setTcpAllCallback();
    echo.start();
    return 0;
}

