#include "catch.hpp"

#include "../test_utils/QpsTestUtil.h"
#include "qps/parser/QueryParser.h"
#include "qps/exception/QueryProcessorException.h"

#include <string>

TEST_CASE("Declare clause", "[QueryTokenizer]") {
    SECTION("Single declaration") {
        auto qString = "stmt s;";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"stmt", "s"}}));
    }

    SECTION("Single declaration multiple synonyms") {
        auto qString = "stmt s, s1, s2;";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"stmt", "s", "s1", "s2"}}));
    }

    SECTION("Multiple declarations") {
        auto qString = "stmt s; assign a;";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"stmt",   "s"},
                                                                    {"assign", "a"}}));
    }

    SECTION("Multiple declarations with multiple synonyms") {
        auto qString = "stmt s, s1; assign a, a1;";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"stmt",   "s", "s1"},
                                                                    {"assign", "a", "a1"}}));
    }

    SECTION("Extra whitespaces") {
        auto qString = "   stmt \n  s   ;   assign  \t a   ;";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"stmt",   "s"},
                                                                    {"assign", "a"}}));
    }

    SECTION("Missing semicolon") {
        auto qString = "stmt s Select s such that Follows(1, 2)";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }
}

TEST_CASE("Select clause", "[QueryTokenizer]") {
    SECTION("Single select") {
        auto qString = "Select s";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"Select", "s"}}));
    }

    SECTION("Multiple selects") {
        auto qString = "Select <s, s1>";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"Select", "s", "s1"}}));
    }

    SECTION("Boolean Select") {
        auto qString = "Select BOOLEAN";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"Select", "BOOLEAN"}}));
    }

    SECTION("Extra whitespaces") {
        auto qString = "   Select \n  s   ";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"Select", "s"}}));
    }

    SECTION("Missing closing bracket") {
        auto qString = R"(Select <s, s1 pattern a("x", _"a"_))";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }
}

TEST_CASE("Single such that clause", "[QueryTokenizer]") {
    SECTION("Single relationship") {
        auto qString = "such that Follows(1, 2)";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"such that", "Follows", "1", "2"}}));
    }

    SECTION("Extra whitespaces") {
        auto qString = "   such \n  that   Follows  (  1  ,  2  )   ";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"such that", "Follows", "1", "2"}}));
    }

    SECTION("Missing opening bracket") {
        auto qString = "such that Follows 1, 2)";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }

    SECTION("Missing comma") {
        auto qString = "such that Follows(1 2)";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }

    SECTION("Missing closing bracket") {
        auto qString = "such that Follows(1, 2";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }
}

TEST_CASE("Multiple such that clauses") {
    SECTION("Multiple relationships using and") {
        auto qString = "such that Follows(1, 2) and Parent(3, 4)";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"such that", "Follows", "1", "2"},
                                                                    {"such that", "Parent",  "3", "4"}}));
    }

    SECTION("Multiple relationships using such that") {
        auto qString = "such that Follows(1, 2) such that Parent(3, 4)";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"such that", "Follows", "1", "2"},
                                                                    {"such that", "Parent",  "3", "4"}}));
    }

    SECTION("Missing first closing bracket") {
        auto qString = "such that Follows(1, 2 such that Parent(3, 4)";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }
}

TEST_CASE("Single pattern clause", "[QueryTokenizer]") {
    SECTION("Single pattern 2nd argument partial expression") {
        auto qString = R"(pattern a("x", _"a"_))";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "a", "\"x\"", "_\"a\"_"}}));
    }

    SECTION("Single pattern 2nd argument exact expression") {
        auto qString = R"(pattern a(_, "a"))";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "a", "_", "\"a\""}}));
    }

    SECTION("Extra whitespaces") {
        auto qString = "   pattern \n  a  (  \"     x   \"  ,  _\"  a  \"_  )   ";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "a", "\"     x   \"", "_\"  a  \"_"}}));
    }

    SECTION("Missing closing quote on partial match") {
        auto qString = R"(pattern a("x", _"a_))";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "a", "\"x\"", "_\"a_"}}));
    }

    SECTION("Expression with parentheses") {
        auto qString = R"(pattern a("x", _"a*(b)+c"_))";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "a", "\"x\"", "_\"a*(b)+c\"_"}}));
    }

    SECTION("Missing opening bracket") {
        auto qString = R"(pattern a "x", _"a"_)";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }

    SECTION("Missing comma") {
        auto qString = R"(pattern a("x" _"a"_))";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }

    SECTION("Missing closing bracket") {
        auto qString = R"(pattern a("x", _)";
        QueryTokenizer tokenizer(qString);

        REQUIRE_THROWS_AS(tokenizer.tokenizeQuery(), QuerySyntaxError);
    }
}

TEST_CASE("Multiple pattern clauses", "[QueryTokenizer]") {
    SECTION("Multiple patterns using pattern") {
        auto qString = R"(pattern a(_, _"a"_) pattern b("y", _))";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "a", "_", "_\"a\"_"},
                                                                    {"pattern", "b", "\"y\"", "_"}}));
    }

    SECTION("Multiple patterns using and") {
        auto qString = R"(pattern a(_, _"a"_) and b("y", _))";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "a", "_", "_\"a\"_"},
                                                                    {"pattern", "b", "\"y\"", "_"}}));
    }
}

TEST_CASE("Single with clause", "[QueryTokenizer]") {
    SECTION("Single with clause") {
        auto qString = "with s.stmt# = 1";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"with", "s.stmt#", "1"}}));
    }

    // TODO: Add more test cases for with clause
}

TEST_CASE("Multiple with clauses", " [QueryTokenizer]") {
    SECTION("Multiple with clauses") {
        auto qString = "with s.stmt# = 1 and a.varName = \"x\"";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"with", "s.stmt#", "1"},
                                                                    {"with", "a.varName", "\"x\""}}));
    }

    // TODO: Add more test cases for multiple with clauses
}

TEST_CASE("Negated clauses", "[QueryTokenizer]") {
    auto qString = R"(stmt s, s1; assign a, a1; Select <s, a> such that Follows(s, s1) and Modifies(a, "x") pattern a(_, _"x"_) and a1("x", " a*(b+c) - 1") such that Parent*(s, s1))";

    QueryTokenizer tokenizer(qString);
    auto tokenized = tokenizer.tokenizeQuery();

    // print tokenized query
    for (const auto &token: *tokenized) {
        for (const auto &t: *token) {
            std::cout << t << " ";
        }
        std::cout << std::endl;
    }
}

TEST_CASE("not tokens", "[QueryTokenizer]") {
    SECTION("such that not") {
        auto qString = "such that not Follows(1, 2)";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"such that", "not", "Follows", "1", "2"}}));
    }

    SECTION("pattern not") {
        auto qString = R"(pattern not a("x", _"a"_))";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"pattern", "not", "a", "\"x\"", "_\"a\"_"}}));
    }

    SECTION("with not") {
        auto qString = "with not s.stmt# = 1";
        QueryTokenizer tokenizer(qString);

        REQUIRE(compareTokenizedClauses(tokenizer.tokenizeQuery(), {{"with", "not", "s.stmt#", "1"}}));
    }
}
