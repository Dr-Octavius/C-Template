#include <iostream>
#include <stdexcept>
#include <regex>

#include "Query.h"

void Query::addDeclaredSynonyms(
        const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Synonym>>>& synonymMap) {
    declaredSynonyms = *synonymMap;
}

void Query::addSelectClause(const std::shared_ptr<SelectClause>& selectClause) {
    selectClauses.push_back(selectClause);
    selectSynonyms.push_back(selectClause->getSynonym());
}

void Query::addEvaluateClause(const std::shared_ptr<EvaluateClause>& evaluateClause) {
    evaluateClauses.push_back(evaluateClause);
}

std::unordered_set<std::string>
Query::intersect(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2) {
    std::unordered_set<std::string> result;
    for (const auto &element: set1) {
        if (set2.find(element) != set2.end()) {
            result.insert(element);
        }
    }
    return result;
}

std::unordered_set<std::string>
Query::unionSets(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2) {
    std::unordered_set<std::string> result;
    for (const auto &element: set1) {
        result.insert(element);
    }
    for (const auto &element: set2) {
        result.insert(element);
    }
    return result;
}

std::vector<std::unordered_map<std::string, std::string>>
Query::getAllValues(std::shared_ptr<Synonym> s, std::shared_ptr<StorageReader> &storageReader) {
    auto name = s->getValue();
    auto entityTypes = s->getAllowedTypes();
    auto allLiterals = std::unordered_set<std::string>();
    for (auto &type: entityTypes) {
        allLiterals = unionSets(allLiterals, storageReader->getEntitiesFromPkb(type));
    }
    std::vector<std::unordered_map<std::string, std::string>> subtuples;
    for (auto &literal: allLiterals) {
        subtuples.push_back({{name, literal}});
    }
    return subtuples;
}

// To be used when all clauses are negations
void Query::populateDataTable(std::shared_ptr<StorageReader> &storageReader) {
    std::unordered_map<std::string, std::vector<std::string>> allowedMap;
    for (auto [synonymName, synonym]: declaredSynonyms) {
        auto types = synonym->getAllowedTypes();
        std::unordered_set<std::string> allowedLiterals{};
        for (auto &type: types) {
            allowedLiterals = unionSets(allowedLiterals, storageReader->getEntitiesFromPkb(type));
        }

        allowedMap[synonymName] = std::vector<std::string>(allowedLiterals.begin(), allowedLiterals.end());
    }
    dataTable.insertCrossProduct(allowedMap);
}

// Return val if no AttributeType
// Else query storageReader and return attribute string
std::string
Query::getType(std::string val, std::shared_ptr<SelectClause> &sClause, std::shared_ptr<StorageReader> &storageReader) {
    return sClause->getAttribute(val, storageReader);
}

void Query::groupEvaluateClauses() {
    auto queryGroupMap = std::unordered_map<std::string, std::shared_ptr<QueryClauseGroup>>();
    auto constantGroup = std::make_shared<QueryClauseGroup>();

    for (const auto& clause: evaluateClauses) {
        if (clause->getSynonyms()->empty()) {
            constantGroup->insertClause(clause);
            continue;
        }

        auto groupKey = synonymGroups.find(*(clause->getSynonyms()->begin()));
        if (queryGroupMap.find(groupKey) == queryGroupMap.end()) {
            queryGroupMap[groupKey] = std::make_shared<QueryClauseGroup>();
        }

        queryGroupMap[groupKey]->insertClause(clause);
    }

    if (!constantGroup->empty()) {
        queryGroups.push_back(constantGroup);
    }

    for (const auto& [key, group]: queryGroupMap) {
        queryGroups.push_back(group);
    }

    std::sort(queryGroups.begin(), queryGroups.end());
}

void Query::groupUsedSynonyms() {
    for (const auto& clause: evaluateClauses) {
        auto newSynonyms = clause->getSynonyms();
        for (const auto& syn: *newSynonyms) {
            usedSynonyms.insert(syn);
            synonymGroups.addString(syn);
        }

        if (newSynonyms->size() <= 1) {
            continue;
        }

        for (auto it = newSynonyms->begin(); it != newSynonyms->end(); it++) {
            for (auto it2 = it; it2 != newSynonyms->end(); it2++) {
                synonymGroups.unite(*it, *it2);
            }
        }
    }
}

std::vector<std::string> Query::resultFalse() {
    // At least 1 select clause is guaranteed by parser
    if (selectClauses[0]->isBooleanClause()) {
        return {"FALSE"};
    } else {
        return {};
    }
}

std::vector<std::string> Query::evaluate(std::shared_ptr<StorageReader> storageReader) {
    auto cache = std::make_shared<QPSCache> (storageReader);
    groupUsedSynonyms();
    groupEvaluateClauses();

    for (const auto& group: queryGroups) {
        auto result = group->evaluate(storageReader, cache);
        if (!result.has_value()) {
            continue;
        }

        if (result->empty()) {
            return resultFalse();
        }

        dataTable.merge(*result);
    }

    // Boolean Select
    if (selectClauses[0]->isBooleanClause()) {
        return {"TRUE"};
    }

    // Add all select synonyms not used in evaluate clauses
    for (const auto& selectSyn: selectSynonyms) {
        if (usedSynonyms.find(selectSyn->getValue()) == usedSynonyms.end()) {
            std::vector<std::unordered_map<std::string, std::string>> col = getAllValues(selectSyn, storageReader);
            dataTable.merge(col);
        }
    }

    // Tuple Select
    auto selectHeaders = std::vector<std::string>();
    for (const auto& syn: selectSynonyms) {
        selectHeaders.push_back(syn->getValue());
    }
    auto table = dataTable.getMultipleColumnValues(selectHeaders);

    for (auto &row: table) {
        if (row.size() != selectClauses.size()) {
            throw QueryUnknownError("Query: Different number of select clauses and select synonyms");
        }
        for (int i = 0; i < row.size(); i++) {
            auto sClause = selectClauses[i];
            auto val = row[i];
            row[i] = getType(val, sClause, storageReader);
        }
    }

    // Convert to vector of space-separated tuples
    auto result = std::vector<std::string>();
    for (auto &row: table) {
        std::string curr;
        for (int i = 0; i < row.size(); i++) {
            if (i > 0) {
                curr += " ";
            }
            curr += row[i];
        }
        result.push_back(curr);
    }
    return result;
}