#include "catch.hpp"
#include <filesystem>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include "program_knowledge_base/storage/StorageManager.h"
#include "sp/pkb_integration/StorageWriter.h"
#include "sp/SP.h"

// ai-gen start(copilot, 1.5, e)
// prompt: used copilot
TEST_CASE("Test insert entity for all managers") {

    std::string testFileName = "./testFile.txt";

    std::string testInput = "procedure test1 {\n"
        "    x = 10;\n"
        "    call test2;\n"
        "    if (a > b) then {\n"
        "        x = x + a;\n"
        "    } else {\n"
        "        x = x + b;\n"
        "    }\n"
        "    y = 3;\n"
        "    while (y < 0) {\n"
        "        print x;\n"
        "        y = y - 1;\n"
        "    }\n"
        "}\n"
        "\n"
        "procedure test2 {\n"
        "    read a;\n"
        "    read b;\n"
        "}";

    std::ofstream testFile;
    testFile.open(testFileName);
    testFile << testInput;
    testFile.close();

    std::shared_ptr<StorageManager> storageManager = std::make_shared<StorageManager>();
    std::shared_ptr<StorageWriter> store = std::make_shared<StorageWriter>(storageManager->getWriteStorage(),
        storageManager->getReadStorage());
    SP sp(storageManager);
    sp.initiateProcessing(testFileName);
    auto readStorage = storageManager->getReadStorage();
    auto entitiesManager = readStorage->getEntitiesManager();

    // test AssignManager
    std::unordered_set<std::string> assignSet = { "1", "4", "5", "6", "9" };
    auto storedAssignSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::ASSIGN);

    REQUIRE(entitiesManager->isPresent("1", EntityType::ASSIGN));
    REQUIRE(entitiesManager->isPresent("4", EntityType::ASSIGN));
    REQUIRE(entitiesManager->isPresent("5", EntityType::ASSIGN));
    REQUIRE(entitiesManager->isPresent("6", EntityType::ASSIGN));
    REQUIRE(entitiesManager->isPresent("9", EntityType::ASSIGN));
    REQUIRE(storedAssignSet.size() == assignSet.size());
    REQUIRE(storedAssignSet == assignSet);

    // test CallProcedureManager
    std::unordered_set<std::string> callProcedureSet = { "test2" };
    auto storedCallProcedureSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::PROCEDURE);

    REQUIRE(entitiesManager->isPresent("test2", EntityType::PROCEDURE));
    REQUIRE(storedCallProcedureSet.size() == callProcedureSet.size());
    REQUIRE(storedCallProcedureSet == callProcedureSet);

    // test CallStmtNoManager
    std::unordered_set<std::string> callStmtNoSet = { "2" };
    auto storedCallStmtNoSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::STMT);

    REQUIRE(entitiesManager->isPresent("2", EntityType::STMT));
    REQUIRE(storedCallStmtNoSet.size() == callStmtNoSet.size());
    REQUIRE(storedCallStmtNoSet == callStmtNoSet);

    // test ConstantManager
    std::unordered_set<std::string> constantSet = { "0", "1", "3", "10"};
    auto storedConstantSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::CONSTANT);

    REQUIRE(entitiesManager->isPresent("0", EntityType::CONSTANT));
    REQUIRE(entitiesManager->isPresent("1", EntityType::CONSTANT));
    REQUIRE(entitiesManager->isPresent("3", EntityType::CONSTANT));
    REQUIRE(entitiesManager->isPresent("10", EntityType::CONSTANT));
    REQUIRE(storedConstantSet.size() == constantSet.size());
    REQUIRE(storedConstantSet == constantSet);

    // test IfManager
    std::unordered_set<std::string> ifSet = { "3" };
    auto storedIfSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::IF);

    REQUIRE(entitiesManager->isPresent("3", EntityType::IF));
    REQUIRE(storedIfSet.size() == ifSet.size());
    REQUIRE(storedIfSet == ifSet);

    // test PrintStmtNoManager
    std::unordered_set<std::string> printStmtNoSet = { "8" };
    auto storedPrintStmtNoSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::PRINT);

    REQUIRE(entitiesManager->isPresent("8", EntityType::PRINT));
    REQUIRE(storedPrintStmtNoSet.size() == printStmtNoSet.size());
    REQUIRE(storedPrintStmtNoSet == printStmtNoSet);

    // test PrintVariableManager
    std::unordered_set<std::string> printVariableSet = { "x" };
    auto storedPrintVariableSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::PRINT);

    REQUIRE(entitiesManager->isPresent("x", EntityType::PRINT));
    REQUIRE(storedPrintVariableSet.size() == printVariableSet.size());
    REQUIRE(storedPrintVariableSet == printVariableSet);

    // test ProcedureManager
    std::unordered_set<std::string> procedureSet = { "test1", "test2" };
    auto storedProcedureSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::PROCEDURE);

    REQUIRE(entitiesManager->isPresent("test1", EntityType::PROCEDURE));
    REQUIRE(entitiesManager->isPresent("test2", EntityType::PROCEDURE));
    REQUIRE(storedProcedureSet.size() == procedureSet.size());
    REQUIRE(storedProcedureSet == procedureSet);

    // test ReadStmtNoManager
    std::unordered_set<std::string> readStmtNoSet = { "10", "11" };
    auto storedReadStmtNoSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::READ);

    REQUIRE(entitiesManager->isPresent("10", EntityType::READ));
    REQUIRE(entitiesManager->isPresent("11", EntityType::READ));
    REQUIRE(storedReadStmtNoSet.size() == readStmtNoSet.size());
    REQUIRE(storedReadStmtNoSet == readStmtNoSet);

    // test ReadVariableManager
    std::unordered_set<std::string> readVariableSet = { "a", "b" };
    auto storedReadVariableSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::READ);

    REQUIRE(entitiesManager->isPresent("a", EntityType::READ));
    REQUIRE(entitiesManager->isPresent("b", EntityType::READ));
    REQUIRE(storedReadVariableSet.size() == readVariableSet.size());
    REQUIRE(storedReadVariableSet == readVariableSet);

    // test StmtManager
    std::unordered_set<std::string> stmtSet = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
    auto storedStmtSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::STMT);

    REQUIRE(entitiesManager->isPresent("1", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("2", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("3", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("4", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("5", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("6", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("7", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("8", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("9", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("10", EntityType::STMT));
    REQUIRE(entitiesManager->isPresent("11", EntityType::STMT));
    REQUIRE(storedStmtSet.size() == stmtSet.size());
    REQUIRE(storedStmtSet == stmtSet);

    // test VariableManager
    std::unordered_set<std::string> variableSet = { "a", "b", "x", "y" };
    auto storedVariableSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::VARIABLE);

    REQUIRE(entitiesManager->isPresent("a", EntityType::VARIABLE));
    REQUIRE(entitiesManager->isPresent("b", EntityType::VARIABLE));
    REQUIRE(entitiesManager->isPresent("x", EntityType::VARIABLE));
    REQUIRE(entitiesManager->isPresent("y", EntityType::VARIABLE));
    REQUIRE(storedVariableSet.size() == variableSet.size());
    REQUIRE(storedVariableSet == variableSet);

    // test WhileManager
    std::unordered_set<std::string> whileSet = { "7" };
    auto storedWhileSet = entitiesManager->getSpecificEntityTypeEntries(EntityType::WHILE);

    REQUIRE(entitiesManager->isPresent("7", EntityType::WHILE));
    REQUIRE(storedWhileSet.size() == whileSet.size());
    REQUIRE(storedWhileSet == whileSet);

    std::filesystem::remove(testFileName);
}
// ai-gen end
