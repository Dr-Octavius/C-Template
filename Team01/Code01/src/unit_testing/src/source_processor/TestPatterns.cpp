#include "../../include/catch.hpp"

#include "sp/Tokenizer/CompositeLexer.h"
#include "common/Utils.h"
#include "sp/Tokenizer/Token.h"
#include "sp/parser/MainParser.h"
#include "sp/ast/nodes/ASTNode.h"
#include <vector>
#include <memory>
#include "sp/design_extractor/DesignExtractor.h"
#include "sp/design_extractor/visitors/FollowsVisitor.h"
#include "sp/design_extractor/visitors/ParentVisitor.h"
#include "sp/design_extractor/visitors/UsesVisitor.h"
#include "sp/design_extractor/visitors/ModifiesVisitor.h"
#include "sp/design_extractor/visitors/EntityVisitor.h"
#include "sp/design_extractor/visitors/CallsVisitor.h"
#include "sp/design_extractor/visitors/PatternVisitor.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include <utility>
#include <unordered_set>

std::shared_ptr<PatternVisitor> getPatternVisitor(const std::string& program) {
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
    return patternVisitor;
}

TEST_CASE("Assign, While, If Pattern test") {
    std::string program = "procedure testAssignmentPattern {\n"
                          "    a = 0;\n"
                          "    x = x + 1 + a;\n"
                          "    y = x - a;\n"
                          "    read b;\n"
                          "    call Another; \n"
                          "    if (b > 0) then {\n"
                          "        x = x % b + a;\n"
                          "        y = (y + x) % (b % a) - y;\n"
                          "        while (x != y) {\n"
                          "            read c;\n"
                          "            x = (((x - y) - c) % b) + a;\n"
                          "            if (c > a) then {\n"
                          "                a = c + b;\n"
                          "            } else {\n"
                          "                a = c - b % 2;\n"
                          "            }\n"
                          "        }\n"
                          "    } else {\n"
                          "        print b;\n"
                          "        if (x < b) then {\n"
                          "            print x;\n"
                          "            x = x + 1;\n"
                          "        } else {\n"
                          "            x = ((x - b) % b) + (a - c);\n"
                          "        }\n"
                          "    } }\n"
                          "procedure Another {\n"
                          "        while (i > 0) {\n"
                          "            r = J;\n"
                          "            while (i > 1) {\n"
                          "                x = x + z * 5;\n"
                          "                while (j > 3) {\n"
                          "                    read c;\n"
                          "                }\n"
                          "            }\n"
                          "        }\n"
                          "  print t;\n"
                          "    }";

    auto patternVisitor = getPatternVisitor(program);

    auto assignPattern = patternVisitor->getAssignPatternData();
    auto ifPattern = patternVisitor->getIfPatternData();
    auto whilePattern = patternVisitor->getWhilePatternData();

    std::map<std::pair<std::string, EntityType>, std::string> expectedAssignPatterns{
            {{"1", EntityType::ASSIGN}, "0"},
            {{"2", EntityType::ASSIGN}, "x+1+a"},
            {{"3", EntityType::ASSIGN}, "x-a"},
            {{"7", EntityType::ASSIGN}, "x%b+a"},
            {{"8", EntityType::ASSIGN}, "y+x%b%a-y"},
            {{"11", EntityType::ASSIGN}, "x-y-c%b+a"},
            {{"13", EntityType::ASSIGN}, "c+b"},
            {{"14", EntityType::ASSIGN}, "c-b%2"},
            {{"18", EntityType::ASSIGN}, "x+1"},
            {{"19", EntityType::ASSIGN}, "x-b%b+a-c"},
            {{"21", EntityType::ASSIGN}, "J"},
            {{"23", EntityType::ASSIGN}, "x+z*5"}
    };

    REQUIRE(expectedAssignPatterns.size() == assignPattern.size());
    for (const auto& [key, expectedExpr]: expectedAssignPatterns) {
        auto node = assignPattern.at(key);
        REQUIRE(node->getRightChild()->generateExpressionString() == expectedExpr);
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedIfPatterns{
            {{"6", EntityType::IF}, {{"b", EntityType::VARIABLE}}},
            {{"12", EntityType::IF}, {{"c", EntityType::VARIABLE}, {"a", EntityType::VARIABLE}}},
            {{"16", EntityType::IF}, {{"x", EntityType::VARIABLE}, {"b", EntityType::VARIABLE}}}
    };

    REQUIRE(expectedIfPatterns.size() == ifPattern.size());
    for (const auto& [stmtPair, varSet]: ifPattern) {
        auto expectedSet = expectedIfPatterns[stmtPair];
        REQUIRE(varSet == expectedSet);
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedWhilePatterns{
            {{"9", EntityType::WHILE}, {{"x", EntityType::VARIABLE}, {"y", EntityType::VARIABLE}}},
            {{"20", EntityType::WHILE}, {{"i", EntityType::VARIABLE}}},
            {{"22", EntityType::WHILE}, {{"i", EntityType::VARIABLE}}},
            {{"24", EntityType::WHILE}, {{"j", EntityType::VARIABLE}}}
    };

    REQUIRE(expectedWhilePatterns.size() == whilePattern.size());
    for (const auto& [stmtPair, varSet]: whilePattern) {
        auto expectedSet = expectedWhilePatterns[stmtPair];
        REQUIRE(varSet == expectedSet);
    }

    REQUIRE(patternVisitor != nullptr);
}

TEST_CASE("While, If pattern test") {
    std::string program = "procedure sampleProc123 {\n"
                          "    while (y != 2 * z - a + (b - c)) { \n"
                          "    print a; } \n"
                          "    if ((b == 0) && (a == d)) then {\n"
                          "      read b; \n"
                          "    } else { \n"
                          "      print a;} } ";

    auto patternVisitor = getPatternVisitor(program);

    auto ifPattern = patternVisitor->getIfPatternData();
    auto whilePattern = patternVisitor->getWhilePatternData();

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedIfPatterns{
            {{"3", EntityType::IF}, {{"b", EntityType::VARIABLE}, {"a", EntityType::VARIABLE}, {"d", EntityType::VARIABLE}}}
    };

    REQUIRE(expectedIfPatterns.size() == ifPattern.size());
    for (const auto& [stmtPair, varSet]: ifPattern) {
        auto expectedSet = expectedIfPatterns[stmtPair];
        REQUIRE(varSet == expectedSet);
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedWhilePatterns{
            {{"1", EntityType::WHILE}, {{"z", EntityType::VARIABLE}, {"y", EntityType::VARIABLE}, {"a", EntityType::VARIABLE}, {"b", EntityType::VARIABLE},
                                        {"c", EntityType::VARIABLE}}},
    };

    REQUIRE(expectedWhilePatterns.size() == whilePattern.size());
    for (const auto& [stmtPair, varSet]: whilePattern) {
        auto expectedSet = expectedWhilePatterns[stmtPair];
        REQUIRE(varSet == expectedSet);
    }

    REQUIRE(patternVisitor != nullptr);
}