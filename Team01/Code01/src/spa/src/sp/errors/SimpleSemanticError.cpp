//
// Created by Michael on 24/3/24.
//

#include "SimpleSemanticError.h"

SimpleSemanticError::SimpleSemanticError(const std::string &message) : errorMessage(message) {}

const std::string& SimpleSemanticError::getMessage() const {
    return errorMessage;
}