#include "RelExprParser.h"
#include "sp/parser/factory/ParserFactory.h" // For obtaining factor parsers

std::shared_ptr<RelExprNode> RelExprParser::parse(ParserContext& ctx) {
    // Defaults to parsing LHS
    auto left = parseRelFactor(ctx);

    // Get enum
    RelExprOperator opEnum = RelExprOperatorUtils::fromString(ctx.getCurrentToken().value);

    // Verify & Consume '<', '<=', '>', '>=', '==', '!='
    if (!ctx.expectToken(TokenType::RELATIONAL_OPERATOR)) {
        throw SimpleSyntaxError("Expected Relational Operator in Relational Expression");
    }

    // Parsing RHS
    auto right = parseRelFactor(ctx);

    return std::make_shared<RelExprNode>(std::move(left), std::move(right), opEnum, ctx.getCurrentLine());
}

std::shared_ptr<RelFactorNode> RelExprParser::parseRelFactor(ParserContext& ctx) {
    // Parsing Rel Factor (A Rel Factor And Expr are equivalent)
    ctx.giveKey(RegistryKey::EXPR);
    auto exprParser = ParserFactory::createParser<ExprParser>(ctx);
    auto expr = exprParser->parse(ctx);

    // Check Expr Received
    if (!expr) {
        throw SimpleSyntaxError("Rel Factor was not returned from Relational Factor Parser");
    }

    return expr;
}
