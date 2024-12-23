#include "SP.h"
#include "sp/astTraversal/ASTTraversal.h"

//SP::SP(std::shared_ptr<StorageWriter> storageWriter) : storageWriter(std::move(storageWriter)) {}
SP::SP(std::shared_ptr<StorageManager> storageManager) : storageManager(storageManager) {}

void SP::initiateProcessing(std::string &input) {
    CompositeLexer lexer;
    std::vector<Token> actualTokens;
    // Isolate Error catching to specific Code Blocks
    try {
        actualTokens = lexer.tokenize(input);
    } catch (const SimpleSyntaxError& error) {
        std::cout << "\n"
                  << error.getMessage() << "\n\n"
                  << "Terminating Program due to Invalid Simple Code." << std::endl;
        // Exits with Error Code
        std::exit(EXIT_FAILURE);
    }
    MainParser parser(actualTokens);
    std::shared_ptr<ASTNode> astRoot;
    // Isolate Error catching to specific Code Blocks
    try {
        astRoot = parser.parse();
    } catch (const SimpleSyntaxError& error) {
        std::cout << "\n"
                  << error.getMessage() << "\n\n"
                  << "Terminating Program due to Parsing Errors." << std::endl;
        // Exits with Error Code
        std::exit(EXIT_FAILURE);
    }
    ASTTraversal traversal(astRoot);
    traversal.traverse();
    SemanticAnalyzer analyzer(traversal.context);
    // Isolate Error catching to specific Code Blocks
    try {
        analyzer.analyze();
    } catch (const SimpleSemanticError& error) {
        std::cout << "\n"
                  << error.getMessage() << "\n\n"
                  << "Terminating Program due to Semantic Errors." << std::endl;
        // Exits with Error Code
        std::exit(EXIT_FAILURE);
    }
    auto parentVisitor = std::make_shared<ParentVisitor>();
    auto followsVisitor = std::make_shared<FollowsVisitor>();
    auto modifiesVisitor = std::make_shared<ModifiesVisitor>();
    auto usesVisitor = std::make_shared<UsesVisitor>();
    auto entitiesVisitor = std::make_shared<EntityVisitor>();
    auto callsVisitor = std::make_shared<CallsVisitor>();
    auto patternVisitor = std::make_shared<PatternVisitor>();

    DesignExtractor designExtractor(followsVisitor, parentVisitor, usesVisitor, modifiesVisitor, entitiesVisitor, callsVisitor, patternVisitor, storageManager);
    designExtractor.traverseAll(astRoot);
    designExtractor.writeToPkb();
    traversal.context.writeToPkb(storageManager);
}
