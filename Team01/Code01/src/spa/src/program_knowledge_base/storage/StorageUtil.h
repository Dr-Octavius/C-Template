#ifndef STORAGE_UTIL_H
#define STORAGE_UTIL_H

#include <memory>
#include <string>
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
#include "program_knowledge_base/pattern/pattern_child_managers/IfPatternManager.h"
#include "program_knowledge_base/pattern/pattern_child_managers/WhilePatternManager.h"
#include "program_knowledge_base/attributes/attributes_child_managers/CallProcNameManager.h"
#include "program_knowledge_base/attributes/attributes_child_managers/PrintVarNameManager.h"
#include "program_knowledge_base/attributes/attributes_child_managers/ReadVarNameManager.h"

class StorageUtil {
private:
    // Relationship managers
    std::shared_ptr<FollowsManager> followsManager;
    std::shared_ptr<FollowsTManager> followsTManager;
    std::shared_ptr<ParentManager> parentManager;
    std::shared_ptr<ParentTManager> parentTManager;
    std::shared_ptr<ModifiesManager> modifiesManager;
    std::shared_ptr<UsesManager> usesManager;
    std::shared_ptr<NextManager> nextManager;
    std::shared_ptr<NextTManager> nextTManager;
    std::shared_ptr<CallsManager> callsManager;
    std::shared_ptr<CallsTManager> callsTManager;

    // Pattern managers that look more like relationship managers
    std::shared_ptr<IfPatternManager> ifPatternManager;
    std::shared_ptr<WhilePatternManager> whilePatternManager;

    // Entities manager
    std::shared_ptr<EntitiesManager> entitiesManager;

    //Pattern  manager
    std::shared_ptr<AssignPatternManager> assignPatternManager;

    // Attributes managers
    std::shared_ptr<CallProcNameManager> callProcNameManager;
    std::shared_ptr<PrintVarNameManager> printVarNameManager;
    std::shared_ptr<ReadVarNameManager> readVarNameManager;
public:
    StorageUtil() {
        followsManager = std::make_shared<FollowsManager>();
        followsTManager = std::make_shared<FollowsTManager>();
        parentManager = std::make_shared<ParentManager>();
        parentTManager = std::make_shared<ParentTManager>();
        modifiesManager = std::make_shared<ModifiesManager>();
        usesManager = std::make_shared<UsesManager>();
        nextManager = std::make_shared<NextManager>();
        nextTManager = std::make_shared<NextTManager>();
        callsManager = std::make_shared<CallsManager>();
        callsTManager = std::make_shared<CallsTManager>();

        ifPatternManager = std::make_shared<IfPatternManager>();
		whilePatternManager = std::make_shared<WhilePatternManager>();

        entitiesManager = std::make_shared<EntitiesManager>();

        assignPatternManager = std::make_shared<AssignPatternManager>();
      
        callProcNameManager = std::make_shared<CallProcNameManager>();
        printVarNameManager = std::make_shared<PrintVarNameManager>();
        readVarNameManager = std::make_shared<ReadVarNameManager>();
    }

    std::shared_ptr<FollowsManager> getFollowsManager() {
        return followsManager;
    }

    std::shared_ptr<FollowsTManager> getFollowsTManager() {
        return followsTManager;
    }

    std::shared_ptr<ParentManager> getParentManager() {
        return parentManager;
    }

    std::shared_ptr<ParentTManager> getParentTManager() {
        return parentTManager;
    }

    std::shared_ptr<ModifiesManager> getModifiesManager() {
	    return modifiesManager;
	}

    std::shared_ptr<UsesManager> getUsesManager() {
        return usesManager;
    }

    std::shared_ptr<NextManager> getNextManager() {
		return nextManager;
	}

    std::shared_ptr<NextTManager> getNextTManager() {
        return nextTManager;
    }

    std::shared_ptr<CallsManager> getCallsManager() {
        return callsManager;
    }

    std::shared_ptr<CallsTManager> getCallsTManager() {
        return callsTManager;
    }

    std::shared_ptr<EntitiesManager> getEntitiesManager() {
        return entitiesManager;
    }

    std::shared_ptr<AssignPatternManager> getAssignPatternManager() {
		return assignPatternManager;
	}

    std::shared_ptr<IfPatternManager> getIfPatternManager() {
		return ifPatternManager;
	}

    std::shared_ptr<WhilePatternManager> getWhilePatternManager() {
        return whilePatternManager;
    }

    std::shared_ptr<CallProcNameManager> getCallProcNameManager() {
		return callProcNameManager;
	}

    std::shared_ptr<PrintVarNameManager> getPrintVarNameManager() {
        return printVarNameManager;
    }

    std::shared_ptr<ReadVarNameManager> getReadVarNameManager() {
		return readVarNameManager;
	}

    static std::unordered_map<EntityType, std::unordered_set<std::string>> getAllEntityValues(std::shared_ptr<ReadOnlyEntitiesManager> entitiesManager) {
        return entitiesManager->getAllEntityEntries();
    }

    static std::unordered_set<std::string> getSpecificEntityValues(std::shared_ptr<ReadOnlyEntitiesManager> entitiesManager, EntityType entityType) {
		return entitiesManager->getSpecificEntityTypeEntries(entityType);
	}

    static bool isEntityEmpty(std::shared_ptr<ReadOnlyEntitiesManager> entitiesManager) {
        return entitiesManager->isEmpty();
    }

    static bool isSpecificEntityEmpty(std::shared_ptr<ReadOnlyEntitiesManager> entitiesManager, EntityType entityType) {
        return entitiesManager->specificEntityIsEmpty(entityType);
    }

    static std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getRelationshipMap(std::shared_ptr<ReadOnlyRelationshipManager> relationshipManager) {
		return relationshipManager->getAllRelationshipEntries();
	}

    static std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> getReverseRelationshipMap(std::shared_ptr<ReadOnlyRelationshipManager> relationshipManager) {
		return relationshipManager->getAllReversedRelationshipEntries();
	}

    static bool isRelationEmpty(std::shared_ptr<ReadOnlyRelationshipManager> relationshipManager) {
		return relationshipManager->isEmptyMap();
	}

    static bool isReversedRelationEmpty(std::shared_ptr<ReadOnlyRelationshipManager> relationshipManager) {
		return relationshipManager->isEmptyReversedMap();
	}

    static std::unordered_map<std::pair<std::string, EntityType>,
        std::pair<std::string, std::shared_ptr<RelFactorNode>>,
        PairHash> getPatternMap(std::shared_ptr<ReadOnlyPatternManager> assignPatternManager) {
		return assignPatternManager->getAllPatternEntries();
	}
};

#endif // STORAGE_UTIL_H
