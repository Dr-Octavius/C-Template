#ifndef SPA_ASSIGNMENTOPERATORRULE_H
#define SPA_ASSIGNMENTOPERATORRULE_H

#include "Token.h"
#include "IRule.h"

class AssignmentOperatorRule : public IRule {
public:
    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};

#endif //SPA_ASSIGNMENTOPERATORRULE_H
