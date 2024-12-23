#ifndef SPA_IFNODE_H
#define SPA_IFNODE_H

#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "sp/ast/nodes/expressions/CondExprNode.h"
#include <memory>

class IfNode : public ASTNode {
private:
    std::shared_ptr<CondExprNode> condition;
    std::shared_ptr<StmtListNode> thenBranch;
    std::shared_ptr<StmtListNode> elseBranch;

public:
    IfNode(std::shared_ptr<CondExprNode> condition, std::shared_ptr<StmtListNode> thenBranch, std::shared_ptr<StmtListNode> elseBranch, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<CondExprNode>& getCondition();
    std::shared_ptr<StmtListNode>& getThenBranch();
    std::shared_ptr<StmtListNode>& getElseBranch();
};

#endif //SPA_IFNODE_H
