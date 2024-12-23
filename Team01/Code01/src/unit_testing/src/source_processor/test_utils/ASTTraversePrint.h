//
// Created by Michael on 25/2/24.
//

#ifndef SPA_ASTTRAVERSALPRINT_H
#define SPA_ASTTRAVERSALPRINT_H

#include <iostream>
#include <memory>
#include <map>
#include <unordered_set>
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/ast/nodes/expressions/CondExprNode.h"
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondBinaryExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondUnaryExprNode.h"
#include "sp/ast/nodes/expressions/relational/RelExprNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include "sp/ast/nodes/controlFlow/ProgramNode.h"
#include "sp/ast/nodes/controlFlow/CallNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/ReadNode.h"
#include "sp/ast/nodes/statements/PrintNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include "sp/ast/nodes/statements/AssignNode.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm> // for std::transform

// Enum to represent statement types
enum class TraversalPrintType {
    TEST,
    ACTUAL
};

// Function to print the contents of the statement line map in uppercase
void printTestStatementLineMap();
void printActualStatementLineMap();
void printActualStatementLines();
void printTestStatementLines();

// Define the map type for statement to line number mapping
extern std::unordered_map<int, std::vector<std::string>> TestStatementLineMap;
extern std::unordered_map<int, std::vector<std::string>> ActualStatementLineMap;

// Function to update the map during traversal
void updateTestLineMap(int lineNumber, const std::string& statement);
void updateActualLineMap(int lineNumber, const std::string& statement);

// Declare traversal functions
void traversePrintProgram(const std::shared_ptr<ProgramNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintProcedure(const std::shared_ptr<ProcedureNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintWhileStatement(const std::shared_ptr<WhileNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintCallStatement(const std::shared_ptr<CallNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintReadStatement(const std::shared_ptr<ReadNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintPrintStatement(const std::shared_ptr<PrintNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintIfStatement(const std::shared_ptr<IfNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintAssignStatement(const std::shared_ptr<AssignNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintStatementList(const std::shared_ptr<StmtListNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintCondExpr(const std::shared_ptr<CondExprNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintCondBinaryExpr(const std::shared_ptr<CondBinaryExprNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintCondUnaryExpr(const std::shared_ptr<CondUnaryExprNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintRelExpr(const std::shared_ptr<RelExprNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintRelFactor(const std::shared_ptr<RelFactorNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintExpr(const std::shared_ptr<ExprNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintConstant(const std::shared_ptr<ConstantNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void traversePrintVariable(const std::shared_ptr<VariableNode>& node, int depth = 0, TraversalPrintType type = TraversalPrintType::ACTUAL);
void printNextMap(const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& nextMap);

#endif //SPA_ASTTRAVERSALPRINT_H
