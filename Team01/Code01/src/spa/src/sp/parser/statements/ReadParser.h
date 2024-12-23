#ifndef SPA_READPARSER_H
#define SPA_READPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/parser/expressions/variables/IdentifierParser.h"
#include "sp/ast/nodes/statements/ReadNode.h"
#include "sp/parser/factory/ParserFactory.h"

class ReadParser : public IParser {
public:
    std::shared_ptr<ReadNode> parse(ParserContext& context);
};

#endif //SPA_READPARSER_H
