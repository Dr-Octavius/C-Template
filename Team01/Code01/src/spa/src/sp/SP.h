#ifndef SPA_SP_H
#define SPA_SP_H

#include "tokenizer/CompositeLexer.h"
#include "tokenizer/Token.h"
#include <string>
#include "sp/parser/MainParser.h"
#include "sp/ast/nodes/ASTNode.h"
#include "sp/analyzer/SemanticAnalyzer.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include "sp/design_extractor//DesignExtractor.h"
#include "sp/design_extractor/visitors/FollowsVisitor.h"
#include "sp/design_extractor/visitors/ParentVisitor.h"
#include "sp/design_extractor/visitors/UsesVisitor.h"
#include "sp/design_extractor/visitors/ModifiesVisitor.h"
#include "sp/design_extractor/visitors/EntityVisitor.h"
#include "sp/design_extractor/visitors/PatternVisitor.h"
#include "sp/pkb_integration/StorageWriter.h"
#include "sp/pkb_integration/storage/RelationshipStorage.h"
#include "common/Utils.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include "sp/design_extractor/visitors/CallsVisitor.h"

class SP {
private:
    std::shared_ptr<StorageManager> storageManager;
public:
    explicit SP(std::shared_ptr<StorageManager> storageManager);

    void initiateProcessing(std::string &input);
};

#endif //SPA_SP_H
