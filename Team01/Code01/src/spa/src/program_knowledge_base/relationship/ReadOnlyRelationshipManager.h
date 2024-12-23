#ifndef READ_ONLY_RELATIONSHIP_MANAGER_H
#define READ_ONLY_RELATIONSHIP_MANAGER_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <utility>
#include "common/Utils.h"

class ReadOnlyRelationshipManager {
public:
    virtual bool isEmptyMap() = 0;
    virtual bool hasMap(const std::pair<std::string, EntityType>&, const std::pair<std::string, EntityType>&) = 0;
    virtual bool isEmptyReversedMap() = 0;
    virtual bool hasReversedMap(const std::pair<std::string, EntityType>&, const std::pair<std::string, EntityType>&) = 0;
    virtual std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getAllRelationshipEntries() = 0;
    virtual std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getAllReversedRelationshipEntries() = 0;
};

#endif // READ_ONLY_RELATIONSHIP_MANAGER_H
