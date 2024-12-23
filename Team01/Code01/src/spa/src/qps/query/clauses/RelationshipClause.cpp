#include "qps/query/QueryClause.h"

#include <regex>
#include <iostream>
#include <optional>
#include <queue>

RelationshipClause::RelationshipClause(std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2) :
        arg1(arg1), arg2(arg2) {
    addSynonym(arg1);
    addSynonym(arg2);
}


std::unordered_set<std::pair<std::string, EntityType>, PairHash> RelationshipClause::setWithTypes(std::unordered_set<EntityType> allowedTypes, std::shared_ptr<StorageReader> storageReader) {
    auto result = std::unordered_set<std::pair<std::string, EntityType>, PairHash> ();
    for (auto& type : allowedTypes) {
        auto literals = storageReader->getEntitiesFromPkb(type);
        for (auto& literal : literals) {
            result.insert(std::make_pair(literal, type));
        }
    }
    return result;
}

EntityType RelationshipClause::getEntityType(std::string stmtNo, std::shared_ptr<StorageReader> storageReader) {
    if ((storageReader->getEntitiesFromPkb(EntityType::READ)).find(stmtNo) != (storageReader->getEntitiesFromPkb(EntityType::READ)).end()) {
        return EntityType::READ;
    } else if ((storageReader->getEntitiesFromPkb(EntityType::PRINT)).find(stmtNo) != (storageReader->getEntitiesFromPkb(EntityType::PRINT)).end()) {
        return EntityType::PRINT;
    } else if ((storageReader->getEntitiesFromPkb(EntityType::ASSIGN)).find(stmtNo) != (storageReader->getEntitiesFromPkb(EntityType::ASSIGN)).end()) {
        return EntityType::ASSIGN;
    } else if ((storageReader->getEntitiesFromPkb(EntityType::CALL)).find(stmtNo) != (storageReader->getEntitiesFromPkb(EntityType::CALL)).end()) {
        return EntityType::CALL;
    } else if ((storageReader->getEntitiesFromPkb(EntityType::IF)).find(stmtNo) != (storageReader->getEntitiesFromPkb(EntityType::IF)).end()) {
        return EntityType::IF;
    } else {
        return EntityType::WHILE;
    }
}

// BFS Traversal of CFG
std::unordered_set<std::pair<std::string, EntityType>, PairHash> RelationshipClause::cacheReachableNodes(
    std::pair<std::string, EntityType> startNode,
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> nextMap, 
    std::shared_ptr<QPSCache>& cache) {
    
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> closure;
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> visited;
    std::queue<std::pair<std::string, EntityType>> q;
    q.push(startNode);
    while (!q.empty()) {
        std::pair<std::string, EntityType> current = q.front();
        q.pop();
        for (const auto& neighbor : nextMap[current]) {
            if (neighbor.second == EntityType::PROCEDURE_END) {
                continue;
            }
            if (visited.find(neighbor) == visited.end()) {
                closure.insert(neighbor);
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    cache->cacheReachable(startNode, closure);
    return closure;
}

std::optional<std::pair<std::string, EntityType>> RelationshipClause::getIfExists(std::pair<std::string, EntityType> key, 
std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> map) {

    for (auto& pair : map) {
        if (pair.first == key) {
            std::vector<std::pair<std::string, EntityType>> vec(pair.second.begin(), pair.second.end());
            if (vec.empty()) {
                return std::nullopt;
            }
            return vec[0];
        }
    }
    return std::nullopt;
}


// Shared logic for Next* and Affects
// Get result when arg1 is a literal
// Cases: Next*(1, 2) or Next*(1, s)
std::optional<std::vector<std::unordered_map<std::string, std::string>>> RelationshipClause::findResult(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache,
                std::pair<std::string, EntityType> arg1Node, std::shared_ptr<DesignEntity> arg1, std::shared_ptr<DesignEntity> arg2, bool arg2Syn, QueryType qType) {
    
    auto nextMap = storageReader->getNextMap();
    auto modifiesMap = storageReader->getModifiesMap();
    auto modifiedVarOpt = getIfExists(arg1Node, modifiesMap);
    auto result = std::vector<std::unordered_map<std::string, std::string>>();
    
    std::optional<std::unordered_set<std::pair<std::string, EntityType>, PairHash>> closureOpt;
    if (qType == QueryType::NEXTT) {
        closureOpt = cache->getReachable(arg1Node);
    } else {
        closureOpt = cache->getAffected(arg1Node);
    }

    std::unordered_set<std::pair<std::string, EntityType>, PairHash> closure;
    if (closureOpt.has_value()) {
        closure = *closureOpt;
    } else {
        if (qType == QueryType::NEXTT) {
            // If node is an assign stmt and affected map has not been cached, then cache it too
            if ((arg1Node.second == EntityType::ASSIGN) && !(cache->getAffected(arg1Node).has_value())) {
                if (!modifiedVarOpt.has_value()) {
                    throw QueryUnknownError("Affects Clause Eval: Assign stmt not in modifies map");
                }
                closure = cacheReachableAndAffectedNodes(arg1Node, nextMap, storageReader->getUsesMap(), storageReader->getModifiesMap(), *modifiedVarOpt, cache, qType);
            } else {
                closure = cacheReachableNodes(arg1Node, nextMap, cache);
            }
        } else {
            if (arg1Node.second != EntityType::ASSIGN) {
                return result;
            }
            if (!modifiedVarOpt.has_value()) {
                throw QueryUnknownError("Affects Clause Eval: Assign stmt not in modifies map");
            }
            closure = cacheReachableAndAffectedNodes(arg1Node, nextMap, storageReader->getUsesMap(), storageReader->getModifiesMap(), *modifiedVarOpt, cache, qType);
        }
    }
    // Next*(1, 2) or Next*(1, _)
    if (!arg2Syn) {
        auto arg2Node = std::make_pair(arg2->getValue(), getEntityType(arg2->getValue(), storageReader));
        bool found = closure.find(arg2Node) != closure.end();
        bool matches = (!this->isNegated() && found) || (this->isNegated() && !found);
        if (matches) {
            return std::nullopt;
        }
        return result;
    }
    // Next*(1, s)
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> stmts;
    if (qType == QueryType::NEXTT) {
        stmts = setWithTypes({EntityType::READ, EntityType::PRINT, EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE, EntityType::IF}, storageReader);
    } else {
        stmts = setWithTypes({EntityType::ASSIGN}, storageReader);
    }
    for (auto& arg2Node : stmts) {
        // In case of same synonym, values should be same Next*(s, s)
        if ((arg1->getValue() == arg2->getValue()) && arg1Node.first != arg2Node.first) {
            continue;
        }
        if (!arg2->matches(arg2Node.first, std::make_shared<EntityType>(arg2Node.second))) {
            continue;
        }
        bool found = closure.find(arg2Node) != closure.end();
        bool matches = (!this->isNegated() && found) || (this->isNegated() && !found);
        if (matches) {
            auto row = std::unordered_map<std::string, std::string> ();
            row[arg2->getValue()] = arg2Node.first;
            result.push_back(row);
        }
    }
    return result;
}

// DFS Implementation
void RelationshipClause::dfs(
    std::pair<std::string, EntityType> currentNode,
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> nextMap, 
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> usesMap,
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap,
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> visited, 
    std::unordered_set<std::pair<std::string, EntityType>, PairHash>& reachable,
    std::unordered_set<std::pair<std::string, EntityType>, PairHash>& affected,
    std::pair<std::string, EntityType> modifiedVar,
    bool modified) {

    auto allowedModifiers = std::unordered_set<EntityType> {EntityType::ASSIGN, EntityType::READ, EntityType::CALL};
    
    // Explore neighbors
    for (const auto& neighbor : nextMap[currentNode]) {
        if (neighbor.second == EntityType::PROCEDURE_END) {
            continue;
        }
        if (visited.find(neighbor) == visited.end()) {
            visited.insert(neighbor);
            reachable.insert(neighbor);
            if (!modified && (neighbor.second == EntityType::ASSIGN) && (usesMap[neighbor].find(modifiedVar) != usesMap[neighbor].end())) {
                affected.insert(neighbor);
            }
            if ((modifiesMap[neighbor].find(modifiedVar) != modifiesMap[neighbor].end()) && (allowedModifiers.find(neighbor.second) != allowedModifiers.end())) {
                dfs(neighbor, nextMap, usesMap, modifiesMap, visited, reachable, affected, modifiedVar, true);
            } else  {
                dfs(neighbor, nextMap, usesMap, modifiesMap, visited, reachable, affected, modifiedVar, false);
            }
        }
    }
}

// DFS traversal explores all paths to all nodes, along with finding all reachable nodes - so can be used to cache Affects queries too
std::unordered_set<std::pair<std::string, EntityType>, PairHash> RelationshipClause::cacheReachableAndAffectedNodes(
    std::pair<std::string, EntityType> startNode,
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> nextMap,
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> usesMap,
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap,
    std::pair<std::string, EntityType> modifiedVar,
    std::shared_ptr<QPSCache>& cache,
    QueryType qType) {
    
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> visited;
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> reachable;
    std::unordered_set<std::pair<std::string, EntityType>, PairHash> affected;

    dfs(startNode, nextMap, usesMap, modifiesMap, visited, reachable, affected, modifiedVar, false);

    cache->cacheReachable(startNode, reachable);
    cache->cacheAffected(startNode, affected);
    
    if (qType == QueryType::NEXTT) {
        return reachable;
    } 
    return affected;
}

std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> RelationshipClause::computeDifference(
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> map,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities1, std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities2) {

            std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> result;
            for (auto& ent1 : entities1) {
                for (auto& ent2 : entities2) {
                    auto mapValues = map[ent1];
                    if (mapValues.find(ent2) == mapValues.end()) {
                        result[ent1].insert(ent2);
                    }
                }
            }
            return result;
    }


// Returns matching subtuples for clause given a relationMap
std::optional<std::vector<std::unordered_map<std::string, std::string>>>
RelationshipClause::getResult(std::unordered_map<std::pair<std::string, EntityType>,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> relationMap, std::shared_ptr<StorageReader> storageReader,
        std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities1, std::unordered_set<std::pair<std::string, EntityType>, PairHash> entities2,
        bool reverse) {

    auto validEntities = std::unordered_set<EntityType> {EntityType::STMT, EntityType::READ, EntityType::PRINT, EntityType::ASSIGN, EntityType::CALL, EntityType::WHILE, EntityType::IF,
                                EntityType::VARIABLE, EntityType::CONSTANT, EntityType::PROCEDURE};

    std::vector<std::unordered_map<std::string, std::string>> subtuples;
    
    // Get inverted map if negated
    if (this->isNegated()) {
        relationMap = computeDifference(relationMap, entities1, entities2);
    }

    for (const auto &it: relationMap) {
        std::pair<std::string, EntityType> parentPair = it.first;
        std::unordered_set<std::pair<std::string, EntityType>, PairHash> childrenPairs = it.second;
        for (const auto &childPair: childrenPairs) {
            std::string keyName;
            std::shared_ptr<EntityType> keyType;
            std::string valueName;
            std::shared_ptr<EntityType> valueType;
            // For reverse, the order is flipped
            if (!reverse) {
                keyName = parentPair.first;
                keyType = std::make_shared<EntityType>(parentPair.second);
                valueName = childPair.first;
                valueType = std::make_shared<EntityType>(childPair.second);
            } else {
                keyName = childPair.first;
                keyType = std::make_shared<EntityType>(childPair.second);
                valueName = parentPair.first;
                valueType = std::make_shared<EntityType>(parentPair.second);
            }
            std::unordered_map<std::string, std::string> tuple;
            std::shared_ptr<Synonym> syn1 = std::dynamic_pointer_cast<Synonym> (arg1);
            std::shared_ptr<Synonym> syn2 = std::dynamic_pointer_cast<Synonym> (arg2);
            bool arg1Match = arg1->matches(keyName, keyType) && validEntities.find(*keyType) != validEntities.end();
            bool arg2Match = arg2->matches(valueName, valueType) && validEntities.find(*valueType) != validEntities.end();
            
            // First check if synonyms match, if they don't continue. If they do, add to tuple
            if (syn1) {
                if (!arg1Match) {
                    continue;
                }
                tuple[syn1->getValue()] = keyName;
            }
            if (syn2) {
                std::string syn2Value = syn2->getValue();
                if ((!arg2Match) || (tuple.find(syn2Value) != tuple.end() && tuple[syn2Value] != valueName)) {
                    continue;
                }
                tuple[syn2Value] = valueName;
            }
            // If both args match, everything matches
            if (arg1Match && arg2Match) {
                // If tuple is empty, clause contains no synonyms -> return nullopt
                if (tuple.empty()) {
                    return std::nullopt;
                } else {
                    subtuples.push_back(tuple);
                }
            }
        }
    }
    return subtuples;
}