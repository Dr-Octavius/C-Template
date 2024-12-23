#include "IfNode.h"

IfNode::IfNode(std::shared_ptr<CondExprNode> condition, std::shared_ptr<StmtListNode> thenBranch, std::shared_ptr<StmtListNode> elseBranch, int lineNum)
        : ASTNode(lineNum,EntityType::IF), condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

void IfNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<CondExprNode>& IfNode::getCondition() {
    return condition;
}

std::shared_ptr<StmtListNode>& IfNode::getThenBranch() {
    return thenBranch;
}

std::shared_ptr<StmtListNode>& IfNode::getElseBranch() {
    return elseBranch;
}
