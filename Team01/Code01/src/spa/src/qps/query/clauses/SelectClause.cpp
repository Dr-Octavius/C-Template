#include "../QueryClause.h"

#include <regex>

// stmt BOOLEAN; ...
// Select BOOLEAN such that Parent (BOOLEAN, a)

SelectClause::SelectClause(std::shared_ptr<Synonym> syn, bool isBoolean, std::shared_ptr<AttributeType> attributeType) : syn(syn), isBoolean(isBoolean), attributeType(attributeType) {}

std::shared_ptr<Synonym> SelectClause::getSynonym() {
    return syn;
}

bool SelectClause::isBooleanClause() {
    return isBoolean;
}

std::shared_ptr<AttributeType> SelectClause::getAttributeType() {
    return attributeType;
}

std::string SelectClause::getAttribute(std::string val, std::shared_ptr<StorageReader>& storageReader) {
    // Return val if attribute type is nullptr
    if (! attributeType) {
        return val;
    }
    return syn->getAttribute(val, attributeType, storageReader);
}