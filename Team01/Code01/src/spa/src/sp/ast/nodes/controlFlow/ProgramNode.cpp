#include "ProgramNode.h"

ProgramNode::ProgramNode(int lineNum)
        : ASTNode(lineNum, EntityType::PROGRAM) {}

void ProgramNode::accept(RelationshipVisitor& visitor) {
    visitor.visit(*this);
}

// Adjust the implementation to accept shared_ptr
void ProgramNode::addProcedure(std::shared_ptr<ProcedureNode> procedure) {
    procedures.push_back(procedure);
}

// Adjust the return type to match the updated vector type
std::vector<std::shared_ptr<ProcedureNode>>& ProgramNode::getProcedures() {
    return procedures;
}