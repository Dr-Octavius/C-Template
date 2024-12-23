#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H


#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>

class PrintNode : public ASTNode {
private:
    std::shared_ptr<VariableNode> variable;

public:
    PrintNode(std::shared_ptr<VariableNode> variable, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::shared_ptr<VariableNode>& getVariable();
};


#endif //SPA_PRINTNODE_H
