#include "ParentVisitor.h"

#include <string>
#include <vector>
#include <memory>

ParentVisitor::ParentVisitor() {}

void ParentVisitor::addTransitiveParentChild(const std::string& parentLine, const std::shared_ptr<ASTNode>& child) {
    std::string childLineStr = std::to_string(child->getLineNumber());
    EntityType childType = stmtNumberMap[childLineStr];
    std::pair<std::string, EntityType> childPair = {childLineStr, childType};
    EntityType parentType = stmtNumberMap[parentLine];
    std::pair<std::string, EntityType> parentPair = {parentLine, parentType};

    parents[childPair].insert(parentPair);

    for (auto& ancestorLineStr : ancestorStack) {
        EntityType ancestorType = stmtNumberMap[ancestorLineStr];
        std::pair<std::string, EntityType> ancestorKey = {ancestorLineStr, ancestorType};
        parentT[ancestorKey].insert(childPair);
    }
}

void ParentVisitor::visit(ASTNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(ProgramNode& node) {
    for (const auto& proc : node.getProcedures()) {
        proc->accept(*this);
    }
}

void ParentVisitor::visit(ProcedureNode& node) {
    const std::shared_ptr<StmtListNode>& procStatementList = node.getBody();
    procStatementList->accept(*this);
}

void ParentVisitor::visit(CallNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(ReadNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(PrintNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(AssignNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(ExprNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(RelExprNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(CondBinaryExprNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(CondUnaryExprNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(ConstantNode& node) {
    // do nothing
    return;
}

void ParentVisitor::visit(VariableNode& node) {
    // do nothing
    return;
}

void ParentVisitor::markParentChild(const std::shared_ptr<StmtListNode>& branch, const std::string& parentLineStr) {
    for (const auto& stmt : branch->getStatements()) {
        addTransitiveParentChild(parentLineStr, stmt);
        stmt->accept(*this);
    }
}

void ParentVisitor::visit(IfNode& node) {
    std::string nodeLineStr = std::to_string(node.getLineNumber());
    ancestorStack.push_back(nodeLineStr);
    markParentChild(node.getThenBranch(), nodeLineStr);
    if (node.getElseBranch()) {
        markParentChild(node.getElseBranch(), nodeLineStr);
    }
    ancestorStack.pop_back();
}

void ParentVisitor::visit(WhileNode& node) {
    std::string whileLineStr = std::to_string(node.getLineNumber());
    ancestorStack.push_back(whileLineStr);
    markParentChild(node.getBody(), whileLineStr);
    ancestorStack.pop_back();
}

void ParentVisitor::visit(StmtListNode& node) {
    const std::vector<std::shared_ptr<ASTNode>>& statements = node.getStatements();
    for (const auto& stmt : statements) {
        stmt->accept(*this);
    }
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& ParentVisitor::getParents() const {
    return parents;
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& ParentVisitor::getParentT() const {
    return parentT;
}

void ParentVisitor::setStmtNumberMap(const std::map<std::string, EntityType>& map) {
    this->stmtNumberMap = map;
}