//
// Created by Michael on 26/3/24.
//

#ifndef SPA_COMPRESSEDCFGNODE_H
#define SPA_COMPRESSEDCFGNODE_H

#include <memory>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <unordered_set>
#include "common/Utils.h"

class CompressedCFGNode {
public:
    std::vector<std::string> lineNumbers;
    std::vector<std::shared_ptr<CompressedCFGNode>> successors;
    EntityType entityType;

    // Constructor
    CompressedCFGNode(EntityType entityType);

    // Method to add a successor to the CFG node
    void addSuccessor(const std::shared_ptr<CompressedCFGNode>& node);
    // Method to get CFG node type
    EntityType getEntityType();
};


#endif //SPA_COMPRESSEDCFGNODE_H
