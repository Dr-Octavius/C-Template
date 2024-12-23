#include "CallsVisitor.h"
#include <vector>
#include <string>

CallsVisitor::CallsVisitor() {}

void CallsVisitor::visit(ASTNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(ProgramNode& node) {
    for (const auto& proc : node.getProcedures()) {
        proc->accept(*this);
    }
}

void CallsVisitor::visit(ProcedureNode& node) {
    currentProcedure = node.getName();
    const std::shared_ptr<StmtListNode>& procStatementList = node.getBody();
    procStatementList->accept(*this);
}

void CallsVisitor::visit(CallNode& node) {
    std::pair<std::string, EntityType> caller = {currentProcedure, EntityType::PROCEDURE};
    std::pair<std::string, EntityType> callee = {node.getName(), EntityType::PROCEDURE};
    calls[caller].insert(callee);
}

void CallsVisitor::visit(ReadNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(PrintNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(AssignNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(ExprNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(RelExprNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(CondBinaryExprNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(CondUnaryExprNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(ConstantNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(VariableNode& node) {
    // do nothing
    return;
}

void CallsVisitor::visit(IfNode& node) {
    const std::shared_ptr<StmtListNode>& thenStatementList = node.getThenBranch();
    thenStatementList->accept(*this);

    const std::shared_ptr<StmtListNode>& elseStatementList = node.getElseBranch();
    elseStatementList->accept(*this);
}

void CallsVisitor::visit(WhileNode& node) {
    const std::shared_ptr<StmtListNode>& whileStatementList = node.getBody();
    whileStatementList->accept(*this);
}

void CallsVisitor::visit(StmtListNode& node) {
    const std::vector<std::shared_ptr<ASTNode>>& statements = node.getStatements();
    for (const auto& stmt : statements) {
        stmt->accept(*this);
    }
}

void CallsVisitor::populateCallsT() {
    for (const auto& call : calls) {
        const auto& caller = call.first;
        auto& reachableProcedures = callsT[caller];

        std::function<void(const std::pair<std::string, EntityType>&)> explore;
        explore = [&](const std::pair<std::string, EntityType>& currentProc) {
            if (calls.find(currentProc) != calls.end()) {
                for (const auto& callee : calls.at(currentProc)) {
                    if (reachableProcedures.insert(callee).second) {
                        explore(callee);
                    }
                }
            }
        };
        explore(caller);
    }
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& CallsVisitor::getCalls() const {
    return calls;
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& CallsVisitor::getCallsT() const {
    return callsT;
}
