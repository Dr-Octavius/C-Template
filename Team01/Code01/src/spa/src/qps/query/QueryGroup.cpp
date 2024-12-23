#include <algorithm>

#include "QueryClauseGroup.h"

QueryClauseGroup::QueryClauseGroup() {
    this->clauses = std::make_shared<std::vector<std::shared_ptr<EvaluateClause>>>();
}

bool QueryClauseGroup::operator<(const QueryClauseGroup &other) const {
    return this->score < other.score;
}

bool QueryClauseGroup::operator>(const QueryClauseGroup &other) const {
    return this->score > other.score;
}

// ai-gen start(gpt,1,e)
// prompt: https://chat.openai.com/share/cfe33cc2-79cb-4e1a-8229-49ec7d6a405e
bool QueryClauseGroup::intersects(const std::shared_ptr<std::unordered_set<std::string>>& set1,
                            const std::shared_ptr<std::unordered_set<std::string>>& set2) {
    for (const auto& word : *set1) {
        if (set2->find(word) != set2->end()) {
            return true; // Intersection found
        }
    }
    return false; // No intersection
}

void QueryClauseGroup::sortClauses() {
    if (!clauses || clauses->empty()) return;

    auto currSynonyms = std::make_shared<std::unordered_set<std::string>>();
    unionSynonyms(currSynonyms, clauses->at(0)->getSynonyms());

    for (size_t i = 1; i < clauses->size(); ++i) {
        if (intersects(currSynonyms, clauses->at(i)->getSynonyms())) {
            unionSynonyms(currSynonyms, clauses->at(i)->getSynonyms());
            continue;
        }

        for (size_t j = i + 1; j < clauses->size(); ++j) {
            if (!intersects(currSynonyms, clauses->at(j)->getSynonyms())) {
                continue;
            }

            std::swap(clauses->at(i), clauses->at(j));
            unionSynonyms(currSynonyms, clauses->at(i)->getSynonyms());
            break;
        }
    }
}
// ai-gen end

// ai-gen start(copilot,0,e)
void QueryClauseGroup::unionSynonyms(std::shared_ptr<std::unordered_set<std::string>>& set1,
                           const std::shared_ptr<std::unordered_set<std::string>>& set2) {
    for (const auto& word : *set2) {
        set1->insert(word);
    }
}
// ai-gen end

bool QueryClauseGroup::insertClause(const std::shared_ptr<EvaluateClause> &clause) {
    this->clauses->push_back(clause);
    score += clause->getScore();
    return true;
}

bool QueryClauseGroup::empty() const {
    return this->clauses->empty();
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>>
QueryClauseGroup::evaluate(const std::shared_ptr<StorageReader> &storageReader, std::shared_ptr<QPSCache>& cache) {
    std::shared_ptr<DataTable> result = std::make_shared<DataTable>();
    sortClauses();
    for (const auto &clause: *this->clauses) {
        auto clauseResult = clause->evaluate(storageReader, cache);
        if (!clauseResult.has_value()) {
            continue; // TRUE
        }

        if (clauseResult->empty()) {
            return clauseResult; // FALSE
        }

        result->merge(*clauseResult);
    }

    if (result->isEmpty()) {
        return std::nullopt; // TRUE
    }

    return result->getRows();
}
