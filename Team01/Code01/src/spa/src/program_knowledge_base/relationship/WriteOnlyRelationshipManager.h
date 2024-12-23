#ifndef WRITE_ONLY_RELATIONSHIP_MANAGER_H
#define WRITE_ONLY_RELATIONSHIP_MANAGER_H

#include <memory>
#include <string>
#include <utility>
#include "common/Utils.h"

class WriteOnlyRelationshipManager {
public:
    virtual bool insertRelationship(const std::pair<std::string, EntityType>& first, const std::pair<std::string, EntityType>& second) = 0;
};

#endif // WRITE_ONLY_RELATIONSHIP_MANAGER_H
