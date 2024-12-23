#include "CondBinaryExprNode.h"

CondBinaryExprNode::CondBinaryExprNode(std::shared_ptr<CondExprNode> leftChild, std::shared_ptr<CondExprNode> rightChild, CondExprOperator op, int lineNum)
        : CondExprNode(lineNum, EntityType::COND_EXPR), left(leftChild), right(rightChild), op(op) {}

void CondBinaryExprNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<CondExprNode>& CondBinaryExprNode::getLeftChild() {
    return left;
}

std::shared_ptr<CondExprNode>& CondBinaryExprNode::getRightChild() {
    return right;
}

CondExprOperator CondBinaryExprNode::getOperator() const {
    return op;
}
