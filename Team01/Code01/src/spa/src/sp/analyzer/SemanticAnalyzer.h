//
// Created by Michael on 25/3/24.
//

#ifndef SPA_SEMANTICANALYZER_H
#define SPA_SEMANTICANALYZER_H

#include "sp/errors/SimpleSemanticError.h"
#include "sp/astTraversal/TraversalContext.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>

class SemanticAnalyzer {
public:
    SemanticAnalyzer(TraversalContext& ctx);
    void analyze(); // Add this method
    bool hasDuplicateProcedures();
    bool hasUndefinedProcedures();
    bool hasRecursiveAndCyclicCalls();

private:
    TraversalContext context;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, bool> recStack;

    bool isCyclicUtil(const std::string& procedure);
    // ... other private members or methods
};


#endif //SPA_SEMANTICANALYZER_H
