//
// Created by Michael on 25/3/24.
//

#include "ASTTraversal.h"

ASTTraversal::ASTTraversal(std::shared_ptr<ASTNode> astRoot) : root(std::move(astRoot)) {}

void ASTTraversal::traverse() {
    if (!root) return;

    if (auto programNode = std::dynamic_pointer_cast<ProgramNode>(root)) {
        traverseProgram(programNode);
        // add collateMap here
    } else return;
}

void ASTTraversal::traverseProgram(std::shared_ptr<ProgramNode> node) {
    for (auto procedure : node->getProcedures()) {
        traverseProcedure(procedure);
    }
}

void ASTTraversal::traverseProcedureStmtList(std::shared_ptr<StmtListNode> node, std::shared_ptr<CFGNode> parent) {
    std::shared_ptr<CFGNode> lexicalParent = parent;
    for (auto stmt : node->getStatements()) {
        traverseStmt(stmt,lexicalParent);
        if (lexicalParent->getEntityType() == EntityType::IF) {
            std::vector<std::shared_ptr<CFGNode>> vec1 = {};
            auto lastThen = context.getDeepestNode(lexicalParent->successors[0],vec1);
            std::vector<std::shared_ptr<CFGNode>> vec2 = {};
            auto lastElse = context.getDeepestNode(lexicalParent->successors[1],vec2);
            auto nextStmt = lexicalParent->successors[2];
            // Print the extended vector
            for (auto value : lastThen) {
                context.linkCFGNodes(value, nextStmt);
                context.insertToNextMap(value->key, nextStmt->key);
            }
            for (auto value : lastElse) {
                context.linkCFGNodes(value, nextStmt);
                context.insertToNextMap(value->key, nextStmt->key);
            }
            lexicalParent = nextStmt;
        } else if (lexicalParent->getEntityType() == EntityType::WHILE) {
            lexicalParent = lexicalParent->successors[1];
        } else {
            lexicalParent = lexicalParent->successors[0];
        }
    }
    // Place a Dummy Node at the end to signify end
    auto endCfgNode = std::make_shared<CFGNode>(-1,EntityType::PROCEDURE_END);
    if (lexicalParent->getEntityType() == EntityType::IF) {
        std::vector<std::shared_ptr<CFGNode>> vec1 = {};
        auto lastThen = context.getDeepestNode(lexicalParent->successors[0],vec1);
        std::vector<std::shared_ptr<CFGNode>> vec2 = {};
        auto lastElse = context.getDeepestNode(lexicalParent->successors[1],vec2);
        // Print the extended vector
        for (auto value : lastThen) {
            context.linkCFGNodes(value, endCfgNode);
            context.insertToNextMap(value->key, endCfgNode->key);
        }
        for (auto value : lastElse) {
            context.linkCFGNodes(value, endCfgNode);
            context.insertToNextMap(value->key, endCfgNode->key);
        }
    } else {
        context.linkCFGNodes(lexicalParent, endCfgNode);
    }
    context.insertToNextMap(lexicalParent->key,endCfgNode->key);
}

void ASTTraversal::traverseWhileStmtList(std::shared_ptr<StmtListNode> node, std::shared_ptr<CFGNode> parent) {
    std::shared_ptr<CFGNode> lexicalParent = parent;
    for (const auto& stmt : node->getStatements()) {
        traverseStmt(stmt,lexicalParent);
        if (lexicalParent->getEntityType() == EntityType::WHILE) {
            if (lexicalParent->successors.size() > 1) {
                lexicalParent = lexicalParent->successors[1];
            } else {
                lexicalParent = lexicalParent->successors[0];
            }
            continue;
        }
        if (lexicalParent->getEntityType() == EntityType::IF) {
            std::vector<std::shared_ptr<CFGNode>> vec1 = {};
            auto lastThen = context.getDeepestNode(lexicalParent->successors[0],vec1);
            std::vector<std::shared_ptr<CFGNode>> vec2 = {};
            auto lastElse = context.getDeepestNode(lexicalParent->successors[1],vec2);
            auto curr = lexicalParent->successors[2];
            // Print the extended vector
            for (auto value : lastThen) {
                context.linkCFGNodes(value, curr);
                context.insertToNextMap(value->key, curr->key);
            }
            for (auto value : lastElse) {
                context.linkCFGNodes(value, curr);
                context.insertToNextMap(value->key, curr->key);
            }
            lexicalParent = curr;
            continue;
        }
        lexicalParent = lexicalParent->successors[0];
    }
    // loop back to parent
    context.linkCFGNodes(lexicalParent,parent);
    if (lexicalParent->getEntityType() == EntityType::IF) {
        std::vector<std::shared_ptr<CFGNode>> vec1 = {};
        auto lastThen = context.getDeepestNode(lexicalParent->successors[0],vec1);
        std::vector<std::shared_ptr<CFGNode>> vec2 = {};
        auto lastElse = context.getDeepestNode(lexicalParent->successors[1],vec2);
        auto curr = lexicalParent->successors[2];
        // Print the extended vector
        for (auto value : lastThen) {
            context.linkCFGNodes(value, curr);
            context.insertToNextMap(value->key, curr->key);
        }
        for (auto value : lastElse) {
            context.linkCFGNodes(value, curr);
            context.insertToNextMap(value->key, curr->key);
        }
    }
    context.insertToNextMap(lexicalParent->key,parent->key);
}

void ASTTraversal::traverseThen(std::shared_ptr<StmtListNode> node, std::shared_ptr<CFGNode> parent) {
    std::shared_ptr<CFGNode> lexicalParent;
    for (const auto& stmt : node->getStatements()) {
        if (!lexicalParent) {
            traverseStmt(stmt,parent);
            lexicalParent = parent->successors[0];
            continue;
        }
        traverseStmt(stmt,lexicalParent);
        if (lexicalParent->getEntityType() == EntityType::WHILE) {
            if (lexicalParent->successors.size() > 1) {
                lexicalParent = lexicalParent->successors[1];
            } else {
                lexicalParent = lexicalParent->successors[0];
            }
            continue;
        }
        if ((lexicalParent->getEntityType() == EntityType::IF)) {
            std::vector<std::shared_ptr<CFGNode>> vec1 = {};
            auto lastThen = context.getDeepestNode(lexicalParent->successors[0],vec1);
            std::vector<std::shared_ptr<CFGNode>> vec2 = {};
            auto lastElse = context.getDeepestNode(lexicalParent->successors[1],vec2);
            auto curr = lexicalParent->successors[2];
            // Print the extended vector
            for (auto value : lastThen) {
                context.linkCFGNodes(value, curr);
                context.insertToNextMap(value->key, curr->key);
            }
            for (auto value : lastElse) {
                context.linkCFGNodes(value, curr);
                context.insertToNextMap(value->key, curr->key);
            }
            lexicalParent = curr;
            continue;
        }
        lexicalParent = lexicalParent->successors[0];
    }
}


void ASTTraversal::traverseElse(std::shared_ptr<StmtListNode> node, std::shared_ptr<CFGNode> parent) {
    std::shared_ptr<CFGNode> lexicalParent;
    for (const auto& stmt : node->getStatements()) {
        if (!lexicalParent) {
            traverseStmt(stmt,parent);
            lexicalParent = parent->successors[1];
            continue;
        }
        traverseStmt(stmt,lexicalParent);
        if (lexicalParent->getEntityType() == EntityType::WHILE) {
            if (lexicalParent->successors.size() > 1) {
                lexicalParent = lexicalParent->successors[1];
            } else {
                lexicalParent = lexicalParent->successors[0];
            }
            continue;
        }
        if (lexicalParent->getEntityType() == EntityType::IF) {
            std::vector<std::shared_ptr<CFGNode>> vec1 = {};
            auto lastThen = context.getDeepestNode(lexicalParent->successors[0],vec1);
            std::vector<std::shared_ptr<CFGNode>> vec2 = {};
            auto lastElse = context.getDeepestNode(lexicalParent->successors[1],vec2);
            auto curr = lexicalParent->successors[2];
            // Print the extended vector
            for (auto value : lastThen) {
                context.linkCFGNodes(value, curr);
                context.insertToNextMap(value->key, curr->key);
            }
            for (auto value : lastElse) {
                context.linkCFGNodes(value, curr);
                context.insertToNextMap(value->key, curr->key);
            }
            lexicalParent = curr;
            continue;
        }
        lexicalParent = lexicalParent->successors[0];
    }
}

void ASTTraversal::traverseCall(std::shared_ptr<CallNode> node) {
    // Handle call
    context.annotateCallGraph(context.getCurrentProcedure(),node->getName());
}

void ASTTraversal::traverseStmt(std::shared_ptr<ASTNode> node, std::shared_ptr<CFGNode> parentHere) {
    auto curr = std::make_shared<CFGNode>(node->getLineNumber(),node->getEntityType());
    context.linkCFGNodes(parentHere,curr);
    context.insertToNextMap(parentHere->key,curr->key);
    // Based on the node type, call the appropriate traversal function
    if (auto callNode = std::dynamic_pointer_cast<CallNode>(node)) {
        traverseCall(callNode);
    } else if (auto readNode = std::dynamic_pointer_cast<ReadNode>(node)) {
        traverseRead(readNode);
    } else if (auto printNode = std::dynamic_pointer_cast<PrintNode>(node)) {
        traversePrint(printNode);
    } else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(node)) {
        traverseWhile(whileNode,curr);
    } else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(node)) {
        traverseIf(ifNode,curr);
    } else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(node)) {
        // Handle assignment
        traverseAssign(assignNode);
    }
}

void ASTTraversal::traverseProcedure(std::shared_ptr<ProcedureNode> node) {
    // Handle the procedure node
    auto procedureCFGLabel = std::make_shared<CFGNode>(-1,EntityType::PROCEDURE);
    auto procedureCompressedCFGLabel = std::make_shared<CompressedCFGNode>(EntityType::PROCEDURE);
    context.cfgList.push_back(procedureCFGLabel);
    context.annotateProcedureCounts(node->getName());
    // Procedures contain statement lists, so call traverseStmtList
    traverseProcedureStmtList(node->getBody(),procedureCFGLabel);
}

void ASTTraversal::traverseRead(std::shared_ptr<ReadNode> node) {
    // ... implementation of traverseRead if needed
}

void ASTTraversal::traversePrint(std::shared_ptr<PrintNode> node) {
    // ... implementation of traversePrint if needed
}

void ASTTraversal::traverseWhile(std::shared_ptr<WhileNode> node, std::shared_ptr<CFGNode> parent) {
    // Handle while loop, traverse the condition and the body statement list
    traverseCondExpr(node->getCondition());
    traverseWhileStmtList(node->getBody(),parent);
}

void ASTTraversal::traverseIf(std::shared_ptr<IfNode> node, std::shared_ptr<CFGNode> parent) {
    // Handle if statement, traverse the condition and then/else statement lists
    traverseCondExpr(node->getCondition());
    traverseThen(node->getThenBranch(),parent);
    traverseElse(node->getElseBranch(),parent);
}


void ASTTraversal::traverseAssign(std::shared_ptr<AssignNode> node) {
    traverseVariable(node->getLeftChild());
    traverseRelFactor(node->getRightChild());
}

void ASTTraversal::traverseCondExpr(std::shared_ptr<CondExprNode> node) {
    if (auto binaryExpr = std::dynamic_pointer_cast<CondBinaryExprNode>(node)) {
        // Handle binary condition expressions
        traverseCondExpr(binaryExpr->getLeftChild());
        traverseCondExpr(binaryExpr->getRightChild());
    } else if (auto unaryExpr = std::dynamic_pointer_cast<CondUnaryExprNode>(node)) {
        // Handle unary condition expressions
        traverseCondExpr(unaryExpr->getOperand());
    } else if (auto relExpr = std::dynamic_pointer_cast<RelExprNode>(node)) {
        // Handle relational expressions
        traverseRelExpr(relExpr);
    }
}

void ASTTraversal::traverseCondBinaryExpr(std::shared_ptr<CondBinaryExprNode> node) {
    // ... implementation of traverseCondBinaryExpr
}

void ASTTraversal::traverseCondUnaryExpr(std::shared_ptr<CondUnaryExprNode> node) {
    // ... implementation of traverseCondUnaryExpr
}

void ASTTraversal::traverseRelExpr(std::shared_ptr<RelExprNode> node) {
    traverseRelFactor(node->getLeftChild());
    traverseRelFactor(node->getRightChild());
}

void ASTTraversal::traverseRelFactor(std::shared_ptr<RelFactorNode> node) {
    if (auto exprNode = std::dynamic_pointer_cast<ExprNode>(node)) {
        // Handle expression nodes
        traverseExpr(exprNode);
    } else if (auto variableNode = std::dynamic_pointer_cast<VariableNode>(node)) {
        // Handle variable nodes
        // Variables do not need further traversal
    } else if (auto constantNode = std::dynamic_pointer_cast<ConstantNode>(node)) {
        // Handle constant nodes
        // Constants do not need further traversal
    } else {
        return;
    }
}

void ASTTraversal::traverseExpr(std::shared_ptr<ExprNode> node) {
    if (node->getLeftChild()) {
        traverseRelFactor(node->getLeftChild());
    }
    if (node->getRightChild()) {
        traverseRelFactor(node->getRightChild());
    }
}

void ASTTraversal::traverseVariable(std::shared_ptr<VariableNode> node) {
    // ... implementation of traverseVariable if needed
}

void ASTTraversal::traverseConstant(std::shared_ptr<ConstantNode> node) {
    // ... implementation of traverseConstant if needed
}


