#ifndef SPA_CONSTANTNODE_H
#define SPA_CONSTANTNODE_H

#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include <string>
#include <vector>

class ConstantNode : public RelFactorNode {
private:
    int value;

public:
    ConstantNode(int value, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    int getValue() const;
    std::vector<std::string> generateSubtreeStrings() override;
    std::string generateExpressionString() override;
};

#endif //SPA_CONSTANTNODE_H
