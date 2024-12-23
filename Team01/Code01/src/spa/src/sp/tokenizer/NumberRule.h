#ifndef SPA_NUMBERRULE_H
#define SPA_NUMBERRULE_H

#include "Token.h"
#include "IRule.h"
#include "sp/errors/SimpleSyntaxError.h"

class NumberRule : public IRule {
public:
    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};

#endif //SPA_NUMBERRULE_H
