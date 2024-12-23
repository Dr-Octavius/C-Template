#include "../../include/catch.hpp"

#include "sp/Tokenizer/CompositeLexer.h"
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
#include "common/Utils.h"
#include <utility>
#include <unordered_set>

std::shared_ptr<UsesVisitor> getUsesVisitor(const std::string& program) {
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
    return usesVisitor;
}

TEST_CASE("Uses Test Basic Statements") {
    std::string program = "procedure Example {\n"
                          "        r = a + x % 5;\n"
                          "        val = 24;\n"
                          "        r = a;\n"
                          "        print a;\n"
                          "    }";

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
            {{ "3", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }}},
            {{ "4", EntityType::PRINT }, {{ "a", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Uses Test If") {
    std::string program = "procedure S1THINGyes {\n"
                          "        if (x > 0) then {\n"
                          "            a = 3;\n"
                          "        } else {\n"
                          "            b = x % 5;\n"
                          "        }\n"
                          "    }";

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::IF }, {{ "x", EntityType::VARIABLE }}},
            {{ "3", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }}},
            {{ "S1THINGyes", EntityType::PROCEDURE }, {{ "x", EntityType::VARIABLE }}}
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Uses Test While with Constant in Condition") {
    std::string program = "procedure Example {\n"
                          "        while (i > 0) {\n"
                          "            r = r;\n"
                          "            x = x + z * 5;\n"
                          "        }\n"
                          "    }";

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::WHILE}, {{ "i", EntityType::VARIABLE },{"r", EntityType::VARIABLE}, { "x", EntityType::VARIABLE },{ "z", EntityType::VARIABLE }}},
            {{"2", EntityType::ASSIGN}, {{ "r", EntityType::VARIABLE }}},
            {{"3", EntityType::ASSIGN}, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "i", EntityType::VARIABLE },{ "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE },{ "z", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Uses Test While with Variable in Condition") {
    std::string program = "procedure Example {\n"
                          "        while (o > i) {\n"
                          "            a = r;\n"
                          "            x = x + z * 5;\n"
                          "        }\n"
                          "    }";

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::WHILE }, {{ "z", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }, { "o", EntityType::VARIABLE }, { "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
            {{ "2", EntityType::ASSIGN }, {{ "r", EntityType::VARIABLE }}},
            {{ "3", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "z", EntityType::VARIABLE }, { "i", EntityType::VARIABLE }, { "o", EntityType::VARIABLE }, { "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Uses 3-Nested While Test") {
    std::string program = "procedure MaIn {\n"
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

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::WHILE }, {{ "i", EntityType::VARIABLE }, { "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, {"z", EntityType::VARIABLE}, { "j", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }}},
            {{ "2", EntityType::ASSIGN }, {{ "r", EntityType::VARIABLE }}},
            {{ "3", EntityType::WHILE }, {{ "i", EntityType::VARIABLE }, {"x", EntityType::VARIABLE}, {"j", EntityType::VARIABLE}, {"c", EntityType::VARIABLE}, { "z", EntityType::VARIABLE }}},
            {{ "4", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{ "5", EntityType::WHILE }, {{ "j", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }}},
            {{ "6", EntityType::ASSIGN }, {{ "c", EntityType::VARIABLE }}},
            {{ "MaIn", EntityType::PROCEDURE }, {{ "i", EntityType::VARIABLE }, { "r", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, {"z", EntityType::VARIABLE}, { "j", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Uses 2-Nested If Test") {
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

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::IF }, {{ "x", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }, { "j", EntityType::VARIABLE }, { "f", EntityType::VARIABLE }, { "g", EntityType::VARIABLE }}},
            {{ "3", EntityType::IF }, {{ "c", EntityType::VARIABLE}, { "f", EntityType::VARIABLE }}},
            {{ "5", EntityType::ASSIGN }, {{ "f", EntityType::VARIABLE }}},
            {{ "6", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }}},
            {{ "7", EntityType::IF }, {{ "g", EntityType::VARIABLE }, { "j", EntityType::VARIABLE }}},
            {{ "9", EntityType::ASSIGN }, {{ "j", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "x", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }, { "j", EntityType::VARIABLE }, { "f", EntityType::VARIABLE }, { "g", EntityType::VARIABLE }}}
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Uses multiple procedures simple") {
    std::string program = "procedure Example {\n"
                          "        r = a + x % 5;\n"
                          "        val = 24;\n"
                          "        r = a;\n"
                          "        print a;\n"
                          "    }\n"
                          "procedure Another {\n"
                          "        r = a + y % 5;\n"
                          "        val = 24;\n"
                          "        r = 19;\n"
                          "        print b;\n"
                          "    }";

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
            {{ "3", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }}},
            {{ "4", EntityType::PRINT }, {{ "a", EntityType::VARIABLE }}},
            {{ "5", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }, { "y", EntityType::VARIABLE }}},
            {{ "8", EntityType::PRINT }, {{ "b", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
            {{ "Another", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "y", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("Uses multiple procedures with conditionals") {
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
                          "                    print c;\n"
                          "                }\n"
                          "            }\n"
                          "        }\n"
                          "  print t;\n"
                          "    }";

    auto usesVisitor = getUsesVisitor(program);
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::ASSIGN }, {{ "A", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }}},
            {{ "2", EntityType::IF }, {{ "x", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }}},
            {{ "4", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }}},
            {{ "5", EntityType::WHILE }, {{ "i", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }, { "j", EntityType::VARIABLE },
                                          { "J", EntityType::VARIABLE }}},
            {{ "6", EntityType::ASSIGN }, {{ "J", EntityType::VARIABLE }}},
            {{ "7", EntityType::WHILE }, {{ "i", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }, { "j", EntityType::VARIABLE }}},
            {{ "8", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{ "9", EntityType::WHILE }, {{ "c", EntityType::VARIABLE }, { "j", EntityType::VARIABLE }}},
            {{ "10", EntityType::PRINT }, {{ "c", EntityType::VARIABLE }}},
            {{ "11", EntityType::PRINT }, {{ "t", EntityType::VARIABLE }}},
            {{ "Example", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "A", EntityType::VARIABLE }}},
            {{ "Another", EntityType::PROCEDURE }, {{ "i", EntityType::VARIABLE }, { "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "c", EntityType::VARIABLE }, { "j", EntityType::VARIABLE },
                                                    { "J", EntityType::VARIABLE }, { "t", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Uses] Procedure call variable usage propagation") {
    std::string program =  "procedure Main {\n"
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

    auto usesVisitor = getUsesVisitor(program);
    auto usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::CALL }, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{ "2", EntityType::PRINT }, {{ "y", EntityType::VARIABLE }}},
            {{ "4", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }}},
            {{ "5", EntityType::CALL }, {{ "z", EntityType::VARIABLE }}},
            {{ "6", EntityType::PRINT }, {{ "z", EntityType::VARIABLE }}},
            {{ "Main", EntityType::PROCEDURE }, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "y", EntityType::VARIABLE }}},
            {{ "Compute", EntityType::PROCEDURE }, {{ "x", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }}},
            {{ "Output", EntityType::PROCEDURE }, {{ "z", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Uses] Complex procedure call and conditional variable usage") {
    std::string program =  "procedure Main {\n"
                           "    call Processor;\n"
                           "    if (flag > 0) then {\n"
                           "        call Display;\n"
                           "    } else {\n"
                           "        print error;\n"
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
                           "    print z;\n"
                           "}";

    auto usesVisitor = getUsesVisitor(program);
    auto usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::CALL }, {{ "x", EntityType::VARIABLE }, { "y", EntityType::VARIABLE }}},
            {{ "2", EntityType::IF }, {{ "flag", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "error", EntityType::VARIABLE }}},
            {{ "3", EntityType::CALL }, {{ "z", EntityType::VARIABLE }}},
            {{ "4", EntityType::PRINT }, {{ "error", EntityType::VARIABLE }}},
            {{ "5", EntityType::ASSIGN }, {{ "x", EntityType::VARIABLE }}},
            {{ "6", EntityType::CALL }, {{ "y", EntityType::VARIABLE }}},
            {{ "7", EntityType::ASSIGN }, {{ "y", EntityType::VARIABLE }}},
            {{ "8", EntityType::PRINT }, {{ "z", EntityType::VARIABLE }}},
            {{ "Main", EntityType::PROCEDURE }, {{ "x", EntityType::VARIABLE }, { "y", EntityType::VARIABLE }, { "z", EntityType::VARIABLE }, { "flag", EntityType::VARIABLE }, { "error", EntityType::VARIABLE }}},
            {{ "Processor", EntityType::PROCEDURE }, {{ "x", EntityType::VARIABLE }, { "y", EntityType::VARIABLE }}},
            {{ "Calculator", EntityType::PROCEDURE }, {{ "y", EntityType::VARIABLE }}},
            {{ "Display", EntityType::PROCEDURE }, {{ "z", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Uses] Advanced variable usage and edge cases in procedure calls") {
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

    auto usesVisitor = getUsesVisitor(program);
    auto usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{ "1", EntityType::IF }, {{ "cond1", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE },
                                       { "k", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE }, { "cond3", EntityType::VARIABLE },
                                       { "g", EntityType::VARIABLE }, { "cond12", EntityType::VARIABLE }}},
            {{ "2", EntityType::CALL }, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE },
                                         { "cond12", EntityType::VARIABLE }, { "k", EntityType::VARIABLE }}},
            {{ "3", EntityType::CALL }, {{ "cond3", EntityType::VARIABLE }, { "g", EntityType::VARIABLE }, { "a", EntityType::VARIABLE },
                                         { "b", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE }, { "cond12", EntityType::VARIABLE },
                                         { "k", EntityType::VARIABLE }}},
            {{ "4", EntityType::CALL }, {{ "l", EntityType::VARIABLE }}},
            {{ "5", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }}},
            {{ "6", EntityType::IF }, {{ "cond2", EntityType::VARIABLE }, { "cond12", EntityType::VARIABLE }, { "a", EntityType::VARIABLE },
                                       { "k", EntityType::VARIABLE }}},
            {{ "7", EntityType::ASSIGN }, {{ "a", EntityType::VARIABLE }}},
            {{ "8", EntityType::CALL }, {{ "k", EntityType::VARIABLE }}},
            {{ "9", EntityType::WHILE }, {{ "cond3", EntityType::VARIABLE }, { "g", EntityType::VARIABLE }, { "a", EntityType::VARIABLE },
                                          { "b", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE }, { "cond12", EntityType::VARIABLE },
                                          { "k", EntityType::VARIABLE }}},
            {{ "10", EntityType::ASSIGN }, {{ "g", EntityType::VARIABLE }}},
            {{ "11", EntityType::CALL }, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE },
                                          { "cond12", EntityType::VARIABLE }, { "k", EntityType::VARIABLE }}},
            {{ "12", EntityType::ASSIGN }, {{ "k", EntityType::VARIABLE }}},
            {{ "13", EntityType::ASSIGN }, {{ "l", EntityType::VARIABLE }}},
            {{ "Main", EntityType::PROCEDURE }, {{ "cond1", EntityType::VARIABLE }, { "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE },
                                              { "k", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE }, { "cond3", EntityType::VARIABLE },
                                              { "g", EntityType::VARIABLE }, { "cond12", EntityType::VARIABLE }, { "l", EntityType::VARIABLE }}},
            {{ "Compute", EntityType::PROCEDURE }, {{ "a", EntityType::VARIABLE }, { "b", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE },
                                              { "cond12", EntityType::VARIABLE }, { "k", EntityType::VARIABLE }}},
            {{ "Process", EntityType::PROCEDURE }, {{ "cond3", EntityType::VARIABLE }, { "g", EntityType::VARIABLE }, { "a", EntityType::VARIABLE },
                                              { "b", EntityType::VARIABLE }, { "cond2", EntityType::VARIABLE }, { "cond12", EntityType::VARIABLE },
                                              { "k", EntityType::VARIABLE }}},
            {{ "Helper", EntityType::PROCEDURE }, {{ "k", EntityType::VARIABLE }}},
            {{ "Finalize", EntityType::PROCEDURE }, {{ "l", EntityType::VARIABLE }}},
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}

TEST_CASE("[Uses] Multiple backward calls with edge cases") {
    std::string program = "procedure Main {\n"
                          "    assign = if + 3;\n"
                          "    print read;\n"
                          "    call Output;\n"
                          "}\n"
                          "procedure While {\n"
                          "    print print;\n"
                          "    call Main;\n"
                          "}\n"
                          "procedure Output {\n"
                          "    read procedure;\n"
                          "}\n"
                          "procedure Output2 {\n"
                          "    call = call + 1;\n"
                          "    call While;\n"
                          "}\n"
                          "procedure Output3 {\n"
                          "    call Output2;\n"
                          "}";

    auto usesVisitor = getUsesVisitor(program);
    auto usesRecords = usesVisitor->getUses();

    for (const auto &pair: usesRecords) {
        std::cout << "Uses: " << pair.first.first << " (" << entityTypeToString(pair.first.second)
                  << ") uses: ";
        for (const auto &follower: pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> expected{
            {{"1", EntityType::ASSIGN}, {{ "if", EntityType::VARIABLE }}},
            {{"2", EntityType::PRINT}, {{ "read", EntityType::VARIABLE }}},
            {{"4", EntityType::PRINT}, {{ "print", EntityType::VARIABLE }}},
            {{"5", EntityType::CALL}, {{ "read", EntityType::VARIABLE }, { "if", EntityType::VARIABLE }}},
            {{"7", EntityType::ASSIGN}, {{ "call", EntityType::VARIABLE }}},
            {{"8", EntityType::CALL}, {{ "print", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "if", EntityType::VARIABLE }}},
            {{"9", EntityType::CALL}, {{ "call", EntityType::VARIABLE }, { "print", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "if", EntityType::VARIABLE }}},
            {{ "Main", EntityType::PROCEDURE }, {{ "if", EntityType::VARIABLE },  { "read", EntityType::VARIABLE }}},
            {{ "While", EntityType::PROCEDURE }, {{ "print", EntityType::VARIABLE }, { "if", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }}},
            {{ "Output2", EntityType::PROCEDURE }, {{ "print", EntityType::VARIABLE }, { "if", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "call", EntityType::VARIABLE }}},
            {{ "Output3", EntityType::PROCEDURE }, {{ "print", EntityType::VARIABLE }, { "if", EntityType::VARIABLE }, { "read", EntityType::VARIABLE }, { "call", EntityType::VARIABLE }}}
    };

    REQUIRE(usesRecords.size() == expected.size());

    for (const auto& expectedPair : expected) {
        auto found = usesRecords.find(expectedPair.first);
        REQUIRE(found != usesRecords.end());
        REQUIRE(found->second == expectedPair.second);
    }
}
