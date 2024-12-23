#ifndef SPA_PARSERCONTEXT_H
#define SPA_PARSERCONTEXT_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/tokenizer/Token.h"
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include <vector>
#include <cstddef>
#include <memory>
#include <iostream> // For error logging, if needed
#include <unordered_map>
#include <sstream>

// Holds all keys to factory parsers
enum class RegistryKey {
    PROGRAM,
    PROCEDURE,
    CALL,
    READ,
    PRINT,
    WHILE,
    IF,
    THEN,
    ELSE,
    EXPR,
    COND_EXPR,
    REL_EXPR,
    STMT_LIST,
    IDENTIFIER,
    CONSTANT,
    ASSIGNMENT,
    // Add more enum values as needed
};

class ParserContext {
private:
    // Holds the lookup table of the procedures parsed so far
    std::unordered_map<std::string, std::shared_ptr<ProcedureNode>> procedureLookupTable;

    // Holds the key being used to get the Parser from the factoryRegistry
    RegistryKey key;

    // Holds all tokens that are part of the input to be parsed.
    std::vector<Token> tokens;

    // Index to the current token in the `tokens` vector being processed.
    size_t currentToken = 0;

    // Index of the line number of the current token in the `tokens` vector being processed.
    int lineNumber = 1;

    // A special token that represents the end of the stream of tokens,
    // used when the parser goes beyond the available tokens.
    const Token endOfStreamToken = Token(TokenType::PUNCTUATION, "<EOS>");

    // let's the context know if program is just a list of Statments (defaults to false)
    bool programIsStmtList;

public:
    explicit ParserContext(const std::vector<Token>& tokens);
    [[nodiscard]] RegistryKey getCurrentKey() const;
    void giveKey(RegistryKey k);
    [[nodiscard]] const Token& getCurrentToken() const;
    [[nodiscard]] int getCurrentLine() const;
    [[nodiscard]] bool isStmtOnly() const;
    void makeStmtOnly();
    [[nodiscard]] const Token& peekNextToken(size_t peekAhead = 1) const;
    [[nodiscard]] bool checkTokenPattern(const TokenType& pattern) const;
    void consumeToken();
    void consumeLine();
    // Checks if the current token matches the expected type and, optionally, the value.
    // If the token matches, it consumes the token and returns true to indicate success.
    // If the token does not match, it does not consume the token, logs an error message, and returns false.
    template<typename... Args>
    bool expectToken(TokenType type, Args&&... args) {
        if (getCurrentToken().type == type &&
            (sizeof...(args) == 0 ||
             ((getCurrentToken().value == args) || ...))
                ) {
            consumeToken(); // Token matches the expectations, consume it
            return true;
        } else {
            return false;
        }
    }
    void addProcedure(std::shared_ptr<ProcedureNode> proc);
    std::shared_ptr<ProcedureNode> getProcedure(const std::string& procName);
};


#endif //SPA_PARSERCONTEXT_H
