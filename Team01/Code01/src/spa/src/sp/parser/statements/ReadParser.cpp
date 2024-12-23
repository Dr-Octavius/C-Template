#include "ReadParser.h"

std::shared_ptr<ReadNode> ReadParser::parse(ParserContext& ctx) {

    int lineNum = ctx.getCurrentLine();

    // Always begin each parse method with the assertion of the return type
    static_assert(std::is_base_of<ASTNode, ReadNode>::value, "<T> returned in ProcedureParser must be derived from ASTNode");

    // The next token should be an identifier (the procedure name)
    if (!ctx.expectToken(TokenType::KEYWORD, "read")) {
        throw SimpleSyntaxError("Expected read name token");
    }

    // Use the factory to get and invoke the parser for the statement list inside the loop
    ctx.giveKey(RegistryKey::IDENTIFIER);
    auto identifierParser = ParserFactory::createParser<IdentifierParser>(ctx);
    auto varNode = identifierParser->parse(ctx);

    // Check Statement List received
    if (!varNode) {
        throw SimpleSyntaxError("Variable Node was not returned from StatmentParser");
    }

    // Expect the closing ';' for the else block
    if (!ctx.expectToken(TokenType::PUNCTUATION, ";")) {
        throw SimpleSyntaxError("Expected '}' at the end of 'else' block");
    }

    // consume the current line when ";" is verified (i.e. increase line)
    ctx.consumeLine();

    // Assuming semicolon checking is done by the caller or elsewhere
    return std::make_shared<ReadNode>(varNode, lineNum);
}