#include "MainQueryProcessor.h"
#include "qps/parser/QueryParser.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include "qps/pkb_integration/StorageReader.h"
#include "exception/QueryProcessorException.h"

#include <iostream>
#include <vector>
#include <string>

// Facade class - Takes query string, calls parser and evaluator, returns query result
std::vector<std::string>
MainQueryProcessor::processQuery(const std::string &queryString, std::shared_ptr<ReadStorage> readStorage) {
    std::shared_ptr<Query> query;
    try {
        QueryParser parser(queryString);
        query = parser.parseQuery();
        // Create ReadStorage object from PKB
        auto getReadStorage = std::make_shared<StorageReader>(readStorage);
        return query->evaluate(getReadStorage);
    } catch (const QuerySyntaxError& e) {
        return std::vector<std::string>{"SyntaxError"};
    } catch (const QuerySemanticError& e) {
        return std::vector<std::string>{"SemanticError"};
    } catch (const QueryUnknownError& e) {
        return std::vector<std::string>{e.what()};
    }
}
