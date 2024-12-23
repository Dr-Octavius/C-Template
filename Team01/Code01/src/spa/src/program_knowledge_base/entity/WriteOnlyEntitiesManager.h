#ifndef WRITE_ONLY_ENTITIES_MANAGER_H
#define WRITE_ONLY_ENTITIES_MANAGER_H

#include <string>
#include "common/Utils.h"

class WriteOnlyEntitiesManager {
public:
    virtual bool insertEntity(std::string entity, EntityType entityType) = 0;
};


#endif // WRITE_ONLY_ENTITIES_MANAGER_H