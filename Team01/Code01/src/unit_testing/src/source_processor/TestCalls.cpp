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

std::shared_ptr<CallsVisitor> getCallsVisitor(const std::string& program) {
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
    return callsVisitor;
}

TEST_CASE("[Calls] Basic test") {
    std::string program = "procedure func { call main; call Example; } procedure main { print var;  } procedure Example { a = b + c; call main; while (d > e) { f = g + h; } }";
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedCalls{
            {{"func", EntityType::PROCEDURE}, {{"main", EntityType::PROCEDURE}, {"Example", EntityType::PROCEDURE}}},
            {{"Example", EntityType::PROCEDURE}, {{"main", EntityType::PROCEDURE}}}
    };

    auto callsVisitor = getCallsVisitor(program);
    auto actualCalls = callsVisitor->getCalls();

    REQUIRE(actualCalls.size() == expectedCalls.size());
    for (const auto& pair : expectedCalls) {
        auto found = actualCalls.find(pair.first);
        REQUIRE(found != actualCalls.end());
        REQUIRE(found->second == pair.second);
    }
}

TEST_CASE("[CallsT] Comprehensive test for CallsT") {
    std::string program = R"(
        procedure A { call B; }
        procedure B { call C; while (i>0) { call D; } }
        procedure C { call E; }
        procedure D { print something; }
        procedure E { call F; }
        procedure F { read x; while (i>0) { read b; }}
    )";

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expectedCallsT{
            {{"A", EntityType::PROCEDURE}, {{"B", EntityType::PROCEDURE}, {"C", EntityType::PROCEDURE}, {"D", EntityType::PROCEDURE}, {"E", EntityType::PROCEDURE}, {"F", EntityType::PROCEDURE}}},
            {{"B", EntityType::PROCEDURE}, {{"C", EntityType::PROCEDURE}, {"D", EntityType::PROCEDURE}, {"E", EntityType::PROCEDURE}, {"F", EntityType::PROCEDURE}}},
            {{"C", EntityType::PROCEDURE}, {{"E", EntityType::PROCEDURE}, {"F", EntityType::PROCEDURE}}},
            {{"E", EntityType::PROCEDURE}, {{"F", EntityType::PROCEDURE}}},
    };

    auto callsVisitor = getCallsVisitor(program);
    auto actualCallsT = callsVisitor->getCallsT();

    REQUIRE(actualCallsT.size() == expectedCallsT.size());
    for (const auto& pair : expectedCallsT) {
        auto found = actualCallsT.find(pair.first);
        REQUIRE(found != actualCallsT.end());
        REQUIRE(found->second.size() == pair.second.size());
        for (const auto& innerPair : pair.second) {
            REQUIRE(found->second.find(innerPair) != found->second.end());
        }
    }
}
