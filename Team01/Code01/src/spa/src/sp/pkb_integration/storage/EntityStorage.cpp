#include "EntityStorage.h"

#include <utility>

EntityStorage::EntityStorage(std::shared_ptr<WriteStorage> storage) {
    entitiesManager = storage->getEntitiesManager();
}

void EntityStorage::insertEntity(const std::string& entity, const EntityType& entityType) {
    entitiesManager->insertEntity(entity, entityType);
}