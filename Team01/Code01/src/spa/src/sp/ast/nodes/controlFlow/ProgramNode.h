#ifndef SPA_PROGRAMNODE_H
#define SPA_PROGRAMNODE_H

#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include <memory>

class ProgramNode : public ASTNode {
private:
    // Only 1 procedure for MS1
    std::vector<std::shared_ptr<ProcedureNode>> procedures;

public:
    // Will be enhanced to children after MS1
    ProgramNode(int lineNum);
    void accept(RelationshipVisitor& visitor) override;
    void addProcedure(std::shared_ptr<ProcedureNode> statement);
    std::vector<std::shared_ptr<ProcedureNode>>& getProcedures();
    std::shared_ptr<ProcedureNode>& getProcedure();
};

#endif //SPA_PROGRAMNODE_H
