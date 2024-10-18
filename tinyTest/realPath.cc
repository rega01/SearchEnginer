#include <stdio.h>
#include <limits.h>
#include <stdlib.h>


int main(int argc,char * argv[])
{
    char realPath[100];
    const char *absolutePath = "./demo.cpp";
    printf("%s\n",realpath(absolutePath,realPath));
    return 0;
}

