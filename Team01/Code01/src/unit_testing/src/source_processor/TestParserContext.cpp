//
// Created by Michael on 26/2/24.
//

#include "../../include/catch.hpp"

#include "common/Utils.h"
#include "sp/tokenizer/Token.h"
#include "sp/parser/MainParser.h"
#include <vector>

TEST_CASE("Parser Context Test") {
    std::vector<Token> tokens = {
            {TokenType::KEYWORD, "while"},
    };

    ParserContext ctx(tokens);

    if (ctx.getCurrentLine() == 1) {
        std::cout << "\n------ START LINE TEST PASSED! ------";
    } else {
        std::cout << "\n------ START LINE TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
        std::cout << "Expected: 1, Actual: " << ctx.getCurrentLine();
    }

    if (ctx.getCurrentToken().type == TokenType::KEYWORD) {
        std::cout << "\n------ STORE TOKEN TYPE TEST PASSED! ------";
    } else {
        std::cout << "\n------ STORE TOKEN TYPE TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
        std::cout << "Expected TokenType to be KEYWORD for " << ctx.getCurrentToken().value;
    }

    if (ctx.getCurrentToken().value == "while") {
        std::cout << "\n------ STORE TOKEN VALUE TEST PASSED! ------";
    } else {
        std::cout << "\n------ STORE TOKEN VALUE TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
        std::cout << "Expected Token Value to be 'while' but got " << ctx.getCurrentToken().value;
    }

    if (ctx.peekNextToken().value == "<EOS>") {
        std::cout << "\n------ APPEND END STREAM TOKEN TEST PASSED! ------";
    } else {
        std::cout << "\n------ APPEND END STREAM TOKEN TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
        std::cout << "Expected endOfStreamToken to be appended";
    }

    REQUIRE(ctx.getCurrentLine() == 1);
    REQUIRE(ctx.getCurrentToken().type == TokenType::KEYWORD);
    REQUIRE(ctx.getCurrentToken().value == "while");
    REQUIRE(ctx.peekNextToken().value == "<EOS>");

    ctx.consumeToken();

    if (ctx.getCurrentToken().value == "<EOS>") {
        std::cout << "\n------ TOKEN CONSUMPTION TEST PASSED! ------";
    } else {
        std::cout << "\n------ TOKEN CONSUMPTION TEST FAILED! ------";
        std::cout << "\n------ CHECK OUTPUT ------\n";
        std::cout << "Expected endOfStreamToken to be reached, instead got " << ctx.getCurrentToken().value;
    }
    REQUIRE(ctx.getCurrentToken().type == TokenType::PUNCTUATION);
    REQUIRE(ctx.getCurrentToken().value == "<EOS>");
}