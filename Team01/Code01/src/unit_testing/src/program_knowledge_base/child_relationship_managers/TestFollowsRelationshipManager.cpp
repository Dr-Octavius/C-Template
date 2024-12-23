#include "../../include/catch.hpp"
#include "program_knowledge_base/relationship/relationship_child_managers/FollowsManager.h"

TEST_CASE("FollowsManager isEmptyMap string,string") {
    FollowsManager followsManager;
    REQUIRE(followsManager.isEmptyMap());
    REQUIRE(followsManager.insertRelationship({ "x", EntityType::STMT }, { "y",EntityType::STMT }));
    REQUIRE_FALSE(followsManager.isEmptyMap());
}

TEST_CASE("FollowsManager hasMap string,string") {
    FollowsManager followsManager;
    REQUIRE_FALSE(followsManager.hasMap({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE(followsManager.insertRelationship({ "x", EntityType::IF }, { "y", EntityType::STMT }));
    REQUIRE_FALSE(followsManager.isEmptyMap());
}

TEST_CASE("FollowsManager getAllRelationshipEntries string,string") {
    FollowsManager followsManager;
    auto map = followsManager.getAllRelationshipEntries();
    REQUIRE(map.empty());
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> testMap
        = { {{"a", EntityType::IF}, {{"x", EntityType::STMT}, {"y", EntityType::STMT}}} };
    REQUIRE(followsManager.insertRelationship({ "a", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(followsManager.insertRelationship({ "a", EntityType::IF }, { "y",  EntityType::STMT }));
    map = followsManager.getAllRelationshipEntries();
    auto set = followsManager.getAllRelationshipEntries()[{ "a", EntityType::IF }];
    REQUIRE(map.size() == 1);
    REQUIRE(set.size() == 2);
    REQUIRE(followsManager.getAllRelationshipEntries() == testMap);
}

TEST_CASE("FollowsManager insertRelationship string,string") {
    FollowsManager followsManager;
    REQUIRE(followsManager.insertRelationship({ "a", EntityType::WHILE }, { "x",  EntityType::STMT }));
    REQUIRE(followsManager.insertRelationship({ "a", EntityType::WHILE }, { "y",  EntityType::STMT }));
    REQUIRE(followsManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    auto map = followsManager.getAllRelationshipEntries();
    REQUIRE(map.size() == 2);
    auto set = followsManager.getAllRelationshipEntries()[{ "a", EntityType::WHILE }];
    REQUIRE(set.size() == 2);
    REQUIRE(set.find({ "x",  EntityType::STMT }) != set.end());
    REQUIRE_FALSE(followsManager.isEmptyMap());

    REQUIRE_FALSE(followsManager.insertRelationship({ "b", EntityType::IF }, { "x",  EntityType::STMT }));
    REQUIRE(map.size() == 2);
}
