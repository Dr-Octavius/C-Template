#ifndef SPA_PROCEDURNODE_H
#define SPA_PROCEDURNODE_H

#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>
#include <string>

class ProcedureNode : public ASTNode {
private:
    std::string name;
    std::shared_ptr<StmtListNode> body;

public:
    ProcedureNode(const std::string& name, std::shared_ptr<StmtListNode> body, int lineNum);
    void accept(RelationshipVisitor& visitor) override;

    const std::string& getName() const;
    std::shared_ptr<StmtListNode>& getBody();
};


#endif //SPA_PROCEDURNODE_H
