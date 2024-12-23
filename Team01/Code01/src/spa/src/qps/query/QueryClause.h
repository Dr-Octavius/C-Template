#pragma once

#include "DesignEntity.h"
#include "common/Utils.h"
#include "QPSCache.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional>

class QueryClause {
    std::shared_ptr<std::unordered_set<std::string>> synonyms;

public:
    QueryClause();

    void addSynonym(const std::shared_ptr<DesignEntity> &arg);

    std::shared_ptr<std::unordered_set<std::string>> getSynonyms();
};

class DeclareClause : public QueryClause {
private:
    std::shared_ptr<Synonym> syn;

public:
    explicit DeclareClause(std::shared_ptr<Synonym> syn);

    std::shared_ptr<Synonym> getSynonym();
};

class SelectClause : public QueryClause {
private:
    std::shared_ptr<Synonym> syn;
    bool isBoolean;
    std::shared_ptr<AttributeType> attributeType;

public:
    explicit SelectClause(std::shared_ptr<Synonym> syn, bool isBoolean = false,
                std::shared_ptr<AttributeType> attributeType = nullptr);

    std::shared_ptr<Synonym> getSynonym();

    bool isBooleanClause();

    std::shared_ptr<AttributeType> getAttributeType();

    std::string getAttribute(std::string val, std::shared_ptr<StorageReader> &storageReader);
};

class EvaluateClause : public QueryClause {
protected:
    bool negated = false;

public:
    void setNegated(bool b);

    bool isNegated() const;

    virtual int getScore();

    virtual std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) = 0;
};

class RelationshipClause : public EvaluateClause {
protected:
    std::shared_ptr<DesignEntity> arg1;
    std::shared_ptr<DesignEntity> arg2;

    std::unordered_set<std::shared_ptr<Synonym>> clauseSynonyms;

    //helper
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> setWithTypes(std::unordered_set<EntityType> allowedTypes, std::shared_ptr<StorageReader> storageReader);
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> computeDifference(
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> map,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities1, std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities2);

    //Common functions for NextT and Affects clauses
    std::optional<std::vector<std::unordered_map<std::string, std::string>>> findResult(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache,
                std::pair<std::string, EntityType> arg1Node, std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2, bool arg2Syn, QueryType qType);
    
    void dfs(
        std::pair<std::string, EntityType> currentNode,
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> nextMap, 
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> usesMap,
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash> visited, 
        std::unordered_set<std::pair<std::string, EntityType>, PairHash>& reachable,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash>& affected,
        std::pair<std::string, EntityType> modifiedVar,
        bool modified);
    
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> cacheReachableAndAffectedNodes(
        std::pair<std::string, EntityType> startNode,
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> nextMap,
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> usesMap,
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap,
        std::pair<std::string, EntityType> modifiedVar,
        std::shared_ptr<QPSCache>& cache,
        QueryType qType);
    
    EntityType getEntityType(std::string stmtNo, std::shared_ptr<StorageReader> storageReader);

    std::unordered_set<std::pair<std::string, EntityType>, PairHash> cacheReachableNodes(
        std::pair<std::string, EntityType> startNode,
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> nextMap, 
        std::shared_ptr<QPSCache>& cache);

    // Helper
    std::optional<std::pair<std::string, EntityType>> getIfExists(std::pair<std::string, EntityType> key, 
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> map);

public:
    RelationshipClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    // Checks whether arg is a literal or synonym
    virtual void setArgTypes() = 0;

    // Common logic that searches map
    virtual std::optional<std::vector<std::unordered_map<std::string, std::string>>> getResult(
            std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> relationMap,
            std::shared_ptr<StorageReader> storageReader,
            std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities1, 
            std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities2,
            bool reverse = false);
};

class FollowsClause : public RelationshipClause {
public:
    FollowsClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class FollowsTClause : public RelationshipClause {
public:
    FollowsTClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class ParentClause : public RelationshipClause {
public:
    ParentClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class ParentTClause : public RelationshipClause {
public:
    ParentTClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class UsesClause : public RelationshipClause {
public:
    UsesClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class ModifiesClause : public RelationshipClause {
public:
    ModifiesClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class CallsClause : public RelationshipClause {
public:
    CallsClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class CallsTClause : public RelationshipClause {
public:
    CallsTClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};


class NextTClause : public RelationshipClause {
private:

public:
    NextTClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    int getScore() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class NextClause : public RelationshipClause {
public:
    NextClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class AffectsClause : public RelationshipClause {
public:
    AffectsClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    int getScore() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};

class PatternClause : public EvaluateClause {
public:
    explicit PatternClause(std::shared_ptr<Synonym> syn);

    virtual void setArgTypes() = 0;

protected:
    std::shared_ptr<Synonym> syn;
};

class AssignPatternClause : public PatternClause {
public:
    AssignPatternClause(std::shared_ptr<Synonym> syn, std::shared_ptr<DesignEntity> lhs,
                        std::shared_ptr<DesignEntity> rhs);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>> processPatternMap(
            std::unordered_map<std::pair<std::string, EntityType>, std::pair<std::string, std::shared_ptr<RelFactorNode>>, PairHash> patternMap,
            std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap,
            std::unordered_set<std::string> variableMap);

protected:
    std::shared_ptr<DesignEntity> lhs;
    std::shared_ptr<DesignEntity> rhs;
};

class IfPatternClause : public PatternClause {
public:
    IfPatternClause(std::shared_ptr<Synonym> syn, std::shared_ptr<DesignEntity> arg1,
                    std::shared_ptr<DesignEntity> arg2, std::shared_ptr<DesignEntity> arg3);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;

protected:
    std::shared_ptr<DesignEntity> arg1;
    std::shared_ptr<DesignEntity> arg2;
    std::shared_ptr<DesignEntity> arg3;
};

class WhilePatternClause : public PatternClause {
public:
    WhilePatternClause(std::shared_ptr<Synonym> syn, std::shared_ptr<DesignEntity> arg1,
                       std::shared_ptr<DesignEntity> arg2);

    void setArgTypes() override;

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;

protected:
    std::shared_ptr<DesignEntity> arg1;
    std::shared_ptr<DesignEntity> arg2;
};

class WithClause : public EvaluateClause {
protected:
    std::shared_ptr<DesignEntity> arg1;
    std::shared_ptr<AttributeType> attr1;
    std::shared_ptr<DesignEntity> arg2;
    std::shared_ptr<AttributeType> attr2;

    // Helpers 
    std::unordered_set<std::string>
    unionSets(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2);

    std::vector<std::pair<std::string, std::string>>
    getAttributeColumn(std::shared_ptr<Synonym> syn, std::shared_ptr<AttributeType> attr,
                       std::shared_ptr<StorageReader> storageReader);

    std::vector<std::unordered_map<std::string, std::string>>
    pairsToMap(std::vector<std::pair<std::string, std::string>> pairVector, std::string filterLiteral,
               std::shared_ptr<Synonym> syn);

    std::vector<std::unordered_map<std::string, std::string>>
    getCross(std::vector<std::pair<std::string, std::string>> col1,
             std::vector<std::pair<std::string, std::string>> col2, std::shared_ptr<Synonym> syn1,
             std::shared_ptr<Synonym> syn2);

public:
    WithClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2,
               std::shared_ptr<AttributeType> attr1 = nullptr, std::shared_ptr<AttributeType> attr2 = nullptr);

    void setArgTypes();

    std::optional<std::vector<std::unordered_map<std::string, std::string>>>
    evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) override;
};