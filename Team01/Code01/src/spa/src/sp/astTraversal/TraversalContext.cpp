//
// Created by Michael on 25/3/24.
//

#include "TraversalContext.h"

void TraversalContext::annotateProcedureCounts(const std::string& name) {
    // Add the procedure name to the set
    ++procedureCounts[name];
    // When a new procedure is added, it becomes the current procedure.
    setCurrentProcedure(name);
}

void TraversalContext::annotateCallGraph(const std::string& caller, const std::string& callee) {
    // Add the callee to the list of calls for the caller in the call graph
    callGraph[caller].push_back(callee);
}

void TraversalContext::setCurrentProcedure(const std::string& name) {
    currentProcedure = name;
}

const std::string& TraversalContext::getCurrentProcedure() {
    return currentProcedure;
}

std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> TraversalContext::getNextMap() {
    return nextMap;
}

void TraversalContext::linkCFGNodes(std::shared_ptr<CFGNode> parent, std::shared_ptr<CFGNode> child) {
    parent->addSuccessor(child);
}

void TraversalContext::linkCompressedCFGNodes(std::shared_ptr<CompressedCFGNode> parent, std::shared_ptr<CompressedCFGNode> child) {
    parent->addSuccessor(child);
}

void TraversalContext::insertToNextMap(std::pair<std::string, EntityType> key, std::pair<std::string, EntityType> value) {
    if (key.second == EntityType::IF) {
        if (nextMap[key].size() < 2) {
            nextMap[key].insert(value);
        }
        return;
    }
    nextMap[key].insert(value);
}

std::vector<std::shared_ptr<CFGNode>> TraversalContext::getDeepestNode(std::shared_ptr<CFGNode> node, std::vector<std::shared_ptr<CFGNode>> lastNodes) {
    if (node->successors.empty()) {
        lastNodes.push_back(node);
        return lastNodes;
    }
    if (node->getEntityType() == EntityType::WHILE) {
        if (node->successors.size() > 1) {
            return getDeepestNode(node->successors[1],lastNodes);
        } else {
            lastNodes.push_back(node);
            return lastNodes;
        }
    }
    if (node->getEntityType() == EntityType::IF) {
        if (node->successors.size() > 2) {
            return getDeepestNode(node->successors[2],lastNodes);
        } else {
            std::vector<std::shared_ptr<CFGNode>> vec1 = getDeepestNode(node->successors[0],lastNodes);
            std::vector<std::shared_ptr<CFGNode>> vec2 = getDeepestNode(node->successors[1],lastNodes);
            lastNodes.insert(lastNodes.end(), vec1.begin(), vec1.end());
            lastNodes.insert(lastNodes.end(), vec2.begin(), vec2.end());
            return lastNodes;
        }
    }
    return getDeepestNode(node->successors[0],lastNodes);
}

void TraversalContext::insertRelationshipRecords(
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

void TraversalContext::writeToPkb(std::shared_ptr<StorageManager> pkbStorageManager) {
    std::shared_ptr<StorageWriter> storageWriter = std::make_shared<StorageWriter>(pkbStorageManager->getWriteStorage(), pkbStorageManager->getReadStorage());
    std::shared_ptr<RelationshipStorage> relationshipStorage = storageWriter->getRelationshipStorage();

    for (const auto& pair : nextMap) {
        for (const auto& innerPair : pair.second) {
            relationshipStorage->insertNextRelationship(pair.first, innerPair);
        }
    }

    // insertRelationshipRecords(nextMap, &RelationshipStorage::insertNextRelationship, relationshipStorage);
}