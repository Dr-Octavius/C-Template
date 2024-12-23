#include "catch.hpp"

#include "qps/query/DesignEntity.h"

TEST_CASE("Valid expression syntax", "[Expression]") {
    SECTION("Test syntax check for different entity types") {
        auto expr = "x + y * z - 9 / (2%val1)";

        REQUIRE_NOTHROW(ExactExpression(expr));
    }
}

TEST_CASE("Invalid expression syntax", "[Expression]") {
    SECTION("Missing closing bracket") {
        auto expr = "x + y * z - 9 / (2%val1";

        REQUIRE_THROWS_AS(ExactExpression(expr), QuerySyntaxError);
    }

    SECTION("Consecutive operators") {
        auto expr = "x +- y * z - 9 / (2%val1)";

        REQUIRE_THROWS_AS(ExactExpression(expr), QuerySyntaxError);
    }

    SECTION("Missing operand") {
        auto expr = "x + y * z - 9 / (2%val1) +";

        REQUIRE_THROWS_AS(ExactExpression(expr), QuerySyntaxError);
    }

    SECTION("Missing operator") {
        auto expr = "x + y * z - 9 / (2%val1) 2";

        REQUIRE_THROWS_AS(ExactExpression(expr), QuerySyntaxError);
    }
}
