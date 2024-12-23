#include "../../QueryClause.h"

WhilePatternClause::WhilePatternClause(std::shared_ptr<Synonym> syn, std::shared_ptr<DesignEntity> arg1,
                                       std::shared_ptr<DesignEntity> arg2) : PatternClause(syn), arg1(arg1),
                                                                             arg2(arg2) {
    setArgTypes();

    addSynonym(syn);
    addSynonym(arg1);
}

void WhilePatternClause::setArgTypes() {
    if (!std::dynamic_pointer_cast<WhileSynonym>(syn)) {
        throw QuerySyntaxError("PatternClause: Invalid statement synonym: " + syn->getValue());
    }

    if (std::dynamic_pointer_cast<VariableSynonym>(arg1) ||
        std::dynamic_pointer_cast<EntityLiteral>(arg1) ||
        std::dynamic_pointer_cast<Wildcard>(arg1)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(arg1)) {
        throw QuerySyntaxError("PatternClause: Invalid arg1: " + arg1->getValue());
    } else {
        throw QuerySemanticError("PatternClause::setArgTypes: invalid arg1 type");
    }


    if (!std::dynamic_pointer_cast<Wildcard>(arg2)) {
        throw QuerySyntaxError("PatternClause: Invalid block, must be a wildcard: " + arg2->getValue());
    }
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>>
WhilePatternClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    std::vector<std::unordered_map<std::string, std::string>> result;
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> whiles = storageReader->getAllWhilePatternEntries();
    result.reserve(whiles.size());
    std::unordered_set<std::string> variableMap = storageReader->getEntitiesFromPkb(EntityType::VARIABLE);

	// While
	// 1. pattern while(_, _)
	// Return while: for all while in whiles
	if (std::dynamic_pointer_cast<Wildcard>(arg1) && std::dynamic_pointer_cast<Wildcard>(arg2)) {
		//Look through the storage reader for all ifs
		for (const auto& whileEntry : whiles) {
			bool matches = (!this->isNegated() && (whileEntry.second.size() > 0)) || (this->isNegated() && (whileEntry.second.size() == 0));
			if (matches) {
				std::unordered_map<std::string, std::string> intermediate;
				intermediate[syn->getValue()] = whileEntry.first.first;
				result.push_back(intermediate);
			}
		}
		return result;
	}

	// 2. pattern while(v, _)
	// Return while, v: for all while in whiles and v is one of all terminal variables in
	// the pattern tree of this while
	if (std::dynamic_pointer_cast<VariableSynonym>(arg1) && std::dynamic_pointer_cast<Wildcard>(arg2)) {
		// Look through the storage reader for all ifs
		// Check if the variable is in the pattern tree of the if
		for (const auto& whileEntry : whiles) {
			for (auto& var  : variableMap) {
				bool exists = (whileEntry.second.find(std::make_pair(var, EntityType::VARIABLE)) != whileEntry.second.end());
				bool matches = (!this->isNegated() && exists) || (this->isNegated() && !exists);
				if (matches) {
					std::unordered_map<std::string, std::string> intermediate;
					intermediate[syn->getValue()] = whileEntry.first.first;
					intermediate[arg1->getValue()] = var;
					result.push_back(intermediate);
				}
			}
		}
		return result;
	}

	// 3. pattern while("x", _, _)
	// Return empty one column table if "x" not in variable database
	// Return while: for all while in w where "x" also a terminal variable in the
	// pattern tree of this w
	if (std::dynamic_pointer_cast<EntityLiteral>(arg1) && std::dynamic_pointer_cast<Wildcard>(arg2)) {
		if (!this->isNegated() && variableMap.find(arg1->getValue()) == variableMap.end()) {
			return result;
		}
		for (const auto& whileEntry : whiles) {
			std::pair<std::string, EntityType> variablePair = { arg1->getValue(), EntityType::VARIABLE };
			bool matches = (!this->isNegated() && (whileEntry.second.find(variablePair) != whileEntry.second.end())) || 
				(this->isNegated() && (whileEntry.second.find(variablePair) == whileEntry.second.end()));
			if (matches) {
				std::unordered_map<std::string, std::string> whileIntermediate;
				whileIntermediate[syn->getValue()] = whileEntry.first.first;
				result.push_back(whileIntermediate);
			}
		}
		return result;
	}
}