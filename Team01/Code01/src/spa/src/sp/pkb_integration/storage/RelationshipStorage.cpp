#include "RelationshipStorage.h"
#include <utility>
#include <string>

RelationshipStorage::RelationshipStorage(const std::shared_ptr<WriteStorage>& writeStorage,
    const std::shared_ptr<ReadStorage>& readStorage) {
    followsManager = writeStorage->getFollowsManager();
    followsTManager = writeStorage->getFollowsTManager();

    parentsManager = writeStorage->getParentManager();
    parentsTManager = writeStorage->getParentTManager();

    modifiesManager = writeStorage->getModifiesManager();
    usesManager = writeStorage->getUsesManager();

    callsManager = writeStorage->getCallsManager();
    callsTManager = writeStorage->getCallsTManager();

    nextManager = writeStorage->getNextManager();
    nextTManager = writeStorage->getNextTManager();

    readVarNameManager = writeStorage->getReadVarNameManager();
    printVarNameManager = writeStorage->getPrintVarNameManager();
    callProcNameManager = writeStorage->getCallProcNameManager();
}

void RelationshipStorage::insertFollowsRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
    followsManager->insertRelationship(first, second);
}

void RelationshipStorage::insertFollowsTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
    followsTManager->insertRelationship(first, second);
}

void RelationshipStorage::insertParentsRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
    parentsManager->insertRelationship(first, second);
}

void RelationshipStorage::insertParentsTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
    parentsTManager->insertRelationship(first, second);
}

void RelationshipStorage::insertModifiesRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	modifiesManager->insertRelationship(first, second);
}

void RelationshipStorage::insertUsesRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	usesManager->insertRelationship(first, second);
}

void RelationshipStorage::insertNextRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	nextManager->insertRelationship(first, second);
}

void RelationshipStorage::insertNextTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	nextTManager->insertRelationship(first, second);
}

void RelationshipStorage::insertCallsRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	callsManager->insertRelationship(first, second);
}

void RelationshipStorage::insertCallsTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
    callsTManager->insertRelationship(first, second);
}

void RelationshipStorage::insertReadVarNameRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	readVarNameManager->insertRelationship(first, second);
}

void RelationshipStorage::insertPrintVarNameRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	printVarNameManager->insertRelationship(first, second);
}

void RelationshipStorage::insertCallProcNameRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	callProcNameManager->insertRelationship(first, second);
}
