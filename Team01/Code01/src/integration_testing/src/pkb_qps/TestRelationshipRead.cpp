#include "catch.hpp"
#include <string>
#include <utility>
#include "program_knowledge_base/storage/StorageManager.h"
#include "qps/MainQueryProcessor.h"
#include "../helpers/stringIntComparator.h"
/*
// ai-gen start(copilot, 1.5, e)
// prompt: used copilot
TEST_CASE("Test reading relationships for all managers") {
    std::shared_ptr<StorageManager> storageManager = std::make_shared<StorageManager>();
    MainQueryProcessor;
    auto writeStorage = storageManager->getWriteStorage();

    // populate PKB
    auto entitiesManager = writeStorage->getEntitiesManager();
    entitiesManager->insertEntity("2", EntityType::ASSIGN);
    entitiesManager->insertEntity("4", EntityType::ASSIGN);
    entitiesManager->insertEntity("6", EntityType::ASSIGN);
    entitiesManager->insertEntity("7", EntityType::ASSIGN);
    entitiesManager->insertEntity("11", EntityType::ASSIGN);
    entitiesManager->insertEntity("13", EntityType::ASSIGN);
    entitiesManager->insertEntity("14", EntityType::ASSIGN);

    entitiesManager->insertEntity("15", EntityType::CALL);
    entitiesManager->insertEntity("17", EntityType::CALL);

    entitiesManager->insertEntity("0", EntityType::CONSTANT);
    entitiesManager->insertEntity("1", EntityType::CONSTANT);
    entitiesManager->insertEntity("2", EntityType::CONSTANT);

    entitiesManager->insertEntity("3", EntityType::IF);
    entitiesManager->insertEntity("9", EntityType::IF);

    entitiesManager->insertEntity("5", EntityType::PRINT);
    entitiesManager->insertEntity("10", EntityType::PRINT);
    entitiesManager->insertEntity("12", EntityType::PRINT);

    entitiesManager->insertEntity("test1", EntityType::PROCEDURE);
    entitiesManager->insertEntity("test2", EntityType::PROCEDURE);
    entitiesManager->insertEntity("test3", EntityType::PROCEDURE);

    entitiesManager->insertEntity("1", EntityType::READ);
    entitiesManager->insertEntity("16", EntityType::READ);
    entitiesManager->insertEntity("18", EntityType::READ);

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
    entitiesManager->insertEntity("12", EntityType::STMT);
    entitiesManager->insertEntity("13", EntityType::STMT);
    entitiesManager->insertEntity("14", EntityType::STMT);
    entitiesManager->insertEntity("15", EntityType::STMT);
    entitiesManager->insertEntity("16", EntityType::STMT);
    entitiesManager->insertEntity("17", EntityType::STMT);
    entitiesManager->insertEntity("18", EntityType::STMT);

    entitiesManager->insertEntity("a", EntityType::VARIABLE);
    entitiesManager->insertEntity("b", EntityType::VARIABLE);
    entitiesManager->insertEntity("c", EntityType::VARIABLE);
    entitiesManager->insertEntity("x", EntityType::VARIABLE);
    entitiesManager->insertEntity("y", EntityType::VARIABLE);

    entitiesManager->insertEntity("8", EntityType::WHILE);

    auto followsManager = writeStorage->getFollowsManager();
    followsManager->insertRelationship({ "1", EntityType::STMT }, { "2", EntityType::STMT });
    followsManager->insertRelationship({ "2", EntityType::STMT }, { "3", EntityType::STMT });
    followsManager->insertRelationship({ "4", EntityType::STMT }, { "5", EntityType::STMT });
    followsManager->insertRelationship({ "3", EntityType::STMT }, { "7", EntityType::STMT });
    followsManager->insertRelationship({ "7", EntityType::STMT }, { "8", EntityType::STMT });
    followsManager->insertRelationship({ "8", EntityType::STMT }, { "14", EntityType::STMT });
    followsManager->insertRelationship({ "9", EntityType::STMT }, { "13", EntityType::STMT });
    followsManager->insertRelationship({ "11", EntityType::STMT }, { "12", EntityType::STMT });
    followsManager->insertRelationship({ "14", EntityType::STMT }, { "15", EntityType::STMT });
    followsManager->insertRelationship({ "16", EntityType::STMT }, { "17", EntityType::STMT });

    auto followsTManager = writeStorage->getFollowsTManager();
    followsTManager->insertRelationship({ "1", EntityType::STMT }, { "2", EntityType::STMT });
    followsTManager->insertRelationship({ "1", EntityType::STMT }, { "3", EntityType::STMT });
    followsTManager->insertRelationship({ "1", EntityType::STMT }, { "7", EntityType::STMT });
    followsTManager->insertRelationship({ "1", EntityType::STMT }, { "8", EntityType::STMT });
    followsTManager->insertRelationship({ "1", EntityType::STMT }, { "14", EntityType::STMT });
    followsTManager->insertRelationship({ "1", EntityType::STMT }, { "15", EntityType::STMT });
    followsTManager->insertRelationship({ "2", EntityType::STMT }, { "3", EntityType::STMT });
    followsTManager->insertRelationship({ "2", EntityType::STMT }, { "7", EntityType::STMT });
    followsTManager->insertRelationship({ "2", EntityType::STMT }, { "8", EntityType::STMT });
    followsTManager->insertRelationship({ "2", EntityType::STMT }, { "14", EntityType::STMT });
    followsTManager->insertRelationship({ "2", EntityType::STMT }, { "15", EntityType::STMT });
    followsTManager->insertRelationship({ "4", EntityType::STMT }, { "5", EntityType::STMT });
    followsTManager->insertRelationship({ "3", EntityType::STMT }, { "7", EntityType::STMT });
    followsTManager->insertRelationship({ "3", EntityType::STMT }, { "8", EntityType::STMT });
    followsTManager->insertRelationship({ "3", EntityType::STMT }, { "14", EntityType::STMT });

    followsTManager->insertRelationship({ "3", EntityType::STMT }, { "15", EntityType::STMT });
    followsTManager->insertRelationship({ "7", EntityType::STMT }, { "8", EntityType::STMT });
    followsTManager->insertRelationship({ "7", EntityType::STMT }, { "14", EntityType::STMT });
    followsTManager->insertRelationship({ "7", EntityType::STMT }, { "15", EntityType::STMT });
    followsTManager->insertRelationship({ "8", EntityType::STMT }, { "14", EntityType::STMT });
    followsTManager->insertRelationship({ "8", EntityType::STMT }, { "15", EntityType::STMT });
    followsTManager->insertRelationship({ "9", EntityType::STMT }, { "13", EntityType::STMT });
    followsTManager->insertRelationship({ "11", EntityType::STMT }, { "12", EntityType::STMT });
    followsTManager->insertRelationship({ "14", EntityType::STMT }, { "15", EntityType::STMT });
    followsTManager->insertRelationship({ "16", EntityType::STMT }, { "17", EntityType::STMT });

    auto modifiesManager = writeStorage->getModifiesManager();
    modifiesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "a", EntityType::VARIABLE });
    modifiesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "b", EntityType::VARIABLE });
    modifiesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "c", EntityType::VARIABLE });
    modifiesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "x", EntityType::VARIABLE });
    modifiesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "y", EntityType::VARIABLE });
    modifiesManager->insertRelationship({ "test2", EntityType::PROCEDURE }, { "b", EntityType::VARIABLE });
    modifiesManager->insertRelationship({ "test2", EntityType::PROCEDURE }, { "c", EntityType::VARIABLE });
    modifiesManager->insertRelationship({ "test3", EntityType::PROCEDURE }, { "c", EntityType::VARIABLE });

    auto parentManager = writeStorage->getParentManager();
    parentManager->insertRelationship({ "3", EntityType::IF }, { "4", EntityType::STMT });
    parentManager->insertRelationship({ "3", EntityType::IF }, { "5", EntityType::STMT });
    parentManager->insertRelationship({ "3", EntityType::IF }, { "6", EntityType::STMT });
    parentManager->insertRelationship({ "8", EntityType::PRINT }, { "9", EntityType::STMT });
    parentManager->insertRelationship({ "8", EntityType::PRINT }, { "13", EntityType::STMT });
    parentManager->insertRelationship({ "9", EntityType::STMT }, { "10", EntityType::STMT });
    parentManager->insertRelationship({ "9", EntityType::STMT }, { "11", EntityType::STMT });
    parentManager->insertRelationship({ "9", EntityType::STMT }, { "12", EntityType::STMT });

    auto parentTManager = writeStorage->getParentTManager();
    parentTManager->insertRelationship({ "3", EntityType::IF }, { "4", EntityType::STMT });
    parentTManager->insertRelationship({ "3", EntityType::IF }, { "5", EntityType::STMT });
    parentTManager->insertRelationship({ "3", EntityType::IF }, { "6", EntityType::STMT });
    parentTManager->insertRelationship({ "8", EntityType::PRINT }, { "9", EntityType::STMT });
    parentTManager->insertRelationship({ "8", EntityType::PRINT }, { "10", EntityType::STMT });
    parentTManager->insertRelationship({ "8", EntityType::PRINT }, { "11", EntityType::STMT });
    parentTManager->insertRelationship({ "8", EntityType::PRINT }, { "12", EntityType::STMT });
    parentTManager->insertRelationship({ "8", EntityType::PRINT }, { "13", EntityType::STMT });
    parentTManager->insertRelationship({ "9", EntityType::STMT }, { "10", EntityType::STMT });
    parentTManager->insertRelationship({ "9", EntityType::STMT }, { "11", EntityType::STMT });
    parentTManager->insertRelationship({ "9", EntityType::STMT }, { "12", EntityType::STMT });

    auto usesManager = writeStorage->getUsesManager();
    usesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "a", EntityType::VARIABLE });
    usesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "x", EntityType::VARIABLE });
    usesManager->insertRelationship({ "test1", EntityType::PROCEDURE }, { "y", EntityType::VARIABLE });

    // test Follows
    std::string q1a = "stmt s; Select s such that Follows(1, s)";
    auto q1aResultsSet = MainQueryProcessor::processQuery(q1a, storageManager->getReadStorage());

    std::list<std::string> q1aResults(q1aResultsSet.begin(), q1aResultsSet.end());

    std::list<std::string> a1aResults = { "2" };
    REQUIRE(q1aResults == a1aResults);

    std::string q1b = "stmt s; Select s such that Follows(1, 2)";
    auto q1bResultsSet = MainQueryProcessor::processQuery(q1b, storageManager->getReadStorage());

    std::list<std::string> q1bResults(q1bResultsSet.begin(), q1bResultsSet.end());

    std::list<std::string> a1bResults = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
                                         "14", "15", "16", "17", "18" };
    q1bResults.sort(stringIntComparator);
    REQUIRE(q1bResults == a1bResults);

    std::string q1c = "stmt s; Select s such that Follows(2, 1)";
    auto q1cResultsSet = MainQueryProcessor::processQuery(q1c, storageManager->getReadStorage());

    std::list<std::string> q1cResults(q1cResultsSet.begin(), q1cResultsSet.end());

    std::list<std::string> a1cResults = {};
    REQUIRE(q1cResults == a1cResults);

    //std::string q1f = "stmt s; assign a; Select s such that Follows(s, a)";
    //auto q1fResultsSet = MainQueryProcessor::processQuery(q1f, storageManager->getReadStorage());

    //std::list<std::string> q1fResults(q1fResultsSet.begin(), q1fResultsSet.end());

    //std::list<std::string> a1fResults = { "1", "3", "8", "9" };
    //q1fResults.sort(stringIntComparator);
    //REQUIRE(q1fResults == a1fResults);

    std::string q1g = "stmt s; assign a; Select s such that follows(s, a)";
    auto q1gResultsSet = MainQueryProcessor::processQuery(q1g, storageManager->getReadStorage());

    std::list<std::string> q1gResults(q1gResultsSet.begin(), q1gResultsSet.end());

    std::list<std::string> a1gResults = { "SyntaxError" }; // Hmmm todo!!
    REQUIRE(q1gResults == a1gResults);

    // test FollowsT
    std::string q2a = "stmt s; Select s such that Follows*(1, s)";
    auto q2aResultsSet = MainQueryProcessor::processQuery(q2a, storageManager->getReadStorage());

    std::list<std::string> q2aResults(q2aResultsSet.begin(), q2aResultsSet.end());

    std::list<std::string> a2aResults = { "2", "3", "7", "8", "14", "15" };
    q2aResults.sort(stringIntComparator);
    REQUIRE(q2aResults == a2aResults);

    std::string q2b = "stmt s; Select s such that Follows*(s, 8)";
    auto q2bResultsSet = MainQueryProcessor::processQuery(q2b, storageManager->getReadStorage());

    std::list<std::string> q2bResults(q2bResultsSet.begin(), q2bResultsSet.end());

    std::list<std::string> a2bResults = { "1", "2", "3", "7" };
    q2bResults.sort(stringIntComparator);
    REQUIRE(q2bResults == a2bResults);

    std::string q2d = "stmt s; print p; Select s such that Follows*(p, 3)";
    auto q2dResultsSet = MainQueryProcessor::processQuery(q2d, storageManager->getReadStorage());

    std::list<std::string> q2dResults(q2dResultsSet.begin(), q2dResultsSet.end());

    std::list<std::string> a2dResults = {};
    REQUIRE(q2dResults == a2dResults);

    //std::string q2e = "assign a; while w; Select a such that Follows*(a, w)";
    //auto q2eResultsSet = MainQueryProcessor::processQuery(q2e, storageManager->getReadStorage());

    //std::list<std::string> q2eResults(q2eResultsSet.begin(), q2eResultsSet.end());

    //std::list<std::string> a2eResults = { "2", "7" };
    //q2eResults.sort(stringIntComparator);
    //REQUIRE(q2eResults == a2eResults);

    //std::string q2f = "if ifs; while w; Select ifs such that Follows*(ifs, w)";
    //auto q2fResultsSet = MainQueryProcessor::processQuery(q2f, storageManager->getReadStorage());

    //std::list<std::string> q2fResults(q2fResultsSet.begin(), q2fResultsSet.end());

    //std::list<std::string> a2fResults = { "3" };
    //REQUIRE(q2fResults == a2fResults);

    // test Modifies
    std::string q3a = "stmt s; Select s such that Modifies(s, \"x\")";
    auto q3aResultsSet = MainQueryProcessor::processQuery(q3a, storageManager->getReadStorage());

    std::list<std::string> q3aResults(q3aResultsSet.begin(), q3aResultsSet.end());

    std::list<std::string> a3aResults = { "2", "3", "4", "6", "8", "9", "11" };
    q3aResults.sort(stringIntComparator);
    REQUIRE(q3aResults == a3aResults);

    std::string q3b = "variable v; Select v such that Modifies(8, v)";
    auto q3bResultsSet = MainQueryProcessor::processQuery(q3b, storageManager->getReadStorage());

    std::list<std::string> q3bResults(q3bResultsSet.begin(), q3bResultsSet.end());

    std::list<std::string> a3bResults = { "x", "y" };
    q3bResults.sort(stringIntComparator);
    REQUIRE(q3bResults == a3bResults);

    std::string q3c = "assign a; variable v; Select a such that Modifies(a, v)";
    auto q3cResultsSet = MainQueryProcessor::processQuery(q3c, storageManager->getReadStorage());

    std::list<std::string> q3cResults(q3cResultsSet.begin(), q3cResultsSet.end());

    std::list<std::string> a3cResults = { "2", "4", "6", "7", "11", "13", "14" };
    q3cResults.sort(stringIntComparator);
    REQUIRE(q3cResults == a3cResults);

    // test Parent
    std::string q4a = "stmt s; Select s such that Parent(8, s)";
    auto q4aResultsSet = MainQueryProcessor::processQuery(q4a, storageManager->getReadStorage());

    std::list<std::string> q4aResults(q4aResultsSet.begin(), q4aResultsSet.end());

    std::list<std::string> a4aResults = { "9", "13" };
    q4aResults.sort(stringIntComparator);
    REQUIRE(q4aResults == a4aResults);

    std::string q4b = "stmt s; Select s such that Parent(s, 3)";
    auto q4bResultsSet = MainQueryProcessor::processQuery(q4b, storageManager->getReadStorage());

    std::list<std::string> q4bResults(q4bResultsSet.begin(), q4bResultsSet.end());

    std::list<std::string> a4bResults = {};
    REQUIRE(q4bResults == a4bResults);

    // test ParentT
    std::string q5a = "stmt s; Select s such that Parent*(8, s)";
    auto q5aResultsSet = MainQueryProcessor::processQuery(q5a, storageManager->getReadStorage());

    std::list<std::string> q5aResults(q5aResultsSet.begin(), q5aResultsSet.end());

    std::list<std::string> a5aResults = { "9", "10", "11", "12", "13" };
    q5aResults.sort(stringIntComparator);
    REQUIRE(q5aResults == a5aResults);

    std::string q5b = "stmt s; Select s such that Parent*(3, s)";
    auto q5bResultsSet = MainQueryProcessor::processQuery(q5b, storageManager->getReadStorage());

    std::list<std::string> q5bResults(q5bResultsSet.begin(), q5bResultsSet.end());

    std::list<std::string> a5bResults = { "4", "5", "6" };
    q5bResults.sort(stringIntComparator);
    REQUIRE(q5bResults == a5bResults);

    // test Uses
    std::string q6a = "stmt s; Select s such that Uses(s, \"a\")";
    auto q6aResultsSet = MainQueryProcessor::processQuery(q6a, storageManager->getReadStorage());

    std::list<std::string> q6aResults(q6aResultsSet.begin(), q6aResultsSet.end());

    std::list<std::string> a6aResults = { "3" };
    REQUIRE(q6aResults == a6aResults);

    std::string q6b = "assign a; Select a such that Uses(a, \"x\")";
    auto q6bResultsSet = MainQueryProcessor::processQuery(q6b, storageManager->getReadStorage());

    std::list<std::string> q6bResults(q6bResultsSet.begin(), q6bResultsSet.end());

    std::list<std::string> a6bResults = { "4", "14" };
    q6bResults.sort(stringIntComparator);
    REQUIRE(q6bResults == a6bResults);

    std::string q6c = "print pn; variable v; Select v such that Uses(pn, v)";
    auto q6cResultsSet = MainQueryProcessor::processQuery(q6c, storageManager->getReadStorage());

    std::list<std::string> q6cResults(q6cResultsSet.begin(), q6cResultsSet.end());

    std::list<std::string> a6cResults = { "x" };
    REQUIRE(q6cResults == a6cResults);

    std::string q6e = "stmt s; call cl; Select s such that Uses(s, cl)";
    auto q6eResultsSet = MainQueryProcessor::processQuery(q6e, storageManager->getReadStorage());

    std::list<std::string> q6eResults;

    std::list<std::string> a6eResults = { "SemanticError" }; //Hmmm todo!!
    REQUIRE(q6eResults == a6eResults);
}
// ai-gen end
 */