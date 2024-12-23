#include "../../QueryClause.h"

#include <regex>
#include <iostream>

ParentClause::ParentClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2)
        : RelationshipClause(arg1, arg2) {
    ParentClause::setArgTypes();
}

void ParentClause::setArgTypes() {
    if (std::dynamic_pointer_cast<StatementRef>(arg1)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg1)) {
        throw QuerySyntaxError("ParentClause::setArgTypes: invalid arg1 literal");
    } else {
        throw QuerySemanticError("ParentClause::setArgTypes: invalid arg1 type");
    }

    if (std::dynamic_pointer_cast<StatementRef>(arg2)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg2)) {
        throw QuerySyntaxError("ParentClause::setArgTypes: invalid arg2 literal");
    } else {
        throw QuerySemanticError("ParentClause::setArgTypes: invalid arg2 type");
    }
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>> ParentClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> parentMap;
    parentMap = storageReader->getParentMap();
    auto stmts = setWithTypes({EntityType::READ, EntityType::PRINT, EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE, EntityType::IF}, storageReader);
    return getResult(parentMap, storageReader, stmts, stmts, true);
}
