#ifndef MAINQUERYPROCESSOR_H
#define MAINQUERYPROCESSOR_H

#include <string>
#include <unordered_set>
#include "program_knowledge_base/storage/StorageManager.h"
//#include "qps/pkb_integration/StorageReader.h"

class MainQueryProcessor {
public:
    MainQueryProcessor() {};

    // Facade method called by UI
    static std::vector<std::string> processQuery(const std::string& queryString, std::shared_ptr<ReadStorage> readStorage);
};

#endif