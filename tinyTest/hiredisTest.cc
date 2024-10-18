#include <iostream>

#include <hiredis/hiredis.h>

using namespace std;

int main(int argc,char * argv[])
{
    redisContext * redis = redisConnect("127.0.0.1",6379);

    /* redisReply* reply =static_cast<redisReply*>( redisCommand(redis,"GET ggg")); */
    redisReply* reply =static_cast<redisReply*>( redisCommand(redis,"select %d",5));
    if(reply == nullptr){
        perror("command");
        exit(1);
    }
    
    if(reply->type == REDIS_REPLY_STRING){
        cout << reply->str << "\n";
    }
    else if(reply->type == REDIS_REPLY_STATUS){
        cout<< reply->str<<"\n";
    }

    return 0;
}

