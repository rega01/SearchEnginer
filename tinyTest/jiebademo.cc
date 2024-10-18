#include <cppjieba/Jieba.hpp>

using namespace std;

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

int main(int argc, char** argv) {
    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          USER_DICT_PATH,
                          IDF_PATH,
                          STOP_WORD_PATH);
    vector<string> words;
    vector<cppjieba::Word> jiebawords;
    /* string s; */
    string result;
    string s("在仙游期间，听闻了许多福森董事长王福森的创业故事，参观完福森展厅后，令我们对这家企业的印象更加深刻。企业的发展历程于王福森而言仿佛历历在目");

    /* s = "他来到了fuck网易 杭研大厦"; */
    cout << s << endl;
    cout << "[demo] Cut With HMM" << endl;
    jieba.Cut(s, words, true);
    cout << limonp::Join(words.begin(), words.end(), "/") << endl;

    /* cout << count(s.begin(),s.end(),"二") <<"\n"; */

    return EXIT_SUCCESS;
}
