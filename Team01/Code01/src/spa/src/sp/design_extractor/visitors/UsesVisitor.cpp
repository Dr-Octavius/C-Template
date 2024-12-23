#include "UsesVisitor.h"
#include <vector>
#include <string>
#include "sp/ast/nodes/statements/PrintNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"

UsesVisitor::UsesVisitor() {}

void UsesVisitor::visit(ASTNode& node) {
    // do nothing
    return;
}

void UsesVisitor::visit(ProgramNode& node) {
    for (const auto& proc : node.getProcedures()) {
        proc->accept(*this);
    }
}

void UsesVisitor::visit(ProcedureNode& node) {
    nestedLines.insert(node.getName());
    const std::shared_ptr<StmtListNode>& procStatementList = node.getBody();
    procStatementList->accept(*this);
    nestedLines.erase(node.getName());
}

void UsesVisitor::visit(CallNode& node) {
    std::pair<std::string, EntityType> callKey = {std::to_string(node.getLineNumber()), EntityType::CALL};

    const std::shared_ptr<ProcedureNode>& proc = procToNodeMap[node.getName()];
    std::pair<std::string, EntityType> procCalledKey = {node.getName(), EntityType::PROCEDURE};

    if (uses.find(procCalledKey) != uses.end()) {
        auto& procUsesSet = uses[procCalledKey];
        uses[callKey] = procUsesSet;
        for (const std::string& line : nestedLines) {
            if (!line.empty() && std::isalpha(static_cast<unsigned char>(line[0]))) {
                std::pair<std::string, EntityType> procedurePair = {line, EntityType::PROCEDURE};
                uses[procedurePair].insert(procUsesSet.begin(), procUsesSet.end());
            } else {
                EntityType nestedType = stmtNumberMap[line];
                std::pair<std::string, EntityType> ancestorKey = {line, nestedType};
                uses[ancestorKey].insert(procUsesSet.begin(), procUsesSet.end());
            }
        }
    } else {
        nestedLines.insert(std::to_string(node.getLineNumber()));
        proc->accept(*this);
        nestedLines.erase(std::to_string(node.getLineNumber()));
    }
    return;
}

void UsesVisitor::visit(ReadNode& node) {
    // do nothing
    return;
}

void UsesVisitor::visit(PrintNode& node) {
    std::string nodeName = node.getVariable()->getName();
    EntityType nodeType = EntityType::VARIABLE;
    std::pair<std::string, EntityType> nodePair = {nodeName, nodeType};

    std::string usingLineNum = std::to_string(node.getLineNumber());
    EntityType usingType = stmtNumberMap[usingLineNum];
    std::pair<std::string, EntityType> usesPair = {usingLineNum, usingType};

    uses[usesPair].insert(nodePair);

    for (const std::string& line : nestedLines) {
        if (!line.empty() && std::isalpha(static_cast<unsigned char>(line[0]))) {
            std::pair<std::string, EntityType> procedurePair = {line, EntityType::PROCEDURE};
            uses[procedurePair].insert(nodePair);
        } else {
            EntityType nestedType = stmtNumberMap[line];
            std::pair<std::string, EntityType> ancestorKey = {line, nestedType};
            uses[ancestorKey].insert(nodePair);
        }
    }
}

void UsesVisitor::visit(AssignNode& node) {
    nestedLines.insert(std::to_string(node.getLineNumber()));

    auto rightChild = node.getRightChild();
    if (rightChild) {
        rightChild->accept(*this);
    }

    nestedLines.erase(std::to_string(node.getLineNumber()));
}

void UsesVisitor::visit(ExprNode& node) {
    const std::shared_ptr<RelFactorNode> left = node.getLeftChild();
    const std::shared_ptr<RelFactorNode> right = node.getRightChild();
    if (left) {
        left->accept(*this);
    }
    if (right) {
        right->accept(*this);
    }
}

void UsesVisitor::visit(RelExprNode& node) {
    const std::shared_ptr<RelFactorNode> left = node.getLeftChild();
    const std::shared_ptr<RelFactorNode> right = node.getRightChild();
    if (left) {
        left->accept(*this);
    }
    if (right) {
        right->accept(*this);
    }
}

void UsesVisitor::visit(CondBinaryExprNode& node) {
    const std::shared_ptr<CondExprNode> left = node.getLeftChild();
    const std::shared_ptr<CondExprNode> right = node.getRightChild();
    if (left) {
        left->accept(*this);
    }
    if (right) {
        right->accept(*this);
    }
    return;
}

void UsesVisitor::visit(CondUnaryExprNode& node) {
    const std::shared_ptr<CondExprNode> expr = node.getOperand();
    if (expr) {
        expr->accept(*this);
    }
    return;
}

void UsesVisitor::visit(ConstantNode& node) {
    // do nothing
}

void UsesVisitor::visit(VariableNode& node) {
    std::pair<std::string, EntityType> toInsert = {node.getName(), EntityType::VARIABLE};
    for (const std::string& line: nestedLines) {
        if (!line.empty() && std::isalpha(static_cast<unsigned char>(line[0]))) {
            std::pair<std::string, EntityType> procedurePair = {line, EntityType::PROCEDURE};
            uses[procedurePair].insert(toInsert);
        } else {
            std::pair<std::string, EntityType> ancestorKey = {line, stmtNumberMap[line]};
            uses[ancestorKey].insert(toInsert);
        }
    }
}

void UsesVisitor::visit(IfNode& node) {
    nestedLines.insert(std::to_string(node.getLineNumber()));
    const std::shared_ptr<CondExprNode> cond = node.getCondition();
    cond->accept(*this);

    const std::shared_ptr<StmtListNode>& thenStatementList = node.getThenBranch();
    thenStatementList->accept(*this);

    const std::shared_ptr<StmtListNode>& elseStatementList = node.getElseBranch();
    elseStatementList->accept(*this);

    nestedLines.erase(std::to_string(node.getLineNumber()));
}

void UsesVisitor::visit(WhileNode& node) {
    nestedLines.insert(std::to_string(node.getLineNumber()));

    const std::shared_ptr<CondExprNode> cond = node.getCondition();
    cond->accept(*this);

    const std::shared_ptr<StmtListNode>& whileStatementList = node.getBody();
    whileStatementList->accept(*this);

    nestedLines.erase(std::to_string(node.getLineNumber()));
}

void UsesVisitor::visit(StmtListNode& node) {
    const std::vector<std::shared_ptr<ASTNode>>& statements = node.getStatements();
    for (const auto& curr : statements) {
        curr->accept(*this);
    }
}

const std::map<std::pair<std::string, EntityType>,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& UsesVisitor::getUses() const {
    return uses;
}

void UsesVisitor::setStmtNumberMap(const std::map<std::string, EntityType>& map) {
    this->stmtNumberMap = map;
}

void UsesVisitor::setProcToNodeMap(const std::map<std::string, std::shared_ptr<ProcedureNode>>& map) {
    this->procToNodeMap = map;
}
