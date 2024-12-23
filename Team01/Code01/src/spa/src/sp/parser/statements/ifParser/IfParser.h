#ifndef SPA_IFPARSER_H
#define SPA_IFPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/parser/factory/ParserFactory.h"
#include "sp/ast/nodes/ASTNode.h" // Define or include your ASTNode definitions here
#include "sp/ast/nodes/statements/IfNode.h" // Make sure this file has your ASTNode definitions
#include "sp/ast/nodes/controlFlow/StmtListNode.h" // Make sure this file has your ASTNode definitions

class IfParser : public IParser {
public:
    std::shared_ptr<IfNode> parse(ParserContext& ctx);
};

#endif //SPA_IFPARSER_H
