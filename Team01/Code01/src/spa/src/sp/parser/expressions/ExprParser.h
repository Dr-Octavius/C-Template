#ifndef SPA_EXPRPARSER_H
#define SPA_EXPRPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/parser/factory/ParserFactory.h"
#include "sp/parser/expressions/variables/IdentifierParser.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNodeUtil.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"

class ExprParser : public IParser {
public:
    std::shared_ptr<RelFactorNode> parse(ParserContext& context);
    std::shared_ptr<RelFactorNode> parseTerm(ParserContext& context);
    std::shared_ptr<RelFactorNode> parseFactor(ParserContext& context);
};


#endif //SPA_EXPRPARSER_H
