//
// Created by Michael on 26/2/24.
//

#include "../../include/catch.hpp"

#include "common/Utils.h"
#include "sp/tokenizer/Token.h"
#include "sp/parser/expressions/ExprParser.h"
#include "test_utils/ASTTraversePrint.h"
#include "test_utils/ASTCompare.h"
#include <vector>
#include <memory>

TEST_CASE("Assignment Parser Test") {
    std::vector<Token> tokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::ASSIGNMENT);
    auto parser = ParserFactory::createParser<AssignmentParser>(ctx);
    auto node = std::dynamic_pointer_cast<AssignNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<VariableNode>("x",1);
    auto rightChild = std::make_shared<ConstantNode>(2,1);
    auto testNode = std::make_shared<AssignNode>(leftChild, rightChild, 1);

    if (compareAssignNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintAssignStatement(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintAssignStatement(node);
    std::cout << "\n------ VERBOSE LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED LINE NUMBER ------\n\n";
    printTestStatementLineMap();
    std::cout << "\n------ ACTUAL LINE NUMBER ------\n\n";
    printActualStatementLineMap();
    std::cout << "\n------ PROGRAM LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED PROGRAM LINE NUMBER ------\n\n";
    printTestStatementLines();
    std::cout << "\n------ ACTUAL PROGRAM LINE NUMBER ------\n\n";
    printActualStatementLines();
    std::cout << "\n";
    REQUIRE(compareAssignNodes(node,testNode));
}

TEST_CASE("Assignment Parser Random Test") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "read"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::KEYWORD, "procedure"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::KEYWORD, "read"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::KEYWORD, "print"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::ASSIGNMENT);
    auto parser = ParserFactory::createParser<AssignmentParser>(ctx);
    auto node = std::dynamic_pointer_cast<AssignNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<VariableNode>("read",1);
    auto rightChild = std::make_shared<ExprNode>(
            std::make_shared<ExprNode>(
                    std::make_shared<VariableNode>("procedure",1),
                    std::make_shared<ExprNode>(
                            std::make_shared<ExprNode>(
                                    std::make_shared<VariableNode>("read", 1),
                                    std::make_shared<VariableNode>("print",1),
                                    ExprOperator::Times, 1),
                            std::make_shared<VariableNode>("x",1),
                            ExprOperator::Divided, 1),
                    ExprOperator::Times, 1),
            std::make_shared<ConstantNode>(5,1),
            ExprOperator::Times, 1);
    auto testNode = std::make_shared<AssignNode>(leftChild, rightChild, 1);

    if (compareAssignNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintAssignStatement(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintAssignStatement(node);
    std::cout << "\n------ VERBOSE LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED LINE NUMBER ------\n\n";
    printTestStatementLineMap();
    std::cout << "\n------ ACTUAL LINE NUMBER ------\n\n";
    printActualStatementLineMap();
    std::cout << "\n------ PROGRAM LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED PROGRAM LINE NUMBER ------\n\n";
    printTestStatementLines();
    std::cout << "\n------ ACTUAL PROGRAM LINE NUMBER ------\n\n";
    printActualStatementLines();
    std::cout << "\n";
    REQUIRE(compareAssignNodes(node,testNode));
}

TEST_CASE("While Parser Test") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::KEYWORD, "while"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"}
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::WHILE);
    auto parser = ParserFactory::createParser<WhileParser>(ctx);
    auto node = std::dynamic_pointer_cast<WhileNode>(parser->parse(ctx));

    auto condition = std::make_shared<RelExprNode>(
        std::make_shared<VariableNode>("while",1),
        std::make_shared<VariableNode>("while",1),
        RelExprOperator::Greater, 1
    );
    auto body = std::make_shared<StmtListNode>(0);
    body->addStatement(std::make_shared<AssignNode>(
            std::make_shared<VariableNode>("x",2),
            std::make_shared<ConstantNode>(2,2),
            2
    ));
    body->addStatement(std::make_shared<AssignNode>(
            std::make_shared<VariableNode>("y",3),
            std::make_shared<ConstantNode>(3,3),
            3
    ));
    body->addStatement(std::make_shared<AssignNode>(
            std::make_shared<VariableNode>("x",4),
            std::make_shared<VariableNode>("y",4),
            4
    ));
    auto testNode = std::make_shared<WhileNode>(condition, body, 1);

    if (compareWhileNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintWhileStatement(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintWhileStatement(node);
    std::cout << "\n------ VERBOSE LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED LINE NUMBER ------\n\n";
    printTestStatementLineMap();
    std::cout << "\n------ ACTUAL LINE NUMBER ------\n\n";
    printActualStatementLineMap();
    std::cout << "\n------ PROGRAM LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED PROGRAM LINE NUMBER ------\n\n";
    printTestStatementLines();
    std::cout << "\n------ ACTUAL PROGRAM LINE NUMBER ------\n\n";
    printActualStatementLines();
    std::cout << "\n";
    REQUIRE(compareWhileNodes(node,testNode));
}

TEST_CASE("While Milestone 1 Integration Fix") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "1"},
            {TokenType::RELATIONAL_OPERATOR, ">="},
            {TokenType::CONSTANT, "1"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "0"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::PUNCTUATION, "}"},
    };


    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::WHILE);
    auto parser = ParserFactory::createParser<WhileParser>(ctx);
    auto node = std::dynamic_pointer_cast<WhileNode>(parser->parse(ctx));

    auto testLeftChild = std::make_shared<ConstantNode>(1,1);
    auto testRightChild = std::make_shared<ExprNode>(
            std::make_shared<ConstantNode>(1,1),
            std::make_shared<ExprNode>(
                    std::make_shared<ConstantNode>(0,1),
                    std::make_shared<ConstantNode>(1,1),
                    ExprOperator::Minus,1),
            ExprOperator::Modulo,1);
    auto relNode = std::make_shared<RelExprNode>(testLeftChild,testRightChild,RelExprOperator::GreaterEqual,1);
    auto testNode = std::make_shared<WhileNode>(relNode,std::make_shared<StmtListNode>(2),1);

    if (compareWhileNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintWhileStatement(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintWhileStatement(node);
    std::cout << "\n------ VERBOSE LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED LINE NUMBER ------\n\n";
    printTestStatementLineMap();
    std::cout << "\n------ ACTUAL LINE NUMBER ------\n\n";
    printActualStatementLineMap();
    std::cout << "\n------ PROGRAM LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED PROGRAM LINE NUMBER ------\n\n";
    printTestStatementLines();
    std::cout << "\n------ ACTUAL PROGRAM LINE NUMBER ------\n\n";
    printActualStatementLines();
    std::cout << "\n";
    REQUIRE(compareWhileNodes(node,testNode));
}

TEST_CASE("While Milestone 1 Fix2") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD,             "while"},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::CONSTANT,            "1"},
            {TokenType::RELATIONAL_OPERATOR, ">="},
            {TokenType::CONSTANT,            "1"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::CONSTANT,            "0"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::PUNCTUATION,         "}"},
    };


    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::WHILE);
    auto parser = ParserFactory::createParser<WhileParser>(ctx);
    auto node = std::dynamic_pointer_cast<WhileNode>(parser->parse(ctx));

    auto testLeftChild = std::make_shared<ConstantNode>(1, 1);
    auto testRightChild = std::make_shared<ExprNode>(
            std::make_shared<ConstantNode>(1, 1),
            std::make_shared<ConstantNode>(0, 1),
            ExprOperator::Modulo, 1);
    auto relNode = std::make_shared<RelExprNode>(testLeftChild, testRightChild, RelExprOperator::GreaterEqual, 1);
    auto testNode = std::make_shared<WhileNode>(relNode, std::make_shared<StmtListNode>(2), 1);

    if (compareWhileNodes(node, testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintWhileStatement(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintWhileStatement(node);
    std::cout << "\n------ VERBOSE LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED LINE NUMBER ------\n\n";
    printTestStatementLineMap();
    std::cout << "\n------ ACTUAL LINE NUMBER ------\n\n";
    printActualStatementLineMap();
    std::cout << "\n------ PROGRAM LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED PROGRAM LINE NUMBER ------\n\n";
    printTestStatementLines();
    std::cout << "\n------ ACTUAL PROGRAM LINE NUMBER ------\n\n";
    printActualStatementLines();
    std::cout << "\n";
    REQUIRE(compareWhileNodes(node, testNode));
}

TEST_CASE("While Milestone 1 Fix3") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONDITIONAL_OPERATOR, "!"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "1"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "1"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::PUNCTUATION, "}"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::WHILE);
    auto parser = ParserFactory::createParser<WhileParser>(ctx);
    auto node = std::dynamic_pointer_cast<WhileNode>(parser->parse(ctx));

    auto leftVariableNode = std::make_shared<ConstantNode>(1, 1);
    auto leftConstantNode = std::make_shared<ConstantNode>(0, 1);
    auto leftRelExprNode = std::make_shared<RelExprNode>(leftVariableNode, leftConstantNode, RelExprOperator::Equal, 1);
    auto rightVariableNode = std::make_shared<ConstantNode>(1, 1);
    auto rightConstantNode = std::make_shared<ConstantNode>(0, 1);
    auto rightRelExprNode = std::make_shared<RelExprNode>(rightVariableNode, rightConstantNode, RelExprOperator::Equal, 1);
    auto operand = std::make_shared<CondBinaryExprNode>(leftRelExprNode, rightRelExprNode, CondExprOperator::And, 1);
    auto condNode = std::make_shared<CondUnaryExprNode>(operand, CondExprOperator::Not, 1);
    auto testNode = std::make_shared<WhileNode>(condNode, std::make_shared<StmtListNode>(2), 1);

    if (compareWhileNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintWhileStatement(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintWhileStatement(node);
    std::cout << "\n------ VERBOSE LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED LINE NUMBER ------\n\n";
    printTestStatementLineMap();
    std::cout << "\n------ ACTUAL LINE NUMBER ------\n\n";
    printActualStatementLineMap();
    std::cout << "\n------ PROGRAM LINE NUMBER CHECK ------";
    std::cout << "\n------ EXPECTED PROGRAM LINE NUMBER ------\n\n";
    printTestStatementLines();
    std::cout << "\n------ ACTUAL PROGRAM LINE NUMBER ------\n\n";
    printActualStatementLines();
    std::cout << "\n";
    REQUIRE(compareWhileNodes(node,testNode));
}
