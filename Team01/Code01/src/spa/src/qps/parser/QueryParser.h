#pragma once

#include <vector>
#include <sstream>
#include <string>
#include <regex>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "common/Utils.h"
#include "../query/Query.h"
#include "../query/QueryClause.h"
#include "../query/DesignEntity.h"

class QueryParser {
public:
    explicit QueryParser(const std::string &queryString);

    void parseClauses();

    std::shared_ptr<Query> parseQuery();

    std::unordered_set<std::shared_ptr<DeclareClause>> getDeclareClauses();

    std::vector<std::shared_ptr<SelectClause>> getSelectClauses();

    std::unordered_set<std::shared_ptr<RelationshipClause>> getRelationshipClauses();

    std::unordered_set<std::shared_ptr<PatternClause>> getPatternClauses();

    std::unordered_set<std::shared_ptr<WithClause>> getWithClauses();

private:
    std::string queryString;

    std::shared_ptr<Query> query;

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Synonym>>> declaredSynonyms;

    // TODO: remove these
    std::unordered_set<std::shared_ptr<DeclareClause>> declareClauses;
    std::vector<std::shared_ptr<SelectClause>> selectClauses;
    std::unordered_set<std::shared_ptr<RelationshipClause>> relationshipClauses;
    std::unordered_set<std::shared_ptr<PatternClause>> patternClauses;
    std::unordered_set<std::shared_ptr<WithClause>> withClauses;

    static std::shared_ptr<Synonym> parseSynonym(const std::string &entityType, const std::string &name);

    std::shared_ptr<Synonym> getDeclaredSynonym(const std::string &arg);

    std::shared_ptr<DesignEntity> parseStmtEnt(const std::string &arg);

    static std::shared_ptr<Expression> parseExpr(const std::string &expr);

    std::pair<std::shared_ptr<DesignEntity>, std::shared_ptr<AttributeType>> parseWithArg(const std::string &arg);

    std::pair<std::shared_ptr<Synonym>, std::shared_ptr<AttributeType>> parseAttrRef(const std::string &attrRef);

    std::pair<std::shared_ptr<Synonym>, std::shared_ptr<AttributeType>> parseElem(const std::string &elem);

    // Clause parsing methods
    void parseTokenizedClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens);

    void parseDeclarationClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens);

    void parseSelectClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens);

    void parseRelationshipClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens);

    void parsePatternClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens);

    void parseWithClause(const std::shared_ptr<std::vector<std::string>> &clauseTokens);

    static void
    checkClauseOrder(const std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>> &tokenizedClauses);
};

class QueryTokenizer {
public:
    explicit QueryTokenizer(const std::string &qString);

    std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>> tokenizeQuery();

private:
    std::istringstream queryStream;
    std::shared_ptr<std::vector<std::string>> tokenizedQuery;
    size_t currTokenIdx;
    std::string lastClause;
    std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>> tokenizedClauses;

    std::shared_ptr<std::vector<std::string>> tokenizeDeclaration(const std::string &synType);

    std::shared_ptr<std::vector<std::string>> tokenizeSelect();

    std::shared_ptr<std::vector<std::string>> tokenizeSuchThat();

    std::shared_ptr<std::vector<std::string>> tokenizePattern();

    std::shared_ptr<std::vector<std::string>> tokenizeWith();

    std::shared_ptr<std::vector<std::string>> splitTokens();

    std::string getNextToken();

    std::string getNextTokenExpect(char expectedChar);

    std::string getNextTokenExpectNot(char unexpectedChar);

    std::string peekNextToken();

    std::string readUntilChar(char stopChar, bool includeStopChar = false);

    static bool isSpecialChar(char c);

    static bool equalsStringChar(std::string str, char c);

    static std::string charToString(char c);

    static std::string sizetToString(size_t i);
};

class QueryParserUtil {
public:
    static std::shared_ptr<std::vector<std::string>> splitString(const std::string &str, char delimiter);

    static std::string removeWhitespace(const std::string &str);
};
