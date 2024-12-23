#include "../../QueryClause.h"

IfPatternClause::IfPatternClause(std::shared_ptr<Synonym> syn, std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2, std::shared_ptr<DesignEntity> arg3) : PatternClause(syn),
	arg1(arg1), arg2(arg2), arg3(arg3) {
	setArgTypes();

    addSynonym(syn);
    addSynonym(arg1);
}

void IfPatternClause::setArgTypes() {
	if (!std::dynamic_pointer_cast<IfSynonym>(syn)) {
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
		throw QuerySyntaxError("PatternClause: Invalid rhs, must be a wildcard: " + arg2->getValue());
	}

	if (!std::dynamic_pointer_cast<Wildcard>(arg3)) {
		throw QuerySyntaxError("PatternClause: Invalid rhs, must be a wildcard: " + arg3->getValue());
	}
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>> IfPatternClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
	std::vector<std::unordered_map<std::string, std::string>> result;
	std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> ifs = storageReader->getAllIfPatternEntries();
	result.reserve(ifs.size());
	std::unordered_set<std::string> variableMap = storageReader->getEntitiesFromPkb(EntityType::VARIABLE);

	// If/While
	// 1. pattern ifs(_, _, _)
	// Return if: for all if in ifs
	if (std::dynamic_pointer_cast<Wildcard>(arg1) && std::dynamic_pointer_cast<Wildcard>(arg2) && std::dynamic_pointer_cast<Wildcard>(arg3)) {
		if (this->isNegated()) {
			return result;
		}
		//Look through the storage reader for all ifs
		for (const auto& ifEntry : ifs) {
			bool matches = (!this->isNegated() && (ifEntry.second.size() > 0)) || (this->isNegated() && (ifEntry.second.size() == 0));
			if (matches) {
				std::unordered_map<std::string, std::string> intermediate;
				intermediate[syn->getValue()] = ifEntry.first.first;
				result.push_back(intermediate);
			}
		}
		return result;
	}

	// 2. pattern ifs(v, _, _)
	// Return if, v: for all if in ifs and v is one of all terminal variables in
	// the pattern tree of this if
	if (std::dynamic_pointer_cast<VariableSynonym>(arg1) && std::dynamic_pointer_cast<Wildcard>(arg2) && std::dynamic_pointer_cast<Wildcard>(arg3)) {
		// Look through the storage reader for all ifs
		// Check if the variable is in the pattern tree of the if
		for (const auto& ifEntry : ifs) {
			for (auto& var  : variableMap) {
				bool exists = (ifEntry.second.find(std::make_pair(var, EntityType::VARIABLE)) != ifEntry.second.end());
				bool matches = (!this->isNegated() && exists) || (this->isNegated() && !exists);
				if (matches) {
					std::unordered_map<std::string, std::string> intermediate;
					intermediate[syn->getValue()] = ifEntry.first.first;
					intermediate[arg1->getValue()] = var;
					result.push_back(intermediate);
				}
			}
		}
		return result;
	}
	
	// 3. pattern ifs("x", _, _)
	// Return empty one column table if "x" not in variable database
	// Return if: for all if in ifs where "x" also a terminal variable in the
	// pattern tree of this if
	if (std::dynamic_pointer_cast<EntityLiteral>(arg1) && std::dynamic_pointer_cast<Wildcard>(arg2) && std::dynamic_pointer_cast<Wildcard>(arg3)) {
		if (!this->isNegated() && variableMap.find(arg1->getValue()) == variableMap.end()) {
			return result;
		}
		for (const auto& ifEntry : ifs) {
			std::pair<std::string, EntityType> variablePair = { arg1->getValue(), EntityType::VARIABLE };
			bool matches = (!this->isNegated() && (ifEntry.second.find(variablePair) != ifEntry.second.end())) || 
				(this->isNegated() && (ifEntry.second.find(variablePair) == ifEntry.second.end()));
			if (matches) {
				std::unordered_map<std::string, std::string> ifIntermediate;
				ifIntermediate[syn->getValue()] = ifEntry.first.first;
				result.push_back(ifIntermediate);
			}
		}
		return result;
	}
	return result;
}
