#ifndef SPA_RELATIONALOPERATORRULE_H
#define SPA_RELATIONALOPERATORRULE_H

#include "Token.h"
#include "IRule.h"

class RelationalOperatorRule : public IRule {
public:
    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};

#endif //SPA_RELATIONALOPERATORRULE_H
