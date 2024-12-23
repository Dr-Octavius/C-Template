#include "ConstantParser.h"

std::shared_ptr<ConstantNode> ConstantParser::parse(ParserContext& ctx) {
    // should catch an error to see if constant is actually of type constant
    int value = std::stoi(ctx.getCurrentToken().value);
    int lineNum = ctx.getCurrentLine();


    // Always begin each parse method with the assertion of the return type
    static_assert(std::is_base_of<ASTNode, ConstantNode>::value, "<T> returned in ConstantParser must be derived from ASTNode");

    // No consumption done in factory, all operations done in Parser

    // Verify and consume CONSTANT
    if (!ctx.expectToken(TokenType::CONSTANT)) {
        throw SimpleSyntaxError("Expected a constant here");
    }

    // Directly create a node for the constant.
    // This might result in errors when converting the token's value.
    // Can call this a syntax error (Does not match SIMPLE Syntax)
    auto node = std::make_shared<ConstantNode>(value,lineNum);

    return node;
}