#include "StmtListParser.h"

std::shared_ptr<StmtListNode> StmtListParser::parse(ParserContext& ctx) {

    std::shared_ptr<StmtListNode> stmtListNode = std::make_shared<StmtListNode>(ctx.getCurrentLine()); // Assumes a node that holds a list of statements

    while (ctx.getCurrentToken().value != "<EOS>") {
          if (ctx.getCurrentToken().value == "}") {
            break;
          }

        stmtListNode->addStatement(std::move(chooseStmt(ctx)));

        // No need to manually consume semicolons or other terminators here, assuming the parsers do so.
    }

    // If reached EOF without finding a '}', it could indicate a syntax error in the input.
    if (ctx.getCurrentToken().value == "<EOS>" && !ctx.isStmtOnly()) {
        throw SimpleSyntaxError("Unexpected end of file; '}' was expected.");
    }

    return stmtListNode;
}

std::shared_ptr<ASTNode> StmtListParser::chooseStmt(ParserContext& ctx) {

    std::string val = ctx.getCurrentToken().value;

    bool isAlphanumeric = true;
    for (char c : val) {
        if (!std::isalnum(static_cast<unsigned char>(c))) {
            isAlphanumeric = false;
            break;
        }
    }

    if (!isAlphanumeric) {
        throw SimpleSyntaxError("Invalid character error");
    }

    if (ctx.peekNextToken().type == TokenType::ASSIGNMENT_OPERATOR) {
        // Assignment otherwise
        ctx.giveKey((RegistryKey::ASSIGNMENT));
        auto parser = ParserFactory::createParser<AssignmentParser>(ctx);
        auto node = parser->parse(ctx);
        return node;
    }

    return ParserFactory::stmtListWorker(ctx);
}

//std::shared_ptr<ASTNode> StmtListParser::chooseStmt(ParserContext& ctx) {
//
//    std::string val = ctx.getCurrentToken().value;
//
//    bool isAlphanumeric = true;
//    for (char c : val) {
//        if (!std::isalnum(static_cast<unsigned char>(c))) {
//            isAlphanumeric = false;
//            break;
//        }
//    }
//
//    if (!isAlphanumeric) {
//        throw SimpleSyntaxError("Invalid character error");
//    }
//
//    if (ctx.peekNextToken().type == TokenType::ASSIGNMENT_OPERATOR) {
//        // Assignment otherwise
//        ctx.giveKey((RegistryKey::ASSIGNMENT));
//        auto parser = ParserFactory::createParser<AssignmentParser>(ctx);
//        auto node = parser->parse(ctx);
//        return node;
//    }
//
//    if (val == "read") {
//        ctx.giveKey((RegistryKey::READ));
//        auto parser = ParserFactory::createParser<ReadParser>(ctx);
//        auto node = parser->parse(ctx);
//        return node;
//    } else if (val == "print") {
//        ctx.giveKey((RegistryKey::PRINT));
//        auto parser = ParserFactory::createParser<PrintParser>(ctx);
//        auto node = parser->parse(ctx);
//        return node;
//    } else if (val == "call") {
//        ctx.giveKey((RegistryKey::CALL));
//        auto parser = ParserFactory::createParser<CallParser>(ctx);
//        auto node = parser->parse(ctx);
//        return node;
//    } else if (val == "while") {
//        ctx.giveKey((RegistryKey::WHILE));
//        auto parser = ParserFactory::createParser<WhileParser>(ctx);
//        auto node = parser->parse(ctx);
//        return node;
//    } else if (val == "if") {
//        ctx.giveKey((RegistryKey::IF));
//        auto parser = ParserFactory::createParser<IfParser>(ctx);
//        auto node = parser->parse(ctx);
//        return node;
//    }
//
//}