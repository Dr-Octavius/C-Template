#include "EntityVisitor.h"
#include <vector>
#include <string>

EntityVisitor::EntityVisitor() {}

void EntityVisitor::addStmtForEntity(EntityType type, const std::string& item) {
    entityToStatementsMap[type].insert(item);
}

void EntityVisitor::addToStmtNumMap(const std::string& lineNum, EntityType type) {
    stmtNumberMap[lineNum] = type;
}

void EntityVisitor::visit(ASTNode& node) {
    // do nothing
    return;
}

void EntityVisitor::visit(ProgramNode& node) {
    for (const auto& proc : node.getProcedures()) {
        proc->accept(*this);
    }
}

void EntityVisitor::visit(ProcedureNode& node) {
    entityToStatementsMap[EntityType::PROCEDURE].insert(node.getName());
    procToNodeMap[node.getName()] = std::make_shared<ProcedureNode>(node);

    const std::shared_ptr<StmtListNode>& procStatementList = node.getBody();
    procStatementList->accept(*this);
}

void EntityVisitor::visit(CallNode& node) {
    int lineNumber = node.getLineNumber();
    std::string lineNumberStr = std::to_string(lineNumber);

    std::string procName = node.getName();
    std::pair<std::string, EntityType> keyPair = {lineNumberStr, EntityType::CALL};
    std::pair<std::string, EntityType> valuePair = {procName, EntityType::PROCEDURE};
    stmtSpecificEntitiesMap[keyPair].insert(valuePair);

    addStmtForEntity(EntityType::CALL, lineNumberStr);
    addStmtForEntity(EntityType::STMT, lineNumberStr);
    addToStmtNumMap(lineNumberStr, EntityType::CALL);

    return;
}

void EntityVisitor::visit(ReadNode& node) {
    int lineNumber = node.getLineNumber();
    std::string lineNumberStr = std::to_string(lineNumber);

    const std::shared_ptr<VariableNode>& variableNode = node.getVariable();
    std::string variableName = variableNode->getName();
    std::pair<std::string, EntityType> keyPair = {lineNumberStr, EntityType::READ};
    std::pair<std::string, EntityType> valuePair = {variableName, EntityType::VARIABLE};
    stmtSpecificEntitiesMap[keyPair].insert(valuePair);

    addStmtForEntity(EntityType::READ, lineNumberStr);
    addStmtForEntity(EntityType::STMT, lineNumberStr);
    addToStmtNumMap(lineNumberStr, EntityType::READ);
    node.getVariable()->accept(*this);

    return;
}

void EntityVisitor::visit(PrintNode& node) {
    int lineNumber = node.getLineNumber();
    std::string lineNumberStr = std::to_string(lineNumber);

    const std::shared_ptr<VariableNode>& variableNode = node.getVariable();
    std::string variableName = variableNode->getName();
    std::pair<std::string, EntityType> keyPair = {lineNumberStr, EntityType::PRINT};
    std::pair<std::string, EntityType> valuePair = {variableName, EntityType::VARIABLE};
    stmtSpecificEntitiesMap[keyPair].insert(valuePair);

    addStmtForEntity(EntityType::PRINT, lineNumberStr);
    addStmtForEntity(EntityType::STMT, lineNumberStr);
    addToStmtNumMap(lineNumberStr, EntityType::PRINT);
    node.getVariable()->accept(*this);

    return;
}

void EntityVisitor::visit(AssignNode& node) {
    int lineNumber = node.getLineNumber();
    std::string lineNumberStr = std::to_string(lineNumber);

    addStmtForEntity(EntityType::ASSIGN, lineNumberStr);
    addStmtForEntity(EntityType::STMT, lineNumberStr);
    addToStmtNumMap(lineNumberStr, EntityType::ASSIGN);
    node.getLeftChild()->accept(*this);
    node.getRightChild()->accept(*this);

    return;
}

void EntityVisitor::visit(ExprNode& node) {
    node.getLeftChild()->accept(*this);
    node.getRightChild()->accept(*this);
    return;
}

void EntityVisitor::visit(RelExprNode& node) {
    node.getLeftChild()->accept(*this);
    node.getRightChild()->accept(*this);
    return;
}

void EntityVisitor::visit(CondBinaryExprNode& node) {
    node.getLeftChild()->accept(*this);
    node.getRightChild()->accept(*this);
    return;
}

void EntityVisitor::visit(CondUnaryExprNode& node) {
    node.getOperand()->accept(*this);
    return;
}

void EntityVisitor::visit(ConstantNode& node) {
    addStmtForEntity(EntityType::CONSTANT, std::to_string(node.getValue()));
    return;
}

void EntityVisitor::visit(VariableNode& node) {
    addStmtForEntity(EntityType::VARIABLE, node.getName());
    return;
}

void EntityVisitor::visit(IfNode& node) {
    const std::shared_ptr<CondExprNode> cond = node.getCondition();
    cond->accept(*this);

    int lineNumber = node.getLineNumber();
    std::string lineNumberStr = std::to_string(lineNumber);

    addStmtForEntity(EntityType::IF, lineNumberStr);
    addStmtForEntity(EntityType::STMT, lineNumberStr);
    addToStmtNumMap(lineNumberStr, EntityType::IF);

    const std::shared_ptr<StmtListNode>& thenStatementList = node.getThenBranch();
    thenStatementList->accept(*this);

    const std::shared_ptr<StmtListNode>& elseStatementList = node.getElseBranch();
    elseStatementList->accept(*this);

    return;
}

void EntityVisitor::visit(WhileNode& node) {
    const std::shared_ptr<CondExprNode> cond = node.getCondition();
    cond->accept(*this);

    int lineNumber = node.getLineNumber();
    std::string lineNumberStr = std::to_string(lineNumber);

    addStmtForEntity(EntityType::WHILE, lineNumberStr);
    addStmtForEntity(EntityType::STMT, lineNumberStr);
    addToStmtNumMap(lineNumberStr, EntityType::WHILE);

    const std::shared_ptr<StmtListNode>& whileStatementList = node.getBody();
    whileStatementList->accept(*this);

    return;
}

void EntityVisitor::visit(StmtListNode& node) {
    const std::vector<std::shared_ptr<ASTNode>>& statements = node.getStatements();
    for (const auto& stmt : statements) {
        stmt->accept(*this);
    }
}

const std::map<std::string, EntityType>& EntityVisitor::getStmtNumberMap() const {
    return stmtNumberMap;
};

const  std::map<EntityType, std::unordered_set<std::string>>& EntityVisitor::getEntityToStatementsMap() const {
    return entityToStatementsMap;
};

const std::map<std::string, std::shared_ptr<ProcedureNode>>& EntityVisitor::getProcToNodeMap() const {
    return procToNodeMap;
};

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& EntityVisitor::getStmtSpecificEntitiesMap() const {
    return stmtSpecificEntitiesMap;
}
