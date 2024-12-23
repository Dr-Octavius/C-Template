#ifndef SPA_MOCKPATTERNSFACTORY_H
#define SPA_MOCKPATTERNSFACTORY_H

#include "common/Utils.h"
#include <memory>
#include <map>
#include <iostream>
#include <utility>
#include <unordered_set>
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"

class MockPatternsFactory {
private:
    std::map<std::pair<std::string, EntityType>, std::shared_ptr<ExprNode>> patternsData;

public:
    MockPatternsFactory();

    const std::shared_ptr<ExprNode> createFirstExpression() const;
    const std::shared_ptr<ExprNode> createSecondExpression() const;
    const std::shared_ptr<ExprNode> createThirdExpression() const;
    const std::shared_ptr<ExprNode> createFourthExpression() const;
    const std::shared_ptr<ExprNode> createFifthExpression() const;

    void populatePatternsMap();
    const std::map<std::pair<std::string, EntityType>, std::shared_ptr<ExprNode>>& getPatternsData() const;
};


#endif //SPA_MOCKPATTERNSFACTORY_H
