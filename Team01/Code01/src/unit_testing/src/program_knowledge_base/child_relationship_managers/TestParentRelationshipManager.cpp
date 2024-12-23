#include "../../include/catch.hpp"
#include "program_knowledge_base/relationship/relationship_child_managers/ParentManager.h"

TEST_CASE("ParentManager isEmptyMap string,string") {
    ParentManager parentManager;
    REQUIRE(parentManager.isEmptyMap());
    REQUIRE(parentManager.insertRelationship({ "x", EntityType::STMT }, { "y",EntityType::STMT }));
    REQUIRE_FALSE(parentManager.isEmptyMap());
}

TEST_CASE("ParentManager hasMap string,string") {
    ParentManager parentManager;
    REQUIRE_FALSE(parentManager.hasMap({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE(parentManager.insertRelationship({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE_FALSE(parentManager.isEmptyMap());
}

TEST_CASE("ParentManager getAllRelationshipEntries string,string") {
    ParentManager parentManager;
    auto map = parentManager.getAllRelationshipEntries();
    REQUIRE(map.empty());
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> testMap
        = { {{"a", EntityType::IF}, {{"x", EntityType::STMT}, {"y", EntityType::STMT}}} };
    REQUIRE(parentManager.insertRelationship({ "a", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(parentManager.insertRelationship({ "a", EntityType::IF }, { "y",  EntityType::STMT }));
    map = parentManager.getAllRelationshipEntries();
    auto set = parentManager.getAllRelationshipEntries()[{ "a", EntityType::IF }];
    REQUIRE(map.size() == 1);
    REQUIRE(set.size() == 2);
    REQUIRE(parentManager.getAllRelationshipEntries() == testMap);
}

TEST_CASE("ParentManager insertRelationship string,string") {
    ParentManager parentManager;
    REQUIRE(parentManager.insertRelationship({ "a", EntityType::WHILE }, { "x",  EntityType::STMT }));
    REQUIRE(parentManager.insertRelationship({ "a", EntityType::WHILE }, { "y",  EntityType::STMT }));
    REQUIRE(parentManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    auto map = parentManager.getAllRelationshipEntries();
    REQUIRE(map.size() == 2);
    auto set = parentManager.getAllRelationshipEntries()[{ "a", EntityType::WHILE }];
    REQUIRE(set.size() == 2);
    REQUIRE(set.find({ "x",  EntityType::STMT }) != set.end());
    REQUIRE_FALSE(parentManager.isEmptyMap());

    REQUIRE_FALSE(parentManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(map.size() == 2);
}

