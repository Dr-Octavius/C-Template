#ifndef SPA_ARITHMETICOPERATORRULE_H
#define SPA_ARITHMETICOPERATORRULE_H

#include "Token.h"
#include "IRule.h"

class ArithmeticOperatorRule : public IRule {
public:
    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};

#endif //SPA_ARITHMETICOPERATORRULE_H
