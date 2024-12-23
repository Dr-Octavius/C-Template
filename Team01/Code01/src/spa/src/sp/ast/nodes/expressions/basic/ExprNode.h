#ifndef SPA_EXPRNODE_H
#define SPA_EXPRNODE_H

#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNodeUtil.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class ExprNode : public RelFactorNode {
private:
    std::shared_ptr<RelFactorNode> left;
    std::shared_ptr<RelFactorNode> right;
    ExprOperator op;

public:
    ExprNode(std::shared_ptr<RelFactorNode> leftChild, std::shared_ptr<RelFactorNode> rightChild, ExprOperator op, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<RelFactorNode>& getLeftChild();
    std::shared_ptr<RelFactorNode>& getRightChild();
    ExprOperator getOperator() const;
    std::vector<std::string> generateSubtreeStrings() override;
    std::string generateExpressionString();
};


#endif //SPA_EXPRNODE_H
