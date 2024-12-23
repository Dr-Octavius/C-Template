#ifndef SPA_RELFACTORNODE_H
#define SPA_RELFACTORNODE_H

#include "sp/ast/nodes/ASTNode.h"
#include <string>
#include <vector>

class RelFactorNode : public ASTNode {
public:
    using ASTNode::ASTNode;
    virtual std::vector<std::string> generateSubtreeStrings() = 0;
    virtual std::string generateExpressionString() = 0;
};


#endif //SPA_RELFACTORNODE_H
