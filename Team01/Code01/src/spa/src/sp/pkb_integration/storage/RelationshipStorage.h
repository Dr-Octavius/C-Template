#ifndef RELATIONSHIP_STORAGE_H
#define RELATIONSHIP_STORAGE_H


#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "program_knowledge_base/storage/StorageManager.h"
#include "common/Utils.h"

class RelationshipStorage {
private:
    std::shared_ptr<WriteOnlyRelationshipManager> followsManager;
    std::shared_ptr<WriteOnlyRelationshipManager> followsTManager;

    std::shared_ptr<WriteOnlyRelationshipManager> parentsManager;
    std::shared_ptr<WriteOnlyRelationshipManager> parentsTManager;

    std::shared_ptr<WriteOnlyRelationshipManager> usesManager;
    std::shared_ptr<WriteOnlyRelationshipManager> modifiesManager;

    std::shared_ptr<WriteOnlyRelationshipManager> nextManager;
    std::shared_ptr<WriteOnlyRelationshipManager> nextTManager;

    std::shared_ptr<WriteOnlyRelationshipManager> callsManager;
    std::shared_ptr<WriteOnlyRelationshipManager> callsTManager;

    std::shared_ptr<WriteOnlyRelationshipManager> readVarNameManager;
    std::shared_ptr<WriteOnlyRelationshipManager> printVarNameManager;
    std::shared_ptr<WriteOnlyRelationshipManager> callProcNameManager;


public:
    explicit RelationshipStorage(const std::shared_ptr<WriteStorage>& writeStorage,
        const std::shared_ptr<ReadStorage>& readStorage);

    void insertFollowsRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertFollowsTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertParentsRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertParentsTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertUsesRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertModifiesRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertNextRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertNextTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertCallsRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertCallsTRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertReadVarNameRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertPrintVarNameRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertCallProcNameRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
};

#endif // RELATIONSHIP_STORAGE_H