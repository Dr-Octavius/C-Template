
#include "../../include/catch.hpp"
#include "program_knowledge_base/relationship/relationship_child_managers/FollowsTManager.h"

TEST_CASE("FollowsTManager isEmptyMap string,string") {
    FollowsTManager followsTManager;
    REQUIRE(followsTManager.isEmptyMap());
    REQUIRE(followsTManager.insertRelationship({ "x", EntityType::STMT }, { "y",EntityType::STMT }));
    REQUIRE_FALSE(followsTManager.isEmptyMap());
}

TEST_CASE("FollowsTManager hasMap string,string") {
    FollowsTManager followsTManager;
    REQUIRE_FALSE(followsTManager.hasMap({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE(followsTManager.insertRelationship({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE_FALSE(followsTManager.isEmptyMap());
}

TEST_CASE("FollowsTManager getAllRelationshipEntries string,string") {
    FollowsTManager followsTManager;
    auto map = followsTManager.getAllRelationshipEntries();
    REQUIRE(map.empty());
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> testMap
        = { {{"a", EntityType::IF}, {{"x", EntityType::STMT}, {"y", EntityType::STMT}}} };
    REQUIRE(followsTManager.insertRelationship({ "a", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(followsTManager.insertRelationship({ "a", EntityType::IF }, { "y",  EntityType::STMT }));
    map = followsTManager.getAllRelationshipEntries();
    auto set = followsTManager.getAllRelationshipEntries()[{ "a", EntityType::IF }];
    REQUIRE(map.size() == 1);
    REQUIRE(set.size() == 2);
    REQUIRE(followsTManager.getAllRelationshipEntries() == testMap);
}

TEST_CASE("FollowsTManager insertRelationship string,string") {
    FollowsTManager followsTManager;
    REQUIRE(followsTManager.insertRelationship({ "a", EntityType::WHILE }, { "x",  EntityType::STMT }));
    REQUIRE(followsTManager.insertRelationship({ "a", EntityType::WHILE }, { "y",  EntityType::STMT }));
    REQUIRE(followsTManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    auto map = followsTManager.getAllRelationshipEntries();
    REQUIRE(map.size() == 2);
    auto set = followsTManager.getAllRelationshipEntries()[{ "a", EntityType::WHILE }];
    REQUIRE(set.size() == 2);
    REQUIRE(set.find({ "x",  EntityType::STMT }) != set.end());
    REQUIRE_FALSE(followsTManager.isEmptyMap());

    REQUIRE_FALSE(followsTManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(map.size() == 2);
}
