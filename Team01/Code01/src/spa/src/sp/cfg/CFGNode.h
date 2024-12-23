//
// Created by Michael on 22/3/24.
//

#ifndef SPA_CFGNODE_H
#define SPA_CFGNODE_H

#include <memory>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <unordered_set>
#include "common/Utils.h"

class CFGNode {
public:
    std::string lineNumberString;
    EntityType entityType;
    std::pair<std::string, EntityType> key;
    std::vector<std::shared_ptr<CFGNode>> successors;
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> nextMap;

    // Constructor
    CFGNode(int lineNumber, EntityType entityType);

    // Method to add a successor to the CFG node
    void addSuccessor(std::shared_ptr<CFGNode>& node);
    // Method to get CFG node type
    EntityType getEntityType();
};


#endif //SPA_CFGNODE_H
