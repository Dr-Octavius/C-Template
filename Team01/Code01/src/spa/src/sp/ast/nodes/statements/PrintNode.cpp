#include "PrintNode.h"

PrintNode::PrintNode(std::shared_ptr<VariableNode> variable, int lineNum)
        : ASTNode(lineNum, EntityType::PRINT), variable(variable) {}

void PrintNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<VariableNode>& PrintNode::getVariable() {
    return variable;
}