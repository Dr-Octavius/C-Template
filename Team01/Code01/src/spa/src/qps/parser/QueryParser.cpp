#include <stdexcept>
#include <vector>
#include <iostream>

#include "QueryParser.h"
#include "qps/util/QpsConstants.h"

QueryParser::QueryParser(const std::string &queryString) : queryString(queryString) {
    declaredSynonyms = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Synonym>>>();
    declareClauses = std::unordered_set<std::shared_ptr<DeclareClause>>();
    selectClauses = std::vector<std::shared_ptr<SelectClause>>();
    relationshipClauses = std::unordered_set<std::shared_ptr<RelationshipClause>>();
    patternClauses = std::unordered_set<std::shared_ptr<PatternClause>>();
    withClauses = std::unordered_set<std::shared_ptr<WithClause>>();

    query = std::make_shared<Query>();
}

std::unordered_set<std::shared_ptr<DeclareClause>> QueryParser::getDeclareClauses() {
    return declareClauses;
}

std::vector<std::shared_ptr<SelectClause>> QueryParser::getSelectClauses() {
    return selectClauses;
}

std::unordered_set<std::shared_ptr<RelationshipClause>> QueryParser::getRelationshipClauses() {
    return relationshipClauses;
}

std::unordered_set<std::shared_ptr<PatternClause>> QueryParser::getPatternClauses() {
    return patternClauses;
}

std::unordered_set<std::shared_ptr<WithClause>> QueryParser::getWithClauses() {
    return withClauses;
}

std::shared_ptr<Query> QueryParser::parseQuery() {
    parseClauses();

    query->addDeclaredSynonyms(declaredSynonyms);
    return query;
}

void QueryParser::parseClauses() {
    auto tokenizedClauses = QueryTokenizer(queryString).tokenizeQuery();

    checkClauseOrder(tokenizedClauses);

    for (const auto &clauseTokens: *tokenizedClauses) {
        parseTokenizedClause(clauseTokens);
    }
}

void QueryParser::checkClauseOrder(
        const std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>> &tokenizedClauses) {
    auto selectFound = false;

    for (const auto &clauseTokens: *tokenizedClauses) {
        auto first = clauseTokens->at(0);

        if (QpsConstants::isDeclarationKeyword(first) && selectFound) {
            throw QuerySyntaxError("QueryParser: declarations must be before 'Select'");
        } else if (first == QpsConstants::SELECT_KEYWORD) {
            if (selectFound) {
                throw QuerySyntaxError("QueryParser: there must be exactly one 'Select'");
            }

            selectFound = true;
        } else if (first == QpsConstants::SUCH_THAT_KEYWORD && !selectFound) {
            throw QuerySyntaxError("QueryParser: 'such that' must be after 'Select'");
        } else if (first == QpsConstants::PATTERN_KEYWORD && !selectFound) {
            throw QuerySyntaxError("QueryParser: 'pattern' must be after 'Select'");
        }
    }

    if (!selectFound) {
        throw QuerySyntaxError("QueryParser: there must be exactly one 'Select'");
    }
}

void QueryParser::parseTokenizedClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens) {
    auto first = clauseTokens->at(0);

    if (QpsConstants::isDeclarationKeyword(first)) {
        return parseDeclarationClause(clauseTokens);
    } else if (first == QpsConstants::SELECT_KEYWORD) {
        return parseSelectClause(clauseTokens);
    } else if (first == QpsConstants::SUCH_THAT_KEYWORD) {
        return parseRelationshipClause(clauseTokens);
    } else if (first == QpsConstants::PATTERN_KEYWORD) {
        return parsePatternClause(clauseTokens);
    } else if (first == QpsConstants::WITH_KEYWORD) {
        return parseWithClause(clauseTokens);
    } else {
        throw QuerySyntaxError("QueryParser: Invalid clause type");
    }
}

void QueryParser::parseDeclarationClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens) {
    auto synType = clauseTokens->at(0);
    for (int i = 1; i < clauseTokens->size(); ++i) {
        auto synName = clauseTokens->at(i);
        auto syn = parseSynonym(synType, synName);
        auto clause = std::make_shared<DeclareClause>(syn);

        declareClauses.insert(clause);
        auto res = declaredSynonyms->insert({synName, syn});

        if (!res.second) {
            throw QuerySemanticError("QueryParser::parseDeclarationClause: multiple declaration");
        }
    }
}

void QueryParser::parseSelectClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens) {
    if (clauseTokens->at(1) == QpsConstants::BOOLEAN_KEYWORD) {
        auto clause = std::make_shared<SelectClause>(nullptr, true);
        selectClauses.push_back(clause);

        query->addSelectClause(clause);
        return;
    }

    for (int i = 1; i < clauseTokens->size(); ++i) {
        auto elem = parseElem(clauseTokens->at(i));
        auto clause = std::make_shared<SelectClause>(elem.first, false, elem.second);

        selectClauses.push_back(clause);
        query->addSelectClause(clause);
    }
}

void QueryParser::parseRelationshipClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens) {
    auto isNegated = false;
    auto relTypePos = 1;
    auto arg1Pos = 2;
    auto arg2Pos = 3;

    if (clauseTokens->at(1) == QpsConstants::NOT_KEYWORD) {
        isNegated = true;
        relTypePos = 2;
        arg1Pos = 3;
        arg2Pos = 4;
    }

    auto relType = clauseTokens->at(relTypePos);
    auto arg1 = parseStmtEnt(clauseTokens->at(arg1Pos));
    auto arg2 = parseStmtEnt(clauseTokens->at(arg2Pos));

    std::shared_ptr<RelationshipClause> clause;
    if (relType == QpsConstants::FOLLOWS_KEYWORD) {
        clause = std::make_shared<FollowsClause>(arg1, arg2);
    } else if (relType == QpsConstants::FOLLOWS_T_KEYWORD) {
        clause = std::make_shared<FollowsTClause>(arg1, arg2);
    } else if (relType == QpsConstants::PARENT_KEYWORD) {
        clause = std::make_shared<ParentClause>(arg1, arg2);
    } else if (relType == QpsConstants::PARENT_T_KEYWORD) {
        clause = std::make_shared<ParentTClause>(arg1, arg2);
    } else if (relType == QpsConstants::USES_KEYWORD) {
        clause = std::make_shared<UsesClause>(arg1, arg2);
    } else if (relType == QpsConstants::MODIFIES_KEYWORD) {
        clause = std::make_shared<ModifiesClause>(arg1, arg2);
    } else if (relType == QpsConstants::CALLS_KEYWORD) {
        clause = std::make_shared<CallsClause>(arg1, arg2);
    } else if (relType == QpsConstants::CALLS_T_KEYWORD) {
        clause = std::make_shared<CallsTClause>(arg1, arg2);
    } else if (relType == QpsConstants::NEXT_KEYWORD) {
        clause = std::make_shared<NextClause>(arg1, arg2);
    } else if (relType == QpsConstants::NEXT_T_KEYWORD) {
        clause = std::make_shared<NextTClause>(arg1, arg2);
    } else if (relType == QpsConstants::AFFECTS_KEYWORD) {
        clause = std::make_shared<AffectsClause>(arg1, arg2);
    } else {
        throw QuerySyntaxError("QueryParser::parseClause: unknown relationship: " + relType);
    }

    clause->setNegated(isNegated);
    relationshipClauses.insert(clause);
    query->addEvaluateClause(clause);
}

void QueryParser::parsePatternClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens) {
    auto isNegated = false;
    auto synPos = 1;
    auto arg1Pos = 2;
    auto arg2Pos = 3;
    auto arg3Pos = 4;

    if (clauseTokens->at(1) == QpsConstants::NOT_KEYWORD) {
        isNegated = true;
        synPos = 2;
        arg1Pos = 3;
        arg2Pos = 4;
        arg3Pos = 5;
    }

    auto syn = getDeclaredSynonym(clauseTokens->at(synPos));
    std::shared_ptr<PatternClause> clause;
    if (std::dynamic_pointer_cast<AssignSynonym>(syn)) {
        if (isNegated ? clauseTokens->size() != 5 : clauseTokens->size() != 4) {
            throw QuerySyntaxError("QueryParser::parsePatternClause: assign pattern clause must have 3 tokens");
        }

        auto arg1 = parseStmtEnt(clauseTokens->at(arg1Pos));
        auto arg2 = parseExpr(clauseTokens->at(arg2Pos));

        clause = std::make_shared<AssignPatternClause>(syn, arg1, arg2);
    } else if (std::dynamic_pointer_cast<WhileSynonym>(syn)) {
        if (isNegated ? clauseTokens->size() != 5 : clauseTokens->size() != 4) {
            throw QuerySyntaxError("QueryParser::parsePatternClause: while pattern clause must have 3 tokens");
        }

        auto arg1 = parseStmtEnt(clauseTokens->at(arg1Pos));
        auto arg2 = parseExpr(clauseTokens->at(arg2Pos));

         clause = std::make_shared<WhilePatternClause>(syn, arg1, arg2);
    } else if (std::dynamic_pointer_cast<IfSynonym>(syn)) {
        if (isNegated ? clauseTokens->size() != 6 : clauseTokens->size() != 5) {
            throw QuerySyntaxError("QueryParser::parsePatternClause: if pattern clause must have 4 tokens");
        }

        auto arg1 = parseStmtEnt(clauseTokens->at(arg1Pos));
        auto arg2 = parseExpr(clauseTokens->at(arg2Pos));
        auto arg3 = parseExpr(clauseTokens->at(arg3Pos));

         clause = std::make_shared<IfPatternClause>(syn, arg1, arg2, arg3);
    } else {
        throw QuerySemanticError("QueryParser::parsePatternClause: invalid synonym type");
    }

    clause->setNegated(isNegated);
    patternClauses.insert(clause);
    query->addEvaluateClause(clause);
}

void QueryParser::parseWithClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens) {
    auto isNegated = false;
    auto arg1Pos = 1;
    auto arg2Pos = 2;

    if (clauseTokens->at(1) == QpsConstants::NOT_KEYWORD) {
        isNegated = true;
        arg1Pos = 2;
        arg2Pos = 3;
    }

    auto arg1 = parseWithArg(clauseTokens->at(arg1Pos));
    auto arg2 = parseWithArg(clauseTokens->at(arg2Pos));

    auto clause = std::make_shared<WithClause>(arg1.first, arg2.first, arg1.second, arg2.second);
    clause->setNegated(isNegated);
    withClauses.insert(clause);
    query->addEvaluateClause(clause);
}

std::shared_ptr<Synonym> QueryParser::parseSynonym(const std::string &entityType, const std::string &name) {
    if (!std::regex_match(name, QpsConstants::ALPHANUMERIC_REGEX)) {
        throw QuerySyntaxError("Invalid synonym format");
    }

    if (entityType == QpsConstants::STMT_KEYWORD) {
        return std::make_shared<StatementSynonym>(name);
    } else if (entityType == QpsConstants::READ_KEYWORD) {
        return std::make_shared<ReadSynonym>(name);
    } else if (entityType == QpsConstants::PRINT_KEYWORD) {
        return std::make_shared<PrintSynonym>(name);
    } else if (entityType == QpsConstants::ASSIGN_KEYWORD) {
        return std::make_shared<AssignSynonym>(name);
    } else if (entityType == QpsConstants::CALL_KEYWORD) {
        return std::make_shared<CallSynonym>(name);
    } else if (entityType == QpsConstants::WHILE_KEYWORD) {
        return std::make_shared<WhileSynonym>(name);
    } else if (entityType == QpsConstants::IF_KEYWORD) {
        return std::make_shared<IfSynonym>(name);
    } else if (entityType == QpsConstants::VARIABLE_KEYWORD) {
        return std::make_shared<VariableSynonym>(name);
    } else if (entityType == QpsConstants::CONSTANT_KEYWORD) {
        return std::make_shared<ConstantSynonym>(name);
    } else if (entityType == QpsConstants::PROCEDURE_KEYWORD) {
        return std::make_shared<ProcedureSynonym>(name);
    } else {
        throw QuerySyntaxError("Invalid synonym type: " + entityType);
    }
}

std::shared_ptr<Synonym> QueryParser::getDeclaredSynonym(const std::string &arg) {
    if (!std::regex_match(arg, QpsConstants::ALPHANUMERIC_REGEX)) {
        throw QuerySyntaxError("Invalid synonym format");
    }

    auto res = declaredSynonyms->find(arg);

    if (res == declaredSynonyms->end()) {
        throw QuerySemanticError("QueryParser: undeclared synonym : " + arg);
    }

    return res->second;
}

std::shared_ptr<DesignEntity> QueryParser::parseStmtEnt(const std::string &arg) {
    if (arg.length() == 1 && arg[0] == QpsConstants::UNDERSCORE) {
        return std::make_shared<Wildcard>();
    } else if (std::regex_match(arg, QpsConstants::NUMERIC_REGEX)) {
        return std::make_shared<StatementLiteral>(arg);
    } else if (std::regex_match(arg, QpsConstants::QUOTED_REGEX)) {
        auto formattedArg = arg.substr(1, arg.length() - 2);
        return std::make_shared<EntityLiteral>(formattedArg);
    } else if (std::regex_match(arg, QpsConstants::ALPHANUMERIC_REGEX)) {
        return getDeclaredSynonym(arg);
    } else {
        throw QuerySyntaxError("Invalid arg: " + arg);
    }
}

std::shared_ptr<Expression> QueryParser::parseExpr(const std::string &expr) {
    if (expr == QpsConstants::WILDCARD_KEYWORD) {
        return std::make_shared<Wildcard>();
    }

    auto isPartial = false;
    std::string trimmedExp;

    if (std::regex_match(expr, QpsConstants::UNDERSCORE_QUOTED_REGEX)) {
        trimmedExp = expr.substr(2, expr.length() - 4);
        isPartial = true;
    } else if (std::regex_match(expr, QpsConstants::QUOTED_REGEX)) {
        trimmedExp = expr.substr(1, expr.length() - 2);
    } else {
        throw QuerySyntaxError("QueryParser::parseExpr: invalid expression");
    }

    if (isPartial) {
        return std::make_shared<PartialExpression>(trimmedExp);
    } else {
        return std::make_shared<ExactExpression>(trimmedExp);
    }
}

std::pair<std::shared_ptr<DesignEntity>, std::shared_ptr<AttributeType>>
QueryParser::parseWithArg(const std::string &arg) {
    if (std::regex_match(arg, QpsConstants::NUMERIC_REGEX)) {
        return {std::make_shared<ConstantLiteral>(arg), nullptr};
    } else if (std::regex_match(arg, QpsConstants::QUOTED_REGEX)) {
        auto trimmedExp = arg.substr(1, arg.length() - 2);
        return {std::make_shared<VariableLiteral>(trimmedExp), nullptr};
    } else if (std::regex_match(arg, QpsConstants::ATTR_REF_REGEX)) {
        return parseAttrRef(arg);
    } else {
        throw QuerySyntaxError("QueryParser::parseWithArg: invalid attribute reference");
    }
}

std::pair<std::shared_ptr<Synonym>, std::shared_ptr<AttributeType>>
QueryParser::parseAttrRef(const std::string &attrRef) {
    auto args = QueryParserUtil::splitString(attrRef, QpsConstants::PERIOD);
    auto syn = getDeclaredSynonym(args->at(0));
    auto attrType = args->at(1);

    if (attrType == QpsConstants::PROC_NAME_KEYWORD) {
        return {syn, std::make_shared<AttributeType>(AttributeType::PROC_NAME)};
    } else if (attrType == QpsConstants::VAR_NAME_KEYWORD) {
        return {syn, std::make_shared<AttributeType>(AttributeType::VAR_NAME)};
    } else if (attrType == QpsConstants::VALUE_KEYWORD) {
        return {syn, std::make_shared<AttributeType>(AttributeType::VALUE)};
    } else if (attrType == QpsConstants::STMT_NO_KEYWORD) {
        return {syn, std::make_shared<AttributeType>(AttributeType::STMT_NO)};
    } else {
        throw QuerySyntaxError("QueryParser::parseAttrRef: invalid attribute type");
    }
}

std::pair<std::shared_ptr<Synonym>, std::shared_ptr<AttributeType>> QueryParser::parseElem(const std::string &elem) {
    if (std::regex_match(elem, QpsConstants::ALPHANUMERIC_REGEX)) {
        return {getDeclaredSynonym(elem), nullptr};
    } else if (std::regex_match(elem, QpsConstants::ATTR_REF_REGEX)) {
        return parseAttrRef(elem);
    } else {
        throw QuerySyntaxError("QueryParser::parseElem: invalid element");
    }
}
