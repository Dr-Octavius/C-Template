//
// Created by Michael on 24/3/24.
//

#ifndef SPA_SIMPLESEMANTICERROR_H
#define SPA_SIMPLESEMANTICERROR_H

#include <iostream>
#include <string>

class SimpleSemanticError : public std::exception {
private:
    std::string errorMessage;
public:
    explicit SimpleSemanticError(const std::string& message);
    [[nodiscard]] const std::string& getMessage() const;
};


#endif //SPA_SIMPLESEMANTICERROR_H
