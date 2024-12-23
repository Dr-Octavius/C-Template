//
// Created by Michael on 25/2/24.
//

#include "ASTTraversePrint.h"
#include "sp/parser/IParser.h"

// Define the maps
std::unordered_map<int, std::vector<std::string>> TestStatementLineMap;
std::unordered_map<int, std::vector<std::string>> ActualStatementLineMap;

// Function to update the map during traversal
void updateTestLineMap(int lineNumber, const std::string& statement) {
    // Update the map with the line number and statement
    TestStatementLineMap[lineNumber].push_back(statement);
}

// Function to update the map during traversal
void updateActualLineMap(int lineNumber, const std::string& statement) {
    // Update the map with the line number and statement
    ActualStatementLineMap[lineNumber].push_back(statement);
}

// Function to print the contents of the statement line map in uppercase
void printTestStatementLineMap() {
    for (const auto &entry : TestStatementLineMap) {
        // Print the line number
        std::cout << "LINE " << entry.first << " HAS THESE NODES: ";

        // Print each statement in the vector
        std::for_each(entry.second.begin(), entry.second.end(), [](const std::string &statement) {
            // Convert statement to uppercase
            std::string upperStatement = statement;
            std::transform(upperStatement.begin(), upperStatement.end(), upperStatement.begin(), ::toupper);
            std::cout << upperStatement << " ";
        });

        std::cout << std::endl;
    }
}

// Function to print the contents of the statement line map in uppercase
void printActualStatementLineMap() {
    for (const auto &entry : ActualStatementLineMap) {
        // Print the line number
        std::cout << "LINE " << entry.first << " HAS THESE NODES: ";

        // Print each statement in the vector
        std::for_each(entry.second.begin(), entry.second.end(), [](const std::string &statement) {
            // Convert statement to uppercase
            std::string upperStatement = statement;
            std::transform(upperStatement.begin(), upperStatement.end(), upperStatement.begin(), ::toupper);
            std::cout << upperStatement << " ";
        });

        std::cout << std::endl;
    }
}

void printActualStatementLines() {
    // Types to be printed
    std::vector<std::string> typesToPrint = {"PROCEDURE", "ASSIGN", "IF", "WHILE", "READ", "PRINT", "CALL"};

    // Iterate over the map
    for (const auto& entry : ActualStatementLineMap) {
        // Part 1: Extract statements to be printed
        std::vector<std::string> statementsToPrint;
        for (const auto& statement : entry.second) {
            if (std::find(typesToPrint.begin(), typesToPrint.end(), statement) != typesToPrint.end()) {
                statementsToPrint.push_back(statement);
            }
        }

        // Part 2: Print statements or indicate absence
        if (!statementsToPrint.empty()) {
            // Print the line number and statement types
            std::cout << "LINE " << entry.first << ": ";
            for (const auto& statement : statementsToPrint) {
                std::cout << statement << ", ";
            }
            std::cout << std::endl;
        } else {
            // Print the line number and indicate absence of statements
            std::cout << "LINE " << entry.first << " HAS NO STATEMENTS" << std::endl;
        }
    }
}

void printTestStatementLines() {
    // Types to be printed
    std::vector<std::string> typesToPrint = {"PROCEDURE", "ASSIGN", "IF", "WHILE", "READ", "PRINT", "CALL"};

    // Iterate over the map
    for (const auto& entry : TestStatementLineMap) {
        // Part 1: Extract statements to be printed
        std::vector<std::string> statementsToPrint;
        for (const auto& statement : entry.second) {
            if (std::find(typesToPrint.begin(), typesToPrint.end(), statement) != typesToPrint.end()) {
                statementsToPrint.push_back(statement);
            }
        }

        // Part 2: Print statements or indicate absence
        if (!statementsToPrint.empty()) {
            // Print the line number and statement types
            std::cout << "LINE " << entry.first << ": ";
            for (const auto& statement : statementsToPrint) {
                std::cout << statement << ", ";
            }
            std::cout << std::endl;
        } else {
            // Print the line number and indicate absence of statements
            std::cout << "LINE " << entry.first << " HAS NO STATEMENTS" << std::endl;
        }
    }
}

// Pre-order traversal for Program Node
// (will be done when Program Node is here - for now highest level is procedure)
void traversePrintProgram(const std::shared_ptr<ProgramNode>& node, int depth, TraversalPrintType type) {
    std::cout << std::string(depth * 2, ' ') << "|-- " << entityTypeToString(node->getEntityType()) << std::endl;
    for (const auto& child : node->getProcedures()) {
        traversePrintProcedure(child, depth + 1);
    }
}

// Pre-order traversal for Procedure node
void traversePrintProcedure(const std::shared_ptr<ProcedureNode>& node, int depth, TraversalPrintType type) {
    // Update line map based on statement type
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"PROCEDURE");
    } else {
        updateTestLineMap(node->getLineNumber(),"PROCEDURE");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- " << "Procedure: " << node->getName() << std::endl;
    traversePrintStatementList(node->getBody(), depth + 1, type);
}

// Pre-order traversal for WhileNode
void traversePrintWhileStatement(const std::shared_ptr<WhileNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"WHILE");
    } else {
        updateTestLineMap(node->getLineNumber(),"WHILE");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- WhileStatement" << std::endl;

    // TraversePrint condition
    traversePrintCondExpr(node->getCondition(), depth + 1, type);

    // TraversePrint body
    traversePrintStatementList(node->getBody(), depth + 1, type);
}

// Pre-order traversal for ReadNode
void traversePrintReadStatement(const std::shared_ptr<ReadNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"READ");
    } else {
        updateTestLineMap(node->getLineNumber(),"READ");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- ReadStatement" << std::endl;

    // TraversePrint variable
    traversePrintVariable(node->getVariable(), depth + 1, type);
}

// Pre-order traversal for PrintNode
void traversePrintPrintStatement(const std::shared_ptr<PrintNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"PRINT");
    } else {
        updateTestLineMap(node->getLineNumber(),"PRINT");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- PrintStatement" << std::endl;

    // TraversePrint variable
    traversePrintVariable(node->getVariable(), depth + 1, type);
}

// Pre-order traversal for CallNode
void traversePrintCallStatement(const std::shared_ptr<CallNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"CALL");
    } else {
        updateTestLineMap(node->getLineNumber(),"CALL");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- CallStatement: " << node->getName() << std::endl;
}

// Pre-order traversal for IfNode
void traversePrintIfStatement(const std::shared_ptr<IfNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"IF");
    } else {
        updateTestLineMap(node->getLineNumber(),"IF");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- IfStatement" << std::endl;

    // TraversePrint condition
    traversePrintCondExpr(node->getCondition(), depth + 1, type);

    // TraversePrint then branch
    if (node->getThenBranch() != nullptr) {
        if (type == TraversalPrintType::ACTUAL) {
            updateActualLineMap(node->getLineNumber(),"THEN");
        } else {
            updateTestLineMap(node->getLineNumber(),"THEN");
        }

        std::cout << std::string(depth * 2, ' ') << "|-- ThenBranch" << std::endl;
        traversePrintStatementList(node->getThenBranch(), depth + 1, type);
    }

    // TraversePrint else branch if it exists
    if (node->getElseBranch() != nullptr) {
        if (type == TraversalPrintType::ACTUAL) {
            updateActualLineMap(node->getLineNumber(),"ELSE");
        } else {
            updateTestLineMap(node->getLineNumber(),"ELSE");
        }
        std::cout << std::string(depth * 2, ' ') << "|-- ElseBranch" << std::endl;
        traversePrintStatementList(node->getElseBranch(), depth + 1, type);
    }
}

void traversePrintAssignStatement(const std::shared_ptr<AssignNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"ASSIGN");
    } else {
        updateTestLineMap(node->getLineNumber(),"ASSIGN");
    }
    std::cout << std::string(depth * 2, ' ') << "|-- AssignStatement" << std::endl;

    // TraversePrint left child (variable)
    traversePrintVariable(node->getLeftChild(), depth + 1, type);

    // TraversePrint right child (relational factor)
    traversePrintRelFactor(node->getRightChild(), depth + 1, type);
}

// Pre-order traversal for StatementList node
void traversePrintStatementList(const std::shared_ptr<StmtListNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"STMT_LIST");
    } else {
        updateTestLineMap(node->getLineNumber(),"STMT_LIST");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- StatementList" << std::endl;
    for (const auto& child : node->getStatements()) {
        if (auto readStmt = std::dynamic_pointer_cast<ReadNode>(child)) {
            traversePrintReadStatement(readStmt, depth + 1, type);
        } else if (auto printStmt = std::dynamic_pointer_cast<PrintNode>(child)) {
            traversePrintPrintStatement(printStmt, depth + 1, type);
        } else if (auto assignStmt = std::dynamic_pointer_cast<AssignNode>(child)) {
            traversePrintAssignStatement(assignStmt, depth + 1, type);
        } else if (auto whileStmt = std::dynamic_pointer_cast<WhileNode>(child)) {
            traversePrintWhileStatement(whileStmt, depth + 1, type);
        } else if (auto ifStmt = std::dynamic_pointer_cast<IfNode>(child)) {
            traversePrintIfStatement(ifStmt, depth + 1, type);
        }else if (auto callStmt = std::dynamic_pointer_cast<CallNode>(child)) {
            traversePrintCallStatement(callStmt, depth + 1, type);
        }
    }
}

void traversePrintCondExpr(const std::shared_ptr<CondExprNode>& node, int depth, TraversalPrintType type) {
    if (auto relExpr = std::dynamic_pointer_cast<RelExprNode>(node)) {
        traversePrintRelExpr(relExpr, depth, type);
    } else if (auto unaryExpr = std::dynamic_pointer_cast<CondUnaryExprNode>(node)) {
        traversePrintCondUnaryExpr(unaryExpr, depth, type);
    } else if (auto binaryExpr = std::dynamic_pointer_cast<CondBinaryExprNode>(node)) {
        traversePrintCondBinaryExpr(binaryExpr, depth, type);
    }
}

// Pre-order traversal for CondBinaryExprNode
void traversePrintCondBinaryExpr(const std::shared_ptr<CondBinaryExprNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"COND_BINARY_EXPR");
    } else {
        updateTestLineMap(node->getLineNumber(),"COND_BINARY_EXPR");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- CondBinaryExpr: ";

    // Print operator
    switch (node->getOperator()) {
        case CondExprOperator::And:
            std::cout << "&&" << std::endl;
            break;
        case CondExprOperator::Or:
            std::cout << "||" << std::endl;
            break;
        default:
            std::cout << "TRAVERSEPrint_ERR" << std::endl;
            break;
    }

    // TraversePrint left and right children
    traversePrintCondExpr(node->getLeftChild(), depth + 1, type);
    traversePrintCondExpr(node->getRightChild(), depth + 1, type);
}

// Pre-order traversal for CondUnaryExprNode
void traversePrintCondUnaryExpr(const std::shared_ptr<CondUnaryExprNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"COND_UNARY_EXPR");
    } else {
        updateTestLineMap(node->getLineNumber(),"COND_UNARY_EXPR");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- CondUnaryExpr: ";

    // Print operator
    switch (node->getOperator()) {
        case CondExprOperator::Not:
            std::cout << "!" << std::endl;
            break;
        default:
            std::cout << "TRAVERSEPrint_ERR" << std::endl;
            break;
    }

    // TraversePrint operand
    traversePrintCondExpr(node->getOperand(), depth + 1, type);
}

// Pre-order traversal for RelExprNode
void traversePrintRelExpr(const std::shared_ptr<RelExprNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"REL_EXPR");
    } else {
        updateTestLineMap(node->getLineNumber(),"REL_EXPR");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- RelExpr: ";

    // Print operator
    switch (node->getOperator()) {
        case RelExprOperator::Greater:
            std::cout << ">" << std::endl;
            break;
        case RelExprOperator::GreaterEqual:
            std::cout << ">=" << std::endl;
            break;
        case RelExprOperator::Less:
            std::cout << "<" << std::endl;
            break;
        case RelExprOperator::LessEqual:
            std::cout << "<=" << std::endl;
            break;
        case RelExprOperator::Equal:
            std::cout << "==" << std::endl;
            break;
        case RelExprOperator::NotEqual:
            std::cout << "!=" << std::endl;
            break;
        case RelExprOperator::ERROR:
            std::cout << "!=" << std::endl;
            break;
    }

    // TraversePrint left and right children
    traversePrintRelFactor(node->getLeftChild(), depth + 1, type);
    traversePrintRelFactor(node->getRightChild(), depth + 1, type);
}

// Pre-order traversal for RelFactorNode
void traversePrintRelFactor(const std::shared_ptr<RelFactorNode>& node, int depth, TraversalPrintType type) {
    if (auto varNode = std::dynamic_pointer_cast<VariableNode>(node)) {
        traversePrintVariable(varNode, depth, type);
    } else if (auto constNode = std::dynamic_pointer_cast<ConstantNode>(node)) {
        traversePrintConstant(constNode, depth, type);
    } else if (auto exprNode = std::dynamic_pointer_cast<ExprNode>(node)) {
        traversePrintExpr(exprNode, depth, type);
    }
}

// Pre-order traversal for ExprNode
void traversePrintExpr(const std::shared_ptr<ExprNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"EXPR");
    } else {
        updateTestLineMap(node->getLineNumber(),"EXPR");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- Expr: ";

    // Print operator
    switch (node->getOperator()) {
        case ExprOperator::Plus:
            std::cout << "+" << std::endl;
            break;
        case ExprOperator::Minus:
            std::cout << "-" << std::endl;
            break;
        case ExprOperator::Times:
            std::cout << "*" << std::endl;
            break;
        case ExprOperator::Divided:
            std::cout << "/" << std::endl;
            break;
        case ExprOperator::Modulo:
            std::cout << "%" << std::endl;
            break;
        default:
            std::cout << "TRAVERSEPrint_ERR" << std::endl;
            break;
    }

    // TraversePrint left and right children
    traversePrintRelFactor(node->getLeftChild(), depth + 1, type);
    traversePrintRelFactor(node->getRightChild(), depth + 1, type);
}

// Pre-order traversal for ConstantNode
void traversePrintConstant(const std::shared_ptr<ConstantNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"CONSTANT");
    } else {
        updateTestLineMap(node->getLineNumber(),"CONSTANT");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- Constant: " << node->getValue() << std::endl;
}

// Pre-order traversal for VariableNode
void traversePrintVariable(const std::shared_ptr<VariableNode>& node, int depth, TraversalPrintType type) {
    if (type == TraversalPrintType::ACTUAL) {
        updateActualLineMap(node->getLineNumber(),"VARIABLE");
    } else {
        updateTestLineMap(node->getLineNumber(),"VARIABLE");
    }

    std::cout << std::string(depth * 2, ' ') << "|-- Variable: " << node->getName() << std::endl;
}

void printNextMap(const std::map<std::pair<std::string, EntityType>, std::unordered_set<std::pair<std::string, EntityType>, PairHash>>& nextMap) {
    for (const auto& kv : nextMap) {
        // Extracting the key parts
        const auto& key = kv.first;
        std::string keyStr = key.first; // The string part of the key
        std::string keyEntityTypeStr = entityTypeToString(key.second); // The EntityType part of the key, converted to string

        // Start printing the key information
        std::cout << "Key: (" << keyStr << ", " << keyEntityTypeStr << "), Values: {";

        // Iterating over the unordered_set to print all values
        bool isFirst = true;
        for (const auto& val : kv.second) {
            if (!isFirst) {
                std::cout << ", ";
            }
            isFirst = false;
            // Extracting and printing the value parts
            std::string valueStr = val.first; // The string part of the value
            std::string valueEntityTypeStr = entityTypeToString(val.second); // The EntityType part of the value, converted to string
            std::cout << "(" << valueStr << ", " << valueEntityTypeStr << ")";
        }

        // Closing the set of values
        std::cout << "}" << std::endl;
    }
}