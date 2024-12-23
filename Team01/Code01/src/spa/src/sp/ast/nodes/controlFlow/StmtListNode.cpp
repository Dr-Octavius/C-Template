#include "StmtListNode.h"

StmtListNode::StmtListNode(int lineNum) : ASTNode(lineNum, EntityType::STMT_LIST) {}

void StmtListNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

// Adjust the implementation to accept shared_ptr
void StmtListNode::addStatement(std::shared_ptr<ASTNode> statement) {
    statements.push_back(statement);
}

// Adjust the return type to match the updated vector type
std::vector<std::shared_ptr<ASTNode>>& StmtListNode::getStatements() {
    return statements;
}
