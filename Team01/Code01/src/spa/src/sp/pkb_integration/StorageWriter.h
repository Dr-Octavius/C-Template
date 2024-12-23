#ifndef STORAGE_WRITER_H
#define STORAGE_WRITER_H

#include <vector>
#include <memory>
#include "sp/pkb_integration/storage/EntityStorage.h"
#include "sp/pkb_integration/storage/RelationshipStorage.h"
#include "sp/pkb_integration/storage/PatternStorage.h"
#include "program_knowledge_base/storage/StorageManager.h"


class StorageWriter {
private:
    std::shared_ptr<EntityStorage> entityStorage;
    std::shared_ptr<RelationshipStorage> relationshipStorage;
    std::shared_ptr<PatternStorage> patternStorage;

public:
    explicit StorageWriter(const std::shared_ptr<WriteStorage>& writeStorage, const std::shared_ptr<ReadStorage>& readStorage);

    std::shared_ptr<EntityStorage> getEntityStorage();
    std::shared_ptr<RelationshipStorage> getRelationshipStorage();
    std::shared_ptr<PatternStorage> getPatternStorage();
};

#endif // STORAGE_WRITER_H
