#include "PatternVisitor.h"
#include <vector>
#include <string>

PatternVisitor::PatternVisitor() {}

void PatternVisitor::visit(ASTNode& node) {
    // do nothing
    return;
}

void PatternVisitor::visit(ProgramNode& node) {
    for (const auto& proc : node.getProcedures()) {
        proc->accept(*this);
    }
}

void PatternVisitor::visit(ProcedureNode& node) {
    const std::shared_ptr<StmtListNode>& procStatementList = node.getBody();
    procStatementList->accept(*this);
}

void PatternVisitor::visit(CallNode& node) {
    // do nothing
    return;
}

void PatternVisitor::visit(ReadNode& node) {
    // do nothing
    return;
}

void PatternVisitor::visit(PrintNode& node) {
    // do nothing
    return;
}

void PatternVisitor::visit(AssignNode& node) {
    std::pair<std::string, EntityType> assignPair = {std::to_string(node.getLineNumber()), EntityType::ASSIGN};
    assignPatternData[assignPair] = std::make_shared<AssignNode>(node);
    return;
}

void PatternVisitor::visit(ExprNode& node) {
    const std::shared_ptr<RelFactorNode> left = node.getLeftChild();
    const std::shared_ptr<RelFactorNode> right = node.getRightChild();
    if (left) {
        left->accept(*this);
    }
    if (right) {
        right->accept(*this);
    }
}

void PatternVisitor::visit(RelExprNode& node) {
    const std::shared_ptr<RelFactorNode> left = node.getLeftChild();
    const std::shared_ptr<RelFactorNode> right = node.getRightChild();
    if (left) {
        left->accept(*this);
    }
    if (right) {
        right->accept(*this);
    }
}

void PatternVisitor::visit(CondBinaryExprNode& node) {
    const std::shared_ptr<CondExprNode> left = node.getLeftChild();
    const std::shared_ptr<CondExprNode> right = node.getRightChild();
    if (left) {
        left->accept(*this);
    }
    if (right) {
        right->accept(*this);
    }
}

void PatternVisitor::visit(CondUnaryExprNode& node) {
    const std::shared_ptr<CondExprNode> expr = node.getOperand();
    if (expr) {
        expr->accept(*this);
    }
}

void PatternVisitor::visit(ConstantNode& node) {
    // do nothing
    return;
}

void PatternVisitor::visit(VariableNode& node) {
    EntityType statementType = stmtNumberMap[std::to_string(node.getLineNumber())];
    std::pair<std::string, EntityType> keyPair = {std::to_string(node.getLineNumber()), statementType};
    std::pair<std::string, EntityType> valuePair = {node.getName(), EntityType::VARIABLE};

    if (statementType == EntityType::IF) {
        ifPatternData[keyPair].insert(valuePair);
    } else if (statementType == EntityType::WHILE) {
        whilePatternData[keyPair].insert(valuePair);
    }
}

void PatternVisitor::visit(IfNode& node) {
    const std::shared_ptr<CondExprNode> cond = node.getCondition();
    cond->accept(*this);

    const std::shared_ptr<StmtListNode>& thenStatementList = node.getThenBranch();
    thenStatementList->accept(*this);

    const std::shared_ptr<StmtListNode>& elseStatementList = node.getElseBranch();
    elseStatementList->accept(*this);
}

void PatternVisitor::visit(WhileNode& node) {
    const std::shared_ptr<CondExprNode> cond = node.getCondition();
    cond->accept(*this);

    const std::shared_ptr<StmtListNode>& whileStatementList = node.getBody();
    whileStatementList->accept(*this);
}

void PatternVisitor::visit(StmtListNode& node) {
    const std::vector<std::shared_ptr<ASTNode>>& statements = node.getStatements();
    for (const auto& curr : statements) {
        curr->accept(*this);
    }
}

const std::map<std::pair<std::string, EntityType>, std::shared_ptr<AssignNode>>& PatternVisitor::getAssignPatternData() const {
    return assignPatternData;
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& PatternVisitor::getIfPatternData() const {
    return ifPatternData;
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& PatternVisitor::getWhilePatternData() const {
    return whilePatternData;
}

void PatternVisitor::setStmtNumberMap(const std::map<std::string, EntityType>& map) {
    this->stmtNumberMap = map;
}
