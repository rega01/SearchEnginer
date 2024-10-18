#include "Task.h"
#include "KeyRecommander.h"
#include "WebPageSearcher.h"
#include <nlohmann/json.hpp>

Task::Task(const std::string &msg,std::shared_ptr<TcpConnection> pcon)
    :_msg(msg)
     ,_con(pcon)
{}

void Task::process(LRU* cacheLocal){
    nlohmann::json json = nlohmann::json::parse(_msg);
    string type = json["type"];
    /* cout << type << "\n"; */
    string query =  json["value"];
    /* cout << query << "\n"; */

    if(type == "KeyRecommander"){
        KeyRecommander keyRec(query,_con);
        keyRec.execute();
        keyRec.response();
    }
    else if(type == "WebPageSeacher"){
        WebPageSearcher web(query,_con,cacheLocal);
        web.doQuery();
    }
}

Task::~Task(){

}

