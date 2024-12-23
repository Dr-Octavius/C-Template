#include "catch.hpp"
#include <string>
#include <utility>
#include "program_knowledge_base/storage/StorageManager.h"
#include "qps/MainQueryProcessor.h"
#include "../helpers/stringIntComparator.h"
/*
// ai-gen start(copilot, 1.5, e)
// prompt: used copilot
TEST_CASE("Test reading entity for all manager") {
    std::unique_ptr<StorageManager> storageManager = std::make_unique<StorageManager>();
    MainQueryProcessor;
    auto writeStorage = storageManager->getWriteStorage();

    // populate PKB
    auto entitiesManager = writeStorage->getEntitiesManager();
    entitiesManager->insertEntity("1", EntityType::ASSIGN);
    entitiesManager->insertEntity("4", EntityType::ASSIGN);
    entitiesManager->insertEntity("5", EntityType::ASSIGN);
    entitiesManager->insertEntity("6", EntityType::ASSIGN);
    entitiesManager->insertEntity("9", EntityType::ASSIGN);

    entitiesManager->insertEntity("test2", EntityType::PROCEDURE);

    entitiesManager->insertEntity("2", EntityType::CALL);

    entitiesManager->insertEntity("0", EntityType::CONSTANT);
    entitiesManager->insertEntity("1", EntityType::CONSTANT);
    entitiesManager->insertEntity("3", EntityType::CONSTANT);
    entitiesManager->insertEntity("10", EntityType::CONSTANT);

    entitiesManager->insertEntity("3", EntityType::IF);

    entitiesManager->insertEntity("8", EntityType::PRINT);

    entitiesManager->insertEntity("x", EntityType::PRINT);

    entitiesManager->insertEntity("test1", EntityType::PROCEDURE);
    entitiesManager->insertEntity("test2", EntityType::PROCEDURE);

    entitiesManager->insertEntity("10", EntityType::READ);
    entitiesManager->insertEntity("11", EntityType::READ);

    entitiesManager->insertEntity("1", EntityType::STMT);
    entitiesManager->insertEntity("2", EntityType::STMT);
    entitiesManager->insertEntity("3", EntityType::STMT);
    entitiesManager->insertEntity("4", EntityType::STMT);
    entitiesManager->insertEntity("5", EntityType::STMT);
    entitiesManager->insertEntity("6", EntityType::STMT);
    entitiesManager->insertEntity("7", EntityType::STMT);
    entitiesManager->insertEntity("8", EntityType::STMT);
    entitiesManager->insertEntity("9", EntityType::STMT);
    entitiesManager->insertEntity("10", EntityType::STMT);
    entitiesManager->insertEntity("11", EntityType::STMT);

    entitiesManager->insertEntity("a", EntityType::VARIABLE);
    entitiesManager->insertEntity("b", EntityType::VARIABLE);
    entitiesManager->insertEntity("x", EntityType::VARIABLE);
    entitiesManager->insertEntity("y", EntityType::VARIABLE);

    entitiesManager->insertEntity("7", EntityType::WHILE);

    // test Assign 
    std::string q1 = "assign a; Select a";
    auto q1ResultsSet = MainQueryProcessor::processQuery(q1, storageManager->getReadStorage());

    std::list<std::string> q1Results(q1ResultsSet.begin(), q1ResultsSet.end());

    std::list<std::string> a1Results = { "1", "4", "5", "6", "9" };
    q1Results.sort(stringIntComparator);
    REQUIRE(q1Results == a1Results);

    //// test Call
    //std::string q2 = "call cl; Select cl";
    //auto q2ResultsSet = MainQueryProcessor::processQuery(q2, storageManager->getReadStorage());

    //std::list<std::string> q2Results(q2ResultsSet.begin(), q2ResultsSet.end());

    //std::list<std::string> a2Results = { "2" };
    //q2Results.sort(stringIntComparator);
    //REQUIRE(q2Results == a2Results);

    // test Constant
    std::string q3 = "constant c; Select c";
    auto q3ResultsSet = MainQueryProcessor::processQuery(q3, storageManager->getReadStorage());

    std::list<std::string> q3Results(q3ResultsSet.begin(), q3ResultsSet.end());

    std::list<std::string> a3Results = { "0", "1", "3", "10" };
    q3Results.sort(stringIntComparator);
    REQUIRE(q3Results == a3Results);

    // test If
    std::string q4 = "if ifs; Select ifs";
    auto q4ResultsSet = MainQueryProcessor::processQuery(q4, storageManager->getReadStorage());

    std::list<std::string> q4Results(q4ResultsSet.begin(), q4ResultsSet.end());

    std::list<std::string> a4Results = { "3" };
    q4Results.sort(stringIntComparator);
    REQUIRE(q4Results == a4Results);

    // test Print
    std::string q5 = "print pn; Select pn";
    auto q5ResultsSet = MainQueryProcessor::processQuery(q5, storageManager->getReadStorage());

    std::list<std::string> q5Results(q5ResultsSet.begin(), q5ResultsSet.end());

    std::list<std::string> a5Results = { "8", "x" };
    q5Results.sort(stringIntComparator);
    REQUIRE(q5Results == a5Results);

    // test Procedure
    std::string q6 = "procedure p; Select p";
    auto q6ResultsSet = MainQueryProcessor::processQuery(q6, storageManager->getReadStorage());

    std::list<std::string> q6Results(q6ResultsSet.begin(), q6ResultsSet.end());

    std::list<std::string> a6Results = { "test1", "test2" };
    q6Results.sort(stringIntComparator);
    REQUIRE(q6Results == a6Results);

    // test Read
    std::string q7 = "read r; Select r";
    auto q7ResultsSet = MainQueryProcessor::processQuery(q7, storageManager->getReadStorage());

    std::list<std::string> q7Results(q7ResultsSet.begin(), q7ResultsSet.end());

    std::list<std::string> a7Results = { "10", "11" };
    q7Results.sort(stringIntComparator);
    REQUIRE(q7Results == a7Results);

    // test Stmt
    std::string q8 = "stmt s; Select s";
    auto q8ResultsSet = MainQueryProcessor::processQuery(q8, storageManager->getReadStorage());

    std::list<std::string> a8Results = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" , "x"};

    std::list<std::string> q8Results(q8ResultsSet.begin(), q8ResultsSet.end());

    q8Results.sort(stringIntComparator);
    REQUIRE(q8Results == a8Results);

    // test Variable
    std::string q9 = "variable v; Select v";
    auto q9ResultsSet = MainQueryProcessor::processQuery(q9, storageManager->getReadStorage());

    std::list<std::string> q9Results(q9ResultsSet.begin(), q9ResultsSet.end());

    std::list<std::string> a9Results = { "a", "b", "x", "y" };
    q9Results.sort(stringIntComparator);
    REQUIRE(q9Results == a9Results);

    // test While
    std::string q10 = "while w; Select w";
    auto q10ResultsSet = MainQueryProcessor::processQuery(q10, storageManager->getReadStorage());

    std::list<std::string> q10Results(q10ResultsSet.begin(), q10ResultsSet.end());

    std::list<std::string> a10Results = { "7" };
    REQUIRE(q10Results == a10Results);
}
// ai-gen end
*/