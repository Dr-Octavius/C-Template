#include "VariableNode.h"

VariableNode::VariableNode(const std::string& name, int lineNum)
        : RelFactorNode(lineNum, EntityType::VARIABLE), name(name) {}

void VariableNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::string& VariableNode::getName() {
    return name;
}

std::vector<std::string> VariableNode::generateSubtreeStrings() {
    return {name};
}

std::string VariableNode::generateExpressionString() {
    return name;
}