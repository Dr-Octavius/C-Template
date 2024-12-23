#include "../../include/catch.hpp"
#include "program_knowledge_base/relationship/relationship_child_managers/ParentTManager.h"

TEST_CASE("ParentTManager isEmptyMap string,string") {
    ParentTManager parentTManager;
    REQUIRE(parentTManager.isEmptyMap());
    REQUIRE(parentTManager.insertRelationship({"x", EntityType::STMT}, {"y",EntityType::STMT }));
    REQUIRE_FALSE(parentTManager.isEmptyMap());
}

TEST_CASE("ParentTManager hasMap string,string") {
    ParentTManager parentTManager;
    REQUIRE_FALSE(parentTManager.hasMap({ "x", EntityType::IF }, {"y", EntityType::STMT }));
    REQUIRE(parentTManager.insertRelationship({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE_FALSE(parentTManager.isEmptyMap());
}

TEST_CASE("ParentTManager getAllRelationshipEntries string,string") {
    ParentTManager parentTManager;
    auto map = parentTManager.getAllRelationshipEntries();
    REQUIRE(map.empty());
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> testTMap
        = { {{"a", EntityType::IF}, {{"x", EntityType::STMT}, {"y", EntityType::STMT}}} };
    REQUIRE(parentTManager.insertRelationship({ "a", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(parentTManager.insertRelationship({ "a", EntityType::IF }, { "y",  EntityType::STMT }));
    map = parentTManager.getAllRelationshipEntries();
    auto set = parentTManager.getAllRelationshipEntries()[{ "a", EntityType::IF }];
    REQUIRE(map.size() == 1);
    REQUIRE(set.size() == 2);
    REQUIRE(parentTManager.getAllRelationshipEntries() == testTMap);
}

TEST_CASE("ParentTManager insertRelationship string,string") {
    ParentTManager parentTManager;
    REQUIRE(parentTManager.insertRelationship({ "a", EntityType::WHILE }, { "x",  EntityType::STMT }));
    REQUIRE(parentTManager.insertRelationship({ "a", EntityType::WHILE }, { "y",  EntityType::STMT }));
    REQUIRE(parentTManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    auto map = parentTManager.getAllRelationshipEntries();
    REQUIRE(map.size() == 2);
    auto set = parentTManager.getAllRelationshipEntries()[{ "a", EntityType::WHILE }];
    REQUIRE(set.size() == 2);
    REQUIRE(set.find({ "x",  EntityType::STMT }) != set.end());
    REQUIRE_FALSE(parentTManager.isEmptyMap());

    REQUIRE_FALSE(parentTManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(map.size() == 2);
}

