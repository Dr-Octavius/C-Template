#ifndef SPA_RELOPNODE_H
#define SPA_RELOPNODE_H

#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include "sp/ast/nodes/expressions/CondExprNode.h"
#include "sp/ast/nodes/expressions/relational/RelExprUtil.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>
#include <string>

class RelExprNode : public CondExprNode {
private:
    std::shared_ptr<RelFactorNode> left;
    std::shared_ptr<RelFactorNode> right;
    RelExprOperator op;

public:
    RelExprNode(std::shared_ptr<RelFactorNode> leftChild, std::shared_ptr<RelFactorNode> rightChild, RelExprOperator op, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<RelFactorNode> getLeftChild();
    std::shared_ptr<RelFactorNode>& getRightChild();
    RelExprOperator getOperator();
};


#endif //SPA_RELOPNODE_H
