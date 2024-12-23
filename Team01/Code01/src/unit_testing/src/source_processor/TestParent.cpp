#include "../../../include/catch.hpp"

#include "sp/Tokenizer/CompositeLexer.h"
#include "sp/Tokenizer/Token.h"
#include "common/Utils.h"
#include "sp/Tokenizer/Token.h"
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

std::shared_ptr<ParentVisitor> getParentVisitor(const std::string& program) {
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
    return parentVisitor;
}

TEST_CASE("[Parent] No nested structures") {
    std::string program = "procedure Example { a = b + c; }";

    auto parentVisitor = getParentVisitor(program);
    auto actualParents = parentVisitor->getParents();
    REQUIRE(actualParents.size() == 0);
}

TEST_CASE("[Parent] Simple Parent-Child Relationship") {
    std::string program = R"(
        procedure Main {
            read x;
            if (x > 10) then {
                print y;
            } else {
                B = B % C * A + Z;
        }
    })";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedParents{
            {{"3", EntityType::PRINT}, {{ "2", EntityType::IF }}},
            {{"4", EntityType::ASSIGN}, {{ "2", EntityType::IF }}}
    };

    auto parentVisitor = getParentVisitor(program);
    auto actualParents = parentVisitor->getParents();

    REQUIRE(actualParents.size() == expectedParents.size());

    for (const auto& expectedPair : expectedParents) {
        auto found = actualParents.find(expectedPair.first);
        REQUIRE(found != actualParents.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Parent] Nested Structure") {
    std::string program = R"(
        procedure Example {
            while (i < 100) {
                if (i % 2 == 0) then {
                    print even;
                } else {
                    print odd;
                    while (j < 50) {
                        read j;
                    }
                }
                print someVar;
            }
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedParents{
            {{"2", EntityType::IF}, {{ "1", EntityType::WHILE }}},
            {{"7", EntityType::PRINT}, {{ "1", EntityType::WHILE }}},
            {{"3", EntityType::PRINT}, {{ "2", EntityType::IF }}},
            {{"4", EntityType::PRINT}, {{ "2", EntityType::IF }}},
            {{"5", EntityType::WHILE}, {{ "2", EntityType::IF }}},
            {{"6", EntityType::READ}, {{ "5", EntityType::WHILE }}}
    };

    auto parentVisitor = getParentVisitor(program);
    auto actualParents = parentVisitor->getParents();

    REQUIRE(actualParents.size() == expectedParents.size());

    for (const auto& expectedPair : expectedParents) {
        auto found = actualParents.find(expectedPair.first);
        REQUIRE(found != actualParents.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Parent] Multiple Independent Structures") {
    std::string program2 = R"(
        procedure Sample {
            read inputData;
            if ((inputData != VAR) && (inputData <= VAR2)) then {
                print inputData;
            } else {
                read something;
            }
            while (k > 0) {
                k = k - 1;
            }
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedParents{
            {{"3", EntityType::PRINT}, {{ "2", EntityType::IF }}},
            {{"4", EntityType::READ}, {{ "2", EntityType::IF }}},
            {{"6", EntityType::ASSIGN}, {{ "5", EntityType::WHILE }}},
    };

    auto parentVisitor = getParentVisitor(program2);
    auto actualParents = parentVisitor->getParents();

    REQUIRE(actualParents.size() == expectedParents.size());

    for (const auto& expectedPair : expectedParents) {
        auto found = actualParents.find(expectedPair.first);
        REQUIRE(found != actualParents.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Parent] Deeply Nested Structure") {
    std::string program2 = R"(
        procedure Example {
            read x;
            while (x > 0) {
                if (x % 2 == 0) then {
                    print even;
                    x = x - 1;
                    if (x < 5) then {
                        read y;
                    } else {
                        print x;
                    }
                } else {
                    while (x > 5) {
                        x = x - 2;
                        if (x == 4) then {
                            print four;
                        } else {
                            print x;
                        }
                    }
                }
            }
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedParents{
            {{"3", EntityType::IF}, {{ "2", EntityType::WHILE }}},
            {{"4", EntityType::PRINT}, {{ "3", EntityType::IF }}},
            {{"5", EntityType::ASSIGN}, {{ "3", EntityType::IF }}},
            {{"6", EntityType::IF}, {{ "3", EntityType::IF }}},
            {{"9", EntityType::WHILE}, {{ "3", EntityType::IF }}},
            {{"7", EntityType::READ}, {{ "6", EntityType::IF }}},
            {{"8", EntityType::PRINT}, {{ "6", EntityType::IF }}},
            {{"10", EntityType::ASSIGN}, {{ "9", EntityType::WHILE }}},
            {{"11", EntityType::IF}, {{ "9", EntityType::WHILE }}},
            {{"12", EntityType::PRINT}, {{ "11", EntityType::IF }}},
            {{"13", EntityType::PRINT}, {{ "11", EntityType::IF }}},
    };

    auto parentVisitor = getParentVisitor(program2);
    auto actualParents = parentVisitor->getParents();

    REQUIRE(actualParents.size() == expectedParents.size());

    for (const auto& expectedPair : expectedParents) {
        auto found = actualParents.find(expectedPair.first);
        REQUIRE(found != actualParents.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Parent*] No nested structures") {
    std::string program = "procedure Example { a = b + c; read b; print c; read d; print e; read f; }";

    auto parentVisitor = getParentVisitor(program);
    auto actualParents = parentVisitor->getParentT();
    REQUIRE(actualParents.size() == 0);
}

TEST_CASE("[Parent*] Simple Nested Structure") {
    std::string program = R"(
        procedure Main {
            a = 1;
            if (x == y) then {
                b = 2;
                while (c < d) {
                    e = 5;
                }
            } else {
                print x;
            }
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedParentT{
            {{"2", EntityType::IF}, {{ "3", EntityType::ASSIGN }, { "4", EntityType::WHILE }, { "5", EntityType::ASSIGN }, { "6", EntityType::PRINT }}},
            {{"4", EntityType::WHILE}, {{ "5", EntityType::ASSIGN }}},
    };

    auto parentVisitor = getParentVisitor(program);
    auto actualParents = parentVisitor->getParentT();

    REQUIRE(actualParents.size() == expectedParentT.size());

    for (const auto& expectedPair : expectedParentT) {
        auto found = actualParents.find(expectedPair.first);
        REQUIRE(found != actualParents.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Parent*] Multiple Nested if and while Statements") {
    std::string program = R"(
        procedure Example {
            while (i < 10) {
                if (j == 10) then {
                    read j;
                    if (k != 10) then {
                        print k;
                    } else {
                        hsdbhjsbchjd = ehfieufiue + uerihfuierhui;
                    }
                } else {
                    read a;
                }
            }
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedParentT{
            {{"1", EntityType::WHILE}, {{ "2", EntityType::IF }, { "3", EntityType::READ }, { "4", EntityType::IF }, { "5", EntityType::PRINT }, { "6", EntityType::ASSIGN }, { "7", EntityType::READ }}},
            {{"2", EntityType::IF}, {{ "3", EntityType::READ }, { "4", EntityType::IF }, { "5", EntityType::PRINT }, { "6", EntityType::ASSIGN }, { "7", EntityType::READ }}},
            {{"4", EntityType::IF}, {{ "5", EntityType::PRINT }, { "6", EntityType::ASSIGN }}}
    };

    auto parentVisitor = getParentVisitor(program);
    auto actualParents = parentVisitor->getParentT();

    REQUIRE(actualParents.size() == expectedParentT.size());

    for (const auto& expectedPair : expectedParentT) {
        auto found = actualParents.find(expectedPair.first);
        REQUIRE(found != actualParents.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Parent*] Deeply nested structure with multiple top level statements") {
    std::string program = R"(
        procedure Example {
            read x;
            print y;
            while (i < 100) {
                if (i % 2 == 0) then {
                    print even;
                } else {
                    print odd;
                    while (j < 50) {
                        read j;
                        if (j == 25) then {
                            x = x + 1;
                        } else {
                            while (k < 5) {
                                k = k + 1;
                            }
                        }
                    }
                }
                i = i + 1;
            }
            print z;
        }
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedParentT{
            {{"3", EntityType::WHILE}, {{ "4", EntityType::IF }, { "5", EntityType::PRINT }, { "6", EntityType::PRINT }, { "7", EntityType::WHILE }, { "8", EntityType::READ }, { "9", EntityType::IF },{ "10", EntityType::ASSIGN }, { "11", EntityType::WHILE }, { "12", EntityType::ASSIGN }, { "13", EntityType::ASSIGN }}},
            {{"7", EntityType::WHILE}, {{ "8", EntityType::READ }, { "9", EntityType::IF }, { "10", EntityType::ASSIGN }, { "11", EntityType::WHILE }, { "12", EntityType::ASSIGN }}},
            {{"4", EntityType::IF}, {{ "5", EntityType::PRINT }, { "6", EntityType::PRINT }, { "7", EntityType::WHILE }, { "8", EntityType::READ }, { "9", EntityType::IF }, { "10", EntityType::ASSIGN }, { "11", EntityType::WHILE }, { "12", EntityType::ASSIGN }}},
            {{"9", EntityType::IF}, {{ "10", EntityType::ASSIGN }, { "11", EntityType::WHILE }, { "12", EntityType::ASSIGN }}},
            {{"11", EntityType::WHILE}, {{ "12", EntityType::ASSIGN }}}
    };

    auto parentVisitor = getParentVisitor(program);
    auto actualParents = parentVisitor->getParentT();

    REQUIRE(actualParents.size() == expectedParentT.size());

    for (const auto& expectedPair : expectedParentT) {
        auto found = actualParents.find(expectedPair.first);
        REQUIRE(found != actualParents.end());
        REQUIRE(found->second == expectedPair.second);
    }
}
