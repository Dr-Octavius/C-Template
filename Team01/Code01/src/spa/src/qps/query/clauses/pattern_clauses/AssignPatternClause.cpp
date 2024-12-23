#include "../../QueryClause.h"
#include "sp/ast/nodes/expressions/RelFactorNode.h"
#include "sp/ast/nodes/expressions/basic/ExprNode.h"
#include "sp/ast/nodes/expressions/variables/ConstantNode.h"
#include "sp/ast/nodes/expressions/variables/VariableNode.h"
#include <regex>
#include <memory>

AssignPatternClause::AssignPatternClause(std::shared_ptr<Synonym> syn, std::shared_ptr<DesignEntity> lhs,
                                         std::shared_ptr<DesignEntity> rhs)
        : PatternClause(syn), lhs(lhs), rhs(rhs) {
    AssignPatternClause::setArgTypes();

    addSynonym(syn);
    addSynonym(lhs);
}

void AssignPatternClause::setArgTypes() {
    if (!std::dynamic_pointer_cast<AssignSynonym>(syn)) {
        throw QuerySyntaxError("AssignPatternClause: Invalid statement synonym: " + syn->getValue());
    }

    if (std::dynamic_pointer_cast<VariableSynonym>(lhs) ||
        std::dynamic_pointer_cast<EntityLiteral>(lhs) ||
        std::dynamic_pointer_cast<Wildcard>(lhs)) {
        // do nothing
    } else if (std::dynamic_pointer_cast<Literal>(lhs)) {
        throw QuerySyntaxError("AssignPatternClause: Invalid lhs: " + lhs->getValue());
    } else {
        throw QuerySemanticError("AssignPatternClause::setArgTypes: invalid lhs type");
    }
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>>
AssignPatternClause::evaluate(std::shared_ptr<StorageReader> storageReader, std::shared_ptr<QPSCache>& cache) {
    std::unordered_map<std::pair<std::string, EntityType>, std::pair<std::string, std::shared_ptr<RelFactorNode>>, PairHash> patternMap;
    std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap;
    std::unordered_set<std::string> variableMap;

    patternMap = storageReader->getAllAssignPatternEntries();
    modifiesMap = storageReader->getModifiesMap();
    variableMap = storageReader->getEntitiesFromPkb(EntityType::VARIABLE);

    return processPatternMap(patternMap, modifiesMap, variableMap);
}

bool areExprNodesEquivalent(std::shared_ptr<RelFactorNode> node1, std::shared_ptr<RelFactorNode> node2) {
    if (!node1 && !node2) return true; // Both nodes are null
    if (!node1 || !node2) return false; // One node is null, the other isn't

    // Attempt to cast both nodes to ExprNode
    auto exprNode1 = std::dynamic_pointer_cast<ExprNode>(node1);
    auto exprNode2 = std::dynamic_pointer_cast<ExprNode>(node2);

    // Attempt to cast both nodes to ConstantNode
    auto constNode1 = std::dynamic_pointer_cast<ConstantNode>(node1);
    auto constNode2 = std::dynamic_pointer_cast<ConstantNode>(node2);

    // Attempt to cast both nodes to VariableNode
    auto varNode1 = std::dynamic_pointer_cast<VariableNode>(node1);
    auto varNode2 = std::dynamic_pointer_cast<VariableNode>(node2);

    // Both are ExprNode
    if (exprNode1 && exprNode2) {
        if (exprNode1->getOperator() != exprNode2->getOperator()) return false; // Different operators
        // Recursively compare left and right children
        return areExprNodesEquivalent(exprNode1->getLeftChild(), exprNode2->getLeftChild()) &&
               areExprNodesEquivalent(exprNode1->getRightChild(), exprNode2->getRightChild());
    }

    // Both are ConstantNode
    if (constNode1 && constNode2) {
        return constNode1->getValue() == constNode2->getValue(); // Compare constant values
    }

    // Both are VariableNode
    if (varNode1 && varNode2) {
        return varNode1->getName() == varNode2->getName(); // Compare variable names
    }

    // Nodes are of different types or comparison criteria not met
    return false;
}

bool isSubtreePresent(const std::shared_ptr<RelFactorNode> &reference, const std::shared_ptr<RelFactorNode> &subtree) {
    if (!subtree) return true; // An empty subtree is considered to be always present.
    if (!reference) return false; // An empty reference tree cannot contain a subtree.

    if (areExprNodesEquivalent(reference, subtree)) {
        return true;
    }

    // If reference is an ExprNode, check its children.
    auto exprNode = std::dynamic_pointer_cast<ExprNode>(reference);
    if (exprNode) {
        return isSubtreePresent(exprNode->getLeftChild(), subtree) ||
               isSubtreePresent(exprNode->getRightChild(), subtree);
    }

    return false;
}

std::string buildExprString(const std::shared_ptr<RelFactorNode> &node) {
    if (!node) return "";

    // Attempt dynamic casts to check the type of node
    auto exprNode = std::dynamic_pointer_cast<ExprNode>(node);
    auto constNode = std::dynamic_pointer_cast<ConstantNode>(node);
    auto varNode = std::dynamic_pointer_cast<VariableNode>(node);

    // Handle ExprNode: Recursively build string for left and right children, placing operator between
    if (exprNode) {
        std::string op;
        switch (exprNode->getOperator()) {
            case ExprOperator::Plus:
                op = "+";
                break;
            case ExprOperator::Minus:
                op = "-";
                break;
            case ExprOperator::Times:
                op = "*";
                break;
            case ExprOperator::Divided:
                op = "/";
                break;
            case ExprOperator::Modulo:
                op = "%";
                break;
            default:
                throw std::invalid_argument("Unsupported operator");
        }
        return buildExprString(exprNode->getLeftChild()) + op + buildExprString(exprNode->getRightChild());
    }

    // Handle ConstantNode: Return its value
    if (constNode) {
        return std::to_string(constNode->getValue());
    }

    // Handle VariableNode: Return its name
    if (varNode) {
        return varNode->getName();
    }

    return ""; // In case of an unrecognized node
}

std::string removeBrackets(const std::string &str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
                [](char c){ return ((c != '(') && (c != ')')); });
    return result;
}

void collectSubtrees(const std::shared_ptr<RelFactorNode> &node, std::vector<std::string> &subtrees) {
    if (!node) return;

    // Generate and add the current node's subtree string to the vector
    subtrees.push_back(buildExprString(node));

    // Attempt dynamic casts to check if the node is an ExprNode
    auto exprNode = std::dynamic_pointer_cast<ExprNode>(node);

    // If it's an ExprNode, recursively collect subtrees from its children
    if (exprNode) {
        collectSubtrees(exprNode->getLeftChild(), subtrees);
        collectSubtrees(exprNode->getRightChild(), subtrees);
    }
}

std::optional<std::vector<std::unordered_map<std::string, std::string>>> AssignPatternClause::processPatternMap(
        std::unordered_map<std::pair<std::string, EntityType>, std::pair<std::string, std::shared_ptr<RelFactorNode>>, PairHash> patternMap,
        std::unordered_map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>, PairHash> modifiesMap,
        std::unordered_set<std::string> variableMap
) {
    std::vector<std::unordered_map<std::string, std::string>> result;
    result.reserve(patternMap.size());

    // 1. pattern a(_, _)
    // Return a: for all a in assignments
    if (std::dynamic_pointer_cast<Wildcard>(lhs) && std::dynamic_pointer_cast<Wildcard>(rhs)) {
        // NOT CASE - Return empty table
        if (this->isNegated()) {
            return result;
        }
        // For each key-value pair in patternMap, we want to form a tuple of key's string to value's pair's string
        for (const auto &kvp: patternMap) {
            std::unordered_map<std::string, std::string> intermediate;
            intermediate[syn->getValue()] = kvp.first.first;
			result.push_back(intermediate);
        }
        return result;
    }

    // 2. pattern a (_, "x+1")
    // Return a: for all a in assignments and "x+1" is identical to pattern tree
    // of this a, we have to check each ExprNode in the patternMap and compare it with rhs, exactly
    if (std::dynamic_pointer_cast<Wildcard>(lhs) && std::dynamic_pointer_cast<ExactExpression>(rhs)) {
        std::string rhsExprString = rhs->getValue();
        rhsExprString = removeBrackets(rhsExprString);
        for (const auto &kvp: patternMap) {
            std::string patternExprString = buildExprString(kvp.second.second);
            // NOT CASE - Add only if doesn't match
            if ((!this->isNegated() && (patternExprString == rhsExprString)) || (this->isNegated() && (patternExprString != rhsExprString))) {
                std::unordered_map<std::string, std::string> intermediate;
                intermediate[syn->getValue()] = kvp.first.first;
                result.push_back(intermediate);
            }
        }
        return result;
    }

    // 3. pattern a (_, _"x+1"_)
    // Return a: for all a in assignments and "x+1" is subtree of pattern tree of
    // this a, similar as above, but we have to check if rhs is a subtree of the patternMap's ExprNode
    if (std::dynamic_pointer_cast<Wildcard>(lhs) && std::dynamic_pointer_cast<PartialExpression>(rhs)) {
        std::string rhsExprString = rhs->getValue();
        rhsExprString = removeBrackets(rhsExprString);
        for (const auto &kvp: patternMap) {
            std::vector<std::string> substrings;
            collectSubtrees(kvp.second.second, substrings);
            // NOT CASE - Add only if doesn't match
            if ((!this->isNegated() && (std::find(substrings.begin(), substrings.end(), rhsExprString) != substrings.end())) || 
                    (this->isNegated() && (std::find(substrings.begin(), substrings.end(), rhsExprString) == substrings.end()))) {
                std::unordered_map<std::string, std::string> intermediate;
                intermediate[syn->getValue()] = kvp.first.first;
                result.push_back(intermediate);
            }
        }
        return result;
    }

    // 4. pattern a("x", _)
    // Return empty one column table if "x" not in variable 
    // Return a: for all a in Modifies(a, "x") that are also in assignments
    if (std::dynamic_pointer_cast<EntityLiteral>(lhs) && std::dynamic_pointer_cast<Wildcard>(rhs)) {
        if (variableMap.find(lhs->getValue()) == variableMap.end()) {
            return result;
        }
        // Go through modifies map, if key is assignment, check if "x" is in assignments
        for (const auto& modifiesEntry : modifiesMap) {
            if (modifiesEntry.first.second != EntityType::ASSIGN) {
                continue;
            }
            // NOT CASE - Add if "x" not found in modifies set
            if ((!this->isNegated() && (modifiesEntry.second.find({ lhs->getValue(), EntityType::VARIABLE }) != modifiesEntry.second.end())) ||
                    (this->isNegated() && (modifiesEntry.second.find({ lhs->getValue(), EntityType::VARIABLE }) == modifiesEntry.second.end()))) {
                std::unordered_map<std::string, std::string> intermediate;
                intermediate[syn->getValue()] = modifiesEntry.first.first;
                result.push_back(intermediate);
            }
		}
        return result;
    }

    // 5. pattern a("x", "x+1")
    // Return empty one column table if "x" not in variable list
    // Return a: for all a in Modifies(a, "x") that are also in assignments and
    // "x+1" is identical to pattern tree of this a
    if (std::dynamic_pointer_cast<EntityLiteral>(lhs) && std::dynamic_pointer_cast<ExactExpression>(rhs)) {
        if (!this->isNegated() && variableMap.find(lhs->getValue()) == variableMap.end()) {
            return result;
        }
        std::string rhsExprString = rhs->getValue();
        rhsExprString = removeBrackets(rhsExprString);
        for (const auto& patternEntry : patternMap) {
            std::string patternExprString = buildExprString(patternEntry.second.second);
            bool matches = (patternEntry.second.first == lhs->getValue()) && (patternExprString == rhsExprString);
            // NOT CASE - Only add when any of the conditions don't match
            if ((!this->isNegated() && matches) || (this->isNegated() && !matches)) {
                std::unordered_map<std::string, std::string> intermediate;
                intermediate[syn->getValue()] = patternEntry.first.first;
                result.push_back(intermediate);
            }
        }
        return result;
    }

    // 6. pattern a("x", _"x+1"_)
    // Return empty one column table if "x" not in variable list
    // Return a: for all a in Modifies(a, "x") that are also in assignments and
    // "x+1" is subtree of pattern tree of this a
    if (std::dynamic_pointer_cast<EntityLiteral>(lhs) && std::dynamic_pointer_cast<PartialExpression>(rhs)) {
        if (!this->isNegated() && variableMap.find(lhs->getValue()) == variableMap.end()) {
            return result;
        }
        std::string rhsExprString = rhs->getValue();
        rhsExprString = removeBrackets(rhsExprString);
        for (const auto& patternEntry : patternMap) {
            std::vector<std::string> substrings;
            collectSubtrees(patternEntry.second.second, substrings);
            bool matches = (patternEntry.second.first == lhs->getValue()) && (std::find(substrings.begin(), substrings.end(), rhsExprString) != substrings.end());
            // NOT CASE - Only add when any of the conditions don't match
            if ((!this->isNegated() && matches) || (this->isNegated() && !matches)) {
                std::unordered_map<std::string, std::string> intermediate;
                intermediate[syn->getValue()] = patternEntry.first.first;
                result.push_back(intermediate);
            }
        }
        return result;
    }

    // 7. pattern a(v, _)
    // Return a, v: for all a in Modifies(a, v) that are also in assignments
    if (std::dynamic_pointer_cast<VariableSynonym>(lhs) && std::dynamic_pointer_cast<Wildcard>(rhs)) {
        // Go through modifies map, if key is assignment, check if any variables is in assignments
        for (const auto& modifiesEntry : modifiesMap) {
            if (modifiesEntry.first.second != EntityType::ASSIGN) {
                continue;
            }
            for (auto& var : variableMap) {
                auto modifiedVars = modifiesEntry.second;
                // NOT CASE - Only add when variable not modified for assignment
                if ((!this->isNegated() && (modifiedVars.find(std::make_pair(var, EntityType::VARIABLE)) != modifiedVars.end())) ||
                    (this->isNegated() && (modifiedVars.find(std::make_pair(var, EntityType::VARIABLE)) == modifiedVars.end()))) {
                        std::unordered_map<std::string, std::string> intermediate;
                        intermediate[syn->getValue()] = modifiesEntry.first.first;
                        intermediate[lhs->getValue()] = var;
                        result.push_back(intermediate);
                }
            }
        }
        return result;
    }

    // 8. pattern a(v, "x+1")
    // Return a, v: for all a in Modifies(a, v) that are also in assignments and
    // "x+1" is identical to pattern tree of this a
    if (std::dynamic_pointer_cast<VariableSynonym>(lhs) && std::dynamic_pointer_cast<ExactExpression>(rhs)) {
        std::string rhsExprString = rhs->getValue();
        rhsExprString = removeBrackets(rhsExprString);
        for (const auto& patternEntry : patternMap) {
            std::string rhsString = buildExprString(patternEntry.second.second);
            std::string assignLiteral = patternEntry.first.first;
            std::string lhsLiteral = patternEntry.second.first;
            for (auto& v : variableMap) {
                bool matches = (rhsString == rhsExprString) && (v == lhsLiteral);
                if ((!this->isNegated() && matches) || this->isNegated() && !matches) {
                    std::unordered_map<std::string, std::string> intermediate;
                    intermediate[syn->getValue()] = assignLiteral;
                    intermediate[lhs->getValue()] = v;
                    result.push_back(intermediate);
                }
            }
        }
        return result;
    }

    // 9. pattern a(v, _"x+1"_)
    // Return a, v: for all a in Modifies(a, v) that are also in assignments and
    // "x+1" is subtree of pattern tree of this a
    if (std::dynamic_pointer_cast<VariableSynonym>(lhs) && std::dynamic_pointer_cast<PartialExpression>(rhs)) {
        std::string rhsExprString = rhs->getValue();
        rhsExprString = removeBrackets(rhsExprString);
        for (const auto& patternEntry : patternMap) {
            std::vector<std::string> substrings;
            collectSubtrees(patternEntry.second.second, substrings);
            std::string assignLiteral = patternEntry.first.first;
            std::string lhsLiteral = patternEntry.second.first;
            for (auto& v : variableMap) {
                bool matches = (std::find(substrings.begin(), substrings.end(), rhsExprString) != substrings.end()) && (v == lhsLiteral);
                if ((!this->isNegated() && matches) || this->isNegated() && !matches) {
                    std::unordered_map<std::string, std::string> intermediate;
                    intermediate[syn->getValue()] = assignLiteral;
                    intermediate[lhs->getValue()] = v;
                    result.push_back(intermediate);
                }
            }
        }
        return result;
    }

    return result;
}