#include "FollowsVisitor.h"
#include <vector>
#include <string>

FollowsVisitor::FollowsVisitor() {}

void FollowsVisitor::visit(ASTNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(ProgramNode& node) {
    for (const auto& proc : node.getProcedures()) {
        proc->accept(*this);
    }
}

void FollowsVisitor::visit(ProcedureNode& node) {
    const std::shared_ptr<StmtListNode>& procStatementList = node.getBody();
    procStatementList->accept(*this);
}

void FollowsVisitor::visit(CallNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(ReadNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(PrintNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(AssignNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(ExprNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(RelExprNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(CondBinaryExprNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(CondUnaryExprNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(ConstantNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(VariableNode& node) {
    // do nothing
    return;
}

void FollowsVisitor::visit(IfNode& node) {
    const std::shared_ptr<StmtListNode>& thenStatementList = node.getThenBranch();
    thenStatementList->accept(*this);

    const std::shared_ptr<StmtListNode>& elseStatementList = node.getElseBranch();
    elseStatementList->accept(*this);
}

void FollowsVisitor::visit(WhileNode& node) {
    const std::shared_ptr<StmtListNode>& whileStatementList = node.getBody();
    whileStatementList->accept(*this);
}

void FollowsVisitor::visit(StmtListNode& node) {
    const std::vector<std::shared_ptr<ASTNode>>& statements = node.getStatements();
    for (int i = 0; i < statements.size() - 1; ++i) {
        std::string currentStmtNum = std::to_string(statements[i]->getLineNumber());
        std::string nextStmtNum = std::to_string(statements[i + 1]->getLineNumber());
        EntityType currentStmtType = stmtNumberMap[currentStmtNum];
        EntityType nextStmtType = stmtNumberMap[nextStmtNum];

        std::pair<std::string, EntityType> currentStmtPair = {currentStmtNum, currentStmtType};
        std::pair<std::string, EntityType> nextStmtPair = {nextStmtNum, nextStmtType};
        follows[currentStmtPair].insert(nextStmtPair);

        for (int j = i + 1; j < statements.size(); ++j) {
            std::string followingStmtNum = std::to_string(statements[j]->getLineNumber());
            EntityType followingStmtType = stmtNumberMap[followingStmtNum];
            std::pair<std::string, EntityType> followingStmtPair = {followingStmtNum, followingStmtType};
            followsT[currentStmtPair].insert(followingStmtPair);
        }
    }

    for (const auto& stmt : statements) {
        stmt->accept(*this);
    }
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& FollowsVisitor::getFollows() const {
    return follows;
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& FollowsVisitor::getFollowsT() const {
    return followsT;
}

void FollowsVisitor::setStmtNumberMap(const std::map<std::string, EntityType>& map) {
    this->stmtNumberMap = map;
}
