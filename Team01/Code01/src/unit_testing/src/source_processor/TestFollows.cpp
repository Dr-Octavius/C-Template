#include "../../../include/catch.hpp"

#include "sp/Tokenizer/CompositeLexer.h"
#include "sp/Tokenizer/Token.h"
#include "common/Utils.h"
#include "sp/parser/MainParser.h"
#include "sp/ast/nodes/ASTNode.h"
#include <vector>
#include <memory>
#include "sp/design_extractor/DesignExtractor.h"
#include "sp/design_extractor/visitors/FollowsVisitor.h"
#include "sp/design_extractor/visitors/ParentVisitor.h"
#include "sp/design_extractor/visitors/EntityVisitor.h"
#include "sp/design_extractor/visitors/ModifiesVisitor.h"
#include "sp/design_extractor/visitors/UsesVisitor.h"
#include "sp/design_extractor/visitors/CallsVisitor.h"
#include "sp/design_extractor/visitors/PatternVisitor.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include <utility>
#include <unordered_set>

std::shared_ptr<FollowsVisitor> getFollowsVisitor(const std::string& program) {
    CompositeLexer lexer;
    std::vector<Token> tokens = lexer.tokenize(program);
    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    auto parentVisitor = std::make_shared<ParentVisitor>();
    auto followsVisitor = std::make_shared<FollowsVisitor>();
    auto entitiesVisitor = std::make_shared<EntityVisitor>();
    auto modifiesVisitor = std::make_shared<ModifiesVisitor>();
    auto usesVisitor = std::make_shared<UsesVisitor>();
    auto callsVisitor = std::make_shared<CallsVisitor>();
    auto patternVisitor = std::make_shared<PatternVisitor>();
    auto storageManager = std::make_shared<StorageManager>();

    DesignExtractor designExtractor(followsVisitor, parentVisitor, usesVisitor, modifiesVisitor, entitiesVisitor, callsVisitor, patternVisitor, storageManager);
    designExtractor.traverseAll(astRoot);
    return followsVisitor;
}

TEST_CASE("[Follows] Test FollowsVisitor with Sample Program") {
    std::string program = "procedure Example { a = b + c; while (d > e) { f = g + h; } }";
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollows{
            {{"1", EntityType::ASSIGN}, {{"2", EntityType::WHILE}}},
    };

    auto followsVisitor = getFollowsVisitor(program);
    auto actualFollows = followsVisitor->getFollows();
    REQUIRE(actualFollows.size() == expectedFollows.size());
    for (const auto& pair : expectedFollows) {
        auto found = actualFollows.find(pair.first);
        REQUIRE(found != actualFollows.end());
        REQUIRE(found->second == pair.second);
    }
}

TEST_CASE("[Follows] Sequential Assignments") {
    std::string program = "procedure Example { x = 1; y = x + 2; z = y * 3; }";
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollows{
            {{"1", EntityType::ASSIGN}, {{"2", EntityType::ASSIGN}}},
            {{"2", EntityType::ASSIGN}, {{"3", EntityType::ASSIGN}}}
    };

    auto followsVisitor = getFollowsVisitor(program);
    auto actualFollows = followsVisitor->getFollows();
    REQUIRE(actualFollows.size() == expectedFollows.size());
    for (const auto& pair : expectedFollows) {
        auto found = actualFollows.find(pair.first);
        REQUIRE(found != actualFollows.end());
        REQUIRE(found->second == pair.second);
    }
}

TEST_CASE("[Follows] Assignment Inside While Loop") {
    std::string program = "procedure Example { while (x < 5) { i = i + 1; } j = 10; }";
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollows {
            {{"1", EntityType::WHILE}, {{"3", EntityType::ASSIGN}}}
    };

    auto followsVisitor = getFollowsVisitor(program);
    auto actualFollows = followsVisitor->getFollows();
    REQUIRE(actualFollows.size() == expectedFollows.size());
    for (const auto& pair : expectedFollows) {
        auto found = actualFollows.find(pair.first);
        REQUIRE(found != actualFollows.end());
        REQUIRE(found->second == pair.second);
    }
}

TEST_CASE("[Follows] Nested While Loop") {
    std::string program = "procedure Example { a = 1; while (b < 2) { while (c < 3) { d = 4; } e = 5; } f = 6; }";
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollows {
            {{"1", EntityType::ASSIGN}, {{"2", EntityType::WHILE}}},
            {{"3", EntityType::WHILE}, {{"5", EntityType::ASSIGN}}},
            {{"2", EntityType::WHILE}, {{"6", EntityType::ASSIGN}}}
    };

    auto followsVisitor = getFollowsVisitor(program);
    auto actualFollows = followsVisitor->getFollows();
    REQUIRE(actualFollows.size() == expectedFollows.size());
    for (const auto& pair : expectedFollows) {
        auto found = actualFollows.find(pair.first);
        REQUIRE(found != actualFollows.end());
        REQUIRE(found->second == pair.second);
    }
}

TEST_CASE("[Follows] If-Else Structure") {
    std::string program = "procedure   Example { if (   x == 1   ) then{ a = 2; } else { b = 3; } c=4; }";
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollows {
            {{"1", EntityType::IF}, {{"4", EntityType::ASSIGN}}}
    };

    auto followsVisitor = getFollowsVisitor(program);
    auto actualFollows = followsVisitor->getFollows();
    REQUIRE(actualFollows.size() == expectedFollows.size());
    for (const auto& pair : expectedFollows) {
        auto found = actualFollows.find(pair.first);
        REQUIRE(found != actualFollows.end());
        REQUIRE(found->second == pair.second);
    }
}

TEST_CASE("[Follows & FollowsT] Complex Control Structures") {
    std::string program = "procedure Example { a = 1; if (x > 0) then { while (y < 2) { b = 2; } } else { c = 3; } d = 4; }";
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollows {
            {{"1", EntityType::ASSIGN}, {{"2", EntityType::IF}}},
            {{"2", EntityType::IF}, {{"6", EntityType::ASSIGN}}}
    };
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollowsT {
            {{"1", EntityType::ASSIGN}, {{ "2", EntityType::IF }, { "6", EntityType::ASSIGN }}},
            {{"2", EntityType::IF}, {{"6", EntityType::ASSIGN}}},
    };

    auto followsVisitor = getFollowsVisitor(program);
    auto actualFollows = followsVisitor->getFollows();
    REQUIRE(actualFollows.size() == expectedFollows.size());
    for (const auto& pair : expectedFollows) {
        auto found = actualFollows.find(pair.first);
        REQUIRE(found != actualFollows.end());
        REQUIRE(found->second == pair.second);
    }

    auto actualFollowsT = followsVisitor->getFollowsT();

    REQUIRE(actualFollowsT.size() == expectedFollowsT.size());

    for (const auto& expectedPair : expectedFollowsT) {
        auto found = actualFollowsT.find(expectedPair.first);
        REQUIRE(found != actualFollowsT.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[FollowsT] Simple Linear Sequence") {
    std::string program = R"(
        procedure Example {
            read x;
            print y;
            z = x + y;
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollowsT{
            {{"1", EntityType::READ}, {{ "2", EntityType::PRINT }, { "3", EntityType::ASSIGN }}},
            {{"2", EntityType::PRINT}, {{ "3", EntityType::ASSIGN }}}
    };

    auto followsVisitor = getFollowsVisitor(program);

    auto actualFollowsT = followsVisitor->getFollowsT();

    REQUIRE(actualFollowsT.size() == expectedFollowsT.size());

    for (const auto& expectedPair : expectedFollowsT) {
        auto found = actualFollowsT.find(expectedPair.first);
        REQUIRE(found != actualFollowsT.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[FollowsT] Nested Structures with a conditional statement") {
    std::string program = R"(
        procedure Example {
            a = b + c;
            while (d > e) {
                f = g + h;
                i = j + k;
            }
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollowsT{
            {{"1", EntityType::ASSIGN}, {{ "2", EntityType::WHILE }}},
            {{"3", EntityType::ASSIGN}, {{ "4", EntityType::ASSIGN }}}
    };

    auto followsVisitor = getFollowsVisitor(program);

    auto actualFollowsT = followsVisitor->getFollowsT();

    REQUIRE(actualFollowsT.size() == expectedFollowsT.size());

    for (const auto& expectedPair : expectedFollowsT) {
        auto found = actualFollowsT.find(expectedPair.first);
        REQUIRE(found != actualFollowsT.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[FollowsT] Deeply nested structure with mixed statement types") {
    std::string program = R"(
        procedure Example {
        read x;
        while (x > 0) {
            if (y > 1) then {
                print z;
                a = b + c;
            } else {
                read Process;
                while (z < 5) {
                    y = y + 1;
                }
                print something;
            }
            print x;
        }
        read y;
    }
    )";

    // Expected FollowsT relationship map
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollowsT{
            {{"1", EntityType::READ}, {{ "11", EntityType::READ }, { "2", EntityType::WHILE }}},
            {{"2", EntityType::WHILE}, {{ "11", EntityType::READ }}},
            {{"3", EntityType::IF}, {{ "10", EntityType::PRINT }}},
            {{"4", EntityType::PRINT}, {{"5", EntityType::ASSIGN}}},
            {{"6", EntityType::READ}, {{ "7", EntityType::WHILE }, { "9", EntityType::PRINT }}},
            {{"7", EntityType::WHILE}, {{ "9", EntityType::PRINT }}},
    };

    auto followsVisitor = getFollowsVisitor(program);

    auto actualFollowsT = followsVisitor->getFollowsT();

    REQUIRE(actualFollowsT.size() == expectedFollowsT.size());

    for (const auto& expectedPair : expectedFollowsT) {
        auto found = actualFollowsT.find(expectedPair.first);
        REQUIRE(found != actualFollowsT.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[FollowsT] Several adjacent lines") {
    std::string program = R"(
        procedure AdjacentExample {
        read x;
        print y;
        read SomeVariable;
        a = b + c;
        while(x<10){
            read z;
            print LOL;
        }
        print a;
    }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedFollowsT{
            {{ "1", EntityType::READ}, {{ "2", EntityType::PRINT }, { "3", EntityType::READ }, { "4", EntityType::ASSIGN }, { "5", EntityType::WHILE }, { "8", EntityType::PRINT }}},
            {{ "2", EntityType::PRINT}, {{ "3", EntityType::READ }, { "4", EntityType::ASSIGN }, { "5", EntityType::WHILE }, { "8", EntityType::PRINT }}},
            {{ "3", EntityType::READ }, {{ "4", EntityType::ASSIGN }, { "5", EntityType::WHILE }, { "8", EntityType::PRINT }}},
            {{ "4", EntityType::ASSIGN }, {{ "5", EntityType::WHILE }, { "8", EntityType::PRINT }}},
            {{ "5", EntityType::WHILE }, {{ "8", EntityType::PRINT }}},
            {{ "6", EntityType::READ }, {{ "7", EntityType::PRINT }}}
    };

    auto followsVisitor = getFollowsVisitor(program);

    auto actualFollowsT = followsVisitor->getFollowsT();

    REQUIRE(actualFollowsT.size() == expectedFollowsT.size());

    for (const auto& expectedPair : expectedFollowsT) {
        auto found = actualFollowsT.find(expectedPair.first);
        REQUIRE(found != actualFollowsT.end());
        REQUIRE(found->second == expectedPair.second);
    }
}