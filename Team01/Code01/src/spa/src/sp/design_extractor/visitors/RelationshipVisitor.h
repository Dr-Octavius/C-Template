#ifndef SPA_RELATIONSHIPVISITOR_H
#define SPA_RELATIONSHIPVISITOR_H

class ASTNode;
class AssignNode;
class ExprNode;
class RelExprNode;
class CondBinaryExprNode;
class CondUnaryExprNode;
class ConstantNode;
class VariableNode;
class IfNode;
class WhileNode;
class ProcedureNode;
class ReadNode;
class CallNode;
class PrintNode;
class StmtListNode;
class ProgramNode;

class RelationshipVisitor {
public:
    virtual ~RelationshipVisitor() = default;

    virtual void visit(ASTNode& node) = 0;
    virtual void visit(ProcedureNode& node) = 0;
    virtual void visit(CallNode& node) = 0;
    virtual void visit(ReadNode& node) = 0;
    virtual void visit(PrintNode& node) = 0;
    virtual void visit(AssignNode& node) = 0;
    virtual void visit(ExprNode& node) = 0;
    virtual void visit(RelExprNode& node) = 0;
    virtual void visit(CondBinaryExprNode& node) = 0;
    virtual void visit(CondUnaryExprNode& node) = 0;
    virtual void visit(ConstantNode& node) = 0;
    virtual void visit(VariableNode& node) = 0;
    virtual void visit(IfNode& node) = 0;
    virtual void visit(WhileNode& node) = 0;
    virtual void visit(StmtListNode& node) = 0;
    virtual void visit(ProgramNode& node) = 0;
};

#endif //SPA_RELATIONSHIPVISITOR_H
