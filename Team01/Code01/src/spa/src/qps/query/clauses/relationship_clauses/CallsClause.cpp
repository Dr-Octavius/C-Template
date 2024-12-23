#include "../../QueryClause.h"

#include <regex>

CallsClause::CallsClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2)
        : RelationshipClause(arg1, arg2) {
    CallsClause::setArgTypes();
}
// Calls(arg1, arg2)
void CallsClause::setArgTypes() {
    if (std::dynamic_pointer_cast<ProcedureSynonym>(arg1) ||
        std::dynamic_pointer_cast<EntityLiteral>(arg1) ||
        std::dynamic_pointer_cast<Wildcard>(arg1)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg1)) {
        throw QuerySyntaxError("CallsClause::setArgTypes: invalid arg1 literal");
    } else {
        throw QuerySemanticError("CallsClause::setArgTypes: invalid arg1 type");
    }

    if (std::dynamic_pointer_cast<ProcedureSynonym>(arg2) ||
        std::dynamic_pointer_cast<EntityLiteral>(arg2) ||
        std::dynamic_pointer_cast<Wildcard>(arg2)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg2)) {
        throw QuerySyntaxError("CallsClause::setArgTypes: invalid arg2 literal");
    } else {
        throw QuerySemanticError("CallsClause::setArgTypes: invalid arg2 type");
    }
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>> CallsClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> callsMap;
    callsMap = storageReader->getCallsMap();
    auto procedures = setWithTypes({EntityType::PROCEDURE}, storageReader);
    return getResult(callsMap, storageReader, procedures, procedures);
}
