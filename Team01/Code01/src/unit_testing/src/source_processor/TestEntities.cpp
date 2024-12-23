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

std::shared_ptr<EntityVisitor> getEntitiesVisitor(const std::string& program) {
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
    return entitiesVisitor;
}

TEST_CASE("[Entities Extractor] Simple read statement") {
    std::string program = R"(
        procedure Main {
            read someRandomVariable;
        }
    )";

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto entityMap = entitiesVisitor->getEntityToStatementsMap();

    std::unordered_map<EntityType, std::unordered_set<std::string>> expected = {
            {EntityType::READ, {"1"}},
            {EntityType::STMT, {"1"}},
            {EntityType::VARIABLE, {"someRandomVariable"}},
            {EntityType::PROCEDURE, {"Main"}}
    };

    REQUIRE(entityMap.size() == expected.size());
    for (const auto& [type, names] : expected) {
        auto found = entityMap.find(type);
        REQUIRE(found != entityMap.end());
        REQUIRE(found->second == names);
    }
}

TEST_CASE("[Entities Extractor] Simple program that captures Parent-Child relation") {
    std::string program = R"(
        procedure Main {
            read x;
            if (x > 10) then {
                print y;
            } else {
                B = B % C * A + Z;
        }
    })";

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto entityMap = entitiesVisitor->getEntityToStatementsMap();

    std::unordered_map<EntityType, std::unordered_set<std::string>> expected = {
            {EntityType::READ, {"1"}},
            {EntityType::PRINT, {"3"}},
            {EntityType::ASSIGN, {"4"}},
            {EntityType::IF, {"2"}},
            {EntityType::STMT, {"1", "2", "3", "4"}},
            {EntityType::VARIABLE, {"x", "y", "B", "C", "A", "Z"}},
            {EntityType::CONSTANT, {"10"}},
            {EntityType::PROCEDURE, {"Main"}}
    };

    REQUIRE(entityMap.size() == expected.size());
    for (const auto& [type, names] : expected) {
        auto found = entityMap.find(type);
        REQUIRE(found != entityMap.end());
        REQUIRE(found->second == names);
    }
}

TEST_CASE("[Entities Extractor] Mixed Entity Types (all possible ones)") {
    std::string program = R"(
        procedure Example {
            read x;
            print y;
            z = x + 1;
            if (x < y) then {
                a = z + 2;
            } else {
                while (i > 0) {
                    val = mock + 1;
                }
            }
        }
    )";

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto entityMap = entitiesVisitor->getEntityToStatementsMap();

    std::unordered_map<EntityType, std::unordered_set<std::string>> expected = {
            {EntityType::READ, {"1"}},
            {EntityType::PRINT, {"2"}},
            {EntityType::ASSIGN, {"3", "5", "7"}},
            {EntityType::IF, {"4"}},
            {EntityType::WHILE, {"6"}},
            {EntityType::STMT, {"1", "2", "3", "4", "5", "6", "7"}},
            {EntityType::VARIABLE, {"x", "y", "z", "a", "i", "val", "mock"}},
            {EntityType::CONSTANT, {"0", "1", "2"}},
            {EntityType::PROCEDURE, {"Example"}}
    };

    REQUIRE(entityMap.size() == expected.size());
    for (const auto& [type, names] : expected) {
        auto found = entityMap.find(type);
        REQUIRE(found != entityMap.end());
        REQUIRE(found->second == names);
    }
}

TEST_CASE("[Entities Extractor] Complex construct with deeply nested structure") {
    std::string program = R"(
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

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto entityMap = entitiesVisitor->getEntityToStatementsMap();

    std::unordered_map<EntityType, std::unordered_set<std::string>> expected = {
            {EntityType::READ, {"1", "7"}},
            {EntityType::PRINT, {"4", "8", "12", "13"}},
            {EntityType::ASSIGN, {"5", "10"}},
            {EntityType::IF, {"3", "6", "11"}},
            {EntityType::WHILE, {"2", "9"}},
            {EntityType::STMT, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"}},
            {EntityType::VARIABLE, {"x", "y", "four", "even"}},
            {EntityType::CONSTANT, {"0", "1", "2", "4", "5"}},
            {EntityType::PROCEDURE, {"Example"}}
    };

    REQUIRE(entityMap.size() == expected.size());
    for (const auto& [type, names] : expected) {
        auto found = entityMap.find(type);
        REQUIRE(found != entityMap.end());
        REQUIRE(found->second == names);
    }
}

TEST_CASE("[Statement number map] Simple one line program") {
    std::string program = R"(
        procedure Main {
            print somethingggg;
        }
    )";

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto stmtNumberMap = entitiesVisitor->getStmtNumberMap();

    std::map<std::string, EntityType> expected = {
            {"1", EntityType::PRINT},
    };

    REQUIRE(stmtNumberMap.size() == expected.size());
    for (const auto& [stmt, entity] : expected) {
        auto found = stmtNumberMap.find(stmt);
        REQUIRE(found != stmtNumberMap.end());
        REQUIRE(found->second == entity);
    }
}

TEST_CASE("[Statement number map] Program with different entity types (no nesting yet)") {
    std::string program = R"(
        procedure Main {
            print a;
            read b;
            b=b+2;
            print c;
            read d;
            a = a + 1 % b;
        }
    )";

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto stmtNumberMap = entitiesVisitor->getStmtNumberMap();

    std::map<std::string, EntityType> expected = {
            {"1", EntityType::PRINT},
            {"2", EntityType::READ},
            {"3", EntityType::ASSIGN},
            {"4", EntityType::PRINT},
            {"5", EntityType::READ},
            {"6", EntityType::ASSIGN},
    };

    REQUIRE(stmtNumberMap.size() == expected.size());
    for (const auto& [stmt, entity] : expected) {
        auto found = stmtNumberMap.find(stmt);
        REQUIRE(found != stmtNumberMap.end());
        REQUIRE(found->second == entity);
    }
}

TEST_CASE("[Statement number map] More complex program with nesting") {
    std::string program = R"(
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

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto stmtNumberMap = entitiesVisitor->getStmtNumberMap();

    std::map<std::string, EntityType> expected = {
            {"1", EntityType::READ},
            {"2", EntityType::IF},
            {"3", EntityType::PRINT},
            {"4", EntityType::READ},
            {"5", EntityType::WHILE},
            {"6", EntityType::ASSIGN},
    };

    REQUIRE(stmtNumberMap.size() == expected.size());
    for (const auto& [stmt, entity] : expected) {
        auto found = stmtNumberMap.find(stmt);
        REQUIRE(found != stmtNumberMap.end());
        REQUIRE(found->second == entity);
    }
}

TEST_CASE("[Statement number map] Complex program with deep nesting") {
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

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto stmtNumberMap = entitiesVisitor->getStmtNumberMap();

    std::map<std::string, EntityType> expected = {
            {"1", EntityType::READ},
            {"2", EntityType::PRINT},
            {"3", EntityType::WHILE},
            {"4", EntityType::IF},
            {"5", EntityType::PRINT},
            {"6", EntityType::PRINT},
            {"7", EntityType::WHILE},
            {"8", EntityType::READ},
            {"9", EntityType::IF},
            {"10", EntityType::ASSIGN},
            {"11", EntityType::WHILE},
            {"12", EntityType::ASSIGN},
            {"13", EntityType::ASSIGN},
            {"14", EntityType::PRINT},
    };

    REQUIRE(stmtNumberMap.size() == expected.size());
    for (const auto& [stmt, entity] : expected) {
        auto found = stmtNumberMap.find(stmt);
        REQUIRE(found != stmtNumberMap.end());
        REQUIRE(found->second == entity);
    }
}

TEST_CASE("[stmtSpecificEntitiesMap used for with clause eval in qps] Simple test") {
    std::string program = "procedure Main {\n"
                          "    call Compute;\n"
                          "    print y;\n"
                          "}\n"
                          "procedure Compute {\n"
                          "    read x;\n"
                          "    z = x * 2;\n"
                          "    call Output;\n"
                          "}\n"
                          "procedure Output {\n"
                          "    print z;\n"
                          "}";

    auto entitiesVisitor = getEntitiesVisitor(program);
    auto stmtSpecificEntitiesMap = entitiesVisitor->getStmtSpecificEntitiesMap();

    for (const auto& mapEntry : stmtSpecificEntitiesMap) {
        const auto& key = mapEntry.first;
        const auto& value = mapEntry.second;

        std::cout << "Key: (" << key.first << ", " << entityTypeToString(key.second) << "), Values: {";

        for (const auto& setEntry : value) {
            std::cout << "(" << setEntry.first << ", " << entityTypeToString(setEntry.second) << "), ";
        }

        std::cout << "}\n";
    }

    REQUIRE(entitiesVisitor != nullptr);
}
