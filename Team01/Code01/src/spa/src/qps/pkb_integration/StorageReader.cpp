#include "StorageReader.h"

StorageReader::StorageReader(std::shared_ptr<ReadStorage> PKBStore_)
    : PKBStore(PKBStore_) {}

std::unordered_set<std::string> StorageReader::getEntitiesFromPkb(EntityType entityType) {
    return StorageUtil::getSpecificEntityValues(PKBStore->getEntitiesManager(), entityType);
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getFollowsMap() {
    return StorageUtil::getRelationshipMap(PKBStore->getFollowsManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getParentMap() {
    return StorageUtil::getRelationshipMap(PKBStore->getParentManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getFollowsTMap() {
    return StorageUtil::getRelationshipMap(PKBStore->getFollowsTManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getParentTMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getParentTManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getFollowsReverseMap() {
    return StorageUtil::getReverseRelationshipMap(PKBStore->getFollowsManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getParentReverseMap() {
    return StorageUtil::getReverseRelationshipMap(PKBStore->getParentManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getModifiesMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getModifiesManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getUsesMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getUsesManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getNextMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getNextManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getNextTMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getNextTManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getCallsMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getCallsManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getCallsTMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getCallsTManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::pair<std::string, std::shared_ptr<RelFactorNode>>, PairHash> StorageReader::getAllAssignPatternEntries() {
	return StorageUtil::getPatternMap(PKBStore->getAssignPatternManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getAllIfPatternEntries() {
	return StorageUtil::getRelationshipMap(PKBStore->getIfPatternManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getAllWhilePatternEntries() {
	return StorageUtil::getRelationshipMap(PKBStore->getWhilePatternManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getPrintVarNameMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getPrintVarNameManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getReadVarNameMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getReadVarNameManager());
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> StorageReader::getCallProcNameMap() {
	return StorageUtil::getRelationshipMap(PKBStore->getCallProcNameManager());
}

bool StorageReader::isFollowsEmpty() {
    return StorageUtil::isRelationEmpty(PKBStore->getFollowsManager());
}

bool StorageReader::isParentEmpty() {
    return StorageUtil::isRelationEmpty(PKBStore->getParentManager());
}
