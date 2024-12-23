#include "ParserContext.h"

// Constructor that initializes the `ParserContext` with a vector of tokens
// Sets default Key to STMT_LIST.
ParserContext::ParserContext(const std::vector<Token>& tokens) :
        tokens(tokens), key(RegistryKey::STMT_LIST), programIsStmtList(false) {
    // push the 'endOfStreamToken' such that every token vector
    // ends with the endOfStreamToken
    this->tokens.push_back(endOfStreamToken);
}

// Retrieves the current key for the factory method
RegistryKey ParserContext::getCurrentKey() const {
    // Return the current registry key
    return key;
}

// Sets (Gives) the current key to the context
void ParserContext::giveKey(RegistryKey k) {
    // Sets the registry key to the one it was provided
    key = k;
}

// Retrieves the current token being processed. If the current token index
// is beyond the available tokens, it returns an end-of-stream token.
const Token& ParserContext::getCurrentToken() const {
    return tokens[currentToken];
}

// Retrieves the value of the current line number.
int ParserContext::getCurrentLine() const {
    return lineNumber;
}

// Makes it known that the program is just a list of statements
bool ParserContext::isStmtOnly() const {
    return programIsStmtList;
}

// Makes it known that the program is just a list of statements
void ParserContext::makeStmtOnly() {
    programIsStmtList = true;
}

// Retrieves the next token being processed. If the current token index
// is beyond the available tokens, it returns an end-of-stream token.
const Token& ParserContext::peekNextToken(size_t peekAhead) const {
    // Check if the next token is within the bounds of the tokens vector
    size_t targetIndex = currentToken + peekAhead;
    if (targetIndex < tokens.size()) {
        return tokens[targetIndex];
    }
    // If the next index is out of bounds, return the end-of-stream token
    return endOfStreamToken;
}

bool ParserContext::checkTokenPattern(const TokenType& pattern) const {
    int openBracket = 0;
    int closeBracket = 0;
    for (size_t i = currentToken; i < tokens.size(); i++) {
        const Token& token = tokens[i];

        if (token.type == TokenType::PUNCTUATION &&
            token.value == "(") {
            openBracket++;
        }

        if (token.type == TokenType::PUNCTUATION &&
            token.value == ")") {
            closeBracket++;
            if (closeBracket > openBracket) {
                break;
            }
        }

        if (token.type == pattern) {
            return true;
        }
    }
    return false;
}

// Advances to the next token in the stream, if available.
// Prints a warning if trying to consume beyond the available tokens.
void ParserContext::consumeToken() {
    if (currentToken < tokens.size()) {
        currentToken++;
    } else {
        // Optionally handle trying to consume past the end of stream
        std::cerr << "Warning: Attempted to consume token past the end of the stream." << std::endl;
    }
}

// Advances to the next token in the stream, if available.
// Prints a warning if trying to consume beyond the available tokens.
void ParserContext::consumeLine() {
    lineNumber++;
}

void ParserContext::addProcedure(std::shared_ptr<ProcedureNode> proc) {
    procedureLookupTable[proc->getName()] = proc;
}

std::shared_ptr<ProcedureNode> ParserContext::getProcedure(const std::string& procName) {
    return procedureLookupTable[procName];
}
