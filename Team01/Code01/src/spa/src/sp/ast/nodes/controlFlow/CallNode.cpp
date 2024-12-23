#include "CallNode.h"

CallNode::CallNode(const std::string& name, int lineNum)
        : ASTNode(lineNum, EntityType::CALL), procName(name) {}

void CallNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::string& CallNode::getName() {
    return procName;
}