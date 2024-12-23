#include "IdentifierParser.h"

std::shared_ptr<VariableNode> IdentifierParser::parse(ParserContext& ctx) {

    std::string name = ctx.getCurrentToken().value;
    int lineNum = ctx.getCurrentLine();

    // Always begin each parse method with the assertion of the return type
    static_assert(std::is_base_of<ASTNode, VariableNode>::value, "<T> returned in IdentifierParser must be derived from ASTNode");

    // No consumption done in factory, all operations done in Parser

    // Verify and consume IDENTIFIER
    if (ctx.getCurrentToken().type != TokenType::KEYWORD) {
        if (ctx.getCurrentToken().type != TokenType::IDENTIFIER) {
            throw SimpleSyntaxError("Expected an identifier here");
        } else {
            ctx.consumeToken();
        }
    } else {
        ctx.consumeToken();
    }

    // Directly create a node for the variable.
    // This might result in errors when converting the token's value.
    // Can call this a syntax error (Does not match SIMPLE Syntax)
    auto node = std::make_shared<VariableNode>(name,lineNum);

    return node;
}
