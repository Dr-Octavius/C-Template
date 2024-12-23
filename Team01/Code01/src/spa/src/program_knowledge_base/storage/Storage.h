#ifndef STORAGE_H
#define STORAGE_H

#include <memory>
#include <string>
#include "StorageUtil.h"
#include "program_knowledge_base/entity/EntitiesManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/FollowsTManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/ParentManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/ParentTManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/FollowsManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/ModifiesManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/UsesManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/NextManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/NextTManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/CallsManager.h"
#include "program_knowledge_base/relationship/relationship_child_managers/CallsTManager.h"
#include "program_knowledge_base/pattern/pattern_child_managers/AssignPatternManager.h"
#include "program_knowledge_base/pattern/pattern_child_managers/WhilePatternManager.h"
#include "program_knowledge_base/pattern/pattern_child_managers/IfPatternManager.h"
#include "program_knowledge_base/attributes/attributes_child_managers/CallProcNameManager.h"
#include "program_knowledge_base/attributes/attributes_child_managers/PrintVarNameManager.h"
#include "program_knowledge_base/attributes/attributes_child_managers/ReadVarNameManager.h"
 
template<typename Relationship_Read_Or_Write, typename Entity_Read_Or_Write, typename Pattern_Read_Or_Write>
class Storage {
private:
    std::shared_ptr<StorageUtil> storage;

public:
    explicit Storage(std::shared_ptr<StorageUtil> storageUtil) : storage(storageUtil) {}

    std::shared_ptr<Relationship_Read_Or_Write> getFollowsManager() {
        return storage->getFollowsManager();
    }

    std::shared_ptr<Relationship_Read_Or_Write> getFollowsTManager() {
        return storage->getFollowsTManager();
    }

    std::shared_ptr<Relationship_Read_Or_Write> getParentManager() {
        return storage->getParentManager();
    }

    std::shared_ptr<Relationship_Read_Or_Write> getParentTManager() {
        return storage->getParentTManager();
    }

    std::shared_ptr<Relationship_Read_Or_Write> getModifiesManager() {
		return storage->getModifiesManager();
	}

    std::shared_ptr<Relationship_Read_Or_Write> getUsesManager() {
        return storage->getUsesManager();
    }

    std::shared_ptr<Relationship_Read_Or_Write> getNextManager() {
		return storage->getNextManager();
	}

    std::shared_ptr<Relationship_Read_Or_Write> getNextTManager() {
        return storage->getNextTManager();
    }

    std::shared_ptr<Relationship_Read_Or_Write> getCallsManager() {
        return storage->getCallsManager();
    }

    std::shared_ptr<Relationship_Read_Or_Write> getCallsTManager() {
        return storage->getCallsTManager();
    }

    std::shared_ptr<Entity_Read_Or_Write> getEntitiesManager() {
		return storage->getEntitiesManager();
	}

    std::shared_ptr<Pattern_Read_Or_Write> getAssignPatternManager() {
		return storage->getAssignPatternManager();
	}

    std::shared_ptr<Relationship_Read_Or_Write> getWhilePatternManager() {
        return storage->getWhilePatternManager();
	}

	std::shared_ptr<Relationship_Read_Or_Write> getIfPatternManager() {
		return storage->getIfPatternManager();
	}
  
  std::shared_ptr<Relationship_Read_Or_Write> getCallProcNameManager() {
		return storage->getCallProcNameManager();
	}

  std::shared_ptr<Relationship_Read_Or_Write> getPrintVarNameManager() {
		return storage->getPrintVarNameManager();
	}

  std::shared_ptr<Relationship_Read_Or_Write> getReadVarNameManager() {
		return storage->getReadVarNameManager();
	}
};

#endif // STORAGE_H
