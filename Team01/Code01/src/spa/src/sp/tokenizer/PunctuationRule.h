#ifndef SPA_PUNCTUATIONRULE_H
#define SPA_PUNCTUATIONRULE_H

#include "Token.h"
#include "IRule.h"

class PunctuationRule : public IRule {
public:
    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};

#endif //SPA_PUNCTUATIONRULE_H
