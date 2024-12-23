//
// Created by Michael on 26/3/24.
//

#include "CompressedCFGNode.h"

// Constructor implementation
CompressedCFGNode::CompressedCFGNode(EntityType entityType)
        : entityType(entityType) {}

// Method implementation to add a successor
void CompressedCFGNode::addSuccessor(const std::shared_ptr<CompressedCFGNode> &node) {
    successors.push_back(node);
}

// Method implementation to add a successor
EntityType CompressedCFGNode::getEntityType() {
    return entityType;
}