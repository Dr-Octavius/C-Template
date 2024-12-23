//
// Created by Michael on 25/2/24.
//

#ifndef SPA_ASTCOMPARE_H
#define SPA_ASTCOMPARE_H

#include <iostream>
#include <memory>
#include "ASTTraversePrint.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/ast/nodes/expressions/CondExprNode.h"
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondBinaryExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondUnaryExprNode.h"
#include "sp/ast/nodes/expressions/relational/RelExprNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/ReadNode.h"
#include "sp/ast/nodes/statements/PrintNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include "sp/ast/nodes/statements/AssignNode.h"

bool compareProcedureNodes(const std::shared_ptr<ProcedureNode>& actual, const std::shared_ptr<ProcedureNode>& expected);
bool compareWhileNodes(const std::shared_ptr<WhileNode>& actual, const std::shared_ptr<WhileNode>& expected);
bool compareReadNodes(const std::shared_ptr<ReadNode>& actual, const std::shared_ptr<ReadNode>& expected);
bool comparePrintNodes(const std::shared_ptr<PrintNode>& actual, const std::shared_ptr<PrintNode>& expected);
bool compareIfNodes(const std::shared_ptr<IfNode>& actual, const std::shared_ptr<IfNode>& expected);
bool compareAssignNodes(const std::shared_ptr<AssignNode>& actual, const std::shared_ptr<AssignNode>& expected);
bool compareStmtListNodes(const std::shared_ptr<StmtListNode>& actual, const std::shared_ptr<StmtListNode>& expected);
bool compareCondExprNodes(const std::shared_ptr<CondExprNode>& actual, const std::shared_ptr<CondExprNode>& expected);
bool compareCondBinaryExprNodes(const std::shared_ptr<CondBinaryExprNode>& actual, const std::shared_ptr<CondBinaryExprNode>& expected);
bool compareCondUnaryExprNodes(const std::shared_ptr<CondUnaryExprNode>& actual, const std::shared_ptr<CondUnaryExprNode>& expected);
bool compareRelExprNodes(const std::shared_ptr<RelExprNode>& actual, const std::shared_ptr<RelExprNode>& expected);
bool compareRelFactorNodes(const std::shared_ptr<RelFactorNode>& actual, const std::shared_ptr<RelFactorNode>& expected);
bool compareExprNodes(const std::shared_ptr<ExprNode>& actual, const std::shared_ptr<ExprNode>& expected);
bool compareConstantNodes(const std::shared_ptr<ConstantNode>& actual, const std::shared_ptr<ConstantNode>& expected);
bool compareVariableNodes(const std::shared_ptr<VariableNode>& actual, const std::shared_ptr<VariableNode>& expected);
bool compareLineNumber(const std::shared_ptr<ASTNode>& actual, const std::shared_ptr<ASTNode>& expected);



#endif //SPA_ASTCOMPARE_H
