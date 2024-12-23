#ifndef SPA_ITOKENIZER_H
#define SPA_ITOKENIZER_H

#include <string>
#include <vector>
#include "Token.h"

class ITokenizer {
public:
    virtual std::vector<Token> tokenize(const std::string& input) = 0;

    virtual ~ITokenizer() = default;
};

#endif //SPA_ITOKENIZER_H
