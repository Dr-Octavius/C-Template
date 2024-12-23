#ifndef SPA_CALLSVISITOR_H
#define SPA_CALLSVISITOR_H
#include <map>
#include "common/Utils.h"
#include "RelationshipVisitor.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/CallNode.h"
#include "sp/ast/nodes/controlFlow/ProgramNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include <memory>
#include <iostream>
#include <utility>
#include <unordered_set>

class CallsVisitor : public RelationshipVisitor {
private:
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> calls;
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> callsT;
    std::string currentProcedure;
public:
    CallsVisitor();

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

    void populateCallsT();
    const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& getCalls() const;
    const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& getCallsT() const;
};

#endif //SPA_CALLSVISITOR_H
