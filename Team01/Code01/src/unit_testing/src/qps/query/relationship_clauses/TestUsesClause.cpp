#include "catch.hpp"
#include "qps/query/QueryClause.h"

TEST_CASE("Valid Uses arguments", "[UsesClause]") {
    // Valid arg1: statement literal, statement synonym, entity literal, procedure synonym
    // Valid arg2: entity literal, variable synonym, wildcard

    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto entLit = std::make_shared<EntityLiteral>("x");
    auto stmtSyn = std::make_shared<StatementSynonym>("s");
    auto subStmtSyn = std::make_shared<WhileSynonym>("ss");
    auto varSyn = std::make_shared<VariableSynonym>("v");
    auto procSyn = std::make_shared<ProcedureSynonym>("p");
    auto wildcard = std::make_shared<Wildcard>();

    SECTION("stmtLit, entLit") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(stmtLit, entLit));
    }

    SECTION("stmtLit, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(stmtLit, varSyn));
    }

    SECTION("stmtLit, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(stmtLit, wildcard));
    }

    SECTION("stmtSyn, entLit") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(stmtSyn, entLit));
    }

    SECTION("stmtSyn, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(stmtSyn, varSyn));
    }

    SECTION("stmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(stmtSyn, wildcard));
    }

    SECTION("subStmtSyn, entLit") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(subStmtSyn, entLit));
    }

    SECTION("subStmtSyn, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(subStmtSyn, varSyn));
    }

    SECTION("subStmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(subStmtSyn, wildcard));
    }

    SECTION("entLit, entLit") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(entLit, entLit));
    }

    SECTION("entLit, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(entLit, varSyn));
    }

    SECTION("entLit, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(entLit, wildcard));
    }

    SECTION("procSyn, entLit") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(procSyn, entLit));
    }

    SECTION("procSyn, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(procSyn, varSyn));
    }

    SECTION("procSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<UsesClause>(procSyn, wildcard));
    }
}

TEST_CASE("Invalid Uses arguments", "[UsesClause]") {
    // Invalid arg1: wildcard, non-procedure entity-synonym
    // Invalid arg2: non-variable synonym, non-entity literal

    auto wildcard = std::make_shared<Wildcard>();
    auto constSyn = std::make_shared<ConstantSynonym>("s");
    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto entLit = std::make_shared<EntityLiteral>("x");

    SECTION("wildcard, valid arg2", "Ambiguous arg1 wildcard") {
        REQUIRE_THROWS_AS(std::make_shared<UsesClause>(wildcard, entLit), QuerySemanticError);
    }

    SECTION("constSyn, valid arg2", "Invalid arg1 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<UsesClause>(constSyn, entLit), QuerySemanticError);
    }

    SECTION("valid arg1, nonVarSyn", "Invalid arg2 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<UsesClause>(entLit, constSyn), QuerySemanticError);
    }

    SECTION("valid arg1, stmtLit", "Invalid arg2 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<UsesClause>(entLit, stmtLit), QuerySyntaxError);
    }
}
