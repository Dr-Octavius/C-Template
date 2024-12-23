#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iterator>
#include "ReadOnlyPatternManager.h"
#include "WriteOnlyPatternManager.h"
#include "common/Utils.h"

class PatternManager : public ReadOnlyPatternManager, public WriteOnlyPatternManager {
private:
    std::unordered_map<std::pair<std::string, EntityType>, std::pair<std::string, std::shared_ptr<RelFactorNode>>, PairHash> patternMap;


public:
    bool insertLhsPattern(const std::pair<std::string, EntityType>& stmtNo,
        const std::pair<std::string, EntityType>& lhs,
        const std::shared_ptr<RelFactorNode>& rhs) override {
        auto inserted = patternMap.insert({ stmtNo, {lhs.first, rhs} });
        return inserted.second;
    }

    bool insertPattern(const std::pair<std::string, EntityType>& stmtNo, const std::shared_ptr<AssignNode>& assignNode) override {
        auto lhs = assignNode->getLeftChild();
        auto rhs = assignNode->getRightChild();
        return insertLhsPattern(stmtNo, std::make_pair( lhs->getName(), EntityType::VARIABLE ), rhs);
	}

    std::unordered_map<std::pair<std::string, EntityType>,
        std::pair<std::string, std::shared_ptr<RelFactorNode>>,
        PairHash> getAllPatternEntries() override {
        return patternMap;
    }

    bool isEmpty() override {
        return patternMap.empty();
    }

    bool stmtNoIsPresent(std::pair<std::string, EntityType> stmtNo) override {
        return patternMap.find(stmtNo) != patternMap.end();
    }

    bool exprNodeIsPresent(std::shared_ptr<RelFactorNode> exprNode) override {
        for (auto& pattern : patternMap) {
            if (pattern.second.second == exprNode) {
                return true;
            }
        }
        return false;
    }
};

#endif // PATTERN_MANAGER_H
