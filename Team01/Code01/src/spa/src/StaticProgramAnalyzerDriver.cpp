//#include "StaticProgramAnalyzerDriver.h"
//#include <utility>
//#include <iostream>
//#include <sstream>
//#include <fstream>
//
//void StaticProgramAnalyzerDriver::parse(const std::string filename) {
//    std::shared_ptr<StorageWriter> store = std::make_shared<StorageWriter>(storageManager->getWriteStorage(),
//        storageManager->getReadStorage());
//
//    std::ifstream file(filename);
//    if (!file.is_open()) {
//        std::cerr << "Error opening file: " << filename << std::endl;
//        return;
//    }
//
//    std::stringstream buffer;
//    buffer << file.rdbuf();
//    std::string contents = buffer.str();
//
//    file.close();
//
//    SP sp(store);
//    sp.initiateProcessing(contents);
//}
//
//void StaticProgramAnalyzerDriver::evaluate(std::string query, std::list<std::string> results) {
//    // call your evaluator to evaluate the query here
//    // ...code to evaluate query...
//    // MainQueryProcessor qp;
//     // Get readStorage object
//    auto rs = storageManager->getReadStorage();
//    auto getReadStorage = std::make_shared<StorageReader>(rs);
//    auto map = getReadStorage->getParentMap();
//
//    for (auto& x : map) {
//        auto child = x.first;
//        auto parents = x.second;
//    }
//
//    auto queryResult = MainQueryProcessor::processQuery(query, storageManager->getReadStorage());
//    // store the answers to the query in the results list (it is initially empty)
//    // each result must be a string.
//    results.insert(results.end(), queryResult.begin(), queryResult.end());
//}
