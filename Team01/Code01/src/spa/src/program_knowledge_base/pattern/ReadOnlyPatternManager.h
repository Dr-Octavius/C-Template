#ifndef READ_ONLY_PATTERN_MANAGER_H
#define READ_ONLY_PATTERN_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include "common/Utils.h"

class ReadOnlyPatternManager {
public:
    virtual std::unordered_map<std::pair<std::string, EntityType>,
        std::pair<std::string, std::shared_ptr<RelFactorNode>>,
        PairHash> getAllPatternEntries() = 0;
    virtual bool isEmpty() = 0;
    virtual bool stmtNoIsPresent(std::pair<std::string, EntityType> stmtNo) = 0;
    virtual bool exprNodeIsPresent(std::shared_ptr<RelFactorNode> exprNode) = 0;
};

#endif // READ_ONLY_PATTERN_MANAGER_H
