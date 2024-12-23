#include "catch.hpp"
#include "qps/query/QueryClause.h"

TEST_CASE("Valid Parent arguments", "[ParentClause]") {
    // Valid arg1: statement literal, statement synonym, wildcard
    // Valid arg2: statement literal, statement synonym, wildcard

    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto stmtSyn = std::make_shared<StatementSynonym>("s");
    auto subStmtSyn = std::make_shared<WhileSynonym>("ss");
    auto wildcard = std::make_shared<Wildcard>();

    SECTION("stmtLit, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtLit, stmtLit));
    }

    SECTION("stmtLit, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtLit, stmtSyn));
    }

    SECTION("stmtLit, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtLit, subStmtSyn));
    }

    SECTION("stmtLit, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtLit, wildcard));
    }

    SECTION("stmtSyn, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtSyn, stmtLit));
    }

    SECTION("stmtSyn, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtSyn, stmtSyn));
    }

    SECTION("stmtSyn, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtSyn, subStmtSyn));
    }

    SECTION("stmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(stmtSyn, wildcard));
    }

    SECTION("subStmtSyn, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(subStmtSyn, stmtLit));
    }

    SECTION("subStmtSyn, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(subStmtSyn, stmtSyn));
    }

    SECTION("subStmtSyn, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(subStmtSyn, subStmtSyn));
    }

    SECTION("subStmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(subStmtSyn, wildcard));
    }

    SECTION("wildcard, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(wildcard, stmtLit));
    }

    SECTION("wildcard, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(wildcard, stmtSyn));
    }

    SECTION("wildcard, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(wildcard, subStmtSyn));
    }

    SECTION("wildcard, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ParentClause>(wildcard, wildcard));
    }
}

TEST_CASE("Invalid Parent arguments", "[ParentClause]") {
    // Invalid arg1: entity literal, entity synonym
    // Invalid arg2: entity literal, entity synonym

    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto entLit = std::make_shared<EntityLiteral>("x");
    auto entSyn = std::make_shared<VariableSynonym>("v");

    SECTION("entLit, valid arg2", "Invalid arg1 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<ParentClause>(entLit, stmtLit), QuerySyntaxError);
    }

    SECTION("entSyn, valid arg2", "Invalid arg1 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<ParentClause>(entSyn, stmtLit), QuerySemanticError);
    }

    SECTION("valid arg1, entLit", "Invalid arg2 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<ParentClause>(stmtLit, entLit), QuerySyntaxError);
    }

    SECTION("valid arg1, entSyn", "Invalid arg2 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<ParentClause>(stmtLit, entSyn), QuerySemanticError);
    }
}
