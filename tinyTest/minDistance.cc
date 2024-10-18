#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <codecvt>
#include <locale>

// 辅助函数：将 UTF-8 字符串转换为 UTF-32 字符串
std::u32string utf8ToUtf32(const std::string& utf8Str) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.from_bytes(utf8Str);
}

class Solution {
public:
    int minDistance(const std::string& word1, const std::string& word2) {
        // 将 UTF-8 格式字符串转换为 UTF-32，能够正确处理英文字母和汉字混合的情况
        std::u32string u32Word1 = utf8ToUtf32(word1);
        std::u32string u32Word2 = utf8ToUtf32(word2);

        int n = u32Word1.size(), m = u32Word2.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));

        // 初始化边界条件
        for (int i = 0; i <= n; ++i) dp[i][0] = i;
        for (int j = 0; j <= m; ++j) dp[0][j] = j;

        // 动态规划计算编辑距离
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (u32Word1[i - 1] == u32Word2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = 1 + std::min(dp[i - 1][j - 1], std::min(dp[i][j - 1], dp[i - 1][j]));
            }
        }
        return dp[n][m];
    }
};

int main() {
    Solution solution;

    // 示例1: 混合汉字和英文字母
    std::string word1 = "hello世界";  // 英文字母 + 汉字
    std::string word2 = "hi世界";     // 英文字母 + 汉字
    std::cout << "编辑距离 (英文字母 + 汉字): " << solution.minDistance(word1, word2) << std::endl;

    // 示例2: 全是汉字
    word1 = "hell王";
    word2 = "hiw";
    std::cout << "编辑距离 (汉字): " << solution.minDistance(word1, word2) << std::endl;

    // 示例3: 全是英文字母
    word1 = "王道";
    word2 = "world";
    std::cout << "编辑距离 (英文字母): " << solution.minDistance(word1, word2) << std::endl;

    return 0;
}

