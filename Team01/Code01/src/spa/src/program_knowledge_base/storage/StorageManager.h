#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <vector>
#include <memory>
#include "Storage.h"
#include "program_knowledge_base/relationship/WriteOnlyRelationshipManager.h"
#include "program_knowledge_base/entity/WriteOnlyEntitiesManager.h"
#include "program_knowledge_base/relationship/ReadOnlyRelationshipManager.h"
#include "program_knowledge_base/entity/ReadOnlyEntitiesManager.h"
#include "program_knowledge_base/pattern/ReadOnlyPatternManager.h"
#include "program_knowledge_base/pattern/WriteOnlyPatternManager.h"

using WriteStorage = Storage<WriteOnlyRelationshipManager, WriteOnlyEntitiesManager, WriteOnlyPatternManager>;
using ReadStorage = Storage<ReadOnlyRelationshipManager, ReadOnlyEntitiesManager, ReadOnlyPatternManager>;

class StorageManager {
private:
    std::shared_ptr<StorageUtil> storageUtil;
public:
    StorageManager() {
        storageUtil = std::make_shared<StorageUtil>();
    }

    std::shared_ptr<WriteStorage> getWriteStorage() {
        return std::make_shared<WriteStorage>(storageUtil);
    }

    std::shared_ptr<ReadStorage> getReadStorage() {
        return std::make_shared<ReadStorage>(storageUtil);
    }
};

#endif // STORAGE_MANAGER_H
