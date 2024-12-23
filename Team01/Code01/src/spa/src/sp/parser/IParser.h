#ifndef INC_23S2_CP_SPA_TEAM_01_PARSER_H
#define INC_23S2_CP_SPA_TEAM_01_PARSER_H

// IParser.h
#include "sp/ast/nodes/ASTNode.h"
#include "sp/tokenizer/Token.h"
#include "sp/parser/factory/ParserContext.h"
#include <vector>

class IParser {
public:
    virtual ~IParser() = default;
};

#endif //INC_23S2_CP_SPA_TEAM_01_PARSER_H