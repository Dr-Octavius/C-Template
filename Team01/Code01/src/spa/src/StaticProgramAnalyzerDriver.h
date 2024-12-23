#ifndef STATIC_PROGRAM_ANALYZER_DRIVER_H
#define STATIC_PROGRAM_ANALYZER_DRIVER_H

#include <string>
#include <list>
#include <memory>
#include "sp/pkb_integration/StorageWriter.h"
#include "program_knowledge_base/storage/StorageManager.h"
#include "qps/MainQueryProcessor.h"
#include "qps/pkb_integration/StorageReader.h"
#include "sp/SP.h"

class StaticProgramAnalyzerDriver {
    

private:
    std::shared_ptr<StorageManager> storageManager = std::make_shared<StorageManager>();

public: 
    // method for parsing the SIMPLE source
    virtual void parse(std::string filename);

    // method for evaluating a query
    virtual void evaluate(std::string query, std::list<std::string> results);
};

#endif //STATIC_PROGRAM_ANALYZER_DRIVER_H