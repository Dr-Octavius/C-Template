#include "CompositeLexer.h"
#include "NumberRule.h"
#include "IdentifierRule.h"
#include "ArithmeticOperatorRule.h"
#include "KeywordRule.h"
#include "RelationalOperatorRule.h"
#include "AssignmentOperatorRule.h"
#include "PunctuationRule.h"
#include "ConditionalOperatorRule.h"
#include <iostream>

void CompositeLexer::addRule(std::unique_ptr<IRule> rule) {
    rules.push_back(std::move(rule));
}

CompositeLexer::CompositeLexer() {
    addRule(std::make_unique<KeywordRule>());
    addRule(std::make_unique<NumberRule>());
    addRule(std::make_unique<IdentifierRule>());
    addRule(std::make_unique<RelationalOperatorRule>());
    addRule(std::make_unique<ConditionalOperatorRule>());
    addRule(std::make_unique<ArithmeticOperatorRule>());
    addRule(std::make_unique<AssignmentOperatorRule>());
    addRule(std::make_unique<PunctuationRule>());
    // ... other rules
}

std::vector<Token> CompositeLexer::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t position = 0;

    while (isspace(input[position])) {
        position++;
    }

    while (position < input.size()) {
        bool matched = false;

        for (const std::unique_ptr<IRule>& rule : rules) {
            if (rule->match(input, position, tokens)) {
                matched = true;
                break;
            }
        }

        if (!matched) {
            std::cout << "syntax error" << std::endl;
            throw SimpleSyntaxError("Syntax error at position: " + std::to_string(position));
        } else {
            position = tokens.back().value.size() + position;
            while (position < input.size() && isspace(input[position])) {
                position++;
            }
        }
    }

    return tokens;
}
