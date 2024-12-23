//#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"
#include "qps/parser/QueryParser.h"

//TEST_CASE("Splitting clauses with semicolons", "[splitClauses]") {
//    SECTION("Valid input with semicolons") {
//        QueryParser parser("clause1; clause2; clause3;");
//        REQUIRE(parser.splitClauses() == true);
//
//        // Check the extracted tokens
//        const auto& tokens = parser.getClauses();
//        REQUIRE(tokens.size() == 3);
//        REQUIRE(tokens[0] == "clause1");
//        REQUIRE(tokens[1] == "clause2");
//        REQUIRE(tokens[2] == "clause3");
//    }
//
//    SECTION("Valid input without semicolons for last clause") {
//        QueryParser parser("clause1; clause2; clause3");
//        REQUIRE(parser.splitClauses() == true);
//
//        const auto& tokens = parser.getClauses();
//        REQUIRE(tokens.size() == 3);
//        REQUIRE(tokens[0] == "clause1");
//        REQUIRE(tokens[1] == "clause2");
//        REQUIRE(tokens[2] == "clause3");
//    }
//
//    SECTION("Valid input with empty clauses") {
//        QueryParser parser("clause1;;;clause2;;;");
//        REQUIRE(parser.splitClauses() == true);
//
//        const auto& tokens = parser.getClauses();
//        REQUIRE(tokens.size() == 2);
//        REQUIRE(tokens[0] == "clause1");
//        REQUIRE(tokens[1] == "clause2");
//    }
//
//    SECTION("Valid input with spaces and newline characters") {
//        QueryParser parser("  clause1  \n; clause2 \t;");
//        REQUIRE(parser.splitClauses() == true);
//
//        const auto& tokens = parser.getClauses();
//        REQUIRE(tokens.size() == 2);
//        REQUIRE(tokens[0] == "clause1");
//        REQUIRE(tokens[1] == "clause2");
//    }
//}