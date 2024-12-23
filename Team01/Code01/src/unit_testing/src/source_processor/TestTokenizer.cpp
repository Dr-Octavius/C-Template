#include "../../../include/catch.hpp"
#include <iostream>

#include "sp/Tokenizer/CompositeLexer.h"
#include "sp/Tokenizer/Token.h"
#include <string>
#include "sp/errors/SimpleSyntaxError.h"

// Testing simple rules at an atomic level

TEST_CASE("KeywordRule Test") {
    CompositeLexer lexer;
    std::string program = "if then else while procedure call read print";
    std::vector<Token> expectedTokens = {
            {TokenType::KEYWORD, "if"},
            {TokenType::KEYWORD, "then"},
            {TokenType::KEYWORD, "else"},
            {TokenType::KEYWORD, "while"},
            {TokenType::KEYWORD, "procedure"},
            {TokenType::KEYWORD, "call"},
            {TokenType::KEYWORD, "read"},
            {TokenType::KEYWORD, "print"},
    };

    std::vector<Token> actualTokens = lexer.tokenize(program);

    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < expectedTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("NumberRule Test") {
    CompositeLexer lexer;
    std::string program = "123 4567 0";
    std::vector<Token> expectedTokens = {
            {TokenType::CONSTANT, "123"},
            {TokenType::CONSTANT, "4567"},
            {TokenType::CONSTANT, "0"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("IdentifierRule Test") {
    CompositeLexer lexer;
    std::string program = "x variable1 tempVAAAAAArIABlE Y valid4734variable";
    std::vector<Token> expectedTokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::IDENTIFIER, "variable1"},
            {TokenType::IDENTIFIER, "tempVAAAAAArIABlE"},
            {TokenType::IDENTIFIER, "Y"},
            {TokenType::IDENTIFIER, "valid4734variable"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("RelationalOperatorRule Test") {
    CompositeLexer lexer;
    std::string program = "< <= > >= == !=";
    std::vector<Token> expectedTokens = {
            {TokenType::RELATIONAL_OPERATOR, "<"},
            {TokenType::RELATIONAL_OPERATOR, "<="},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::RELATIONAL_OPERATOR, ">="},
            {TokenType::RELATIONAL_OPERATOR, "=="},
            {TokenType::RELATIONAL_OPERATOR, "!="},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("ArithmeticOperatorRule Test") {
    CompositeLexer lexer;
    std::string program = "+ - * / %";
    std::vector<Token> expectedTokens = {
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("AssignmentOperatorRule Test") {
    CompositeLexer lexer;
    std::string program = "=";
    std::vector<Token> expectedTokens = {
            {TokenType::ASSIGNMENT_OPERATOR, "="},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("PunctuationRule Test") {
    CompositeLexer lexer;
    std::string program = "; { } ( )";
    std::vector<Token> expectedTokens = {
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::PUNCTUATION, ")"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("ConditionalOperatorRule Test") {
    CompositeLexer lexer;
    std::string program = "&& || !";
    std::vector<Token> expectedTokens = {
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::CONDITIONAL_OPERATOR, "||"},
            {TokenType::CONDITIONAL_OPERATOR, "!"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

// Testing more complex constructs

TEST_CASE("Sample procedure with read and print statements") {
    CompositeLexer lexer;
    std::string program = R"(procedure SampleProc { read x;print y; })";
    std::vector<Token> expectedTokens = {
            {TokenType::KEYWORD, "procedure"},
            {TokenType::IDENTIFIER, "SampleProc"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::KEYWORD, "read"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "print"},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("Simple assignment statement") {
    CompositeLexer lexer;
    std::string program = "x = y + 3 * (z - 2);";
    std::vector<Token> expectedTokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "y"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "3"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "-"},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, ";"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("Simple assignment statement hotfix") {
    CompositeLexer lexer;
    std::string program = "x = right+3 + 2+left;";
    std::vector<Token> expectedTokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "right"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "3"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::CONSTANT, "2"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "left"},
            {TokenType::PUNCTUATION, ";"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

TEST_CASE("While statement") {
    CompositeLexer lexer;
    std::string program = "while (i != 0 && x > 5) { call compute; }";
    std::vector<Token> expectedTokens = {
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, "!="},
            {TokenType::CONSTANT, "0"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::KEYWORD, "call"},
            {TokenType::IDENTIFIER, "compute"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
    };

    auto actualTokens = lexer.tokenize(program);
    REQUIRE(actualTokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < actualTokens.size(); ++i) {
        REQUIRE(actualTokens[i].type == expectedTokens[i].type);
        REQUIRE(actualTokens[i].value == expectedTokens[i].value);
    }
}

// Testing invalid programs & edge cases

TEST_CASE("Invalid character should throw error") {
    CompositeLexer lexer;
    std::string program1 = "@";
    std::string program2 = "var/5   `";
    std::string program3 = "~";
    std::string program4 = "&";
    std::string program5 = "{[]}";
    std::string program6 = "!varname?";

    REQUIRE_THROWS_AS(lexer.tokenize(program1), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program2), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program3), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program4), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program5), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program6), SimpleSyntaxError);
}

TEST_CASE("Invalid variable name should throw error") {
    CompositeLexer lexer;
    std::string program1 = "temp_variable";
    std::string program2 = "03433";
    std::string program3 = "$var";
    std::string program4 = "temp_variable";
    std::string program5 = "9abc";
    std::string program6 = "name&";
    std::string program7 = "567procedure";

    REQUIRE_THROWS_AS(lexer.tokenize(program1), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program2), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program3), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program4), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program5), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program6), SimpleSyntaxError);
    REQUIRE_THROWS_AS(lexer.tokenize(program7), SimpleSyntaxError);
}

TEST_CASE("Edge cases that should work according to the rules") {
    CompositeLexer lexer;
    std::string program1 = "var/5";
    std::string program2 = "  var    /    5  ";
    std::string program3 = "someVariable!";
    std::string program4 = "||cool||";
    std::string program5 = "&&&&&&&&";

    std::vector<Token> expectedTokens1and2 = {
            {TokenType::IDENTIFIER, "var"},
            {TokenType::ARITHMETIC_OPERATOR, "/"},
            {TokenType::CONSTANT, "5"},
    };

    std::vector<Token> expectedTokens3 = {
            {TokenType::IDENTIFIER, "someVariable"},
            {TokenType::CONDITIONAL_OPERATOR, "!"},
    };

    std::vector<Token> expectedTokens4 = {
            {TokenType::CONDITIONAL_OPERATOR, "||"},
            {TokenType::IDENTIFIER, "cool"},
            {TokenType::CONDITIONAL_OPERATOR, "||"},
    };

    std::vector<Token> expectedTokens5 = {
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
            {TokenType::CONDITIONAL_OPERATOR, "&&"},
    };

    auto actualTokens1 = lexer.tokenize(program1);
    auto actualTokens2 = lexer.tokenize(program2);
    auto actualTokens3 = lexer.tokenize(program3);
    auto actualTokens4 = lexer.tokenize(program4);
    auto actualTokens5 = lexer.tokenize(program5);

    REQUIRE(actualTokens1.size() == expectedTokens1and2.size());
    REQUIRE(actualTokens2.size() == expectedTokens1and2.size());
    REQUIRE(actualTokens3.size() == expectedTokens3.size());
    REQUIRE(actualTokens4.size() == expectedTokens4.size());
    REQUIRE(actualTokens5.size() == expectedTokens5.size());

    for (std::size_t i = 0; i < actualTokens1.size(); ++i) {
        REQUIRE(actualTokens1[i].type == expectedTokens1and2[i].type);
        REQUIRE(actualTokens1[i].value == expectedTokens1and2[i].value);

        REQUIRE(actualTokens2[i].type == expectedTokens1and2[i].type);
        REQUIRE(actualTokens2[i].value == expectedTokens1and2[i].value);
    }

    for (std::size_t i = 0; i < actualTokens3.size(); ++i) {
        REQUIRE(actualTokens3[i].type == expectedTokens3[i].type);
        REQUIRE(actualTokens3[i].value == expectedTokens3[i].value);
    }

    for (std::size_t i = 0; i < actualTokens4.size(); ++i) {
        REQUIRE(actualTokens4[i].type == expectedTokens4[i].type);
        REQUIRE(actualTokens4[i].value == expectedTokens4[i].value);
    }

    for (std::size_t i = 0; i < actualTokens5.size(); ++i) {
        REQUIRE(actualTokens5[i].type == expectedTokens5[i].type);
        REQUIRE(actualTokens5[i].value == expectedTokens5[i].value);
    }
}

// Testing a valid, full program (which is a combination of complex constructs)

TEST_CASE("Tokenize program string (with and wihout newline char)") {
    CompositeLexer lexer;
    std::string program1 = "while (i > 0) {    x = x+z     * 5; z = 2; if ( x  >0)   then { a = 3;} else { b=x%5 ; }}";
    std::string program2 = "while (i > 0) {\n"
                          "    x = x + z * 5;\n"
                          "    z = 2;\n"
                          "    if (x > 0)   then {\n"
                          "        a = 3;\n"
                          "    } else {\n"
                          "        b = x % 5;\n"
                          "    }\n"
                          "}";
    std::vector<Token> expectedTokens = {
            {TokenType::KEYWORD, "while"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "i"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "+"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ARITHMETIC_OPERATOR, "*"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::IDENTIFIER, "z"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "2"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::KEYWORD, "if"},
            {TokenType::PUNCTUATION, "("},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::RELATIONAL_OPERATOR, ">"},
            {TokenType::CONSTANT, "0"},
            {TokenType::PUNCTUATION, ")"},
            {TokenType::KEYWORD, "then"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::CONSTANT, "3"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::KEYWORD, "else"},
            {TokenType::PUNCTUATION, "{"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::ASSIGNMENT_OPERATOR, "="},
            {TokenType::IDENTIFIER, "x"},
            {TokenType::ARITHMETIC_OPERATOR, "%"},
            {TokenType::CONSTANT, "5"},
            {TokenType::PUNCTUATION, ";"},
            {TokenType::PUNCTUATION, "}"},
            {TokenType::PUNCTUATION, "}"}
    };

    std::vector<Token> actualTokensP1 = lexer.tokenize(program1);
    std::vector<Token> actualTokensP2 = lexer.tokenize(program2);

    REQUIRE(actualTokensP1.size() == expectedTokens.size());
    REQUIRE(actualTokensP2.size() == expectedTokens.size());

    for (std::size_t i = 0; i < expectedTokens.size(); ++i) {
        REQUIRE(actualTokensP1[i].type == expectedTokens[i].type);
        REQUIRE(actualTokensP1[i].value == expectedTokens[i].value);
        REQUIRE(actualTokensP2[i].type == expectedTokens[i].type);
        REQUIRE(actualTokensP2[i].value == expectedTokens[i].value);
    }
};

// Testing tokenizer's ability to parse large programs and check its validity (performance testing)

TEST_CASE("Test if tokenizer can withstand a relatively large and correct program") {
    CompositeLexer lexer;
    std::string program1 = "procedure X {\n"
                           "alpha = alpha + 2;\n"
                           "s = t * 4 - u;\n"
                           "if ((m == n) && ((m == o) && (m ==p))) then {\n"
                           "if ((o != p) || (!( (o > p) && (p <= o) ))) then {\n"
                           "read input404;\n"
                           "v = (e + f - g / h);\n"
                           "} else {\n"
                           "while(j != j) {\n"
                           "print output404;\n"
                           "}\n"
                           "}\n"
                           "} else {\n"
                           "m = (n + (o * p));\n"
                           "}\n"
                           "call Y;\n"
                           "}\n"
                           "\n"
                           "procedure Y {\n"
                           "while ((!(e != 0) && ( 0 > 2))) {\n"
                           "f = e - 2;\n"
                           "call W;\n"
                           "if (u != s) then {\n"
                           "read e;\n"
                           "} else {\n"
                           "call Z;\n"
                           "}\n"
                           "print e;\n"
                           "print output505;\n"
                           "k =  (k + (2 + 3 + (4 + 5 + (6))));\n"
                           "while ((t > 5) || ((t <= 2) && (t != 6))) {\n"
                           "while (t < 7) {\n"
                           "t = t - 2;\n"
                           "if (s <= 0) then {\n"
                           "e = e + (6 * 11);\n"
                           "}\n"
                           "else {\n"
                           "u = v + 3 + 2 + 4 + 5 + 6 + 7 + 8 + q;\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "call AA;\n"
                           "}\n"
                           "\n"
                           "procedure Z {\n"
                           "if ( n >= 2) then {\n"
                           "read input602;\n"
                           "if ( e == 2 ) then {\n"
                           "read aMassiveInputToCheckParsingLOLHopeItSucceeds8372is43TheTruth;\n"
                           "} else {\n"
                           "complexFormulaHereYAY = (2 + (2+3 + (2+3+4 + (2 + 3 + 4 * 5 * 6 / 7 - 8))));\n"
                           "}\n"
                           "} else {\n"
                           "while (o < 1) {\n"
                           "e = o + 2;\n"
                           "if (n == m + o) then {\n"
                           "n = n * 4 + o / p % 6 + 3 + 4 + 6 + 7 + 8;\n"
                           "o = g + 3 - p;\n"
                           "} else {\n"
                           "if((e==1)&&((f==1)&&(g==1))) then {\n"
                           "e = h + j + e + k + f + g + 2;\n"
                           "} else {\n"
                           "f = g + (e + 2);\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "call BB;\n"
                           "}\n"
                           "\n"
                           "}\n"
                           "\n"
                           "procedure W {\n"
                           "while (t == 1) {\n"
                           "print result404;\n"
                           "read result45678;\n"
                           "while(j == t + 7 % j) {\n"
                           "if (o != e) then {\n"
                           "while (m <= q) {\n"
                           "while (e == f - g * 25 / 11 + 25 % 6 ) {\n"
                           "v = 6 - (n / o % 3);\n"
                           "call BB;\n"
                           "}\n"
                           "}\n"
                           "} else {\n"
                           "while (e - 6 < f) {\n"
                           "if(f == g - m + x) then {\n"
                           "e = 5 / (e % f * g);\n"
                           "} else {\n"
                           "read input501;\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "\n";

    REQUIRE_NOTHROW(lexer.tokenize(program1));
}

TEST_CASE("Test if a small error introduced in same program causes error to be thrown") {
    CompositeLexer lexer;
    // Added invalid variable 123Variable, everything else is same as program in previous test case
    std::string program1 = "procedure X {\n"
                           "alpha = alpha + 2;\n"
                           "s = t * 4 - u;\n"
                           "if ((m == n) && ((m == o) && (m ==p))) then {\n"
                           "if ((o != p) || (!( (o > p) && (p <= o) ))) then {\n"
                           "read input404;\n"
                           "v = (e + f - g / h);\n"
                           "} else {\n"
                           "while(j != j) {\n"
                           "print output404;\n"
                           "}\n"
                           "}\n"
                           "} else {\n"
                           "m = (n + (o * p));\n"
                           "}\n"
                           "call Y;\n"
                           "}\n"
                           "\n"
                           "procedure Y {\n"
                           "while ((!(e != 0) && ( 0 > 2))) {\n"
                           "f = e - 2;\n"
                           "call W;\n"
                           "if (u != s) then {\n"
                           "read e;\n"
                           "} else {\n"
                           "call Z;\n"
                           "}\n"
                           "print e;\n"
                           "print output505;\n"
                           "k =  (k + (2 + 3 + (4 + 5 + (6))));\n"
                           "while ((t > 5) || ((t <= 2) && (t != 6))) {\n"
                           "while (t < 7) {\n"
                           "t = t - 2;\n"
                           "if (s <= 0) then {\n"
                           "e = e + (6 * 11);\n"
                           "}\n"
                           "else {\n"
                           "u = v + 3 + 2 + 4 + 5 + 6 + 7 + 8 + q;\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "call AA;\n"
                           "}\n"
                           "\n"
                           "procedure Z {\n"
                           "if ( n >= 2) then {\n"
                           "read input602;\n"
                           "if ( e == 2 ) then {\n"
                           "read aMassiveInputToCheckParsingLOLHopeItSucceeds8372is43TheTruth;\n"
                           "} else {\n"
                           "complexFormulaHereYAY = (2 + (2+3 + (2+3+4 + (2 + 3 + 4 * 5 * 6 / 7 - 8))));\n"
                           "}\n"
                           "} else {\n"
                           "while (o < 1) {\n"
                           "e = o + 2;\n"
                           "if (n == m + o) then {\n"
                           "n = n * 4 + o / p % 6 + 3 + 4 + 6 + 7 + 8;\n"
                           "o = g + 3 - p;\n"
                           "} else {\n"
                           "if((e==1)&&((f==1)&&(g==1))) then {\n"
                           "e = h + j + e + k + f + g + 2;\n"
                           "} else {\n"
                           "f = g + (e + 2);\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "call BB;\n"
                           "}\n"
                           "\n"
                           "}\n"
                           "\n"
                           "procedure W {\n"
                           "while (t == 1) {\n"
                           "print result404;\n"
                           "read result45678;\n"
                           "while(j == t + 7 % j) {\n"
                           "if (o != 123Variable) then {\n"
                           "while (m <= q) {\n"
                           "while (e == f - g * 25 / 11 + 25 % 6 ) {\n"
                           "v = 6 - (n / o % 3);\n"
                           "call BB;\n"
                           "}\n"
                           "}\n"
                           "} else {\n"
                           "while (e - 6 < f) {\n"
                           "if(f == g - m + x) then {\n"
                           "e = 5 / (e % f * g);\n"
                           "} else {\n"
                           "read input501;\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "}\n"
                           "\n";

    REQUIRE_THROWS_AS(lexer.tokenize(program1), SimpleSyntaxError);
}

TEST_CASE("Space Test") {
    CompositeLexer lexer;
    std::string program = "procedure\n"
                          "              simpleProcedure\n"
                          "\n"
                          "{\n"
                          "            read\n"
                          "                   x1       ;\n"
                          "    while\n"
                          "(x1 <=          50       )\n"
                          "    {\n"
                          "\n"
                          "\n"
                          "        x3\n"
                          "            =\n"
                          "                x1\n"
                          "                    *\n"
                          "                        2\n"
                          "                            +\n"
                          "                                (\n"
                          "                                    10\n"
                          "                                        *\n"
                          "                                            3\n"
                          "                                                )\n"
                          "                                                    ;\n"
                          "    if (        x3  <      10 )\n"
                          "then\n"
                          "        {\n"
                          "            print\n"
                          "            x3\n"
                          "            ;\n"
                          "        }          else\n"
                          "        {\n"
                          "            x6\n"
                          "            =\n"
                          "            0;\n"
                          "        }\n"
                          "        x7 =\n"
                          "                     x1 + 2;\n"
                          "    }\n"
                          "    x8\n"
                          "\n"
                          "    =\n"
                          "\n"
                          "    x1\n"
                          "\n"
                          "    *\n"
                          "\n"
                          "    2\n"
                          "\n"
                          "    ;\n"
                          "          x9 =      (     x8 +      (x1      / 2     ) )  /    2 + 3\n"
                          "          ;\n"
                          "    print     x1;\n"
                          "\n"
                          "\n"
                          "            }";

    REQUIRE_NOTHROW(lexer.tokenize(program));
}