//
// Created by Michael on 26/2/24.
//
//#include "../../include/catch.hpp"
//
//#include "sp/tokenizer/Token.h"
//#include "sp/parser/MainParser.h"
//#include "sp/ast/nodes/ASTNode.h"
//#include "test_utils/ASTTraversePrint.h"
//#include "test_utils/ASTCompare.h"
//#include <vector>
//#include <memory>
//
//TEST_CASE("Parse Main Test") {
//    std::vector<Token> tokens = {
//        {TokenType::KEYWORD, "procedure"},
//        {TokenType::IDENTIFIER, "main"},
//        {TokenType::PUNCTUATION, "{"},
//        {TokenType::KEYWORD, "while"},
//        {TokenType::PUNCTUATION, "("},
//        {TokenType::IDENTIFIER, "i"},
//        {TokenType::RELATIONAL_OPERATOR, ">"},
//        {TokenType::CONSTANT, "0"},
//        {TokenType::PUNCTUATION, ")"},
//        {TokenType::PUNCTUATION, "{"},
//        {TokenType::IDENTIFIER, "x"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::IDENTIFIER, "x"},
//        {TokenType::ARITHMETIC_OPERATOR, "+"},
//        {TokenType::IDENTIFIER, "z"},
//        {TokenType::ARITHMETIC_OPERATOR, "*"},
//        {TokenType::CONSTANT, "5"},
//        {TokenType::PUNCTUATION, ";"},
//        {TokenType::IDENTIFIER, "z"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::CONSTANT, "2"},
//        {TokenType::PUNCTUATION, ";"},
//        {TokenType::KEYWORD, "if"},
//        {TokenType::PUNCTUATION, "("},
//        {TokenType::IDENTIFIER, "x"},
//        {TokenType::RELATIONAL_OPERATOR, ">"},
//        {TokenType::CONSTANT, "0"},
//        {TokenType::PUNCTUATION, ")"},
//        {TokenType::KEYWORD, "then"},
//        {TokenType::PUNCTUATION, "{"},
//        {TokenType::IDENTIFIER, "a"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::CONSTANT, "3"},
//        {TokenType::PUNCTUATION, ";"},
//        {TokenType::PUNCTUATION, "}"},
//        {TokenType::KEYWORD, "else"},
//        {TokenType::PUNCTUATION, "{"},
//        {TokenType::IDENTIFIER, "b"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::IDENTIFIER, "x"},
//        {TokenType::ARITHMETIC_OPERATOR, "%"},
//        {TokenType::CONSTANT, "5"},
//        {TokenType::PUNCTUATION, ";"},
//        {TokenType::PUNCTUATION, "}"},
//        // statement after if within while
//        {TokenType::IDENTIFIER, "val"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::CONSTANT, "24"},
//        {TokenType::PUNCTUATION, ";"},
//        {TokenType::PUNCTUATION, "}"},
//        {TokenType::IDENTIFIER, "r"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::CONSTANT, "2"},
//        {TokenType::PUNCTUATION, ";"},
//        // if after while
//        {TokenType::KEYWORD, "if"},
//        {TokenType::PUNCTUATION, "("},
//        {TokenType::IDENTIFIER, "x"},
//        {TokenType::RELATIONAL_OPERATOR, ">"},
//        {TokenType::CONSTANT, "0"},
//        {TokenType::PUNCTUATION, ")"},
//        {TokenType::KEYWORD, "then"},
//        {TokenType::PUNCTUATION, "{"},
//        {TokenType::IDENTIFIER, "a"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::CONSTANT, "3"},
//        {TokenType::PUNCTUATION, ";"},
//        {TokenType::PUNCTUATION, "}"},
//        {TokenType::KEYWORD, "else"},
//        {TokenType::PUNCTUATION, "{"},
//        {TokenType::IDENTIFIER, "b"},
//        {TokenType::ASSIGNMENT_OPERATOR, "="},
//        {TokenType::IDENTIFIER, "x"},
//        {TokenType::ARITHMETIC_OPERATOR, "%"},
//        {TokenType::CONSTANT, "5"},
//        {TokenType::PUNCTUATION, ";"},
//        {TokenType::PUNCTUATION, "}"},
//        {TokenType::PUNCTUATION, "}"},
//    };
//
//    MainParser parser(tokens);
//    std::shared_ptr<ProcedureNode> astRoot = parser.parse();
//
//    auto firstWhileBody = std::make_shared<StmtListNode>(2);
//    firstWhileBody->addStatement(std::make_shared<AssignNode>(
//        std::make_shared<VariableNode>("x",2),
//        std::make_shared<ExprNode>(
//            std::make_shared<VariableNode>("x",2),
//            std::make_shared<ExprNode>(
//                std::make_shared<VariableNode>("z",2),
//                std::make_shared<ConstantNode>(5,2),
//                ExprOperator::Times,2
//            ),
//            ExprOperator::Plus,2
//        ),
//        2
//    ));
//    firstWhileBody->addStatement(std::make_shared<AssignNode>(
//        std::make_shared<VariableNode>("z",3),
//        std::make_shared<ConstantNode>(2,3),
//        3
//    ));
//    auto thenBlock = std::make_shared<StmtListNode>(5);
//    thenBlock->addStatement(std::make_shared<AssignNode>(
//        std::make_shared<VariableNode>("a",5),
//        std::make_shared<ConstantNode>(3,5),
//        5
//    ));
//    auto elseBlock = std::make_shared<StmtListNode>(6);
//    elseBlock->addStatement(std::make_shared<AssignNode>(
//        std::make_shared<VariableNode>("b",6),
//        std::make_shared<ExprNode>(
//            std::make_shared<VariableNode>("x",6),
//            std::make_shared<ConstantNode>(5,6),
//            ExprOperator::Modulo,6
//        ),
//        6
//    ));
//    firstWhileBody->addStatement(std::make_shared<IfNode>(
//        std::make_shared<RelExprNode>(
//            std::make_shared<VariableNode>("x",4),
//            std::make_shared<ConstantNode>(0,4),
//            RelExprOperator::Greater, 4),
//        thenBlock,
//        elseBlock,
//        4
//    ));
//    firstWhileBody->addStatement(std::make_shared<AssignNode>(
//            std::make_shared<VariableNode>("val",7),
//            std::make_shared<ConstantNode>(24,7),
//            7
//    ));
//    auto firstWhileCondition = std::make_shared<RelExprNode>(
//        std::make_shared<VariableNode>("i",1),
//        std::make_shared<ConstantNode>(0,1),
//        RelExprOperator::Greater, 1
//    );
//    auto firstWhile = std::make_shared<WhileNode>(
//        firstWhileCondition,
//        firstWhileBody,
//        1
//    );
//    auto procedureBody = std::make_shared<StmtListNode>(1);
//    procedureBody->addStatement(firstWhile);
//    procedureBody->addStatement(std::make_shared<AssignNode>(
//            std::make_shared<VariableNode>("r",8),
//            std::make_shared<ConstantNode>(2,8),
//            8
//    ));
//    auto thenBlockOuter = std::make_shared<StmtListNode>(10);
//    thenBlockOuter->addStatement(std::make_shared<AssignNode>(
//            std::make_shared<VariableNode>("a",10),
//            std::make_shared<ConstantNode>(3,10),
//            10
//    ));
//    auto elseBlockOuter = std::make_shared<StmtListNode>(11);
//    elseBlockOuter->addStatement(std::make_shared<AssignNode>(
//            std::make_shared<VariableNode>("b",11),
//            std::make_shared<ExprNode>(
//                    std::make_shared<VariableNode>("x",11),
//                    std::make_shared<ConstantNode>(5,11),
//                    ExprOperator::Modulo,11
//            ),
//            11
//    ));
//    procedureBody->addStatement(std::make_shared<IfNode>(
//            std::make_shared<RelExprNode>(
//                    std::make_shared<VariableNode>("x",9),
//                    std::make_shared<ConstantNode>(0,9),
//                    RelExprOperator::Greater, 9
//            ),
//            thenBlockOuter,
//            elseBlockOuter,
//            9
//    ));
//    auto testNode = std::make_shared<ProcedureNode>(
//        "main",
//        procedureBody,
//        0
//    );
//
//    if (compareProcedureNodes(astRoot,testNode)) {
//        std::cout << "\n------  TEST PASSED! ------";
//        std::cout << "\n------ VERIFY OUTPUT ------\n";
//    } else {
//        std::cout << "\n------ TEST FAILED! ------";
//        std::cout << "\n------ CHECK OUTPUT ------\n";
//    }
//    std::cout << "\n------ EXPECTED TREE------\n\n";
//    traversePrintProcedure(testNode, 0, TraversalType::TEST);
//    std::cout << "\n------ ACTUAL TREE ------\n\n";
//    traversePrintProcedure(astRoot);
//    std::cout << "\n------ VERBOSE LINE NUMBER CHECK ------";
//    std::cout << "\n------ EXPECTED LINE NUMBER ------\n\n";
//    printTestStatementLineMap();
//    std::cout << "\n------ ACTUAL LINE NUMBER ------\n\n";
//    printActualStatementLineMap();
//    std::cout << "\n------ PROGRAM LINE NUMBER CHECK ------";
//    std::cout << "\n------ EXPECTED PROGRAM LINE NUMBER ------\n\n";
//    printTestStatementLines();
//    std::cout << "\n------ ACTUAL PROGRAM LINE NUMBER ------\n\n";
//    printActualStatementLines();
//    std::cout << "\n";
//    REQUIRE(compareProcedureNodes(astRoot,testNode));
//}

// More Test Cases
//TEST_CASE("Parse Main Test II") {
//    std::vector<Token> tokens = {
//            {TokenType::KEYWORD, "procedure"},
//            {TokenType::KEYWORD, "procedure"},
//            {TokenType::PUNCTUATION, "{"},
//            {TokenType::KEYWORD, "read"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::KEYWORD, "procedure"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::IDENTIFIER, "z"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::CONSTANT, "3"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::IDENTIFIER, "i"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::CONSTANT, "5"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::KEYWORD, "while"},
//            {TokenType::PUNCTUATION, "("},
//            {TokenType::KEYWORD, "while"},
//            {TokenType::RELATIONAL_OPERATOR, "!="},
//            {TokenType::KEYWORD, "read"},
//            {TokenType::PUNCTUATION, ")"},
//            {TokenType::PUNCTUATION, "{"},
//            {TokenType::IDENTIFIER, "x"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::IDENTIFIER, "x"},
//            {TokenType::ARITHMETIC_OPERATOR, "-"},
//            {TokenType::CONSTANT, "1"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::KEYWORD, "if"},
//            {TokenType::PUNCTUATION, "("},
//            {TokenType::IDENTIFIER, "x"},
//            {TokenType::RELATIONAL_OPERATOR, "=="},
//            {TokenType::CONSTANT, "1"},
//            {TokenType::PUNCTUATION, ")"},
//            {TokenType::KEYWORD, "then"},
//            {TokenType::PUNCTUATION, "{"},
//            {TokenType::IDENTIFIER, "z"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::IDENTIFIER, "x"},
//            {TokenType::ARITHMETIC_OPERATOR, "+"},
//            {TokenType::CONSTANT, "1"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::PUNCTUATION, "}"},
//            {TokenType::KEYWORD, "else"},
//            {TokenType::PUNCTUATION, "{"},
//            {TokenType::IDENTIFIER, "y"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::IDENTIFIER, "z"},
//            {TokenType::ARITHMETIC_OPERATOR, "+"},
//            {TokenType::IDENTIFIER, "x"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::PUNCTUATION, "}"},
//            {TokenType::IDENTIFIER, "z"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::IDENTIFIER, "z"},
//            {TokenType::ARITHMETIC_OPERATOR, "+"},
//            {TokenType::IDENTIFIER, "x"},
//            {TokenType::ARITHMETIC_OPERATOR, "+"},
//            {TokenType::IDENTIFIER, "i"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::KEYWORD, "print"},
//            {TokenType::KEYWORD, "print"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::IDENTIFIER, "i"},
//            {TokenType::ASSIGNMENT_OPERATOR, "="},
//            {TokenType::IDENTIFIER, "i"},
//            {TokenType::ARITHMETIC_OPERATOR, "-"},
//            {TokenType::CONSTANT, "1"},
//            {TokenType::PUNCTUATION, ";"},
//            {TokenType::PUNCTUATION, "}"},
//            // statement after if within while
//            {TokenType::KEYWORD, "print"},
//            {TokenType::KEYWORD, "print"},
//            {TokenType::PUNCTUATION, ";"},
//            // if after while
//            {TokenType::PUNCTUATION, "}"},
//    };
//
//    MainParser parser(tokens);
//    std::shared_ptr<ASTNode> astRoot = parser.parse();
//
//}
