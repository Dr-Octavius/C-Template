#include "ReadNode.h"

ReadNode::ReadNode(std::shared_ptr<VariableNode> variable, int lineNum)
        : ASTNode(lineNum, EntityType::READ), variable(variable) {}

void ReadNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<VariableNode>& ReadNode::getVariable() {
    return variable;
}