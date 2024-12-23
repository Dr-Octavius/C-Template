#ifndef SPA_THENPARSER_H
#define SPA_THENPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/parser/factory/ParserFactory.h"

class ThenParser : public IParser {
public:
    std::shared_ptr<StmtListNode> parse(ParserContext& ctx);
};

#endif //SPA_THENPARSER_H
