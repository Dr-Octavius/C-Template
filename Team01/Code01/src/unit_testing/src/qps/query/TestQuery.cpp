////#define CATCH_CONFIG_MAIN
//#include "../../../../../lib/catch.hpp"
//
//#include "qps/query/Query.h"
//
//TEST_CASE("Query", "[Query]") {
//    std::unordered_set<std::shared_ptr<DeclareClause>> declareClauses;
//    std::shared_ptr<SelectClause> selectClause;
//    std::unordered_set<std::shared_ptr<RelationshipClause>> relationshipClauses;
//    std::unordered_set<std::shared_ptr<PatternClause>> patternClauses;
//
//    SECTION("Test extractDeclaredSynonyms") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("read", "r"));
//        declareClauses.insert(std::make_shared<DeclareClause>("variable", "v"));
//        declareClauses.insert(std::make_shared<DeclareClause>("assign", "a"));
//
//        selectClause = std::make_shared<SelectClause> ("s");
//
//        relationshipClauses.insert(std::make_shared<ParentClause> ("1", "2"));
//
//        patternClauses.insert(std::make_shared<PatternClause> ("a", "v", "_\"x\"_"));
//
//
//        Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        REQUIRE(query.getDeclareClauses().size() == 4);
//        REQUIRE(query.getSelectClause()); // Select clause is not set in this case
//        REQUIRE(query.getRelationshipClauses().size() == 3); // Relationship clauses are not set in this case
//        REQUIRE(query.getPatternClauses().size() == 1);
//
//        REQUIRE(query.getSynonyms().size() == 4);
//    }
//
//    SECTION("Error: Duplicate declarations") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("variable", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//
//        selectClause = std::make_shared<SelectClause> ("s");
//
//        relationshipClauses.insert(std::make_shared<ParentClause> ("1", "2"));
//
//        REQUIRE_THROWS([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//    }
//
//    SECTION("Error: Undeclared select synonym") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("variable", "v"));
//
//        selectClause = std::make_shared<SelectClause> ("undeclared");
//
//        relationshipClauses.insert(std::make_shared<ParentClause> ("1", "2"));
//
//        REQUIRE_THROWS([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//    }
//
//    SECTION("No relationship clause") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("variable", "v"));
//
//        selectClause = std::make_shared<SelectClause> ("v");
//
//        REQUIRE_NOTHROW([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//        Query q(declareClauses, selectClause, relationshipClauses, patternClauses);
//        REQUIRE(std::dynamic_pointer_cast<VariableSynonym> (q.getSelectSynonym()));
//    }
//
//    SECTION("No declare clause") {
//        selectClause = std::make_shared<SelectClause> ("v");
//
//        REQUIRE_THROWS([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//    }
//
//    SECTION("Error: Wrong entity type in Parent Relationship") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("variable", "v"));
//
//        selectClause = std::make_shared<SelectClause> ("s");
//
//        relationshipClauses.insert(std::make_shared<ParentClause> ("s", "v"));
//
//        REQUIRE_THROWS([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//    }
//
//    SECTION("Pattern Clause with RHS variable synonym, LHS variable literal partial match") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("variable", "v"));
//        declareClauses.insert(std::make_shared<DeclareClause>("assign", "a"));
//
//        selectClause = std::make_shared<SelectClause> ("a");
//
//        relationshipClauses.insert(std::make_shared<ParentClause> ("s", "a"));
//
//        patternClauses.insert(std::make_shared<PatternClause>("a", "v", "_\"x\"_"));
//
//        REQUIRE_NOTHROW([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//        Query q(declareClauses, selectClause, relationshipClauses, patternClauses);
//        REQUIRE(std::dynamic_pointer_cast<AssignSynonym> (q.getSelectSynonym()));
//    }
//
//    SECTION("Pattern Clause with no pattern clause") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//
//        selectClause = std::make_shared<SelectClause> ("s");
//
//        Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym> (query.getSelectSynonym()));
//
//    }
//
//    SECTION("Pattern Clause with constant literal rhs") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("variable", "v"));
//        declareClauses.insert(std::make_shared<DeclareClause>("assign", "a"));
//
//        selectClause = std::make_shared<SelectClause> ("s");
//
//        patternClauses.insert(std::make_shared<PatternClause>("a", "v", "_\"10\"_"));
//
//        Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        REQUIRE(std::dynamic_pointer_cast<StatementSynonym> (query.getSelectSynonym()));
//
//    }
//
//    SECTION("Invalid: Pattern Clause with constant synonym lhs") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("constant", "c"));
//        declareClauses.insert(std::make_shared<DeclareClause>("assign", "a"));
//
//        selectClause = std::make_shared<SelectClause> ("s");
//
//        patternClauses.insert(std::make_shared<PatternClause>("a", "c", "_\"10\"_"));
//
//        REQUIRE_THROWS([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//
//    }
//
//    SECTION("Pattern Clause with wildcard lhs") {
//        declareClauses.insert(std::make_shared<DeclareClause>("stmt", "s"));
//        declareClauses.insert(std::make_shared<DeclareClause>("constant", "c"));
//        declareClauses.insert(std::make_shared<DeclareClause>("assign", "a"));
//
//        selectClause = std::make_shared<SelectClause> ("s");
//
//        patternClauses.insert(std::make_shared<PatternClause>("a", "_", "_\"abd2\"_"));
//
//        REQUIRE_NOTHROW([&]() {
//            Query query(declareClauses, selectClause, relationshipClauses, patternClauses);
//        }());
//
//    }
//
//
//
//    // Add more test cases for setRelationshipSynonyms, evaluateQuery, and other functionalities
//}