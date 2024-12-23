//
// Created by Michael on 25/3/24.
//

#ifndef SPA_ASTTRAVERSAL_H
#define SPA_ASTTRAVERSAL_H

#include <memory>
#include "sp/ast/nodes/ASTNode.h" // Include the header with ASTNode and its subclasses
#include "sp/ast/nodes/controlFlow/ProgramNode.h"
#include "sp/ast/nodes/controlFlow/CallNode.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/ast/nodes/statements/ReadNode.h"
#include "sp/ast/nodes/statements/PrintNode.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include "sp/ast/nodes/statements/AssignNode.h"
#include "sp/ast/nodes/expressions/CondExprNode.h"
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/relational/RelExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondBinaryExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondUnaryExprNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include "TraversalContext.h"

class ASTTraversal {
public:
    ASTTraversal(std::shared_ptr<ASTNode> root);
    TraversalContext context;
    void traverse();
private:
    std::shared_ptr<ASTNode> root;
    void traverseProcedureStmtList(std::shared_ptr<StmtListNode> node,std::shared_ptr<CFGNode> parent);
    void traverseWhileStmtList(std::shared_ptr<StmtListNode> node, std::shared_ptr<CFGNode> parent);
    void traverseThen(std::shared_ptr<StmtListNode> node, std::shared_ptr<CFGNode> parent);
    void traverseElse(std::shared_ptr<StmtListNode> node, std::shared_ptr<CFGNode> parent);
    void traverseCall(std::shared_ptr<CallNode> node);
    void traverseProgram(std::shared_ptr<ProgramNode> node);
    void traverseStmt(std::shared_ptr<ASTNode> node, std::shared_ptr<CFGNode> parent);
    void traverseProcedure(std::shared_ptr<ProcedureNode> node);
    void traversePrint(std::shared_ptr<PrintNode> node);
    void traverseRead(std::shared_ptr<ReadNode> node);
    void traverseWhile(std::shared_ptr<WhileNode> node, std::shared_ptr<CFGNode> parent);
    void traverseIf(std::shared_ptr<IfNode> node, std::shared_ptr<CFGNode> parent);
    void traverseAssign(std::shared_ptr<AssignNode> node);
    void traverseCondExpr(std::shared_ptr<CondExprNode> node);
    void traverseCondBinaryExpr(std::shared_ptr<CondBinaryExprNode> node);
    void traverseCondUnaryExpr(std::shared_ptr<CondUnaryExprNode> node);
    void traverseRelExpr(std::shared_ptr<RelExprNode> node);
    void traverseRelFactor(std::shared_ptr<RelFactorNode> node);
    void traverseExpr(std::shared_ptr<ExprNode> node);
    void traverseVariable(std::shared_ptr<VariableNode> node);
    void traverseConstant(std::shared_ptr<ConstantNode> node);
};



#endif //SPA_ASTTRAVERSAL_H
