#ifndef ENTITY_STORAGE_H
#define ENTITY_STORAGE_H

#include <vector>
#include <memory>
#include <string>
#include "program_knowledge_base/storage/StorageManager.h"
#include "common/Utils.h"

class EntityStorage {
private:
    std::shared_ptr<WriteOnlyEntitiesManager> entitiesManager;

public:
    explicit EntityStorage(std::shared_ptr<WriteStorage> storage);

    void insertEntity(const std::string& entity, const EntityType& entityType);
};

#endif // ENTITY_STORAGE_H