//
// Created by Michael on 25/2/24.
//

#include "ProgramParser.h"

std::shared_ptr<ProgramNode> ProgramParser::parse(ParserContext& ctx) {

    std::shared_ptr<ProgramNode> programNode = std::make_shared<ProgramNode>(ctx.getCurrentLine()); // Assumes a node that holds a list of statements

    // Checks if current node is a procedure
    if (!(ctx.getCurrentToken().value == "procedure")) {
        throw SimpleSyntaxError("Unexpected start to Program; 'procedure' was expected.");
    }


    while (ctx.getCurrentToken().value != "<EOS>") {

        ctx.giveKey((RegistryKey::PROCEDURE));
        auto parser = ParserFactory::createParser<ProcedureParser>(ctx);
        auto procedureNode = parser->parse(ctx);

        programNode->addProcedure(std::move(procedureNode));

        // No need to manually consume semicolons or other terminators here, assuming the parsers do so.
    }

    return programNode;
}