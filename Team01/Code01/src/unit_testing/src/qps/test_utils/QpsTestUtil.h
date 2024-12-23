#pragma once

#include <vector>
#include <string>
#include <memory>

#include "qps/parser/QueryParser.h"

bool compareTokenizedClauses(const std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>> &actual,
                             const std::vector<std::vector<std::string>> &expected);

bool requireParsedClauseCount(const std::shared_ptr<QueryParser> &queryParser,
                              int declareCount = 0, int selectCount = 0, int suchThatCount = 0, int patternCount = 0, int withCount = 0);
