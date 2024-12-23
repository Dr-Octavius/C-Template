#include "../../include/catch.hpp"
#include "program_knowledge_base/entity/EntitiesManager.h"

//To do, a set of test cases for each entity type
TEST_CASE("EntityManager insertEntity string") {
    EntitiesManager entitiesManager;

    // reject repeated entry
    REQUIRE(entitiesManager.insertEntity("x", EntityType::ASSIGN));
    REQUIRE_FALSE(entitiesManager.insertEntity("x", EntityType::ASSIGN));

    // insert new
    REQUIRE(entitiesManager.insertEntity("flag", EntityType::CALL));
}

TEST_CASE("EntityManager isEmpty string") {
    EntitiesManager entitiesManager;
    REQUIRE(entitiesManager.isEmpty());
    REQUIRE(entitiesManager.insertEntity("x", EntityType::PROCEDURE));
    REQUIRE_FALSE(entitiesManager.isEmpty());
}

TEST_CASE("EntityManager isPresent string") {
    EntitiesManager entitiesManager;
    REQUIRE_FALSE(entitiesManager.isPresent("x", EntityType::PRINT));
    REQUIRE(entitiesManager.insertEntity("x", EntityType::PRINT));
    REQUIRE(entitiesManager.isPresent("x", EntityType::PRINT));
}

TEST_CASE("EntityManager getAllEntitiesEntries string") {
    EntitiesManager entitiesManager;
    auto newSet = entitiesManager.getAllEntityEntries();
    REQUIRE(newSet.empty());
    std::unordered_map<EntityType, std::unordered_set<std::string>> entities_map = { {EntityType::PRINT, {"x", "radius"}}};
    std::unordered_map<EntityType, std::unordered_set<std::string>> emptyMap = {};
    REQUIRE(entitiesManager.insertEntity("x", EntityType::PRINT));
    REQUIRE(entitiesManager.insertEntity("radius", EntityType::PRINT));
    newSet = entitiesManager.getAllEntityEntries();
    REQUIRE(newSet.size() == 1);
    REQUIRE(entitiesManager.getAllEntityEntries() == entities_map);
    REQUIRE_FALSE(entitiesManager.getAllEntityEntries() == emptyMap);
}