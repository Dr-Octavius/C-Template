#include "StorageWriter.h"

StorageWriter::StorageWriter(const std::shared_ptr<WriteStorage>& writeStorage, const std::shared_ptr<ReadStorage>& readStorage) {
    entityStorage = std::make_shared<EntityStorage>(writeStorage);
    relationshipStorage = std::make_shared<RelationshipStorage>(writeStorage, readStorage);
    patternStorage = std::make_shared<PatternStorage> (writeStorage);
}

std::shared_ptr<EntityStorage> StorageWriter::getEntityStorage() {
    return entityStorage;
}

std::shared_ptr<RelationshipStorage> StorageWriter::getRelationshipStorage() {
    return relationshipStorage;
}

std::shared_ptr<PatternStorage> StorageWriter::getPatternStorage() {
	return patternStorage;
}

