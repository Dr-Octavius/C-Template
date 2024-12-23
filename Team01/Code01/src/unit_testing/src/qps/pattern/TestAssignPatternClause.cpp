#include "../../include/catch.hpp"
#include "common/MockPatternsFactory.h"
#include "qps/query/clauses/pattern_clauses/AssignPatternClause.cpp"
#include <memory>

MockPatternsFactory factory;

std::shared_ptr<ExprNode> createExprNode(const std::string& op, std::shared_ptr<RelFactorNode> left, std::shared_ptr<RelFactorNode> right) {
    ExprOperator exprOp;
    if (op == "+") exprOp = ExprOperator::Plus;
    else if (op == "-") exprOp = ExprOperator::Minus;
    else if (op == "*") exprOp = ExprOperator::Times;
    else if (op == "/") exprOp = ExprOperator::Divided;
    else throw std::invalid_argument("Unsupported operator");

    return std::make_shared<ExprNode>(left, right, exprOp, 0);
}

TEST_CASE("ExprNode String Representation", "[buildExprString]") {
    auto expr = factory.createFirstExpression();

    REQUIRE(buildExprString(expr) == "v+x*y+z*t");
}

TEST_CASE("Collecting Subtrees", "[collectSubtrees]") {
    auto expr = factory.createFirstExpression();

    std::vector<std::string> subtrees;
    collectSubtrees(expr, subtrees);

    REQUIRE(subtrees.size() == 9);
}


TEST_CASE("Subtree Presence in Tree", "[isSubtreePresent]") {
    auto mainTree = factory.createFirstExpression();
    auto subtree = createExprNode("*", std::make_shared<VariableNode>("x", 0), std::make_shared<VariableNode>("y", 0));

    REQUIRE(isSubtreePresent(mainTree, subtree));
}

TEST_CASE("Expression Node Equivalence", "[areExprNodesEquivalent]") {
    auto node1 = factory.createFirstExpression();
    auto node2 = factory.createFirstExpression();

    REQUIRE(areExprNodesEquivalent(node1, node2));
}

TEST_CASE("Pattern a(_, _) - All Assignments", "[AssignPatternClause]") {
    
}

TEST_CASE("Pattern a(_, 'x+1') - Exact Match", "[AssignPatternClause]") {
 
}

TEST_CASE("Pattern a(_, _'x+1'_) - Partial Match", "[AssignPatternClause]") {
	
}

TEST_CASE("Pattern a('x', _) - Variable Literal with Wildcard", "[AssignPatternClause]") {
	
}

TEST_CASE("Pattern a('x', 'x+1') - Variable Literal with Exact Match", "[AssignPatternClause]") {
	
}

TEST_CASE("Pattern a('x', _'x+1'_) - Variable Literal with Partial Match", "[AssignPatternClause]") {
	
}

TEST_CASE("Pattern a(v, _) - Variable Synonym with wildcard", "[AssignPatternClause]") {
	
}

TEST_CASE("Pattern a(v, 'x+1') - Variable Synonym with Exact Match", "[AssignPatternClause]") {
	
}

TEST_CASE("Pattern a(v, _'x+1'_) - Variable Synonym with Partial Match", "[AssignPatternClause]") {
	
}

