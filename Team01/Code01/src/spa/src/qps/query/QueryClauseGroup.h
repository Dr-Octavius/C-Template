#pragma once

#include "qps/query/QueryClause.h"
#include "DataTable.h"

class QueryClauseGroup {
    int score = 0;

    std::shared_ptr<std::vector<std::shared_ptr<EvaluateClause>>> clauses;

    void sortClauses();

    static bool intersects(const std::shared_ptr<std::unordered_set<std::string>>& set1,
                    const std::shared_ptr<std::unordered_set<std::string>>& set2);

    static void unionSynonyms(std::shared_ptr<std::unordered_set<std::string>>& set1,
               const std::shared_ptr<std::unordered_set<std::string>>& set2);

public:
    QueryClauseGroup();

    bool operator<(const QueryClauseGroup &other) const;

    bool operator>(const QueryClauseGroup &other) const;

    bool insertClause(const std::shared_ptr<EvaluateClause> &clause);

    bool empty() const;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(const std::shared_ptr<StorageReader> &storageReader, std::shared_ptr<QPSCache> &cache);
};