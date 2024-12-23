#ifndef SPA_CONDBINARYEXPRNODE_H
#define SPA_CONDBINARYEXPRNODE_H

#include "sp/ast/nodes/expressions/CondExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondExprNodeUtil.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>
#include <string>

class CondBinaryExprNode : public CondExprNode {
private:
    std::shared_ptr<CondExprNode> left;
    std::shared_ptr<CondExprNode> right;
    CondExprOperator op;

public:
    CondBinaryExprNode(std::shared_ptr<CondExprNode> leftChild, std::shared_ptr<CondExprNode> rightChild, CondExprOperator op, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<CondExprNode>& getLeftChild();
    std::shared_ptr<CondExprNode>& getRightChild();
    CondExprOperator getOperator() const;
};


#endif //SPA_CONDBINARYEXPRNODE_H
