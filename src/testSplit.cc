#include "SplitTool.h"
#include "SplitToolCppJieba.h"

using namespace std;

int main(int argc,char * argv[])
{
    SplitTool * sptool = new SplitToolCppJieba();
    string s="打撒附件撒今后打撒环境搭建的，打撒放技能";
    vector<string> words = sptool->cut(s);
    for(auto word:words){
        cout << word << "\n";
    }
    return 0;
}

