#ifndef SPA_STMTLISTNODE_H
#define SPA_STMTLISTNODE_H

#include "sp/ast/nodes/ASTNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>
#include <vector>

class StmtListNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ASTNode>> statements; // Use shared_ptr instead of unique_ptr

public:
    StmtListNode(int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    // Adjust the addStatement method to accept shared_ptr
    void addStatement(std::shared_ptr<ASTNode> statement);
    // Adjust the return type of getStatements
    std::vector<std::shared_ptr<ASTNode>>& getStatements();
};

#endif //SPA_STMTLISTNODE_H
