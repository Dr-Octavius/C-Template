#pragma once

#include <unordered_set>
#include "common/Utils.h"
#include "qps/pkb_integration/StorageReader.h"
#include <algorithm>
#include <optional>

class QPSCache {
private:
    std::shared_ptr<StorageReader> storageReader;

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> nextTMap;
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> affectsMap;

    std::unordered_set<std::string> unionSets(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2);

    // Helpers
    bool isCachedReachable(std::pair<std::string, EntityType> node);
    bool isCachedAffected(std::pair<std::string, EntityType> node);


public:
    explicit QPSCache(std::shared_ptr<StorageReader> storageReader);

    std::optional<std::unordered_set<std::pair<std::string, EntityType>, PairHash>> getReachable(std::pair<std::string, EntityType> node);
    void cacheReachable(std::pair<std::string, EntityType> node, std::unordered_set<std::pair<std::string, EntityType>, PairHash> reachable);

    std::optional<std::unordered_set<std::pair<std::string, EntityType>, PairHash>> getAffected(std::pair<std::string, EntityType> node);
    void cacheAffected(std::pair<std::string, EntityType> node, std::unordered_set<std::pair<std::string, EntityType>, PairHash> affected);

};
