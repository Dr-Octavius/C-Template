#ifndef SPA_STMTLISTPARSER_H
#define SPA_STMTLISTPARSER_H

#pragma once
#include "sp/parser/factory/ParserContext.h"
#include "sp/parser/factory/ParserFactory.h" // Include your parser factory definition
#include "sp/ast/nodes/ASTNode.h" // Define or include your ASTNode and derived classes here
#include "sp/ast/nodes/controlFlow/StmtListNode.h" // Define or include your ASTNode and derived classes here
#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/controlFlow/CallParser.h"
#include "sp/parser/statements/ReadParser.h"
#include "sp/parser/statements/PrintParser.h"
#include "sp/parser/statements/WhileParser.h"
#include "sp/parser/statements/ifParser/IfParser.h"
#include "sp/parser/statements/AssignmentParser.h"
#include <memory>
#include <iostream>

class StmtListParser : public IParser {
public:
    std::shared_ptr<StmtListNode> parse(ParserContext& ctx);
    std::shared_ptr<ASTNode> chooseStmt(ParserContext& ctx);
};

#endif //SPA_STMTLISTPARSER_H
