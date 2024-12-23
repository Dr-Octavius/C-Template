#ifndef INC_23S2_CP_SPA_TEAM_01_MAINPARSER_H
#define INC_23S2_CP_SPA_TEAM_01_MAINPARSER_H

#include "sp/parser/factory/ParserFactory.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/tokenizer/Token.h"

class MainParser {
private:
    ParserFactory factory;
    ParserContext ctx;

public:
    explicit MainParser(std::vector<Token>& tokens);
    std::shared_ptr<ProgramNode> parse();
};

#endif //INC_23S2_CP_SPA_TEAM_01_MAINPARSER_H
