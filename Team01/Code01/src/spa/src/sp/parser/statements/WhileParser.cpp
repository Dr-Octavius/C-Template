#include "WhileParser.h"
#include "sp/parser/factory/ParserFactory.h" // Assumes this provides methods to get the appropriate parsers
#include "sp/parser/controlFlow/StmtListParser.h"
#include "sp/parser/expressions/CondExprParser.h"
#include "sp/parser/expressions/variables/IdentifierParser.h"

std::shared_ptr<WhileNode> WhileParser::parse(ParserContext& ctx) {

    // Always begin each parse method with the assertion of the return type
    static_assert(std::is_base_of<ASTNode, VariableNode>::value, "<T> returned in IdentifierParser must be derived from ASTNode");

    // No consumption done in factory, all operations done in Parser

    int lineNum = ctx.getCurrentLine();

    // Verify and consume while KEYWORD
    if (!ctx.expectToken(TokenType::KEYWORD, "while")) {
        throw SimpleSyntaxError("Expected while loop operator");
    }

    // Verify and consume '('
    if (!ctx.expectToken(TokenType::PUNCTUATION, "(")) {
        throw SimpleSyntaxError("Expected '(' after 'while'");
    }

    // Parsing the condition
    ctx.giveKey(RegistryKey::COND_EXPR);
    auto condExprParser = ParserFactory::createParser<CondExprParser>(ctx);
    auto condition = condExprParser->parse(ctx);

    // Check condition received
    if (!condition) {
        throw SimpleSyntaxError("Conditional Expression Node was not returned from StatmentParser");
    }

    // Verify and consume ')'
    if (!ctx.expectToken(TokenType::PUNCTUATION, ")")) {
        throw SimpleSyntaxError("Expected ')' after condition expression in 'while' statement");
    }

    // Verify and consume '{'
    if (!ctx.expectToken(TokenType::PUNCTUATION, "{")) {
        throw SimpleSyntaxError("Expected '{' to start the 'while' loop body");
    }

    // consume the current line when "{" is verified (i.e. increase line)
    ctx.consumeLine();

    // Use the factory to get and invoke the parser for the statement list inside the loop
    ctx.giveKey(RegistryKey::STMT_LIST);
    auto stmtListParser = ParserFactory::createParser<StmtListParser>(ctx);
    auto stmtList = stmtListParser->parse(ctx);

    // Check Statement List received
    if (!stmtList) {
        throw SimpleSyntaxError("Statement List Node was not returned from StatmentParser");
    }

    // Expect the closing '}' for the while loop body
    if (!ctx.expectToken(TokenType::PUNCTUATION, "}")) {
        throw SimpleSyntaxError("Expected '}' at the end of 'while' loop body");
    }

    return std::make_shared<WhileNode>(std::move(condition), std::move(stmtList), lineNum);
}