#ifndef SPA_RELEXPRUTIL_H
#define SPA_RELEXPRUTIL_H


#include <memory>
#include <string>
#include <unordered_map>

enum class RelExprOperator {
    Greater, GreaterEqual, Less, LessEqual, Equal, NotEqual, ERROR
};

class RelExprOperatorUtils {
public:
    static RelExprOperator fromString(const std::string& opStr) {
        static const std::unordered_map<std::string, RelExprOperator> opMap = {
                {">", RelExprOperator::Greater},
                {">=", RelExprOperator::GreaterEqual},
                {"<", RelExprOperator::Less},
                {"<=", RelExprOperator::LessEqual},
                {"==", RelExprOperator::Equal},
                {"!=", RelExprOperator::NotEqual},
        };

        auto it = opMap.find(opStr);
        if (it != opMap.end()) {
            return it->second;
        }
        return RelExprOperator::ERROR;
    }
};


#endif //SPA_RELEXPRUTIL_H
