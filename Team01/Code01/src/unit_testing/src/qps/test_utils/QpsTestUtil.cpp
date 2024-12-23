#include <iostream>

#include "QpsTestUtil.h"

// ai-gen start(copilot, 1, e)
bool compareTokenizedClauses(const std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>> > &actual,
                             const std::vector<std::vector<std::string>> &expected) {
    if (actual->size() != expected.size()) {
        std::cout << "actual size: " << actual->size() << " expected size: " << expected.size() << std::endl;
        return false;
    }

    for (size_t i = 0; i < actual->size(); i++) {
        auto actualClause = actual->at(i);
        auto expectedClause = expected[i];

        if (actualClause->size() != expectedClause.size()) {
            std::cout << "actual clause size: " << actualClause->size()
                      << " expected clause size: " << expectedClause.size() << std::endl;
            return false;
        }

        for (size_t j = 0; j < actualClause->size(); j++) {
            if (actualClause->at(j) != expectedClause[j]) {
                std::cout << "actual: " << actualClause->at(j) << " expected: " << expectedClause[j] << std::endl;
                return false;
            }
        }
    }

    return true;
}
// ai-gen end

bool
requireParsedClauseCount(const std::shared_ptr<QueryParser> &queryParser,
                         int declareCount, int selectCount, int suchThatCount, int patternCount, int withCount) {
    auto res = queryParser->getDeclareClauses().size() == declareCount &&
           queryParser->getSelectClauses().size() == selectCount &&
           queryParser->getRelationshipClauses().size() == suchThatCount &&
           queryParser->getPatternClauses().size() == patternCount &&
           queryParser->getWithClauses().size() == withCount;

    if (!res) {
        std::cout << "declare: " << queryParser->getDeclareClauses().size() << " expected: " << declareCount << std::endl;
        std::cout << "select: " << queryParser->getSelectClauses().size() << " expected: " << selectCount << std::endl;
        std::cout << "such that: " << queryParser->getRelationshipClauses().size() << " expected: " << suchThatCount << std::endl;
        std::cout << "pattern: " << queryParser->getPatternClauses().size() << " expected: " << patternCount << std::endl;
        std::cout << "with: " << queryParser->getWithClauses().size() << " expected: " << withCount << std::endl;
    }

    return res;
}
