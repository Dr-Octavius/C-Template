#include "../../QueryClause.h"

#include <regex>

UsesClause::UsesClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2)
        : RelationshipClause(arg1, arg2) {
    UsesClause::setArgTypes();
}

void UsesClause::setArgTypes() {
    if (std::dynamic_pointer_cast<StatementSynonym>(arg1) ||
        std::dynamic_pointer_cast<StatementLiteral>(arg1) ||
        std::dynamic_pointer_cast<ProcedureSynonym>(arg1) ||
        std::dynamic_pointer_cast<EntityLiteral>(arg1)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg1)) {
        throw QuerySyntaxError("UsesClause::setArgTypes: invalid arg1 literal");
    } else {
        throw QuerySemanticError("UsesClause::setArgTypes: invalid arg1 type");
    }

    if (std::dynamic_pointer_cast<VariableSynonym>(arg2) ||
        std::dynamic_pointer_cast<EntityLiteral>(arg2) ||
        std::dynamic_pointer_cast<Wildcard>(arg2)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg2)) {
        throw QuerySyntaxError("UsesClause::setArgTypes: invalid arg2 literal");
    } else {
        throw QuerySemanticError("UsesClause::setArgTypes: invalid arg2 type");
    }
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>> UsesClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> usesMap;
    usesMap = storageReader->getUsesMap();
    auto procsAndStmts = setWithTypes({EntityType::READ, EntityType::PRINT, EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE, EntityType::IF, EntityType::PROCEDURE}, storageReader);
    auto variables = setWithTypes({EntityType::VARIABLE}, storageReader);
    return getResult(usesMap, storageReader, procsAndStmts, variables);
}