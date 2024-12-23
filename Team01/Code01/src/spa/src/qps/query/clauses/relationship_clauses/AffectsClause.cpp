#include "../../QueryClause.h"

AffectsClause::AffectsClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2)
        : RelationshipClause(arg1, arg2) {
    AffectsClause::setArgTypes();
}

void AffectsClause::setArgTypes() {
    if (std::dynamic_pointer_cast<AssignSynonym>(arg1) ||
        std::dynamic_pointer_cast<StatementLiteral>(arg1) ||
        std::dynamic_pointer_cast<Wildcard>(arg1)) {
        // do nothing
    } else if (auto casted = std::dynamic_pointer_cast<StatementSynonym>(arg1)) {
        auto isAllowed = casted->getAllowedTypes().find(EntityType::ASSIGN) != casted->getAllowedTypes().end();
        if (!isAllowed) {
            throw QuerySemanticError("AffectsClause::setArgTypes: invalid arg1 type");
        }
    } else if (std::dynamic_pointer_cast<Literal>(arg1)) {
        throw QuerySyntaxError("AffectsClause::setArgTypes: invalid arg1 literal");
    } else {
        throw QuerySemanticError("AffectsClause::setArgTypes: invalid arg1 type");
    }

    if (std::dynamic_pointer_cast<AssignSynonym>(arg2) ||
        std::dynamic_pointer_cast<StatementLiteral>(arg2)||
        std::dynamic_pointer_cast<Wildcard>(arg2)) {
        // do nothing
    } else if (auto casted = std::dynamic_pointer_cast<StatementSynonym>(arg2)) {
        auto isAllowed = casted->getAllowedTypes().find(EntityType::ASSIGN) != casted->getAllowedTypes().end();
        if (!isAllowed) {
            throw QuerySemanticError("AffectsClause::setArgTypes: invalid arg1 type");
        }
    } else if (std::dynamic_pointer_cast<Literal>(arg2)) {
        throw QuerySyntaxError("AffectsClause::setArgTypes: invalid arg2 literal");
    } else {
        throw QuerySemanticError("AffectsClause::setArgTypes: invalid arg2 type");
    }
}

int AffectsClause::getScore() {
    return 5 * getSynonyms()->size();
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>>
AffectsClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    auto nextMap = storageReader->getNextMap();
    auto result = std::vector<std::unordered_map<std::string, std::string>>();
    bool arg1Syn; 
    bool arg2Syn;
    if(std::dynamic_pointer_cast<StatementSynonym> (arg1) || std::dynamic_pointer_cast<Wildcard> (arg1)) {
        arg1Syn = true;
    }
    if(std::dynamic_pointer_cast<StatementSynonym> (arg2) || std::dynamic_pointer_cast<Wildcard> (arg2)) {
        arg2Syn = true;
    }
    // Affects(1, 2) or Affects(1, s)
    if (!arg1Syn) {
        EntityType entityType = getEntityType(arg1->getValue(), storageReader);
        if (entityType != EntityType::ASSIGN) {
            if (!this->isNegated()) {
                return result;
            }
            return std::nullopt;
        }
        auto arg1Node = std::make_pair(arg1->getValue(), entityType);
        return findResult(storageReader, cache, arg1Node, arg1, arg2, arg2Syn, QueryType::AFFECTS);
    }

    // Affects(s, 2) or Affects(s1, s2)
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
        auto tableOpt = findResult(storageReader, cache, node, arg1, arg2, arg2Syn, QueryType::AFFECTS);
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
