#ifndef SPA_ASSIGNMENTPARSER_H
#define SPA_ASSIGNMENTPARSER_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/ast/nodes/ASTNode.h" // Include for Generic Node & Type Safety
#include "sp/ast/nodes/statements/AssignNode.h" // Include for Parent Node
#include "sp/ast/nodes/expressions/variables/VariableNode.h" // Include for Left Child Node
#include "sp/parser/expressions/variables/IdentifierParser.h" // Include for LHS expression
#include "sp/parser/expressions/ExprParser.h" // Include for RHS expression

class AssignmentParser : public IParser {
public:
    std::shared_ptr<AssignNode> parse(ParserContext& ctx);
};

#endif //SPA_ASSIGNMENTPARSER_H
