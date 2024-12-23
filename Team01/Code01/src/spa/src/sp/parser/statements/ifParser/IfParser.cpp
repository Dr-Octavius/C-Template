#include "IfParser.h"
#include "sp/parser/factory/ParserFactory.h" // Assuming this provides a method to get the appropriate parsers
#include "sp/parser/expressions/CondExprParser.h"
#include "ThenParser.h"
#include "ElseParser.h"

std::shared_ptr<IfNode> IfParser::parse(ParserContext& ctx) {

    int lineNum = ctx.getCurrentLine();

    // Always begin each parse method with the assertion of the return type
    static_assert(std::is_base_of<ASTNode, IfNode>::value, "<T> returned in AssignmentParser must be derived from ASTNode");

    // No consumption done in factory, all operations done in Parser

    // Verify and consume 'if'
    if (!ctx.expectToken(TokenType::KEYWORD, "if")) {
        throw SimpleSyntaxError("Expected 'if'");
    }

    // Verify and consume '('
    if (!ctx.expectToken(TokenType::PUNCTUATION, "(")) {
        throw SimpleSyntaxError("Expected '(' after 'if'");
    }

    // Parsing the conditional expression
    ctx.giveKey(RegistryKey::COND_EXPR);
    auto condExprParser = ParserFactory::createParser<CondExprParser>(ctx);
    auto condition = condExprParser->parse(ctx);

    // Verify and consume ')'
    if (!ctx.expectToken(TokenType::PUNCTUATION, ")")) {
        throw SimpleSyntaxError("Expected ')' after condition expression in 'if' statement");
    }

    // Parsing 'then' block
    ctx.giveKey(RegistryKey::THEN);
    auto thenParser = ParserFactory::createParser<ThenParser>(ctx);
    auto thenBlock = thenParser->parse(ctx);

    // Check 'then' block received
    if (!thenBlock) {
        throw SimpleSyntaxError("Then Block Statement List was not returned from If Parser");
    }

    // Parsing 'else' block
    ctx.giveKey(RegistryKey::ELSE);
    auto elseParser = ParserFactory::createParser<ElseParser>(ctx);
    auto elseBlock = elseParser->parse(ctx);

    // Check 'else' block received
    if (!elseBlock) {
        throw SimpleSyntaxError("Else Block Statement List was not returned from If Parser");
    }

    return std::make_shared<IfNode>(std::move(condition), std::move(thenBlock), std::move(elseBlock), lineNum);
}
