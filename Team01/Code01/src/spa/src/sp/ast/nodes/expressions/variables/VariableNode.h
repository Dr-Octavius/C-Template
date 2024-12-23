#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include <string>
#include <vector>

class VariableNode : public RelFactorNode {
private:
    std::string name;

public:
    VariableNode(const std::string& name, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::string& getName();
    std::vector<std::string> generateSubtreeStrings() override;
    std::string generateExpressionString() override;
};

#endif //SPA_VARIABLENODE_H
