#include "ModifiesVisitor.h"
#include <vector>
#include <string>
#include "sp/ast/nodes/statements/ReadNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "common/Utils.h"

ModifiesVisitor::ModifiesVisitor() {}

void ModifiesVisitor::visit(ASTNode& node) {
    // do nothing
    return;
}

void ModifiesVisitor::visit(ProgramNode& node) {
    for (const auto& proc : node.getProcedures()) {
        proc->accept(*this);
    }
}

void ModifiesVisitor::visit(ProcedureNode& node) {
    nestedLines.insert(node.getName());
    const std::shared_ptr<StmtListNode>& procStatementList = node.getBody();
    procStatementList->accept(*this);
    nestedLines.erase(node.getName());
}

void ModifiesVisitor::visit(CallNode& node) {
    std::pair<std::string, EntityType> callKey = {std::to_string(node.getLineNumber()), EntityType::CALL};

    const std::shared_ptr<ProcedureNode>& proc = procToNodeMap[node.getName()];
    std::pair<std::string, EntityType> procCalledKey = {node.getName(), EntityType::PROCEDURE};

    if (modifies.find(procCalledKey) != modifies.end()) {
        auto& procModifiesSet = modifies[procCalledKey];
        modifies[callKey] = procModifiesSet;
        for (const std::string& line : nestedLines) {
            if (!line.empty() && std::isalpha(static_cast<unsigned char>(line[0]))) {
                std::pair<std::string, EntityType> procedurePair = {line, EntityType::PROCEDURE};
                modifies[procedurePair].insert(procModifiesSet.begin(), procModifiesSet.end());
            } else {
                EntityType nestedType = stmtNumberMap[line];
                std::pair<std::string, EntityType> ancestorKey = {line, nestedType};
                modifies[ancestorKey].insert(procModifiesSet.begin(), procModifiesSet.end());
            }
        }
    } else {
        nestedLines.insert(std::to_string(node.getLineNumber()));
        proc->accept(*this);
        nestedLines.erase(std::to_string(node.getLineNumber()));
    }
    return;
}

void ModifiesVisitor::visit(ReadNode& node) {
    nestedLines.insert(std::to_string(node.getLineNumber()));
    const std::shared_ptr<VariableNode>& varNode = node.getVariable();
    varNode->accept(*this);
    nestedLines.erase(std::to_string(node.getLineNumber()));
}

void ModifiesVisitor::visit(PrintNode& node) {
    // do nothing
}

void ModifiesVisitor::visit(AssignNode& node) {
    nestedLines.insert(std::to_string(node.getLineNumber()));
    const std::shared_ptr<VariableNode>& varNode = node.getLeftChild();
    varNode->accept(*this);
    nestedLines.erase(std::to_string(node.getLineNumber()));
}

void ModifiesVisitor::visit(ExprNode& node) {
    // do nothing
    return;
}

void ModifiesVisitor::visit(RelExprNode& node) {
    // do nothing
    return;
}

void ModifiesVisitor::visit(CondBinaryExprNode& node) {
    // do nothing
    return;
}

void ModifiesVisitor::visit(CondUnaryExprNode& node) {
    // do nothing
    return;
}

void ModifiesVisitor::visit(ConstantNode& node) {
    // do nothing
    return;
}

void ModifiesVisitor::visit(VariableNode& node) {
    std::string name = node.getName();
    std::pair<std::string, EntityType> value = {name, EntityType::VARIABLE};
    for (std::string line : nestedLines) {
        if (!line.empty() && std::isalpha(static_cast<unsigned char>(line[0]))) {
            std::pair<std::string, EntityType> procedurePair = {line, EntityType::PROCEDURE};
            modifies[procedurePair].insert(value);
        } else {
            std::pair<std::string, EntityType> key = {line, stmtNumberMap[line]};
            modifies[key].insert(value);
        }
    }
}

void ModifiesVisitor::visit(IfNode& node) {
    nestedLines.insert(std::to_string(node.getLineNumber()));

    const std::shared_ptr<StmtListNode>& thenStatementList = node.getThenBranch();
    thenStatementList->accept(*this);

    const std::shared_ptr<StmtListNode>& elseStatementList = node.getElseBranch();
    elseStatementList->accept(*this);

    nestedLines.erase(std::to_string(node.getLineNumber()));
}

void ModifiesVisitor::visit(WhileNode& node) {
    nestedLines.insert(std::to_string(node.getLineNumber()));
    const std::shared_ptr<StmtListNode>& whileStatementList = node.getBody();
    whileStatementList->accept(*this);
    nestedLines.erase(std::to_string(node.getLineNumber()));
}

void ModifiesVisitor::visit(StmtListNode& node) {
    const std::vector<std::shared_ptr<ASTNode>>& statements = node.getStatements();
    for (const auto& curr : statements) {
        curr->accept(*this);
    }
}

const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>&
    ModifiesVisitor::getModifies() const {
    return modifies;
}

void ModifiesVisitor::setStmtNumberMap(const std::map<std::string, EntityType>& map) {
    this->stmtNumberMap = map;
}

void ModifiesVisitor::setProcToNodeMap(const std::map<std::string, std::shared_ptr<ProcedureNode>>& map) {
    this->procToNodeMap = map;
}
