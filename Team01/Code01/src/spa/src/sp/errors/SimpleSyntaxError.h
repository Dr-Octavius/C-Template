//
// Created by Michael on 24/2/24.
//

#ifndef SPA_SIMPLESYNTAXERROR_H
#define SPA_SIMPLESYNTAXERROR_H

#include <iostream>
#include <string>

class SimpleSyntaxError : public std::exception {
private:
    std::string errorMessage;
public:
    explicit SimpleSyntaxError(const std::string& message);
    [[nodiscard]] const std::string& getMessage() const;
};

#endif //SPA_SIMPLESYNTAXERROR_H
