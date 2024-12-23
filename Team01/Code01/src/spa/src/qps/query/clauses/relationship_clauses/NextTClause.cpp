#include "../../QueryClause.h"

#include <regex>
#include <iostream>
#include <queue>

NextTClause::NextTClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2)
        : RelationshipClause(arg1, arg2) {
    NextTClause::setArgTypes();
}

void NextTClause::setArgTypes() {
    if (std::dynamic_pointer_cast<StatementRef>(arg1)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg1)) {
        throw QuerySyntaxError("FollowsTClause::setArgTypes: invalid arg1 literal");
    } else {
        throw QuerySemanticError("FollowsTClause::setArgTypes: invalid arg1 type");
    }

    if (std::dynamic_pointer_cast<StatementRef>(arg2)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg2)) {
        throw QuerySyntaxError("FollowsTClause::setArgTypes: invalid arg2 literal");
    } else {
        throw QuerySemanticError("FollowsTClause::setArgTypes: invalid arg2 type");
    }
}

int NextTClause::getScore() {
    return getSynonyms()->size() * 5;
}

// TODO: Get CFG from PKB and return list of tuples that match this clause
// Return std::nullopt if there are no synonyms and the clause is true
// Otherwise, return list of all tuples that match this clause
// Example of return value: { {"s1": "1", "s2": "2"}, {"s1": "3", "s2": "9"}, {"s1": "6", "s2": "4"} }
std::optional<std::vector<std::unordered_map<std::string, std::string>>> NextTClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    auto nextMap = storageReader->getNextMap();
    auto result = std::vector<std::unordered_map<std::string, std::string>>();
    bool arg1Syn; 
    bool arg2Syn;
    if(std::dynamic_pointer_cast<StatementSynonym> (arg1) || std::dynamic_pointer_cast<Wildcard>(arg1)) {
        arg1Syn = true;
    }
    if(std::dynamic_pointer_cast<StatementSynonym> (arg2) || std::dynamic_pointer_cast<Wildcard>(arg2)) {
        arg2Syn = true;
    }
    // Next*(1, 2) or Next*(1, s)
    if (!arg1Syn) {
        EntityType entityType = getEntityType(arg1->getValue(), storageReader);
        auto arg1Node = std::make_pair(arg1->getValue(), entityType);
        return findResult(storageReader, cache, arg1Node, arg1, arg2, arg2Syn, QueryType::NEXTT);
    }

    // Next*(s, 2) or Next*(s1, s2)
    //// Get set of nodes that match arg1 synonym
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> arg1Nodes;
    for (auto& pair : nextMap) {
        auto node = pair.first;
        if (arg1->matches(node.first, std::make_shared<EntityType>(node.second))) {
            if (node.second == EntityType::PROCEDURE || node.second == EntityType::PROCEDURE_END) {
                continue;
            }
            arg1Nodes.insert(node);
        }
    }

    auto finalResult = std::vector<std::unordered_map<std::string, std::string>>();
    for (auto& node : arg1Nodes) {
        auto tableOpt = findResult(storageReader, cache, node, arg1, arg2, arg2Syn, QueryType::NEXTT);
        if (!tableOpt.has_value()) {
            auto row = std::unordered_map<std::string, std::string>();
            row[arg1->getValue()] = node.first;
            finalResult.push_back(row);
            continue;
        } 
        auto table = *tableOpt;
        for (auto& subRow : table) {
            auto row = std::unordered_map<std::string, std::string>();
            row[arg1->getValue()] = node.first;
            for (auto& keyVal : subRow) {
                row.insert(keyVal);
            }
            finalResult.push_back(row);
        }
    }
    return finalResult;
}
