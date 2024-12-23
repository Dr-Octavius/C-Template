#ifndef SPA_CALLPARSER_H
#define SPA_CALLPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/controlFlow/CallNode.h"

class CallParser : public IParser {
public:
    std::shared_ptr<CallNode> parse(ParserContext& ctx);
};

#endif //SPA_CALLPARSER_H
