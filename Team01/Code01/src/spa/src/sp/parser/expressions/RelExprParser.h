#ifndef SPA_RELEXPRPARSER_H
#define SPA_RELEXPRPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h" // Include your ASTNode definitions
#include "sp/ast/nodes/expressions/relational/RelExprNode.h" // Include your ASTNode definitions
#include "sp/parser/expressions/ExprParser.h" // Include your ASTNode definitions

class RelExprParser : public IParser {
public:
    std::shared_ptr<RelExprNode> parse(ParserContext& context);
    std::shared_ptr<RelFactorNode> parseRelFactor(ParserContext& context) ;
};

#endif //SPA_RELEXPRPARSER_H
