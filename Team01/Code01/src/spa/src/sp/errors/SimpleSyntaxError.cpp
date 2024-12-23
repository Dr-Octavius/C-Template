//
// Created by Michael on 24/2/24.
//

#include "SimpleSyntaxError.h"

SimpleSyntaxError::SimpleSyntaxError(const std::string& message) : errorMessage(message) {}

const std::string& SimpleSyntaxError::getMessage() const {
    return errorMessage;
}
