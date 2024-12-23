#ifndef SPA_ASSIGNNODE_H
#define SPA_ASSIGNNODE_H

#include "sp/ast/nodes/ASTNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include <memory>

class AssignNode : public ASTNode {
private:
    std::shared_ptr<VariableNode> leftChild;
    std::shared_ptr<RelFactorNode> rightChild;

public:
    AssignNode(std::shared_ptr<VariableNode> leftChild, std::shared_ptr<RelFactorNode> rightChild, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<VariableNode>& getLeftChild();
    std::shared_ptr<RelFactorNode>& getRightChild();
    int getLineNumber() const;
};

#endif //SPA_ASSIGNNODE_H
