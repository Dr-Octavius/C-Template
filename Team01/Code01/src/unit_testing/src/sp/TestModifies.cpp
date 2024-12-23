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

std::shared_ptr<ModifiesVisitor> getModifiesVisitor(const std::string& program) {
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
    return modifiesVisitor;
}

TEST_CASE("Modifies Test General") {
    std::string program = "procedure Example {\n"
                          "        val = 24;\n"
                          "        r = a;\n"
                          "        if (x > 0) then {\n"
                          "            a = 3;\n"
                          "        } else {\n"
                          "            b = x % 5;\n"
                          "        }\n"
                          "        while (i > 0) {\n"
                          "            x = x + z * 5;\n"
                          "        }\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::ASSIGN }, {{ "val", EntityType::VARIABLE }}},
            {{ "2", EntityType::ASSIGN }, {{ "r", EntityType::VARIABLE }}},
            {{ "3", EntityType::IF }, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }}},
            {{ "4", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }}},
            {{ "5", EntityType::ASSIGN }, {{ "b", EntityType::VARIABLE }}},
            {{ "6", EntityType::WHILE }, {{ "x", EntityType::VARIABLE }}},
            {{ "7", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "val", EntityType::VARIABLE }, { "r", EntityType::VARIABLE }, { "a", EntityType::VARIABLE },
                                                    { "b", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies Test While") {
    std::string program = "procedure Example {\n"
                          "        while (i > 0) {\n"
                          "            r = x;\n"
                          "            x = x + z * 5;\n"
                          "        }\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::WHILE}, {{ "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
            {{"2", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"3", EntityType::ASSIGN}, {{ "x", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies Test If") {
    std::string program = "procedure Main123 {\n"
                          "        if (x > 0) then {\n"
                          "            a = 3;\n"
                          "        } else {\n"
                          "            b = x % 5;\n"
                          "        }\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::IF}, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }}},
            {{"2", EntityType::ASSIGN}, {{ "a", EntityType::VARIABLE }}},
            {{"3", EntityType::ASSIGN}, {{ "b", EntityType::VARIABLE }}},
            {{ "Main123", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies Test Basic Statements") {
    std::string program = "procedure MAIN {\n"
                          "        val = 24;\n"
                          "        r = a;\n"
                          "        r = a + x % 5;\n"
                          "        read a;\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::ASSIGN}, {{ "val", EntityType::VARIABLE }}},
            {{"2", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"3", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"4", EntityType::READ}, {{ "a", EntityType::VARIABLE }}},
            {{ "MAIN", EntityType::PROCEDURE }, {{ "r", EntityType::VARIABLE }, { "val", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies Read Test") {
    std::string program = "procedure Example {\n"
                          "        read val;\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::READ}, {{ "val", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "val", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies 3-Nested While Test") {
    std::string program = "procedure Example {\n"
                          "        while (i > 0) {\n"
                          "            r = r;\n"
                          "            while (i > 1) {\n"
                          "                x = x + z * 5;\n"
                          "                while (j > 3) {\n"
                          "                    a = c;\n"
                          "                }\n"
                          "            }\n"
                          "        }\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::WHILE}, {{ "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE },
                                               {"a", EntityType::VARIABLE}}},
            {{"2", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"3", EntityType::WHILE}, {{ "x", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }}},
            {{"4", EntityType::ASSIGN}, {{ "x", EntityType::VARIABLE }}},
            {{"5", EntityType::WHILE}, {{ "a", EntityType::VARIABLE }}},
            {{"6", EntityType::ASSIGN}, {{ "a", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies 2-Nested If Test") {
    std::string program = "procedure Example {\n"
                          "        if (x > 0) then {\n"
                          "            a = 3;\n"
                          "            \n"
                          "            if (c > 2) then {\n"
                          "                d = 4;\n"
                          "            } else {\n"
                          "                e = f % 6;\n"
                          "            }\n"
                          "        } else {\n"
                          "            b = x % 5;\n"
                          "            \n"
                          "            if (g > 7) then {\n"
                          "                h = 8;\n"
                          "            } else {\n"
                          "                i = j % 9;\n"
                          "            }\n"
                          "        }\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::IF}, {{ "a", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }, {"e", EntityType::VARIABLE},
                                     {"d", EntityType::VARIABLE}, {"b", EntityType::VARIABLE}, {"h", EntityType::VARIABLE}}},
            {{"2", EntityType::ASSIGN}, {{ "a", EntityType::VARIABLE }}},
            {{"3", EntityType::IF}, {{ "d", EntityType::VARIABLE }, { "e", EntityType::VARIABLE }}},
            {{"4", EntityType::ASSIGN}, {{"d", EntityType::VARIABLE}}},
            {{"5", EntityType::ASSIGN}, {{ "e", EntityType::VARIABLE }}},
            {{"6", EntityType::ASSIGN}, {{ "b", EntityType::VARIABLE }}},
            {{"7", EntityType::IF}, {{ "h", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"8", EntityType::ASSIGN}, {{ "h", EntityType::VARIABLE }}},
            {{"9", EntityType::ASSIGN}, {{"i", EntityType::VARIABLE}}},
            {{ "Example", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }, { "e", EntityType::VARIABLE },
                                                    { "d", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }, { "h", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies multiple procedures simple") {
    std::string program = "procedure Example {\n"
                          "        r = a + x % 5;\n"
                          "        val = 24;\n"
                          "        r = a;\n"
                          "        print a;\n"
                          "    }\n"
                          "procedure Another {\n"
                          "        r = a + y % 5;\n"
                          "        val2 = 24;\n"
                          "        r = 19;\n"
                          "        print b;\n"
                          "    }";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"2", EntityType::ASSIGN}, {{ "val", EntityType::VARIABLE }}},
            {{"3", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"5", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"6", EntityType::ASSIGN}, {{ "val2", EntityType::VARIABLE }}},
            {{"7", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "r", EntityType::VARIABLE }, { "val", EntityType::VARIABLE }}},
            {{ "Another", EntityType::PROCEDURE }, {{ "r", EntityType::VARIABLE }, { "val2", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Modifies multiple procedures with conditionals") {
    std::string program = "procedure Example {\n"
                          "        r = A + x % 5;\n"
                          "        if (x > 0) then {\n"
                          "            a = 3;\n"
                          "        } else {\n"
                          "            b = x % 5 / a;\n"
                          "        }\n"
                          "    }\n"
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

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"2", EntityType::IF}, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }}},
            {{"3", EntityType::ASSIGN}, {{ "a", EntityType::VARIABLE }}},
            {{"4", EntityType::ASSIGN}, {{ "b", EntityType::VARIABLE }}},
            {{"5", EntityType::WHILE}, {{ "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }}},
            {{"6", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"7", EntityType::WHILE}, {{ "x", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }}},
            {{"8", EntityType::ASSIGN}, {{ "x", EntityType::VARIABLE }}},
            {{"9", EntityType::WHILE}, {{ "c", EntityType::VARIABLE }}},
            {{"10", EntityType::READ}, {{ "c", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "r", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }}},
            {{ "Another", EntityType::PROCEDURE }, {{ "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Modifies] Procedure call variable modifies propagation") {
    std::string program = "procedure Main {\n"
                          "    call Compute;\n"
                          "    read y;\n"
                          "}\n"
                          "procedure Compute {\n"
                          "    read x;\n"
                          "    z = x * 2;\n"
                          "    call Output;\n"
                          "}\n"
                          "procedure Output {\n"
                          "    read procedure;\n"
                          "}";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::CALL}, {{ "z", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "procedure", EntityType::VARIABLE }}},
            {{"2", EntityType::READ}, {{ "y", EntityType::VARIABLE }}},
            {{"3", EntityType::READ}, {{ "x", EntityType::VARIABLE }}},
            {{"4", EntityType::ASSIGN}, {{ "z", EntityType::VARIABLE }}},
            {{"5", EntityType::CALL}, {{ "procedure", EntityType::VARIABLE }}},
            {{"6", EntityType::READ}, {{ "procedure", EntityType::VARIABLE }}},
            {{ "Main", EntityType::PROCEDURE }, {{ "z", EntityType::VARIABLE },  { "y", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "procedure", EntityType::VARIABLE }}},
            {{ "Compute", EntityType::PROCEDURE }, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "procedure", EntityType::VARIABLE }}},
            {{ "Output", EntityType::PROCEDURE }, {{ "procedure", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Modifies] Complex procedure call and conditional variable usage") {
    std::string program =  "procedure Main {\n"
                           "    call Processor;\n"
                           "    if (flag > 0) then {\n"
                           "        call Display;\n"
                           "    } else {\n"
                           "        read error;\n"
                           "    }\n"
                           "}\n"
                           "procedure Processor {\n"
                           "    y = x + 10;\n"
                           "    call Calculator;\n"
                           "}\n"
                           "procedure Calculator {\n"
                           "    z = y * 2;\n"
                           "}\n"
                           "procedure Display {\n"
                           "    read z;\n"
                           "    print something;\n"
                           "}";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::CALL}, {{ "y", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{"2", EntityType::IF}, {{ "z", EntityType::VARIABLE }, { "error", EntityType::VARIABLE }}},
            {{"3", EntityType::CALL}, {{ "z", EntityType::VARIABLE }}},
            {{"4", EntityType::READ}, {{ "error", EntityType::VARIABLE }}},
            {{"5", EntityType::ASSIGN}, {{ "y", EntityType::VARIABLE }}},
            {{"6", EntityType::CALL}, {{ "z", EntityType::VARIABLE }}},
            {{"7", EntityType::ASSIGN}, {{ "z", EntityType::VARIABLE }}},
            {{"8", EntityType::READ}, {{ "z", EntityType::VARIABLE }}},
            {{ "Main", EntityType::PROCEDURE }, {{ "y", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "error", EntityType::VARIABLE }}},
            {{ "Processor", EntityType::PROCEDURE }, {{ "y", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{ "Calculator", EntityType::PROCEDURE }, {{ "z", EntityType::VARIABLE }}},
            {{ "Display", EntityType::PROCEDURE }, {{ "z", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Modifies] Advanced variable usage and edge cases in procedure calls") {
    std::string program = "procedure Main {\n"
                          "    if (cond1 > 0) then {\n"
                          "        call Compute;\n"
                          "    } else {\n"
                          "        call Process;\n"
                          "    }\n"
                          "    call Finalize;\n"
                          "}\n"
                          "procedure Compute {\n"
                          "    a = b + a;\n"
                          "    if (((cond2 < 0) || (!(cond12  != 76))) && (cond2 >= 508989)) then {\n"
                          "        d = a;\n"
                          "    } else {\n"
                          "        call Helper;\n"
                          "    }\n"
                          "}\n"
                          "procedure Process {\n"
                          "    while (!(cond3 == 6)) {\n"
                          "        f = g;\n"
                          "        call Compute;\n"
                          "    }\n"
                          "}\n"
                          "procedure Helper {\n"
                          "    i = 1+k;\n"
                          "}\n"
                          "procedure Finalize {\n"
                          "    result = l;\n"
                          "}";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"Main", EntityType::PROCEDURE}, {{ "a", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "i", EntityType::VARIABLE },
                                               { "f", EntityType::VARIABLE }, { "result", EntityType::VARIABLE }}},
            {{"Compute", EntityType::PROCEDURE}, {{ "a", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"Process", EntityType::PROCEDURE}, {{ "a", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "f", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"Helper", EntityType::PROCEDURE}, {{ "i", EntityType::VARIABLE }}},
            {{"Finalize", EntityType::PROCEDURE}, {{ "result", EntityType::VARIABLE }}},
            {{"1", EntityType::IF}, {{ "a", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "f", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"2", EntityType::CALL}, {{ "a", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"3", EntityType::CALL}, {{ "f", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"4", EntityType::CALL}, {{ "result", EntityType::VARIABLE }}},
            {{"5", EntityType::ASSIGN}, {{ "a", EntityType::VARIABLE }}},
            {{"6", EntityType::IF}, {{ "d", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"7", EntityType::ASSIGN}, {{ "d", EntityType::VARIABLE }}},
            {{"8", EntityType::CALL}, {{ "i", EntityType::VARIABLE }}},
            {{"9", EntityType::WHILE}, {{ "f", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }}},
            {{"10", EntityType::ASSIGN}, {{ "f", EntityType::VARIABLE }}},
            {{"11", EntityType::CALL}, {{ "i", EntityType::VARIABLE }, { "d", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }}},
            {{"12", EntityType::ASSIGN}, {{ "i", EntityType::VARIABLE }}},
            {{"13", EntityType::ASSIGN}, {{ "result", EntityType::VARIABLE }}},
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Modifies] Multiple backward calls with edge cases") {
    std::string program = "procedure Main {\n"
                          "    assign = 5 + 3;\n"
                          "    read read;\n"
                          "    call Output;\n"
                          "}\n"
                          "procedure While {\n"
                          "    read print;\n"
                          "    call Main;\n"
                          "}\n"
                          "procedure Output {\n"
                          "    print procedure;\n"
                          "}\n"
                          "procedure Output2 {\n"
                          "    call = call + 1;\n"
                          "    call While;\n"
                          "}\n"
                          "procedure Output3 {\n"
                          "    call Output2;\n"
                          "}";

    auto modifiesVisitor = getModifiesVisitor(program);
    auto modifiesRecords = modifiesVisitor->getModifies();

    for (const auto &pair: modifiesRecords) {
        std::cout << "Modifies: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") modifies: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::ASSIGN}, {{ "assign", EntityType::VARIABLE }}},
            {{"2", EntityType::READ}, {{ "read", EntityType::VARIABLE }}},
            {{"4", EntityType::READ}, {{ "print", EntityType::VARIABLE }}},
            {{"5", EntityType::CALL}, {{ "read", EntityType::VARIABLE }, { "assign", EntityType::VARIABLE }}},
            {{"7", EntityType::ASSIGN}, {{ "call", EntityType::VARIABLE }}},
            {{"8", EntityType::CALL}, {{ "print", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "assign", EntityType::VARIABLE }}},
            {{"9", EntityType::CALL}, {{ "call", EntityType::VARIABLE }, { "print", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "assign", EntityType::VARIABLE }}},
            {{ "Main", EntityType::PROCEDURE }, {{ "assign", EntityType::VARIABLE },  { "read", EntityType::VARIABLE }}},
            {{ "While", EntityType::PROCEDURE }, {{ "print", EntityType::VARIABLE }, { "assign", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }}},
            {{ "Output2", EntityType::PROCEDURE }, {{ "print", EntityType::VARIABLE }, { "assign", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "call", EntityType::VARIABLE }}},
            {{ "Output3", EntityType::PROCEDURE }, {{ "print", EntityType::VARIABLE }, { "assign", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "call", EntityType::VARIABLE }}}
    };

    REQUIRE(modifiesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = modifiesRecords.find(expectedPair.first);
        REQUIRE(found != modifiesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}
