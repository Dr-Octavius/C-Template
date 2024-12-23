//
// Created by Michael on 25/2/24.
//

#ifndef SPA_PROGRAMPARSER_H
#define SPA_PROGRAMPARSER_H

#pragma once
#include "sp/parser/factory/ParserContext.h"
#include "sp/parser/factory/ParserFactory.h" // Include your parser factory definition
#include "sp/ast/nodes/controlFlow/ProgramNode.h"
#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/statements/ProcedureParser.h"

class ProgramParser : public IParser {
public:
    std::shared_ptr<ProgramNode> parse(ParserContext& ctx);
};


#endif //SPA_PROGRAMPARSER_H
