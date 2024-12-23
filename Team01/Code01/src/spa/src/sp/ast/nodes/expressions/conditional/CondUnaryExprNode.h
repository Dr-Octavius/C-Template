#ifndef SPA_CONDUNARYEXPRNODE_H
#define SPA_CONDUNARYEXPRNODE_H

#include "sp/ast/nodes/expressions/CondExprNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "sp/ast/nodes/expressions/conditional/CondExprNodeUtil.h"
#include <memory>
#include <string>

class CondUnaryExprNode : public CondExprNode {
private:
    std::shared_ptr<CondExprNode> operand;
    CondExprOperator op;

public:
    CondUnaryExprNode(std::shared_ptr<CondExprNode> operand, CondExprOperator op, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<CondExprNode>& getOperand();
    CondExprOperator getOperator();
};

#endif //SPA_CONDUNARYEXPRNODE_H
