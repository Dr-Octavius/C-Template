#ifndef PATTERN_STORAGE_H
#define PATTERN_STORAGE_H

#include <vector>
#include <memory>
#include <string>
#include "program_knowledge_base/storage/StorageManager.h"
#include "common/Utils.h"

class PatternStorage {
private:
    std::shared_ptr<WriteOnlyPatternManager> assignPatternManager;
    std::shared_ptr<WriteOnlyRelationshipManager> ifPatternManager;
    std::shared_ptr<WriteOnlyRelationshipManager> whilePatternManager;

public:
    explicit PatternStorage(std::shared_ptr<WriteStorage> storage);

    void insertAssignPattern(const std::pair<std::string, EntityType>& stmtNo, const std::shared_ptr<AssignNode>& assignNode);
    void insertIfPattern(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
    void insertWhilePattern(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second);
};

#endif // PATTERN_STORAGE_H 