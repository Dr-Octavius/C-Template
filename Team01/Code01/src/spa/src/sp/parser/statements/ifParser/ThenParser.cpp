#include "ThenParser.h"
#include "sp/parser/controlFlow/StmtListParser.h"

std::shared_ptr<StmtListNode> ThenParser::parse(ParserContext& ctx) {
    // No consumption done in factory, all operations done in Parser

    // Verify and consume 'then'
    if (!ctx.expectToken(TokenType::KEYWORD, "then")) {
        throw SimpleSyntaxError("Expected 'then'");
    }

    // Verify and consume '{'
    if (!ctx.expectToken(TokenType::PUNCTUATION, "{")) {
        throw SimpleSyntaxError("Expected '{' to start the 'then' block");
    }

    // consume the current line when "{" is verified (i.e. increase line)
    ctx.consumeLine();

    // Dynamically get the statement list parser for the 'else' block
    ctx.giveKey(RegistryKey::STMT_LIST);
    auto thenStmtListParser = ParserFactory::createParser<StmtListParser>(ctx);
    auto thenStmtList = thenStmtListParser->parse(ctx);

    // Verify and consume '}'
    if (!ctx.expectToken(TokenType::PUNCTUATION, "}")) {
        throw SimpleSyntaxError("Expected '}' at the end of 'then' block");
    }

    return thenStmtList;
}