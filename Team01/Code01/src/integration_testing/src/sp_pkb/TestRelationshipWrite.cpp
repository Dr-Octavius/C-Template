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
TEST_CASE("Test insert relationship for all managers") {
    std::string testInput = "procedure test1 {\n"
        "    read a;\n"
        "    x = 1;\n"
        "    if (a > 0) then {\n"
        "        x = x + 2;\n"
        "        print x;\n"
        "    } else {\n"
        "        x = 0;\n"
        "    }\n"
        "    y = 2;\n"
        "    while (y > 2) {\n"
        "        if (x > 0) then {\n"
        "            print x;\n"
        "        } else {\n"
        "            x = 1;\n"
        "            print x;\n"
        "        }\n"
        "        y = y - 1;\n"
        "    }\n"
        "    y = x;\n"
        "    call test2;\n"
        "}\n"
        "\n"
        "procedure test2 {\n"
        "    read b;\n"
        "    call test3;\n"
        "}\n"
        "\n"
        "procedure test3 {\n"
        "    read c;\n"
        "}";

    std::string testFileName = "./testFile.txt";

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

    // test FollowsManager
    auto followsManager = readStorage->getFollowsManager();
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> followsMap = { {{"1", EntityType::STMT}, {{"2", EntityType::STMT}}},
    																																			 {{"2", EntityType::STMT}, {{"3", EntityType::STMT}}},
    																																			 {{"4", EntityType::STMT}, {{"5", EntityType::STMT}}},
    																																			 {{"3", EntityType::STMT}, {{"7", EntityType::STMT}}},
    																																			 {{"7", EntityType::STMT}, {{"8", EntityType::STMT}}},
    																																			 {{"8", EntityType::STMT}, {{"14", EntityType::STMT}}},
    																																			 {{"9", EntityType::STMT}, {{"13", EntityType::STMT}}},
    																																			 {{"11", EntityType::STMT}, {{"12", EntityType::STMT}}},
    																																			 {{"14", EntityType::STMT}, {{"15", EntityType::STMT}}},
    																																			 {{"16", EntityType::STMT}, {{"17", EntityType::STMT}}} };

    auto storedFollowsMap = followsManager->getAllRelationshipEntries();

    REQUIRE(followsManager->hasMap({"1", EntityType::STMT}, {"2", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"2", EntityType::STMT}, {"3", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"4", EntityType::STMT}, {"5", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"3", EntityType::STMT}, {"7", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"7", EntityType::STMT}, {"8", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"8", EntityType::STMT}, {"14", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"9", EntityType::STMT}, {"13", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"11", EntityType::STMT}, {"12", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"14", EntityType::STMT}, {"15", EntityType::STMT}));
    REQUIRE(followsManager->hasMap({"16", EntityType::STMT}, {"17", EntityType::STMT}));
    REQUIRE(storedFollowsMap.size() == followsMap.size());
    REQUIRE(storedFollowsMap == followsMap);

    // test FollowsTManager
    auto followsTManager = readStorage->getFollowsTManager();
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> followsTMap = { {{"1", EntityType::STMT}, {{"2", EntityType::STMT}}},
    																																				 {{"2", EntityType::STMT}, {{"3", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"5", EntityType::STMT}}},
    																																				 {{"3", EntityType::STMT}, {{"7", EntityType::STMT}}},
    																																				 {{"7", EntityType::STMT}, {{"8", EntityType::STMT}}},
    																																				 {{"8", EntityType::STMT}, {{"14", EntityType::STMT}}},
    																																				 {{"9", EntityType::STMT}, {{"13", EntityType::STMT}}},
    																																				 {{"11", EntityType::STMT}, {{"12", EntityType::STMT}}},
    																																				 {{"14", EntityType::STMT}, {{"15", EntityType::STMT}}},
    																																				 {{"16", EntityType::STMT}, {{"17", EntityType::STMT}}} };

    auto storedFollowsTMap = followsTManager->getAllRelationshipEntries();

    REQUIRE(followsTManager->hasMap({"1", EntityType::STMT}, {"2", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"1", EntityType::STMT}, {"3", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"1", EntityType::STMT}, {"7", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"1", EntityType::STMT}, {"8", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"1", EntityType::STMT}, {"14", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"1", EntityType::STMT}, {"15", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"2", EntityType::STMT}, {"3", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"2", EntityType::STMT}, {"7", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"2", EntityType::STMT}, {"8", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"2", EntityType::STMT}, {"14", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"2", EntityType::STMT}, {"15", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"3", EntityType::STMT}, {"7", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"3", EntityType::STMT}, {"8", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"3", EntityType::STMT}, {"14", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"3", EntityType::STMT}, {"15", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"4", EntityType::STMT}, {"5", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"7", EntityType::STMT}, {"8", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"7", EntityType::STMT}, {"14", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"7", EntityType::STMT}, {"15", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"8", EntityType::STMT}, {"14", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"8", EntityType::STMT}, {"15", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"9", EntityType::STMT}, {"13", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"11", EntityType::STMT}, {"12", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"14", EntityType::STMT}, {"15", EntityType::STMT}));
    REQUIRE(followsTManager->hasMap({"16", EntityType::STMT}, {"17", EntityType::STMT}));
    REQUIRE(storedFollowsTMap.size() == followsTMap.size());
    REQUIRE(storedFollowsTMap == followsTMap);

    //  test ModifiesPManager
    auto modifiesManager = readStorage->getModifiesManager();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap = { {{"test1", EntityType::PROCEDURE}, {{"a", EntityType::VARIABLE}, {"x", EntityType::VARIABLE}, {"y", EntityType::VARIABLE}}},
    																																					 {{"test2", EntityType::PROCEDURE}, {{"b", EntityType::VARIABLE}, {"c", EntityType::VARIABLE}}},
    																																					 {{"test3", EntityType::PROCEDURE}, {{"c", EntityType::VARIABLE}}} };
    auto storedModifiesMap = modifiesManager->getAllRelationshipEntries();

    REQUIRE(modifiesManager->hasMap({ "test1", EntityType::PROCEDURE }, { "a", EntityType::PROCEDURE }));
    REQUIRE(modifiesManager->hasMap({ "test1", EntityType::PROCEDURE }, { "b", EntityType::PROCEDURE }));
    REQUIRE(modifiesManager->hasMap({ "test1", EntityType::PROCEDURE }, { "c", EntityType::PROCEDURE }));
    REQUIRE(modifiesManager->hasMap({ "test1", EntityType::PROCEDURE }, { "x", EntityType::PROCEDURE }));
    REQUIRE(modifiesManager->hasMap({ "test1", EntityType::PROCEDURE }, { "y", EntityType::PROCEDURE }));
    REQUIRE(modifiesManager->hasMap({ "test2", EntityType::PROCEDURE }, { "b", EntityType::PROCEDURE }));
    REQUIRE(modifiesManager->hasMap({ "test2", EntityType::PROCEDURE }, { "c", EntityType::PROCEDURE }));
    REQUIRE(modifiesManager->hasMap({ "test3", EntityType::PROCEDURE }, { "c", EntityType::PROCEDURE }));
    REQUIRE(storedModifiesMap.size() == modifiesMap.size());
    REQUIRE(storedModifiesMap == modifiesMap);

    // test ParentManager
    auto parentManager = readStorage->getParentManager();
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> parentMap = { {{"1", EntityType::STMT}, {{"2", EntityType::STMT}}},
    																																				 {{"2", EntityType::STMT}, {{"3", EntityType::STMT}}},
    																																				 {{"3", EntityType::STMT}, {{"4", EntityType::STMT}}},
    																																				 {{"3", EntityType::STMT}, {{"16", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"5", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"6", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"7", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"8", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"9", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"10", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"11", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"12", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"13", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"14", EntityType::STMT}}},
    																																				 {{"4", EntityType::STMT}, {{"15", EntityType::STMT}}},
    																																				 {{"7", EntityType::STMT}, {{"8", EntityType::STMT}}},
    																																				 {{"8", EntityType::STMT}, {{"14", EntityType::STMT}}},
    																																				 {{"9", EntityType::STMT}, {{"13", EntityType::STMT}}},
    																																				 {{"11", EntityType::STMT}, {{"12", EntityType::STMT}}},
    																																				 {{"14", EntityType::STMT}, {{"15", EntityType::STMT}}},
    																																				 {{"16", EntityType::STMT}, {{"17", EntityType::STMT}}} };

    auto storedParentMap = parentManager->getAllRelationshipEntries();

    REQUIRE(parentManager->hasMap({"3", EntityType::STMT}, {"4", EntityType::STMT}));
    REQUIRE(parentManager->hasMap({"3", EntityType::STMT}, {"5", EntityType::STMT}));
    REQUIRE(parentManager->hasMap({"3", EntityType::STMT}, {"6", EntityType::STMT}));
    REQUIRE(parentManager->hasMap({"8", EntityType::STMT}, {"9", EntityType::STMT}));
    REQUIRE(parentManager->hasMap({"8", EntityType::STMT}, {"13", EntityType::STMT}));
    REQUIRE(parentManager->hasMap({"9", EntityType::STMT}, {"10", EntityType::STMT}));
    REQUIRE(parentManager->hasMap({"9", EntityType::STMT}, {"11", EntityType::STMT}));
    REQUIRE(parentManager->hasMap({"9", EntityType::STMT}, {"12", EntityType::STMT}));
    REQUIRE(storedParentMap.size() == parentMap.size());
    REQUIRE(storedParentMap == parentMap);

    // test ParentTManager
    auto parentTManager = readStorage->getParentTManager();
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> parentTMap = { {{"1", EntityType::STMT}, {{"2", EntityType::STMT}}},
    																																					 {{"2", EntityType::STMT}, {{"3", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"4", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"5", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"6", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"7", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"8", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"9", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"10", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"11", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"12", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"13", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"14", EntityType::STMT}}},
    																																					 {{"3", EntityType::STMT}, {{"15", EntityType::STMT}}},
    																																					 {{"7", EntityType::STMT}, {{"8", EntityType::STMT}}},
    																																					 {{"8", EntityType::STMT}, {{"14", EntityType::STMT}}},
    																																					 {{"9", EntityType::STMT}, {{"13", EntityType::STMT}}},
    																																					 {{"11", EntityType::STMT}, {{"12", EntityType::STMT}}},
    																																					 {{"14", EntityType::STMT}, {{"15", EntityType::STMT}}},
    																																					 {{"16", EntityType::STMT}, {{"17", EntityType::STMT}}} };

    auto storedParentTMap = parentTManager->getAllRelationshipEntries();

    REQUIRE(parentTManager->hasMap({"3", EntityType::STMT}, {"4", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"3", EntityType::STMT}, {"5", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"3", EntityType::STMT}, {"6", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"8", EntityType::STMT}, {"9", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"8", EntityType::STMT}, {"10", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"8", EntityType::STMT}, {"11", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"8", EntityType::STMT}, {"12", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"8", EntityType::STMT}, {"13", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"9", EntityType::STMT}, {"10", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"9", EntityType::STMT}, {"11", EntityType::STMT}));
    REQUIRE(parentTManager->hasMap({"9", EntityType::STMT}, {"12", EntityType::STMT}));
    REQUIRE(storedParentTMap.size() == parentTMap.size());
    REQUIRE(storedParentTMap == parentTMap);

    // test UsesManager
    auto usesManager = readStorage->getUsesManager();
    auto storedUsesMap = usesManager->getAllRelationshipEntries();

    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> usesMap = { {{"test1", EntityType::PROCEDURE}, {{"a", EntityType::VARIABLE}, {"x", EntityType::VARIABLE}, {"y", EntityType::VARIABLE}}},
    																																				 {{"test2", EntityType::PROCEDURE}, {{"b", EntityType::VARIABLE}, {"c", EntityType::VARIABLE}}},
    																																				 {{"test3", EntityType::PROCEDURE}, {{"c", EntityType::VARIABLE}}},
                                                                                                                                                     {{"3", EntityType::STMT}, {{"a", EntityType::VARIABLE}, {"x", EntityType::VARIABLE}}},
                                                                                                                                                     {{"4", EntityType::STMT}, {{"x", EntityType::VARIABLE}}},
                                                                                                                                                     {{"5", EntityType::STMT}, {{"x", EntityType::VARIABLE}}},
                                                                                                                                                     {{"8", EntityType::STMT}, {{"x", EntityType::VARIABLE}, {"y", EntityType::VARIABLE}}},
                                                                                                                                                     {{"9", EntityType::STMT}, {{"x", EntityType::VARIABLE}}},
                                                                                                                                                     {{"10", EntityType::STMT}, {{"x", EntityType::VARIABLE}}},
                                                                                                                                                     {{"12", EntityType::STMT}, {{"x", EntityType::VARIABLE}}},
                                                                                                                                                     {{"13", EntityType::STMT}, {{"y", EntityType::VARIABLE}}},
                                                                                                                                                     {{"14", EntityType::STMT}, {{"x", EntityType::VARIABLE}}} };


    REQUIRE(usesManager->hasMap({"test1", EntityType::PROCEDURE}, {"a", EntityType::VARIABLE}));
    REQUIRE(usesManager->hasMap({"test1", EntityType::PROCEDURE}, {"x", EntityType::VARIABLE}));
    REQUIRE(usesManager->hasMap({"test1", EntityType::PROCEDURE}, {"y", EntityType::VARIABLE}));
    
    REQUIRE(usesManager->hasMap({ "3", EntityType::STMT }, { "a", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "3", EntityType::STMT }, { "a", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "3", EntityType::STMT }, { "x", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "4", EntityType::STMT }, { "x", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "5", EntityType::STMT }, { "x", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "8", EntityType::STMT }, { "x", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "8", EntityType::STMT }, { "y", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "9", EntityType::STMT }, { "x", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "10", EntityType::STMT }, { "x", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "12", EntityType::STMT }, { "x", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "13", EntityType::STMT }, { "y", EntityType::VARIABLE }));
    REQUIRE(usesManager->hasMap({ "14", EntityType::STMT }, { "x", EntityType::VARIABLE }));


    REQUIRE(storedUsesMap.size() == usesMap.size());
    REQUIRE(storedUsesMap == usesMap);

    //ai-gen end

    std::filesystem::remove(testFileName);
}
