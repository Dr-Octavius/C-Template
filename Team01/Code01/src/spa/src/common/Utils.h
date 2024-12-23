#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <utility>
#include <string>

// ai-gen start(gpt, 1, e)
// prompt: -
struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;  // Use XOR to combine hashes as an example
    }
};
//ai-gen end

enum class EntityType {
    PROGRAM,
    PROCEDURE,
    STMT_LIST,
    STMT,
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
    COND_EXPR,
    REL_EXPR,
    EXPR,
    VARIABLE,
    CONSTANT,
    PROCEDURE_END,
};

enum class AttributeType {
    PROC_NAME,
    VAR_NAME,
    VALUE,
    STMT_NO
};

enum class QueryType {
    PARENT,
    PARENTT,
    FOLLOWS,
    FOLLOWST,
    USES,
    MODIFIES,
    CALLS,
    CALLST,
    NEXT,
    NEXTT,
    AFFECTS,
    ASSIGN_PATTERN,
    IF_PATTERN,
    WHILE_PATTERN
};

inline bool isStatement(EntityType type) {
    return type == EntityType::PROCEDURE
        || type == EntityType::STMT
        || type == EntityType::ASSIGN
        || type == EntityType::CALL
        || type == EntityType::WHILE
        || type == EntityType::IF
        || type == EntityType::PRINT
        || type == EntityType::READ
        || type == EntityType::VARIABLE
        || type == EntityType::CONSTANT;
}

inline std::string entityTypeToString(EntityType type) {
    switch (type) {
    case EntityType::PROCEDURE: return "PROCEDURE";
    case EntityType::STMT: return "STMT";
    case EntityType::READ: return "READ";
    case EntityType::PRINT: return "PRINT";
    case EntityType::ASSIGN: return "ASSIGN";
    case EntityType::CALL: return "CALL";
    case EntityType::WHILE: return "WHILE";
    case EntityType::IF: return "IF";
    case EntityType::VARIABLE: return "VARIABLE";
    case EntityType::CONSTANT: return "CONSTANT";
    default: return "UNKNOWN";
    }
}

#endif // UTILS_H
