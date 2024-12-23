#ifndef SPA_ELSEPARSER_H
#define SPA_ELSEPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/parser/factory/ParserFactory.h"

class ElseParser : public IParser {
public:
    std::shared_ptr<StmtListNode> parse(ParserContext& ctx);
};

#endif //SPA_ELSEPARSER_H
