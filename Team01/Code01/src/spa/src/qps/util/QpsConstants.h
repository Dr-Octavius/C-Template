#pragma once

#include "qps/parser/QueryParser.h"

class QpsConstants {
public:
    // Declaration
    static const std::string STMT_KEYWORD;
    static const std::string READ_KEYWORD;
    static const std::string PRINT_KEYWORD;
    static const std::string CALL_KEYWORD;
    static const std::string WHILE_KEYWORD;
    static const std::string IF_KEYWORD;
    static const std::string ASSIGN_KEYWORD;
    static const std::string VARIABLE_KEYWORD;
    static const std::string CONSTANT_KEYWORD;
    static const std::string PROCEDURE_KEYWORD;
    static const std::vector<std::string> DECLARE_KEYWORDS;

    // Select
    static const std::string SELECT_KEYWORD;
    static const std::string BOOLEAN_KEYWORD;

    // Relationship
    static const std::string SUCH_KEYWORD;
    static const std::string THAT_KEYWORD;
    static const std::string SUCH_THAT_KEYWORD;

    static const std::string FOLLOWS_KEYWORD;
    static const std::string FOLLOWS_T_KEYWORD;
    static const std::string PARENT_KEYWORD;
    static const std::string PARENT_T_KEYWORD;
    static const std::string USES_KEYWORD;
    static const std::string MODIFIES_KEYWORD;
    static const std::string CALLS_KEYWORD;
    static const std::string CALLS_T_KEYWORD;
    static const std::string NEXT_KEYWORD;
    static const std::string NEXT_T_KEYWORD;
    static const std::string AFFECTS_KEYWORD;
    static const std::vector<std::string> RELATIONSHIP_KEYWORDS;

    // Pattern
    static const std::string PATTERN_KEYWORD;

    // With
    static const std::string WITH_KEYWORD;
    static const std::string PROC_NAME_KEYWORD;
    static const std::string VAR_NAME_KEYWORD;
    static const std::string VALUE_KEYWORD;
    static const std::string STMT_NO_KEYWORD;

    // And
    static const std::string AND_KEYWORD;
    static const std::string NOT_KEYWORD;

    // Wildcard
    static const std::string WILDCARD_KEYWORD;

    // Special characters
    static const char L_PARENTHESIS;
    static const char R_PARENTHESIS;
    static const char L_ANGLE_BRACKET;
    static const char R_ANGLE_BRACKET;
    static const char DOUBLE_QUOTE;
    static const char SEMICOLON;
    static const char UNDERSCORE;
    static const char COMMA;
    static const char PERIOD;
    static const char EQUALS;

    // Regex
    static const std::regex ALPHANUMERIC_REGEX;
    static const std::regex NUMERIC_REGEX;
    static const std::regex QUOTED_REGEX;
    static const std::regex UNDERSCORE_QUOTED_REGEX;
    static const std::regex EXPRESSION_REGEX;
    static const std::regex ATTR_REF_REGEX;

    static bool isDeclarationKeyword(const std::string& token);
};
