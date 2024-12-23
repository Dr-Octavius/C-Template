#include "../../../include/catch.hpp"

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
#include "program_knowledge_base/storage/StorageManager.h"
#include "sp/design_extractor/visitors/PatternVisitor.h"
#include "sp/design_extractor/visitors/CallsVisitor.h"
#include <utility>
#include <unordered_set>

TEST_CASE("Main Parser Test") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "main"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            // statement after if within while
            {TokenType::IDENTIFIER, "val"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "24"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::IDENTIFIER, "r"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            // if after while
            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    auto parentVisitor = std::make_shared<ParentVisitor>();
    auto followsVisitor = std::make_shared<FollowsVisitor>();
    auto entitiesVisitor = std::make_shared<EntityVisitor>();
    auto storageManager = std::make_shared<StorageManager>();
    auto usesVisitor = std::make_shared<UsesVisitor>();
    auto callsVisitor = std::make_shared<CallsVisitor>();
    auto patternVisitor = std::make_shared<PatternVisitor>();
    auto modifiesVisitor = std::make_shared<ModifiesVisitor>();
    DesignExtractor designExtractor(followsVisitor, parentVisitor, usesVisitor, modifiesVisitor, entitiesVisitor, callsVisitor, patternVisitor, storageManager);
    designExtractor.traverseAll(astRoot);

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> followsRecords = followsVisitor->getFollows();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> followsTRecords = followsVisitor->getFollowsT();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> parentRecords = parentVisitor->getParents();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> parentTRecords = parentVisitor->getParentT();
    std::map<std::string, EntityType> stmtNumberMap = entitiesVisitor->getStmtNumberMap();
    std::map<EntityType, std::unordered_set<std::string>> entityExtractor = entitiesVisitor->getEntityToStatementsMap();

    std::cout << "\n------STATEMENT NUMBER TO ENTITY------\n\n";

    for (const auto& pair : stmtNumberMap) {
        std::cout << "Statement number " << pair.first << " is of type " << entityTypeToString(pair.second) << std::endl;
    }

    std::cout << "\n------ENTITY TYPE TO STATEMENTS------\n\n";

    for (const auto& pair : entityExtractor) {
        std::cout << "Entity type " << entityTypeToString(pair.first) << " includes: ";
        for (const auto& item : pair.second) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n------FOLLOWS------\n\n";

    for (const auto& pair : followsRecords) {
        std::cout << "Follows: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") is followed by: ";
        for (const auto& follower : pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n------FOLLOWS*------\n\n";

    for (const auto& pair : followsTRecords) {
        std::string keyStr = pair.first.first + " (" + entityTypeToString(pair.first.second) + ")";
        std::cout << keyStr << " follows: ";
        for (const auto& followsPair : pair.second) {
            std::string followsStr = followsPair.first + " (" + entityTypeToString(followsPair.second) + ")";
            std::cout << followsStr << "; ";
        }
        std::cout << std::endl; // New line after each key's unordered_set is printed
    }

    std::cout << "\n------PARENT------\n\n";

    for (const auto& pair : parentRecords) {
        std::cout << "Child: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") has parent: ";
        for (const auto& child : pair.second) {
            std::cout << child.first << " (" << entityTypeToString(child.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n------PARENT*------\n\n";

    for (const auto& pair : parentTRecords) {
        std::cout << "Parent: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") has children: ";
        for (const auto& child : pair.second) {
            std::cout << child.first << " (" << entityTypeToString(child.second) << "), ";
        }
        std::cout << std::endl;
    }

    REQUIRE(astRoot != nullptr);
}

TEST_CASE("Main Parser Test II") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "procedure"},
            {TokenType::KEYWORD, "procedure"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::KEYWORD, "read"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::KEYWORD, "procedure"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::KEYWORD, "while"},
            {TokenType::RELATIONAL_OPERATOR, "!="},
            {TokenType::KEYWORD, "read"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "print"},
            {TokenType::KEYWORD, "print"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            // statement after if within while
            {TokenType::KEYWORD, "print"},
            {TokenType::KEYWORD, "print"},
            {TokenType::PUNCTUATION, ";"},
            // if after while
            {TokenType::PUNCTUATION, "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    auto followsVisitor = std::make_shared<FollowsVisitor>();
    auto parentVisitor = std::make_shared<ParentVisitor>();
    auto modifiesVisitor = std::make_shared<ModifiesVisitor>();
    auto usesVisitor = std::make_shared<UsesVisitor>();
    auto entitiesVisitor = std::make_shared<EntityVisitor>();
    auto storageManager = std::make_shared<StorageManager>();
    auto patternVisitor = std::make_shared<PatternVisitor>();
    auto callsVisitor = std::make_shared<CallsVisitor>();

    DesignExtractor designExtractor(followsVisitor, parentVisitor, usesVisitor, modifiesVisitor, entitiesVisitor, callsVisitor, patternVisitor, storageManager);
    designExtractor.traverseAll(astRoot);

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> followsRecords = followsVisitor->getFollows();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> followsTRecords = followsVisitor->getFollowsT();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> parentRecords = parentVisitor->getParents();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> parentTRecords = parentVisitor->getParentT();
    std::map<std::string, EntityType> stmtNumberMap = entitiesVisitor->getStmtNumberMap();
    std::map<EntityType, std::unordered_set<std::string>> entityExtractor = entitiesVisitor->getEntityToStatementsMap();

    std::cout << "\n------STATEMENT NUMBER TO ENTITY------\n\n";

    for (const auto& pair : stmtNumberMap) {
        std::cout << "Statement number " << pair.first << " is of type " << entityTypeToString(pair.second) << std::endl;
    }

    std::cout << "\n------ENTITY TYPE TO STATEMENTS------\n\n";

    for (const auto& pair : entityExtractor) {
        std::cout << "Entity type " << entityTypeToString(pair.first) << " includes: ";
        for (const auto& item : pair.second) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n------FOLLOWS------\n\n";

    for (const auto& pair : followsRecords) {
        std::cout << "Follows: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") is followed by: ";
        for (const auto& follower : pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n------FOLLOWS*------\n\n";

    for (const auto& pair : followsTRecords) {
        std::string keyStr = pair.first.first + " (" + entityTypeToString(pair.first.second) + ")";
        std::cout << keyStr << " follows: ";
        for (const auto& followsPair : pair.second) {
            std::string followsStr = followsPair.first + " (" + entityTypeToString(followsPair.second) + ")";
            std::cout << followsStr << "; ";
        }
        std::cout << std::endl; // New line after each key's unordered_set is printed
    }

    std::cout << "\n------PARENT------\n\n";

    for (const auto& pair : parentRecords) {
        std::cout << "Child: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") has parent: ";
        for (const auto& child : pair.second) {
            std::cout << child.first << " (" << entityTypeToString(child.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n------PARENT*------\n\n";

    for (const auto& pair : parentTRecords) {
        std::cout << "Parent: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") has children: ";
        for (const auto& child : pair.second) {
            std::cout << child.first << " (" << entityTypeToString(child.second) << "), ";
        }
        std::cout << std::endl;
    }

    REQUIRE(astRoot != nullptr);
}

TEST_CASE("Follows Test") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "procedure"},
            {TokenType::KEYWORD, "main"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            // statement after if within while
            {TokenType::IDENTIFIER, "val"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "24"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            // After while
            {TokenType::IDENTIFIER, "r"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            // if after assign after while
            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();

    auto followsVisitor = std::make_shared<FollowsVisitor>();
    auto parentVisitor = std::make_shared<ParentVisitor>();
    auto modifiesVisitor = std::make_shared<ModifiesVisitor>();
    auto usesVisitor = std::make_shared<UsesVisitor>();
    auto entitiesVisitor = std::make_shared<EntityVisitor>();
    auto callsVisitor = std::make_shared<CallsVisitor>();
    auto patternVisitor = std::make_shared<PatternVisitor>();
    auto storageManager = std::make_shared<StorageManager>();

    DesignExtractor designExtractor(followsVisitor, parentVisitor, usesVisitor, modifiesVisitor, entitiesVisitor, callsVisitor, patternVisitor, storageManager);
    designExtractor.traverseAll(astRoot);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> followsRecords = followsVisitor->getFollows();

    REQUIRE(astRoot != nullptr);
}
