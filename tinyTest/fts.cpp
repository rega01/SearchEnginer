#include <iostream>
#include <fts.h>   // fts API
#include <cstring> // strerror
#include <cstdlib> // exit

using namespace std;

int main(int argc, char* argv[]) {
    /* if (argc < 2) { */
    /*     cout << "Usage: " << argv[0] << " <directory>" << endl; */
    /*     return 1; */
    /* } */

    // 创建目录路径列表
    char* paths[] = {const_cast<char*>("/home/cmj/daily_use/searchProjectv1/01_moudle/art"), nullptr };

    // 打开文件树，返回指向 `fts` 结构体的指针
    FTS* file_system = fts_open(paths,FTS_NOCHDIR , nullptr);
    if (!file_system) {
        perror("fts_open");
        exit(EXIT_FAILURE);
    }

    // 读取文件树中的节点
    FTSENT* node;
    while ((node = fts_read(file_system)) != nullptr) {
        switch (node->fts_info) {
            case FTS_F:   // 普通文件
                cout << "File: " << node->fts_path << endl;
                break;
            case FTS_D:   // 目录
                cout << "Directory: " << node->fts_path << endl;
                break;
            case FTS_DP:  // 目录的末尾
                break;
            case FTS_SL:  // 符号链接
                cout << "Symbolic Link: " << node->fts_path << endl;
                break;
            case FTS_NS:  // 无法访问的文件
                perror("fts_read");
                break;
            default:
                break;
        }
    }

    // 关闭文件树
    if (fts_close(file_system) < 0) {
        perror("fts_close");
        exit(EXIT_FAILURE);
    }

    return 0;
}

