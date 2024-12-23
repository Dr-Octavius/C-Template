#include "catch.hpp"
#include "qps/query/QueryClause.h"

TEST_CASE("Valid Calls arguments", "[CallsClause]") {
    // Valid arg1: entity literal, procedure synonym, wildcard
    // Valid arg2: entity literal, procedure synonym, wildcard

    auto entLit = std::make_shared<EntityLiteral>("e");
    auto procSyn = std::make_shared<ProcedureSynonym>("p");
    auto wildcard = std::make_shared<Wildcard>();

    SECTION("entLit, entLit") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(entLit, entLit));
    }

    SECTION("entLit, procSyn") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(entLit, procSyn));
    }

    SECTION("entLit, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(entLit, wildcard));
    }

    SECTION("procSyn, entLit") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(procSyn, entLit));
    }

    SECTION("procSyn, procSyn") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(procSyn, procSyn));
    }

    SECTION("procSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(procSyn, wildcard));
    }

    SECTION("wildcard, entLit") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(wildcard, entLit));
    }

    SECTION("wildcard, procSyn") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(wildcard, procSyn));
    }

    SECTION("wildcard, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<CallsClause>(wildcard, wildcard));
    }
}

TEST_CASE("Invalid Calls arguments", "[CallsClause]") {
    // Invalid arg1: statement literal, statement synonym, non-procedure entity-synonym
    // Invalid arg2: statement literal, statement synonym, non-procedure entity-synonym

    auto entLit = std::make_shared<EntityLiteral>("e");
    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto stmtSyn = std::make_shared<StatementSynonym>("s");
    auto varSyn = std::make_shared<VariableSynonym>("v");

    SECTION("stmtLit, valid arg2", "Invalid arg1 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<CallsClause>(stmtLit, entLit), QuerySyntaxError);
    }

    SECTION("stmtSyn, valid arg2", "Invalid arg1 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<CallsClause>(stmtSyn, entLit), QuerySemanticError);
    }

    SECTION("varSyn, valid arg2", "Invalid arg1 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<CallsClause>(varSyn, entLit), QuerySemanticError);
    }

    SECTION("valid arg1, stmtLit", "Invalid arg2 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<CallsClause>(entLit, stmtLit), QuerySyntaxError);
    }

    SECTION("valid arg1, stmtSyn", "Invalid arg2 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<CallsClause>(entLit, stmtSyn), QuerySemanticError);
    }

    SECTION("valid arg1, varSyn", "Invalid arg2 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<CallsClause>(entLit, varSyn), QuerySemanticError);
    }
}
