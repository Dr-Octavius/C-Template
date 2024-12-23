#include "QueryClause.h"

QueryClause::QueryClause() {
    synonyms = std::make_shared<std::unordered_set<std::string>>();
}

void QueryClause::addSynonym(const std::shared_ptr<DesignEntity>& arg) {
    if (std::dynamic_pointer_cast<Synonym>(arg)) {
        synonyms->insert(arg->getValue());
    }
}

std::shared_ptr<std::unordered_set<std::string>> QueryClause::getSynonyms() {
    return synonyms;
}

void EvaluateClause::setNegated(bool b) {
    negated = b;
}

bool EvaluateClause::isNegated() const {
    return negated;
}

int EvaluateClause::getScore() {
    return getSynonyms()->size();
}
