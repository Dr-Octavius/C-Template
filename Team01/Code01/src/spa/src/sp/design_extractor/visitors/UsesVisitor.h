#ifndef SPA_USESVISITOR_H
#define SPA_USESVISITOR_H

#include <map>
#include <unordered_set>
#include <set>
#include "RelationshipVisitor.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/ast/nodes/controlFlow/CallNode.h"
#include "sp/ast/nodes/controlFlow/ProgramNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/AssignNode.h"
#include "sp/ast/nodes/expressions/conditional/CondBinaryExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondUnaryExprNode.h"
#include "sp/ast/nodes/expressions/relational/RelExprNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include "common/Utils.h"
#include <memory>
#include <iostream>

class UsesVisitor : public RelationshipVisitor {
public:
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> uses;
    std::set<std::string> nestedLines;
    std::map<std::string, EntityType> stmtNumberMap;
    std::map<std::string, std::shared_ptr<ProcedureNode>> procToNodeMap;
    UsesVisitor();
    void visit(ASTNode& node) override;
    void visit(ProgramNode& node) override;
    void visit(ProcedureNode& node) override;
    void visit(CallNode& node) override;
    void visit(ReadNode& node) override;
    void visit(PrintNode& node) override;
    void visit(AssignNode& node) override;
    void visit(ExprNode& node) override;
    void visit(RelExprNode& node) override;
    void visit(CondBinaryExprNode& node) override;
    void visit(CondUnaryExprNode& node) override;
    void visit(ConstantNode& node) override;
    void visit(VariableNode& node) override;
    void visit(IfNode& node) override;
    void visit(WhileNode& node) override;
    void visit(StmtListNode& node) override;

    const std::map<std::pair<std::string, EntityType>,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& getUses() const;

    void setStmtNumberMap(const std::map<std::string, EntityType>& map);
    void setProcToNodeMap(const std::map<std::string, std::shared_ptr<ProcedureNode>>& map);
};

#endif //SPA_USESVISITOR_H
