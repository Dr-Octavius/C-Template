//
// Created by Michael on 22/3/24.
//

#include "CFGNode.h"

// Constructor implementation
CFGNode::CFGNode(int lineNumber, EntityType entityType)
        : entityType(entityType) {
    key = {std::to_string(lineNumber),entityType};
    lineNumberString = std::to_string(lineNumber);
}

// Method implementation to add a successor
void CFGNode::addSuccessor(std::shared_ptr<CFGNode>& node) {
    successors.push_back(node);
    nextMap[key].insert(node->key);
}

// Method implementation to add a successor
EntityType CFGNode::getEntityType() {
    return entityType;
}