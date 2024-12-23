#ifndef SPA_CALLNODE_H
#define SPA_CALLNODE_H

#include "sp/ast/nodes/ASTNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>
#include <string>

class CallNode : public ASTNode {
private:
    std::string procName;

public:
    CallNode(const std::string& procName, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    std::string& getName();
};


#endif //SPA_CALLNODE_H
