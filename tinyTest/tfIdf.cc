#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <sstream>
#include <set>

// 计算TF-IDF
class TFIDF {
public:
    // 计算每个文档的TF值
    std::map<std::string, double> computeTF(const std::vector<std::string>& doc) {
        std::map<std::string, int> wordCount;
        std::map<std::string, double> tf;
        
        // 统计每个词出现的次数
        for (const std::string& word : doc) {
            wordCount[word]++;
        }

        // 计算每个词的TF值
        for (const auto& pair : wordCount) {
            tf[pair.first] = (double)pair.second / doc.size();
        }

        return tf;
    }

    // 计算所有文档的IDF值
    std::map<std::string, double> computeIDF(const std::vector<std::vector<std::string>>& docs) {
        std::map<std::string, int> docCount;
        std::map<std::string, double> idf;
        int N = docs.size();

        // 统计每个词出现在多少文档中
        for (const std::vector<std::string>& doc : docs) {
            std::set<std::string> uniqueWords(doc.begin(), doc.end());
            for (const std::string& word : uniqueWords) {
                docCount[word]++;
            }
        }

        // 计算每个词的IDF值
        for (const auto& pair : docCount) {
            idf[pair.first] = log((double)N / (pair.second + 1)) + 1; // +1避免除零
        }

        return idf;
    }

    // 计算每个文档的TF-IDF值
    std::map<std::string, double> computeTFIDF(const std::vector<std::string>& doc, const std::map<std::string, double>& idf) {
        std::map<std::string, double> tf = computeTF(doc);
        std::map<std::string, double> tfidf;

        for (const auto& pair : tf) {
            tfidf[pair.first] = pair.second * idf.at(pair.first);
        }

        return tfidf;
    }
};

int main() {
    // 示例文档集合
    std::vector<std::vector<std::string>> docs = {
        {"this", "is", "a", "sample", "document"},
        {"this", "document", "is", "another", "example", "document"},
        {"one", "more", "sample", "document"},
        {"this", "is", "the", "last", "sample", "document"}
    };

    TFIDF tfidfCalculator;

    // 计算IDF值
    std::map<std::string, double> idf = tfidfCalculator.computeIDF(docs);

    // 对每个文档计算TF-IDF
    for (size_t i = 0; i < docs.size(); ++i) {
        std::map<std::string, double> tfidf = tfidfCalculator.computeTFIDF(docs[i], idf);
        
        std::cout << "Document " << i + 1 << " TF-IDF values:\n";
        for (const auto& pair : tfidf) {
            std::cout << pair.first << ": " << pair.second << "\n";
        }
        std::cout << std::endl;
    }

    return 0;
}

