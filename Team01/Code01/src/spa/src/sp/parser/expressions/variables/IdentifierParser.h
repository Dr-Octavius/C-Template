#ifndef SPA_IDENTIFIERPARSER_H
#define SPA_IDENTIFIERPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"

class IdentifierParser : public IParser {
public:
    std::shared_ptr<VariableNode> parse(ParserContext& ctx);
};

#endif //SPA_IDENTIFIERPARSER_H
