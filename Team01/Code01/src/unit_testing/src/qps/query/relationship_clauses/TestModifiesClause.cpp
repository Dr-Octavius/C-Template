#include "catch.hpp"
#include "qps/query/QueryClause.h"

TEST_CASE("Valid Modifies arguments", "[ModifiesClause]") {
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
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(stmtLit, entLit));
    }

    SECTION("stmtLit, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(stmtLit, varSyn));
    }

    SECTION("stmtLit, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(stmtLit, wildcard));
    }

    SECTION("stmtSyn, entLit") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(stmtSyn, entLit));
    }

    SECTION("stmtSyn, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(stmtSyn, varSyn));
    }

    SECTION("stmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(stmtSyn, wildcard));
    }

    SECTION("subStmtSyn, entLit") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(subStmtSyn, entLit));
    }

    SECTION("subStmtSyn, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(subStmtSyn, varSyn));
    }

    SECTION("subStmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(subStmtSyn, wildcard));
    }

    SECTION("entLit, entLit") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(entLit, entLit));
    }

    SECTION("entLit, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(entLit, varSyn));
    }

    SECTION("entLit, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(entLit, wildcard));
    }

    SECTION("procSyn, entLit") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(procSyn, entLit));
    }

    SECTION("procSyn, varSyn") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(procSyn, varSyn));
    }

    SECTION("procSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<ModifiesClause>(procSyn, wildcard));
    }
}

TEST_CASE("Invalid Modifies arguments", "[ModifiesClause]") {
    // Invalid arg1: wildcard, non-procedure entity-synonym
    // Invalid arg2: non-variable synonym, non-entity literal

    auto wildcard = std::make_shared<Wildcard>();
    auto constSyn = std::make_shared<ConstantSynonym>("s");
    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto entLit = std::make_shared<EntityLiteral>("x");

    SECTION("wildcard, valid arg2", "Ambiguous arg1 wildcard") {
        REQUIRE_THROWS_AS(std::make_shared<ModifiesClause>(wildcard, entLit), QuerySemanticError);
    }

    SECTION("constSyn, valid arg2", "Invalid arg1 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<ModifiesClause>(constSyn, entLit), QuerySemanticError);
    }

    SECTION("valid arg1, nonVarSyn", "Invalid arg2 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<ModifiesClause>(entLit, constSyn), QuerySemanticError);
    }

    SECTION("valid arg1, stmtLit", "Invalid arg2 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<ModifiesClause>(entLit, stmtLit), QuerySyntaxError);
    }
}
