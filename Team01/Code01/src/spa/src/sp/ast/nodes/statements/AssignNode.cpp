#include "AssignNode.h"

AssignNode::AssignNode(std::shared_ptr<VariableNode> leftChild, std::shared_ptr<RelFactorNode> rightChild, int lineNum)
        : ASTNode(lineNum,EntityType::ASSIGN), leftChild(leftChild), rightChild(rightChild) {}

void AssignNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<VariableNode>& AssignNode::getLeftChild() {
    return leftChild;
}

std::shared_ptr<RelFactorNode>& AssignNode::getRightChild() {
    return rightChild;
}

int AssignNode::getLineNumber() const {
    return lineNumber;
}
