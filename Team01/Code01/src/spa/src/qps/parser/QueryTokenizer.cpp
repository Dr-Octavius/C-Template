#include <cctype>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "QueryParser.h"
#include "qps/util/QpsConstants.h"

QueryTokenizer::QueryTokenizer(const std::string &qString) {
    queryStream = std::istringstream(qString);
    lastClause = "";
    tokenizedQuery = std::make_shared<std::vector<std::string>>();
    currTokenIdx = 0;
    tokenizedClauses = std::make_shared<std::vector<std::shared_ptr<std::vector<std::string>>>>();
}

std::shared_ptr<std::vector<std::string>> QueryTokenizer::splitTokens() {
    auto tokens = std::make_shared<std::vector<std::string>>();

    std::string nextToken;
    char nextChar;

    auto pushToken = [&]() {
        if (!nextToken.empty()) {
            tokens->push_back(nextToken);
            nextToken.clear();
        }
    };

    while (queryStream.get(nextChar)) {
        if (std::isspace(nextChar)) {
            pushToken();
        } else if (isSpecialChar(nextChar)) {
            pushToken();
            tokens->emplace_back(1, nextChar);
        } else if (nextChar == QpsConstants::DOUBLE_QUOTE) {
            pushToken();
            nextToken.push_back(nextChar);
            nextToken += readUntilChar(QpsConstants::DOUBLE_QUOTE, true);
            tokens->push_back(nextToken);
            nextToken.clear();
        } else if (nextChar == QpsConstants::UNDERSCORE && queryStream.peek() == QpsConstants::DOUBLE_QUOTE) {
            pushToken();
            nextToken.push_back(nextChar);
            queryStream.get(nextChar);
            nextToken.push_back(nextChar);
            nextToken += readUntilChar(QpsConstants::UNDERSCORE, true);
            tokens->push_back(nextToken);
            nextToken.clear();
        } else {
            nextToken.push_back(nextChar);
        }
    }

    pushToken();

    return tokens;
}

std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>> QueryTokenizer::tokenizeQuery() {
    tokenizedQuery = splitTokens();

    std::string nextToken = getNextToken();
    std::shared_ptr<std::vector<std::string>> nextClause;
    while (!nextToken.empty()) {
        if (QpsConstants::isDeclarationKeyword(nextToken)) {
            nextClause = tokenizeDeclaration(nextToken);
        } else if (nextToken == QpsConstants::SELECT_KEYWORD) {
            nextClause = tokenizeSelect();
        } else if (nextToken == QpsConstants::SUCH_THAT_KEYWORD ||
                (nextToken == QpsConstants::SUCH_KEYWORD && getNextToken() == QpsConstants::THAT_KEYWORD)) {
            nextClause = tokenizeSuchThat();
        } else if (nextToken == QpsConstants::PATTERN_KEYWORD) {
            nextClause = tokenizePattern();
        } else if (nextToken == QpsConstants::WITH_KEYWORD) {
            nextClause = tokenizeWith();
        } else if (nextToken == QpsConstants::AND_KEYWORD) {
            nextToken = lastClause;
            continue;
        } else {
            throw QuerySyntaxError("QueryTokenizer: invalid token: " + nextToken + " at " + sizetToString(currTokenIdx));
        }

        tokenizedClauses->push_back(nextClause);
        nextToken = getNextToken();
    }

    return tokenizedClauses;
}

std::shared_ptr<std::vector<std::string>> QueryTokenizer::tokenizeDeclaration(const std::string &synType) {
    auto result = std::make_shared<std::vector<std::string>>();
    result->push_back(synType);
    result->push_back(getNextToken()); // first synonym

    auto nextToken = peekNextToken();
    while (!equalsStringChar(nextToken, QpsConstants::SEMICOLON)) {
        getNextTokenExpect(QpsConstants::COMMA);
        result->push_back(getNextToken());
        nextToken = peekNextToken();
    }
    getNextTokenExpect(QpsConstants::SEMICOLON);

    return result;
}

std::shared_ptr<std::vector<std::string>> QueryTokenizer::tokenizeSelect() {
    auto result = std::make_shared<std::vector<std::string>>();
    result->push_back(QpsConstants::SELECT_KEYWORD);

    auto nextToken = getNextToken();
    if (!equalsStringChar(nextToken, QpsConstants::L_ANGLE_BRACKET)) {
        result->push_back(nextToken);
        return result;
    }

    nextToken = getNextTokenExpectNot(QpsConstants::R_ANGLE_BRACKET);
    result->push_back(nextToken); // first selected elem

    nextToken = peekNextToken();
    while (!equalsStringChar(nextToken, QpsConstants::R_ANGLE_BRACKET)) {
        getNextTokenExpect(QpsConstants::COMMA);
        result->push_back(getNextToken());
        nextToken = peekNextToken();
    }
    getNextTokenExpect(QpsConstants::R_ANGLE_BRACKET);

    return result;
}

std::shared_ptr<std::vector<std::string>> QueryTokenizer::tokenizeSuchThat() {
    auto result = std::make_shared<std::vector<std::string>>();
    result->push_back(QpsConstants::SUCH_THAT_KEYWORD);

    if (peekNextToken() == QpsConstants::NOT_KEYWORD) {
        result->push_back(getNextToken());
    }

    result->push_back(getNextToken()); // relationship type
    getNextTokenExpect(QpsConstants::L_PARENTHESIS);
    result->push_back(getNextToken()); // arg1
    getNextTokenExpect(QpsConstants::COMMA);
    result->push_back(getNextToken()); // arg2
    getNextTokenExpect(QpsConstants::R_PARENTHESIS);

    lastClause = QpsConstants::SUCH_THAT_KEYWORD;
    return result;
}

std::shared_ptr<std::vector<std::string>> QueryTokenizer::tokenizePattern() {
    auto result = std::make_shared<std::vector<std::string>>();
    result->push_back(QpsConstants::PATTERN_KEYWORD);

    if (peekNextToken() == QpsConstants::NOT_KEYWORD) {
        result->push_back(getNextToken());
    }

    result->push_back(getNextToken()); // pattern synonym
    getNextTokenExpect(QpsConstants::L_PARENTHESIS);
    result->push_back(getNextToken()); // arg1
    getNextTokenExpect(QpsConstants::COMMA);
    result->push_back(getNextToken()); // arg2
    if (equalsStringChar(peekNextToken(), QpsConstants::COMMA)) { // optional arg3
        getNextToken(); // COMMA
        result->push_back(getNextToken()); // arg3
    }
    getNextTokenExpect(QpsConstants::R_PARENTHESIS);

    lastClause = QpsConstants::PATTERN_KEYWORD;
    return result;
}

std::shared_ptr<std::vector<std::string>> QueryTokenizer::tokenizeWith() {
    auto result = std::make_shared<std::vector<std::string>>();
    result->push_back(QpsConstants::WITH_KEYWORD);

    if (peekNextToken() == QpsConstants::NOT_KEYWORD) {
        result->push_back(getNextToken());
    }

    result->push_back(getNextToken()); // lhs
    getNextTokenExpect(QpsConstants::EQUALS);
    result->push_back(getNextToken()); // rhs

    lastClause = QpsConstants::WITH_KEYWORD;
    return result;
}

// Utility methods

// ai-gen start(gpt, 1, e)
// prompt: https://chat.openai.com/share/bb74d0a2-a7d0-41c4-957b-d0779eaa00f7
std::string QueryTokenizer::readUntilChar(char stopChar, bool includeStopChar) {
    std::string output;
    char currentChar;

    while (queryStream.get(currentChar)) {
        if (currentChar == stopChar) {
            return includeStopChar ? output + currentChar : output;
        }
        output += currentChar;
    }

    return "";
}
// ai-gen end

bool QueryTokenizer::isSpecialChar(char c) {
    return c == QpsConstants::L_PARENTHESIS || c == QpsConstants::R_PARENTHESIS || c == QpsConstants::COMMA ||
           c == QpsConstants::L_ANGLE_BRACKET || c == QpsConstants::R_ANGLE_BRACKET || c == QpsConstants::SEMICOLON;
}

bool QueryTokenizer::equalsStringChar(std::string str, char c) {
    return str.size() == 1 && str[0] == c;
}

std::string QueryTokenizer::charToString(char c) {
    std::string s;
    return s.append(1, c);
}

std::string QueryTokenizer::sizetToString(size_t i) {
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

// ai-gen start(copilot, 0, e)
std::string QueryTokenizer::getNextToken() {
    if (currTokenIdx >= tokenizedQuery->size()) {
        return "";
    }

    return tokenizedQuery->at(currTokenIdx++);
}
// ai-gen end

// ai-gen start(copilot, 1, e)
std::string QueryTokenizer::getNextTokenExpect(char expectedChar) {
    auto nextToken = getNextToken();
    if (!equalsStringChar(nextToken, expectedChar)) {
        throw QuerySyntaxError("QueryTokenizer: expected " + charToString(expectedChar) + " at " + sizetToString(currTokenIdx) + " but got " + nextToken);
    }

    return nextToken;
}
// ai-gen end

// ai-gen start(copilot, 0, e)
std::string QueryTokenizer::getNextTokenExpectNot(char unexpectedChar) {
    auto nextToken = getNextToken();
    if (equalsStringChar(nextToken, unexpectedChar)) {
        throw QuerySyntaxError("QueryTokenizer: unexpected " + charToString(unexpectedChar));
    }

    return nextToken;
}
// ai-gen end

// ai-gen start(copilot, 0, e)
std::string QueryTokenizer::peekNextToken() {
    if (currTokenIdx >= tokenizedQuery->size()) {
        return "";
    }

    return tokenizedQuery->at(currTokenIdx);
}
// ai-gen end
