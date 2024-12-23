#ifndef SPA_EXPRUTIL_H
#define SPA_EXPRUTIL_H

#include <memory>
#include <string>
#include <unordered_map>

enum class ExprOperator {
    Plus, Minus, Times, Divided, Modulo, ERROR
};

class ExprOperatorUtils {
public:
    static ExprOperator fromString(const std::string& opStr) {
        static const std::unordered_map<std::string, ExprOperator> opMap = {
                {"+", ExprOperator::Plus},
                {"-", ExprOperator::Minus},
                {"*", ExprOperator::Times},
                {"/", ExprOperator::Divided},
                {"%", ExprOperator::Modulo},
        };

        auto it = opMap.find(opStr);
        if (it != opMap.end()) {
            return it->second;
        }
        return ExprOperator::ERROR;
    }

    static std::string toString(ExprOperator op) {
        static const std::unordered_map<ExprOperator, std::string> opMap = {
                {ExprOperator::Plus, "+"},
                {ExprOperator::Minus, "-"},
                {ExprOperator::Times, "*"},
                {ExprOperator::Divided, "/"},
                {ExprOperator::Modulo, "%"},
        };

        auto it = opMap.find(op);
        if (it != opMap.end()) {
            return it->second;
        }
        return "ERROR";
    }
};


#endif //SPA_EXPRUTIL_H
