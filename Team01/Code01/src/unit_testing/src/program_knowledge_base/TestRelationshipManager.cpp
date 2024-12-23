#include "../../include/catch.hpp"
#include "program_knowledge_base/relationship/RelationshipManager.h"

TEST_CASE("RelationshipManager isEmptyMap string,string") {
    RelationshipManager relationshipManager;
    REQUIRE(relationshipManager.isEmptyMap());
    REQUIRE(relationshipManager.insertRelationship({ "x", EntityType::STMT }, { "y",EntityType::STMT }));
    REQUIRE_FALSE(relationshipManager.isEmptyMap());
}

TEST_CASE("RelationshipManager hasMap string,string") {
    RelationshipManager relationshipManager;
    REQUIRE_FALSE(relationshipManager.hasMap({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE(relationshipManager.insertRelationship({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE_FALSE(relationshipManager.isEmptyMap());
}

TEST_CASE("RelationshipManager getAllRelationshipEntries string,string") {
    RelationshipManager relationshipManager;
    auto map = relationshipManager.getAllRelationshipEntries();
    REQUIRE(map.empty());
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> testTMap
        = { {{"a", EntityType::IF}, {{"x", EntityType::STMT}, {"y", EntityType::STMT}}} };
    REQUIRE(relationshipManager.insertRelationship({ "a", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(relationshipManager.insertRelationship({ "a", EntityType::IF }, { "y",  EntityType::STMT }));
    map = relationshipManager.getAllRelationshipEntries();
    auto set = relationshipManager.getAllRelationshipEntries()[{ "a", EntityType::IF }];
    REQUIRE(map.size() == 1);
    REQUIRE(set.size() == 2);
    REQUIRE(relationshipManager.getAllRelationshipEntries() == testTMap);
}

TEST_CASE("RelationshipManager insertRelationship string,string") {
    RelationshipManager relationshipManager;
    REQUIRE(relationshipManager.insertRelationship({ "a", EntityType::WHILE }, { "x",  EntityType::STMT }));
    REQUIRE(relationshipManager.insertRelationship({ "a", EntityType::WHILE }, { "y",  EntityType::STMT }));
    REQUIRE(relationshipManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    auto map = relationshipManager.getAllRelationshipEntries();
    REQUIRE(map.size() == 2);
    auto set = relationshipManager.getAllRelationshipEntries()[{ "a", EntityType::WHILE }];
    REQUIRE(set.size() == 2);
    REQUIRE(set.find({ "x",  EntityType::STMT }) != set.end());
    REQUIRE_FALSE(relationshipManager.isEmptyMap());

    REQUIRE_FALSE(relationshipManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(map.size() == 2);
}

