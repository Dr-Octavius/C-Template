#ifndef SPA_IDENTIFIERRULE_H
#define SPA_IDENTIFIERRULE_H

#include "Token.h"
#include "IRule.h"

class IdentifierRule : public IRule {
public:
    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};

#endif //SPA_IDENTIFIERRULE_H
