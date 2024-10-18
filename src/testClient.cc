//client.c
#include "travial.h"
#include <myownHead.h>
#include <nlohmann/json.hpp>

#define BUFSIZE 1024
#define PORT 8080
#define ADDR "0.0.0.0"

int main(int argc,char*argv[])
{
    struct sockaddr_in serAddr;
    int cliFd = socket(AF_INET,SOCK_STREAM,0);

    memset(&serAddr,0,sizeof(serAddr));
    serAddr.sin_family =AF_INET;
    serAddr.sin_port =htons(PORT);
    serAddr.sin_addr.s_addr = inet_addr(ADDR);

    int ret = connect(cliFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    if(ret){
        printf("connect success\n");
    }
    while(1){
        nlohmann::json jsonreq;
        /* jsonreq["type"] = "KeyRecommander"; */
        /* jsonreq["value"] = "王v道"; */

        jsonreq["type"] = "WebPageSeacher";
        char query[BUFSIZE] = {0};
        fgets(query,BUFSIZE,stdin);
        query[strlen(query)-1] = '\0';
        jsonreq["value"] = query;
        string req = jsonreq.dump();
        int len = req.size();
        write(cliFd,&len,sizeof(len));
        send(cliFd,req.data(),strlen(req.data()),0);

        int retLen = 0;
        read(cliFd,&retLen,sizeof(retLen));
        vector<char> buff;
        buff.resize(retLen);
        /* cout << "len " << retLen <<"\n"; */
        recv(cliFd,&buff[0],retLen,0);
        string res(buff.begin(),buff.end());
        /* cout << "buff " << res <<"\n"; */
        nlohmann::json json = nlohmann::json::parse(res);
        size_t valueSize = json["length"];
        if(json["type"] == "KeyRecommander"){
            for(size_t i=0;i<valueSize;++i){
                cout << json["value"][i]["word"] <<" " 
                    /* << json["value"][i]["freq"] << " " */
                    /* << json["value"][i]["distance"] << */
                    "\n";
            }
        }
        else if(json["type"] == "WebPageSeacher"){
            for(size_t i=0;i<valueSize;++i){
                cout << json["value"][i]["title"] <<"\n " 
                    << json["value"][i]["url"] << " "
                    /* << json["value"][i]["abstract"] << " " */
                    /* << json["value"][i]["content"] << */
                    "\n";
            }
        }
    }
    close(cliFd);

    return 0;
}

