#include "DesignExtractor.h"

void DesignExtractor::traverse(const std::shared_ptr<ASTNode>& node, RelationshipVisitor& visitor) {
    if (!node) return;

    node->accept(visitor);
}

void DesignExtractor::insertRelationshipRecords(
        const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& records,
        void (RelationshipStorage::*insertFunction)(const std::pair<std::string, EntityType>&, const std::pair<std::string, EntityType>&),
        const std::shared_ptr<RelationshipStorage>& relationshipStorage
) {
    for (const auto& pair : records) {
        for (const auto& innerPair : pair.second) {
            (relationshipStorage.get()->*insertFunction)(pair.first, innerPair);
        }
    }
}

void DesignExtractor::insertEntityRecords(
        const std::map<EntityType, std::unordered_set<std::string>>& records,
        const std::shared_ptr<EntityStorage>& entityStorage
) {
    for (const auto& pair : records) {
        for (const auto& name : pair.second) {
            entityStorage->insertEntity(name, pair.first);
        }
    }
}

void DesignExtractor::writeToPkb() {
    std::shared_ptr<StorageWriter> storageWriter = std::make_shared<StorageWriter>(pkbStorageManager->getWriteStorage(), pkbStorageManager->getReadStorage());
    std::shared_ptr<RelationshipStorage> relationshipStorage = storageWriter->getRelationshipStorage();
    std::shared_ptr<EntityStorage> entitiesStorage = storageWriter->getEntityStorage();
    std::shared_ptr<PatternStorage> patternStorage = storageWriter->getPatternStorage();

    insertRelationshipRecords(followsVisitor->getFollows(), &RelationshipStorage::insertFollowsRelationship, relationshipStorage);
    insertRelationshipRecords(followsVisitor->getFollowsT(), &RelationshipStorage::insertFollowsTRelationship, relationshipStorage);
    insertRelationshipRecords(parentVisitor->getParents(), &RelationshipStorage::insertParentsRelationship, relationshipStorage);
    insertRelationshipRecords(parentVisitor->getParentT(), &RelationshipStorage::insertParentsTRelationship, relationshipStorage);
    insertRelationshipRecords(usesVisitor->getUses(), &RelationshipStorage::insertUsesRelationship, relationshipStorage);
    insertRelationshipRecords(modifiesVisitor->getModifies(), &RelationshipStorage::insertModifiesRelationship, relationshipStorage);
    insertEntityRecords(entitiesVisitor->getEntityToStatementsMap(), entitiesStorage);
    insertRelationshipRecords(callsVisitor->getCalls(), &RelationshipStorage::insertCallsRelationship, relationshipStorage);
    insertRelationshipRecords(callsVisitor->getCallsT(), &RelationshipStorage::insertCallsTRelationship, relationshipStorage);

    for (const auto& pair : patternVisitor->getAssignPatternData()) {
        patternStorage->insertAssignPattern(pair.first, pair.second);
    }

    for (const auto& pair : patternVisitor->getIfPatternData()) {
        for (const auto& innerPair : pair.second) {
            patternStorage->insertIfPattern(pair.first, innerPair);
        }
    }

    for (const auto& pair : patternVisitor->getWhilePatternData()) {
        for (const auto& innerPair : pair.second) {
            patternStorage->insertWhilePattern(pair.first, innerPair);
        }
    }

    for (const auto& mapEntry : entitiesVisitor->stmtSpecificEntitiesMap) {
        const auto& stmtAndType = mapEntry.first;
        const auto& entitiesSet = mapEntry.second;

        for (const auto& entityPair : entitiesSet) {
            if (stmtAndType.second == EntityType::READ) {
                relationshipStorage->insertReadVarNameRelationship(stmtAndType, entityPair);
            } else if (stmtAndType.second == EntityType::PRINT) {
                relationshipStorage->insertPrintVarNameRelationship(stmtAndType, entityPair);
            } else if (stmtAndType.second == EntityType::CALL) {
                relationshipStorage->insertCallProcNameRelationship(stmtAndType, entityPair);
            }
        }
    }
}

void DesignExtractor::traverseAll(const std::shared_ptr<ASTNode>& node) {
    DesignExtractor::traverse(node, *entitiesVisitor);
    auto stmtNumberMap = entitiesVisitor->getStmtNumberMap();
    auto procToNode = entitiesVisitor->getProcToNodeMap();

    followsVisitor->setStmtNumberMap(stmtNumberMap);
    parentVisitor->setStmtNumberMap(stmtNumberMap);
    usesVisitor->setStmtNumberMap(stmtNumberMap);
    usesVisitor->setProcToNodeMap(procToNode);
    modifiesVisitor->setStmtNumberMap(stmtNumberMap);
    modifiesVisitor->setProcToNodeMap(procToNode);
    patternVisitor->setStmtNumberMap(stmtNumberMap);

    DesignExtractor::traverse(node, *followsVisitor);
    DesignExtractor::traverse(node, *parentVisitor);
    DesignExtractor::traverse(node, *usesVisitor);
    DesignExtractor::traverse(node, *modifiesVisitor);
    DesignExtractor::traverse(node, *patternVisitor);
    DesignExtractor::traverse(node, *callsVisitor);
    callsVisitor->populateCallsT();
}
