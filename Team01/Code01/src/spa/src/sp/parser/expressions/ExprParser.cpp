#include "ExprParser.h"
#include "sp/parser/expressions/variables/ConstantParser.h"

std::shared_ptr<RelFactorNode> ExprParser::parse(ParserContext& ctx) {
    // Defaults to parsing LHS
    auto left = parseTerm(ctx);

    // Verifies and consume '+', '-'
    while (ctx.getCurrentToken().type == TokenType::ARITHMETIC_OPERATOR &&
           (ctx.getCurrentToken().value == "+" || ctx.getCurrentToken().value == "-")) {
        ExprOperator opEnum = ExprOperatorUtils::fromString(ctx.getCurrentToken().value);
        ctx.consumeToken(); // Consume the operator
        auto right = parseTerm(ctx); // Parse the next term to the right
        left = std::make_shared<ExprNode>(std::move(left), std::move(right), opEnum, ctx.getCurrentLine());
    }

    // No operator returns default
    return left;
}

std::shared_ptr<RelFactorNode> ExprParser::parseTerm(ParserContext& ctx) {
    // Defaults to parsing LHS
    auto left = parseFactor(ctx);

    // Verifies and consume '*', '/', '%'
    while (ctx.getCurrentToken().type == TokenType::ARITHMETIC_OPERATOR &&
           (ctx.getCurrentToken().value == "*" || ctx.getCurrentToken().value == "/" || ctx.getCurrentToken().value == "%")) {
        // Get enum
        ExprOperator opEnum = ExprOperatorUtils::fromString(ctx.getCurrentToken().value);
        ctx.consumeToken(); // Consume the operator
        auto right = parseFactor(ctx); // Parse the next factor to the right
        left = std::make_shared<ExprNode>(std::move(left), std::move(right), opEnum, ctx.getCurrentLine());
    }

    // No operator returns default
    return left;
}

std::shared_ptr<RelFactorNode> ExprParser::parseFactor(ParserContext& ctx) {
    if (ctx.getCurrentToken().type == TokenType::PUNCTUATION && ctx.getCurrentToken().value == "(") {
        // '(' verified, now consume '('
        ctx.consumeToken();

        // Parsing Expr
        auto expr = parse(ctx);

        // Check Expr Received
        if (!expr) {
            throw SimpleSyntaxError("Expression was not returned from Factor Parser");
        }

        // Verify and consume ')'
        if (!ctx.expectToken(TokenType::PUNCTUATION, ")")) {
            throw SimpleSyntaxError("Expected ')' after expression");
        }

        return expr;
    } else if (ctx.getCurrentToken().type == TokenType::IDENTIFIER || ctx.getCurrentToken().type == TokenType::KEYWORD) {
        // Parsing Variable
        ctx.giveKey(RegistryKey::IDENTIFIER);
        auto varParser = ParserFactory::createParser<IdentifierParser>(ctx);
        auto var = varParser->parse(ctx);

        // Check Variable Received
        if (!var) {
            throw SimpleSyntaxError("Variable Node was not returned from Factor Parser");
        }

        return var;
    } else if (ctx.getCurrentToken().type == TokenType::CONSTANT) {
        // Parsing Constant
        ctx.giveKey(RegistryKey::CONSTANT);
        auto constParser = ParserFactory::createParser<ConstantParser>(ctx);
        auto constant = constParser->parse(ctx);

        // Check Constant Received
        if (!constant) {
            throw SimpleSyntaxError("Constant Node was not returned from Factor Parser");
        }

        return constant;
    } else {
        throw SimpleSyntaxError("Unexpected token parsed as factor");
    }
}