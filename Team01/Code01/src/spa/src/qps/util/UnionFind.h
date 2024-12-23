#pragma once

#include <vector>
#include <string>
#include <unordered_map>

// ai-gen start(gpt, 0, e)
// prompt: https://chat.openai.com/share/b1a25e52-491e-4091-a466-bde718df7107
class UnionFind {
private:
    std::vector<std::string> parent;
    std::vector<int> rank;
    std::unordered_map<std::string, int> stringToIndex;

public:
    UnionFind();
    void addString(const std::string& str);
    std::string find(const std::string& str);
    void unite(const std::string& str1, const std::string& str2);
    bool same(const std::string& str1, const std::string& str2);
};
// ai-gen end
