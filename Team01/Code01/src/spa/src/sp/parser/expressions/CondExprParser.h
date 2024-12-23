#ifndef SPA_CONDEXPRPARSER_H
#define SPA_CONDEXPRPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h" // Make sure this file includes definitions for nodes related to conditional expressions
#include "sp/ast/nodes/expressions/CondExprNode.h" // Make sure this file includes definitions for nodes related to conditional expressions
#include "sp/ast/nodes/expressions/conditional/CondBinaryExprNode.h" // Make sure this file includes definitions for nodes related to conditional expressions
#include "sp/ast/nodes/expressions/conditional/CondUnaryExprNode.h" // Make sure this file includes definitions for nodes related to conditional expressions
#include "sp/ast/nodes/expressions/conditional/CondExprNodeUtil.h" // Make sure this file includes definitions for nodes related to conditional expressions
#include "sp/parser/factory/ParserFactory.h"
#include "sp/parser/expressions/RelExprParser.h" // For relational expressions within conditions

class CondExprParser : public IParser {
public:
    std::shared_ptr<CondExprNode> parse(ParserContext& context);
    std::shared_ptr<RelExprNode> parseRelExpr(ParserContext& context);
    std::shared_ptr<CondExprNode> parseAndOrCond(ParserContext& context);
    std::shared_ptr<CondUnaryExprNode> parseNotCond(ParserContext& context);
};

#endif //SPA_CONDEXPRPARSER_H
