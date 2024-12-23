#include "ProcedureParser.h"

std::shared_ptr<ProcedureNode> ProcedureParser::parse(ParserContext& ctx) {
    // Always begin each parse method with the assertion of the return type
    static_assert(std::is_base_of<ASTNode, ProcedureNode>::value, "<T> returned in ProcedureParser must be derived from ASTNode");

    // The next token should be a KEYWORD of type procedure
    if (!ctx.expectToken(TokenType::KEYWORD, "procedure")) {
        throw SimpleSyntaxError("Expected procedure keyword as entry point");
    }

    std::string procName = ctx.getCurrentToken().value;
    int lineNum = 0;

    // The next token should be an identifier (procedure name)
    if (ctx.getCurrentToken().type != TokenType::KEYWORD) {
        if (ctx.getCurrentToken().type != TokenType::IDENTIFIER) {
            throw SimpleSyntaxError("Expected procedure name as entry point");
        } else {
            ctx.consumeToken();
        }
    } else {
        ctx.consumeToken();
    }

    // Expect the opening '{' for the procedure body
    if (!ctx.expectToken(TokenType::PUNCTUATION, "{")) {
        throw SimpleSyntaxError("Expected '{' to start the procedure body");
    }

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

    // DO NOT consume the current line when "}" is verified (i.e. increase line)

    return std::make_shared<ProcedureNode>(procName, std::move(stmtList), lineNum);
}