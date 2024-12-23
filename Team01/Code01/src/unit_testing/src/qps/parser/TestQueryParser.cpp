#include "catch.hpp"

#include "qps/parser/QueryParser.h"
#include "qps/exception/QueryProcessorException.h"
#include "../test_utils/QpsTestUtil.h"

TEST_CASE("Valid queries", "[QueryParser]") {
    SECTION("Single declaration single selection") {
        auto qString = "stmt s; Select s";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 1, 1));
    }

    SECTION("Multiple declarations") {
        auto qString = "stmt s; assign a; Select s";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 2, 1));
    }

    SECTION("With clause") {
        auto qString = "stmt s; Select s with s.stmt# = 1";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 1, 1, 0, 0, 1));
    }

    SECTION("Multiple declarations, multiple selections, multiple relationships, multiple pattern") {
        auto qString = R"(stmt s, s1; assign a, a1; Select <s, a> such that Follows(s, s1) and Modifies(a, "x") pattern a(_, _"x"_) and a1("x", " a*(b+c) - 1") such that Parent*(s, s1))";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 4, 2, 3, 2, 0));
    }

    SECTION("Multiple negated clauses using and") {
        auto qString = "stmt s; Select s such that not Follows(2, s) and not Parent(2,s)";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 1, 1, 2, 0, 0));
    }
}

TEST_CASE("Valid negated queries", "[QueryParser]") {
    SECTION("Negated such that") {
        auto qString = "stmt s; Select s such that not Follows(2, s)";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 1, 1, 1, 0, 0));
    }

    SECTION("Negated pattern") {
        auto qString = "assign a; Select a pattern not a(_, _)";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 1, 1, 0, 1, 0));
    }

    SECTION("Negated with") {
        auto qString = "stmt s; Select s with not s.stmt# = 1";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_NOTHROW(parser->parseQuery());
        REQUIRE(requireParsedClauseCount(parser, 1, 1, 0, 0, 1));
    }
}

TEST_CASE("Invalid queries", "[QueryParser]") {
    SECTION("Empty query") {
        auto qString = "";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }

    SECTION("Declaration after Select") {
        auto qString = "Select s; stmt s";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }

    SECTION("such that before Select") {
        auto qString = "stmt s; such that Follows(s, 1) Select s";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }

    SECTION("pattern before Select") {
        auto qString = "assign s; pattern s(_, _) Select s";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }

    SECTION("Multiple Selects") {
        auto qString = "stmt s; Select s Select s";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }

    SECTION("Invalid relationship") {
        auto qString = "stmt s; Select s such that Modify(s, 1)";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }

    SECTION("Undeclared synonym") {
        auto qString = "stmt s; Select a";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySemanticError);
    }

    SECTION("Multiple declared synonym") {
        auto qString = "stmt s; assign s; Select s";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySemanticError);
    }

    SECTION("Invalid entity literal") {
        auto qString = "stmt s; Select s such that Modifies(\"a b\", _)";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }

    SECTION("Invalid expression") {
        auto qString = "assign a; Select a pattern a(_, _\"a b\"_)";
        auto parser = std::make_shared<QueryParser>(qString);

        REQUIRE_THROWS_AS(parser->parseQuery(), QuerySyntaxError);
    }
}
