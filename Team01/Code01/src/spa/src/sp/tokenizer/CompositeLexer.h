#ifndef SPA_COMPOSITELEXER_H
#define SPA_COMPOSITELEXER_H

#include "ITokenizer.h"
#include "IRule.h"
#include <vector>
#include <memory>
#include "sp/errors/SimpleSyntaxError.h"

class CompositeLexer : public ITokenizer {
private:
    std::vector<std::unique_ptr<IRule>> rules;
    void addRule(std::unique_ptr<IRule> rule);

public:
    CompositeLexer();
    std::vector<Token> tokenize(const std::string& input) override;
};

#endif //SPA_COMPOSITELEXER_H
