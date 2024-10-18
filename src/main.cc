#include "Task.h"
#include "SearchServer.h"
#include <iostream>

using namespace std;


int main(int argc,char * argv[])
{
    SearchServer server(2,10,"0.0.0.0",8080);

    server.setTcpAllCallback();
    server.setLRUCallback();
    server.start();
    return 0;
}
