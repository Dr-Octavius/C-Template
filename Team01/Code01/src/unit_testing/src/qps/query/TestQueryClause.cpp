////#define CATCH_CONFIG_MAIN
//#include "../../../../../lib/catch.hpp"
//
//#include "qps/query/Query.h"
//
//// Test DeclareClause
//TEST_CASE("DeclareClause", "[DeclareClause]") {
//    SECTION("Test syntax check for different entity types") {
//        DeclareClause declareStmt("stmt", "s");
//        REQUIRE(declareStmt.getSynonym()->getValue() == "s");
//    }
//    SECTION("Test wrong entity type") {
//        REQUIRE_THROWS([] {
//            DeclareClause declareStmt("wrong", "s");
//        }());
//    }
//    SECTION("Test wrong synonym syntax") {
//        REQUIRE_THROWS([] {
//            DeclareClause declareStmt("read", "1");
//        }());
//        REQUIRE_THROWS([] {
//            DeclareClause declareStmt("read", "_");
//        }());
//        REQUIRE_THROWS([] {
//            DeclareClause declareStmt("read", "4hdg");
//        }());
//    }
//}
//
//// Test SelectClause
//TEST_CASE("SelectClause", "[SelectClause]") {
//    SECTION("Test syntax check for select synonym") {
//        SelectClause selectSyn("s");
//        REQUIRE(selectSyn.getSynonym()->getValue() == "s");
//    }
//    SECTION("Test wrong synonym syntax") {
//        REQUIRE_THROWS([] {
//            SelectClause selectSyn("00");
//        }());
//        REQUIRE_THROWS([] {
//            SelectClause selectSyn("8098djkb");
//        }());
//        REQUIRE_THROWS([] {
//            SelectClause selectSyn("__@");
//        }());
//    }
//}
//
//// Test ParentClause Constructor
//TEST_CASE("ParentClause", "[ParentClause]") {
//    SECTION("2 Literals") {
//        ParentClause parentClause("1", "2");
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS2()));
//        REQUIRE(parentClause.getS1()->getValue() == "1");
//        REQUIRE(parentClause.getS2()->getValue() == "2");
//    }
//
//    SECTION("1 Literal 1 Synonym") {
//        ParentClause parentClause("1", "s");
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS2()));
//        REQUIRE(parentClause.getS1()->getValue() == "1");
//        REQUIRE(parentClause.getS2()->getValue() == "s");
//    }
//
//    SECTION("2 Synonyms") {
//        ParentClause parentClause("s1", "s2");
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS2()));
//        REQUIRE(parentClause.getS1()->getValue() == "s1");
//        REQUIRE(parentClause.getS2()->getValue() == "s2");
//    }
//
//    SECTION("Invalid Argument") {
//        REQUIRE_THROWS([] {
//            ParentClause parentClause("00", "s");
//        }());
//    }
//}
//
//// Test CallsClause Constructor
//TEST_CASE("CallsClause", "[CallsClause]") {
//    SECTION("2 Literals") {
//        REQUIRE_THROWS([] {
//            CallsClause callsClause("1", "2");
//        }());
//    }
//
//    SECTION("1 Literal 1 Synonym") {
//        REQUIRE_THROWS([] {
//            CallsClause callsClause("1", "s");
//        }());
//    }
//
//    SECTION("1 Synonym 1 Literal") {
//        REQUIRE_THROWS([] {
//            CallsClause callsClause("s", "1");
//        }());
//    }
//
//    SECTION("2 Names") {
//        CallsClause callsClause("s1", "s2");
//        REQUIRE(std::dynamic_pointer_cast<ProcedureSynonym>(callsClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<ProcedureSynonym>(callsClause.getS2()));
//        REQUIRE(callsClause.getS1()->getValue() == "s1");
//        REQUIRE(callsClause.getS2()->getValue() == "s2");
//    }
//
//    SECTION("Invalid Argument") {
//        REQUIRE_THROWS([] {
//            CallsClause callsClause("00", "s");
//        }());
//    }
//}
//
//// Test CallsT Constructor
//TEST_CASE("CallsTClause", "[CallsTClause]") {
//    SECTION("2 Literals") {
//        REQUIRE_THROWS([] {
//            CallsTClause callsTClause("1", "2");
//        }());
//    }
//
//    SECTION("1 Literal 1 Synonym") {
//        REQUIRE_THROWS([] {
//            CallsTClause callsTClause("1", "s");
//        }());
//    }
//
//    SECTION("1 Synonym 1 Literal") {
//        REQUIRE_THROWS([] {
//            CallsTClause callsTClause("s", "1");
//        }());
//    }
//
//    SECTION("2 Names") {
//        CallsTClause callsTClause("s1", "s2");
//        REQUIRE(std::dynamic_pointer_cast<ProcedureSynonym>(callsTClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<ProcedureSynonym>(callsTClause.getS2()));
//        REQUIRE(callsTClause.getS1()->getValue() == "s1");
//        REQUIRE(callsTClause.getS2()->getValue() == "s2");
//    }
//
//    SECTION("Invalid Argument") {
//        REQUIRE_THROWS([] {
//            CallsTClause callsTClause("00", "s");
//        }());
//    }
//}
//
//TEST_CASE("ParentClause::checkSynonyms", "[ParentClause]") {
//    SECTION("2 Literals, 0 Synonyms match") {
//        ParentClause parentClause("1", "2");
//        std::unordered_set<std::shared_ptr<Synonym>> declaredSynonyms{
//            std::make_shared<ReadSynonym>("r"),
//            std::make_shared<IfSynonym>("i"),
//            std::make_shared<VariableSynonym>("v")
//        };
//        REQUIRE_NOTHROW([&]() {
//            parentClause.checkSynonyms(declaredSynonyms);
//        }());
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS2()));
//        REQUIRE(parentClause.getS1()->getValue() == "1");
//        REQUIRE(parentClause.getS2()->getValue() == "2");
//    }
//
//    SECTION("1 Synonym, 1 Synonyms matches") {
//        ParentClause parentClause("r", "2");
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS2()));
//
//        std::unordered_set<std::shared_ptr<Synonym>> declaredSynonyms{
//            std::make_shared<ReadSynonym>("r"),
//            std::make_shared<IfSynonym>("i"),
//            std::make_shared<VariableSynonym>("v")
//        };
//        REQUIRE_NOTHROW([&]() {
//            parentClause.checkSynonyms(declaredSynonyms);
//        }());
//        REQUIRE(std::dynamic_pointer_cast<ReadSynonym>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS2()));
//        REQUIRE(parentClause.getS1()->getValue() == "r");
//        REQUIRE(parentClause.getS2()->getValue() == "2");
//    }
//
//    SECTION("1 Synonym, 0 Synonyms matches (Not declared)") {
//        ParentClause parentClause("s", "2");
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS2()));
//
//        std::unordered_set<std::shared_ptr<Synonym>> declaredSynonyms{
//            std::make_shared<ReadSynonym>("r"),
//            std::make_shared<IfSynonym>("i"),
//            std::make_shared<VariableSynonym>("v")
//        };
//        REQUIRE_THROWS([&]() {
//            parentClause.checkSynonyms(declaredSynonyms);
//        }());
//    }
//
//    SECTION("1 Synonym, 0 Synonyms matches (Not declared, DE type mismatch)") {
//        ParentClause parentClause("v", "2");
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementLiteral>(parentClause.getS2()));
//
//        std::unordered_set<std::shared_ptr<Synonym>> declaredSynonyms{
//            std::make_shared<ReadSynonym>("r"),
//            std::make_shared<IfSynonym>("i"),
//            std::make_shared<VariableSynonym>("v")
//        };
//        REQUIRE_THROWS([&]() {
//            parentClause.checkSynonyms(declaredSynonyms);
//        }());
//    }
//
//    SECTION("1 Synonym, 1 match 1 mismatch") {
//        ParentClause parentClause("s", "c");
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS1()));
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym>(parentClause.getS2()));
//        REQUIRE(parentClause.getS1()->getValue() == "s");
//        REQUIRE(parentClause.getS2()->getValue() == "c");
//
//        std::unordered_set<std::shared_ptr<Synonym>> declaredSynonyms{
//            std::make_shared<ReadSynonym>("r"),
//            std::make_shared<IfSynonym>("i"),
//            std::make_shared<StatementSynonym>("s"),
//            std::make_shared<ConstantSynonym>("c")
//        };
//        REQUIRE_THROWS([&]() {
//            parentClause.checkSynonyms(declaredSynonyms);
//        }());
//    }
//}
//
//// Add more test cases for other classes and functionalities
//
