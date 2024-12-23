//
// Created by Michael on 25/2/24.
//

#include "../../include/catch.hpp"

#include "common/Utils.h"
#include "sp/tokenizer/Token.h"
#include "sp/parser/expressions/ExprParser.h"
#include "sp/parser/expressions/RelExprParser.h"
#include "sp/parser/expressions/CondExprParser.h"
#include "test_utils/ASTTraversePrint.h"
#include "test_utils/ASTCompare.h"
#include <vector>
#include <memory>

TEST_CASE("Rel Expr Test Small") {
    std::vector<Token> tokens = {
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
    };

    ParserContext ctx(tokens);
    ParserFactory::registerParsers();

    ctx.giveKey(RegistryKey::REL_EXPR);
    auto parser = ParserFactory::createParser<RelExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<RelExprNode>(parser->parse(ctx));

    auto testLeftChild = std::make_shared<VariableNode>("i",1);
    auto testRightChild = std::make_shared<ConstantNode>(0,1);
    auto testNode = std::make_shared<RelExprNode>(testLeftChild,testRightChild,RelExprOperator::Greater,1);

    if (compareRelExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintRelExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintRelExpr(node);
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
    REQUIRE(compareRelExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Small") {
    std::vector<Token> tokens = {
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::COND_EXPR);
    auto parser = ParserFactory::createParser<CondExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<CondExprNode>(parser->parse(ctx));

    auto testLeftChild = std::make_shared<VariableNode>("i",1);
    auto testRightChild = std::make_shared<ConstantNode>(0,1);
    auto testNode = std::make_shared<RelExprNode>(testLeftChild,testRightChild,RelExprOperator::Greater,1);

    if (compareCondExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareCondExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Small Milestone 1 Fix") {
    std::vector<Token> tokens = {
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
    };


    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::REL_EXPR);
    auto parser = ParserFactory::createParser<RelExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<RelExprNode>(parser->parse(ctx));

    auto testLeftChild = std::make_shared<ConstantNode>(1,1);
    auto testRightChild = std::make_shared<ExprNode>(
        std::make_shared<ConstantNode>(1,1),
        std::make_shared<ExprNode>(
                std::make_shared<ConstantNode>(0,1),
                std::make_shared<ConstantNode>(1,1),
                ExprOperator::Minus,1),
        ExprOperator::Modulo,1);
    auto testNode = std::make_shared<RelExprNode>(testLeftChild,testRightChild,RelExprOperator::GreaterEqual,1);

    if (compareRelExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareRelExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Small Milestone 1 Fix2") {
    std::vector<Token> tokens = {
            {TokenType::CONSTANT, "1"},
            {TokenType::RELATIONAL_OPERATOR, ">="},
            {TokenType::CONSTANT, "1"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
    };


    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::REL_EXPR);
    auto parser = ParserFactory::createParser<RelExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<RelExprNode>(parser->parse(ctx));

    auto testLeftChild = std::make_shared<ConstantNode>(1,1);
    auto testRightChild = std::make_shared<ExprNode>(
            std::make_shared<ConstantNode>(1,1),
            std::make_shared<ConstantNode>(0,1),
            ExprOperator::Modulo,1);
    auto testNode = std::make_shared<RelExprNode>(testLeftChild,testRightChild,RelExprOperator::GreaterEqual,1);

    if (compareRelExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareRelExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Small Milestone 1 Fix3") {
    std::vector<Token> tokens = {
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
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::COND_EXPR);
    auto parser = ParserFactory::createParser<CondExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<CondExprNode>(parser->parse(ctx));

    auto leftVariableNode = std::make_shared<ConstantNode>(1, 1);
    auto leftConstantNode = std::make_shared<ConstantNode>(0, 1);
    auto leftRelExprNode = std::make_shared<RelExprNode>(leftVariableNode, leftConstantNode, RelExprOperator::Equal, 1);
    auto rightVariableNode = std::make_shared<ConstantNode>(1, 1);
    auto rightConstantNode = std::make_shared<ConstantNode>(0, 1);
    auto rightRelExprNode = std::make_shared<RelExprNode>(rightVariableNode, rightConstantNode, RelExprOperator::Equal, 1);
    auto operand = std::make_shared<CondBinaryExprNode>(leftRelExprNode, rightRelExprNode, CondExprOperator::And, 1);
    auto testNode = std::make_shared<CondUnaryExprNode>(operand, CondExprOperator::Not, 1);

    if (compareCondExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareCondExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Small Edge for Not") {
    std::vector<Token> tokens = {
            {TokenType::CONDITIONAL_OPERATOR, "!"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::KEYWORD, "read"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, ")"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::COND_EXPR);
    auto parser = ParserFactory::createParser<CondExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<CondExprNode>(parser->parse(ctx));

    auto testLeftChild = std::make_shared<VariableNode>("read",1);
    auto testRightChild = std::make_shared<VariableNode>("while",1);
    auto testOperand = std::make_shared<RelExprNode>(testLeftChild,testRightChild,RelExprOperator::Greater,1);
    auto testNode = std::make_shared<CondUnaryExprNode>(testOperand,CondExprOperator::Not,1);

    if (compareCondExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareCondExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Medium") {
    std::vector<Token> tokens = {
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
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::COND_EXPR);
    auto parser = ParserFactory::createParser<CondExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<CondExprNode>(parser->parse(ctx));

    auto leftVariableNode = std::make_shared<ConstantNode>(1, 1);
    auto leftConstantNode = std::make_shared<ConstantNode>(0, 1);
    auto leftRelExprNode = std::make_shared<RelExprNode>(leftVariableNode, leftConstantNode, RelExprOperator::Equal, 1);
    auto rightVariableNode = std::make_shared<ConstantNode>(1, 1);
    auto rightConstantNode = std::make_shared<ConstantNode>(0, 1);
    auto rightRelExprNode = std::make_shared<RelExprNode>(rightVariableNode, rightConstantNode, RelExprOperator::Equal, 1);
    auto operand = std::make_shared<CondBinaryExprNode>(leftRelExprNode, rightRelExprNode, CondExprOperator::And, 1);
    auto testNode = std::make_shared<CondUnaryExprNode>(operand, CondExprOperator::Not, 1);

    if (compareCondExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareCondExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Medium-Hard") {
    std::vector<Token> tokens = {
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::COND_EXPR);
    auto parser = ParserFactory::createParser<CondExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<CondExprNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<CondBinaryExprNode>(
            std::make_shared<RelExprNode>(
                    std::make_shared<VariableNode>("i", 1),
                    std::make_shared<ConstantNode>(0, 1),
                    RelExprOperator::Greater, 1),
            std::make_shared<RelExprNode>(
                    std::make_shared<VariableNode>("i", 1),
                    std::make_shared<ConstantNode>(0, 1),
                    RelExprOperator::Greater, 1),
            CondExprOperator::And, 1);
    auto rightChild = std::make_shared<CondBinaryExprNode>(
            std::make_shared<RelExprNode>(
                    std::make_shared<VariableNode>("i", 1),
                    std::make_shared<ConstantNode>(0, 1),
                    RelExprOperator::Greater, 1),
            std::make_shared<RelExprNode>(
                    std::make_shared<VariableNode>("i", 1),
                    std::make_shared<ConstantNode>(0, 1),
                    RelExprOperator::Greater, 1),
            CondExprOperator::And, 1);
    auto testNode = std::make_shared<CondBinaryExprNode>(leftChild, rightChild, CondExprOperator::And, 1);

    if (compareCondExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareCondExprNodes(node,testNode));
}

TEST_CASE("Expr Test Medium 1") {
    std::vector<Token> tokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "5"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::EXPR);
    auto parser = ParserFactory::createParser<ExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<ExprNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<VariableNode>("x",1);
    auto rightChild = std::make_shared<ExprNode>(
            std::make_shared<VariableNode>("z",1),
            std::make_shared<ConstantNode>(5,1),
            ExprOperator::Times, 1);
    auto testNode = std::make_shared<ExprNode>(leftChild, rightChild, ExprOperator::Plus, 1);

    if (compareExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintExpr(node);
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
    REQUIRE(compareExprNodes(node,testNode));
}

TEST_CASE("Expr Test Medium 2") {
    std::vector<Token> tokens = {
            {TokenType::PUNCTUATION,         "("},
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT,            "5"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::EXPR);
    auto parser = ParserFactory::createParser<ExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<ExprNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<ExprNode>(
            std::make_shared<VariableNode>("x",1),
            std::make_shared<VariableNode>("z",1),
            ExprOperator::Plus, 1);
    auto rightChild = std::make_shared<ConstantNode>(5,1);
    auto testNode = std::make_shared<ExprNode>(leftChild, rightChild, ExprOperator::Times, 1);

    if (compareExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintExpr(node);
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
    REQUIRE(compareExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Hard 2") {
    std::vector<Token> tokens = {
            {TokenType::PUNCTUATION,         "("},
            {TokenType::IDENTIFIER,          "p"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT,          "10"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT,            "9"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT,            "11"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT,            "11"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::COND_EXPR);
    auto parser = ParserFactory::createParser<CondExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<CondExprNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<ExprNode>(
            std::make_shared<ExprNode>(
                    std::make_shared<ExprNode>(
                            std::make_shared<VariableNode>("p",1),
                            std::make_shared<ConstantNode>(10,1),
                            ExprOperator::Divided, 1),
                    std::make_shared<ConstantNode>(9,1),
                    ExprOperator::Times, 1),
            std::make_shared<ConstantNode>(11,1),
            ExprOperator::Plus, 1);
    auto rightChild = std::make_shared<ConstantNode>(11,1);
    auto testNode = std::make_shared<RelExprNode>(leftChild, rightChild, RelExprOperator::Less, 1);

    if (compareCondExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareCondExprNodes(node,testNode));
}

TEST_CASE("Cond Expr Test Hard 3") {
    std::vector<Token> tokens = {
            {TokenType::PUNCTUATION,         "("},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::IDENTIFIER,          "p"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT,          "10"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT,            "9"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT,            "11"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT,            "11"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::IDENTIFIER,          "p"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT,            "11"},
            {TokenType::PUNCTUATION,         ")"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::COND_EXPR);
    auto parser = ParserFactory::createParser<CondExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<CondExprNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<ExprNode>(
            std::make_shared<ExprNode>(
                    std::make_shared<ExprNode>(
                            std::make_shared<VariableNode>("p",1),
                            std::make_shared<ConstantNode>(10,1),
                            ExprOperator::Divided, 1),
                    std::make_shared<ConstantNode>(9,1),
                    ExprOperator::Times, 1),
            std::make_shared<ConstantNode>(11,1),
            ExprOperator::Plus, 1);
    auto rightChild = std::make_shared<ConstantNode>(11,1);
    auto leftNode = std::make_shared<RelExprNode>(leftChild, rightChild, RelExprOperator::Less, 1);
    auto rightNode = std::make_shared<RelExprNode>(
        std::make_shared<VariableNode>("p",1),
        std::make_shared<ConstantNode>(11,1),
        RelExprOperator::Less,1
    );
    auto testNode = std::make_shared<CondBinaryExprNode>(leftNode,rightNode,CondExprOperator::And,1);
    if (compareCondExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintCondExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintCondExpr(node);
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
    REQUIRE(compareCondExprNodes(node,testNode));
}

TEST_CASE("Expr Test Medium 3") {
    std::vector<Token> tokens = {
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT,            "5"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT,            "5"},
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::EXPR);
    auto parser = ParserFactory::createParser<ExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<ExprNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<ExprNode>(
            std::make_shared<ExprNode>(
                    std::make_shared<VariableNode>("x",1),
                    std::make_shared<VariableNode>("z",1),
                    ExprOperator::Plus, 1),
            std::make_shared<ConstantNode>(5,1),
            ExprOperator::Plus, 1);
    auto rightChild = std::make_shared<ConstantNode>(5,1);
    auto testNode = std::make_shared<ExprNode>(leftChild, rightChild, ExprOperator::Minus, 1);

    if (compareExprNodes(node,testNode)) {
        std::cout << "\n------  TEST PASSED! ------";
        std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
        std::cout << "\n------ TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintExpr(node);
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
    REQUIRE(compareExprNodes(node,testNode));
}

TEST_CASE("Expr Test Hard") {
    std::vector<Token> tokens = {
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
    };

    ParserFactory::registerParsers();
    ParserContext ctx(tokens);

    ctx.giveKey(RegistryKey::EXPR);
    auto parser = ParserFactory::createParser<ExprParser>(ctx);
    auto node = std::dynamic_pointer_cast<ExprNode>(parser->parse(ctx));

    auto leftChild = std::make_shared<ExprNode>(
            std::make_shared<VariableNode>("procedure",1),
            std::make_shared<ExprNode>(
                    std::make_shared<ExprNode>(
                            std::make_shared<VariableNode>("read", 1),
                            std::make_shared<VariableNode>("print",1),
                            ExprOperator::Times, 1),
                    std::make_shared<VariableNode>("x",1),
                    ExprOperator::Divided, 1),
            ExprOperator::Times, 1);
    auto rightChild = std::make_shared<ConstantNode>(5,1);
    auto testNode = std::make_shared<ExprNode>(leftChild, rightChild, ExprOperator::Times, 1);

    if (compareExprNodes(node,testNode)) {
    std::cout << "\n------  TEST PASSED! ------";
    std::cout << "\n------ VERIFY OUTPUT ------\n";
    } else {
    std::cout << "\n------ TEST FAILED! ------";
    std::cout << "\n------ CHECK OUTPUT ------\n";
    }
    std::cout << "\n------ EXPECTED TREE------\n\n";
    traversePrintExpr(testNode, 0, TraversalPrintType::TEST);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintExpr(node);
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
    REQUIRE(compareExprNodes(node,testNode));
}