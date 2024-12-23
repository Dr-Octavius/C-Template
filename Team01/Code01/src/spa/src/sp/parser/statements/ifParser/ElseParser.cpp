#include "ElseParser.h"
#include "sp/parser/controlFlow/StmtListParser.h"

std::shared_ptr<StmtListNode> ElseParser::parse(ParserContext& ctx) {
    // No consumption done in factory, all operations done in Parser

    // Verify and consume 'else'
    if (!ctx.expectToken(TokenType::KEYWORD, "else")) {
        throw SimpleSyntaxError("Expected 'else' statements in 'if' statement");
    }

    // Expect the opening '{' for the else block
    if (!ctx.expectToken(TokenType::PUNCTUATION, "{")) {
        throw SimpleSyntaxError("Expected '{' to start the 'else' block");
    }

    // Dynamically get the statement list parser for the 'else' block
    ctx.giveKey(RegistryKey::STMT_LIST);
    auto elseStmtListParser = ParserFactory::createParser<StmtListParser>(ctx);
    auto elseStmtList = elseStmtListParser->parse(ctx);

    // Expect the closing '}' for the else block
    if (!ctx.expectToken(TokenType::PUNCTUATION, "}")) {
        throw SimpleSyntaxError("Expected '}' at the end of 'else' block");
    }

    return elseStmtList;
}