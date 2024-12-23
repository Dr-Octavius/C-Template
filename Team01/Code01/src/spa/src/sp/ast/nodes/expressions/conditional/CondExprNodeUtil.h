#ifndef SPA_CONDEXPRUTIL_H
#define SPA_CONDEXPRUTIL_H

#include <memory>
#include <string>
#include <unordered_map>

enum class CondExprOperator {
    And, Or, Not, ERROR
};

class CondExprOperatorUtils {
public:
    static CondExprOperator fromString(const std::string& opStr) {
        static const std::unordered_map<std::string, CondExprOperator> opMap = {
                {"!", CondExprOperator::Not},
                {"&&", CondExprOperator::And},
                {"||", CondExprOperator::Or},
        };

        auto it = opMap.find(opStr);
        if (it != opMap.end()) {
            return it->second;
        }
        return CondExprOperator::ERROR;
    }
};


#endif //SPA_CONDEXPRUTIL_H
