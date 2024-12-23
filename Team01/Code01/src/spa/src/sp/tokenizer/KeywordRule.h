#ifndef SPA_KEYWORDRULE_H
#define SPA_KEYWORDRULE_H

#include <unordered_set>
#include "Token.h"
#include "IRule.h"

class KeywordRule : public IRule {
private:
    std::unordered_set<std::string> keywords;
public:
    KeywordRule();

    bool match(const std::string& input, size_t position, std::vector<Token>& tokens) override;

    Token createToken(const std::string& value) override;
};

#endif //SPA_KEYWORDRULE_H
