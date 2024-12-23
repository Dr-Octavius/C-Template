#include <iostream>
#include <string>
#include <regex>
#include <unordered_set>

#include "DesignEntity.h"
#include "qps/util/QpsConstants.h"
#include "qps/util/ExprChecker.h"

DesignEntity::DesignEntity(const std::string &value) : value(value) {}

std::string DesignEntity::getValue() const {
    return value;
}

// Synonym Class
Synonym::Synonym(const std::string &value) : DesignEntity(value) {
    if (!checkSyntax()) {
        throw QuerySyntaxError("Synonym: Invalid syntax");
    }
    setAllowedTypes();
}

bool Synonym::checkSyntax() const {
    return std::regex_match(value, QpsConstants::ALPHANUMERIC_REGEX);
}

bool Synonym::matches(const std::string &literal, std::shared_ptr<EntityType> type) const {
    return (allowedTypes.find(*type) != allowedTypes.end());
}

bool Synonym::compareName(std::shared_ptr<Synonym> s) {
    return value == s->getValue();
}

void Synonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::STMT, EntityType::READ, EntityType::PRINT,
                                                EntityType::ASSIGN, EntityType::CALL,
                                                EntityType::WHILE, EntityType::IF, EntityType::VARIABLE,
                                                EntityType::CONSTANT, EntityType::PROCEDURE};
}

std::unordered_set<EntityType> Synonym::getAllowedTypes() {
    return allowedTypes;
}

std::string Synonym::getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader) {
    return val;
}

std::size_t Synonym::hash() const {
    // Using the default hash function for std::string
    return std::hash<std::string>{}(value);
}


Literal::Literal(const std::string &value) : DesignEntity(value) {}

bool Literal::matches(const std::string &literal, std::shared_ptr<EntityType> type) const {
    return (literal == value);
}

// Statement Classes
StatementRef::StatementRef(const std::string &value) : DesignEntity(value) {}


StatementLiteral::StatementLiteral(const std::string &value) : DesignEntity(value), StatementRef(value),
                                                                Literal(value) {
    if (!checkSyntax()) {
        throw QuerySyntaxError("StatementLiteral: Invalid syntax");
    }
}

bool StatementLiteral::checkSyntax() const {
    return std::regex_match(value, QpsConstants::NUMERIC_REGEX);
}


StatementSynonym::StatementSynonym(const std::string &value) : DesignEntity(value), StatementRef(value),
                                                                Synonym(value) {
    setAllowedTypes();
}

void StatementSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::STMT, EntityType::READ, EntityType::PRINT,
                                                    EntityType::ASSIGN, EntityType::CALL,
                                                    EntityType::WHILE, EntityType::IF};
}


ReadSynonym::ReadSynonym(const std::string &value) : DesignEntity(value), StatementSynonym(value) {
    setAllowedTypes();
}

void ReadSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::READ};
}

std::string ReadSynonym::getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader) {
    if (*attributeType == AttributeType::STMT_NO) {
        return val;
    }
    auto readVarMap = storageReader->getReadVarNameMap();
    auto varSet = readVarMap[std::make_pair (val, EntityType::READ)];
    std::vector<std::pair<std::string, EntityType>> varVec(varSet.begin(), varSet.end());
    if (varVec.empty()) {
        throw QueryUnknownError("Read: No varName attribute found");
    }
    return varVec[0].first;
}

PrintSynonym::PrintSynonym(const std::string &value) : DesignEntity(value), StatementSynonym(value) {
    setAllowedTypes();
}

void PrintSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::PRINT};
}

std::string PrintSynonym::getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader) {
    if (*attributeType == AttributeType::STMT_NO) {
        return val;
    }
    auto printVarMap = storageReader->getPrintVarNameMap();
    auto varSet = printVarMap[std::make_pair (val, EntityType::PRINT)];
    std::vector<std::pair<std::string, EntityType>> varVec(varSet.begin(), varSet.end());
    if (varVec.empty()) {
        throw QueryUnknownError("Print: No varName attribute found");
    }
    return varVec[0].first;
}

AssignSynonym::AssignSynonym(const std::string &value) : DesignEntity(value), StatementSynonym(value) {
    setAllowedTypes();
}

void AssignSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::ASSIGN};
}


CallSynonym::CallSynonym(const std::string &value) : DesignEntity(value), StatementSynonym(value) {
    setAllowedTypes();
}

void CallSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::CALL};
}

std::string CallSynonym::getAttribute(std::string val, std::shared_ptr<AttributeType> attributeType, std::shared_ptr<StorageReader>& storageReader) {
    if (*attributeType == AttributeType::STMT_NO) {
        return val;
    }
    auto callVarMap = storageReader->getCallProcNameMap();
    auto varSet = callVarMap[std::make_pair (val, EntityType::CALL)];
    std::vector<std::pair<std::string, EntityType>> varVec(varSet.begin(), varSet.end());
    if (varVec.empty()) {
        throw QueryUnknownError("Call: No procName attribute found");
    }
    return varVec[0].first;
}

WhileSynonym::WhileSynonym(const std::string &value) : DesignEntity(value), StatementSynonym(value) {
    setAllowedTypes();
}

void WhileSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::WHILE};
}


IfSynonym::IfSynonym(const std::string &value) : DesignEntity(value), StatementSynonym(value) {
    setAllowedTypes();
}

void IfSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::IF};
}


// Entity Classes

EntityRef::EntityRef(const std::string &value) : DesignEntity(value) {}

EntityLiteral::EntityLiteral(const std::string &value) : DesignEntity(value), EntityRef(value), Literal(value) {
    if (!checkSyntax()) {
        throw QuerySyntaxError("EntityLiteral: invalid syntax: " + value);
    }
}

bool EntityLiteral::checkSyntax() const {
    // Base implementation for EntityLiteral

    return std::regex_match(value, QpsConstants::ALPHANUMERIC_REGEX);
}

VariableLiteral::VariableLiteral(const std::string &value) : DesignEntity(value), EntityLiteral(value) {
    if (!checkSyntax()) {
        throw QuerySyntaxError("Invalid Variable Literal: " + value);
    }
}

bool VariableLiteral::checkSyntax() const {
    return std::regex_match(value, QpsConstants::ALPHANUMERIC_REGEX);
}

ConstantLiteral::ConstantLiteral(const std::string &value) : DesignEntity(value), Literal(value) {
    if (!checkSyntax()) {
        throw QuerySyntaxError("Invalid Constant Literal: " + value);
    }
}

bool ConstantLiteral::checkSyntax() const {
    return std::regex_match(value, QpsConstants::NUMERIC_REGEX);
}

ProcedureLiteral::ProcedureLiteral(const std::string &value) : DesignEntity(value), EntityLiteral(value) {
    if (!checkSyntax()) {
        throw QuerySyntaxError("Invalid Procedure Literal: " + value);
    }
}

bool ProcedureLiteral::checkSyntax() const {
    return std::regex_match(value, QpsConstants::ALPHANUMERIC_REGEX);
}


EntitySynonym::EntitySynonym(const std::string &value) : DesignEntity(value), EntityRef(value), Synonym(value) {
    setAllowedTypes();
}

void EntitySynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::PROCEDURE, EntityType::VARIABLE, EntityType::CONSTANT};
}


ProcedureSynonym::ProcedureSynonym(const std::string &value) : DesignEntity(value), EntitySynonym(value) {
    setAllowedTypes();
};

void ProcedureSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::PROCEDURE};
}


VariableSynonym::VariableSynonym(const std::string &value) : DesignEntity(value), EntitySynonym(value) {
    setAllowedTypes();
};

void VariableSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::VARIABLE};
}


ConstantSynonym::ConstantSynonym(const std::string &value) : DesignEntity(value), EntitySynonym(value) {
    setAllowedTypes();
};

void ConstantSynonym::setAllowedTypes() {
    allowedTypes = std::unordered_set<EntityType>{EntityType::CONSTANT};
}

// Expression
Expression::Expression(const std::string &value) : DesignEntity(value) {}

ExactExpression::ExactExpression(const std::string &value) : DesignEntity(value), Expression(value) {
    if (!ExactExpression::checkSyntax()) {
        throw QuerySyntaxError("ExactExpression: Invalid syntax");
    }
    this->value = QueryParserUtil::removeWhitespace(value);
}

bool ExactExpression::checkSyntax() const {
    return ExprChecker::parseExpression(value);
}

bool ExactExpression::matches(const std::string &literal, std::shared_ptr<EntityType> type) const {
    // TODO: implement
    return true;
}

PartialExpression::PartialExpression(const std::string &value) : DesignEntity(value), Expression(value) {
    if (!PartialExpression::checkSyntax()) {
        throw QuerySyntaxError("PartialExpression: Invalid syntax");
    }
    this->value = QueryParserUtil::removeWhitespace(value);
}

bool PartialExpression::checkSyntax() const {
    return ExprChecker::parseExpression(value);
}

bool PartialExpression::matches(const std::string &literal, std::shared_ptr<EntityType> type) const {
    return true;
}

// Wildcard
Wildcard::Wildcard() : DesignEntity("_"), StatementRef("_"), EntityRef("_"), Expression(value) {}

bool Wildcard::checkSyntax() const {
    return true;
}

bool Wildcard::matches(const std::string &literal, std::shared_ptr<EntityType> type) const {
    return true;
}
