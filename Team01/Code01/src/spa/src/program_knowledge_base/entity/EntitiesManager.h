#ifndef ENTITIES_MANAGER_H
#define ENTITIES_MANAGER_H

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include "WriteOnlyEntitiesManager.h"
#include "ReadOnlyEntitiesManager.h"

class EntitiesManager : 
    public WriteOnlyEntitiesManager,
    public ReadOnlyEntitiesManager {
private:
    std::unordered_map<EntityType, std::unordered_set<std::string>> entities_map;

public:
    EntitiesManager(size_t reserveSize = 10) {
        if (reserveSize > 0) {
            entities_map.reserve(reserveSize);
        }
    }

    /**
    * @brief Insert Entity
    * 
    * @param entity The entity to be inserted
    * @return is_success True if insertion was a success
    */
    bool insertEntity(const std::string entity, const EntityType entityType) override {
//        std::cout << "PKB: Insert entity" << entity << std::endl;
        auto& set = entities_map[entityType]; // Get the set for the entityType

        auto result = set.insert(entity);
        return result.second;
    }

   /**
   * @brief Checks if the set is the Entity Manager contains is empty
   *
   * @return is_empty True if set is empty
   */
    bool isEmpty() override {
        bool is_empty = true;
        if (entities_map.size() != 0) {
            is_empty = false;
        }
        return is_empty;
    }

    bool specificEntityIsEmpty(const EntityType entityType) override {
        auto it = entities_map.find(entityType); // Find the entityType in the map
        if (it != entities_map.end()) { // If the entityType was found
            if (it->second.empty()) { // Check if the set is empty
                // The set exists but is empty
                return true;
            }
            else {
                // The set exists and is not empty
                return false;
            }
        }
        return true;
    }

    /**
    * @brief Checks if the entity is present in the set
    * 
    * @params entity The entity in question
    * @return is+present True if entity is present in set
    */
    bool isPresent(const std::string entity, const EntityType entityType) override {
        auto it = entities_map.find(entityType); // Attempt to find entityType in the map
        if (it != entities_map.end()) { // If found
            return it->second.count(entity) > 0; // Check if entity is in the set
        }
        return false; // entityType not found, or entity not in the set
    }

    bool entityTypeIsPresent(const EntityType entityType) override {
        bool is_present = entities_map.count(entityType) > 0;
        return is_present;
    }

    std::unordered_map<EntityType, std::unordered_set<std::string>> getAllEntityEntries() override {
        return entities_map;
    }

    std::unordered_set<std::string> getSpecificEntityTypeEntries(const EntityType entityType) override {
		return entities_map[entityType];
	}
};

#endif // ENTITIES_MANAGER_H
