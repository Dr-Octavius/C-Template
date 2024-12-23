#include "QPSCache.h"
#include "../exception/QueryProcessorException.h"

QPSCache::QPSCache(std::shared_ptr<StorageReader> storageReader) : storageReader(storageReader) {}


std::unordered_set<std::string> QPSCache::unionSets(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2) {
    std::unordered_set<std::string> result;
    for (const auto& element : set1) {
        result.insert(element);
    }
    for (const auto& element : set2) {
        result.insert(element);
    }
    return result;
}

bool QPSCache::isCachedReachable(std::pair<std::string, EntityType> node) {
    for (const auto& pair : nextTMap) {
        if (pair.first == node) {
            return true;
        }
    }
    return false;
}

std::optional<std::unordered_set<std::pair<std::string, EntityType>, PairHash>> QPSCache::getReachable(std::pair<std::string, EntityType> node) {
    if (isCachedReachable(node)) {
        return nextTMap[node];
    }
    return std::nullopt;
}

void QPSCache::cacheReachable(std::pair<std::string, EntityType> node, std::unordered_set<std::pair<std::string, EntityType>, PairHash> reachable) {
    nextTMap[node] = reachable;
}

bool QPSCache::isCachedAffected(std::pair<std::string, EntityType> node) {
    for (const auto& pair : affectsMap) {
        if (pair.first == node) {
            return true;
        }
    }
    return false;
}

std::optional<std::unordered_set<std::pair<std::string, EntityType>, PairHash>> QPSCache::getAffected(std::pair<std::string, EntityType> node) {
    if (isCachedAffected(node)) {
        return affectsMap[node];
    }
    return std::nullopt;
}

void QPSCache::cacheAffected(std::pair<std::string, EntityType> node, std::unordered_set<std::pair<std::string, EntityType>, PairHash> affected) {
    affectsMap[node] = affected;
}