#include "AssignmentParser.h"

std::shared_ptr<AssignNode> AssignmentParser::parse(ParserContext& ctx) {

    int lineNum = ctx.getCurrentLine();

    // Always begin each parse method with the assertion of the return type
    static_assert(std::is_base_of<ASTNode, AssignNode>::value, "<T> returned in AssignmentParser must be derived from ASTNode");

    // No consumption done in factory, all operations done in Parser

    // Parsing the LHS
    ctx.giveKey(RegistryKey::IDENTIFIER);
    auto identifierParser = ParserFactory::createParser<IdentifierParser>(ctx);
    auto varNode = identifierParser->parse(ctx);

    // Check LHS received
    if (!varNode) {
        throw SimpleSyntaxError("LHS Variable was not returned from Assignment Parser");
    }

    // Verify and consume ASSIGNMENT_OPERATOR
    if (!ctx.expectToken(TokenType::ASSIGNMENT_OPERATOR, "=")) {
        throw SimpleSyntaxError("Expected '=' in assignment at line: " + std::to_string(ctx.getCurrentLine()));
    }

    // Parsing the RHS
    ctx.giveKey(RegistryKey::EXPR);
    auto exprParser = ParserFactory::createParser<ExprParser>(ctx);
    auto relFactorNode = exprParser->parse(ctx);

    // Check Expression received
    if (!relFactorNode) {
        throw SimpleSyntaxError("Rel Factor Node was not returned from Parsing Statement at line: " + std::to_string(ctx.getCurrentLine()));
    }

    // Verify and consume ";"
    if (!ctx.expectToken(TokenType::PUNCTUATION, ";")) {
        throw SimpleSyntaxError("Expected ';' at the end of an assignment statement at line " + std::to_string(ctx.getCurrentLine()));
    }

    // consume the current line when ";" is verified (i.e. increase line)
    ctx.consumeLine();

    return std::make_shared<AssignNode>(varNode, std::move(relFactorNode), lineNum);
}
