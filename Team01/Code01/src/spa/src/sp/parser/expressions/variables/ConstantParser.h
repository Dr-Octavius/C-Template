#ifndef SPA_CONSTANTPARSER_H
#define SPA_CONSTANTPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"

class ConstantParser : public IParser {
public:
    std::shared_ptr<ConstantNode> parse(ParserContext& ctx) ;
};

#endif //SPA_CONSTANTPARSER_H
