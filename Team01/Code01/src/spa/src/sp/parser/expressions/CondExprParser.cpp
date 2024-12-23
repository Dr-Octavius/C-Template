#include "CondExprParser.h"

std::shared_ptr<CondExprNode> CondExprParser::parse(ParserContext& ctx) {
    if (ctx.getCurrentToken().type == TokenType::IDENTIFIER ||
    ctx.getCurrentToken().type == TokenType::KEYWORD ||
    ctx.getCurrentToken().type == TokenType::CONSTANT ||
    !ctx.checkTokenPattern(TokenType::CONDITIONAL_OPERATOR)) {
        // Pattern contains '||' or '&&'
        // Proceed with parsing a conditional expression involving '||' or '&&'
        auto relExpr = parseRelExpr(ctx);

        // Check RelExpr Received
        if (!relExpr) {
            throw SimpleSyntaxError("Relational Expression was not returned from Conditional Expression Parser");
        }

        return relExpr;
    } else if (ctx.getCurrentToken().type == TokenType::PUNCTUATION) {
        // Parsing "&&" or '||' Conditional Expression
        auto andOrCond = parseAndOrCond(ctx);

        // Check NotNode Received
        if (!andOrCond) {
            throw SimpleSyntaxError("AndOr Conditional was not returned from Conditional Expression Parser");
        }

        return andOrCond;
    } else if (ctx.getCurrentToken().value == "!") {
        // Parsing "!" Conditional Expression
        auto notCond = parseNotCond(ctx);

        // Check NotNode Received
        if (!notCond) {
            throw SimpleSyntaxError("Not Conditional was not returned from Conditional Expression Parser");
        }

        return notCond;
    } else {
        throw SimpleSyntaxError("Unexpected token in conditional expression");
    }
}

std::shared_ptr<RelExprNode> CondExprParser::parseRelExpr(ParserContext& ctx) {
    // Parsing RelExpr
    ctx.giveKey(RegistryKey::REL_EXPR);
    auto relExprParser = ParserFactory::createParser<RelExprParser>(ctx);
    return relExprParser->parse(ctx);
}

std::shared_ptr<CondExprNode> CondExprParser::parseAndOrCond(ParserContext& ctx) {
    // Verify and consume '('
    if (!ctx.expectToken(TokenType::PUNCTUATION, "(")) {
        throw SimpleSyntaxError("Expected '(' at start of AndOr LHS expression");
    }

    // Defaults to parsing LHS
    auto left = parse(ctx);

    // Verify and consume ')'
    if (!ctx.expectToken(TokenType::PUNCTUATION, ")")) {
        throw SimpleSyntaxError("Expected ')' at end of AndOr LHS expression");
    }

    // Get enum
    CondExprOperator opEnum = CondExprOperatorUtils::fromString(ctx.getCurrentToken().value);

    // Verify and consume '&&' or '||'
    if (!ctx.expectToken(TokenType::CONDITIONAL_OPERATOR, "&&","||")) {
        throw SimpleSyntaxError("'&&' or '||' was expected from Not Parser");
    }
    // Verify and consume '('
    if (!ctx.expectToken(TokenType::PUNCTUATION, "(")) {
        throw SimpleSyntaxError("Expected '(' at start of AndOr LHS expression");
    }

    // Parsing RHS if '&&', '||' present and Syntax Verified
    auto right = parse(ctx);

    // Verify and consume ')'
    if (!ctx.expectToken(TokenType::PUNCTUATION, ")")) {
        throw SimpleSyntaxError("Expected ')' at end of AndOr LHS expression");
    }

    return std::make_shared<CondBinaryExprNode>(std::move(left), std::move(right), opEnum, ctx.getCurrentLine());
}

std::shared_ptr<CondUnaryExprNode> CondExprParser::parseNotCond(ParserContext& ctx) {
    // Get enum
    CondExprOperator opEnum = CondExprOperatorUtils::fromString(ctx.getCurrentToken().value);

    // Verify and consume '!'
    if (!ctx.expectToken(TokenType::CONDITIONAL_OPERATOR, "!")) {
        throw SimpleSyntaxError("'!' was expected from Not Parser");
    }

    // Verify and consume '('
    if (!ctx.expectToken(TokenType::PUNCTUATION, "(")) {
        throw SimpleSyntaxError("Expected '(' at start of NOT operand");
    }

    // Parsing the operand
    auto expr = parse(ctx);

    // Verify and consume ')'
    if (!ctx.expectToken(TokenType::PUNCTUATION, ")")) {
        throw SimpleSyntaxError("Expected ')' at start of AndOr LHS expression");
    }

    return std::make_shared<CondUnaryExprNode>(std::move(expr), opEnum, ctx.getCurrentLine());
}