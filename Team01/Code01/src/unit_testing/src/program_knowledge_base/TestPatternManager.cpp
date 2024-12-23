//#include "../../include/catch.hpp"
//#include "program_knowledge_base/pattern/PatternManager.h"
//#include "common/MockPatternsFactory.h"
//#include <memory>
//
//// Ensure the EntityType enum and ExprNode/VariableNode classes are properly defined and included.
//
//TEST_CASE("PatternManager::insertPattern and PatternManager::isEmpty") {
//    PatternManager patternManager;
//    MockPatternsFactory factory;
//
//    // Check if the pattern manager is initially empty
//    REQUIRE(patternManager.isEmpty() == true);
//
//    // Insert the first expression into the pattern manager
//    auto expr1 = factory.createFirstExpression();
//    REQUIRE(patternManager.insertPattern({ "1", EntityType::ASSIGN }, std::make_pair("v", EntityType::VARIABLE), expr1) == true);
//
//    // Verify the pattern manager is no longer empty after insertion
//    REQUIRE(patternManager.isEmpty() == false);
//}
//
//TEST_CASE("PatternManager::stmtNoIsPresent") {
//    PatternManager patternManager;
//    MockPatternsFactory factory;
//
//    // Insert the first expression with a specific statement number and variable
//    auto expr1 = factory.createFirstExpression();
//    patternManager.insertPattern({ "1", EntityType::ASSIGN }, { "v", EntityType::VARIABLE }, expr1);
//
//    // Verify the specific statement number is present
//    REQUIRE(patternManager.stmtNoIsPresent({ "1", EntityType::ASSIGN }) == true);
//    // Verify a non-existent statement number is not found
//    REQUIRE(patternManager.stmtNoIsPresent({ "2", EntityType::ASSIGN }) == false);
//}
//
//TEST_CASE("PatternManager::exprNodeIsPresent") {
//    PatternManager patternManager;
//    MockPatternsFactory factory;
//
//    // Insert the first expression and keep another as not inserted
//    auto expr1 = factory.createFirstExpression();
//    auto expr2 = factory.createSecondExpression(); // Not inserted
//    patternManager.insertPattern({ "1", EntityType::ASSIGN }, { "v", EntityType::VARIABLE }, expr1);
//
//    // Verify the inserted expression node is present
//    REQUIRE(patternManager.exprNodeIsPresent(expr1) == true);
//    // Verify the non-inserted expression node is not present
//    REQUIRE(patternManager.exprNodeIsPresent(expr2) == false);
//}
//
//TEST_CASE("PatternManager::getAllPatternEntries") {
//    PatternManager patternManager;
//    MockPatternsFactory factory;
//
//    // Insert the first expression
//    auto expr1 = factory.createFirstExpression();
//    patternManager.insertPattern({ "1", EntityType::ASSIGN }, { "v", EntityType::VARIABLE }, expr1);
//
//    // Retrieve all pattern entries and verify their properties
//    auto allEntries = patternManager.getAllPatternEntries();
//    REQUIRE(allEntries.size() == 1);
//    REQUIRE(allEntries.begin()->first == std::pair<std::string, EntityType>{"1", EntityType::ASSIGN});
//    REQUIRE(allEntries.begin()->second.first == "v"); // Assuming 'v' is the LHS in the pair, based on your insertPattern signature
//    REQUIRE(allEntries.begin()->second.second == expr1); // Verify the RHS is the expected expression node
//}
//
//TEST_CASE("PatternManager::insertMultiplePatterns and Check Duplication") {
//    PatternManager patternManager;
//    MockPatternsFactory factory;
//
//    auto expr1 = factory.createFirstExpression();
//    auto expr2 = factory.createSecondExpression();
//
//    // Insert multiple expressions
//    REQUIRE(patternManager.insertPattern({ "1", EntityType::ASSIGN }, { "v", EntityType::VARIABLE }, expr1) == true);
//    REQUIRE(patternManager.insertPattern({ "2", EntityType::ASSIGN }, { "w", EntityType::VARIABLE }, expr2) == true);
//
//    // Attempt to insert a duplicate pattern
//    REQUIRE(patternManager.insertPattern({ "1", EntityType::ASSIGN }, { "v", EntityType::VARIABLE }, expr1) == false);
//
//    // Ensure only unique patterns are inserted
//    REQUIRE(patternManager.getAllPatternEntries().size() == 2);
//}
//
//TEST_CASE("PatternManager::insertMultiplePatterns and Check all") {
//    PatternManager patternManager;
//    MockPatternsFactory factory;
//
//    auto expr1 = factory.createFirstExpression();
//    auto expr2 = factory.createSecondExpression();
//    auto expr3 = factory.createThirdExpression();
//    auto expr4 = factory.createFourthExpression();
//    auto expr5 = factory.createFifthExpression();
//
//    // Insert multiple expressions
//    REQUIRE(patternManager.insertPattern({ "1", EntityType::ASSIGN }, { "v", EntityType::VARIABLE }, expr1) == true);
//    REQUIRE(patternManager.insertPattern({ "2", EntityType::ASSIGN }, { "w", EntityType::VARIABLE }, expr2) == true);
//    REQUIRE(patternManager.insertPattern({ "3", EntityType::ASSIGN }, { "x", EntityType::VARIABLE }, expr3) == true);
//    REQUIRE(patternManager.insertPattern({ "4", EntityType::ASSIGN }, { "y", EntityType::VARIABLE }, expr4) == true);
//    REQUIRE(patternManager.insertPattern({ "5", EntityType::ASSIGN }, { "z", EntityType::VARIABLE }, expr5) == true);
//
//    REQUIRE(patternManager.getAllPatternEntries().size() == 5);
//}
//
