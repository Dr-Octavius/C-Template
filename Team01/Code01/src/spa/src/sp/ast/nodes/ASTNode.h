#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "common/Utils.h"
#include <unordered_map>

class ASTNode {
protected:
    int lineNumber;
    EntityType type;
public:
    ASTNode(int lineNum, EntityType t) : lineNumber(lineNum), type(t) {}
    virtual ~ASTNode() {}
    virtual void accept(RelationshipVisitor& visitor) = 0;
    [[nodiscard]] int getLineNumber() const { return lineNumber; }
    [[nodiscard]] EntityType getEntityType() const { return type; }
};

#endif //SPA_ASTNODE_H
