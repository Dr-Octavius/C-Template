#include "ExprNode.h"

ExprNode::ExprNode(std::shared_ptr<RelFactorNode> leftChild, std::shared_ptr<RelFactorNode> rightChild, ExprOperator op, int lineNum)
        : RelFactorNode(lineNum, EntityType::EXPR), left(leftChild), right(rightChild), op(op) {}

void ExprNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

std::shared_ptr<RelFactorNode>& ExprNode::getLeftChild() {
    return left;
}

std::shared_ptr<RelFactorNode>& ExprNode::getRightChild() {
    return right;
}

ExprOperator ExprNode::getOperator() const {
    return op;
}

std::string ExprNode::generateExpressionString() {
    return left->generateExpressionString() + ExprOperatorUtils::toString(op) + right->generateExpressionString();
}

std::vector<std::string> ExprNode::generateSubtreeStrings() {
    std::vector<std::string> result;

    auto leftSubtrees = left->generateSubtreeStrings();
    auto rightSubtrees = right->generateSubtreeStrings();

    result.push_back(generateExpressionString());

    result.insert(result.end(), leftSubtrees.begin(), leftSubtrees.end());
    result.insert(result.end(), rightSubtrees.begin(), rightSubtrees.end());

    return result;
}