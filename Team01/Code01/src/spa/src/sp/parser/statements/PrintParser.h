#ifndef SPA_PRINTPARSER_H
#define SPA_PRINTPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/statements/PrintNode.h"
#include "sp/parser/factory/ParserFactory.h"
#include "sp/parser/expressions/variables/IdentifierParser.h"

class PrintParser : public IParser {
public:
    std::shared_ptr<PrintNode> parse(ParserContext& ctx);
};

#endif //SPA_PRINTPARSER_H
