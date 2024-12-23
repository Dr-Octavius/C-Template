//
// Created by Michael on 25/3/24.
//

#include "../../../include/catch.hpp"

#include "common/Utils.h"
#include "sp/Tokenizer/Token.h"
#include "sp/parser/MainParser.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/astTraversal/ASTTraversal.h"
#include "sp/analyzer/SemanticAnalyzer.h"
#include "test_utils/ASTTraversePrint.h"
#include <vector>
#include <memory>

TEST_CASE("Semantics Check I") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "read"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT,            "3"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::IDENTIFIER,          "i"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT,            "5"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::KEYWORD,             "while"},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::KEYWORD,             "while"},
            {TokenType::RELATIONAL_OPERATOR, "!="},
            {TokenType::KEYWORD,             "read"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT,            "1"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::KEYWORD,             "if"},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT,            "1"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::KEYWORD,             "then"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT,            "1"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
            {TokenType::KEYWORD,             "else"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::IDENTIFIER,          "y"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER,          "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER,          "i"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::KEYWORD,             "print"},
            {TokenType::KEYWORD,             "print"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::IDENTIFIER,          "i"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER,          "i"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT,            "1"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
            // statement after if within while
            {TokenType::KEYWORD,             "print"},
            {TokenType::KEYWORD,             "print"},
            {TokenType::PUNCTUATION,         ";"},
            // if after while
            {TokenType::PUNCTUATION,         "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::cout << "\n------ NEXT MAP ------\n\n";
    printNextMap(traversal.context.getNextMap());
    std::vector<std::shared_ptr<CFGNode>> cfgList = traversal.context.cfgList;
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    REQUIRE(semanticAnalyzer.hasDuplicateProcedures() == false);
    REQUIRE(semanticAnalyzer.hasUndefinedProcedures() == false);
    REQUIRE(semanticAnalyzer.hasRecursiveAndCyclicCalls() == false);
}

TEST_CASE("Semantics Duplicate Check") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "read"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "read"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::cout << "\n------ NEXT MAP ------\n\n";
    printNextMap(traversal.context.getNextMap());
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    REQUIRE(semanticAnalyzer.hasDuplicateProcedures() == true);;
}

TEST_CASE("Semantics Undefined Check") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "call"},
            {TokenType::KEYWORD,             "procedure1"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::cout << "\n------ NEXT MAP ------\n\n";
    printNextMap(traversal.context.getNextMap());
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    REQUIRE(semanticAnalyzer.hasUndefinedProcedures() == true);
}

TEST_CASE("Semantics Cyclic Check") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "call"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::cout << "\n------ NEXT MAP ------\n\n";
    printNextMap(traversal.context.getNextMap());
    std::vector<std::shared_ptr<CFGNode>> cfgList = traversal.context.cfgList;
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    REQUIRE(semanticAnalyzer.hasRecursiveAndCyclicCalls() == true);
}

TEST_CASE("Semantics Recursive Check") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "call"},
            {TokenType::KEYWORD,             "procedure1"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure1"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "call"},
            {TokenType::KEYWORD,             "procedure2"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure2"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "call"},
            {TokenType::KEYWORD,             "procedure3"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::KEYWORD,             "procedure3"},
            {TokenType::PUNCTUATION,         "{"},
            {TokenType::KEYWORD,             "call"},
            {TokenType::KEYWORD,             "procedure"},
            {TokenType::PUNCTUATION,         ";"},
            {TokenType::PUNCTUATION,         "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ProgramNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::vector<std::shared_ptr<CFGNode>> cfgList = traversal.context.cfgList;
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintProgram(astRoot);
    REQUIRE(semanticAnalyzer.hasRecursiveAndCyclicCalls() == true);
}

TEST_CASE("MS2 Check") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "double"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
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
            {TokenType::PUNCTUATION,         "("},
            {TokenType::IDENTIFIER,          "p"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT,            "11"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::PUNCTUATION,         "("},
            {TokenType::IDENTIFIER,          "q"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT,            "2"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::PUNCTUATION,         ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "p"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "q"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER,          "p"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT,          "10"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT,            "9"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT,            "11"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT,            "11"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "3"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::RELATIONAL_OPERATOR, "<="},
            {TokenType::CONSTANT, "10"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "1"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "p"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "single"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "12"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "c"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT, "8"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
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
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "1"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "r"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "e"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "p"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "q"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONDITIONAL_OPERATOR, "!"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "17"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::RELATIONAL_OPERATOR, "!="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "p"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "q"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "9"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "c"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "c"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "1"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "single"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::IDENTIFIER, "c"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "26"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT, "8"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "1"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::IDENTIFIER, "c"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "29"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "29"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, ">="},
            {TokenType::CONSTANT, "55555"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "1234567"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "child2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "1"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "12"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT, "13"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "14"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "36"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "child"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "p"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::IDENTIFIER, "q"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "41"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "var"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "var"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT, "2"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "var"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "54321"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "99"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::IDENTIFIER, "c"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "47"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            // Note: The following line appears to be pseudocode or an error:
            // read = print;
            // It has been translated as if `read` and `print` were identifiers being assigned,
            // which is likely not correct in a real programming language context.
            {TokenType::KEYWORD, "read"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::KEYWORD, "print"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "print"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::KEYWORD, "read"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::KEYWORD, "if"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::KEYWORD, "while"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::KEYWORD, "assign"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::KEYWORD, "call"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "11"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "12"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "13"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "y"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "51"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "8"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "6"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "17"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "single"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            // Procedure "single"
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "single"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "p"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "q"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "55"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "child"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "child2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "c"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},

            // Procedure "child"
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "child"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},

            // Procedure "child2"
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "child2"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"}
    };

    MainParser parser(tokens);
    std::shared_ptr<ProgramNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::vector<std::shared_ptr<CFGNode>> cfgList = traversal.context.cfgList;
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    std::cout << "\n------ NEXT MAP ------\n\n";
    printNextMap(traversal.context.getNextMap());
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintProgram(astRoot);
    REQUIRE(semanticAnalyzer.hasRecursiveAndCyclicCalls() == false);
    REQUIRE(semanticAnalyzer.hasUndefinedProcedures() == false);
    REQUIRE(semanticAnalyzer.hasDuplicateProcedures() == false);
}

TEST_CASE("MS2 Check II") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "simpleProcedure"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "x1"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x1"},
            {TokenType::RELATIONAL_OPERATOR, "<="},
            {TokenType::CONSTANT, "50"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x3"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x1"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::CONSTANT, "10"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x3"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT, "10"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "x3"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x3"},
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::CONSTANT, "10"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "x3"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x6"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::IDENTIFIER, "x6"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::IDENTIFIER, "x7"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x1"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},

            {TokenType::IDENTIFIER, "x8"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x1"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "x9"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x8"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x1"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT, "2"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "x1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
    };

    MainParser parser(tokens);
    std::shared_ptr<ASTNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::vector<std::shared_ptr<CFGNode>> cfgList = traversal.context.cfgList;
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    std::cout << "\n------ NEXT MAP ------\n\n";
    printNextMap(traversal.context.getNextMap());
    REQUIRE(semanticAnalyzer.hasRecursiveAndCyclicCalls() == false);
    REQUIRE(semanticAnalyzer.hasUndefinedProcedures() == false);
    REQUIRE(semanticAnalyzer.hasDuplicateProcedures() == false);
}

TEST_CASE("MS3 Check") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "Test"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::IDENTIFIER, "y"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER,          "i"},
            {TokenType::RELATIONAL_OPERATOR, "!="},
            {TokenType::CONSTANT,          "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER,          "t"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT,          "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER,          "y"},
            {TokenType::RELATIONAL_OPERATOR, "!="},
            {TokenType::CONSTANT,          "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "y"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER,          "z"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::CONSTANT,          "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "t"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},

            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "y"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "1"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER,          "y"},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::IDENTIFIER,            "x"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "t"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"},

            // Procedure "single"
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},

            // Procedure "child"
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::PUNCTUATION, "{"},

            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},

            {TokenType::PUNCTUATION, "}"},

    };

    MainParser parser(tokens);
    std::shared_ptr<ProgramNode> astRoot = parser.parse();
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    std::vector<std::shared_ptr<CFGNode>> cfgList = traversal.context.cfgList;
    SemanticAnalyzer semanticAnalyzer(traversal.context);
    std::cout << "\n------ NEXT MAP ------\n\n";
    printNextMap(traversal.context.getNextMap());
    std::cout << "\n------ ACTUAL TREE ------\n\n";
    traversePrintProgram(astRoot);
    REQUIRE(semanticAnalyzer.hasRecursiveAndCyclicCalls() == false);
    REQUIRE(semanticAnalyzer.hasUndefinedProcedures() == false);
    REQUIRE(semanticAnalyzer.hasDuplicateProcedures() == false);
}
