#ifndef READ_ONLY_ENTITIES_MANAGER_H
#define READ_ONLY_ENTITIES_MANAGER_H

#include <unordered_set>
#include <unordered_map>
#include <string>
#include "common/Utils.h"

class ReadOnlyEntitiesManager {
public:
    virtual bool isEmpty() = 0;
    virtual bool specificEntityIsEmpty(EntityType) = 0;
    virtual bool isPresent(std::string, EntityType) = 0;
    virtual bool entityTypeIsPresent(EntityType) = 0;
    virtual std::unordered_map<EntityType, std::unordered_set<std::string>> getAllEntityEntries() = 0;
    virtual std::unordered_set<std::string> getSpecificEntityTypeEntries(EntityType) = 0;
};

#endif // READ_ONLY_ENTITIES_MANAGER_H