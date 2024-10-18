#include "DirScanner.h"

#include <fts.h>
#include <limits.h>
#include <iostream>

DirScanner::DirScanner(){}

void DirScanner::traverse(string dir){
    char *paths[] = {const_cast<char*>(dir.c_str()),nullptr};
    FTS* file_system = fts_open(paths,FTS_NOCHDIR,nullptr);
    if(!file_system){
        perror("fts open");
        exit(EXIT_FAILURE);
    }
    FTSENT *node;
    while((node = fts_read(file_system)) != nullptr){
        if(node->fts_info == FTS_F){
            char realPath[1280];
            _files.push_back(realpath(node->fts_path,realPath));
        }
    }
    if(fts_close(file_system) < 0){
        perror("fts_close");
        return;
    }
}

vector<string> & DirScanner::getFiles(){
    return _files;
}

DirScanner::~DirScanner(){
    
}
