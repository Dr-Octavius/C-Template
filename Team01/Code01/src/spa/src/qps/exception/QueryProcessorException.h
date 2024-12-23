#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class QueryProcessorException : public std::exception {
    std::string errorMessage;
public:
    explicit QueryProcessorException(const std::string& message);

    const char* what() const noexcept override;
};

class QuerySyntaxError : public QueryProcessorException {
public:
    explicit QuerySyntaxError(const std::string& message);
};

class QuerySemanticError : public QueryProcessorException {
public:
    explicit QuerySemanticError(const std::string& message);
};

class QueryUnknownError : public QueryProcessorException {
public:
    explicit QueryUnknownError(const std::string& message);
};
