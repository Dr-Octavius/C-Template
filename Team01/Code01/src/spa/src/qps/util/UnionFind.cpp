#include "UnionFind.h"

// ai-gen start(gpt, 1, e)
// prompt: https://chat.openai.com/share/b1a25e52-491e-4091-a466-bde718df7107
UnionFind::UnionFind() = default;

void UnionFind::addString(const std::string& str) {
    if (stringToIndex.find(str) == stringToIndex.end()) {
        int newIndex = parent.size();
        parent.push_back(str);
        rank.push_back(0);
        stringToIndex[str] = newIndex;
    }
}

std::string UnionFind::find(const std::string& str) {
    if (stringToIndex.find(str) != stringToIndex.end()) {
        int index = stringToIndex[str];
        if (parent[index] != str) {
            parent[index] = find(parent[index]); // Path compression
        }
        return parent[index];
    }
    return "";
}

void UnionFind::unite(const std::string& str1, const std::string& str2) {
    if (stringToIndex.find(str1) != stringToIndex.end() && stringToIndex.find(str2) != stringToIndex.end()) {
        std::string rootX = find(str1);
        std::string rootY = find(str2);
        if (rootX != rootY) {
            // Union by rank heuristic
            int indexX = stringToIndex[rootX];
            int indexY = stringToIndex[rootY];
            if (rank[indexX] < rank[indexY]) {
                parent[indexX] = rootY;
            } else if (rank[indexX] > rank[indexY]) {
                parent[indexY] = rootX;
            } else {
                parent[indexY] = rootX;
                rank[indexX]++;
            }
        }
    }
}

bool UnionFind::same(const std::string& str1, const std::string& str2) {
    return find(str1) == find(str2);
}
// ai-gen end
