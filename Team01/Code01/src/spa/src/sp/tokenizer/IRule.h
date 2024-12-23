#ifndef SPA_IRULE_H
#define SPA_IRULE_H

#include <string>
#include <vector>
#include "Token.h"

class IRule {
public:
    virtual bool match(const std::string& input, size_t position, std::vector<Token>& tokens) = 0;
    virtual ~IRule() = default;
    virtual Token createToken(const std::string& value) = 0;
};

#endif //SPA_IRULE_H
