#include "PatternStorage.h"

#include <utility>

PatternStorage::PatternStorage(std::shared_ptr<WriteStorage> storage) {
    assignPatternManager = storage->getAssignPatternManager();
    ifPatternManager = storage->getIfPatternManager();
    whilePatternManager = storage->getWhilePatternManager();
}

void PatternStorage::insertAssignPattern(const std::pair<std::string, EntityType>& stmtNo, const std::shared_ptr<AssignNode>& assignNode) {
	assignPatternManager->insertPattern(stmtNo, assignNode);
}

void PatternStorage::insertIfPattern(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	ifPatternManager->insertRelationship(first, second);
}

void PatternStorage::insertWhilePattern(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) {
	whilePatternManager->insertRelationship(first, second);
}
