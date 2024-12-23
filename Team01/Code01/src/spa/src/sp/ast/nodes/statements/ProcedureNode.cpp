#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(const std::string& name, std::shared_ptr<StmtListNode> body, int lineNum)
        : ASTNode(lineNum, EntityType::PROCEDURE), name(name), body(body) {}

void ProcedureNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& ProcedureNode::getName() const {
    return name;
}

std::shared_ptr<StmtListNode>& ProcedureNode::getBody() {
    return body;
}