#include "CallParser.h"

std::shared_ptr<CallNode> CallParser::parse(ParserContext& ctx) {
    // Verify and consume KEYWORD
    if (!ctx.expectToken(TokenType::KEYWORD, "call")) {
        throw SimpleSyntaxError("Expected call here");
    }

    std::string procName = ctx.getCurrentToken().value;
    int lineNum = ctx.getCurrentLine();

    // Verify and consume IDENTIFIER (procedure name)
    if (ctx.getCurrentToken().type != TokenType::KEYWORD) {
        if (ctx.getCurrentToken().type != TokenType::IDENTIFIER) {
            throw SimpleSyntaxError("Expected procedure name after 'call'");
        } else {
            ctx.consumeToken();
        }
    } else {
        ctx.consumeToken();
    }

    // Expect the closing ';' for the else block
    if (!ctx.expectToken(TokenType::PUNCTUATION, ";")) {
        throw SimpleSyntaxError("Expected '}' at the end of 'else' block");
    }

    // consume the current line when ";" is verified (i.e. increase line)
    ctx.consumeLine();

    return std::make_shared<CallNode>(procName,lineNum);
}
