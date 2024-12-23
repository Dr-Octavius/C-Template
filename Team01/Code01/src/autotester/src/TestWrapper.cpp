#include "TestWrapper.h"
//#include "StaticProgramAnalyzerDriver.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include "qps/MainQueryProcessor.h"
#include "sp/SP.h"
#include <iostream>
#include <sstream>
#include <fstream>


//// implementation code of WrapperFactory - do NOT modify the next 5 lines
//AbstractWrapper* WrapperFactory::wrapper = 0;
//AbstractWrapper* WrapperFactory::createWrapper() {
//  if (wrapper == 0) wrapper = new TestWrapper;
//  return wrapper;
//}
//// Do not modify the following line
//volatile bool AbstractWrapper::GlobalStop = false;
//
//// a default constructor
//TestWrapper::TestWrapper() {
//    // create any objects here as instance variables of this class
//    // as well as any initialization required for your spa program
//    spa_driver = std::make_shared<StaticProgramAnalyzerDriver>();
//}
//
//// method for parsing the SIMPLE source
//void TestWrapper::parse(std::string filename) {
//  // call your parser to do the parsing
//  spa_driver->parse(filename);
//}
//
//// method to evaluating a query
//void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
//// call your evaluator to evaluate the query here
//  // ...code to evaluate query...
//  // MainQueryProcessor qp;
//  // Get readStorage object
//  spa_driver->evaluate(query, results);
//}


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program
    storageManager = std::make_shared<StorageManager>();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    // call your parser to do the parsing
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string contents = buffer.str();

    file.close();

    SP sp (storageManager);
    sp.initiateProcessing(contents);

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
    // ...code to evaluate query...
    // MainQueryProcessor qp;
    // Get readStorage object
    auto rs = storageManager->getReadStorage();

    auto queryResult = MainQueryProcessor::processQuery(query, rs);

    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    results.insert(results.end(), queryResult.begin(), queryResult.end());
}