//
// Created by Michael on 25/3/24.
//

#ifndef SPA_TRAVERSALCONTEXT_H
#define SPA_TRAVERSALCONTEXT_H

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <stack>
#include "sp/ast/nodes/ASTNode.h"
#include "sp/ast/nodes/statements/ProcedureNode.h" // Include the header that defines ASTNode and its subclasses
#include "sp/ast/nodes/statements/WhileNode.h" // Include the header that defines ASTNode and its subclasses
#include "sp/ast/nodes/statements/IfNode.h" // Include the header that defines ASTNode and its subclasses
#include "sp/ast/nodes/controlFlow/StmtListNode.h" // Include the header that defines ASTNode and its subclasses
#include "sp/errors/SimpleSyntaxError.h" // Include the header that defines ASTNode and its subclasses
#include "sp/ast/nodes/statements/ProcedureNode.h"
#include "sp/cfg/CFGNode.h"
#include "sp/cfg/CompressedCFGNode.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include "sp/pkb_integration/StorageWriter.h"
#include "sp/pkb_integration/storage/RelationshipStorage.h"

class TraversalContext {
private:
    std::string currentProcedure;
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> nextMap;
public:
    TraversalContext() = default;
    std::vector<std::shared_ptr<CFGNode>> cfgList;
    std::vector<std::shared_ptr<CompressedCFGNode>> compressedCfgList;
    std::unordered_map<std::string, int> procedureCounts;
    std::unordered_map<std::string, std::vector<std::string>> callGraph;
    // Utility methods for handling annotations calls
    void annotateProcedureCounts(const std::string& name);
    void annotateCallGraph(const std::string& caller, const std::string& callee);
    // Sets the current procedure being traversed
    void setCurrentProcedure(const std::string& name);
    // Gets the leave CFGNode
    std::shared_ptr<CFGNode> getNextMap(std::shared_ptr<CFGNode> node);
    // Gets the current procedure being traversed
    const std::string& getCurrentProcedure();
    // Gets the current procedure being traversed
    std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>> getNextMap();
    // links 2 CFG Nodes together
    void linkCFGNodes(std::shared_ptr<CFGNode> parent, std::shared_ptr<CFGNode> child);
    // links 2 Compressed CFG Nodes together
    void linkCompressedCFGNodes(std::shared_ptr<CompressedCFGNode> parent, std::shared_ptr<CompressedCFGNode> child);
    // insert map entry
    void insertToNextMap(std::pair<std::string, EntityType> key, std::pair<std::string, EntityType> value);
    // Gets the leaf CFGNode
    std::vector<std::shared_ptr<CFGNode>> getDeepestNode(std::shared_ptr<CFGNode> node,std::vector<std::shared_ptr<CFGNode>> lastNodes);
    // PKB Integration
    void insertRelationshipRecords(
            const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& records,
            void (RelationshipStorage::*insertFunction)(const std::pair<std::string, EntityType>&, const std::pair<std::string, EntityType>&),
            const std::shared_ptr<RelationshipStorage>& relationshipStorage
    );
    void writeToPkb(std::shared_ptr<StorageManager> pkbStorageManager);
};


#endif //SPA_TRAVERSALCONTEXT_H
