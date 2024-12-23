#include "ConstantNode.h"

ConstantNode::ConstantNode(int value, int lineNum)
        : RelFactorNode(lineNum, EntityType::CONSTANT), value(value) {}

void ConstantNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

int ConstantNode::getValue() const {
    return value;
}

std::vector<std::string> ConstantNode::generateSubtreeStrings() {
    return {std::to_string(value)};
}

std::string ConstantNode::generateExpressionString() {
    return std::to_string(value);
}
