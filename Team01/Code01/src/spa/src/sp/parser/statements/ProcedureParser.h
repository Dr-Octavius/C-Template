#ifndef SPA_PROCEDUREPARSER_H
#define SPA_PROCEDUREPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/statements/ProcedureNode.h" // Include ASTNode definitions
#include "sp/parser/factory/ParserFactory.h" // Include the ParserFactory definition
#include "sp/parser/controlFlow/StmtListParser.h"

class ProcedureParser : public IParser {
public:
    std::shared_ptr<ProcedureNode> parse(ParserContext& ctx);
};

#endif //SPA_PROCEDUREPARSER_H
