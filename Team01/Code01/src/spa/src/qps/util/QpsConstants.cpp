#include <unordered_map>

#include "qps/util/QpsConstants.h"

// Declaration
const std::string QpsConstants::STMT_KEYWORD = "stmt";
const std::string QpsConstants::READ_KEYWORD = "read";
const std::string QpsConstants::PRINT_KEYWORD = "print";
const std::string QpsConstants::CALL_KEYWORD = "call";
const std::string QpsConstants::WHILE_KEYWORD = "while";
const std::string QpsConstants::IF_KEYWORD = "if";
const std::string QpsConstants::ASSIGN_KEYWORD = "assign";
const std::string QpsConstants::VARIABLE_KEYWORD = "variable";
const std::string QpsConstants::CONSTANT_KEYWORD = "constant";
const std::string QpsConstants::PROCEDURE_KEYWORD = "procedure";

const std::vector<std::string> QpsConstants::DECLARE_KEYWORDS = {
        STMT_KEYWORD,
        READ_KEYWORD,
        PRINT_KEYWORD,
        CALL_KEYWORD,
        WHILE_KEYWORD,
        IF_KEYWORD,
        ASSIGN_KEYWORD,
        VARIABLE_KEYWORD,
        CONSTANT_KEYWORD,
        PROCEDURE_KEYWORD
};

// Select
const std::string QpsConstants::SELECT_KEYWORD = "Select";
const std::string QpsConstants::BOOLEAN_KEYWORD = "BOOLEAN";

// Relationship
const std::string QpsConstants::SUCH_KEYWORD = "such";
const std::string QpsConstants::THAT_KEYWORD = "that";
const std::string QpsConstants::SUCH_THAT_KEYWORD = "such that";

const std::string QpsConstants::FOLLOWS_KEYWORD = "Follows";
const std::string QpsConstants::FOLLOWS_T_KEYWORD = "Follows*";
const std::string QpsConstants::PARENT_KEYWORD = "Parent";
const std::string QpsConstants::PARENT_T_KEYWORD = "Parent*";
const std::string QpsConstants::USES_KEYWORD = "Uses";
const std::string QpsConstants::MODIFIES_KEYWORD = "Modifies";
const std::string QpsConstants::CALLS_KEYWORD = "Calls";
const std::string QpsConstants::CALLS_T_KEYWORD = "Calls*";
const std::string QpsConstants::NEXT_KEYWORD = "Next";
const std::string QpsConstants::NEXT_T_KEYWORD = "Next*";
const std::string QpsConstants::AFFECTS_KEYWORD = "Affects";

const std::vector<std::string> QpsConstants::RELATIONSHIP_KEYWORDS = {
        FOLLOWS_KEYWORD,
        FOLLOWS_T_KEYWORD,
        PARENT_KEYWORD,
        PARENT_T_KEYWORD,
        USES_KEYWORD,
        MODIFIES_KEYWORD,
        CALLS_KEYWORD,
        CALLS_T_KEYWORD,
        NEXT_KEYWORD,
        NEXT_T_KEYWORD
};

// Pattern
const std::string QpsConstants::PATTERN_KEYWORD = "pattern";

// With
const std::string QpsConstants::WITH_KEYWORD = "with";
const std::string QpsConstants::PROC_NAME_KEYWORD = "procName";
const std::string QpsConstants::VAR_NAME_KEYWORD = "varName";
const std::string QpsConstants::VALUE_KEYWORD = "value";
const std::string QpsConstants::STMT_NO_KEYWORD = "stmt#";

// And
const std::string QpsConstants::AND_KEYWORD = "and";
const std::string QpsConstants::NOT_KEYWORD = "not";

// Wildcard
const std::string QpsConstants::WILDCARD_KEYWORD = "_";

// Special characters
const char QpsConstants::L_PARENTHESIS = '(';
const char QpsConstants::R_PARENTHESIS = ')';
const char QpsConstants::L_ANGLE_BRACKET = '<';
const char QpsConstants::R_ANGLE_BRACKET = '>';
const char QpsConstants::DOUBLE_QUOTE = '"';
const char QpsConstants::SEMICOLON = ';';
const char QpsConstants::UNDERSCORE = '_';
const char QpsConstants::COMMA = ',';
const char QpsConstants::PERIOD = '.';
const char QpsConstants::EQUALS = '=';

// Regex
const std::regex QpsConstants::ALPHANUMERIC_REGEX = std::regex("[a-zA-Z][a-zA-Z0-9]*");
const std::regex QpsConstants::NUMERIC_REGEX = std::regex("0|[1-9][0-9]*");
const std::regex QpsConstants::QUOTED_REGEX = std::regex("\".*\"");
const std::regex QpsConstants::UNDERSCORE_QUOTED_REGEX = std::regex("_\".*\"_");
const std::regex QpsConstants::ATTR_REF_REGEX = std::regex("[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#)");

bool QpsConstants::isDeclarationKeyword(const std::string &token) {
    return std::find(DECLARE_KEYWORDS.begin(), DECLARE_KEYWORDS.end(), token) !=
           DECLARE_KEYWORDS.end();
}
