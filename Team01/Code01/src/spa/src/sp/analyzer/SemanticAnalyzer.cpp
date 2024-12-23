//
// Created by Michael on 25/3/24.
//

#include "SemanticAnalyzer.h"
#include <stack>

SemanticAnalyzer::SemanticAnalyzer(TraversalContext& ctx) : context(ctx){}

void SemanticAnalyzer::analyze() {
    if (hasDuplicateProcedures()) {
        throw SimpleSemanticError("Duplicate procedures found.");
    }
    if (hasUndefinedProcedures()) {
        throw SimpleSemanticError("Undefined procedures called.");
    }
    if (hasRecursiveAndCyclicCalls()) {
        throw SimpleSemanticError("Recursion or cyclic dependencies found.");
    }
}

bool SemanticAnalyzer::hasDuplicateProcedures() {
    // Check if the size of the set is less than the number of procedures collected
    for (const auto& entry : context.procedureCounts) {
        if (entry.second > 1) {
            return true; // Found a procedure name that appears more than once
        }
    }
    return false; // No duplicates found
}

bool SemanticAnalyzer::hasUndefinedProcedures() {
    // Iterate through the call graph and check if every called procedure exists
    for (const auto& [caller, callees] : context.callGraph) {
        for (const auto& callee : callees) {
            // Check if the callee is found in procedureCounts and its count is greater than 0
            auto found = context.procedureCounts.find(callee);
            if (found == context.procedureCounts.end() || found->second == 0) {
                // Found a call to an undefined procedure
                return true;
            }
        }
    }
    return false;
}

bool SemanticAnalyzer::hasRecursiveAndCyclicCalls() {
    // Clear the visited and recursion stack tracking maps
    visited.clear();
    recStack.clear();

    // Check each procedure for cycles
    for (const auto& procedure : context.procedureCounts) {
        if (!visited[procedure.first] && isCyclicUtil(procedure.first)) {
            return true; // A cycle is found, so return false
        }
    }
    return false; // No cycles found
}

bool SemanticAnalyzer::isCyclicUtil(const std::string& procedure) {
    if (!visited[procedure]) {
        visited[procedure] = true;
        recStack[procedure] = true;

        // Check all adjacent procedures (calls)
        for (const auto& calledProcedure : context.callGraph[procedure]) {
            if (!visited[calledProcedure] && isCyclicUtil(calledProcedure)) {
                return true; // Recursion detected
            } else if (recStack[calledProcedure]) {
                return true; // Back edge detected, indicating a cycle
            }
        }
    }
    recStack[procedure] = false; // Remove the procedure from the recursion stack
    return false; // No cycles found from this node
}