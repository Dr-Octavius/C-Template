#ifndef SPA_CONDTIONALOPERATORRULE_H
#define SPA_CONDTIONALOPERATORRULE_H

#include "Token.h"
#include "IRule.h"

class ConditionalOperatorRule : public IRule {
public:
    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};



#endif //SPA_CONDTIONALOPERATORRULE_H
