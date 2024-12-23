//
// Created by Michael on 25/2/24.
//

#include "ASTCompare.h"

bool compareProcedureNodes(const std::shared_ptr<ProcedureNode>& actual, const std::shared_ptr<ProcedureNode>& expected) {
    // Compare procedure name
    if (actual->getName() != expected->getName()) {
        return false;
    }
    // Compare body
    return compareStmtListNodes(actual->getBody(), expected->getBody());
}

bool compareWhileNodes(const std::shared_ptr<WhileNode>& actual, const std::shared_ptr<WhileNode>& expected) {
    // Compare condition
    if (!compareCondExprNodes(actual->getCondition(), expected->getCondition())) {
        return false;
    }
    // Compare body
    return compareStmtListNodes(actual->getBody(), expected->getBody());
}

bool compareReadNodes(const std::shared_ptr<ReadNode>& actual, const std::shared_ptr<ReadNode>& expected) {
    // Compare variable
    return compareVariableNodes(actual->getVariable(), expected->getVariable());
}

bool comparePrintNodes(const std::shared_ptr<PrintNode>& actual, const std::shared_ptr<PrintNode>& expected) {
    // Compare variable
    return compareVariableNodes(actual->getVariable(), expected->getVariable());
}

bool compareIfNodes(const std::shared_ptr<IfNode>& actual, const std::shared_ptr<IfNode>& expected) {
    // Compare condition
    if (!compareCondExprNodes(actual->getCondition(), expected->getCondition())) {
        return false;
    }
    // Compare then branch
    if (!compareStmtListNodes(actual->getThenBranch(), expected->getThenBranch())) {
        return false;
    }
    // Compare else branch if it exists
    if ((actual->getElseBranch() == nullptr && expected->getElseBranch() != nullptr) ||
        (actual->getElseBranch() != nullptr && expected->getElseBranch() == nullptr)) {
        return false;
    }
    if (actual->getElseBranch() != nullptr && expected->getElseBranch() != nullptr) {
        return compareStmtListNodes(actual->getElseBranch(), expected->getElseBranch());
    }
    return true;
}

bool compareAssignNodes(const std::shared_ptr<AssignNode>& actual, const std::shared_ptr<AssignNode>& expected) {
    // Compare left child (variable)
    if (!compareVariableNodes(actual->getLeftChild(), expected->getLeftChild())) {
        traversePrintVariable(expected->getLeftChild());
        traversePrintVariable(actual->getLeftChild());
        return false;
    }
    // Compare right child (relational factor)
    return compareRelFactorNodes(actual->getRightChild(), expected->getRightChild());
}

bool compareStmtListNodes(const std::shared_ptr<StmtListNode>& actual, const std::shared_ptr<StmtListNode>& expected) {
    // Compare size
    if (actual->getStatements().size() != expected->getStatements().size()) {
        return false;
    }

    // Compare each statement
    for (size_t i = 0; i < actual->getStatements().size(); ++i) {
        auto actualStmt = actual->getStatements()[i];
        auto expectedStmt = expected->getStatements()[i];

        // Dynamic casting to the actual type of statement
        if (auto whileStmt = std::dynamic_pointer_cast<WhileNode>(actualStmt)) {
            if (!compareWhileNodes(whileStmt, std::dynamic_pointer_cast<WhileNode>(expectedStmt))) {
                return false;
            }
        } else if (auto ifStmt = std::dynamic_pointer_cast<IfNode>(actualStmt)) {
            if (!compareIfNodes(ifStmt, std::dynamic_pointer_cast<IfNode>(expectedStmt))) {
                return false;
            }
        } else if (auto readStmt = std::dynamic_pointer_cast<ReadNode>(actualStmt)) {
            if (!compareReadNodes(readStmt, std::dynamic_pointer_cast<ReadNode>(expectedStmt))) {
                return false;
            }
        } else if (auto printStmt = std::dynamic_pointer_cast<PrintNode>(actualStmt)) {
            if (!comparePrintNodes(printStmt, std::dynamic_pointer_cast<PrintNode>(expectedStmt))) {
                return false;
            }
        } else if (auto assignStmt = std::dynamic_pointer_cast<AssignNode>(actualStmt)) {
            if (!compareAssignNodes(assignStmt, std::dynamic_pointer_cast<AssignNode>(expectedStmt))) {
                return false;
            }
        } else {
            // Handle unsupported statement type
            return false;
        }
    }

    return true;
}

bool compareCondExprNodes(const std::shared_ptr<CondExprNode>& actual, const std::shared_ptr<CondExprNode>& expected) {
    // Compare types
    if (auto relExpr = std::dynamic_pointer_cast<RelExprNode>(actual)) {
        if (!compareRelExprNodes(relExpr, std::dynamic_pointer_cast<RelExprNode>(expected))) {
            return false;
        }
    } else if (auto unaryExpr = std::dynamic_pointer_cast<CondUnaryExprNode>(actual)) {
        if (!compareCondUnaryExprNodes(unaryExpr, std::dynamic_pointer_cast<CondUnaryExprNode>(expected))) {
            traversePrintCondUnaryExpr(unaryExpr);
            return false;
        }
    } else if (auto binaryExpr = std::dynamic_pointer_cast<CondBinaryExprNode>(actual)) {
        auto expectedExpr = std::dynamic_pointer_cast<CondBinaryExprNode>(expected);
        if (!compareCondBinaryExprNodes(binaryExpr, expectedExpr)) {
            return false;
        }
    } else {
        // Handle unsupported statement type
        return false;
    }
    return true;
}

bool compareCondBinaryExprNodes(const std::shared_ptr<CondBinaryExprNode>& actual, const std::shared_ptr<CondBinaryExprNode>& expected) {
    // Compare operator
    if (actual->getOperator() != expected->getOperator()) {
        return false;
    }
    // Compare left and right children
    return compareCondExprNodes(actual->getLeftChild(), expected->getLeftChild()) &&
           compareCondExprNodes(actual->getRightChild(), expected->getRightChild());
}

bool compareCondUnaryExprNodes(const std::shared_ptr<CondUnaryExprNode>& actual, const std::shared_ptr<CondUnaryExprNode>& expected) {
    // Compare operator
    if (actual->getOperator() != expected->getOperator()) {
        return false;
    }
    // Compare operand
    return compareCondExprNodes(actual->getOperand(), expected->getOperand());
}

bool compareRelExprNodes(const std::shared_ptr<RelExprNode>& actual, const std::shared_ptr<RelExprNode>& expected) {
    // Compare operator
    if (actual->getOperator() != expected->getOperator()) {
        return false;
    }
    // Compare left and right children
    return compareRelFactorNodes(actual->getLeftChild(), expected->getLeftChild()) &&
           compareRelFactorNodes(actual->getRightChild(), expected->getRightChild());
}

bool compareRelFactorNodes(const std::shared_ptr<RelFactorNode>& actual, const std::shared_ptr<RelFactorNode>& expected) {

    // If the type is VariableNode, compare the names
    if (auto actualVar = std::dynamic_pointer_cast<VariableNode>(actual)) {
        auto expectedVar = std::dynamic_pointer_cast<VariableNode>(expected);
        return compareVariableNodes(actualVar, expectedVar);
    }

    // If the type is ConstantNode, compare the values
    if (auto actualConst = std::dynamic_pointer_cast<ConstantNode>(actual)) {
        auto expectedConst = std::dynamic_pointer_cast<ConstantNode>(expected);
        return compareConstantNodes(actualConst, expectedConst);
    }

    // If the type is ExprNode, compare the operators
    if (auto actualExpr = std::dynamic_pointer_cast<ExprNode>(actual)) {
        auto expectedExpr = std::dynamic_pointer_cast<ExprNode>(expected);
        return compareExprNodes(actualExpr, expectedExpr);
    }

    // Handle unsupported node type
    return false;
}

bool compareExprNodes(const std::shared_ptr<ExprNode>& actual, const std::shared_ptr<ExprNode>& expected) {
    // Compare operator
    if (actual->getOperator() != expected->getOperator()) {
        return false;
    }
    // Compare left and right children
    return compareRelFactorNodes(actual->getLeftChild(), expected->getLeftChild()) &&
           compareRelFactorNodes(actual->getRightChild(), expected->getRightChild());
}

bool compareConstantNodes(const std::shared_ptr<ConstantNode>& actual, const std::shared_ptr<ConstantNode>& expected) {
    // Compare value
    if (actual->getValue() != expected->getValue()) {
        return false;
    }
    return true;
}

bool compareVariableNodes(const std::shared_ptr<VariableNode>& actual, const std::shared_ptr<VariableNode>& expected) {
    // Compare name
    if (actual->getName() != expected->getName()) {
        return false;
    }
    return true;
}

bool compareLineNumber(const std::shared_ptr<ASTNode>& actual, const std::shared_ptr<ASTNode>& expected) {
    // Compare name
    if (actual->getLineNumber() != expected->getLineNumber()) {
        return false;
    }
    return true;
}
