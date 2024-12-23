#include "../QueryClause.h"

WithClause::WithClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2,
                        std::shared_ptr<AttributeType> attr1, std::shared_ptr<AttributeType> attr2)
        : arg1(arg1), arg2(arg2), attr1(attr1), attr2(attr2) {
    WithClause::setArgTypes();
    addSynonym(arg1);
    addSynonym(arg2);
}

void WithClause::setArgTypes() {
    if (std::dynamic_pointer_cast<VariableLiteral>(arg1) &&
        std::dynamic_pointer_cast<VariableLiteral>(arg2)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<ConstantLiteral>(arg1) &&
                std::dynamic_pointer_cast<ConstantLiteral>(arg2)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Synonym>(arg1) && std::dynamic_pointer_cast<Synonym>(arg2)) {
        if ((*attr1 == AttributeType::PROC_NAME || *attr1 == AttributeType::VAR_NAME) &&
            (*attr2 == AttributeType::PROC_NAME || *attr2 == AttributeType::VAR_NAME)) {
            // do nothing
        } else if ((*attr1 == AttributeType::VALUE || *attr1 == AttributeType::STMT_NO) &&
                   (*attr2 == AttributeType::VALUE || *attr2 == AttributeType::STMT_NO)) {
            // do nothing
        } else {
            throw QuerySemanticError("WithClause::setArgTypes: invalid arg types");
        }
    } else if (std::dynamic_pointer_cast<Synonym>(arg1)) {
        if ((*attr1 == AttributeType::PROC_NAME || *attr1 == AttributeType::VAR_NAME) && std::dynamic_pointer_cast<VariableLiteral>(arg2)) {
            // do nothing
        } else if ((*attr1 == AttributeType::VALUE || *attr1 == AttributeType::STMT_NO) && std::dynamic_pointer_cast<ConstantLiteral>(arg2)) {
            // do nothing
        } else {
            throw QuerySemanticError("WithClause::setArgTypes: invalid arg types");
        }
    } else if (std::dynamic_pointer_cast<Synonym>(arg2)) {
        if ((*attr2 == AttributeType::PROC_NAME || *attr2 == AttributeType::VAR_NAME) &&
            std::dynamic_pointer_cast<VariableLiteral>(arg1)) {
            // do nothing
        } else if ((*attr2 == AttributeType::VALUE || *attr2 == AttributeType::STMT_NO) &&
                    std::dynamic_pointer_cast<ConstantLiteral>(arg1)) {
            // do nothing
        } else {
            throw QuerySemanticError("WithClause::setArgTypes: invalid arg types");
        }
    } else {
        throw QuerySemanticError("WithClause::setArgTypes: invalid arg types");
    }
}

std::vector<std::unordered_map<std::string, std::string>>
WithClause::pairsToMap(std::vector<std::pair<std::string, std::string>> pairVector, std::string filter, std::shared_ptr<Synonym> syn) {
    std::vector<std::unordered_map<std::string, std::string>> mapVector;
    for (auto &pair: pairVector) {
        std::unordered_map<std::string, std::string> map;
        bool matches = (!this->isNegated() && (pair.second == filter)) || (this->isNegated() && (pair.second != filter));
        if (matches) {
            map[syn->getValue()] = pair.first;
            mapVector.push_back(map);
        }
    }
    return mapVector;
}

std::unordered_set<std::string> WithClause::unionSets(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2) {
    std::unordered_set<std::string> result;
    for (const auto& element : set1) {
        result.insert(element);
    }
    for (const auto& element : set2) {
        result.insert(element);
    }
    return result;
}

// Returns vector of literals and attribute pairs
std::vector<std::pair<std::string, std::string>>
WithClause::getAttributeColumn(std::shared_ptr<Synonym> syn, std::shared_ptr<AttributeType> attrType,
                                std::shared_ptr<StorageReader> storageReader) {
    auto entityTypes = syn->getAllowedTypes();
    auto literals = std::unordered_set<std::string>();
    for (auto type: entityTypes) {
        literals = unionSets(literals, storageReader->getEntitiesFromPkb(type));
    }
    auto col = std::vector<std::pair<std::string, std::string>>();
    auto synName = syn->getValue();
    for (auto literal: literals) {
        auto attr = syn->getAttribute(literal, attrType, storageReader);
        col.push_back({literal, attr});
    }
    return col;
}

std::vector<std::unordered_map<std::string, std::string>>
WithClause::getCross(std::vector<std::pair<std::string, std::string>> col1,
                        std::vector<std::pair<std::string, std::string>> col2, std::shared_ptr<Synonym> syn1, std::shared_ptr<Synonym> syn2) {
    auto table = std::vector<std::unordered_map<std::string, std::string>>();
    for (auto pair1: col1) {
        for (auto pair2: col2) {
            auto newRow = std::unordered_map<std::string, std::string>();
            bool matches = (!this->isNegated() && (pair1.second == pair2.second)) || (this->isNegated() && (pair1.second != pair2.second));
            if (matches) {
                newRow[syn1->getValue()] = pair1.first;
                newRow[syn2->getValue()] = pair2.first;
                table.push_back(newRow);
            }
        }
    }
    return table;
}


std::optional<std::vector<std::unordered_map<std::string, std::string>>>
WithClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    std::vector<std::pair<std::string, std::string>> attributeCol1;
    std::vector<std::pair<std::string, std::string>> attributeCol2;
    int synCount = 0;

    if (auto syn1 = std::dynamic_pointer_cast<Synonym>(arg1)) {
        attributeCol1 = getAttributeColumn(syn1, attr1, storageReader);
        synCount++;
    }
    if (auto syn2 = std::dynamic_pointer_cast<Synonym>(arg2)) {
        attributeCol2 = getAttributeColumn(syn2, attr2, storageReader);
        synCount++;
    }

    if (!synCount) {
        bool equal = arg1->getValue() == arg2->getValue();
        if ((!this->isNegated() && equal) || (this->isNegated() && !equal)) {
            return std::nullopt;
        }
        return std::vector<std::unordered_map<std::string, std::string>> ();
    } else if (synCount == 1) {
        if (!attributeCol1.empty()) {
            return pairsToMap(attributeCol1, arg2->getValue(), std::dynamic_pointer_cast<Synonym>(arg1));
        } else {
            return pairsToMap(attributeCol2, arg1->getValue(), std::dynamic_pointer_cast<Synonym>(arg2));
        }
    }
    auto cross = getCross(attributeCol1, attributeCol2, std::dynamic_pointer_cast<Synonym>(arg1), std::dynamic_pointer_cast<Synonym>(arg2));
    return cross;
}