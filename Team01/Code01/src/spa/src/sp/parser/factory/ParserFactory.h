#ifndef INC_23S2_CP_SPA_TEAM_01_PARSERFACTORY_H
#define INC_23S2_CP_SPA_TEAM_01_PARSERFACTORY_H

#include "sp/errors/SimpleSyntaxError.h"
#include "sp/parser/IParser.h"
#include "sp/parser/factory/ParserContext.h"
#include "sp/parser/controlFlow/StmtListParser.h"
#include "sp/parser/controlFlow/CallParser.h"
#include "sp/parser/controlFlow/ProgramParser.h"
#include "sp/parser/expressions/ExprParser.h"
#include "sp/parser/expressions/CondExprParser.h"
#include "sp/parser/expressions/RelExprParser.h"
#include "sp/parser/statements/AssignmentParser.h"
#include "sp/parser/statements/ifParser/ElseParser.h"
#include "sp/parser/statements/ifParser/IfParser.h"
#include "sp/parser/statements/ReadParser.h"
#include "sp/parser/statements/PrintParser.h"
#include "sp/parser/statements/ProcedureParser.h"
#include "sp/parser/statements/ifParser/ThenParser.h"
#include "sp/parser/statements/WhileParser.h"
#include "sp/parser/expressions/variables/ConstantParser.h"
#include "sp/parser/expressions/variables/IdentifierParser.h"
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

class ParserFactory {
private:
    // Define a function type for creating parsers
    using ParserCreator = std::function<std::shared_ptr<ASTNode>(ParserContext&)>;
    static std::unordered_map<RegistryKey, std::function<std::shared_ptr<IParser>()>> factoryRegistry;
    static void registerParser(RegistryKey, const std::function<std::shared_ptr<IParser>()>& createFn);
public:
    // Declaration of the map that associates strings with functions
    static const std::unordered_map<std::string, std::pair<RegistryKey, ParserCreator>> parserMap;
    static void registerParsers();
    static std::shared_ptr<ASTNode> stmtListWorker(ParserContext& ctx);
    template<class P>
    static std::shared_ptr<P> createParser(ParserContext& ctx) {
        // Ensure P is derived from IParser at compile time
        static_assert(std::is_base_of<IParser, P>::value, "Declared Parser return type P must be derived from IParser");

        // Dynamically selects and creates a type P parser based on the provided context
        RegistryKey key = ctx.getCurrentKey();

        // Retrieve the parser creator function based on the key
        auto it = factoryRegistry.find(key);
        if (it != factoryRegistry.end()) {
            auto parser = it->second();

            // May need to rethink how to do the factory getting method if using IParser
            // Step 2: Attempt cast to the desired derived type
            std::shared_ptr<P> castedParser = std::dynamic_pointer_cast<P>(parser);

            // If the cast fails, the found parser is not of the expected type P
            if (!castedParser) {
                throw SimpleSyntaxError("Parser found, but dynamic cast to the requested type failed.");
            }

            // Return the correctly typed parser
            return castedParser;
        }

        // If no parser is found for the key, throw an error
        // Since statically we know that P is already implemented as a subclass of IParser,
        // we are confident that the issue is due to the Parser not being implemented
        throw SimpleSyntaxError("Parser for the given key not registered.");
    }
};
#endif //INC_23S2_CP_SPA_TEAM_01_PARSERFACTORY_H
