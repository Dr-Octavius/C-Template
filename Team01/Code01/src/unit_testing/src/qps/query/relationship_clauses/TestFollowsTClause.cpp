#include "catch.hpp"
#include "qps/query/QueryClause.h"

TEST_CASE("Valid FollowsT arguments", "[FollowsTClause]") {
    // Valid arg1: statement literal, statement synonym, wildcard
    // Valid arg2: statement literal, statement synonym, wildcard

    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto stmtSyn = std::make_shared<StatementSynonym>("s");
    auto subStmtSyn = std::make_shared<WhileSynonym>("ss");
    auto wildcard = std::make_shared<Wildcard>();

    SECTION("stmtLit, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtLit, stmtLit));
    }

    SECTION("stmtLit, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtLit, stmtSyn));
    }

    SECTION("stmtLit, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtLit, subStmtSyn));
    }

    SECTION("stmtLit, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtLit, wildcard));
    }

    SECTION("stmtSyn, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtSyn, stmtLit));
    }

    SECTION("stmtSyn, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtSyn, stmtSyn));
    }

    SECTION("stmtSyn, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtSyn, subStmtSyn));
    }

    SECTION("stmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(stmtSyn, wildcard));
    }

    SECTION("subStmtSyn, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(subStmtSyn, stmtLit));
    }

    SECTION("subStmtSyn, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(subStmtSyn, stmtSyn));
    }

    SECTION("subStmtSyn, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(subStmtSyn, subStmtSyn));
    }

    SECTION("subStmtSyn, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(subStmtSyn, wildcard));
    }

    SECTION("wildcard, stmtLit") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(wildcard, stmtLit));
    }

    SECTION("wildcard, stmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(wildcard, stmtSyn));
    }

    SECTION("wildcard, subStmtSyn") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(wildcard, subStmtSyn));
    }

    SECTION("wildcard, wildcard") {
        REQUIRE_NOTHROW(std::make_shared<FollowsTClause>(wildcard, wildcard));
    }
}

TEST_CASE("Invalid FollowsT arguments", "[FollowsTClause]") {
    // Invalid arg1: entity literal, entity synonym
    // Invalid arg2: entity literal, entity synonym

    auto stmtLit = std::make_shared<StatementLiteral>("1");
    auto entLit = std::make_shared<EntityLiteral>("x");
    auto entSyn = std::make_shared<VariableSynonym>("v");

    SECTION("entLit, valid arg2", "Invalid arg1 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<FollowsTClause>(entLit, stmtLit), QuerySyntaxError);
    }

    SECTION("entSyn, valid arg2", "Invalid arg1 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<FollowsTClause>(entSyn, stmtLit), QuerySemanticError);
    }

    SECTION("valid arg1, entLit", "Invalid arg2 literal type") {
        REQUIRE_THROWS_AS(std::make_shared<FollowsTClause>(stmtLit, entLit), QuerySyntaxError);
    }

    SECTION("valid arg1, entSyn", "Invalid arg2 synonym type") {
        REQUIRE_THROWS_AS(std::make_shared<FollowsTClause>(stmtLit, entSyn), QuerySemanticError);
    }
}
