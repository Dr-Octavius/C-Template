#include "ParserFactory.h"

// Define the static factoryRegistry
std::unordered_map<RegistryKey, std::function<std::shared_ptr<IParser>()>> ParserFactory::factoryRegistry;

// Define the parserMap
// Define the mapping from strings to parser creators
const std::unordered_map<std::string, std::pair<RegistryKey, ParserFactory::ParserCreator>> ParserFactory::parserMap = {
        {"read", {RegistryKey::READ, [](ParserContext& ctx) { return ParserFactory::createParser<ReadParser>(ctx)->parse(ctx); }}},
        {"print", {RegistryKey::PRINT, [](ParserContext& ctx) { return ParserFactory::createParser<PrintParser>(ctx)->parse(ctx); }}},
        {"call", {RegistryKey::CALL, [](ParserContext& ctx) { return ParserFactory::createParser<CallParser>(ctx)->parse(ctx); }}},
        {"while", {RegistryKey::WHILE, [](ParserContext& ctx) { return ParserFactory::createParser<WhileParser>(ctx)->parse(ctx); }}},
        {"if", {RegistryKey::IF, [](ParserContext& ctx) { return ParserFactory::createParser<IfParser>(ctx)->parse(ctx); }}},
        // other mappings...
};

std::shared_ptr<ASTNode> ParserFactory::stmtListWorker(ParserContext& ctx) {
    const std::string& val = ctx.getCurrentToken().value;
    auto it = parserMap.find(val);
    if (it != parserMap.end()) {
        ctx.giveKey(it->second.first); // Set the registry key based on the map
        return it->second.second(ctx); // Invoke the parser creator and parse the statement
    } else {
        throw SimpleSyntaxError("Unknown statement type");
    }
}

void ParserFactory::registerParser(const RegistryKey keyword, const std::function<std::shared_ptr<IParser>()>& createFn) {
    factoryRegistry[keyword] = createFn;
}

void ParserFactory::registerParsers() {
    // Here you would register the specific parsers for each TokenType
    // For example:
    // Not implemented yet
    ParserFactory::registerParser(RegistryKey::PROGRAM, []() { return std::make_shared<ProgramParser>(); });
    ParserFactory::registerParser(RegistryKey::PROCEDURE, []() { return std::make_shared<ProcedureParser>(); });
    ParserFactory::registerParser(RegistryKey::CALL, []() { return std::make_shared<CallParser>(); });
    ParserFactory::registerParser(RegistryKey::READ, []() {return std::make_shared<ReadParser>(); });
    ParserFactory::registerParser(RegistryKey::PRINT, []() { return std::make_shared<PrintParser>(); });
    ParserFactory::registerParser(RegistryKey::WHILE, []() { return std::make_shared<WhileParser>(); });
    ParserFactory::registerParser(RegistryKey::IF, []() { return std::make_shared<IfParser>(); });
    ParserFactory::registerParser(RegistryKey::THEN, []() { return std::make_shared<ThenParser>(); });
    ParserFactory::registerParser(RegistryKey::ELSE, []() { return std::make_shared<ElseParser>(); });
    ParserFactory::registerParser(RegistryKey::STMT_LIST, []() { return std::make_shared<StmtListParser>(); });
    ParserFactory::registerParser(RegistryKey::EXPR, []() { return std::make_shared<ExprParser>(); });
    ParserFactory::registerParser(RegistryKey::COND_EXPR, []() { return std::make_shared<CondExprParser>(); });
    ParserFactory::registerParser(RegistryKey::REL_EXPR, []() { return std::make_shared<RelExprParser>(); });
    ParserFactory::registerParser(RegistryKey::CONSTANT, []() { return std::make_shared<ConstantParser>(); });
    ParserFactory::registerParser(RegistryKey::IDENTIFIER, []() { return std::make_shared<IdentifierParser>(); });
    ParserFactory::registerParser(RegistryKey::ASSIGNMENT, []() { return std::make_shared<AssignmentParser>(); });
    // ... other parser registrations ...
}