#include "WhileNode.h"

WhileNode::WhileNode(std::shared_ptr<CondExprNode> condition, std::shared_ptr<StmtListNode> body, int lineNum)
        : ASTNode(lineNum, EntityType::WHILE), condition(condition), body(body) {}

void WhileNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<CondExprNode>& WhileNode::getCondition() {
    return condition;
}

std::shared_ptr<StmtListNode>& WhileNode::getBody() {
    return body;
}
