#ifndef SPA_ENTITYVISITOR_H
#define SPA_ENTITYVISITOR_H
#include <map>
#include "common/Utils.h"
#include "RelationshipVisitor.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/CallNode.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/ast/nodes/controlFlow/ProgramNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/AssignNode.h"
#include "sp/ast/nodes/statements/PrintNode.h"
#include "sp/ast/nodes/statements/ReadNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/ast/nodes/expressions/conditional/CondBinaryExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondUnaryExprNode.h"
#include "sp/ast/nodes/expressions/relational/RelExprNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include <memory>
#include <iostream>
#include <string>
#include <unordered_set>

class EntityVisitor : public RelationshipVisitor {
public:
    std::map<std::string, EntityType> stmtNumberMap;
    std::map<EntityType, std::unordered_set<std::string>> entityToStatementsMap;
    std::map<std::string, std::shared_ptr<ProcedureNode>> procToNodeMap;
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> stmtSpecificEntitiesMap;

    EntityVisitor();
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

    const std::map<std::string, EntityType>& getStmtNumberMap() const;
    const std::map<std::string, std::shared_ptr<ProcedureNode>>& getProcToNodeMap() const;
    const std::map<EntityType, std::unordered_set<std::string>>& getEntityToStatementsMap() const;
    const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& getStmtSpecificEntitiesMap() const;
private:
    void addStmtForEntity(EntityType type, const std::string& lineNumber);
    void addToStmtNumMap(const std::string& lineNum, EntityType type);
};

#endif //SPA_ENTITYVISITOR_H
