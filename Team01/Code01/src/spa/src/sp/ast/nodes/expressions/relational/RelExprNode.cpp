#include "RelExprNode.h"

RelExprNode::RelExprNode(std::shared_ptr<RelFactorNode> leftChild, std::shared_ptr<RelFactorNode> rightChild, RelExprOperator op, int lineNum)
        : CondExprNode(lineNum, EntityType::REL_EXPR), left(leftChild), right(rightChild), op(op) {}

void RelExprNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<RelFactorNode> RelExprNode::getLeftChild() {
    return left;
}

std::shared_ptr<RelFactorNode>& RelExprNode::getRightChild() {
    return right;
}

RelExprOperator RelExprNode::getOperator() {
    return op;
}