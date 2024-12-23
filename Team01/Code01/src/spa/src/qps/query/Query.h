#pragma once

#include <unordered_set>

#include "QueryClause.h"
#include "DataTable.h"
#include "QueryClauseGroup.h"
#include "qps/util/UnionFind.h"

class Query {
private:
    std::unordered_map<std::string, std::shared_ptr<Synonym>> declaredSynonyms;
    std::unordered_set<std::string> usedSynonyms;
    UnionFind synonymGroups;
    std::vector<std::shared_ptr<Synonym>> selectSynonyms; // Stores select Synonyms

    std::vector<std::shared_ptr<SelectClause>> selectClauses;
    std::vector<std::shared_ptr<EvaluateClause>> evaluateClauses;
    std::vector<std::shared_ptr<QueryClauseGroup>> queryGroups;

    //Store data table and join after every query
    DataTable dataTable;

    // Helpers
    std::unordered_set<std::string>
    intersect(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2);

    std::unordered_set<std::string>
    unionSets(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2);

    void populateDataTable(std::shared_ptr<StorageReader> &storageReader);

    std::string
    getType(std::string val, std::shared_ptr<SelectClause> &sClause, std::shared_ptr<StorageReader> &storageReader);

    // New evaluation helper functions
    void groupUsedSynonyms();

    void groupEvaluateClauses();

    std::vector<std::string> resultFalse();

public:
    void
    addDeclaredSynonyms(const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Synonym>>> &synonymMap);

    void addSelectClause(const std::shared_ptr<SelectClause> &selectClause);

    void addEvaluateClause(const std::shared_ptr<EvaluateClause> &evaluateClause);

    std::vector<std::string> evaluate(std::shared_ptr<StorageReader> storageReader); // new evaluate function

    std::vector<std::unordered_map<std::string, std::string>>
    getAllValues(std::shared_ptr<Synonym> s, std::shared_ptr<StorageReader> &storageReader);
};