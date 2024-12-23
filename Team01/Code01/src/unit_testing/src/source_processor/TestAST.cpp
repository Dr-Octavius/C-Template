//#pragma once
#include <iostream>
#include <memory>
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/statements/IfNode.h"
#include "sp/ast/nodes/statements/AssignNode.h"
#include "sp/ast/nodes/statements/ReadNode.h"
#include "sp/ast/nodes/statements/PrintNode.h"
#include "sp/ast/nodes/expressions/conditional/CondBinaryExprNode.h"
#include "sp/ast/nodes/expressions/conditional/CondUnaryExprNode.h"
#include "sp/ast/nodes/expressions/relational/RelExprNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include "common/Utils.h"
#include "common/MockPatternsFactory.h"
#include "sp/ast/nodes/statements/WhileNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "../../../include/catch.hpp"
#include "sp/design_extractor/DesignExtractor.h"
#include "sp/design_extractor/visitors/FollowsVisitor.h"
#include "sp/design_extractor/visitors/ParentVisitor.h"
#include "sp/design_extractor/visitors/ModifiesVisitor.h"
#include "sp/design_extractor/visitors/UsesVisitor.h"
#include "sp/design_extractor/visitors/CallsVisitor.h"
#include "sp/design_extractor/visitors/EntityVisitor.h"
#include "sp/design_extractor/visitors/PatternVisitor.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include <map>
#include <utility>
#include <unordered_set>

using namespace std;
inline void require(bool b) {
    REQUIRE(b);
};

TEST_CASE("1st Test") {
    require(1 == 1);
};

// For testing manual construction of AST using various nodes
TEST_CASE("2nd Test") {
    // Condition - x > 0
    auto varX = std::make_shared<VariableNode>("x", 1);
    auto constZero = std::make_shared<ConstantNode>(0, 1);
    auto condition = std::make_shared<RelExprNode>(std::move(varX), std::move(constZero), RelExprOperator::Greater, 1);

    // Create nodes for the then branch "y = y + 1"
    auto varY = std::make_shared<VariableNode>("y", 2);
    auto constOne = std::make_shared<ConstantNode>(1, 2);
    auto plusExpr = std::make_shared<ExprNode>(std::make_shared<VariableNode>("y", 2), std::move(constOne), ExprOperator::Plus, 2);
    auto thenBranch = std::make_shared<StmtListNode>(2);
    thenBranch->addStatement(std::make_shared<AssignNode>(std::move(varY), std::move(plusExpr), 2));

    // Create nodes for the else branch "z = z - 1"
    auto varZ = std::make_shared<VariableNode>("z", 3);
    auto minusExpr = std::make_shared<ExprNode>(std::make_shared<VariableNode>("z", 3), std::make_shared<ConstantNode>(1, 3), ExprOperator::Minus, 3);
    auto elseBranch = std::make_shared<StmtListNode>(3);
    elseBranch->addStatement(std::make_shared<AssignNode>(std::move(varZ), std::move(minusExpr), 3));

    // Construct the if node with the condition, then branch, and else branch
    auto ifNode = std::make_shared<IfNode>(std::move(condition), std::move(thenBranch), std::move(elseBranch), 1);

    std::cout << "AST constructed successfully." << std::endl;

    require(ifNode != nullptr);
};

// For testing extraction of data by traverseFollows, traverseParent
TEST_CASE("3rd Test") {
    // Construct the while loop condition
    auto varI = std::make_shared<VariableNode>("i", 1);
    auto constZero = std::make_shared<ConstantNode>(0, 1);
    auto conditionWhile = std::make_shared<RelExprNode>(std::move(varI), std::move(constZero), RelExprOperator::Greater, 1);

    // Construct the first assignment inside the while loop
    auto varZ = std::make_shared<VariableNode>("z", 2);
    auto constTwo = std::make_shared<ConstantNode>(2, 2);
    auto assignZ = std::make_shared<AssignNode>(std::move(varZ), std::move(constTwo), 2);

    // Construct the first assignment inside the while loop
    auto varR = std::make_shared<VariableNode>("r", 3);
    auto constTen = std::make_shared<ConstantNode>(10, 3);
    auto assignR = std::make_shared<AssignNode>(std::move(varR), std::move(constTen), 3);

    // Construct the if statement condition
    auto varX = std::make_shared<VariableNode>("x", 4);
    auto conditionIf = std::make_shared<RelExprNode>(std::move(varX), std::make_shared<ConstantNode>(0, 4), RelExprOperator::Greater, 4);

    // Construct the then statement for the if statement
    auto varA = std::make_shared<VariableNode>("a", 5);
    auto thenConstTwo = std::make_shared<ConstantNode>(2, 5);
    auto thenAssign = std::make_shared<AssignNode>(std::move(varA), std::move(thenConstTwo), 5);

    // Construct the else statement for the if statement
    auto varB = std::make_shared<VariableNode>("b", 6);
    auto elseConstFive = std::make_shared<ConstantNode>(5, 6);
    auto elseAssign = std::make_shared<AssignNode>(std::move(varB), std::move(elseConstFive), 6);

    // Create then and else statement lists for the if statement
    auto thenStmtList = std::make_shared<StmtListNode>(0);
    thenStmtList->addStatement(std::move(thenAssign));

    auto elseStmtList = std::make_shared<StmtListNode>(0);
    elseStmtList->addStatement(std::move(elseAssign));

    // Construct the if statement with its condition, then, and else branches
    auto ifStmt = std::make_shared<IfNode>(std::move(conditionIf), std::move(thenStmtList), std::move(elseStmtList), 4);

    // Create the statement list that will be the body of the while loop
    auto whileBody = std::make_shared<StmtListNode>(0);
    whileBody->addStatement(std::move(assignZ));
    whileBody->addStatement(std::move(assignR));
    whileBody->addStatement(std::move(ifStmt));

    // Construct the while node with its condition and body
    auto whileNode = std::make_shared<WhileNode>(std::move(conditionWhile), std::move(whileBody), 1);

    // Create the root statement list and add the while node to it
    // Statement number inside statement list nodes don't have meaning, will be removing in next iteration
    auto root = std::make_shared<StmtListNode>(0);
    root->addStatement(std::move(whileNode));

    // std::vector<std::shared_ptr<ASTNode>> allStatements = root->getStatements();

    // Main procedure
    auto procRoot = std::make_shared<ProcedureNode>("main", root, 0);

    auto followsVisitor = std::make_shared<FollowsVisitor>();
    auto parentVisitor = std::make_shared<ParentVisitor>();
    auto modifiesVisitor = std::make_shared<ModifiesVisitor>();
    auto usesVisitor = std::make_shared<UsesVisitor>();
    auto entitiesVisitor = std::make_shared<EntityVisitor>();
    auto storageManager = std::make_shared<StorageManager>();
    auto patternVisitor = std::make_shared<PatternVisitor>();
    auto callsVisitor = std::make_shared<CallsVisitor>();

    DesignExtractor designExtractor(followsVisitor, parentVisitor, usesVisitor, modifiesVisitor, entitiesVisitor, callsVisitor, patternVisitor, storageManager);
    designExtractor.traverseAll(procRoot);

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> followsRecords = followsVisitor->getFollows();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> parentRecords = parentVisitor->getParents();

    for (const auto& pair : parentRecords) {
        std::cout << "Child: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") has parent: ";
        for (const auto& child : pair.second) {
            std::cout << child.first << " (" << entityTypeToString(child.second) << "), ";
        }
        std::cout << std::endl;
    }

    for (const auto& pair : followsRecords) {
        std::cout << "Follows: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") is followed by: ";
        for (const auto& follower : pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    require(root != nullptr);
}

// For testing extraction of data by traverseFollows, traverseParent
TEST_CASE("4th Test") {
    // Construct the first assignment
    auto varZ = std::make_shared<VariableNode>("z", 1);
    auto constTwo = std::make_shared<ConstantNode>(2, 1);
    auto assignZ = std::make_shared<AssignNode>(std::move(varZ), std::move(constTwo), 1);

    // Construct the second assignment
    auto varR = std::make_shared<VariableNode>("r", 2);
    auto constTen = std::make_shared<ConstantNode>(10, 2);
    auto assignR = std::make_shared<AssignNode>(std::move(varR), std::move(constTen), 2);

    // Construct the if statement condition
    auto varX = std::make_shared<VariableNode>("x", 3);
    auto conditionIf = std::make_shared<RelExprNode>(std::move(varX), std::make_shared<ConstantNode>(0, 3), RelExprOperator::Greater, 3);

    // Construct the then statement for the if statement
    auto varA = std::make_shared<VariableNode>("a", 4);
    auto thenConstTwo = std::make_shared<ConstantNode>(2, 4);
    auto thenAssign = std::make_shared<AssignNode>(std::move(varA), std::move(thenConstTwo), 4);

    // Construct the else statement for the if statement
    auto varB = std::make_shared<VariableNode>("b", 5);
    auto elseConstFive = std::make_shared<ConstantNode>(5, 5);
    auto elseAssign = std::make_shared<AssignNode>(std::move(varB), std::move(elseConstFive), 5);

    // Construct a while statement inside else statement for the if statement
    auto varJ = std::make_shared<VariableNode>("j", 6);
    auto constZero = std::make_shared<ConstantNode>(0, 6);
    auto conditionWhile = std::make_shared<RelExprNode>(std::move(varJ), std::move(constZero), RelExprOperator::Greater, 6);

    auto printVar1 = std::make_shared<VariableNode>("r", 7);
    auto prntStmt1 = std::make_shared<PrintNode>(std::move(printVar1), 7);

    auto whileBody = std::make_shared<StmtListNode>(0);
    whileBody->addStatement(std::move(prntStmt1));

    auto elseWhileNode = std::make_shared<WhileNode>(std::move(conditionWhile), std::move(whileBody), 6);

    // Create then and else statement lists for the if statement
    auto thenStmtList = std::make_shared<StmtListNode>(0);
    thenStmtList->addStatement(std::move(thenAssign));

    auto elseStmtList = std::make_shared<StmtListNode>(0);
    elseStmtList->addStatement(std::move(elseAssign));
    elseStmtList->addStatement(std::move(elseWhileNode));

    // Construct the if statement with its condition, then, and else branches
    auto ifStmt = std::make_shared<IfNode>(std::move(conditionIf), std::move(thenStmtList), std::move(elseStmtList), 3);

    // Read statement
    auto readVar = std::make_shared<VariableNode>("input", 8);
    auto readStatement = std::make_shared<ReadNode>(std::move(readVar), 8);

    // Print statement
    auto printVar2 = std::make_shared<VariableNode>("z", 9);
    auto prntStmt2 = std::make_shared<PrintNode>(std::move(printVar2), 9);

    auto root = std::make_shared<StmtListNode>(0);
    root->addStatement(std::move(assignZ));
    root->addStatement(std::move(assignR));
    root->addStatement(std::move(ifStmt));
    root->addStatement(std::move(readStatement));
    root->addStatement(std::move(prntStmt2));

    // Main procedure
    auto procRoot = std::make_shared<ProcedureNode>("main", root, 0);

    auto followsVisitor = std::make_shared<FollowsVisitor>();
    auto parentVisitor = std::make_shared<ParentVisitor>();
    auto modifiesVisitor = std::make_shared<ModifiesVisitor>();
    auto usesVisitor = std::make_shared<UsesVisitor>();
    auto entitiesVisitor = std::make_shared<EntityVisitor>();
    auto storageManager = std::make_shared<StorageManager>();
    auto callsVisitor = std::make_shared<CallsVisitor>();
    auto patternVisitor = std::make_shared<PatternVisitor>();

    DesignExtractor designExtractor(followsVisitor, parentVisitor, usesVisitor, modifiesVisitor, entitiesVisitor, callsVisitor, patternVisitor, storageManager);
    designExtractor.traverseAll(procRoot);

    std::map<std::string, EntityType> stmtNumberMap = entitiesVisitor->getStmtNumberMap();
    std::map<EntityType, std::unordered_set<std::string>> entityExtractor = entitiesVisitor->getEntityToStatementsMap();

    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> followsRecords = followsVisitor->getFollows();
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> parentRecords = parentVisitor->getParents();

    for (const auto& pair : parentRecords) {
        std::cout << "Child: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") has parent: ";
        for (const auto& child : pair.second) {
            std::cout << child.first << " (" << entityTypeToString(child.second) << "), ";
        }
        std::cout << std::endl;
    }

    for (const auto& pair : followsRecords) {
        std::cout << "Follows: " << pair.first.first << " (" << entityTypeToString(pair.first.second) << ") is followed by: ";
        for (const auto& follower : pair.second) {
            std::cout << follower.first << " (" << entityTypeToString(follower.second) << "), ";
        }
        std::cout << std::endl;
    }

    for (const auto& pair : stmtNumberMap) {
        std::cout << "Statement number " << pair.first << " is of type " << entityTypeToString(pair.second) << std::endl;
    }

    for (const auto& pair : entityExtractor) {
        std::cout << "Entity type " << entityTypeToString(pair.first) << " includes: ";
        for (const auto& item : pair.second) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    require(root != nullptr);
}

TEST_CASE("5th Test") {
    // Sample usage for MockPatternsFactory
    MockPatternsFactory factory;
    factory.populatePatternsMap();
    auto patternsData = factory.getPatternsData();
}
