#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "sp/ast/nodes/expressions/CondExprNode.h"
#include <memory>

class WhileNode : public ASTNode {
private:
    std::shared_ptr<CondExprNode> condition;
    std::shared_ptr<StmtListNode> body;

public:
    WhileNode(std::shared_ptr<CondExprNode> condition, std::shared_ptr<StmtListNode> body, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<CondExprNode>& getCondition();
    std::shared_ptr<StmtListNode>& getBody();
};

#endif //SPA_WHILENODE_H
