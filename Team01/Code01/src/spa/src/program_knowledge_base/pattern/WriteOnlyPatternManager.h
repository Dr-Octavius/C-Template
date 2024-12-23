#ifndef WRITE_ONLY_PATTERN_MANAGER_H
#define WRITE_ONLY_PATTERN_MANAGER_H

#include <memory>
#include <string>
#include "sp/ast/nodes/statements/AssignNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "common/Utils.h"

class WriteOnlyPatternManager {
public:
    virtual bool insertLhsPattern(const std::pair<std::string, EntityType>& stmtNo, const std::pair<std::string, EntityType>& lhs, const std::shared_ptr<RelFactorNode>& rhs) = 0;

    virtual bool insertPattern(const std::pair<std::string, EntityType>& stmtNo, const std::shared_ptr<AssignNode>& assignNode) = 0;
};

#endif // WRITE_ONLY_PATTERN_MANAGER_H
