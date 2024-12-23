#ifndef RELATIONSHIP_MANAGER_H
#define RELATIONSHIP_MANAGER_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "ReadOnlyRelationshipManager.h"
#include "WriteOnlyRelationshipManager.h"
#include "string"
#include "common/Utils.h"

class RelationshipManager : public ReadOnlyRelationshipManager,
    public WriteOnlyRelationshipManager {
protected:
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> relationships_map;
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> reversed_relationships_map;
public:
    /**
     * @brief Checks if relationship map is empty
     * 
     * @ return boolean whether or not map is empty
     */
    bool isEmptyMap() override {
        return relationships_map.empty();
    }

    /**
     * @brief Checks if there's an existing map between the 2 params
     * 
     * @param first
     * @param second
     * 
     * @return boolean if parameters are mapped to each other
     */
    bool hasMap(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) override {
        auto key = relationships_map.find(first);
        return key != relationships_map.end() && key->second.count(second) > 0;
    }

    /**
     * @brief Returns the entire relationship map
     * 
     * @return relationships_map
     */
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getAllRelationshipEntries() override {
        return relationships_map;
    }

    /**
     * @brief Checks if reversed relationship map is empty
     *
     * @ return boolean whether or not map is empty
     */
    bool isEmptyReversedMap() override {
        return reversed_relationships_map.empty();
    }

    /**
    * @brief Checks if there's an existing map between the 2 params in the reversed
    * map
    *
    * @param first The first pair to check in the reversed mapping
    * @param second The second pair that should map to the first pair in the reversed mapping
    *
    * @return boolean indicating if the first parameter is mapped to the second parameter in the reversed map
    */
    bool hasReversedMap(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) override {
        auto key = reversed_relationships_map.find(second); // Find the 'second' pair in the reversed map
        if (key != reversed_relationships_map.end()) {
            return key->second.count(first) > 0; // Check if 'first' is associated with 'second' in the reversed map
        }
        return false; // 'second' not found, meaning no reverse mapping exists for 'first'
    }

    /**
     * @brief Returns the entire reversed relationship map
     *
     * @return relationships_map
     */
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getAllReversedRelationshipEntries() override {
        return reversed_relationships_map;
    }

    /**
     * @brief Inserts the inputs given into the map, and maps it to the second param
     * 
     * @param first
     * @param second
     * 
     * @return insert and recursive insert success True if insertion(s) was a success
     */
    bool insertRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) override {
        // Directly insert the second value into the set mapped by 'first'.
        // If 'first' does not exist, it will be default-constructed (empty set) before insertion.
        auto& set = relationships_map[first];
        return set.insert(second).second; // The 'insert' method returns a pair, where the second element is a bool indicating success.
    }

};

#endif // RELATIONSHIP_MANAGER_H
