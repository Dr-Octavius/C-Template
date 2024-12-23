#include "QueryProcessorException.h"

QueryProcessorException::QueryProcessorException(const std::string &message) : errorMessage(message) {

}

const char *QueryProcessorException::what() const noexcept {
    return errorMessage.c_str();
}


QuerySyntaxError::QuerySyntaxError(const std::string &message) : QueryProcessorException(message) {

}

QuerySemanticError::QuerySemanticError(const std::string &message) : QueryProcessorException(message) {

}

QueryUnknownError::QueryUnknownError(const std::string &message) : QueryProcessorException(message) {

}
