#ifndef STORAGE_READER_H
#define STORAGE_READER_H

#include <memory>
#include <string>
#include "program_knowledge_base/storage/StorageUtil.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include "common/Utils.h"


class StorageReader {
private:
    std::shared_ptr<ReadStorage> PKBStore;

public:
    explicit StorageReader(std::shared_ptr<ReadStorage> PKBStore_);

    std::unordered_set<std::string> getEntitiesFromPkb(EntityType entity);

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getFollowsMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getFollowsTMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getParentMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getParentTMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getFollowsReverseMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getParentReverseMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getModifiesMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getUsesMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getNextMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getNextTMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getCallsMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getCallsTMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::pair<std::string, std::shared_ptr<RelFactorNode>>, PairHash> getAllAssignPatternEntries();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getAllIfPatternEntries();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getAllWhilePatternEntries();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getReadVarNameMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getPrintVarNameMap();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getCallProcNameMap();

    bool isFollowsEmpty();

    bool isParentEmpty();
};

#endif // STORAGE_READER_H
