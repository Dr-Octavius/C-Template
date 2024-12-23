#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "sp/design_extractor/visitors/FollowsVisitor.h"
#include "sp/design_extractor/visitors/EntityVisitor.h"
#include "sp/design_extractor/visitors/ParentVisitor.h"
#include "sp/design_extractor/visitors/UsesVisitor.h"
#include "sp/design_extractor/visitors/CallsVisitor.h"
#include "sp/design_extractor/visitors/ModifiesVisitor.h"
#include "sp/design_extractor/visitors/RelationshipVisitor.h"
#include "sp/design_extractor/visitors/PatternVisitor.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/StmtListNode.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/ast/nodes/controlFlow/ProgramNode.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include "sp/pkb_integration/StorageWriter.h"
#include "sp/pkb_integration/storage/RelationshipStorage.h"
#include <memory>

class DesignExtractor {
private:
    std::shared_ptr<FollowsVisitor> followsVisitor;
    std::shared_ptr<ParentVisitor> parentVisitor;
    std::shared_ptr<UsesVisitor> usesVisitor;
    std::shared_ptr<ModifiesVisitor> modifiesVisitor;
    std::shared_ptr<EntityVisitor> entitiesVisitor;
    std::shared_ptr<CallsVisitor> callsVisitor;
    std::shared_ptr<PatternVisitor> patternVisitor;
    std::shared_ptr<StorageManager> pkbStorageManager;
public:
    DesignExtractor(std::shared_ptr<FollowsVisitor> followsV, std::shared_ptr<ParentVisitor> parentV,
                    std::shared_ptr<UsesVisitor> usesV, std::shared_ptr<ModifiesVisitor> modifiesV,
                    std::shared_ptr<EntityVisitor> entityV, std::shared_ptr<CallsVisitor> callsV,
                    std::shared_ptr<PatternVisitor> patterns, std::shared_ptr<StorageManager> pkbStorageManager)
            : followsVisitor(followsV), parentVisitor(parentV), usesVisitor(usesV), modifiesVisitor(modifiesV),
              entitiesVisitor(entityV), callsVisitor(callsV), patternVisitor(patterns), pkbStorageManager(pkbStorageManager) {}

    void insertRelationshipRecords(
            const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& records,
            void (RelationshipStorage::*insertFunction)(const std::pair<std::string, EntityType>&, const std::pair<std::string, EntityType>&),
            const std::shared_ptr<RelationshipStorage>& relationshipStorage
    );

    void insertEntityRecords(
            const std::map<EntityType, std::unordered_set<std::string>>& records,
            const std::shared_ptr<EntityStorage>& entityStorage
    );

    void traverse(const std::shared_ptr<ASTNode>& node, RelationshipVisitor& visitor);
    void traverseAll(const std::shared_ptr<ASTNode>& node);
    void writeToPkb();
};

#endif //SPA_DESIGNEXTRACTOR_H
