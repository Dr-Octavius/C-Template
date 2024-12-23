#include "catch.hpp"

#include "qps/MainQueryProcessor.h"

TEST_CASE("SyntaxError", "[MainQueryProcessor]") {
    SECTION("Empty query") {
        auto qString = "";

//        auto res = MainQueryProcessor().processQuery(qString);
//
//        REQUIRE(res.size() == 1);
//
//        for (const auto& element : res) {
//            REQUIRE(element == "SyntaxError");
//        }
    }
}

