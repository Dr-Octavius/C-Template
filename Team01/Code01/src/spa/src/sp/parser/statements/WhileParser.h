#ifndef SPA_WHILEPARSER_H
#define SPA_WHILEPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h" // Make sure this file has your ASTNode definitions
#include "sp/ast/nodes/statements/WhileNode.h" // Make sure this file has your ASTNode definitions
#include "sp/parser/factory/ParserFactory.h" // Include the ParserFactory definition

class WhileParser : public IParser {
public:
    std::shared_ptr<WhileNode> parse(ParserContext& ctx);
};


#endif //SPA_WHILEPARSER_H
