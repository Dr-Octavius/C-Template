#include "CondUnaryExprNode.h"

CondUnaryExprNode::CondUnaryExprNode(std::shared_ptr<CondExprNode> operand, CondExprOperator op, int lineNum)
        : CondExprNode(lineNum, EntityType::COND_EXPR), operand(operand), op(op) {}

void CondUnaryExprNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<CondExprNode>& CondUnaryExprNode::getOperand() {
    return operand;
}

CondExprOperator CondUnaryExprNode::getOperator() {
    return op;
}
